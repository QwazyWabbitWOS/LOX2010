#ifndef SWORD_H
#define SWORD_H

#define SWORD_NORMAL_DAMAGE 100
#define SWORD_DEATHMATCH_DAMAGE 150
#define SWORD_KICK 500

void fire_sword ( edict_t *self, vec3_t start, vec3_t aimdir, int damage, int kick);
void sword_attack (edict_t *ent, vec3_t g_offset, int damage);
void Weapon_Sword_Fire (edict_t *ent);

#endif
