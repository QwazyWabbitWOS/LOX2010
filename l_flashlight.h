//
// Public declarations of flashlight functions
//
#pragma once

void Cmd_Flashlight(edict_t* ent);
void Use_Flashlight (edict_t *player);
void flashlight_think (edict_t *self);
void FlashlightReset (edict_t *self);
