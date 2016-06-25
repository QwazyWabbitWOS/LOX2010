#ifndef ICEBALLGUN_H
#define ICEBALLGUN_H

void IceThink (edict_t *self);
void ThrowIce (edict_t *self);
void IcePosFromPlayer (edict_t *ent, float size, vec3_t output);
void UnfreezeMonster (edict_t *self);
void AttachIceThink (edict_t *self);
void AttachIce (edict_t *self);
void T_FreezeRadiusDamage (edict_t *inflictor, edict_t *attacker, float damage, edict_t *ignore, float radius, int mod);
void IceTouch (edict_t *ent, edict_t *other, cplane_t *plane, csurface_t *surf);
void CreateIce (edict_t *self);
void Weapon_Freezegun_Fire (edict_t *ent);

#endif
