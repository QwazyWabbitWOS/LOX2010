
/***************************************************/
/**********  Guided Nuclear Missiles      **********/
/***************************************************/

#include "g_local.h"
#include "l_rockets.h"
#include "x_fire.h"
#include "l_nuke.h"
#include "g_weapon.h"
#include "l_positron.h"
#include "l_angels.h"

void RemoveNuke(edict_t* ent)
{
	ent->client->guidedMissileFired = 0;
	if (!ent->client->missile)
		return;

	ent->client->missile = NULL;

	if (!ent->client->chasetoggle && !ent->client->onturret)
	{
		ent->client->ps.pmove.pm_flags &= ~PMF_NO_PREDICTION;
		ent->client->ps.gunindex = gi.modelindex("models/weapons/v_rocket/tris.md2");
		ent->s.modelindex = ent->client->oldplayer->s.modelindex;
		G_FreeEdict(ent->client->oldplayer);
	}
}

// wrapper for RemoveNuke
/* When nukes can take damage they need a die function. */
void nuke_die(edict_t* self, edict_t* inflictor, edict_t* attacker, int damage, vec3_t point)
{
	RemoveNuke(self);
}

//Explode rocket without touching anything
void GuidedNuke_Explode(edict_t* ent)
{
	if (ent->owner != NULL && ent->owner->client)
	{
		PlayerNoise(ent->owner, ent->s.origin, PNOISE_IMPACT);
		RemoveNuke(ent->owner);
	}

	SpawnExplosion(ent->s.origin, ent->owner, 100, world, 140, MOD_GUIDEDNUKE);

	gi.WriteByte(svc_temp_entity);
	if (ent->waterlevel)
		gi.WriteByte(TE_ROCKET_EXPLOSION_WATER);
	else
		gi.WriteByte(TE_ROCKET_EXPLOSION);

	gi.WritePosition(ent->s.origin);
	gi.multicast(ent->s.origin, MULTICAST_PVS);

	G_FreeEdict(ent);
}

// When a rocket 'dies', it blows up next frame
void GuidedNuke_Think(edict_t* self)
{
	VectorClear(self->velocity);
	self->nextthink = level.time + FRAMETIME;
	self->think = GuidedNuke_Explode;
}

void guidednuke_touch(edict_t* ent, edict_t* other, cplane_t* plane, csurface_t* surf)
{
	vec3_t  end, aimdir = { 0 };
	vec3_t  cloud = { 10, 100, 100 };
	vec3_t  timer = { 0.2f, 0.4f, 1 };
	vec3_t  fire_damage = { 6, 4, 25 };
	vec3_t  radii = { 120, 500, 2000 };
	int radius_damage = 500;
	int damage;

	damage = 1000;

	if (other == ent->owner)
		return;

	VectorCopy(ent->velocity, aimdir);
	VectorNormalize(aimdir);

	if (ent->owner->client)
		PlayerNoise(ent->owner, ent->s.origin, PNOISE_IMPACT);

	if (other->takedamage)
	{
		if (other->client)
		{       /* Blind target. */
			other->client->flash_alpha = 1;
			other->client->flash_fade = 0.05f;
		}
		PBM_Ignite2(other, ent->owner, ent->s.origin);
		T_Damage(other, ent->owner, ent->owner,
			aimdir, ent->s.origin, ent->velocity,
			damage, damage, DAMAGE_ENERGY, MOD_NUKE);
	}

	VectorMA(ent->s.origin, -4, aimdir, end);
	PBM_FlashExplosion(ent->owner, ent->s.origin, end, (float)radius_damage, radii, NULL, MOD_NUKE);
	PBM_FlameCloud2(ent->owner, ent->s.origin, cloud, timer, true, fire_damage, fire_damage, 0, 100);
}

void guidedNuke_touch(edict_t* ent, edict_t* other, cplane_t* plane, csurface_t* surf)
{
	vec3_t	aimdir = { 0 };

	guidednuke_touch(ent, other, plane, surf);

	if (other == ent->owner)
		return;
	VectorCopy(ent->velocity, aimdir);
	VectorNormalize(aimdir);

	if (surf && (surf->flags & SURF_SKY))
	{
		if (ent->owner->client)
			RemoveNuke(ent->owner);
		G_FreeEdict(ent);
		return;
	}

	GuidedNuke_Think(ent);
	return;
}

