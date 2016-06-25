#ifndef MACE_H
#define MACE_H

void weapon_mace_fire (edict_t *ent);
void fire_mace (edict_t *self, vec3_t start, vec3_t aimdir, int inDamage, int strength, float inRange, float recoil);
qboolean mace_horn_infront (edict_t *self, edict_t *other);
edict_t *mace_horn_findradius (edict_t *from, vec3_t org, float rad);

#endif
