#ifndef STICKYGREN_H
#define STICKYGREN_H

void fire_stickinggrenade (edict_t *self, vec3_t start, vec3_t aimdir, int damage, int speed, float timer, float damage_radius);
void StickingGrenade_Touch (edict_t *ent, edict_t *other, cplane_t *plane, csurface_t *surf);
void StickingGrenade_Explode (edict_t *ent);
void weapon_stickinggrenadelauncher_fire (edict_t *ent);
void Stick(edict_t *ent, edict_t *other);

#endif
