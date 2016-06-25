/*
* Copyright (C) 2006 by QwazyWabbit and ClanWOS.org
*
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation; either version 2
* of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  
*
* See the GNU General Public License for more details.
* 
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*
* You may freely use and alter this code so long as this banner
* remains and credit is given for its source. 
*/

// Definitions for DarKMajick Grenades

#ifndef DM_GRENADES_H
#define DM_GRENADES_H

// darKMajick grenade types. (dM_grenade)
typedef enum grenade_n
{ 	
	DM_NORMALGRENADE, //keep normals first
		DM_CLUSTERGRENADE,
		DM_RAILBOMB,
		DM_PLASMAGRENADE,
		DM_NAPALMGRENADE,
		DM_SHRAPNELGRENADE,
		DM_CATACLYSM,
		DM_PLASMACLUSTER,
		DM_TELEGRENADE,
		DM_FREEZEGRENADE,
		DM_HOMERGRENADE,
		DM_ROCKETGRENADE,
		DM_BLINDINGGRENADE,
		DM_POSITRONGRENADE,
		DM_MIDNIGHTGRENADE,
		DM_FREEZATRONGRENADE,
		DM_BANZAIGRENADE,
		DM_SPINNINGRAILBOMB //keep this one last (assert catches undefined grenade types)
} grenade_t;

void grenade_float(edict_t *ent);
void banzai_fire(edict_t *ent);
void spinning_rail_fire(edict_t *ent);
void plasma_explode (edict_t *ent);
void dM_Flame_Touch (edict_t *ent, edict_t *other, cplane_t *plane, csurface_t *surf);
void Flame_Burn (edict_t *ent);
void Cata_Explode (edict_t *ent);
void Cata_Explode_Touch (edict_t *ent, edict_t *other, cplane_t *plane, csurface_t *surf);
void Shrap_Explode (edict_t *ent);
void Shrap_Explode_Touch (edict_t *ent, edict_t *other, cplane_t *plane, csurface_t *surf);
void Proxie_think(edict_t *ent);
void Floating_Mine(edict_t *ent);
void Grenade_Explode_dM (edict_t *ent);
void Grenade_Touch_dM (edict_t *ent, edict_t *other, cplane_t *plane, csurface_t *surf);
void SetGrenadeEffects(edict_t *grenade, int typ);
void fire_grenade_plasma (edict_t *self, vec3_t start, vec3_t aimdir, int damage, int speed, float timer, float damage_radius);
void fire_grenade_dM (edict_t *self, vec3_t start, vec3_t aimdir, int damage,
					  int speed, float timer, float damage_radius, int typ,
					  qboolean held, qboolean bazookad);
void BecomeNewExplosion (edict_t *ent);

#endif

