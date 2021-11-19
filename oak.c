/*************************************************
*
* Module:		oak.c
*
* Author:		John Crickett
*				john@crickett.demon.co.uk
*
* Date:			12/12/97
*
* Version:		0.0
*
*
* Module description:
* -------------------
*
* Oak main code (spawning etc)
*
*
* Version History:
* ----------------
*
* 0.0 - intitial release
*
* Added to LOX by QwazyWabbit
* Lots of cleanup from 'demo' version
*
************************************************/

/************************************************
*
* Includes
*
************************************************/

#include "g_local.h"
#include "oak.h"

/************************************************
*
* Function:	Oak_Check_SP
*
* Description: checks whether it is ok to spawn a oak
*
* Arguments:
* void
*
* Returns:
* void
*
*************************************************/

void OAK_Check_SP(edict_t* ent)
{
	// only spawn a bot in deathmatch
	if (deathmatch->value)
	{
		SP_Oak_f(ent);
	}
	else
	{
		gi.cprintf(ent, PRINT_HIGH, "Sorry Oak II is a deathmatch only bot.\n");
	}
}

/************************************************
*
* Function:	SP_Oak_f
*
* Description: spawns an oak bot
*
* Arguments: pointer to the bot owner
*
*
* Returns:
* void
*
*************************************************/

void SP_Oak_f(edict_t* owner)
{
	edict_t* ent;	// the bot entity
	vec3_t	spawn_origin, spawn_angles;

	if (deathmatch->value)
	{
		// spawn the bot on a spawn point
		ent = G_Spawn();
		SelectSpawnPoint(ent, spawn_origin, spawn_angles);
		VectorCopy(spawn_origin, ent->s.origin);
		ent->s.origin[2] += 1;	// make sure off ground

		//Use same model and skin as the person creating decoy
		ent->model = owner->model;
		ent->s.skinnum = owner->s.skinnum;
		ent->s.modelindex = owner->s.modelindex;
		ent->s.modelindex2 = owner->s.modelindex2;
		ent->s.modelindex3 = owner->s.modelindex3;
		ent->s.modelindex4 = owner->s.modelindex4;
		ent->s.number = ent - g_edicts;

		ent->classname = "bot";
		ent->takedamage = DAMAGE_AIM;
		ent->movetype = MOVETYPE_STEP;
		ent->mass = 200;
		ent->solid = SOLID_BBOX;
		ent->deadflag = DEAD_NO;
		ent->clipmask = MASK_PLAYERSOLID;
		ent->s.frame = 0;
		ent->waterlevel = 0;
		ent->watertype = 0;
		ent->health = 100;
		ent->max_health = 100;
		ent->gib_health = -40;
		/* FIXME err we need to set these!!!
		ent->max_bullets = 200;
		ent->max_shells = 100;
		ent->max_rockets = 50;
		ent->max_grenades = 50;
		ent->max_cells = 200;
		ent->max_slugs = 50;
		ent->inuse = true;
		*/

		// think functions
		ent->pain = oak_pain;
		ent->die = oak_die;
		ent->monsterinfo.stand = oak_stand;
		ent->monsterinfo.attack = oak_attack;
		ent->monsterinfo.sight = oak_sight;
		ent->monsterinfo.run = oak_run;

		//FIXME need to add these
		ent->monsterinfo.walk = NULL;
		ent->monsterinfo.dodge = NULL;
		ent->monsterinfo.melee = NULL;
		ent->monsterinfo.idle = NULL;

		//Set up sounds
		oak_sound_idle = gi.soundindex("soldier/solidle1.wav");
		oak_sound_sight1 = gi.soundindex("soldier/solsght1.wav");
		oak_sound_sight2 = gi.soundindex("soldier/solsrch1.wav");
		oak_sound_pain = gi.soundindex("soldier/solpain1.wav");
		oak_sound_death = gi.soundindex("misc/keyuse.wav");
		gi.soundindex("soldier/solatck1.wav");

		ent->monsterinfo.idle_time = 0;

		ent->think = oak_stand;
		ent->nextthink = level.time + 0.1;

		ent->yaw_speed = 20;
		ent->s.angles[PITCH] = 0;
		ent->s.angles[YAW] = spawn_angles[YAW];
		ent->s.angles[ROLL] = 0;

		VectorSet(ent->mins, -16, -16, -32);
		VectorSet(ent->maxs, 16, 16, 32);
		VectorClear(ent->velocity);

		KillBox(ent);

		gi.linkentity(ent);
		gi.bprintf(PRINT_HIGH, "An Oak bot has entered the game\n");
	}
	else
	{
		gi.cprintf(owner, PRINT_HIGH, "Sorry Oak II is a deathmatch only bot.\n");
	}
}

void OAK_Respawn(edict_t* self)
{
	vec3_t	spawn_origin, spawn_angles;

	// spawn the bot on a spawn spot
	self->s.event = EV_OTHER_TELEPORT;	//prevent lerping
	SelectSpawnPoint(self, spawn_origin, spawn_angles);
	VectorCopy(spawn_origin, self->s.origin);
	self->s.origin[2] += 1;	// make sure off ground

	self->deadflag = DEAD_NO;
	self->takedamage = DAMAGE_AIM;

	self->model = "players/male/tris.md2";
	self->s.modelindex = 255;
	self->s.modelindex2 = 255;		// custom gun model
	self->s.frame = 0;
	self->waterlevel = 0;
	self->watertype = 0;
	self->health = 100;
	self->max_health = 100;
	self->gib_health = -40;

	self->pain = oak_pain;
	self->die = oak_die;
	self->monsterinfo.stand = oak_stand;

	VectorSet(self->mins, -16, -16, -32);
	VectorSet(self->maxs, 16, 16, 32);
	VectorClear(self->velocity);

	self->think = oak_stand;
	self->nextthink = level.time + 0.1;

	KillBox(self);

	gi.linkentity(self);
	gi.bprintf(PRINT_HIGH, "A Oak bot has respawned\n");

}

