
//****************************************************
//		Angel of Unfreeze
//****************************************************

#include "g_local.h"
#include "x_fire.h"
#include "l_angels.h"

void AOU_Think(edict_t* ent)
{
	vec3_t	offset = { 0 };
	vec3_t	vel = { 0 }, right, forward;
	trace_t tr;

	AngleVectors(ent->owner->client->v_angle, forward, right, NULL);
	ent->groundentity = NULL;
	if (ent->owner->client->angel != ent || ent->owner->client->pers.special != AOUNFREEZE)
	{
		G_FreeEdict(ent);
		return;
	}
	vectoangles(ent->velocity, ent->s.angles);
	tr = gi.trace(ent->s.origin, NULL, NULL, ent->owner->s.origin, NULL, MASK_SHOT);
	if (tr.ent != ent->owner)
	{
		VectorAdd(ent->owner->s.origin, ent->move_origin, offset);
		VectorCopy(offset, ent->s.origin);
	}
	// new offset?
	if (rand() % 10 < 1 || ent->move_origin[0] == 0)
	{
		VectorSet(offset, rand() % 100 - 50.0f, rand() % 100 - 50.0f, (float)(rand() % 50));
		VectorCopy(offset, ent->move_origin);
	}

	if (!(ent->owner->health <= 0))
	{
		if (ent->owner->frozen == 1)
		{
			ent->owner->frozen = 0;
			ent->owner->frozentime = level.time;
			ent->owner->unfrozen = 1;
		}
		VectorAdd(ent->owner->s.origin, ent->move_origin, offset);
		VectorSubtract(offset, ent->s.origin, vel);
		VectorScale(vel, 2, vel);
		VectorAdd(vel, ent->velocity, ent->velocity);
	}
	ent->nextthink = level.time + FRAMETIME;
}
