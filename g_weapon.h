/*
Copyright (C) 1997-2001 Id Software, Inc.

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/

//
// g_weapon.c
//

#ifndef G_WEAPON_H
#define G_WEAPON_H

void check_dodge (edict_t *self, vec3_t start, vec3_t dir, int speed);
void fire_explosive_bullet (edict_t *self, vec3_t start, vec3_t aimdir, int damage, int kick, int hspread, int vspread, int mod);
void ThrowDebris (edict_t *self, char *modelname, float speed, vec3_t origin);
qboolean fire_hit (edict_t *self, vec3_t aim, int damage, int kick);
void fire_bullet (edict_t *self, vec3_t start, vec3_t aimdir, int damage, int kick, int hspread, int vspread, int mod);
void fire_shotgun (edict_t *self, vec3_t start, vec3_t aimdir, int damage, int kick, int hspread, int vspread, int count, int mod);
void fire_explosive_shotgun (edict_t *self, vec3_t start, vec3_t aimdir, int damage, int kick,int te_impact, int hspread, int vspread, int count, int mod);
void fire_explosive_lead (edict_t *self, vec3_t start, vec3_t aimdir, int damage, int kick, int te_impact, int hspread, int vspread, int mod);
void fire_grenade (edict_t *self, vec3_t start, vec3_t aimdir, int damage, int speed, float timer, float damage_radius);
void fire_rocket (edict_t *self, vec3_t start, vec3_t dir, int damage, int speed, float damage_radius, int radius_damage);
void fire_rail (edict_t *self, vec3_t start, vec3_t aimdir, int damage, int kick);
void fire_bfg (edict_t *self, vec3_t start, vec3_t dir, int damage, int speed, float damage_radius);

void fire_blaster (edict_t *self, vec3_t start, vec3_t dir, int damage, int speed, int effect);
void fire_sniper (edict_t *self, vec3_t start, vec3_t dir, int damage, int speed, int effect);
void fire_mr (edict_t *self, vec3_t start, vec3_t dir, int damage, int speed, float damage_radius, int radius_damage);
void fire_super (edict_t *self, vec3_t start, vec3_t dir, int damage, int speed, int effect);
void fire_plasma (edict_t *self, vec3_t start, vec3_t dir, int damage, int speed, int effect);
void fire_bolt (edict_t *self, vec3_t start, vec3_t aimdir, int damage);

#endif
