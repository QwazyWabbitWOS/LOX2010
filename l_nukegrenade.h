//
// prototypes of nuke grenade functions
//
#ifndef NUKE_G_H
#define NUKE_G_H

void Weapon_NukeGrenade(edict_t *ent);
void weapon_nukegrenade_fire (edict_t *ent, qboolean held);
void fire_nukegrenade2 (edict_t *self, vec3_t start, vec3_t aimdir, int damage, int speed, float timer, float damage_radius, qboolean held);
void NukeGrenade_Explode (edict_t *ent);
void ThrowShrapnel4 (edict_t *self, char *modelname, float speed, vec3_t origin);
void ThrowShrapnel3 (edict_t *self, char *modelname, float speed, vec3_t origin);
//Unused void Cluster_SM_Explode (edict_t *ent);
//Unused void Flashbang_Hurt (edict_t *self);
void NukeGrenade_Touch (edict_t *ent, edict_t *other, cplane_t *plane, csurface_t *surf);
void WeirdLights(edict_t *ent);
void NormalLights(edict_t *ent);
void fire_fire (edict_t *self, vec3_t start, vec3_t aimdir, int damage, int speed, float timer, float damage_radius, qboolean held);
void Fire_Touch (edict_t *ent, edict_t *other, cplane_t *plane, csurface_t *surf);
//Unused void Fire_Burn_U(edict_t* ent); // Unused
void Fire_Explode (edict_t *ent);

#endif
