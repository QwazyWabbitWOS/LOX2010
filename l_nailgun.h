#ifndef NAILGUN_H
#define NAILGUN_H

void fire_nail (edict_t *self, vec3_t start, vec3_t dir, int damage, int speed, int mod);
void spike_touch (edict_t *self, edict_t *other, cplane_t *plane, csurface_t *surf);
void Weapon_Nailgun (edict_t *ent);
void Nailgun_Fire (edict_t *ent);
void Weapon_SuperNailgun (edict_t *ent);
void SuperNailgun_Fire (edict_t *ent);

#endif
