#ifndef FREEZATRON_H
#define FREEZATRON_H

void FreezatronBeamFire(edict_t *ent, vec3_t start, vec3_t dir);
void weapon_freezatron_fire (edict_t *ent);
void MakeFreezatron (edict_t *maker, edict_t *ignore, vec3_t origin, float delay);
void Freezatron_Exp (edict_t *self);
void Freezatron_Think (edict_t *self);
void Shake (edict_t *self);

#endif
