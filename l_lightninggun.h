//
// prototypes for lightning gun functions
//
#ifndef LIGHTNINGGUN_H
#define LIGHTNINGGUN_H

void fire_lightning (edict_t *self, vec3_t start, vec3_t dir, int damage);
void Weapon_LightningGun (edict_t *ent);
void weapon_lightning_fire (edict_t *ent);

#endif
