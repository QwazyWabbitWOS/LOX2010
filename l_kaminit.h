
#ifndef L_KAMINIT_H
#define L_KAMINIT_H

//public
void Weapon_Kaminit (edict_t *ent);
void fire_kaminit (edict_t *self, vec3_t start, vec3_t aimdir, int damage, int kick);

//private
void weapon_kaminit_fire (edict_t *ent);

#endif
