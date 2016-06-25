#ifndef L_NUKE_H
#define L_NUKE_H


// defined in l_guidednuke.c */
void RemoveNuke(edict_t *ent);
void GuidedNuke_Explode (edict_t *ent);
void GuidedNuke_Think (edict_t *self);
void guidednuke_touch (edict_t *ent, edict_t *other, cplane_t *plane, csurface_t *surf);
void guidedNuke_touch (edict_t *ent, edict_t *other, cplane_t *plane, csurface_t *surf);
void fire_guidednuke (edict_t *self, vec3_t start, vec3_t dir, int damage, int speed, float damage_radius, int radius_damage);
void Weapon_GuidedNuke_Fire (edict_t *ent);
void Weapon_GuidedNuke (edict_t *ent);
void nuke_die (edict_t *self, edict_t *inflictor, edict_t *attacker, int damage, vec3_t point);


/* defined in l_nuke.c */
void nuke_touch (edict_t *ent, edict_t *other, cplane_t *plane, csurface_t *surf);
void fire_nuke (edict_t *self, vec3_t start, vec3_t dir, int damage, int speed, float damage_radius, int radius_damage);
void weapon_nuke_fire (edict_t *ent);
void Weapon_Nuke (edict_t *ent);

#endif

