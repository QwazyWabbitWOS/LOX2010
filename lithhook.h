/* Vanilla CTF Grappling Hook */

#ifndef LITHHOOK_H
#define LITHHOOK_H

qboolean hook_cond_reset(edict_t *self);

edict_t *hook_laser_start (edict_t *ent);

void hook_laser_think (edict_t *self);
void hook_reset (edict_t *rhook);
void hook_service (edict_t *self);
void hook_track (edict_t *self);
void hook_touch (edict_t *self, edict_t *other, cplane_t *plane, csurface_t *surf);
void hook_fire (edict_t *ent);
void fire_hook (edict_t *owner, vec3_t start, vec3_t forward);

// edict->hookstate constants
#define	HOOK_READY	0
#define HOOK_ON		1		// set if hook command is active
#define HOOK_IN		2		// set if hook has attached
#define SHRINK_ON	4		// set if shrink chain is active 
#define GROW_ON		8		// set if grow chain is active
#define HOOK_OUT	HOOK_ON

#endif
