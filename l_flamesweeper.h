#ifndef L_FLAMESWEEPER_H
#define L_FLAMESWEEPER_H

#define FLAMESWEEPER_SPEED	200
#define FLAMESWEEPER_DAMAGE	6

void flame_think (edict_t *ent);
void PBM_FireFlameThrower(edict_t *self, vec3_t start, vec3_t spread, vec3_t dir, int speed, vec3_t damage, vec3_t radius_damage, int rain_chance, int blast_chance);
void PBM_FlameThrowerThink (edict_t *self);
void PBM_FlameThrowerTouch(edict_t *self, edict_t *other, cplane_t *plane, csurface_t *surf);
void PBM_FireAngleSpread (vec3_t spread, vec3_t dir);
void flame_touch (edict_t *ent, edict_t *other, cplane_t *plane, csurface_t *surf);
void fire_flame (edict_t *self, vec3_t start, vec3_t dir, int damage, int speed);

void Flamethrower_Fire (edict_t *ent);

#endif
