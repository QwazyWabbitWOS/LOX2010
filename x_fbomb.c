/*
This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/
/*
//==========================================================================
//  x_fbomb.c -- by Patrick Martin              Last updated:  2-19-1998
//--------------------------------------------------------------------------
//  This file contains copies of functions found in 'g_weapon.c'.
//  The copied functions are slightly modified so that they can
//  produce flaming clouds upon exploding.
//==========================================================================
*/

#include "g_local.h"
#include "x_fbomb.h"
#include "x_fire.h"

/*
=================
fire_grenade
=================
*/
void FireGrenade_Explode(edict_t* ent)
{
	vec3_t		origin;
	vec3_t		cloud = { 8, 160, 100 };
	vec3_t		timer = { 0, 0.2f, 0 };
	vec3_t		damage = { 6, 9, 25 };
	vec3_t		radius_damage = { 6, 4, 25 };
	int			mod;

	if (ent->owner->client)
		PlayerNoise(ent->owner, ent->s.origin, PNOISE_IMPACT);

	// Set up the means of death.
	mod = MOD_FGRENADE;
	if (i_fragban & WFB_GRENADE)
		mod |= MOD_NOFRAG;

	//FIXME: if we are onground then raise our Z just a bit since we are a point?
	T_RadiusDamage(ent, ent->owner, ent->dmg, NULL, ent->dmg_radius, mod);
	// Flame cloud.
	PBM_FlameCloud(ent->owner, ent->s.origin, cloud, timer, true, 70, damage,
		radius_damage, 100, 75);
	T_ShockWave(ent, 255, 1024);

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

	G_FreeEdict(ent);
}

void FireGrenade_Touch(edict_t* ent, edict_t* other, cplane_t* plane, csurface_t* surf)
{
	if (other == ent->owner)
		return;

	if (surf && (surf->flags & SURF_SKY))
	{
		G_FreeEdict(ent);
		return;
	}

	if (!other->takedamage)
	{
		if (ent->spawnflags & 1)
		{
			if (random() > 0.5)
				gi.sound(ent, CHAN_VOICE, gi.soundindex("weapons/hgrenb1a.wav"), 1, ATTN_NORM, 0);
			else
				gi.sound(ent, CHAN_VOICE, gi.soundindex("weapons/hgrenb2a.wav"), 1, ATTN_NORM, 0);
		}
		else
		{
			gi.sound(ent, CHAN_VOICE, gi.soundindex("weapons/grenlb1b.wav"), 1, ATTN_NORM, 0);
		}
		return;
	}
	FireGrenade_Explode(ent);
}

void fire_flamegrenade(edict_t* self, vec3_t start, vec3_t aimdir, int damage, int speed, float timer, float damage_radius)
{
	edict_t* grenade;
	vec3_t	dir;
	vec3_t	forward, right, up;

	vectoangles(aimdir, dir);
	AngleVectors(dir, forward, right, up);

	grenade = G_Spawn();
	VectorCopy(start, grenade->s.origin);
	VectorScale(aimdir, speed, grenade->velocity);
	VectorMA(grenade->velocity, 200 + crandom() * 10.0f, up, grenade->velocity);
	VectorMA(grenade->velocity, crandom() * 10.0f, right, grenade->velocity);
	VectorSet(grenade->avelocity, 300, 300, 300);
	grenade->movetype = MOVETYPE_BOUNCE;
	grenade->clipmask = MASK_SHOT;
	grenade->solid = SOLID_BBOX;
	grenade->s.effects |= EF_GRENADE;
	VectorClear(grenade->mins);
	VectorClear(grenade->maxs);
	grenade->s.modelindex = gi.modelindex("models/objects/grenade/tris.md2");
	grenade->owner = self;
	grenade->touch = FireGrenade_Touch;
	grenade->nextthink = level.time + timer;
	grenade->think = FireGrenade_Explode;
	grenade->dmg = damage;
	grenade->dmg_radius = damage_radius;
	grenade->classname = "grenade";
	grenade->classnum = CN_GRENADE;

	gi.linkentity(grenade);
}

