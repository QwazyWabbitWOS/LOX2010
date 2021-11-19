#include "g_local.h"

#include "l_positron.h"
#include "l_freezatron.h"
#include "l_freezegun.h"
#include "l_kaminit.h"
#include "l_antiflaregun.h"
#include "l_dm_grenades.h"
#include "l_rockets.h"


// darKMajick: ///----------------------------------------------------->>
//

void fire_grenade_plasma(edict_t* self, vec3_t start, vec3_t aimdir, int damage,
	int speed, float timer, float damage_radius)
{
	edict_t* grenade;
	vec3_t	dir, forward, right, up;

	vectoangles(aimdir, dir);
	AngleVectors(dir, forward, right, up);

	grenade = G_Spawn();
	VectorCopy(start, grenade->s.origin);
	VectorScale(aimdir, (float)speed, grenade->velocity);
	VectorMA(grenade->velocity, 200.0f + crandom() * 10.0f, up, grenade->velocity);
	VectorMA(grenade->velocity, crandom() * 10.0f, right, grenade->velocity);
	VectorSet(grenade->avelocity, 300, 300, 300);
	grenade->movetype = MOVETYPE_BOUNCE;
	grenade->clipmask = MASK_SHOT;
	grenade->solid = SOLID_BBOX;
	grenade->s.effects |= EF_GRENADE;
	VectorClear(grenade->mins);
	VectorClear(grenade->maxs);
	grenade->s.modelindex = gi.modelindex("sprites/s_bfg3.sp2");
	grenade->owner = self;
	grenade->s.frame = 0;
	grenade->touch = Grenade_Touch;
	grenade->nextthink = level.time + FRAMETIME;
	grenade->think = plasma_explode;
	grenade->dmg = damage;
	grenade->dmg_radius = damage_radius;
	grenade->classname = "plasma explosion";
	grenade->classnum = CN_PLASMAEXPLOSION;
	grenade->s.renderfx |= RF_TRANSLUCENT;
	gi.linkentity(grenade);
}

void plasma_explode(edict_t* ent)
{
	ent->nextthink = level.time + FRAMETIME;
	ent->s.frame++;
	if (ent->s.frame >= 5)
		ent->think = G_FreeEdict;
}


void banzai_fire(edict_t* ent)
{

	int n;
	vec3_t		origin;
	int mod;
	edict_t* blip = NULL;
	vec3_t blipdir = { 0 };
	// Set up the means of death.
	mod = MOD_RAILGUN2;

	ent->nextthink = level.time + FRAMETIME;


	// Now trace the kaminit shots:
	VectorMA(ent->s.origin, -0.02f, ent->velocity, origin);

	for (n = 0; n < 12; n++)
	{
		blip = NULL;
		while ((blip = findradius(blip, origin, 1024)) != NULL)
		{
			// See if this is the kind of blip we can home in on.
			// more or less with most-likely events first.
			if (blip->cloaked)
				continue;
			if (blip->health <= 0)
				continue;
			if (blip == ent->owner)
				continue;
			if (!visible(ent, blip))
				continue;
			if (!blip->takedamage)
				continue;
			if (blip->oldmodel)
				continue;
			if (blip->client == NULL || blip->client->kamikaze_mode)
				continue;
			if (!(blip->svflags & SVF_MONSTER) && !blip->client)
				continue;
			if ((blip->client == NULL) || (blip->client->invisible))
				continue; //QW// decoys have null client ID's so ignore them

			// Determine where the blip is in relation to us.
			VectorSubtract(blip->s.origin, ent->s.origin, blipdir);
			blipdir[2] += 16;

			// Determine how "good" of a target it is.  (Currently, that's the
			// enemy that's most being aimed at, regardless of distance.)
			VectorNormalize(blipdir);

			fire_kaminit(ent->owner, origin, blipdir, 0, 2);
			break;
		}
	}

	T_RadiusDamage(ent, ent->owner, ent->dmg_radius - 50, NULL,
		ent->dmg_radius - 40, mod);
	VectorMA(ent->s.origin, -0.02f, ent->velocity, origin);
	gi.WriteByte(svc_temp_entity);
	if (ent->waterlevel)
	{
		if (ent->groundentity)
			gi.WriteByte(TE_GRENADE_EXPLOSION_WATER);
		else
			gi.WriteByte(TE_ROCKET_EXPLOSION_WATER);
	}
	else
	{
		if (ent->groundentity)
			gi.WriteByte(TE_GRENADE_EXPLOSION);
		else
			gi.WriteByte(TE_ROCKET_EXPLOSION);
	}
	gi.WritePosition(origin);
	gi.multicast(ent->s.origin, MULTICAST_PVS);

	ent->think = G_FreeEdict;

}

void freezer_fire(edict_t* ent)
{

	int n;
	vec3_t		origin;
	int mod;
	edict_t* blip = NULL;
	vec3_t blipdir = { 0 };
	// Set up the means of death.
	mod = MOD_RAILGUN2;

	ent->nextthink = level.time + FRAMETIME;


	// Now trace the kaminit shots:
	VectorMA(ent->s.origin, -0.02f, ent->velocity, origin);

	for (n = 0; n < 12; n++)
	{
		blip = NULL;
		while ((blip = findradius(blip, origin, 1024)) != NULL)
		{
			// See if this is the kind of blip we can home in on.
			// more or less with most-likely events first.
			if (blip->cloaked)
				continue;
			if (blip->health <= 0)
				continue;
			if (blip == ent->owner)
				continue;
			if (!visible(ent, blip))
				continue;
			if (!blip->takedamage)
				continue;
			if (blip->oldmodel)
				continue;
			if (blip->client == NULL || blip->client->kamikaze_mode)
				continue;
			if (!(blip->svflags & SVF_MONSTER) && !blip->client)
				continue;

			if ((blip->client == NULL) || (blip->client->invisible))
				continue; //QW// decoys have null client ID's so ignore them

			// Determine where the blip is in relation to us.
			VectorSubtract(blip->s.origin, ent->s.origin, blipdir);
			blipdir[2] += 16;

			// Determine how "good" of a target it is.  (Currently, that's the
			// enemy that's most being aimed at, regardless of distance.)
			VectorNormalize(blipdir);

			fire_freezer(ent->owner, origin, blipdir, FREEZER_DAMAGE, FREEZER_SPEED, EF_BLASTER);
			break;
		}
	}

	T_RadiusDamage(ent, ent->owner, 100, NULL, 300, mod);

	gi.sound(ent, CHAN_AUTO, gi.soundindex("weapons/bfg__x1b.wav"), 1, ATTN_NORM, 0);

	VectorMA(ent->s.origin, -0.02f, ent->velocity, origin);
	gi.WriteByte(svc_temp_entity);
	if (ent->waterlevel)
	{
		if (ent->groundentity)
			gi.WriteByte(TE_GRENADE_EXPLOSION_WATER);
		else
			gi.WriteByte(TE_ROCKET_EXPLOSION_WATER);
	}
	else
	{
		if (ent->groundentity)
			gi.WriteByte(TE_GRENADE_EXPLOSION);
		else
			gi.WriteByte(TE_ROCKET_EXPLOSION);
	}
	gi.WritePosition(origin);
	gi.multicast(ent->s.origin, MULTICAST_PVS);

	ent->think = G_FreeEdict;

}

void spinning_rail_fire(edict_t* ent)
{

	int n;
	vec3_t	origin;
	vec3_t	grenade_angs = { 0 };
	vec3_t	forward;
	vec3_t	right;
	vec3_t	up;

	int mod;

	// Set up the means of death.
	mod = MOD_RAILGUN;

	ent->nextthink = level.time + FRAMETIME;

	if (ent->fireCounter == 36)
	{
		T_RadiusDamage(ent, ent->owner, ent->dmg_radius - 50, NULL, ent->dmg_radius - 40, mod);
		VectorMA(ent->s.origin, -0.02f, ent->velocity, origin);
		gi.WriteByte(svc_temp_entity);

		if (ent->waterlevel)
		{
			if (ent->groundentity)
				gi.WriteByte(TE_GRENADE_EXPLOSION_WATER);
			else
				gi.WriteByte(TE_ROCKET_EXPLOSION_WATER);
		}
		else
		{
			if (ent->groundentity)
				gi.WriteByte(TE_GRENADE_EXPLOSION);
			else
				gi.WriteByte(TE_ROCKET_EXPLOSION);
		}

		gi.WritePosition(origin);
		gi.multicast(ent->s.origin, MULTICAST_PVS);
		ent->think = G_FreeEdict;
		return;
	}

	// Now trace the rail shots:
	VectorMA(ent->s.origin, -0.02f, ent->velocity, origin);

	for (n = 0; n < 6; n++)
	{
		grenade_angs[0] = 0;//horizontal plane only now, was: 10*n*ent->fireCounter * (crandom() * 4);
		grenade_angs[1] = (float)(((360.0f / 6) * n) + (10.0f * ent->fireCounter));
		grenade_angs[2] = 0;
		AngleVectors(grenade_angs, forward, right, up);
		fire_rail(ent->owner, origin, forward, 60, 120);
	}

	ent->fireCounter++;
}

