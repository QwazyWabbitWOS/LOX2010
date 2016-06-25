//*********************
// NUKE GRENADE CODE
//*********************
//
#include "g_local.h"
#include "m_player.h"
#include "l_dm_grenades.h"
#include "l_nukegrenade.h"

void Weapon_NukeGrenade (edict_t *ent)
{
	if ((ent->client->newweapon) && (ent->client->weaponstate == WEAPON_READY))
	{
		ChangeWeapon (ent);
		return;
	}
	
	if (ent->client->weaponstate == WEAPON_ACTIVATING)
	{
		ent->client->weaponstate = WEAPON_READY;
		ent->client->ps.gunframe = 16;
		return;
	}
	
	if (ent->client->weaponstate == WEAPON_READY)
	{
		if ( ((ent->client->latched_buttons|ent->client->buttons) & BUTTON_ATTACK) )
		{
			ent->client->latched_buttons &= ~BUTTON_ATTACK;
			if (ent->client->pers.inventory[ent->client->ammo_index])
			{
				ent->client->ps.gunframe = 1;
				ent->client->weaponstate = WEAPON_FIRING;
				ent->client->grenade_time = 0;
			}
			else
			{
				if (level.time >= ent->pain_debounce_time)
				{
					gi.sound(ent, CHAN_VOICE, gi.soundindex("weapons/noammo.wav"), 1, ATTN_NORM, 0);
					ent->pain_debounce_time = level.time + 1;
				}
				NoAmmoWeaponChange (ent);
			}
			return;
		}
		
		if ((ent->client->ps.gunframe == 29) || (ent->client->ps.gunframe == 34) || (ent->client->ps.gunframe == 39) || (ent->client->ps.gunframe == 48))
		{
			if (rand()&15)
				return;
		}
		
		if (++ent->client->ps.gunframe > 48)
			ent->client->ps.gunframe = 16;
		return;
	}
	
	if (ent->client->weaponstate == WEAPON_FIRING)
	{
		if (ent->client->ps.gunframe == 5)
			gi.sound(ent, CHAN_WEAPON, gi.soundindex("weapons/hgrena1b.wav"), 1, ATTN_NORM, 0);
		
		if (ent->client->ps.gunframe == 11)
		{
			if (!ent->client->grenade_time)
			{
				ent->client->grenade_time = level.time + 30 + 0.2f;
				ent->client->weapon_sound = gi.soundindex("weapons/hgrenc1b.wav");
			}
			
			// they waited too long, detonate it in their hand
			if (!ent->client->grenade_blew_up && level.time >= ent->client->grenade_time)
			{
				ent->client->weapon_sound = 0;
				weapon_nukegrenade_fire (ent, QTRUE);
				ent->client->grenade_blew_up = QTRUE;
			}
			
			if (ent->client->buttons & BUTTON_ATTACK)
				return;
			
			if (ent->client->grenade_blew_up)
			{
				if (level.time >= ent->client->grenade_time)
				{
					ent->client->ps.gunframe = 15;
					ent->client->grenade_blew_up = QFALSE;
				}
				else
				{
					return;
				}
			}
		}
		
		if (ent->client->ps.gunframe == 12)
		{
			ent->client->weapon_sound = 0;
			weapon_nukegrenade_fire (ent, QFALSE);
		}
		
		if ((ent->client->ps.gunframe == 15) && (level.time < ent->client->grenade_time))
			return;
		
		ent->client->ps.gunframe++;
		
		if (ent->client->ps.gunframe == 16)
		{
			ent->client->grenade_time = 0;
			ent->client->weaponstate = WEAPON_READY;
		}
	}
}