void fire_flamegrenade2(edict_t* self, vec3_t start, vec3_t aimdir, int damage, int speed, float timer, float damage_radius)
{
	edict_t* grenade;
	vec3_t	dir;
	vec3_t	forward, right, up;

	vectoangles(aimdir, dir);
	AngleVectors(dir, forward, right, up);

	grenade = G_Spawn();
	VectorCopy(start, grenade->s.origin);
	VectorScale(aimdir, speed, grenade->velocity);
	VectorMA(grenade->velocity, 200 + crandom() * 10.0f, up, grenade->velocity);
	VectorMA(grenade->velocity, crandom() * 10.0f, right, grenade->velocity);
	VectorSet(grenade->avelocity, 300, 300, 300);
	grenade->movetype = MOVETYPE_BOUNCE;
	grenade->clipmask = MASK_SHOT;
	grenade->solid = SOLID_BBOX;
	grenade->s.effects |= EF_GRENADE;
	VectorClear(grenade->mins);
	VectorClear(grenade->maxs);
	grenade->s.modelindex = gi.modelindex("models/objects/grenade2/tris.md2");
	grenade->owner = self;
	grenade->touch = FireGrenade_Touch;
	grenade->nextthink = level.time + timer;
	grenade->think = FireGrenade_Explode;
	grenade->dmg = damage;
	grenade->dmg_radius = damage_radius;
	//	grenade->gravity = 0.1f;
	grenade->classname = "grenade";
	grenade->classnum = CN_GRENADE;
	grenade->spawnflags = 1;
	grenade->s.sound = gi.soundindex("weapons/hgrenc1b.wav");

	if (timer <= 0.0)
		FireGrenade_Explode(grenade);
	else
	{
		gi.sound(self, CHAN_WEAPON, gi.soundindex("weapons/hgrent1a.wav"), 1, ATTN_NORM, 0);
		gi.linkentity(grenade);
	}
}


/*
=================
fire_rocket
=================
*/
void firerocket_touch(edict_t* ent, edict_t* other, cplane_t* plane, csurface_t* surf)
{
	vec3_t		origin;
	vec3_t		cloud = { 8, 160, 100 };
	vec3_t		timer = { 0, 0.2f, 0 };
	vec3_t		damage = { 6, 9, 25 };
	vec3_t		radius_damage = { 6, 4, 25 };
	int			n, mod;

	if (other == ent->owner)
		return;

	if (surf && (surf->flags & SURF_SKY))
	{
		G_FreeEdict(ent);
		return;
	}

	if (ent->owner->client)
		PlayerNoise(ent->owner, ent->s.origin, PNOISE_IMPACT);

	// calculate position for the explosion entity
	VectorMA(ent->s.origin, -0.02f, ent->velocity, origin);

	if (other->takedamage)
	{
		// Set up the means of death.
		mod = MOD_ROCKET;
		if (i_fragban & WFB_ROCKETLAUNCHER)
			mod |= MOD_NOFRAG;

		T_Damage(other, ent, ent->owner, ent->velocity, ent->s.origin,
			plane->normal, ent->dmg, 0, 0, mod);
	}
	else
	{
		// don't throw any debris in net games
		if (!deathmatch->value)
		{
			if ((surf) && !(surf->flags & (SURF_WARP | SURF_TRANS33 | SURF_TRANS66 | SURF_FLOWING)))
			{
				n = rand() % 5;
				while (n--)
					ThrowDebris(ent, "models/objects/debris2/tris.md2", 2, ent->s.origin);
			}
		}
	}

	mod = MOD_R_SPLASH;
	if (i_fragban & WFB_ROCKETLAUNCHER)
		mod |= MOD_NOFRAG;
	T_RadiusDamage(ent, ent->owner, ent->radius_dmg, other, ent->dmg_radius,
		mod);

	// Flame cloud.
	PBM_FlameCloud(ent->owner, ent->s.origin, cloud, timer, true, 70, damage, radius_damage, 100, 75);

	gi.WriteByte(svc_temp_entity);
	if (ent->waterlevel)
		gi.WriteByte(TE_ROCKET_EXPLOSION_WATER);
	else
		gi.WriteByte(TE_ROCKET_EXPLOSION);
	gi.WritePosition(origin);
	gi.multicast(ent->s.origin, MULTICAST_PVS);

	G_FreeEdict(ent);
}

void fire_flamerocket(edict_t* self, vec3_t start, vec3_t dir, int damage, int speed, float damage_radius, int radius_damage)
{
	edict_t* rocket;

	rocket = G_Spawn();
	VectorCopy(start, rocket->s.origin);
	VectorCopy(dir, rocket->movedir);
	vectoangles(dir, rocket->s.angles);
	VectorScale(dir, speed, rocket->velocity);
	rocket->movetype = MOVETYPE_FLYMISSILE;
	rocket->clipmask = MASK_SHOT;
	rocket->solid = SOLID_BBOX;
	rocket->s.effects |= EF_ROCKET;
	VectorClear(rocket->mins);
	VectorClear(rocket->maxs);
	rocket->s.modelindex = gi.modelindex("models/objects/rocket/tris.md2");
	rocket->owner = self;
	rocket->touch = firerocket_touch;
	rocket->nextthink = level.time + 8000.0f / speed;
	rocket->think = G_FreeEdict;
	rocket->dmg = damage;
	rocket->radius_dmg = radius_damage;
	rocket->dmg_radius = damage_radius;
	rocket->s.sound = gi.soundindex("weapons/rockfly.wav");

	if (self->client)
		check_dodge(self, rocket->s.origin, dir, speed);

	gi.linkentity(rocket);
}
