#pragma once

#define DEATHRAY_DAMAGE 200
#define DEATHRAY_SPEED 2500

void Weapon_Deathray(edict_t* ent);
void fire_deathray(edict_t* self, vec3_t start, vec3_t dir, int damage, int speed, int effect);
void Weapon_Deathray_Fire(edict_t* ent);
void Deathray_Fire(edict_t* ent, vec3_t g_offset, int damage, qboolean hyper, int effect);
