//
// Prototypes and declarations for all angel types.
//

#ifndef ANGELS_H
#define ANGELS_H

typedef enum AngelType {
    AONONE,
    AODEATH,
    AOLIFE,
    AOMERCY,
    AOFROST,
    AOPLAGUE,
    AOFLAME,
    AOHORROR,
    AOBLIND,
    AOREVENGE,
    AOUNFREEZE,
    AOENERGY
} AngelType;

//angels.c
void Angel_Touch (edict_t *ent, edict_t *other, cplane_t *plane, csurface_t *surf);
void Spawn_Angel(edict_t *ent);

//angel of blindness
void AOBlind_Think (edict_t *ent);

//angel of flame
void AOB_Think (edict_t *ent);

//angel of death
void AOD_Think (edict_t *ent);

//angel of energy
void AOE_Think (edict_t *ent);

//angel of frost
void AOF_Think (edict_t *ent);

//angel of horror
void AOH_Think (edict_t *ent);

//life
void AOL_Think (edict_t *ent);

//mercy
void AOM_Think (edict_t *ent);

//angel of plague
void AOP_Think (edict_t *ent);

//angel of revenge
void AOR_Think (edict_t *ent);

//unfreeze
void AOU_Think (edict_t *ent);

#endif