void grenade_float(edict_t* ent)
{

	ent->nextthink = level.time + FRAMETIME;
	//ent->floatTime++;

	if (ent->canFloat == 0 || (ent->floatTime + ent->floatRiseTime) <= level.time)
	{
		ent->fireCounter = 0;
		ent->think = ent->floatthink;
		ent->nextthink = level.time + FRAMETIME;
		return;
	}

	ent->s.origin[2] += ent->floatRiseRate;
	gi.multicast(ent->s.origin, MULTICAST_PVS);
}


void dM_Flame_Touch(edict_t* ent, edict_t* other, cplane_t* plane, csurface_t* surf)
{
	if (surf && (surf->flags & SURF_SKY))
	{
		G_FreeEdict(ent);
		return;
	}

	if (other->takedamage)
	{
		int mod;

		// Set up the means of death.
		mod = MOD_NAPALM;
		if (i_fragban & WFB_NAPALMGRENADE)
			mod |= MOD_NOFRAG;

		T_Damage(other, ent, ent->owner, ent->velocity, ent->s.origin,
			plane->normal, ent->dmg, 0, 0, mod);
		G_FreeEdict(ent);
	}
}

void Flame_Burn(edict_t* ent)
{
	gi.WriteByte(svc_temp_entity);
	gi.WriteByte(TE_GRENADE_EXPLOSION);
	gi.WritePosition(ent->s.origin);
	gi.multicast(ent->s.origin, MULTICAST_PVS);

	{
		int mod;

		// Set up the means of death.
		mod = MOD_NAPALM;
		if (i_fragban & WFB_NAPALMGRENADE)
			mod |= MOD_NOFRAG;

		T_RadiusDamage(ent, ent->owner, 120, NULL, 120, mod);
	}

	ent->nextthink = level.time + 0.8f + random();
	ent->delay = ent->delay - 1;
	if (ent->delay <= 0)
		G_FreeEdict(ent);
}

void Cata_Explode(edict_t* ent)
{
	ent->s.frame += 2;

	if (ent->s.frame > 12)
		ent->s.frame = 0;

	ent->nextthink = level.time + FRAMETIME;
	ent->delay = ent->delay - FRAMETIME;

	if (ent->delay <= 0)
		G_FreeEdict(ent);
}

void Cata_Explode_Touch(edict_t* ent, edict_t* other, cplane_t* plane, csurface_t* surf)
{
	edict_t* head = NULL;

	if (other->classname == ent->classname)
		return;

	if (surf && (surf->flags & SURF_SKY))
	{
		G_FreeEdict(ent);
		return;
	}

	{
		int mod;

		// Set up the means of death.
		mod = MOD_CATA;
		if (i_fragban & WFB_CATACLYSMDEVICE)
			mod |= MOD_NOFRAG;

		T_RadiusDamage(ent, ent->owner, 250, NULL, 400, mod);
	}

	// Work out screen shakes:
	while ((head = findradius(head, ent->s.origin, 1024)) != NULL)
	{
		if (!head->client)
			continue;
		if (!CanDamage(head, ent))
			continue;
		head->client->v_dmg_pitch = 8 * crandom();
		head->client->v_dmg_roll = 8 * crandom();
		head->client->v_dmg_time = level.time + 1;
	}

	gi.sound(ent, CHAN_WEAPON, gi.soundindex("weapons/grenlx1a.wav"), 1, ATTN_NORM, 0);

	G_FreeEdict(ent);
}

void Shrap_Explode(edict_t* ent)
{
	ent->s.frame += 0;
	if (ent->s.frame > 12)
		ent->s.frame = 0;
	ent->nextthink = level.time + FRAMETIME;
	ent->delay = ent->delay - FRAMETIME;
	if (ent->delay <= 0)
		G_FreeEdict(ent);
}

void Shrap_Explode_Touch(edict_t* ent, edict_t* other, cplane_t* plane, csurface_t* surf)
{
	edict_t* head = NULL;

	if (other->classname == ent->classname)
		return;

	if (surf && (surf->flags & SURF_SKY))
	{
		G_FreeEdict(ent);
		return;
	}

	{
		int mod;

		// Set up the means of death.
		mod = MOD_SHRAPG;
		if (i_fragban & WFB_SHRAPNELGRENADE)
			mod |= MOD_NOFRAG;

		T_RadiusDamage(ent, ent->owner, 100, NULL, 200, mod);
	}

	// Work out screen shakes:
	while ((head = findradius(head, ent->s.origin, 1024)) != NULL)
	{
		if (!head->client)
			continue;
		if (!CanDamage(head, ent))
			continue;
		head->client->v_dmg_pitch = 2 * crandom();
		head->client->v_dmg_roll = 2 * crandom();
		head->client->v_dmg_time = level.time + 1;
	}

	gi.sound(ent, CHAN_WEAPON, gi.soundindex("weapons/grenlx1a.wav"), 1, ATTN_NORM, 0);

	G_FreeEdict(ent);
}

void Proxie_think(edict_t* ent)
{
	edict_t* blip = NULL;

	if (level.time > ent->delay)
	{
		Grenade_Explode_dM(ent);	// lifetime expired
		return;
	}

	if (ent->owner->health <= 0)
	{
		Grenade_Explode_dM(ent);	// owner was fragged
		return;
	}

	ent->think = Proxie_think;

	while ((blip = findradius(blip, ent->s.origin, 100)) != NULL)
	{
		if (!(blip->svflags & SVF_MONSTER) && !blip->client)
			continue;
		if (OnSameTeam(ent, blip))
			continue;
		if (blip == ent->owner)
			continue;
		if (!blip->takedamage)
			continue;
		if (blip->health <= 0)
			continue;
		if (!visible(ent, blip))
			continue;
		ent->think = Grenade_Explode_dM;
		break;
	}
	ent->nextthink = level.time + 0.1f;
}

void Floating_Mine(edict_t* ent)
{
	float speed;

	speed = VectorLength(ent->velocity);

	if (speed - 5 < 0)
	{
		speed = 0;
		ent->s.effects = EF_NONE;		// they stop glowing
		ent->s.renderfx = 0;
		ent->delay = 60 + level.time;		// they live for 60 seconds
		ent->nextthink = level.time + 0.1f;
		ent->think = Proxie_think;
	}
	else
	{
		speed -= 75;
		ent->nextthink = level.time + 0.4f;
		ent->think = Floating_Mine;
	}

	VectorScale(ent->movedir, speed, ent->velocity);
	gi.linkentity(ent);
}

