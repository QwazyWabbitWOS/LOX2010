
/***************************************************************************/
/*                    Spiral Rockets MOD                                   */
/***************************************************************************/

#include "g_local.h"
#include "l_angels.h"
#include "l_spiralrocket.h"
#include "x_fbomb.h"

/***********Update Rocket Position every .1 seconds**************/
void mod_SpiralThink(edict_t* ent)
{
	vec3_t newpos, pos = { 0 };

	/*translate current position*/
	VectorSubtract(ent->s.origin, ent->pos2, pos);

	/*So we can rotate around our original line of sight*/
	RotatePointAroundVector(newpos, ent->pos1, pos, 30);

	/*then translate the current position back*/
	VectorAdd(ent->pos2, newpos, ent->s.origin);

	/*The velocity vector must be rotated also*/
	RotatePointAroundVector(ent->velocity, ent->pos1, ent->velocity, 30);

	//Need to think again in .1 seconds
	ent->nextthink = level.time + 0.1f;
}


//***************** fire_rocket2 ********************************************
//* This procedure is basically a carbon copy of fire_rocket(), except for a 
//* few variable initializations. Also, the line of sight of the center rocket
//* is passed in and saved
//***************************************************************************
void mod_fire_rocket2(edict_t* self, vec3_t start, vec3_t dir,
	vec3_t los, int damage, int speed,
	float damage_radius, int radius_damage)
{
	edict_t* rocket;

	rocket = G_Spawn();

	/*Added: Save the line of sight of the center rocket. This is
	the axis around which the other two rockets rotate*/
	VectorCopy(los, rocket->pos1);

	/*Added: Save the start position of the rocket (Not sure if
	this is necessary. This info might already be somewhere else)?*/
	VectorCopy(start, rocket->pos2);
	VectorCopy(start, rocket->s.origin);
	VectorCopy(dir, rocket->movedir);
	vectoangles(dir, rocket->s.angles);
	VectorScale(dir, (float)speed, rocket->velocity);
	rocket->movetype = MOVETYPE_FLYMISSILE;
	rocket->clipmask = MASK_SHOT;
	rocket->solid = SOLID_BBOX;
	rocket->s.effects |= EF_ROCKET;
	VectorClear(rocket->mins);
	VectorClear(rocket->maxs);
	
	//QW// added selection of rocket model
	if(spiral_heads->value)
		rocket->s.modelindex = gi.modelindex("models/objects/gibs/head2/tris.md2");
	else
		rocket->s.modelindex = gi.modelindex("models/objects/rocket/tris.md2");

	rocket->owner = self;
	rocket->touch = firerocket_touch;
	rocket->nextthink = level.time + 0.1f;
	rocket->think = mod_SpiralThink;
	rocket->dmg = damage;
	rocket->radius_dmg = radius_damage;
	rocket->dmg_radius = damage_radius;
	rocket->s.sound = gi.soundindex("weapons/rockfly.wav");

	if (self->client)
		check_dodge(self, rocket->s.origin, dir, speed);

	gi.linkentity(rocket);
}

/*********** Weapon_RocketLauncher_Fire ************************/
/* Modified to launch three rockets, two of which rotate in a  */
/* spiral pattern around the center rocket.                    */
/***************************************************************/

void Weapon_SpiralRocketLauncher_Fire(edict_t* ent)
{
	vec3_t offset = { 0 }, start, forward, right;
	/*Added: Trajectory angle of rocket and line of sight for the center rocket*/
	vec3_t traj_angle = { 0 }, lineofsight = { 0 };
	/*Added: Start speed of the rockets*/
	int startspeed;
	int damage;
	float damage_radius;
	int radius_damage;

	/*Added: Set the rocket speed to taste*/
	startspeed = 600;

	damage = 100 + (int)(random() * 20.0);
	radius_damage = 120;
	damage_radius = 120;
	if (is_quad)
	{
		damage *= 4;
		radius_damage *= 4;
	}

	AngleVectors(ent->client->v_angle, forward, right, NULL);

	VectorScale(forward, -2, ent->client->kick_origin);
	ent->client->kick_angles[0] = -1;

	/*Added: Start of modified launcher code*/
	/*our line of sight is the same as our forward view vector*/
	VectorCopy(forward, lineofsight);
	/*get the trajectory angles for later modification*/
	VectorCopy(ent->client->v_angle, traj_angle);

	/*This is the original launch code*/
	VectorSet(offset, 8, 8, ent->viewheight - 8.0f);
	P_ProjectSource(ent->client, ent->s.origin, offset, forward, right, start);
	mod_fire_rocket2(ent, start, forward, lineofsight, damage, startspeed, damage_radius, radius_damage);
	if (ent->client && ent->client->pers.special == AODEATH)
		mod_fire_rocket2(ent, ent->client->angel->s.origin, forward, lineofsight, damage, startspeed, damage_radius, radius_damage);

	/*Added: Now, modify the trajectory of the second rocket to 5 degrees off of the center axis*/
	traj_angle[1] = ent->client->v_angle[1] + 5;
	AngleVectors(traj_angle, forward, right, NULL);

	VectorSet(offset, 8, 8, ent->viewheight - 8.0f);
	P_ProjectSource(ent->client, ent->s.origin, offset, forward, right, start);
	mod_fire_rocket2(ent, start, forward, lineofsight, damage, startspeed, damage_radius, radius_damage);
	if (ent->client && ent->client->pers.special == AODEATH)
		mod_fire_rocket2(ent, ent->client->angel->s.origin, forward, lineofsight, damage, startspeed, damage_radius, radius_damage);

	/*Added: Now, modify the trajectory again, 5 degrees in the other direction*/
	traj_angle[1] = ent->client->v_angle[1] - 5;
	AngleVectors(traj_angle, forward, right, NULL);

	VectorSet(offset, 8, 8, ent->viewheight - 8.0f);
	P_ProjectSource(ent->client, ent->s.origin, offset, forward, right, start);
	mod_fire_rocket2(ent, start, forward, lineofsight, damage, startspeed, damage_radius, radius_damage);
	if (ent->client && ent->client->pers.special == AODEATH)
		mod_fire_rocket2(ent, ent->client->angel->s.origin, forward, lineofsight, damage, startspeed, damage_radius, radius_damage);
	//Added: End modified launcher code


	// send muzzle flash
	gi.WriteByte(svc_muzzleflash);
	gi.WriteShort(ent - g_edicts);
	gi.WriteByte(MZ_ROCKET | is_silenced);
	gi.multicast(ent->s.origin, MULTICAST_PVS);

	ent->client->ps.gunframe++;

	PlayerNoise(ent, start, PNOISE_WEAPON);

	if ((!((int)dmflags->value & DF_INFINITE_AMMO))
		&& (ent->client->ammo_index))
		ent->client->pers.inventory[ent->client->ammo_index] -= ent->client->pers.weapon->quantity;
	/*enable this if you want your rocket supply to be updated*/
}

void Weapon_SpiralRocketLauncher(edict_t* ent)
{
	static int	pause_frames[] = { 25, 33, 42, 50, 0 };
	static int	fire_frames[] = { 5, 0 };

	Weapon_Generic(ent, 4, 12, 50, 54, pause_frames, fire_frames, Weapon_SpiralRocketLauncher_Fire);
}
