
#ifndef L_TRACKER_H
#define L_TRACKER_H

#define TRK_DAMAGE	120
#define TRK_SPEED	600
#define TRK_LIFETIME	60.0f	//tracker max lifetime

void Tracker_Think (edict_t *ent);
void Tracker_Fire (edict_t *self, vec3_t start, vec3_t aimdir, int damage, int speed, float damage_radius);
void Tracker_Touch (edict_t *ent, edict_t *other, cplane_t *plane, csurface_t *surf);
void Tracker_Weapon_Fire (edict_t *ent);

#endif
