/**************************************************/
/*                 Trackers                       */
/**************************************************/

#include "g_local.h"

#include "l_tracker.h"

#include "l_airfist.h"
#include "l_bucky.h"
#include "l_chunkgun.h"
#include "l_disintegrator.h"
#include "l_durg.h"
#include "l_energyvortex.h"
#include "l_fbfg.h"
#include "l_flamesweeper.h"
#include "l_flaregun.h"
#include "l_freezatron.h"
#include "l_freezegun.h"
#include "l_kaminit.h"
#include "l_lightninggun.h"
#include "l_mace.h"
#include "l_nailgun.h"
#include "l_positron.h"
#include "l_pulserifle.h"
#include "l_stickinggrenades.h"
#include "x_fbomb.h"
#include "x_fire.h"

// darKMajick definitions
#include "l_dm_grenades.h"

void Tracker_Think (edict_t *ent)
{
	vec3_t targetdir, forward, blipdir, v;
	edict_t *blip = NULL;
	float blipDot, targetDot;
	edict_t *owner = NULL;

	// trackers think every 2 seconds
	ent->nextthink = level.time + 2.0f;
	
	// clean up edicts if he has no trackers out
	if (ent->owner->client->tracker == 0) // gets zeroed when player respawns
	{
		ent->dm_type = DM_NORMALGRENADE;
		Grenade_Explode_dM (ent); //explode them
//		G_FreeEdict(ent);		// or just delete them
		return;
	}
	
	// trackerstarttime is set at deployment to be level.time + TRK_LIFETIME
	// when level time reaches this time, blow up the tracker.
	if (ent->trackerstarttime <= level.time)
	{
		ent->owner->client->tracker--;
		ent->dm_type = DM_NORMALGRENADE;
		Grenade_Explode_dM (ent);
		return;
	}
	
	// if tracker runs out of ammo, it dies.
	if (ent->trackerammo == 0)
	{
		ent->owner->client->tracker--;
		ent->dm_type = DM_NORMALGRENADE;
		Grenade_Explode_dM (ent);
		return;
	}
	
	if (ent->enemy != NULL)
	{
		VectorSubtract (ent->enemy->s.origin, ent->s.origin, targetdir);
		targetdir[2] += 16;
		VectorNormalize (targetdir);
		VectorScale (targetdir, 2.0, targetdir);
		VectorAdd (targetdir, ent->movedir, targetdir); 
		VectorNormalize (targetdir);
		VectorCopy (targetdir, ent->movedir); 
		vectoangles (targetdir, ent->s.angles);
	}

	// Ha ha, just joking, you don't really think we would allow the tracker to 
	// fire on its owner, right? 
	if (ent->enemy != NULL && ent->enemy == ent->owner)
		ent->enemy = NULL;

	if (ent->enemy != NULL)
	{ 	
		// if target is on same team as owner, we're done looking at him
		if (OnSameTeam(ent->owner, ent->enemy))
		{
			ent->nextthink = level.time + FRAMETIME;
			return;
		}

		// the fun begins, the enemy gets shot at
		ent->trackerammo--;
		ent->enemy = NULL;
		ent->nextthink = level.time + 0.5f;	
		VectorCopy(ent->movedir,forward);
		v[PITCH] = ent->movedir[PITCH];
		
		// trackerammotype contains the pickup_name of the weapon 
		// the user was holding when he deployed the tracker.
		// We use it here to determine the fire function.
		
		if (Q_strcasecmp (ent->trackerammotype, "Blaster") == 0) 
			fire_blaster (ent->owner, ent->s.origin, ent->movedir, 15, 1000, EF_BLASTER /* , hyper */);
		else if (Q_strcasecmp (ent->trackerammotype, "Mace") == 0) 
			fire_mace (ent->owner, ent->s.origin, ent->movedir, 2, 150, 200, 0);
		else if (Q_strcasecmp (ent->trackerammotype, "Flaregun") == 0) 
			fire_flaregun (ent->owner, ent->s.origin, ent->movedir, 0, 1000);
		else if (Q_strcasecmp (ent->trackerammotype, "Airfist") == 0) 
            fire_airfist (ent->owner, ent->s.origin, ent->movedir, 2, 150, 400, 300);
		else if (Q_strcasecmp (ent->trackerammotype, "Shotgun") == 0) 
			fire_shotgun (ent->owner, ent->s.origin, ent->movedir, 4, 8, 500, 500, DEFAULT_DEATHMATCH_SHOTGUN_COUNT, MOD_SHOTGUN);
		else if (Q_strcasecmp (ent->trackerammotype, "Super Shotgun") == 0)
		{
			v[YAW]   = ent->movedir[YAW] - 4;
			v[ROLL]  = ent->movedir[ROLL];
			AngleVectors (v, forward, NULL, NULL);
			fire_shotgun (ent->owner, ent->s.origin, forward, 4, 8, 500, 500, DEFAULT_DEATHMATCH_SHOTGUN_COUNT, MOD_SHOTGUN);
			v[YAW]   = ent->movedir[YAW] + 4;
			AngleVectors (v, forward, NULL, NULL);
			fire_shotgun (ent->owner, ent->s.origin, forward, 4, 8, 500, 500, DEFAULT_DEATHMATCH_SHOTGUN_COUNT, MOD_SHOTGUN);
		}
		else if (Q_strcasecmp (ent->trackerammotype, "Standard Machinegun") == 0) 
			fire_bullet (ent->owner, ent->s.origin, ent->movedir, 8, 2, DEFAULT_BULLET_HSPREAD, DEFAULT_BULLET_VSPREAD, MOD_MACHINE);
		else if (Q_strcasecmp (ent->trackerammotype, "Nailgun") == 0) 
			fire_nail (ent->owner, ent->s.origin, ent->movedir, 18, 1000, MOD_NAIL);
		else if (Q_strcasecmp (ent->trackerammotype, "Pulserifle") == 0) 
			fire_pulserifle (ent->owner, ent->s.origin, ent->movedir, 30, 500, EF_GRENADE);
		else if (Q_strcasecmp (ent->trackerammotype, "Machinegun") == 0) 
			fire_mr (ent->owner, ent->s.origin, ent->movedir, (15 + (int)(random() * 10.0)), 1000, 20, (15 + (int)(random() * 10.0)));
		else if (Q_strcasecmp (ent->trackerammotype, "Freezer") == 0) 
			fire_freezer (ent->owner, ent->s.origin, ent->movedir, FREEZER_DAMAGE, FREEZER_SPEED, EF_BLASTER);
		else if (Q_strcasecmp (ent->trackerammotype, "Chaingun") == 0) 
			fire_bullet (ent->owner, ent->s.origin, ent->movedir, 8, 2, DEFAULT_BULLET_HSPREAD, DEFAULT_BULLET_VSPREAD, MOD_CHAINGUN);
		else if (Q_strcasecmp (ent->trackerammotype, "Streetsweeper") == 0) 
			fire_shotgun (ent->owner, ent->s.origin, ent->movedir, 4, 8, 500, 500, DEFAULT_DEATHMATCH_SHOTGUN_COUNT, MOD_STREETSWEEP);
		else if (Q_strcasecmp (ent->trackerammotype, "SuperBlastersweeper") == 0) 
			fire_super (ent->owner, ent->s.origin, ent->movedir, 50, 1000, EF_BLASTER /* , hyper */);
		else if (Q_strcasecmp (ent->trackerammotype, "Grenadesweeper") == 0) 
			fire_flamegrenade (ent->owner, ent->s.origin, ent->movedir, 120, 600, 2.5, 50);		
		else if (Q_strcasecmp (ent->trackerammotype, "Chunkgun") == 0) 
			fire_chunk (ent->owner, ent->s.origin, ent->movedir, 10, 500, MOD_PLAGUE);
		else if (Q_strcasecmp (ent->trackerammotype, "Freezersweeper") == 0) 
			fire_freezer (ent->owner, ent->s.origin, ent->movedir, FREEZER_DAMAGE, FREEZER_SPEED, EF_BLASTER);
		else if (Q_strcasecmp (ent->trackerammotype, "Flamesweeper") == 0) 
			fire_flame (ent->owner, ent->s.origin, ent->movedir, FLAMESWEEPER_DAMAGE, /*speed*/ FLAMESWEEPER_SPEED);
		else if (Q_strcasecmp (ent->trackerammotype, "Bazookasweeper") == 0) 
			fire_grenade_dM (ent->owner, ent->s.origin, ent->movedir, 120, 500, 2.5, 50, 0, /* held */ QFALSE, /* bazookad */ QTRUE);		
		else if (Q_strcasecmp (ent->trackerammotype, "Railgunsweeper") == 0) 
			fire_rail (ent->owner, ent->s.origin, ent->movedir, 150, 8);
		else if (Q_strcasecmp (ent->trackerammotype, "Super Rail Shotgun") == 0) 
			fire_rail (ent->owner, ent->s.origin, ent->movedir, 150, 8);
		else if (Q_strcasecmp (ent->trackerammotype, "Antimatter Cannon") == 0) 
			fire_rail (ent->owner, ent->s.origin, ent->movedir, 150, 8);
		else if (Q_strcasecmp (ent->trackerammotype, "Disruptor") == 0) 
			fire_Disintegrator (ent->owner, ent->s.origin, ent->movedir, DIS_DAMAGE, DIS_SPEED, ent->enemy);
		else if (Q_strcasecmp (ent->trackerammotype, "Kaminit") == 0) 
			fire_kaminit (ent->owner, ent->s.origin, ent->movedir, /*damage*/0, /*kick*/ 0);
		else if (Q_strcasecmp (ent->trackerammotype, "Grenade Launcher") == 0) 
			fire_grenade_dM (ent->owner, 
				ent->s.origin, 
				ent->movedir, 
				120, //damage
				500, //speed
				2.5, //timer
				50,  //damage radius
				ent->dm_type, //type
				QFALSE, // held
				QFALSE); // bazookad		
		else if (Q_strcasecmp (ent->trackerammotype, "Sticking Grenades") == 0) 
			fire_stickinggrenade (ent->owner, ent->s.origin, ent->movedir, 120, 600, 2.5, 40);
		else if (Q_strcasecmp (ent->trackerammotype, "Buckyball Launcher") == 0) 
			fire_buckyball (ent->owner, ent->s.origin, ent->movedir, BUCKY_DAMAGE, BUCKY_SPEED, 90);
		else if (Q_strcasecmp (ent->trackerammotype, "Bazooka") == 0) 
			fire_grenade_dM (ent->owner, 
				ent->s.origin, 
				ent->movedir, 
				120, //damage
				500, //speed
				2.5, //timer
				50,  //damage radius
				ent->dm_type, //type
				QFALSE, // held
				QTRUE); // bazookad
		else if (Q_strcasecmp (ent->trackerammotype, "Double Impact") == 0)
		{
			v[YAW]   = ent->movedir[YAW] - 6;
			v[ROLL]  = ent->movedir[ROLL];
			AngleVectors (v, forward, NULL, NULL);
			fire_grenade_dM (ent->owner, 
				ent->s.origin, 
				forward, 
				120, //damage
				500, //speed
				2.5, //timer
				50,  //damage radius
				ent->dm_type, //type
				QFALSE, //held
				QTRUE); //bazookad
			v[YAW]   = ent->movedir[YAW] + 6;
			AngleVectors (v, forward, NULL, NULL);
			fire_grenade_dM (ent->owner, 
				ent->s.origin, 
				forward, 
				120, 
				500, 
				2.5, 
				50, 
				ent->dm_type, 
				/* held */ QFALSE, 
				/* bazookad */ QTRUE);		
		}
		else if (Q_strcasecmp (ent->trackerammotype, "Rocket Launcher") == 0)
			fire_flamerocket (ent->owner, ent->s.origin, ent->movedir, (100 + (int)(random() * 20.0)), 650, 120, 120);
		else if (Q_strcasecmp (ent->trackerammotype, "Lightning Gun") == 0)
			fire_lightning (ent->owner, ent->s.origin, ent->movedir, 30);
		else if (Q_strcasecmp (ent->trackerammotype, "Positron") == 0)
		{
			PositronBeamFire(ent->owner, ent->s.origin, ent->movedir);
			ent->nextthink = level.time + 1.0f;	
		}
		else if (Q_strcasecmp (ent->trackerammotype, "Freezatron") == 0)
		{
			FreezatronBeamFire(ent->owner, ent->s.origin, ent->movedir);
			ent->nextthink = level.time + 1.0f;	
		}
		else if (Q_strcasecmp (ent->trackerammotype, "HyperBlaster") == 0)
			fire_blaster (ent->owner, ent->s.origin, ent->movedir, 15, 1000, EF_HYPERBLASTER /* , hyper */);
		else if (Q_strcasecmp (ent->trackerammotype, "Icecube Gun") == 0)
			fire_freezer (ent->owner, ent->s.origin, ent->movedir, FREEZER_DAMAGE, FREEZER_SPEED, EF_BLASTER);
		else if (Q_strcasecmp (ent->trackerammotype, "Super Blaster") == 0)
			fire_super (ent->owner, ent->s.origin, ent->movedir, 50, 1000, EF_BLASTER /* , hyper */);
		else if (Q_strcasecmp (ent->trackerammotype, "Railgun2") == 0)
			fire_rail (ent->owner, ent->s.origin, ent->movedir, 150, 8);
		else if (Q_strcasecmp (ent->trackerammotype, "Wall Piercing Railgun") == 0)
			fire_rail (ent->owner, ent->s.origin, ent->movedir, 150, 8);
		else if (Q_strcasecmp (ent->trackerammotype, "Railgun") == 0)
		{
			vec3_t temp = {6, 9, 25};
			vec3_t temp2 = {6, 4, 25};
			PBM_FireFlamer (ent->owner, ent->s.origin, ent->movedir, 1200, 70, temp ,temp2 , 100, 50);
		}
		else if (Q_strcasecmp (ent->trackerammotype, "Durg") == 0)
			fire_durg (ent->owner, ent->s.origin, ent->movedir, 0, 2);		
		else if (Q_strcasecmp (ent->trackerammotype, "Energy Vortex") == 0)
			fire_energyvortex (ent->owner, ent->s.origin, ent->movedir, 200, 500, 1000);
		else if (Q_strcasecmp (ent->trackerammotype, "BFG10K") == 0)
			fire_bfg (ent->owner, ent->s.origin, ent->movedir, 200, 400, 1000);
		else if (Q_strcasecmp (ent->trackerammotype, "FBFG") == 0)
			fire_fbfg (ent->owner, ent->s.origin, ent->movedir, 200, 400, 1000);
		else if (Q_strcasecmp (ent->trackerammotype, "Sniper Gun") == 0)
			fire_sniper (ent->owner, ent->s.origin, ent->movedir, 300, 2500, EF_BLASTER);
		else if (Q_strcasecmp (ent->trackerammotype, "Snipersweeper") == 0)
			fire_sniper (ent->owner, ent->s.origin, ent->movedir, 300, 2500, EF_BLASTER);
		else if (Q_strcasecmp (ent->trackerammotype, "Plasma Rifle") == 0)
			fire_plasma (ent->owner, ent->s.origin, ent->movedir, 15, 1200, EF_BFG);
		
		else if (Q_strcasecmp (ent->trackerammotype, "Explosive Chaingun") == 0)
			fire_explosive_bullet (ent->owner, ent->s.origin, ent->movedir, 6, 7, 300,500, MOD_EXPLOSIVECHAINGUN);
		else if (Q_strcasecmp (ent->trackerammotype, "Explosive Super Shotgun") == 0)
		{
			v[YAW]   = ent->movedir[YAW] - 4;
			v[ROLL]  = ent->movedir[ROLL];
			AngleVectors (v, forward, NULL, NULL);
			fire_explosive_shotgun (ent->owner, ent->s.origin, forward, 4, 8,EF_BLASTER, 500, 500, DEFAULT_DEATHMATCH_SHOTGUN_COUNT/2, MOD_EXPLOSIVESSHOTGUN);
			v[YAW]   = ent->movedir[YAW] + 4;
			AngleVectors (v, forward, NULL, NULL);
			fire_explosive_shotgun (ent->owner, ent->s.origin, forward, 4, 8,EF_BLASTER, 500, 500, DEFAULT_DEATHMATCH_SHOTGUN_COUNT/2, MOD_EXPLOSIVESSHOTGUN);
		}
		
		else if (Q_strcasecmp (ent->trackerammotype, "Explosive Shotgun") == 0)
			fire_explosive_shotgun (ent->owner, ent->s.origin, ent->movedir, 4, 8,TE_BLASTER, 500, 500, DEFAULT_DEATHMATCH_SHOTGUN_COUNT, MOD_EXPLOSIVESHOTGUN);
		else if (Q_strcasecmp (ent->trackerammotype, "Explosive Streetsweeper") == 0)
			fire_explosive_shotgun (ent->owner, ent->s.origin, ent->movedir, 4, 8,TE_BLASTER, 500, 500, DEFAULT_DEATHMATCH_SHOTGUN_COUNT, MOD_EXPLOSIVESTREETSWEEPER);
	}
	
	blip = NULL;
	targetDot = 0;
	while ((blip = findradius (blip, ent->s.origin, 1024)) != NULL)
	{
		// See if this is the kind of blip we can home in on.
		// more or less with most-likely events first.
		if (blip->cloaked)	continue;
		if (blip->health <= 0)	continue;
		
		if (blip == ent->owner)
		{ 
			owner = blip;
			continue;
		}
		
		if (!visible (ent,blip))
			continue;
		if (!blip->takedamage)
			continue;
		if (blip->oldmodel)
			continue;
		if (!(blip->svflags & SVF_MONSTER) && !blip->client)
			continue;
		
		if ((blip->client == NULL) || (blip->client->invisible)) 
			continue; //QW// decoys have null client ID's so ignore them
		
		// Determine where the blip is in relation to us.
		VectorSubtract (blip->s.origin, ent->s.origin, blipdir);
		blipdir[2] += 16;
		
		// Determine how "good" of a target it is.  (Currently, that's the
		// enemy that's most being aimed at, regardless of distance.)
		VectorNormalize (blipdir);
		blipDot = DotProduct (targetdir, blipdir);
		
		// Remember the "best" target so far.
		if (ent->enemy == NULL || targetDot < blipDot)
		{
			ent->enemy = blip;
			targetDot = blipDot;
		}
	}
	
	// if there are no enemies to find and the owner was found then look at the 
	// owner.
	if (ent->enemy == NULL && owner != NULL)
		ent->enemy = owner;
}

