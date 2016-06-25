
#ifndef L_BUCKY_H
#define L_BUCKY_H

#define BUCKY_DAMAGE 50
#define BUCKY_SPEED 1000

void Weapon_Bucky_Fire (edict_t *ent);
void fire_buckyball (edict_t *self, vec3_t start, vec3_t aimdir, int damage, int speed, float damage_radius);
void BuckyBall_Think (edict_t *ent);
void BuckyBall_OnGroundThink (edict_t *ent);
void BuckyBall_Touch (edict_t *ent, edict_t *other, cplane_t *plane, csurface_t *surf);
void BuckyBall_Explode (edict_t *ent);

#endif
