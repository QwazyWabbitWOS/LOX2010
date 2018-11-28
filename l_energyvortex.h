#ifndef L_ENERGYVORTEX_H
#define L_ENERGYVORTEX_H

//public
void Weapon_EnergyVortex (edict_t *ent);
void fire_energyvortex (edict_t *self, vec3_t start, vec3_t dir, int damage, int speed, float damage_radius);

//private
void energyvortex_explode (edict_t *self);
void energyvortex_active_touch (edict_t *self, edict_t *other, cplane_t *plane, csurface_t *surf);
void energyvortex_think (edict_t *self);
void energyvortex_touch (edict_t *self, edict_t *other, cplane_t *plane, csurface_t *surf);
void weapon_energyvortex_fire (edict_t *ent);

#endif
