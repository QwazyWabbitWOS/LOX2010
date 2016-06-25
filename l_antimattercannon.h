#ifndef ANTIMATTER_H
#define ANTIMATTER_H

void weapon_asha_fire (edict_t *ent);
void fire_asha  (edict_t *self, vec3_t start, vec3_t aimdir, int damage, int kick, int hspread, int vspread, int mod);

void BombExplosion (edict_t *self);

#endif