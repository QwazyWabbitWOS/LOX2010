
#ifndef L_TURRET_H
#define L_TURRET_H

#define TRT_DAMAGE	120
#define TRT_SPEED	600
#define TRT_LIFETIME	60.0f	//turret max lifetime

void Turret_Think (edict_t *ent);
void Turret_Fire (edict_t *self, vec3_t start, vec3_t aimdir, int damage, int speed, float damage_radius);
void Turret_Touch (edict_t *ent, edict_t *other, cplane_t *plane, csurface_t *surf);
void Turret_Stick(edict_t *ent, edict_t *other);
void Turret_Weapon_Fire (edict_t *ent);

#endif
