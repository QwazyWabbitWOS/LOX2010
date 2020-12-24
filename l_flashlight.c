
//****************************************************
//		Flashlight
//****************************************************

#include "g_local.h"

//#define FL_IMPROVED	//defined for improved flashlight

void nightmareResetFlashlight (edict_t *self)
{
	if (self->owner->client && self->owner->client->flashlight)
	{
		float volume = 1.0f;
		
		if (self->owner->client->silencer_shots)
			volume = 0.2f;
		gi.sound(self->owner, CHAN_RELIABLE+CHAN_WEAPON, gi.soundindex("misc/lasfly.wav"), volume, ATTN_NORM, 0.0f);
		self->owner->client->flashlight = NULL;
		self->owner->client->flashtype = 0;
	}
	G_FreeEdict(self);
}

void flashlight_think (edict_t *self)
{
	vec3_t	forward, right, up, offset, start, end;
	trace_t	tr;
	int content= (CONTENTS_SOLID|CONTENTS_MONSTER|CONTENTS_DEADMONSTER);
	if (level.intermissiontime || !self->owner->inuse || self->owner->deadflag)
	{
		nightmareResetFlashlight(self);
		return;
	}
	AngleVectors(self->owner->client->v_angle, forward, right, up);
	VectorSet(offset, 0, 8, self->owner->viewheight - 8.0f);
	P_ProjectSource(self->owner->client, self->owner->s.origin, offset, forward, right, start);
	VectorMA(start, 8192, forward, end);
	tr = gi.trace(start, NULL, NULL, end, self->owner, content);
	
#ifdef FL_IMPROVED
	self->svflags &= ~SVF_NOCLIENT;
#else
	if (!((tr.surface) && (tr.surface->flags & SURF_SKY)) && tr.fraction < 1.0)
		self->svflags &= ~SVF_NOCLIENT;
	else
		self->svflags |= SVF_NOCLIENT;
#endif
	
	vectoangles(tr.plane.normal, self->s.angles);
	VectorCopy(tr.endpos, self->s.origin);
	self->nextthink = level.time + 0.1f;
	
#ifdef FL_IMPROVED
	//QW// new, FL_IMPROVED nice white flashlight
	gi.WriteByte (svc_temp_entity);
	gi.WriteByte (TE_FLASHLIGHT);
	gi.WritePosition (self->s.origin);
	gi.WriteShort (self - g_edicts);
	gi.multicast (self->s.origin, MULTICAST_PVS);
#endif
	
	gi.linkentity(self);
}


void Use_Flashlight (edict_t *player)
{
	if (player->client->flashlight)
	{
		if (player->client->flashtype == 0)
		{
			player->client->flashtype = 1;
			player->client->flashlight->s.effects = 0;
			player->client->flashlight->s.renderfx = RF_TRANSLUCENT;
			gi.linkentity(player->client->flashlight);
		}
		else if (player->client->flashtype == 1)
		{
			player->client->flashtype = 2;
			player->client->flashlight->s.effects = EF_HYPERBLASTER;
			gi.linkentity(player->client->flashlight);
		}
		else
		{
			player->client->flashtype = 0;
			nightmareResetFlashlight(player->client->flashlight);
		}
	}
	else
	{
		float volume = 1.0;
		
		if (player->client->silencer_shots)
			volume = 0.2f;
		gi.sound(player, CHAN_RELIABLE+CHAN_WEAPON, gi.soundindex("misc/lasfly.wav"), volume, ATTN_NORM, 0);
		player->client->flashlight = G_Spawn();
		VectorCopy(player->s.origin, player->client->flashlight->s.origin);
		player->client->flashlight->movetype = MOVETYPE_NOCLIP;
		player->client->flashlight->clipmask = MASK_SHOT;
		player->client->flashlight->solid = SOLID_NOT;
		player->client->flashlight->owner = player;
		player->client->flashlight->nextthink = level.time + 0.1f;
		player->client->flashlight->think = flashlight_think;
		player->client->flashlight->classname = "flashlight";
		player->client->flashlight->svflags = SVF_NOCLIENT;
		//QW// invisible model (the laser sight rescaled)
#ifdef FL_IMPROVED
		player->client->flashlight->s.modelindex = gi.modelindex("models/flight/tris.md2");
#else
		player->client->flashlight->s.modelindex = gi.modelindex("models/lsight/tris.md2");
#endif
		player->client->flashlight->s.frame      = 0;
		player->client->flashlight->s.skinnum    = 0;
		
		VectorClear(player->client->flashlight->mins);
		VectorClear(player->client->flashlight->maxs);
		
#ifndef FL_IMPROVED
		//QW// old, dim flashlight
		player->client->flashlight->s.effects = EF_HYPERBLASTER;
		player->client->flashlight->s.renderfx |= RF_TRANSLUCENT;
		player->client->flashlight->s.renderfx |= RF_FULLBRIGHT;
#endif
		gi.linkentity(player->client->flashlight);
	}
}

