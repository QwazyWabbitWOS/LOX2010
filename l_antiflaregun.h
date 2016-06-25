// prototypes for anti-flaregun

#ifndef L_ANTIFLAREGUN_H
#define L_ANTIFLAREGUN_H


void fire_antiflaregun (edict_t *self, vec3_t start, vec3_t aimdir, int damage, int speed);
void antiflare_touch( edict_t *ent, edict_t *other, cplane_t *plane, csurface_t *surf );
void antiflare_think(edict_t *self);
void Weapon_AntiFlaregun_Fire (edict_t *ent);

void AntiFlaregun_Stick(edict_t *ent, edict_t *other);

#endif