void Grenade_Explode_dM(edict_t* ent)
{
	vec3_t	origin;
	vec3_t	grenade_angs = { 0 };
	vec3_t	forward;
	vec3_t	right;
	vec3_t	up;
	edict_t* flame, * head = NULL;
	int n, mod;

	// this should never happen in release but if it does, log it and fall through
	if (!(ent->dm_type >= DM_NORMALGRENADE && ent->dm_type <= DM_SPINNINGRAILBOMB))
		gi.dprintf("Oops: dM grenade type out of range %i\n", ent->dm_type);

	// If we somehow don't have an owner any more, just die.
	// (Why wouldn't we have an owner?)
	if (!(ent->owner) || !(ent->owner->client))
	{
		gi.dprintf("Oops: dM grenade without an owner (freeing it now)\n");
		G_FreeEdict(ent);
		return;
	}

	if (ent->owner->client)
		PlayerNoise(ent->owner, ent->s.origin, PNOISE_IMPACT);

	VectorMA(ent->s.origin, -0.02f, ent->velocity, origin);

	if (ent->classnum == CN_FLOATINGMINE)
		ent->owner->client->mine_count--;

	if (ent->classnum == CN_LASERTRIP_GRENADE)  //QW/// for laser tripwire limits
		if (ent->owner->client->lasertrips > 0) ent->owner->client->lasertrips--;

	if (ent->dm_type == DM_NORMALGRENADE)
	{
		// ///------>> Standard:
		// dmg_radius is really the damage:

		// Set up the means of death.
		mod = MOD_HANDGRENADE;
		if (i_fragban & WFB_GRENADE)
			mod |= MOD_NOFRAG;

		T_RadiusDamage(ent, ent->owner, ent->dmg_radius, NULL,
			ent->dmg_radius + 40, mod);

		gi.WriteByte(svc_temp_entity);
		if (ent->waterlevel)
		{
			if (ent->groundentity)
				gi.WriteByte(TE_GRENADE_EXPLOSION_WATER);
			else
				gi.WriteByte(TE_ROCKET_EXPLOSION_WATER);
		}
		else
		{
			if (ent->groundentity)
				gi.WriteByte(TE_GRENADE_EXPLOSION);
			else
				gi.WriteByte(TE_ROCKET_EXPLOSION);
		}

		gi.WritePosition(origin);
		gi.multicast(ent->s.origin, MULTICAST_PVS);
	}
	else if (ent->dm_type == DM_CLUSTERGRENADE)
	{
		// ///------>> Cluster:

		// Set up the means of death.
		mod = MOD_CLUSTER;
		if (i_fragban & WFB_CLUSTERGRENADE)
			mod |= MOD_NOFRAG;

		T_RadiusDamage(ent, ent->owner, 120, NULL, 120, mod);

		VectorMA(ent->s.origin, -0.02f, ent->velocity, origin);
		gi.WriteByte(svc_temp_entity);
		if (ent->waterlevel)
		{
			if (ent->groundentity)
				gi.WriteByte(TE_GRENADE_EXPLOSION_WATER);
			else
				gi.WriteByte(TE_ROCKET_EXPLOSION_WATER);
		}
		else
		{
			if (ent->groundentity)
				gi.WriteByte(TE_GRENADE_EXPLOSION);
			else
				gi.WriteByte(TE_ROCKET_EXPLOSION);
		}
		gi.WritePosition(origin);
		gi.multicast(ent->s.origin, MULTICAST_PVS);

		// Cluster grenades:
		for (n = 0; n < 12; n++)
		{
			grenade_angs[0] = -45;
			grenade_angs[1] = n * 30.0f;
			grenade_angs[2] = 0;
			AngleVectors(grenade_angs, forward, right, up);
			fire_grenade(ent->owner, origin, forward, 60, 600, 2.0, 120);
		}
	}
	else if (ent->dm_type == DM_RAILBOMB)
	{
		// ///------>> RailBomb:

		// Set up the means of death.
		mod = MOD_RAILBOMB;
		if (i_fragban & WFB_RAILBOMB)
			mod |= MOD_NOFRAG;

		T_RadiusDamage(ent, ent->owner, ent->dmg_radius - 50, NULL,
			ent->dmg_radius - 40, mod);

		VectorMA(ent->s.origin, -0.02f, ent->velocity, origin);
		gi.WriteByte(svc_temp_entity);
		if (ent->waterlevel)
		{
			if (ent->groundentity)
				gi.WriteByte(TE_GRENADE_EXPLOSION_WATER);
			else
				gi.WriteByte(TE_ROCKET_EXPLOSION_WATER);
		}
		else
		{
			if (ent->groundentity)
				gi.WriteByte(TE_GRENADE_EXPLOSION);
			else
				gi.WriteByte(TE_ROCKET_EXPLOSION);
		}
		gi.WritePosition(origin);
		gi.multicast(ent->s.origin, MULTICAST_PVS);
		// Now trace the railgun shots:
		for (n = 0; n < 30; n++)
		{
			grenade_angs[0] = -5 + (crandom() * 2);
			grenade_angs[1] = n * 36 + crandom() * 2;
			grenade_angs[2] = 0;
			AngleVectors(grenade_angs, forward, right, up);
			fire_rail(ent->owner, origin, forward, 60, 120);
		}
	}
	else if (ent->dm_type == DM_PLASMAGRENADE)
	{
		// ///------>> Plasma:

		// Set up the means of death.
		mod = MOD_PLASMAG;
		if (i_fragban & WFB_PLASMAGRENADE)
			mod |= MOD_NOFRAG;

		ent->classname = "plasma explosion";
		ent->classnum = CN_PLASMAEXPLOSION;
		T_RadiusDamage(ent, ent->owner, 150, NULL, 300, mod);

		// Kludge to get louder sound, since vol can't exceed 1.0
		gi.sound(ent, CHAN_WEAPON, gi.soundindex("weapons/bfg__x1b.wav"), 1, ATTN_NORM, 0);
		gi.sound(ent, CHAN_VOICE, gi.soundindex("weapons/bfg__x1b.wav"), 1, ATTN_NORM, 0);
		gi.sound(ent, CHAN_ITEM, gi.soundindex("weapons/bfg__x1b.wav"), 1, ATTN_NORM, 0);

		ent->s.modelindex = gi.modelindex("sprites/s_bfg3.sp2");
		ent->s.frame = 0;
		ent->think = plasma_explode;
		ent->nextthink = level.time + FRAMETIME;
		ent->movetype = MOVETYPE_NONE;
		ent->s.renderfx |= RF_TRANSLUCENT;
		gi.WriteByte(svc_temp_entity);
		gi.WriteByte(TE_BFG_BIGEXPLOSION);
		gi.WritePosition(ent->s.origin);
		gi.multicast(ent->s.origin, MULTICAST_PVS);
	}
	else if (ent->dm_type == DM_NAPALMGRENADE)
	{
		// ///------>> Napalm:

		// Set up the means of death.
		mod = MOD_NAPALM;
		if (i_fragban & WFB_NAPALMGRENADE)
			mod |= MOD_NOFRAG;

		T_RadiusDamage(ent, /*inflictor*/
			ent->owner, /*attacker*/
			50,		/*damage*/
			NULL,	/*ignore*/
			80,		/*radius*/
			mod);	/*means of death*/

		VectorMA(ent->s.origin, -0.02f, ent->velocity, origin);
		gi.WriteByte(svc_temp_entity);
		if (ent->waterlevel)
		{
			if (ent->groundentity)
				gi.WriteByte(TE_GRENADE_EXPLOSION_WATER);
			else
				gi.WriteByte(TE_ROCKET_EXPLOSION_WATER);
		}
		else
		{
			if (ent->groundentity)
				gi.WriteByte(TE_GRENADE_EXPLOSION);
			else
				gi.WriteByte(TE_ROCKET_EXPLOSION);
		}
		gi.WritePosition(origin);
		gi.multicast(ent->s.origin, MULTICAST_PVS);
		origin[2] = origin[2] + 4;
		// Throw napalm flames:
		for (n = 0; n < 8; n++)
		{
			flame = G_Spawn();
			VectorCopy(origin, flame->s.origin);
			VectorSet(flame->velocity, crandom() * 500, crandom() * 500, random() * 400 + 50); // Velocity - FIXME/dM
			VectorSet(flame->avelocity, crandom() * 400, crandom() * 400, 0); // Avel - FIXME/dM
			flame->movetype = MOVETYPE_TOSS;
			flame->clipmask = MASK_SHOT;
			flame->solid = SOLID_TRIGGER;
			flame->s.modelindex = gi.modelindex("models/objects/grenade2/tris.md2");
			flame->s.effects |= EF_COLOR_SHELL;
			flame->s.renderfx |= RF_SHELL_RED;
			flame->s.frame = 0;
			VectorClear(flame->mins);
			VectorClear(flame->maxs);
			flame->owner = ent->owner;
			flame->touch = dM_Flame_Touch;
			flame->delay = 3 + random() * 3;
			flame->think = Flame_Burn;
			flame->nextthink = level.time + random();
			flame->classname = "napalm";
			flame->classnum = CN_NAPALM;
		}
	}
	else if (ent->dm_type == DM_SHRAPNELGRENADE)
	{
		// ///------>> Shrapnel:

		// Set up the means of death.
		mod = MOD_SHRAPNEL;
		if (i_fragban & WFB_SHRAPNELGRENADE)
			mod |= MOD_NOFRAG;

		T_RadiusDamage(ent, /*inflictor*/
			ent->owner, /*attacker*/
			20,	/*damage*/
			NULL,	/*ignore*/
			ent->dmg_radius - 100,	/*radius*/
			mod);	/*means of death*/

		// Big explosion effect:
		for (n = 0; n < 128; n += 32)
		{
			VectorMA(ent->s.origin, -0.02f, ent->velocity, origin);
			gi.WriteByte(svc_temp_entity);
			gi.WriteByte(TE_GRENADE_EXPLOSION);
			gi.WritePosition(origin);
			gi.multicast(origin, MULTICAST_PVS);
		}

		VectorMA(ent->s.origin, -0.02f, ent->velocity, origin);
		origin[2] = origin[2] + 64;

		for (n = 0; n < 30; n++)
		{
			grenade_angs[0] = 0;
			grenade_angs[1] = n * 12.0f;
			grenade_angs[2] = 0;
			AngleVectors(grenade_angs, forward, right, up);
			flame = G_Spawn();
			VectorCopy(origin, flame->s.origin);
			VectorClear(flame->velocity);
			VectorMA(flame->velocity, 550, forward, flame->velocity);
			flame->movetype = MOVETYPE_TOSS;
			flame->clipmask = MASK_SHOT;
			flame->solid = SOLID_TRIGGER;
			flame->dmg = 80;

			flame->s.modelindex = gi.modelindex("models/objects/debris2/tris.md2");
			flame->s.frame = 0;
			flame->s.effects |= EF_ROCKET;
			flame->s.sound = gi.soundindex("weapons/rocklx1a.wav");

			VectorSet(flame->mins, -3, -3, -3);
			VectorSet(flame->maxs, 3, 3, 3);
			flame->owner = ent->owner;
			flame->touch = Shrap_Explode_Touch;

			flame->delay = 5;
			flame->think = Shrap_Explode;
			flame->nextthink = level.time + FRAMETIME;

			flame->classname = "shrapnel";
			flame->classnum = CN_SHRAPNEL;
		}
	}
	else if (ent->dm_type == DM_CATACLYSM)
	{
		// ///------>> Cataclysm:

		// Big explosion effect:
		for (n = 0; n < 128; n += 32)
		{
			VectorMA(ent->s.origin, -0.02f, ent->velocity, origin);
			origin[0] = origin[0] + 16 * crandom();
			origin[1] = origin[1] + 16 * crandom();
			origin[2] = origin[2] + n;
			gi.WriteByte(svc_temp_entity);
			gi.WriteByte(TE_GRENADE_EXPLOSION);
			gi.WritePosition(origin);
			gi.multicast(origin, MULTICAST_PVS);
		}

		VectorMA(ent->s.origin, -0.02f, ent->velocity, origin);
		origin[2] = origin[2] + 64;

		for (n = 0; n < 30; n++)
		{
			grenade_angs[0] = 0;
			grenade_angs[1] = n * 12.0f;
			grenade_angs[2] = 0;
			AngleVectors(grenade_angs, forward, right, up);
			flame = G_Spawn();
			VectorCopy(origin, flame->s.origin);
			VectorClear(flame->velocity);
			VectorMA(flame->velocity, 550, forward, flame->velocity);
			flame->movetype = MOVETYPE_FLYMISSILE;
			flame->clipmask = MASK_SHOT;
			flame->solid = SOLID_TRIGGER;

			flame->s.modelindex = gi.modelindex("sprites/s_explo2.sp2");
			flame->s.frame = (int)random() * 4;
			flame->s.effects |= EF_HYPERBLASTER;
			flame->s.sound = gi.soundindex("weapons/bfg__l1a.wav");

			VectorSet(flame->mins, -3, -3, -3);
			VectorSet(flame->maxs, 3, 3, 3);
			flame->owner = ent->owner;
			flame->touch = Cata_Explode_Touch;

			flame->delay = 5;
			flame->think = Cata_Explode;
			flame->nextthink = level.time + FRAMETIME;

			flame->classname = "cataclysm explosion";
			flame->classnum = CN_CATACLYSMEXPLOSION;
		}

		// Do the gamma:
		while ((head = findradius(head, ent->s.origin, 1024)) != NULL)
		{
			if (!head->client)
				continue;
			if (!CanDamage(head, ent))
				continue;
			head->client->v_dmg_pitch = 20 * crandom();
			head->client->v_dmg_roll = 20 * crandom();
			head->client->damage_blend[0] = 1;
			head->client->damage_blend[1] = 1;
			head->client->damage_blend[2] = 1;
			head->client->damage_alpha = 0.8f;
			head->client->v_dmg_time = level.time + 1;
		}
	}

	else if (ent->dm_type == DM_PLASMACLUSTER)
	{
		// ///------>> Cluster:
		mod = MOD_PLASMACLUSTERGRENADE;
		if (i_fragban & WFB_PLASMACLUSTERGRENADE)
			mod |= MOD_NOFRAG;

		ent->classname = "plasma explosion";
		ent->classnum = CN_PLASMAEXPLOSION;
		T_RadiusDamage(ent, ent->owner, 500, NULL, 300, mod);

		// Kludge to get louder sound, since vol can't exceed 1.0
		gi.sound(ent, CHAN_WEAPON, gi.soundindex("weapons/bfg__x1b.wav"), 1, ATTN_NORM, 0);
		gi.sound(ent, CHAN_VOICE, gi.soundindex("weapons/bfg__x1b.wav"), 1, ATTN_NORM, 0);
		gi.sound(ent, CHAN_ITEM, gi.soundindex("weapons/bfg__x1b.wav"), 1, ATTN_NORM, 0);

		ent->s.modelindex = gi.modelindex("sprites/s_bfg3.sp2");
		ent->s.frame = 0;
		ent->think = plasma_explode;
		ent->nextthink = level.time + FRAMETIME;
		ent->movetype = MOVETYPE_NONE;
		ent->s.renderfx |= RF_TRANSLUCENT;
		gi.WriteByte(svc_temp_entity);
		gi.WriteByte(TE_BFG_BIGEXPLOSION);
		gi.WritePosition(ent->s.origin);
		gi.multicast(ent->s.origin, MULTICAST_PVS);


		// Cluster grenades:
		for (n = 0; n < 6; n++)
		{
			grenade_angs[0] = -45;
			grenade_angs[1] = n * 30.0f;
			grenade_angs[2] = 0;
			AngleVectors(grenade_angs, forward, right, up);
			fire_grenade_plasma(ent->owner, origin, forward, 150, 600, 8.0, 300);
		}
	}

	else if (ent->dm_type == DM_TELEGRENADE)
	{
		trace_t tr;
		// ///------>> Cluster:
		mod = MOD_TELEPORTGRENADE;

		ent->classname = "plasma explosion";
		ent->classnum = CN_PLASMAEXPLOSION;
		//T_RadiusDamage(ent, ent->owner, 150, NULL, 300, mod);

		// Kludge to get louder sound, since vol can't exceed 1.0
		gi.sound(ent, CHAN_WEAPON, gi.soundindex("weapons/bfg__x1b.wav"), 1, ATTN_NORM, 0);
		gi.sound(ent, CHAN_VOICE, gi.soundindex("weapons/bfg__x1b.wav"), 1, ATTN_NORM, 0);
		gi.sound(ent, CHAN_ITEM, gi.soundindex("weapons/bfg__x1b.wav"), 1, ATTN_NORM, 0);
		T_RadiusDamage(ent, ent->owner, 150, NULL, 300, mod);
		ent->s.modelindex = gi.modelindex("sprites/s_bfg3.sp2");
		ent->s.frame = 0;
		ent->think = plasma_explode;
		ent->nextthink = level.time + FRAMETIME;
		ent->movetype = MOVETYPE_NONE;
		ent->s.renderfx |= RF_TRANSLUCENT;
		{
			float temp1, temp2, temp3;
			float delta = .10f;
			temp1 = ent->s.origin[0] - ent->s.old_origin[0];
			temp2 = ent->s.origin[1] - ent->s.old_origin[1];
			temp3 = ent->s.origin[2] - ent->s.old_origin[2];

			if ((temp3 < 50) && (temp3 >= 0))
			{
			}

			else if ((temp3 >= -50) && (temp3 <= 0))
			{
			}
			else
			{
				temp1 = temp1 * delta;
				temp2 = temp2 * delta;
				temp3 = temp3 * delta;
			}

			ent->owner->s.origin[0] = ent->s.origin[0] - temp1;
			ent->owner->s.origin[1] = ent->s.origin[1] - temp2;
			ent->owner->s.origin[2] = ent->s.origin[2] - temp3;
		}

		if (ent->owner->client)
			check_dodge(ent->owner, ent->s.origin, ent->movedir, (int)ent->speed);

		tr = gi.trace(ent->owner->s.origin, NULL, NULL, ent->s.origin, ent, MASK_SHOT);
		if (tr.fraction < 1.0)
		{
			VectorMA(ent->s.origin, -10, ent->movedir, ent->s.origin);
			ent->touch(ent, tr.ent, NULL, NULL);		//QW// found a big bug here.
		}

		/* Big bug: Teleport grenades cause server to crash when planted as laser tripwires
		on a spawn pad.  A player hits it and this function fails on the line indicated above with
		a null pointer exception.  Fix: if player is carrying teleport grenades, plant
		plasma cluster grenades instead. See laser tripwire code.
		Tele-trips are not really desirable anyway, getting teleported back to
		your cluster of floating mines when somebody stumbles into them would not be fun.
		Teleporting into a teleporting player would be interesting.
		Fails miserably when you are teleporting onto your own teleport tripwire.
		This bug can be used to mount a DoS attack on older LOX servers. //QW//
		*/

		// draw the teleport splash at dest 
		gi.WriteByte(svc_temp_entity);		// this is a really cool teleporter effect.
		gi.WriteByte(TE_BOSSTPORT);			// since we are moving the player
		gi.WritePosition(ent->s.origin);		// we cant use the other way to 
		gi.multicast(ent->s.origin, MULTICAST_PVS);	// write the teleporter splash,
		ent->s.event = EV_PLAYER_TELEPORT;	// but we can at the dest
		gi.WriteByte(svc_temp_entity);
		gi.WriteByte(TE_BFG_BIGEXPLOSION);
		gi.WritePosition(ent->s.origin);
		gi.multicast(ent->s.origin, MULTICAST_PVS);
	}

	//	*********************************************************
	//	*************** freeze grenade detonation ***************
	//	*********************************************************

	else if (ent->dm_type == DM_FREEZEGRENADE)
	{

		// Set up the means of death.
		mod = MOD_FREEZE;
		if (i_fragban & WFB_FREEZEGRENADE)
			mod |= MOD_NOFRAG;

		ent->classname = "freeze explosion";
		ent->classnum = CN_FREEZEEXPLOSION;
		T_RadiusDamage(ent, ent->owner, 100, NULL, 300, mod);

		// Kludge to get louder sound, since vol can't exceed 1.0
		gi.sound(ent, CHAN_WEAPON, gi.soundindex("weapons/bfg__x1b.wav"), 1, ATTN_NORM, 0);
		gi.sound(ent, CHAN_VOICE, gi.soundindex("weapons/bfg__x1b.wav"), 1, ATTN_NORM, 0);
		gi.sound(ent, CHAN_ITEM, gi.soundindex("weapons/bfg__x1b.wav"), 1, ATTN_NORM, 0);

		ent->fireCounter = 0;
		ent->floatTime = level.time;
		ent->floatRiseTime = 0.3f;
		ent->floatRiseRate = 15;
		ent->think = grenade_float;
		ent->floatthink = freezer_fire;
		ent->nextthink = level.time + FRAMETIME;
		ent->touch = NULL;
		return;
	}

	else if (ent->dm_type == DM_HOMERGRENADE)
	{

		// Set up the means of death.
		mod = MOD_HOMERGRENADE;
		if (i_fragban & WFB_HOMERGRENADE)
			mod |= MOD_NOFRAG;

		T_RadiusDamage(ent, ent->owner, ent->dmg_radius - 50, NULL,
			ent->dmg_radius - 40, mod);

		VectorMA(ent->s.origin, -0.02f, ent->velocity, origin);
		gi.WriteByte(svc_temp_entity);
		if (ent->waterlevel)
		{
			if (ent->groundentity)
				gi.WriteByte(TE_GRENADE_EXPLOSION_WATER);
			else
				gi.WriteByte(TE_ROCKET_EXPLOSION_WATER);
		}
		else
		{
			if (ent->groundentity)
				gi.WriteByte(TE_GRENADE_EXPLOSION);
			else
				gi.WriteByte(TE_ROCKET_EXPLOSION);
		}
		gi.WritePosition(origin);
		gi.multicast(ent->s.origin, MULTICAST_PVS);
		// Now trace the railgun shots:
		for (n = 0; n < 4; n++)
		{
			grenade_angs[0] = -5 + (crandom() * 2);
			grenade_angs[1] = n * 36 + crandom() * 2;
			grenade_angs[2] = n * 10 + (crandom() * 4);
			if (grenade_angs[2] < 0)grenade_angs[2] *= -1;
			AngleVectors(grenade_angs, forward, right, up);
			fire_homing(ent->owner, origin, forward, 30, 800, 0, 0);
		}
	}

	else if (ent->dm_type == DM_ROCKETGRENADE)
	{

		// Set up the means of death.
		mod = MOD_ROCKETGRENADE;
		if (i_fragban & WFB_ROCKETGRENADE)
			mod |= MOD_NOFRAG;

		T_RadiusDamage(ent, ent->owner, ent->dmg_radius - 50, NULL,
			ent->dmg_radius - 40, mod);

		VectorMA(ent->s.origin, -0.02f, ent->velocity, origin);
		gi.WriteByte(svc_temp_entity);
		if (ent->waterlevel)
		{
			if (ent->groundentity)
				gi.WriteByte(TE_GRENADE_EXPLOSION_WATER);
			else
				gi.WriteByte(TE_ROCKET_EXPLOSION_WATER);
		}
		else
		{
			if (ent->groundentity)
				gi.WriteByte(TE_GRENADE_EXPLOSION);
			else
				gi.WriteByte(TE_ROCKET_EXPLOSION);
		}
		gi.WritePosition(origin);
		gi.multicast(ent->s.origin, MULTICAST_PVS);
		// Now trace the railgun shots:
		for (n = 0; n < 4; n++)
		{
			grenade_angs[0] = -5 + (crandom() * 2);
			grenade_angs[1] = n * 36 + crandom() * 2;
			grenade_angs[2] = n * 10 + (crandom() * 4);
			if (grenade_angs[2] < 0)grenade_angs[2] *= -1;
			AngleVectors(grenade_angs, forward, right, up);
			fire_rocket(ent->owner, origin, forward, 30, 800, 0, 0);
		}
	}

	else if (ent->dm_type == DM_BLINDINGGRENADE)
	{
		// ///------>> EggBomb:

		// Set up the means of death.
		mod = MOD_AIRFIST;
		if (i_fragban & WFB_BLINDINGGRENADE)
			mod |= MOD_NOFRAG;

		T_RadiusDamage(ent, ent->owner, ent->dmg_radius - 50, NULL,
			ent->dmg_radius - 40, mod);

		VectorMA(ent->s.origin, -0.02f, ent->velocity, origin);

		ent->s.renderfx |= RF_TRANSLUCENT;
		gi.WriteByte(svc_temp_entity);
		if (ent->waterlevel)
		{
			if (ent->groundentity)
				gi.WriteByte(TE_GRENADE_EXPLOSION_WATER);
			else
				gi.WriteByte(TE_ROCKET_EXPLOSION_WATER);
		}
		else
		{
			if (ent->groundentity)
				gi.WriteByte(TE_GRENADE_EXPLOSION);
			else
				gi.WriteByte(TE_ROCKET_EXPLOSION);
		}

		gi.WritePosition(origin);
		gi.multicast(ent->s.origin, MULTICAST_PVS);
	}

	else if (ent->dm_type == DM_POSITRONGRENADE)
	{

		// Set up the means of death.
		mod = MOD_POSITRON;
		if (i_fragban & WFB_POSITRONGRENADE)
			mod |= MOD_NOFRAG;

		ent->classname = "positron explosion";
		ent->classnum = CN_POSITRONEXPLOSION;
		MakePositron(ent->owner, NULL, ent->s.origin, 0.4f);

		gi.WriteByte(svc_temp_entity);
		if (ent->waterlevel)
		{
			if (ent->groundentity)
				gi.WriteByte(TE_GRENADE_EXPLOSION_WATER);
			else
				gi.WriteByte(TE_ROCKET_EXPLOSION_WATER);
		}
		else
		{
			if (ent->groundentity)
				gi.WriteByte(TE_GRENADE_EXPLOSION);
			else
				gi.WriteByte(TE_ROCKET_EXPLOSION);
		}

		gi.WritePosition(origin);
		gi.multicast(ent->s.origin, MULTICAST_PVS);
	}
	else if (ent->dm_type == DM_FREEZATRONGRENADE)
	{
		// ///------>> Positron:

		// Set up the means of death.
		mod = MOD_FREEZERSWEEPER;
		if (i_fragban & WFB_FREEZATRONGRENADE)
			mod |= MOD_NOFRAG;

		ent->classname = "freeza explosion";
		ent->classnum = CN_FREEZATRONEXPLOSION;
		MakeFreezatron(ent->owner, NULL, ent->s.origin, 0.4f);

		gi.WriteByte(svc_temp_entity);
		if (ent->waterlevel)
		{
			if (ent->groundentity)
				gi.WriteByte(TE_GRENADE_EXPLOSION_WATER);
			else
				gi.WriteByte(TE_ROCKET_EXPLOSION_WATER);
		}
		else
		{
			if (ent->groundentity)
				gi.WriteByte(TE_GRENADE_EXPLOSION);
			else
				gi.WriteByte(TE_ROCKET_EXPLOSION);
		}

		gi.WritePosition(origin);
		gi.multicast(ent->s.origin, MULTICAST_PVS);
	}

	else if (ent->dm_type == DM_MIDNIGHTGRENADE)
	{
		// ///------>> Cluster:
		mod = MOD_GRENADE;

		T_RadiusDamage(ent, ent->owner, 20, NULL, 10, mod);

		VectorMA(ent->s.origin, -0.02f, ent->velocity, origin);
		gi.WriteByte(svc_temp_entity);
		if (ent->waterlevel)
		{
			if (ent->groundentity)
				gi.WriteByte(TE_GRENADE_EXPLOSION_WATER);
			else
				gi.WriteByte(TE_ROCKET_EXPLOSION_WATER);
		}
		else
		{
			if (ent->groundentity)
				gi.WriteByte(TE_GRENADE_EXPLOSION);
			else
				gi.WriteByte(TE_ROCKET_EXPLOSION);
		}
		gi.WritePosition(origin);
		gi.multicast(ent->s.origin, MULTICAST_PVS);

		// Cluster grenades:
		for (n = 0; n < 12; n++)
		{
			grenade_angs[0] = -45;
			grenade_angs[1] = n * 30.0f;
			grenade_angs[2] = 45;
			AngleVectors(grenade_angs, forward, right, up);
			fire_antiflaregun(ent->owner, origin, forward, 5, 600);
		}
	}

	else if (ent->dm_type == DM_BANZAIGRENADE)
	{
		// ///------>> BANZAI!!!!!!!!!!!!!!!!:

		// Set up the means of death.
		mod = MOD_RAILGUN2;

		if (i_fragban & WFB_KAMIKAZE) // same fragban on all kamikaze modes
			mod |= MOD_NOFRAG;

		ent->fireCounter = 0;
		ent->floatTime = level.time;
		ent->floatRiseTime = 0.3f;
		ent->floatRiseRate = 15;
		ent->think = grenade_float;
		ent->floatthink = banzai_fire;
		ent->nextthink = level.time + FRAMETIME;
		ent->touch = NULL;
		return;
	}

	else if (ent->dm_type == DM_SPINNINGRAILBOMB)
	{
		// ///------>> Spinning rail grenade

		// Set up the means of death.
		mod = MOD_RAILGUN;
		if (i_fragban & WFB_RAILBOMB) // same fragban for railbomb and spinning rail bomb
			mod |= MOD_NOFRAG;

		ent->fireCounter = 0;
		ent->floatTime = level.time;
		ent->floatRiseTime = 0.3f;
		ent->floatRiseRate = 15;
		ent->think = grenade_float;
		ent->floatthink = spinning_rail_fire;
		ent->nextthink = level.time + FRAMETIME;
		return;
	}

	else
	{
		// if we get here something went horribly wrong.
		gi.dprintf("Unknown grenade type! (%i)\n", ent->dm_type);
	}

	ent->owner->client->explosive = NULL;
	G_FreeEdict(ent);
}