void fire_guidednuke(edict_t* self, vec3_t start, vec3_t dir, int damage, int speed, float damage_radius, int radius_damage)
{
	edict_t* rocket;

	rocket = G_Spawn();
	VectorCopy(start, rocket->s.origin);
	VectorCopy(dir, rocket->movedir);
	vectoangles(dir, rocket->s.angles);
	VectorScale(dir, (float)speed, rocket->velocity);
	rocket->movetype = MOVETYPE_FLYMISSILE;

	// David Martin - 06/30/05
	// Allow people to shoot down the guided rockets. This should have always 
	// been here, but we forgot to add it ... or maybe it was because we didn't
	// know what we were doing.

	// 06/30/05 QwazyWabbit
	// We actually did know what we were doing but didn't intend for 
	// them to be vulnerable.
	// Add cvar to make this an admin option. Normal defense against guided
	// rockets or nukes is the angel of mercy or angel of energy.
	// The original 1.11 code defaulted to no damage.
	// Default value of the cvar is 0.
	if (g_nukes_takedamage->value)
		rocket->takedamage = DAMAGE_YES;
	else
		rocket->takedamage = DAMAGE_NO;

	rocket->clipmask = MASK_SHOT;
	rocket->solid = SOLID_BBOX;
	rocket->s.effects |= EF_ROCKET;
	VectorClear(rocket->mins);
	VectorClear(rocket->maxs);
	rocket->s.modelindex = gi.modelindex("models/objects/rocket/tris.md2");
	rocket->owner = self;
	rocket->client = self->client;
	rocket->touch = guidedNuke_touch;
	rocket->nextthink = level.time + 25;
	rocket->think = GuidedNuke_Think;
	rocket->dmg = damage;
	rocket->die = nuke_die;
	rocket->radius_dmg = radius_damage;
	rocket->dmg_radius = damage_radius;
	rocket->s.sound = gi.soundindex("weapons/rockfly.wav");
	rocket->classname = "guided nuke";
	rocket->classnum = CN_GUIDEDNUKE;

	if (self->client->missile)
		GuidedNuke_Think(self->client->missile);

	if (self->client)
	{
		self->client->missile = rocket;
		self->client->ps.gunindex = 0;
		if (!self->client->chasetoggle) self->client->oldplayer = G_Spawn();
		self->client->ps.pmove.pm_flags |= PMF_NO_PREDICTION;

		check_dodge(self, rocket->s.origin, dir, speed);
	}

	gi.linkentity(rocket);
}


void Weapon_GuidedNuke_Fire(edict_t* ent)
{
	vec3_t	offset = { 0 }, start, forward, right;
	int		damage;
	int		radius_damage;

	damage = 400 + (int)(random() * 20.0);
	radius_damage = 300;

	if (is_quad)
	{
		damage *= 4;
		radius_damage *= 4;
	}

	AngleVectors(ent->client->v_angle, forward, right, NULL);

	VectorScale(forward, -2, ent->client->kick_origin);
	ent->client->kick_angles[0] = -1;

	VectorSet(offset, 8, 8, ent->viewheight - 8.0f);
	P_ProjectSource(ent->client, ent->s.origin, offset, forward, right, start);
	fire_guidednuke(ent, start, forward, damage, 650, 200, 200);

	// send muzzle flash
	gi.WriteByte(svc_muzzleflash);
	gi.WriteShort(ent - g_edicts);
	gi.WriteByte(MZ_ROCKET | is_silenced);
	gi.multicast(ent->s.origin, MULTICAST_PVS);

	ent->client->ps.gunframe++;

	PlayerNoise(ent, start, PNOISE_WEAPON);

	if ((!((int)dmflags->value & DF_INFINITE_AMMO))
		&& (ent->client->ammo_index))
	{
		ent->client->pers.inventory[ent->client->ammo_index] -= 1;
		ent->client->pers.inventory[ent->client->ammo2_index] -= 4;
	}

	ent->client->guidedMissileFired = 1;
}

void Weapon_GuidedNuke(edict_t* ent)
{
	static int	pause_frames[] = { 25, 33, 42, 50, 0 };
	static int	fire_frames[] = { 5, 0 };

	if (ent->client->guidedMissileFired == 1)
		return;
	Weapon_Generic(ent, 4, 12, 50, 54, pause_frames, fire_frames, Weapon_GuidedNuke_Fire);
}
