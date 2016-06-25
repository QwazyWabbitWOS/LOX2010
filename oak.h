/*************************************************
*
* Module:		oak.h
*
* Author:		John Crickett
*				john@crickett.demon.co.uk
*
* Date:			13/12/97
*
* Version:		0.0
*
*
* Module description:
* -------------------
*
* Oak header file
*
*
* Version History:
* ----------------
*
* 0.0 - initial release
*
* Added to LOX by QwazyWabbit
* Lots of cleanup from 'demo' version
*
************************************************/
#ifndef OAK_H
#define OAK_H

/************************************************
*
* define constants
*
************************************************/

#define OAK_FIND_RANGE 700.00

#define OAK_WALK	16
#define OAK_RUN		36

/************************************************
*
* Prototypes
*
************************************************/

//
// oak.c 
//
void OAK_Check_SP(edict_t *ent);
void SP_Oak_f(edict_t *owner);	//QW// give them the owner's skin
void OAK_Respawn(edict_t *self);

//
// oakai.c
//
void oak_stand(edict_t *self);
void oak_run(edict_t *self);
void oak_standclose(edict_t *self);
void oak_sight(edict_t *self, edict_t *other);

void oak_pain(edict_t *self, edict_t *other, float kick, int damage);
void oak_die(edict_t *self, edict_t *inflictor, edict_t *attacker, int damage, vec3_t point);
void oak_painthink(edict_t *self);
void oak_fire (edict_t *self, int flash_number);
void oak_fire1 (edict_t *self);
void oak_attack(edict_t *self);


void OakAI_FaceEnemy(edict_t *self);
void OakAI_MoveToEnemy(edict_t *self, float dist);
void OakAI_MoveToGoal (edict_t *ent, float dist);

void OakAI_RunFrames(edict_t *self, int start, int end);
void OakAI_Point(edict_t *self);
void OakAI_Finger(edict_t *self);
void OakAI_Salute(edict_t *self);
void OakAI_Taunt(edict_t *self);
void OakAI_Wave(edict_t *self);

static int sound_pain;
static int sound_die;
static int sound_idle;
static int sound_punch;
static int sound_sight;
static int sound_search;
static int sound_sight1;
static int sound_sight2;
static int sound_death;


/************************************************
*
* 
*
************************************************/

#endif