// OLD Version...
void Grenade_Touch_dM(edict_t* ent, edict_t* other, cplane_t* plane, csurface_t* surf)
{
	if (other == ent->owner)
		return;

	if (surf && (surf->flags & SURF_SKY))
	{
		G_FreeEdict(ent);
		return;
	}

	// Handle bazooka shots.
	if (ent->movetype == MOVETYPE_FLYMISSILE)
	{
		// No matter what we hit, explode.
		Grenade_Explode_dM(ent);
		return;
	}

	if (!other->takedamage)
	{
		if (ent->spawnflags & 1)
		{
			if (random() > 0.5)
				gi.sound(ent, CHAN_VOICE, gi.soundindex("weapons/hgrenb1a.wav"),
					1, ATTN_NORM, 0);
			else
				gi.sound(ent, CHAN_VOICE, gi.soundindex("weapons/hgrenb2a.wav"),
					1, ATTN_NORM, 0);
		}
		else
		{
			gi.sound(ent, CHAN_VOICE, gi.soundindex("weapons/grenlb1b.wav"),
				1, ATTN_NORM, 0);
		}
		return;
	}

	if ((ent->dmg == 0) || (ent->dmg == 1) || (ent->dmg == 2) || (ent->dmg == 3)
		|| (ent->dmg == 4))
		Grenade_Explode_dM(ent);
}


