#ifndef PULSE_H
#define PULSE_H

void weapon_pulserifle_fire (edict_t *ent);
void fire_pulserifle (edict_t *self, vec3_t start, vec3_t dir, int damage, int speed, int effect);
void pulserifle_touch (edict_t *self, edict_t *other, cplane_t *plane, csurface_t *surf);
void pulserifle_sparks (edict_t *self);

#endif
