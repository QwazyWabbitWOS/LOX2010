/*==========================================================================
//  x_fire.h -- by Patrick Martin               Last updated:  2-19-1998
//--------------------------------------------------------------------------
//  This is the header file for x_fire.c.  Any files that use the
//  functions in x_fire.c must include this file.
//========================================================================*/

#ifndef XFIRE_H
#define XFIRE_H

/***************/
/*  CONSTANTS  */
/***************/

/* Time in seconds the entity on fire will burn. */
#define BURN_TIME   30

/* Important frames in my flame model. */
#define FRAME_FIRST_SMALLFIRE   0
#define FRAME_LAST_SMALLFIRE    5
#define FRAME_FIRST_LARGEFIRE   6
#define FRAME_LAST_LARGEFIRE   15
#define FRAME_FLAMEOUT         16
/* Important frames in my flame model. */
#define FRAME_FIRST_SMALLIGNITE   0
#define FRAME_FIRST_LARGEIGNITE  15
#define SKIN_FIRE_RED   0
#define SKIN_FIRE_BLUE  1
#define RADIUS_FIRE_SPLASH   80

#define FIREFLAG_SHIFT_Z        0x000000ff
#define FIREFLAG_DOWN           0x00000100
#define FIREFLAG_DELTA_ALIVE    0x00000200
#define FIREFLAG_DELTA_BASE     0x00000400
#define FIREFLAG_DELTA_VIEW     0x00000800
#define FIREFLAG_IGNITE         0x00001000

/* Path to my flame model.  (Used to be "models/fire/tris.md2".) */
//#define MD2_FIRE  gi.modelindex("fire.md2")
#define MD2_FIRE  gi.modelindex("models/fire/tris.md2")

/****************/
/*  PROTOTYPES  */
/****************/

/*
// g_misc.c
*/
void BecomeExplosion2 (edict_t *self);

/*
// x_fire.c
*/
void     PBM_BecomeSmoke (edict_t *self);
void     PBM_BecomeSteam (edict_t *self);
void     PBM_SmallExplodeThink (edict_t *self);
void     PBM_BecomeNewExplosion (edict_t *self);

qboolean PBM_InWater (edict_t *ent);
qboolean PBM_Flammable (edict_t *ent);
qboolean PBM_FireResistant (edict_t *ent);
void     PBM_BurnDamage (edict_t *victim, edict_t *fire, vec3_t damage);
void     PBM_BurnRadius (edict_t *fire, float radius, vec3_t damage, edict_t *ignore);
void     PBM_FireSpot (vec3_t spot, edict_t *ent);
qboolean PBM_FlameOut (edict_t *self);
void     PBM_Burn (edict_t *self);
void     PBM_Ignite (edict_t *victim, edict_t *attacker);

void     PBM_CheckFire (edict_t *self);
void     PBM_FireDropTouch (edict_t *self, edict_t *other, cplane_t *plane, csurface_t *surf);
void     PBM_FireDrop (edict_t *self);

void     PBM_CloudBurst (edict_t *self);
void     PBM_FlameCloud (edict_t *attacker, vec3_t start, vec3_t cloud, vec3_t timer,
						 qboolean deadly, float radius, vec3_t damage, vec3_t radius_damage,
						 int rain_chance, int blast_chance);

void     PBM_FireballTouch (edict_t *self, edict_t *other, cplane_t *plane, csurface_t *surf);
void     PBM_FireFlamer (edict_t *self, vec3_t start, vec3_t dir, int speed, float radius,
						 vec3_t damage, vec3_t radius_damage, int rain_chance, int blast_chance);
void PBM_FireSpot2 (vec3_t spot, edict_t *ent);
void LPBM_BurnDamage(edict_t *victim, edict_t *fire, vec3_t point, vec3_t damage, int dflags, int mod);
void PBM_EasyFireDrop (edict_t *self);
void LPBM_Ignite (edict_t *victim, edict_t *attacker, vec3_t point);
void LPBM_FireDrop(edict_t *attacker, vec3_t spot, vec3_t damage, vec3_t radius_damage, int blast_chance);
qboolean LPBM_FireResistant (edict_t *ent, vec3_t point);
qboolean PBM_Inflammable (edict_t *ent);
qboolean PBM_ActivePowerArmor (edict_t *ent, vec3_t point);
void PBM_FlashExplosion(edict_t *attacker, vec3_t p1, vec3_t p2, float damage, vec3_t radii, edict_t *ignore, int mod);
void PBM_FlashEffects (vec3_t p1, vec3_t p2);
void PBM_FlashRadius(edict_t *attacker, vec3_t origin, float damage, vec3_t radii, edict_t *ignore, int mod);
qboolean PBM_CanDamageFromPoint (edict_t *targ, vec3_t point);
qboolean PBM_NegateMissile (edict_t *ent);
void PBM_FlameCloud2(edict_t *attacker, vec3_t start, vec3_t cloud, vec3_t timer, qboolean deadly, vec3_t damage, vec3_t radius_damage, int rain_chance, int blast_chance);
void PBM_Ignite2 (edict_t *victim, edict_t *attacker, vec3_t point);
/*=============================  END OF FILE =============================*/

#endif
