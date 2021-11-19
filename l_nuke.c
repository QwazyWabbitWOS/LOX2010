#include "g_local.h"
#include "x_fire.h"
#include "l_nuke.h"

void nuke_touch(edict_t* ent, edict_t* other, cplane_t* plane, csurface_t* surf)
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

	if (surf && (surf->flags & SURF_SKY))
	{
		G_FreeEdict(ent);
		return;
	}

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

	G_FreeEdict(ent);
}

void fire_nuke(edict_t* self, vec3_t start, vec3_t dir, int damage, int speed, float damage_radius, int radius_damage)
{
	edict_t* nuke;

	nuke = G_Spawn();
	VectorCopy(start, nuke->s.origin);
	VectorCopy(dir, nuke->movedir);
	vectoangles(dir, nuke->s.angles);
	VectorScale(dir, (float)speed, nuke->velocity);
	nuke->movetype = MOVETYPE_FLYMISSILE;
	nuke->clipmask = MASK_SHOT;
	nuke->solid = SOLID_BBOX;
	nuke->s.effects = EF_ROCKET;
	VectorClear(nuke->mins);
	VectorClear(nuke->maxs);
	nuke->s.modelindex = gi.modelindex("models/objects/bomb/tris.md2");
	nuke->owner = self;
	nuke->touch = nuke_touch;
	nuke->nextthink = level.time + 8000.0f / speed;
	nuke->think = G_FreeEdict;
	nuke->dmg = damage;
	nuke->radius_dmg = radius_damage;
	nuke->dmg_radius = damage_radius;
	nuke->s.sound = gi.soundindex("weapons/rockfly.wav");
	nuke->classname = "nuke";
	nuke->classnum = CN_NUKE;

	if (self->client)
		check_dodge(self, nuke->s.origin, dir, speed);

	gi.linkentity(nuke);
}

void weapon_nuke_fire(edict_t* ent)
{
	vec3_t	offset = { 0 }, start;
	vec3_t	forward, right;
	int		damage;
	float	damage_radius = 1000;

	if (deathmatch->value)
		damage = 200;
	else
		damage = 500;

	if (ent->client->ps.gunframe == 9)
	{
		// send muzzle flash
		gi.WriteByte(svc_muzzleflash);
		gi.WriteShort(ent - g_edicts);
		gi.WriteByte(MZ_BFG | is_silenced);
		gi.multicast(ent->s.origin, MULTICAST_PVS);

		ent->client->ps.gunframe++;

		PlayerNoise(ent, start, PNOISE_WEAPON);
		return;
	}

	if (is_quad)
		damage *= 4;

	AngleVectors(ent->client->v_angle, forward, right, NULL);

	VectorScale(forward, -2, ent->client->kick_origin);

	// make a big pitch kick with an inverse fall
	ent->client->v_dmg_pitch = -40;
	ent->client->v_dmg_roll = crandom() * 8;
	ent->client->v_dmg_time = level.time + DAMAGE_TIME;

	VectorSet(offset, 8, 8, ent->viewheight - 8.0f);
	P_ProjectSource(ent->client, ent->s.origin, offset, forward, right, start);
	fire_nuke(ent, start, forward, damage, 800, damage_radius, damage);

	ent->client->ps.gunframe++;

	PlayerNoise(ent, start, PNOISE_WEAPON);

	if ((!((int)dmflags->value & DF_INFINITE_AMMO))
		&& (ent->client->ammo_index))
	{
		ent->client->pers.inventory[ent->client->ammo_index] -= 3;
		ent->client->pers.inventory[ent->client->ammo2_index] -= 3;
	}
}

void Weapon_Nuke(edict_t* ent)
{
	static int	pause_frames[] = { 39, 45, 50, 55, 0 };
	static int	fire_frames[] = { 9, 17, 0 };

	Weapon_Generic(ent, 8, 32, 55, 58, pause_frames, fire_frames, weapon_nuke_fire);
}
