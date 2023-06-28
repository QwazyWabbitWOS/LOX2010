
//****************************************************
//		Angel of Energy
//****************************************************


#include "g_local.h"
#include "x_fire.h"
#include "l_angels.h"
#include "l_rockets.h"
#include "l_nuke.h"

void aoe_findtarg(edict_t* ent);
int aoe_valid(edict_t* cur);


void aoe_findtarg(edict_t* ent)
{
	edict_t* cur;
	trace_t	tr;

	cur = findradius(world, ent->owner->s.origin, 100);

	while (cur != NULL)
	{
		if (aoe_valid(cur) && cur->owner != ent->owner && cur->s.modelindex)
		{
			//gi.cprintf (ent->owner, PRINT_HIGH, "aoe found an item, %s\n", cur->classname);
			tr = gi.trace(cur->s.origin, ent->mins, ent->maxs, ent->s.origin, ent, MASK_SOLID);
			if (tr.fraction == 1.0)
			{
				//gi.cprintf (ent->owner, PRINT_HIGH, "ent: %s, targ: %s\n", tr.ent->classname, cur->classname);
				ent->goalentity = cur;
				return;
			}
		}
		cur = findradius(cur, ent->owner->s.origin, 100);
	}
	return;
}

//angel of energy
void AOE_Think(edict_t* ent)
{
	vec3_t	offset = { 0 }, vel = { 0 }, len = { 0 };
	trace_t tr;

	ent->groundentity = NULL;

	if (ent->owner->client->angel != ent ||
		(ent->owner->client->pers.special != AOENERGY &&
			ent->owner->client->pers.special != MAGE))
	{
		G_FreeEdict(ent);
		return;
	}

	VectorCopy(ent->owner->s.angles, ent->s.angles);
	vectoangles(ent->velocity, ent->s.angles);

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
		VectorSet(offset, rand() % 70 - 35.0f, rand() % 70 - 35.0f, (float)(rand() % 35));
		VectorCopy(offset, ent->move_origin);
	}
	// set velocity
	if (!(ent->goalentity))
		aoe_findtarg(ent);

	if (ent->goalentity)
	{
		VectorSubtract(ent->owner->s.origin, ent->goalentity->s.origin, offset);

		if (VectorLength(offset) > 100.0f)
		{
			ent->goalentity = NULL;
		}
		else
		{
			{
				VectorSet(len, 1, 1, 1);
				VectorAdd(len, ent->goalentity->s.origin, len);
				VectorCopy(len, ent->s.origin);
			}

			VectorSubtract(ent->s.origin, ent->goalentity->s.origin, len);
			if (VectorLength(len) <= 10)
			{
				// this correctly deletes the guided missile and pilot edicts 
				// so the owner of the rocket doesn't end up in limbo space.
				if (ent->goalentity->classnum == CN_GUIDEDROCKET)
				{
					RemoveGuided(ent->goalentity);
					G_FreeEdict(ent->goalentity);
					ent->goalentity = NULL;
				}
				// this correctly deletes the guided missile and pilot edicts 
				// so the owner of the guided nuke doesn't end up in limbo space.
				else if (ent->goalentity->classnum == CN_GUIDEDNUKE)
				{
					RemoveNuke(ent->goalentity);
					G_FreeEdict(ent->goalentity);
					ent->goalentity = NULL;
				}
				else
				{
					gi.cprintf(ent->owner, PRINT_HIGH, "Angel target: %s eliminated\n", ent->goalentity->classname);
					SpawnDamage(TE_SPARKS, ent->goalentity->s.origin, ent->goalentity->s.origin, 100);
					if (ent->goalentity->classnum == CN_FLOATINGMINE) // clean up the owner's mine count
					{
						ent->goalentity->owner->client->mine_count--;
						//OutputDebugString("Floating mine count decremented\n");
					}
					G_FreeEdict(ent->goalentity);
					ent->goalentity = NULL;
				}
			}
		}
	}
	else
	{
		VectorAdd(ent->owner->s.origin, ent->move_origin, offset);
		VectorSubtract(offset, ent->s.origin, vel);
		VectorScale(vel, 3.5, vel);
		VectorAdd(vel, ent->velocity, ent->velocity);
	}

	ent->nextthink = level.time + FRAMETIME;
}

// the classes angeloe can intercept
int aoe_valid(edict_t* cur)
{
	if (cur->think == target_laser_think)
		return true;

	switch (cur->classnum)
	{
	case	CN_PLASMA:
	case	CN_BFGBLAST:
	case	CN_BOLT:
	case	CN_GRENADE:
	case	CN_SHRAPNEL:
	case	CN_NAPALM:
	case	CN_FLARE:
	case	CN_ANTIFLARE:
	case	CN_FREEZATRON:
	case	CN_FLOATINGMINE:
	case	CN_POSITRONEXPLOSION:
	case	CN_FREEZATRONEXPLOSION:
	case	CN_CATACLYSMEXPLOSION:
	case	CN_PLASMAEXPLOSION:
	case	CN_PULSE:
	case	CN_HOMINGROCKET:
	case	CN_GUIDEDROCKET:
	case	CN_GUIDEDNUKE:
	case	CN_NUKE:
		return true;
		break;
	default:
		return false;
		break;
	}
}