// darKMajick ///----------------------------------------------------->>

// ****************************************************
// SetGrenadeEffects
//
// This sets the glowing shells around the
// darKMajick grenade types. //QW//
//
// ****************************************************
void SetGrenadeEffects(edict_t* grenade, int type)
{
	if (type == DM_RAILBOMB)
	{
		grenade->s.effects |= EF_COLOR_SHELL;
		grenade->s.renderfx |= RF_SHELL_BLUE;
		grenade->s.effects |= EF_GRENADE;
	}

	else if (type == DM_PLASMAGRENADE)
	{
		grenade->s.effects |= EF_COLOR_SHELL;
		grenade->s.renderfx |= RF_SHELL_GREEN;
		grenade->s.effects |= EF_BFG;
	}

	else if (type == DM_NAPALMGRENADE)
	{
		grenade->s.effects |= EF_COLOR_SHELL;
		grenade->s.renderfx |= RF_SHELL_RED;
		grenade->s.effects |= EF_ROCKET;
	}
	else if (type == DM_CATACLYSM)
	{
		grenade->s.effects |= EF_COLOR_SHELL;
		grenade->s.renderfx |= RF_SHELL_BLUE;
		grenade->s.renderfx |= RF_SHELL_GREEN;
		grenade->s.renderfx |= RF_SHELL_RED;
		grenade->s.effects |= EF_ROCKET;
	}

	else if (type == DM_PLASMACLUSTER)
	{
		grenade->s.effects |= EF_COLOR_SHELL;
		grenade->s.renderfx |= RF_SHELL_BLUE;
		grenade->s.renderfx |= RF_SHELL_GREEN;
		grenade->s.renderfx |= RF_GLOW;
		grenade->s.effects |= EF_BFG;
	}

	else if (type == DM_TELEGRENADE)
	{
		grenade->s.effects |= EF_COLOR_SHELL;
		grenade->s.renderfx |= RF_SHELL_BLUE;
		grenade->s.renderfx |= RF_SHELL_RED;
		grenade->s.effects |= EF_GRENADE;
	}

	else if (type == DM_FREEZEGRENADE)
	{
		grenade->s.effects |= EF_COLOR_SHELL;
		grenade->s.renderfx |= RF_SHELL_BLUE;
		grenade->s.effects |= EF_TELEPORTER;
	}

	else if (type == DM_HOMERGRENADE)
	{
		grenade->s.effects |= EF_COLOR_SHELL;
		grenade->s.renderfx |= RF_SHELL_RED;
		grenade->s.effects |= EF_GRENADE;
	}

	else if (type == DM_ROCKETGRENADE)
	{
		grenade->s.effects |= EF_COLOR_SHELL;
		grenade->s.renderfx |= RF_SHELL_RED;
		grenade->s.effects |= EF_GRENADE;
	}

	else if (type == DM_BLINDINGGRENADE)
	{
		grenade->s.effects |= EF_GRENADE;
	}

	else if (type == DM_POSITRONGRENADE)
	{
		grenade->s.effects |= EF_COLOR_SHELL;
		grenade->s.renderfx |= RF_SHELL_BLUE;
		grenade->s.renderfx |= RF_SHELL_GREEN;
		grenade->s.renderfx |= RF_GLOW;
		grenade->s.effects |= EF_TRACKER;
	}

	else if (type == DM_FREEZATRONGRENADE)
	{
		grenade->s.effects |= EF_COLOR_SHELL;
		grenade->s.renderfx |= RF_SHELL_BLUE;
		grenade->s.renderfx |= RF_SHELL_GREEN;
		grenade->s.renderfx |= RF_GLOW;
		grenade->s.effects |= EF_SPINNINGLIGHTS;
	}

	else if (type == DM_MIDNIGHTGRENADE)
	{
		grenade->s.effects |= EF_TRACKER;
		grenade->s.effects |= EF_GREENGIB;
	}

	else if (type == DM_BANZAIGRENADE)
	{
		grenade->s.effects |= EF_ROTATE;
		grenade->s.effects |= EF_GRENADE;
		grenade->s.effects |= EF_HYPERBLASTER;
		grenade->s.effects |= EF_COLOR_SHELL;
		grenade->s.effects |= EF_BLUEHYPERBLASTER;
		grenade->s.renderfx |= RF_TRANSLUCENT;
		grenade->s.renderfx |= RF_GLOW;
		grenade->s.renderfx |= RF_SHELL_RED;
		grenade->s.renderfx |= RF_SHELL_GREEN;
		grenade->s.renderfx |= RF_SHELL_BLUE;
	}

	else if (type == DM_SPINNINGRAILBOMB)
	{
		grenade->s.effects |= EF_COLOR_SHELL;
		grenade->s.renderfx |= RF_SHELL_BLUE;
		grenade->s.effects |= EF_GRENADE;
	}

	else
	{
		grenade->s.effects |= EF_GRENADE;
	}
}