void weapon_nukegrenade_fire (edict_t *ent, qboolean held)
{
	vec3_t	offset;
	vec3_t	forward, right;
	vec3_t	start;
	int		damage = 5000;
	float	timer;
	int		speed;
	float	radius;

	radius = 1000;
/*	if (is_quad)
		damage *= 4;*/

	VectorSet(offset, 8, 8, ent->viewheight-8.0f);
	AngleVectors (ent->client->v_angle, forward, right, NULL);
	P_ProjectSource (ent->client, ent->s.origin, offset, forward, right, start);

	timer = ent->client->grenade_time - level.time;
	speed = 500;
	fire_nukegrenade2 (ent, start, forward, damage, speed, timer, radius, held);

	if (! ( (int)dmflags->value & DF_INFINITE_AMMO ) )
		ent->client->pers.inventory[ent->client->ammo_index]--;

	ent->client->grenade_time = level.time + 1.0f;

	if (ent->deadflag || ent->s.modelindex != 255) // VWep animations screw up corpses
	{
		return;
	}

	if (ent->health <= 0)
		return;

	if (ent->client->ps.pmove.pm_flags & PMF_DUCKED)
	{
		ent->client->anim_priority = ANIM_ATTACK;
		ent->s.frame = FRAME_crattak1-1;
		ent->client->anim_end = FRAME_crattak3;
	}
	else
	{
		ent->client->anim_priority = ANIM_REVERSE;
		ent->s.frame = FRAME_wave08;
		ent->client->anim_end = FRAME_wave01;
	}
}

void fire_nukegrenade2 (edict_t *self, vec3_t start, vec3_t aimdir, int damage, int speed, float timer, float damage_radius, qboolean held)
{
	edict_t	*grenade;
	vec3_t	dir;
	vec3_t	forward, right, up;

	vectoangles (aimdir, dir);
	AngleVectors (dir, forward, right, up);

	grenade = G_Spawn();
	VectorCopy (start, grenade->s.origin);
	VectorScale (aimdir, (float)speed, grenade->velocity);
	VectorMA (grenade->velocity, 200 + crandom() * 10.0f, up, grenade->velocity);
	VectorMA (grenade->velocity, crandom() * 10.0f, right, grenade->velocity);
	VectorSet (grenade->avelocity, 300, 300, 300);
	grenade->movetype = MOVETYPE_BOUNCE;
	grenade->clipmask = MASK_SHOT;
	grenade->solid = SOLID_BBOX;
	grenade->s.effects = EF_GRENADE | EF_SPINNINGLIGHTS | EF_PENT;
	//grenade->s.renderfx |= RF_SHELL_RED;
	VectorClear (grenade->mins);
	VectorClear (grenade->maxs);
	grenade->s.modelindex = gi.modelindex ("models/objects/grenade2/tris.md2");
	grenade->owner = self;
	grenade->touch = NukeGrenade_Touch;
	grenade->nextthink = level.time + timer;
	grenade->think = NukeGrenade_Explode;
	grenade->dmg = damage;
	grenade->dmg_radius = damage_radius;
	grenade->classname = "nukegrenade";
	if (held)
		grenade->spawnflags = 3;
	else
		grenade->spawnflags = 1;
	grenade->s.sound = gi.soundindex("weapons/nuketick.wav");

	if (timer <= 0.0)
		NukeGrenade_Explode (grenade);
	else
	{
		gi.sound (self, CHAN_WEAPON, gi.soundindex ("weapons/hgrent1a.wav"), 1, ATTN_NORM, 0);
		gi.linkentity (grenade);
	}
}

//EARTHQUAKE GENERATOR
static void SpawnQuake(edict_t *ent, int counts, int speeds)
{
	edict_t		*EarthQuake;

	EarthQuake = G_Spawn();
	EarthQuake->svflags |= SVF_NOCLIENT;
	EarthQuake->think = target_earthquake_think;
	EarthQuake->nextthink = level.time + 0.1f;
	EarthQuake->use = target_earthquake_use;
	EarthQuake->noise_index = gi.soundindex ("world/quake.wav");
	EarthQuake->count = 10;
	EarthQuake->speed = (float)speeds;
	target_earthquake_use(EarthQuake,ent->owner,ent->owner);
}


