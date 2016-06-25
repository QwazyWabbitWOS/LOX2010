//
// Prototypes and declarations for all angel types.
//

#ifndef ANGELS_H
#define ANGELS_H


#define		AODEATH			2  //	angel of death
#define		AOLIFE			3  //	angel of life
#define		AOMERCY			4  //	angel of mercy
#define		AOFROST			5  //   angel of frost	
#define		AOPLAGUE		6  //	angel of plague
#define		AOFLAME			7  //	angel of flame
#define		AOHORROR		8  //	angel of horror
#define		AOBLIND  		9  //	angel of blindness
#define		AOREVENGE		10 //	angel of revenge
#define		AOUNFREEZE		11 //	angel of unfreeze
#define		AOENERGY		12 //	angel of energy
#define		MAGE			-2  //	???

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