// ****************************************************
//
// This is pretty ugly. It handles all the methods of
// grenade tosses. The handheld and bazookad methods
// should have been separate functions. 
// I can't imagine what this thing would do if both
// booleans were true. //QW//
//
// ****************************************************
void fire_grenade_dM(edict_t* self, vec3_t start, vec3_t aimdir, int damage,
	int speed, float timer, float damage_radius, int typ,
	qboolean held, qboolean bazookad)
{
	edict_t* grenade;
	vec3_t	dir;
	vec3_t	forward, right, up;

	vectoangles(aimdir, dir);
	AngleVectors(dir, forward, right, up);

	grenade = G_Spawn();
	grenade->canFloat = 0;

	VectorCopy(start, grenade->s.origin);
	VectorScale(aimdir, (float)speed, grenade->velocity);
	if (bazookad)
	{
		grenade->movetype = MOVETYPE_FLYMISSILE;
		vectoangles(forward, grenade->s.angles);
	}
	else
	{
		VectorMA(grenade->velocity, 200 + crandom() * 10.0f, up, grenade->velocity);
		VectorMA(grenade->velocity, crandom() * 10.0f, right, grenade->velocity);
		VectorSet(grenade->avelocity, 300, 300, 300);
		grenade->movetype = MOVETYPE_BOUNCE;
	}
	grenade->clipmask = MASK_SHOT;
	grenade->solid = SOLID_BBOX;
	VectorClear(grenade->mins);
	VectorClear(grenade->maxs);
	if (held)
	{
		grenade->s.modelindex = gi.modelindex("models/objects/grenade2/tris.md2");
		// Allow the grenade to float if it was tossed
		grenade->canFloat = 1;
	}
	else
		grenade->s.modelindex = gi.modelindex("models/objects/grenade/tris.md2");

	// If the grenade was thrown using a grenade launcher allow it to float.
	if (!held && !bazookad)
		grenade->canFloat = 1;

	// moved to before SetGrenadeEffects so we can monkey around with the look of the grenade
	grenade->owner = self;

	SetGrenadeEffects(grenade, typ);

	if (bazookad)
	{

		if (grenade->owner->client->mine == 1)
		{
			if (max_floatingmines->value > 10) gi.cvar_set("max_floatingmines", "10");  //set a reasonable hard coded limit.
			if (max_floatingmines->value < 1) gi.cvar_set("max_floatingmines", "1");  //use weaponban to disable them
			if (grenade->owner->client->mine_count >= max_floatingmines->value)
			{
				gi.cprintf(self, PRINT_HIGH, "Too many mines\n");
				G_FreeEdict(grenade);
				return;
			}

			grenade->owner->client->mine_count++;
			grenade->s.modelindex = gi.modelindex("models/objects/grenade2/tris.md2");
			grenade->nextthink = level.time + 0.8f;
			grenade->think = Floating_Mine;

		}
		else
		{
			grenade->nextthink = level.time + 8000.0f / speed;
			grenade->think = G_FreeEdict;
		}
	}
	else if (self->client->expactive == 1)
	{
		self->client->explosive = grenade;
		grenade->nextthink = level.time + timer;
		grenade->think = Grenade_Explode_dM;

	}
	else
	{
		grenade->nextthink = level.time + timer;
		grenade->think = Grenade_Explode_dM;
	}

	grenade->touch = Grenade_Touch_dM;
	grenade->dmg = damage;
	grenade->dm_type = typ;
	grenade->dmg_radius = damage + 40.0f;
	grenade->classname = "grenade";
	grenade->classnum = CN_GRENADE;

	if (grenade->owner->client->mine == 1)
	{
		grenade->classname = "floating mine";
		grenade->classnum = CN_FLOATINGMINE;
	}

	if (held)
	{
		grenade->spawnflags = 1;
		grenade->s.sound = gi.soundindex("weapons/hgrenc1b.wav");
	}
	else if (bazookad)
		grenade->s.sound = gi.soundindex("flyer/flyidle1.wav");

	if (timer <= 0.0)
	{
		Grenade_Explode_dM(grenade);
	}
	else
	{
		gi.sound(self, CHAN_WEAPON, gi.soundindex("weapons/hgrent1a.wav"), 1, ATTN_NORM, 0);
		gi.linkentity(grenade);
	}
}