//************************
//  NUKE GRENADE EXPLODE
//************************
void NukeGrenade_Explode (edict_t *ent)
{
	vec3_t		origin;
	int			mod;
	edict_t		*LightMesser;
	int			i;


	if (ent->owner->client)
		PlayerNoise(ent->owner, ent->s.origin, PNOISE_IMPACT);

	if (ent->spawnflags & 2)
		mod = MOD_HELD_NUKE;
	else
		mod = MOD_NUKE_BLAST;

	T_RadiusDamage(ent, ent->owner, (float)ent->dmg, ent->enemy, ent->dmg_radius, mod);

	VectorMA (ent->s.origin, -0.02f, ent->velocity, origin);
	gi.WriteByte (svc_temp_entity);
	gi.WriteByte (TE_BOSSTPORT);
	gi.WritePosition (origin);
	gi.multicast (ent->s.origin, MULTICAST_PHS);

		for (i = 0;i < 4;i++)
		{
			ThrowShrapnel4(ent,"models/objects/debris2/tris.md2",crandom() * 10,ent->s.origin);
			ThrowShrapnel3(ent,"models/objects/debris2/tris.md2",crandom() * 10,ent->s.origin);
		}

		
		/*
	EarthQuake->classname="target_earthquake";
	EarthQuake->think = target_earthquake_use;
	EarthQuake->nextthink = level.time + .1;*/


	//for (i = 0;i < 5;i++)
	//{
	SpawnQuake(ent,(int)random()*7+2,150);
	//}

	LightMesser = G_Spawn();
	LightMesser->svflags = SVF_NOCLIENT;
	LightMesser->think = WeirdLights;
	LightMesser->nextthink = level.time + 0.1f;

	G_FreeEdict (ent);
}

//*************
// LAUNCH FIRE
//*************

void fire_fire (edict_t *self, vec3_t start, vec3_t aimdir, int damage, int speed, float timer, float damage_radius, qboolean held)
{
	edict_t	*grenade;
	vec3_t	dir;
	vec3_t	forward, right, up;

	vectoangles (aimdir, dir);
	AngleVectors (dir, forward, right, up);

	grenade = G_Spawn();
	VectorCopy (start, grenade->s.origin);
	VectorScale (aimdir, (float)speed, grenade->velocity);
	VectorMA (grenade->velocity, 200 + crandom() * 10.0f, up, grenade->velocity);
	VectorMA (grenade->velocity, crandom() * 10.0f, right, grenade->velocity);
	VectorSet (grenade->avelocity, 300, 300, 300);
	grenade->movetype = MOVETYPE_BOUNCE;
	grenade->clipmask = MASK_SHOT;
	grenade->solid = SOLID_BBOX;
	grenade->s.effects = EF_FLAG1; //EF_GRENADE | EF_BLASTER;
	VectorClear (grenade->mins);
	VectorClear (grenade->maxs);
	grenade->s.modelindex = gi.modelindex ("models/fire/tris.md2");
	grenade->owner = self;
	grenade->touch = Fire_Touch;
	grenade->nextthink = level.time + timer;
	grenade->think = Fire_Explode;
//	grenade->nextthink2 = level.time + .3;
//	grenade->think2 = Fire_Burn_U;
	grenade->dmg = damage;
	grenade->dmg_radius = damage_radius;
	grenade->classname = "fire";

	if (timer <= 0.0)
		Fire_Explode (grenade);
	else
	{
		gi.linkentity (grenade);
	}
}

//************
// TOUCH FIRE
//************

void Fire_Touch (edict_t *ent, edict_t *other, cplane_t *plane, csurface_t *surf)
{
	vec3_t   grenade1;
	
	if (surf && (surf->flags & SURF_SKY))
	{
		G_FreeEdict (ent);
		return;
	}

	if (!other->takedamage)
		if (crandom() < .4)
		{
			VectorSet(grenade1,20,20,crandom()*40+30);
			fire_fire(ent->owner, ent->s.origin, grenade1, 90, 5, crandom()*2, 120, QFALSE);
			if (crandom() < .2)
				Fire_Explode (ent);
		}
	
	ent->enemy = other;
	Fire_Explode (ent);

		return;

}

//********************
// FIRE (NOT GRENADE)
//********************

void Fire_Explode (edict_t *ent)
{
	vec3_t		origin;
	int			mod;

	mod = MOD_FIRE_SPLASH;
	//FIXME: if we are onground then raise our Z just a bit since we are a point?
	if (ent->enemy)
	{
		float	points;
		vec3_t	v;
		vec3_t	dir;

		VectorAdd (ent->enemy->mins, ent->enemy->maxs, v);
		VectorMA (ent->enemy->s.origin, 0.5f, v, v);
		VectorSubtract (ent->s.origin, v, v);
		points = ent->dmg - 0.5f * VectorLength (v);
		VectorSubtract (ent->enemy->s.origin, ent->s.origin, dir);
		T_Damage (ent->enemy, ent, ent->owner, dir, ent->s.origin, vec3_origin, (int)points, (int)points, DAMAGE_RADIUS, mod);
	}
	T_RadiusDamage(ent, ent->owner, (float)ent->dmg, ent->enemy, ent->dmg_radius, mod);

	VectorMA (ent->s.origin, -0.02f, ent->velocity, origin);
	gi.WriteByte (svc_temp_entity);
	gi.WriteByte (TE_EXPLOSION2);
	gi.WritePosition (origin);
	gi.multicast (ent->s.origin, MULTICAST_PHS);

	G_FreeEdict (ent);
}

