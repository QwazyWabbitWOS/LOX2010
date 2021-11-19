
//****************************************************
//		Angel of Death
//****************************************************


#include "g_local.h"
#include "x_fire.h"
#include "l_angels.h"

//angel of death
void AOD_Think(edict_t* ent)
{
	vec3_t	offset = { 0 };
	vec3_t	vel = { 0 };
	trace_t tr;

	ent->groundentity = NULL;

	if (ent->owner->client->angel != ent || ent->owner->client->pers.special != AODEATH)
	{
		G_FreeEdict(ent);
		return;
	}

	VectorCopy(ent->owner->s.angles, ent->s.angles);

	// check if master in view
	tr = gi.trace(ent->s.origin, NULL, NULL, ent->owner->s.origin, NULL, MASK_SHOT);

	if (tr.ent != ent->owner)
	{
		VectorAdd(ent->owner->s.origin, ent->move_origin, offset);
		VectorCopy(offset, ent->s.origin);
	}

	// new offset?
	if (rand() % 10 < 1 || ent->move_origin[0] == 0)
	{
		VectorSet(offset, rand() % 50 - 25.0f, rand() % 50 - 25.0f, (float)(rand() % 25));
		VectorCopy(offset, ent->move_origin);
	}
	// set velocity
	VectorAdd(ent->owner->s.origin, ent->move_origin, offset);
	VectorSubtract(offset, ent->s.origin, vel);
	VectorScale(vel, 2, vel);
	VectorAdd(vel, ent->velocity, ent->velocity);

	ent->nextthink = level.time + FRAMETIME;
}

