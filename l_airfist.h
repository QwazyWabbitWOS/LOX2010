#ifndef AIRFIST_H
#define AIRFIST_H

void weapon_airfist_fire (edict_t *ent);
void fire_airfist (edict_t *self, vec3_t start, vec3_t aimdir, int inDamage, int strength, float inRange, float recoil);
qboolean horn_infront (edict_t *self, edict_t *other);
edict_t *horn_findradius (edict_t *from, vec3_t org, float rad);

#endif
