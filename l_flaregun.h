#ifndef FLAREGUN_H
#define FLAREGUN_H

void fire_flaregun (edict_t *self, vec3_t start, vec3_t aimdir, int damage, int speed);
void flare_touch( edict_t *ent, edict_t *other, cplane_t *plane, csurface_t *surf );
void flare_think(edict_t *self);
void Weapon_Flaregun_Fire (edict_t *ent);

#endif
