// Header file for all rocket types.
// this file contains the prototypes for all the rockets
// Use this with l_rockets.c, l_guidedrocket.c, and l_guidednuke.c
// QwazyWabbit.

#ifndef L_ROCKETS_H
#define L_ROCKETS_H

//public
void RemoveGuided(edict_t *ent);
void GuidedRocket_Think (edict_t *self);
void weapon_grenade_fire (edict_t *ent, qboolean held);
void SpawnExplosion( vec3_t start, edict_t *self, float damage, edict_t *ignore, float radius, int mod);
void fire_homing (edict_t *self, vec3_t start, vec3_t dir, int damage, int speed, float damage_radius, int radius_damage);

//private
void guided_die (edict_t *self, edict_t *inflictor, edict_t *attacker, int damage, vec3_t point);
void GuidedRocket_Explode (edict_t *ent);
void guidedrocket_touch (edict_t *ent, edict_t *other, cplane_t *plane, csurface_t *surf);
void fire_guidedrocket (edict_t *self, vec3_t start, vec3_t dir, 
						int damage, int speed, float damage_radius, int radius_damage, int dM_Type);
void Weapon_GuidedMissile_Fire (edict_t *ent);
void homing_think2 (edict_t *ent) ;
void homing_rethink (edict_t *rocket);
void Weapon_Skipper_Fire (edict_t *ent);
void myhoming_rethink (edict_t *rocket);
void fire_myhoming (edict_t *self, vec3_t start, vec3_t dir, int damage, int speed, float damage_radius, int radius_damage);
void myhoming_touch (edict_t *ent, edict_t *other, cplane_t *plane, csurface_t *surf);
void myhoming_die (edict_t *self, edict_t *inflictor, edict_t *attacker, int damage,vec3_t point);
void myhoming_explode (edict_t *ent);
void myhoming_think2 (edict_t *ent); 
void myhoming_think (edict_t *ent);

#endif

