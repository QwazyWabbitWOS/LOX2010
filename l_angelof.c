
//****************************************************
//		Angel of Frost
//****************************************************


#include "g_local.h"
#include "g_team.h"
#include "x_fire.h"
#include "l_angels.h"

//angel of frost
void AOF_Think (edict_t *ent)
{
	vec3_t	offset,blipdir;
	vec3_t	vel,right,forward;
	trace_t tr;
	edict_t *blip = NULL;
	float blipDot, targetDot;
	
	AngleVectors (ent->owner->client->v_angle, forward, right, NULL);
	
	ent->groundentity = NULL;
	
	if (ent->owner->client->angel != ent || ent->owner->client->pers.special != AOFROST)
	{
		G_FreeEdict (ent);
		return;
	}
	
	vectoangles (ent->velocity, ent->s.angles);
	
	tr = gi.trace (ent->s.origin, NULL, NULL, ent->owner->s.origin, NULL, MASK_SHOT);
	if (tr.ent != ent->owner)
	{
		VectorAdd (ent->owner->s.origin, ent->move_origin, offset);
		VectorCopy (offset, ent->s.origin);
	}
	
	// new offset?
	if (rand()%10 < 1 || ent->move_origin[0] == 0)
	{
		VectorSet (offset, rand()%100 - 50.0f, rand()%100 - 50.0f, (float)(rand()%50));
		VectorCopy (offset, ent->move_origin);
	}
	
	
	{
		blip = NULL;
		targetDot = 0;
		while ((blip = findradius (blip, ent->owner->s.origin, 100)) != NULL)
		{
			// See if this is the kind of blip we can home in on.
			
			if (blip->frozen)
				continue;
			if (OnSameTeam(ent->owner,blip))
				continue;
			if (!(blip->svflags & SVF_MONSTER) && !blip->client)
				continue;
			if (blip == ent->owner)
				continue;
			if (!blip->takedamage)
				continue;
			if (blip->health <= 0)
				continue;
			if ((ctf->value || coop->value) && blip->client 
				&& ent->owner->client->resp.ctf_team == blip->client->resp.ctf_team) 
				continue;
			
			if (ent->owner->oldmodel)
				continue;
			if (blip->client == NULL)
				continue;  //QW// decoys have null client id's so ignore them
			if (blip->client->pers.special == AOMERCY)
				continue; //mercy deflects frost
			if (blip->client->pers.special == AOUNFREEZE)
				continue; //unfreeze deflects frost
			if (blip->client->pers.special == AOFLAME)
				continue; //flame deflects frost
			if (blip->oldmodel)
				continue;
			
			
			// Determine where the blip is in relation to us.
			VectorSubtract (blip->s.origin, ent->s.origin, blipdir);
			blipdir[2] += 16;
			
			// Determine how "good" of a target it is.  (Currently, that's the
			// enemy that's most being aimed at, regardless of distance.)
			VectorNormalize (blipdir);
			blipDot = DotProduct (forward, blipdir);
			
			// Remember the "best" target so far.
			if (ent->enemy == NULL || targetDot < blipDot)
			{
				ent->enemy = blip;
				targetDot = blipDot;
			}
		}
		
		if (ent->enemy != NULL)
		{
			tr = gi.trace (ent->enemy->s.origin, ent->mins, ent->maxs, ent->s.origin, ent, MASK_SOLID);
			if (tr.fraction != 1.0) ent->enemy = NULL;
		}
		
		//if we're frozen the angel can't seek a target.
		if (ent->owner->frozen == 1) ent->enemy = NULL;
		
		if (ent->enemy != NULL && ent->enemy->frozen != 1)
		{
			if (ent->enemy->frozen == 0)
				ent->enemy->frozentime = level.time + 8.0f;
			
			// Freeze them.
			ent->enemy->frozen = 1;
			ent->enemy->unfrozen = 0;
			VectorSubtract (ent->enemy->s.origin, ent->s.origin, vel);
			VectorScale (vel, 7, vel);
			VectorCopy (vel, ent->velocity);
			
		}
	}
	
	if (!ent->owner->health <= 0)
	{
		VectorAdd (ent->owner->s.origin, ent->move_origin, offset);
		VectorSubtract (offset, ent->s.origin, vel);
		VectorScale (vel, 2, vel);
		VectorAdd (vel, ent->velocity, ent->velocity);
	}
	
	ent->nextthink = level.time + FRAMETIME;
}
