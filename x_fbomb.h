/*
//==========================================================================
//  x_fbomb.h -- by Patrick Martin              Last updated:  2-19-1998
//--------------------------------------------------------------------------
//  This file contains copies of functions found in 'g_weapon.c'.
//  The copied functions are slightly modified so that they can
//  produce flaming clouds upon exploding.
//==========================================================================
*/
//QW// Modified for LOX updates.
#ifndef FBOMB_H
#define FBOMB_H

void fire_flamegrenade (edict_t *self, vec3_t start, vec3_t aimdir, int damage, int speed, float timer, float damage_radius);
void fire_flamegrenade2 (edict_t *self, vec3_t start, vec3_t aimdir, int damage, int speed, float timer, float damage_radius);
void firerocket_touch (edict_t *ent, edict_t *other, cplane_t *plane, csurface_t *surf);
void fire_flamerocket (edict_t *self, vec3_t start, vec3_t dir, int damage, int speed, float damage_radius, int radius_damage);
void FireGrenade_Explode (edict_t *ent);
void FireGrenade_Touch (edict_t *ent, edict_t *other, cplane_t *plane, csurface_t *surf);

#endif
