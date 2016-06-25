#ifndef DISINTEGRATOR_H
#define DISINTEGRATOR_H

#define TRACKER_DAMAGE_FLAGS	(DAMAGE_NO_POWER_ARMOR | DAMAGE_ENERGY | DAMAGE_NO_KNOCKBACK)
#define TRACKER_IMPACT_FLAGS	(DAMAGE_NO_POWER_ARMOR | DAMAGE_ENERGY)
#define TRACKER_DAMAGE_TIME		1.0f		// seconds (was 0.5)

#define	DIS_DAMAGE 175		// originally 100
#define DIS_SPEED 1000

// constants for the Weapon_Generic call
#define DIS_ACTIVATE_LAST 5
#define DIS_FIRE_LAST	20
#define DIS_IDLE_LAST	49
#define DIS_DEACTIVATE_LAST	53

//public
void Weapon_Disintegrator (edict_t *ent);
void fire_Disintegrator (edict_t *self, vec3_t start, vec3_t dir, int damage, int speed, edict_t *enemy);

//private
void Disintegrator_pain_daemon_think (edict_t *self);
void Disintegrator_pain_daemon_spawn (edict_t *owner, edict_t *enemy, int damage);
void Disintegrator_explode (edict_t *self, cplane_t *plane);
void Disintegrator_touch (edict_t *self, edict_t *other, cplane_t *plane, csurface_t *surf);
void Disintegrator_fly (edict_t *self);
void weapon_Disintegrator_fire (edict_t *self);

#endif