/*
=================
BecomeNewExplosion
=================
*/
void BecomeNewExplosion(edict_t* ent)
{
	vec3_t	origin;

	// calculate position for the explosion entity
	VectorMA(ent->s.origin, -0.02f, ent->velocity, origin);

	gi.WriteByte(svc_temp_entity);
	gi.WriteByte(TE_EXPLOSION1);
	gi.WritePosition(ent->s.origin);
	gi.multicast(ent->s.origin, MULTICAST_PVS);

	// send flash & bang
	gi.WriteByte(svc_muzzleflash);
	gi.WriteShort(ent - g_edicts);
	// any way to get a mz flash without hearing the weapon?
	gi.WriteByte(MZ_CHAINGUN2);
	gi.multicast(ent->s.origin, MULTICAST_PVS);
	// any other way to make this loud enough?
	BigBang(ent);

	// destroy object
	G_FreeEdict(ent);
}

/*
=================
Grenade_Explode

New grenade explosion.  Creates ~12 new entities (9 debris, 1 flash, 1 sound + 1 sound per client).
(Id barrel explosion creates 15)
=================
*/
void Grenade_Explode(edict_t* ent)
{
	int mod;

	// Set up the means of death.
	mod = MOD_CLUSTER;
	if (i_fragban & WFB_CLUSTERGRENADE)
		mod |= MOD_NOFRAG;

	// do blast damage	
	T_RadiusDamage(ent, ent->owner, (float)ent->dmg, NULL, ent->dmg_radius, mod);

	// shake view
	T_ShockWave(ent, 255, 1024);

	// let blast move items
	T_ShockItems(ent);

	// explode and destroy grenade
	BecomeNewExplosion(ent);
}
/*
=================
BigBang

Loud bang.
=================
*/
void BigBang(edict_t* ent)
{
	int		i;
	edict_t* ear;
	float		radius = 1024;
	vec3_t	d = { 0 };

	gi.sound(ent, CHAN_ITEM, gi.soundindex("weapons/rocklx1a.wav"), 1, ATTN_NORM, 0);

	// Unfortunately, this sounds weak, so check each client to see if
	// it is within the blast radius or in line of sight; if so,
	// send each client a loud ATTN_STATIC bang

	ear = &g_edicts[0];
	for (i = 0; i < globals.num_edicts; i++, ear++)
	{
		if (!ear->inuse)
			continue;
		if (!ear->client)
			continue;
		VectorSubtract(ear->s.origin, ent->s.origin, d);
		if ((VectorLength(d) < radius) | (isvisible(ent, ear)))
			gi.sound(ear, CHAN_VOICE, gi.soundindex("weapons/rocklx1a.wav"), 1, ATTN_STATIC, 0);
	}
}

/*
=================
ThrowShrapnel

This is just ThrowDebris with EF_GRENADE set.
Note: if debris is created before calling T_Damage,
setting DAMAGE_YES will give an orange splash effect.

=================
*/
void ThrowShrapnel(edict_t* self, char* modelname, float speed, vec3_t origin)
{
	edict_t* chunk;
	vec3_t	v = { 0 };

	chunk = G_Spawn();
	VectorCopy(origin, chunk->s.origin);
	gi.setmodel(chunk, modelname);
	v[0] = 100.0f * crandom();
	v[1] = 100.0f * crandom();
	v[2] = 100.0f + 100 * crandom();
	VectorMA(self->velocity, speed, v, chunk->velocity);
	chunk->movetype = MOVETYPE_BOUNCE;
	chunk->solid = SOLID_NOT;
	chunk->avelocity[0] = random() * 600;
	chunk->avelocity[1] = random() * 600;
	chunk->avelocity[2] = random() * 600;
	chunk->think = G_FreeEdict;
	chunk->nextthink = level.time + 5 + random() * 5;
	chunk->s.frame = 0;
	chunk->flags = 0;
	chunk->classname = "debris";
	chunk->classnum = CN_DEBRIS;
	chunk->takedamage = DAMAGE_NO;
	chunk->die = misc_die;

	chunk->s.effects |= EF_GRENADE;

	gi.linkentity(chunk);
}

/*
=================
ThrowShrapnel2

Less persistent
=================
*/
void ThrowShrapnel2(edict_t* self, char* modelname, float speed, vec3_t origin)
{
	edict_t* chunk;
	vec3_t	v = { 0 };

	chunk = G_Spawn();
	VectorCopy(origin, chunk->s.origin);
	gi.setmodel(chunk, modelname);
	v[0] = 100 * crandom();
	v[1] = 100 * crandom();
	v[2] = 100 * crandom();
	VectorMA(self->velocity, speed, v, chunk->velocity);
	chunk->movetype = MOVETYPE_BOUNCE;
	chunk->solid = SOLID_NOT;
	chunk->avelocity[0] = random() * 600;
	chunk->avelocity[1] = random() * 600;
	chunk->avelocity[2] = random() * 600;
	chunk->think = G_FreeEdict;
	chunk->nextthink = level.time + 0.5f + random() * 0.5f;
	chunk->s.frame = 0;
	chunk->flags = 0;
	chunk->classname = "debris";
	chunk->classnum = CN_DEBRIS;
	chunk->takedamage = DAMAGE_NO;
	chunk->die = misc_die;

	chunk->s.effects |= EF_GRENADE;

	gi.linkentity(chunk);
}

/*
=================
ThrowShrapnel3

Least persistent
=================
*/
void ThrowShrapnel3(edict_t* self, char* modelname, float speed, vec3_t origin)
{
	edict_t* chunk;
	vec3_t	v = { 0 };

	chunk = G_Spawn();
	VectorCopy(origin, chunk->s.origin);
	gi.setmodel(chunk, modelname);
	v[0] = 100 * crandom();
	v[1] = 100 * crandom();
	v[2] = 100 * crandom();
	VectorMA(self->velocity, speed, v, chunk->velocity);
	chunk->movetype = MOVETYPE_BOUNCE;
	chunk->solid = SOLID_NOT;
	chunk->avelocity[0] = random() * 600;
	chunk->avelocity[1] = random() * 600;
	chunk->avelocity[2] = random() * 600;
	chunk->think = G_FreeEdict;
	chunk->nextthink = level.time + random() * 0.3f;
	chunk->s.frame = 0;
	chunk->flags = 0;
	chunk->classname = "debris";
	chunk->classnum = CN_DEBRIS;
	chunk->takedamage = DAMAGE_NO;
	chunk->die = misc_die;

	chunk->s.effects |= EF_GRENADE;

	gi.linkentity(chunk);
}

