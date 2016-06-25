#ifndef FBFG_H
#define FBFG_H

void fbfg_explode (edict_t *self);
void fbfg_touch (edict_t *self, edict_t *other, cplane_t *plane, csurface_t *surf);
void fbfg_think (edict_t *self);
void fire_fbfg (edict_t *self, vec3_t start, vec3_t dir, int damage, int speed, float damage_radius);
void weapon_fbfg_fire (edict_t *ent);

#endif