void Tracker_Fire (edict_t *self, vec3_t start, vec3_t aimdir, int damage, int speed, float damage_radius)
{
	edict_t	*grenade;
	vec3_t	dir, forward, right, up;
	
	// trackers have low damage tolerance
	damage = 0;		//QW//
	vectoangles (aimdir, dir);
	AngleVectors (dir, forward, right, up);
	
	grenade = G_Spawn();
	VectorCopy (start, grenade->s.origin);
	VectorScale (aimdir, (float) speed, grenade->velocity);
	VectorMA (grenade->velocity, 200 + crandom() * 10.0f, up, grenade->velocity);
	VectorMA (grenade->velocity, crandom() * 10.0f, right, grenade->velocity);
	VectorSet (grenade->avelocity, 300, 300, 300);
	grenade->movetype = MOVETYPE_BOUNCE;
	grenade->movetype |= MOVETYPE_FLYMISSILE;
	grenade->clipmask = MASK_SHOT;
	grenade->health = 100; // Hard to target but easy to Kill 
	grenade->max_health= 100; 
	grenade->solid = SOLID_BBOX;
	grenade->s.effects |= EF_GRENADE;
	VectorClear (grenade->mins);
	VectorClear (grenade->maxs);
	grenade->s.modelindex = gi.modelindex ("models/objects/gibs/head2/tris.md2");
	grenade->owner = self;
	grenade->touch = NULL;
	grenade->nextthink = level.time + FRAMETIME; //was .000001;
	grenade->think = Tracker_Think;
	grenade->dmg = damage;
	grenade->dmg_radius = damage_radius;
	grenade->classname = "Tracker";
	grenade->classnum = CN_TRACKER;
	grenade->trackerammo = 50;
	grenade->dm_type = self->client->dM_grenade;
	grenade->trackerstarttime = level.time + TRK_LIFETIME;
	grenade->trackerammotype = self->client->pers.weapon->pickup_name;
	grenade->takedamage = DAMAGE_YES;
	gi.linkentity (grenade);
	speed = 0;		//keep this at the end of the function
}