/*
=================
ThrowShrapnel4

Medium persistence with glowing trail effect
=================
*/
void ThrowShrapnel4(edict_t* self, char* modelname, float speed, vec3_t origin)
{
	edict_t* chunk;
	vec3_t	v = { 0 };

	chunk = G_Spawn();
	VectorCopy(origin, chunk->s.origin);
	gi.setmodel(chunk, modelname);
	v[0] = 100 * crandom();
	v[1] = 100 * crandom();
	v[2] = 100 * crandom();
	VectorMA(self->velocity, speed, v, chunk->velocity);
	chunk->movetype = MOVETYPE_BOUNCE;
	chunk->solid = SOLID_NOT;
	chunk->avelocity[0] = random() * 600;
	chunk->avelocity[1] = random() * 600;
	chunk->avelocity[2] = random() * 600;
	chunk->think = G_FreeEdict;
	chunk->nextthink = level.time + random() * 2;
	chunk->s.frame = 0;
	chunk->flags = 0;
	chunk->classname = "debris";
	chunk->classnum = CN_DEBRIS;
	chunk->takedamage = DAMAGE_NO;
	chunk->die = misc_die;

	chunk->s.effects |= EF_GRENADE | EF_ROCKET;

	gi.linkentity(chunk);
}

/*
=================
barrel_explode

Added ShockItems, ShockWave, changed
some ThrowDebris to ThrowShrapnel
=================
*/
void barrel_explode(edict_t* self)
{
	vec3_t	org = { 0 };
	vec3_t	save = { 0 };
	float		spd;
	int		i;

	T_RadiusDamage(self, self->activator, (float)self->dmg, NULL, self->dmg + 40.0f, MOD_BARREL);
	T_ShockItems(self);
	T_ShockWave(self, 255, 1024);

	VectorCopy(self->s.origin, save);
	VectorMA(self->absmin, 0.5, self->size, self->s.origin);

	// a few big chunks
	spd = 1.5f * self->dmg / 200;
	org[0] = self->s.origin[0] + crandom() * self->size[0];
	org[1] = self->s.origin[1] + crandom() * self->size[1];
	org[2] = self->s.origin[2] + crandom() * self->size[2];
	ThrowDebris(self, "models/objects/debris1/tris.md2", spd, org);
	org[0] = self->s.origin[0] + crandom() * self->size[0];
	org[1] = self->s.origin[1] + crandom() * self->size[1];
	org[2] = self->s.origin[2] + crandom() * self->size[2];
	ThrowDebris(self, "models/objects/debris1/tris.md2", spd, org);

	// bottom corners
	spd = 1.75f * self->dmg / 200;
	VectorCopy(self->absmin, org);
	ThrowShrapnel(self, "models/objects/debris3/tris.md2", spd, org);
	VectorCopy(self->absmin, org);
	org[0] += self->size[0];
	ThrowShrapnel(self, "models/objects/debris3/tris.md2", spd, org);
	VectorCopy(self->absmin, org);
	org[1] += self->size[1];
	ThrowShrapnel(self, "models/objects/debris3/tris.md2", spd, org);
	VectorCopy(self->absmin, org);
	org[0] += self->size[0];
	org[1] += self->size[1];
	ThrowShrapnel(self, "models/objects/debris3/tris.md2", spd, org);

	// a bunch of little chunks
	spd = 10.0f * self->dmg / 200;
	for (i = 0; i < 8; i++)
	{
		org[0] = self->s.origin[0] + crandom() * self->size[0];
		org[1] = self->s.origin[1] + crandom() * self->size[1];
		org[2] = self->s.origin[2] + crandom() * self->size[2];
		ThrowShrapnel3(self, "models/objects/debris2/tris.md2", spd, org);
	}

	VectorCopy(save, self->s.origin);
	if (self->groundentity)
		BecomeExplosion2(self);
	else
		BecomeExplosion1(self);
}

/*
=================
target_explosion_explode

Added ShockWave
=================
*/
void target_explosion_explode(edict_t* self)
{
	float		save;

	gi.WriteByte(svc_temp_entity);
	gi.WriteByte(TE_EXPLOSION1);
	gi.WritePosition(self->s.origin);
	gi.multicast(self->s.origin, MULTICAST_PHS);

	T_RadiusDamage(self, self->activator, (float)self->dmg, NULL, self->dmg + 40.0f, MOD_UNKNOWN);
	T_ShockWave(self, 255, 1024);

	save = self->delay;
	self->delay = 0;
	G_UseTargets(self, self->activator);
	self->delay = save;

}
/*
=================
make_debris
=================
*/
void make_debris(edict_t* ent)
{
	vec3_t	org = { 0 };
	vec3_t	origin;
	float	spd;

	// calculate position for the explosion entity
	VectorMA(ent->s.origin, -0.02f, ent->velocity, origin);

	// make a few big chunks
	spd = 0.5f * ent->dmg / 200;
	org[0] = ent->s.origin[0] + crandom() * ent->size[0];
	org[1] = ent->s.origin[1] + crandom() * ent->size[1];
	org[2] = ent->s.origin[2] + crandom() * ent->size[2];
	ThrowShrapnel(ent, "models/objects/debris1/tris.md2", spd, org);
	spd = 1.5f * ent->dmg / 200;
	VectorCopy(ent->absmin, org);
	ThrowShrapnel(ent, "models/objects/debris2/tris.md2", spd, org);
	spd = 1.5f * ent->dmg / 200;
	VectorCopy(ent->absmin, org);
	ThrowShrapnel(ent, "models/objects/debris3/tris.md2", spd, org);
}

/*
============
T_ShockWave

Knocks view around a bit.  Based on T_RadiusDamage.
============
*/
void T_ShockWave(edict_t* inflictor, float damage, float radius)
{
	float	points;
	edict_t* ent = NULL;
	vec3_t	v = { 0 };
	//vec3_t	dir;

	float 	SHOCK_TIME = 0.1f;

	while ((ent = findradius(ent, inflictor->s.origin, radius)) != NULL)
	{
		if (!ent->takedamage)
			continue;
		if (!ent->client)
			continue;
		VectorAdd(ent->mins, ent->maxs, v);
		VectorMA(ent->s.origin, 0.5, v, v);
		VectorSubtract(inflictor->s.origin, v, v);
		points = 0.5f * (damage - 0.5f * VectorLength(v));
		if (points < 0.5)
			points = 0.5;
		if (points > 10)
			points = 10;

		if (points > 0)
		{
			//VectorSubtract (ent->s.origin, inflictor->s.origin, dir);
			ent->client->v_dmg_pitch = -points;
			ent->client->v_dmg_roll = 0;
			ent->client->v_dmg_time = level.time + SHOCK_TIME;
			ent->client->kick_origin[2] = -points * 4;
		}
	}
}

/*
============
T_ShockItems

Lets explosions move items.  Based on T_RadiusDamage.
TODO: Reorient items after coming to rest?
============
*/
void T_ShockItems(edict_t* inflictor)
{
	float	points;
	edict_t* ent = NULL;
	vec3_t	v = { 0 };
	vec3_t	dir = { 0 };
	vec3_t	kvel;
	float	mass;
	float	radius = 255;
	float	damage = 100;

	while ((ent = findradius(ent, inflictor->s.origin, radius)) != NULL)
	{
		if (ent->item
			&& strcmp(ent->classname, "info_player_deathmatch") != 0)
		{
			VectorAdd(ent->mins, ent->maxs, v);
			VectorMA(ent->s.origin, 0.5, v, v);
			VectorSubtract(inflictor->s.origin, v, v);
			points = damage - 0.5f * VectorLength(v);
			if (ent->mass < 25)
				mass = 25;
			else
				mass = ent->mass;

			if (points > 0)
			{
				VectorSubtract(ent->s.origin, inflictor->s.origin, dir);
				ent->movetype = MOVETYPE_BOUNCE;

				// any problem w/leaving this changed?
				VectorScale(dir, 3.0f * points / mass, kvel);
				VectorAdd(ent->velocity, kvel, ent->velocity);
				if (!(ent->svflags & SVF_MONSTER))
					VectorAdd(ent->avelocity, 1.5f * kvel, ent->avelocity);

				//TODO: check groundentity & lower s.origin to keep objects from sticking to floor?
				ent->velocity[2] += 10;

				gi.linkentity(ent);
			}
		}
	}
}

/*
=============
isvisible

This is the ai.c visible function
=============
*/
qboolean isvisible(edict_t* self, edict_t* other)
{
	vec3_t	spot1 = { 0 };
	vec3_t	spot2 = { 0 };
	trace_t	trace;

	VectorCopy(self->s.origin, spot1);
	spot1[2] += self->viewheight;
	VectorCopy(other->s.origin, spot2);
	spot2[2] += other->viewheight;
	trace = gi.trace(spot1, vec3_origin, vec3_origin, spot2, self, MASK_OPAQUE);
	if (trace.fraction == 1.0)
		return true;
	return false;
}



// End dm grenades
