#ifndef L_FREEZEGUN_H
#define L_FREEZEGUN_H

#define FREEZER_DAMAGE 40
#define FREEZER_SPEED 2500

//public
void Weapon_Freezer (edict_t *ent);
void fire_freezer (edict_t *self, vec3_t start, vec3_t dir, int damage, int speed, int effect);

//private
void Freezer_Fire (edict_t *ent, vec3_t g_offset, int damage, qboolean hyper, int effect);
void Weapon_Freezer_Fire (edict_t *ent);
void freezer_touch (edict_t *self, edict_t *other, cplane_t *plane, csurface_t *surf);

#endif