//deploy up to (max_trackers) trackers
void Tracker_Weapon_Fire (edict_t *ent)
{
	vec3_t	start,forward, right, offset;
	int		damage = TRK_DAMAGE;
	float	radius;
	
	assert(ent->client->pers.weapon->pickup_name != NULL);
	// trap bad grenade types if enum is malformed
	assert(ent->client->dM_grenade >= DM_NORMALGRENADE && ent->client->dM_grenade <= DM_SPINNINGRAILBOMB);

	// Weapons we never want to see used in trackers.
	if (Q_strcasecmp(ent->client->pers.weapon->pickup_name,"Guided Missile")==0) return;
	else if (Q_strcasecmp(ent->client->pers.weapon->pickup_name,"Sword")==0) return;
	else if (Q_strcasecmp(ent->client->pers.weapon->pickup_name,"Flaregun")==0) return;
	else if (Q_strcasecmp(ent->client->pers.weapon->icon,"a_grenades")==0) return;
	else if (Q_strcasecmp(ent->client->pers.weapon->pickup_name,"Vacuum Maker")==0) return;
	else if (Q_strcasecmp(ent->client->pers.weapon->pickup_name,"Spiral Rocket Launcher")==0) return;
	else if (Q_strcasecmp(ent->client->pers.weapon->pickup_name,"Homing Rocket Launcher")==0) return;
	else if (Q_strcasecmp(ent->client->pers.weapon->pickup_name,"Skipper Homing Rocket")==0) return;
	else if (Q_strcasecmp(ent->client->pers.weapon->pickup_name,"Teleport Grenade")==0) return;
	else if (Q_strcasecmp(ent->client->pers.weapon->pickup_name,"Floating Mine Launcher")==0) return;

	// Grenades we never want to see used in trackers
	else if (ent->client->dM_grenade == DM_TELEGRENADE) return;
	else if (ent->client->dM_grenade == DM_MIDNIGHTGRENADE) return;
	else if (ent->client->dM_grenade == DM_BANZAIGRENADE) return;

	//START_PERFORMANCE_TIMER; //test with loxtrackerban = 0
		
	// configurable tracker banning
	if ((Q_strcasecmp(ent->client->pers.weapon->pickup_name,"BFG10K")==0) &&
		((i_loxtrackerban & LTB_BFG10K))) return;
	if ((Q_strcasecmp(ent->client->pers.weapon->pickup_name,"Buckyball Launcher")==0) &&
		((i_loxtrackerban & LTB_BUCKY))) return;
	if ((Q_strcasecmp(ent->client->pers.weapon->pickup_name,"Disruptor")==0) &&
		((i_loxtrackerban & LTB_DISINTEGRATOR))) return;
	if ((Q_strcasecmp(ent->client->pers.weapon->pickup_name,"Double Impact")==0) &&
		((i_loxtrackerban & LTB_DOUBLEIMPACT))) return;
	if ((Q_strcasecmp(ent->client->pers.weapon->pickup_name,"Durg")==0) &&
		((i_loxtrackerban & LTB_DURG))) return;
	if ((Q_strcasecmp(ent->client->pers.weapon->pickup_name,"Energy Vortex")==0) &&
		((i_loxtrackerban & LTB_ENERGYVORTEX))) return;
	if ((Q_strcasecmp(ent->client->pers.weapon->pickup_name,"Explosive Shotgun")==0) &&
		((i_loxtrackerban & LTB_EXPLOSIVESHOTGUN))) return;
	if ((Q_strcasecmp(ent->client->pers.weapon->pickup_name,"Explosive Super Shotgun")==0) &&
		((i_loxtrackerban & LTB_EXPLOSIVESUPERSHOTGUN))) return;
	if ((Q_strcasecmp(ent->client->pers.weapon->pickup_name,"Explosive Machinegun")==0) &&
		((i_loxtrackerban & LTB_EXPLOSIVEMACHINEGUN))) return;
	if ((Q_strcasecmp(ent->client->pers.weapon->pickup_name,"FBFG")==0) &&
		((i_loxtrackerban & LTB_FBFG))) return;
	if ((Q_strcasecmp(ent->client->pers.weapon->pickup_name,"Freezer")==0) &&
		((i_loxtrackerban & LTB_FREEZEGUN))) return;
	if ((Q_strcasecmp(ent->client->pers.weapon->pickup_name,"Icecube Gun")==0) &&
		((i_loxtrackerban & LTB_ICECUBEGUN))) return;
	if ((Q_strcasecmp(ent->client->pers.weapon->pickup_name,"Kaminit")==0) &&
		((i_loxtrackerban & LTB_KAMINIT))) return;
	if ((Q_strcasecmp(ent->client->pers.weapon->pickup_name,"Lightning Gun")==0) &&
		((i_loxtrackerban & LTB_LIGHTNINGGUN))) return;
	if ((Q_strcasecmp(ent->client->pers.weapon->pickup_name,"Nailgun")==0) &&
		((i_loxtrackerban & LTB_NAILGUN))) return;
	if ((Q_strcasecmp(ent->client->pers.weapon->pickup_name,"Positron")==0) &&
		((i_loxtrackerban & LTB_POSITRON))) return;
	if ((Q_strcasecmp(ent->client->pers.weapon->pickup_name,"Freezatron")==0) &&
		((i_loxtrackerban & LTB_FREEZATRON))) return;
	if ((Q_strcasecmp(ent->client->pers.weapon->pickup_name,"Pulserifle")==0) &&
		((i_loxtrackerban & LTB_PULSERIFLE))) return;
	if ((Q_strcasecmp(ent->client->pers.weapon->pickup_name,"Railgun")==0) &&
		((i_loxtrackerban & LTB_RAILGUN))) return;
	if ((Q_strcasecmp(ent->client->pers.weapon->pickup_name,"Sticking Grenades")==0) &&
		((i_loxtrackerban & LTB_STICKGRENADELAUNCHER))) return;
	if ((Q_strcasecmp(ent->client->pers.weapon->pickup_name,"Sniper Gun")==0) &&
		((i_loxtrackerban & LTB_SNIPERGUN))) return;
	if ((Q_strcasecmp(ent->client->pers.weapon->pickup_name,"Snipersweeper")==0) &&
		((i_loxtrackerban & LTB_SNIPERGUN))) return;

	//STOP_PERFORMANCE_TIMER(__func__);

	// passed all that, time to launch one
	if (max_trackers->value > 10) gi.cvar_set("max_trackers", "10");  //set a reasonable hard coded limit.
	if (max_trackers->value < 1) gi.cvar_set("max_trackers", "1");  //supposed to use wepban to disable trackers
	if (ent->client->tracker >= max_trackers->value)	// configurable soft limit from 1 to 10, default 3.
	{
		gi.cprintf(ent,PRINT_HIGH, "Too many trackers activated\n");
		return;
	}
	
	radius = damage + 40.0f;
	if (is_quad)
		damage *= 4;
	
	VectorSet(offset, 8, 8, ent->viewheight - 8.0f);
	AngleVectors (ent->client->v_angle, forward, right, NULL);
	P_ProjectSource (ent->client, ent->s.origin, offset, forward, right, start);
	VectorScale (forward, -2, ent->client->kick_origin);
	ent->client->kick_angles[0] = -1;
	ent->client->tracker++;
	Tracker_Fire(ent, ent->s.origin, forward, damage, TRK_SPEED, radius);

	gi.WriteByte (svc_muzzleflash);
	gi.WriteShort (ent-g_edicts);
	gi.WriteByte (MZ_GRENADE | is_silenced);
	gi.multicast (ent->s.origin, MULTICAST_PVS);

	PlayerNoise(ent, start, PNOISE_WEAPON);

	if (! ((int)dmflags->value & DF_INFINITE_AMMO))
		ent->client->pers.inventory[ent->client->ammo_index] -= 2;

}
