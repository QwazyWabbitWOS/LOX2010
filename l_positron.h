#ifndef POSITRON_H
#define POSITRON_H

void PositronBeamFire(edict_t *ent, vec3_t start, vec3_t dir);
void weapon_positron_fire (edict_t *ent);
void MakePositron (edict_t *maker, edict_t *ignore, vec3_t origin, float delay);
void Positron_Exp (edict_t *self);
void Positron_Think (edict_t *self);
void Shake (edict_t *self);

#endif
