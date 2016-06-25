
/***************************************************/
/**************  Guided Missiles      **************/
/***************************************************/

#include "g_local.h"

#include "l_rockets.h"
#include "x_fire.h"
#include "l_nuke.h"
#include "g_weapon.h"
#include "l_positron.h"
#include "l_angels.h"

// darKMajick definitions
#include "l_dm_grenades.h"

void RemoveGuided(edict_t *ent)
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

// wrapper for RemoveGuided 
/* When guided rockets can take damage they need a die function. */
void guided_die (edict_t *self, edict_t *inflictor, edict_t *attacker, int damage, vec3_t point)
{
	RemoveGuided(self);
}


//Explode rocket without touching anything
void GuidedRocket_Explode (edict_t *ent)
{
	if (ent->owner != NULL && ent->owner->client)
	{
		PlayerNoise(ent->owner, ent->s.origin, PNOISE_IMPACT);
		RemoveGuided(ent->owner);
	}

	Grenade_Explode_dM(ent);	// Use darKMajick Grenades //QW//
}


// When a rocket 'dies', it blows up next frame
void GuidedRocket_Think (edict_t *self)
{
	VectorClear(self->velocity);
//	self->takedamage = DAMAGE_NO;
	self->nextthink = level.time + FRAMETIME;
	self->think = GuidedRocket_Explode;
}


void guidedrocket_touch (edict_t *ent, edict_t *other, cplane_t *plane, csurface_t *surf)
{
	if (other == ent->owner)
		return;

	if (surf && (surf->flags & SURF_SKY))
	{
		if (ent->owner->client)
		RemoveGuided(ent->owner);
		G_FreeEdict (ent);
		return;
	}

	GuidedRocket_Think(ent);
	return;
}

void fire_guidedrocket (edict_t *self, vec3_t start, vec3_t dir, int damage, int speed, float damage_radius, int radius_damage, int dM_Type)
{
	edict_t *rocket;

	rocket = G_Spawn();
	VectorCopy (start, rocket->s.origin);
	VectorCopy (dir, rocket->movedir);
	vectoangles (dir, rocket->s.angles);
	VectorScale (dir, (float) speed, rocket->velocity);
	rocket->movetype = MOVETYPE_FLYMISSILE;

	// David Martin - 06/30/05
	// Allow people to shoot down the guided rockets. This should have always 
	// been here, but we forgot to add it or maybe it was because we didn't
	// know what we were doing.

	// 06/30/05 Geoff Joy
	// Add cvar to make this an admin option. Normal defense against guided
	// rockets or nukes is the angel of mercy or angel of energy.
	// The original 1.11 code defaulted to no damage. 
	// Default value of the cvar is 0.
	if (g_rockets_takedamage->value)
		rocket->takedamage = DAMAGE_YES;
	else
		rocket->takedamage = DAMAGE_NO;

	rocket->clipmask = MASK_SHOT;
	rocket->solid = SOLID_BBOX;
	SetGrenadeEffects(rocket, dM_Type); // the dM_grenade type
	rocket->dm_type = dM_Type;
	rocket->s.effects |= EF_ROCKET;
	VectorClear (rocket->mins);
	VectorClear (rocket->maxs);
	rocket->s.modelindex = gi.modelindex ("models/objects/grenade/tris.md2"); //the piloted grenaded
	rocket->owner = self;
	rocket->client = self->client;
	rocket->touch = guidedrocket_touch;
	rocket->nextthink = level.time + 25; // flight lifetime
	rocket->think = GuidedRocket_Think;	// what to do at end of lifetime
	rocket->dmg = damage;
	rocket->die = guided_die;
	rocket->radius_dmg = radius_damage;
	rocket->dmg_radius = damage_radius;
	rocket->s.sound = gi.soundindex ("weapons/rockfly.wav");
	rocket->classname = "guided rocket";
	rocket->classnum = CN_GUIDEDROCKET;

	if (self->client->missile)
		GuidedRocket_Think(self->client->missile);

	if (self->client)
	{
		self->client->missile = rocket;
		self->client->ps.gunindex = 0;
		if (!self->client->chasetoggle) self->client->oldplayer = G_Spawn();
		self->client->ps.pmove.pm_flags |= PMF_NO_PREDICTION;

		check_dodge (self, rocket->s.origin, dir, speed);
	}

	gi.linkentity (rocket);
}


void Weapon_GuidedMissile_Fire (edict_t *ent)
{
	vec3_t	offset, start, forward, right;
	int		damage;
	float	damage_radius;
	int		radius_damage;
	
	damage = 400 + (int)(random() * 20.0);
	radius_damage = 300;
	damage_radius = 120;
	if (is_quad)
	{
		damage *= 4;
		radius_damage *= 4;
	}
	
	AngleVectors (ent->client->v_angle, forward, right, NULL);
	
	VectorScale (forward, -2, ent->client->kick_origin);
	ent->client->kick_angles[0] = -1;
	
	VectorSet(offset, 8, 8, ent->viewheight - 8.0f);	// the origin of the fire point
	P_ProjectSource (ent->client, ent->s.origin, offset, forward, right, start);
	
	//QW//
	//Re-invent guided grenades.
	//Pick up last grenade selected and pass it in the damage argument
	//for application at the target of the guided missile.
	
	fire_guidedrocket (ent, start, forward,
		damage,
		650,		//speed
		200,		//damage radius
		200,		//radius damage
		ent->client->dM_grenade); // grenade type
	
	
	// send muzzle flash
	gi.WriteByte (svc_muzzleflash);
	gi.WriteShort (ent-g_edicts);
	gi.WriteByte (MZ_ROCKET | is_silenced);
	gi.multicast (ent->s.origin, MULTICAST_PVS);
	
	ent->client->ps.gunframe++;
	
	PlayerNoise(ent, start, PNOISE_WEAPON);
	
	if ((! ( (int)dmflags->value & DF_INFINITE_AMMO ) )
		&&(ent->client->ammo_index))
	{
		ent->client->pers.inventory[ent->client->ammo_index]-=1;
		ent->client->pers.inventory[ent->client->ammo2_index]-=4;
	}
	
	ent->client->guidedMissileFired = 1;
}

void Weapon_GuidedMissiles (edict_t *ent)
{
	static int	pause_frames[]	= {25, 33, 42, 50, 0};
	static int	fire_frames[]	= {5, 0};

	if (ent->client->guidedMissileFired == 1)
		return;
	Weapon_Generic (ent, 4, 12, 50, 54, pause_frames, fire_frames, Weapon_GuidedMissile_Fire);
}

