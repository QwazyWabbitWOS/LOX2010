/**************************************************/
/*           Quake 2 Backpack Tossing             */
/**************************************************/

#ifndef L_BACKPACK_H
#define L_BACKPACK_H

extern cvar_t	*backpacktoss;	// set to 0 to disable

// only one function is exposed
void Backpack_ClientToss(edict_t *player);

#endif