//**********************
//  FIRE RADII DAMAGE
//**********************
/*
static void Fire_Burn_U (edict_t *ent)
{
	if (ent->waterlevel)
		ent->think = G_FreeEdict;

	T_RadiusDamage(ent,ent->owner,50,NULL,100,MOD_FIRE_SPLASH);
	ent->nextthink = level.time + crandom();
}
*/

//***********************
// SMALL FIRE (FOR NUKE)
//***********************
/*
void Cluster_SM_Explode (edict_t *ent)

{
	vec3_t		origin;

	//Sean added these 4 vectors

	vec3_t   grenade1;
	vec3_t   grenade2;
	vec3_t   grenade3;

	int mod = MOD_NUKE_BLAST;

	//FIXME: if we are onground then raise our Z just a bit since we are a point?
	T_RadiusDamage(ent, ent->owner, 70, NULL, 100,mod);

	VectorMA (ent->s.origin, -0.02f, ent->velocity, origin);
	gi.WriteByte (svc_temp_entity);
	if (ent->waterlevel)
	{
		if (ent->groundentity)
			gi.WriteByte (TE_GRENADE_EXPLOSION_WATER);
		else
			gi.WriteByte (TE_ROCKET_EXPLOSION_WATER);
	}
	else
	{
		if (ent->groundentity)
			gi.WriteByte (TE_GRENADE_EXPLOSION);
		else
			gi.WriteByte (TE_ROCKET_EXPLOSION);
	}
	gi.WritePosition (origin);
	gi.multicast (ent->s.origin, MULTICAST_PVS);

	// SumFuka did this bit : give grenades up/outwards velocities
	VectorSet(grenade1,20,20,random()*20+30);
	VectorSet(grenade2,20,-20,random()*20+30);
	VectorSet(grenade3,-20,20,random()*20+30);

	// Sean : explode the four grenades outwards
	fire_fire(ent->owner, origin, grenade1, 90, 10, random()*4+1, 120,QFALSE);
	fire_fire(ent->owner, origin, grenade2, 90, 10, random()*4+1, 120,QFALSE);
	fire_fire(ent->owner, origin, grenade3, 94, 10, random()*4+1, 120,QFALSE);

	 G_FreeEdict (ent);
}
*/

//*********************
//  NUKE FLASH HURTS
//*********************
/*
void Flashbang_Hurt (edict_t *self)
{
	edict_t	*ent;
	edict_t	*ignore;
	vec3_t	point;
	vec3_t	dir;
	int		dmg;

	dmg = 6;

	ent = NULL;
	while ((ent = findradius(ent, self->s.origin, 2048)) != NULL)
	{
		if (ent == self)
			continue;

		if (!visible(self, ent))
			continue;

		if (!ent->takedamage)
			continue;

		if (!(ent->svflags & SVF_MONSTER) && (!ent->client) && (strcmp(ent->classname, "misc_explobox") != 0))
			continue;

		VectorMA (ent->absmin, 0.5, ent->size, point);

		VectorSubtract (point, self->s.origin, dir);
		VectorNormalize (dir);

		ignore = self;
		T_Damage (ent, self, self->owner, dir, self->s.origin, vec3_origin, dmg, 1, DAMAGE_ENERGY, MOD_NUKE_BLAST);

	}

	self->nextthink = level.time + FRAMETIME;

}
*/

void NukeGrenade_Touch (edict_t *ent, edict_t *other, cplane_t *plane, csurface_t *surf)
{
	if (other == ent->owner)
		return;

	if (surf && (surf->flags & SURF_SKY))
	{
		G_FreeEdict (ent);
		return;
	}

	if (!other->takedamage)
	{
		if (ent->spawnflags & 1)
		{
			if (random() > 0.5)
				gi.sound (ent, CHAN_VOICE, gi.soundindex ("weapons/hgrenb1a.wav"), 1, ATTN_NORM, 0);
			else
				gi.sound (ent, CHAN_VOICE, gi.soundindex ("weapons/hgrenb2a.wav"), 1, ATTN_NORM, 0);
		}
		else
		{
			gi.sound (ent, CHAN_VOICE, gi.soundindex ("weapons/grenlb1b.wav"), 1, ATTN_NORM, 0);
		}
		return;
	}

}

void WeirdLights(edict_t *ent)
{
	       // 0 normal
        //gi.configstring(CS_LIGHTS+0, "alzmzmciduauqwjdnianzznian");
		gi.configstring(CS_LIGHTS+0, "zzzzzzzzzzzzzzzzzzzzzzzzzz");
		        
        // 1 FLICKER (first variety)
        gi.configstring(CS_LIGHTS+1, "lainfusuaodnwjuauznsiufima");
        
        // 2 SLOW STRONG PULSE
        gi.configstring(CS_LIGHTS+2, "znfisuypaiqmainvbusifnzjdu");
        
        // 3 CANDLE (first variety)
        gi.configstring(CS_LIGHTS+3, "rkdldisuemanxjzurkoamdiogu");
        
        // 4 FAST STROBE
        gi.configstring(CS_LIGHTS+4, "ahsbexcbkxerswaibldcwersxa");
        
        // 5 GENTLE PULSE 1
        gi.configstring(CS_LIGHTS+5, "disoerodmditusndifiwismdso");
        
        // 6 FLICKER (second variety)
        gi.configstring(CS_LIGHTS+6, "sjigibuieminzzunaiqaainfem");
        
        // 7 CANDLE (second variety)
        gi.configstring(CS_LIGHTS+7, "alzmzmciduauqwjdnianzznian");
        
        // 8 CANDLE (third variety)
        gi.configstring(CS_LIGHTS+8, "aaunxumdmzuiznianianunfsoi");
        
        // 9 SLOW STROBE (fourth variety)
        gi.configstring(CS_LIGHTS+9, "adeiamzmzwimfoaoncsonasuvi");
        
        // 10 FLUORESCENT FLICKER
        gi.configstring(CS_LIGHTS+10, "ahsbexcbkxerswaibldcwersxa");

        // 11 SLOW PULSE NOT FADE TO BLACK
        gi.configstring(CS_LIGHTS+11, "ahsbexcbkxerswaibldcwersxa");
        
		ent->nextthink = level.time + 10;
		ent->think = NormalLights;
}

void NormalLights(edict_t *ent)
{
		// 0 normal
	gi.configstring(CS_LIGHTS+0, "m");
	
	// 1 FLICKER (first variety)
	gi.configstring(CS_LIGHTS+1, "mmnmmommommnonmmonqnmmo");
	
	// 2 SLOW STRONG PULSE
	gi.configstring(CS_LIGHTS+2, "abcdefghijklmnopqrstuvwxyzyxwvutsrqponmlkjihgfedcba");
	
	// 3 CANDLE (first variety)
	gi.configstring(CS_LIGHTS+3, "mmmmmaaaaammmmmaaaaaabcdefgabcdefg");
	
	// 4 FAST STROBE
	gi.configstring(CS_LIGHTS+4, "mamamamamama");
	
	// 5 GENTLE PULSE 1
	gi.configstring(CS_LIGHTS+5,"jklmnopqrstuvwxyzyxwvutsrqponmlkj");
	
	// 6 FLICKER (second variety)
	gi.configstring(CS_LIGHTS+6, "nmonqnmomnmomomno");
	
	// 7 CANDLE (second variety)
	gi.configstring(CS_LIGHTS+7, "mmmaaaabcdefgmmmmaaaammmaamm");
	
	// 8 CANDLE (third variety)
	gi.configstring(CS_LIGHTS+8, "mmmaaammmaaammmabcdefaaaammmmabcdefmmmaaaa");
	
	// 9 SLOW STROBE (fourth variety)
	gi.configstring(CS_LIGHTS+9, "aaaaaaaazzzzzzzz");
	
	// 10 FLUORESCENT FLICKER
	gi.configstring(CS_LIGHTS+10, "mmamammmmammamamaaamammma");

	// 11 SLOW PULSE NOT FADE TO BLACK
	gi.configstring(CS_LIGHTS+11, "abcdefghijklmnopqrrqponmlkjihgfedcba");
	
	G_FreeEdict(ent);
}
