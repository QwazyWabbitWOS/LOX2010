/* Vanilla CTF Grappling Hook */

#include "g_local.h"
#include "m_player.h"
#include "lithhook.h"

void hook_laser_think (edict_t *self)
{
	vec3_t	forward, right, offset, start;
	
	if (!self->owner || !self->owner->owner || !self->owner->owner->client)
	{
		G_FreeEdict(self);
		return;	
	}
	
	AngleVectors (self->owner->owner->client->v_angle, forward, right, NULL);
	VectorSet(offset, 24, -8, self->owner->owner->viewheight-8);
	P_ProjectSource (self->owner->owner->client, self->owner->owner->s.origin, offset, forward, right, start);
	
	VectorCopy (start, self->s.origin);
	VectorCopy (self->owner->s.origin, self->s.old_origin);
	gi.linkentity(self);
	
	self->nextthink = level.time + FRAMETIME;
	return;
}

edict_t *hook_laser_start (edict_t *ent)
{
	edict_t *self;
	
	self = G_Spawn();
	self->movetype = MOVETYPE_NONE;
	self->solid = SOLID_NOT;
	self->s.renderfx |= RF_BEAM|RF_TRANSLUCENT;
	self->s.modelindex = 1;
	self->owner = ent;
	
	self->s.frame = 4;
	
	// set the color
	self->s.skinnum = 0xd0d1d2d3;// green
	
	//		self->s.skinnum = 0xf2f2f0f0;		// red
	//		self->s.skinnum = 0xf3f3f1f1;		// sorta blue
	//		self->s.skinnum = 0xf3f3f1f1;		// red+blue = purple
	//		self->s.skinnum = 0xdcdddedf;		// brown
	//		self->s.skinnum = 0xe0e1e2e3;		// red+brown
	
	
	self->think = hook_laser_think;
	
	VectorSet (self->mins, -8, -8, -8);
	VectorSet (self->maxs, 8, 8, 8);
	gi.linkentity (self);
	
	self->spawnflags |= 0x80000001;
	self->svflags &= ~SVF_NOCLIENT;
	hook_laser_think (self);
	return(self);
}

void hook_reset (edict_t *rhook)
{
	if (!rhook)
		return;
	if (rhook->owner)
	{
		if (rhook->owner->client)
		{
			rhook->owner->client->hookstate = HOOK_READY;
			rhook->owner->client->hook = NULL;
		}
	}
	if (rhook->laser) G_FreeEdict(rhook->laser);
	G_FreeEdict(rhook);
}

qboolean hook_cond_reset(edict_t *self)
{
	if (!self || !self->owner->client)
		return false;

	if (!self->owner || (!self->enemy && self->owner->client && self->owner->client->hookstate == HOOK_ON))
	{
		hook_reset (self);
		return (true);
	}
	
	if ((self->enemy && !self->enemy->inuse) || (!self->owner->inuse) ||
		(self->enemy && self->enemy->client && self->enemy->health <= 0) || 
		(self->owner->health <= 0))
	{
		hook_reset (self);
		return (true);
	}
	
	if (!((self->owner->client->latched_buttons|self->owner->client->buttons) & BUTTON_ATTACK)
		&& (strcmp(self->owner->client->pers.weapon->pickup_name, "Grapple") == 0))
	{
		hook_reset (self);
		return (true);
	}
	
	return(false);
}

void hook_cond_reset_think(edict_t *hook)
{
	if (hook_cond_reset(hook))
		return;
	hook->nextthink = level.time + FRAMETIME;
}

void hook_service (edict_t *self)
{
	vec3_t	hook_dir;
	
	if (hook_cond_reset(self))
		return;
	
	if (self->enemy->client)
		VectorSubtract(self->enemy->s.origin, self->owner->s.origin, hook_dir);
	else
		VectorSubtract(self->s.origin, self->owner->s.origin, hook_dir);
	VectorNormalize(hook_dir);
	VectorScale(hook_dir, 750, self->owner->velocity);
}

void hook_track (edict_t *self)
{
	vec3_t	normal;
	
	if (hook_cond_reset(self))
		return;
	
	if (self->enemy->client)
	{
		VectorCopy(self->enemy->s.origin, self->s.origin);
		
		VectorSubtract(self->owner->s.origin, self->enemy->s.origin, normal);
		
		T_Damage (self->enemy, self, self->owner, vec3_origin, self->enemy->s.origin, normal, 1, 0, DAMAGE_NO_KNOCKBACK, MOD_GRAPPLE);
	}
	else
	{
		VectorCopy(self->enemy->velocity, self->velocity);
	}
	
	gi.linkentity(self);
	self->nextthink = level.time + 0.1;
}

void hook_touch (edict_t *self, edict_t *other, cplane_t *plane, csurface_t *surf)
{
	vec3_t	dir, normal;
	
	if (other == self->owner)
		return;
	
	if (other->solid == SOLID_NOT || other->solid == SOLID_TRIGGER || other->movetype == MOVETYPE_FLYMISSILE)
		return;
	
	if (other->client)
	{
		VectorSubtract(other->s.origin, self->owner->s.origin, dir);
		VectorSubtract(self->owner->s.origin, other->s.origin, normal);
		T_Damage(other, self, self->owner, dir, self->s.origin, normal, 10, 10, 0, MOD_GRAPPLE);
		hook_reset(self);
		return;
	}
	else
	{
		if (other->takedamage)
		{
			VectorSubtract(other->s.origin, self->owner->s.origin, dir);
			VectorSubtract(self->owner->s.origin, other->s.origin, normal);
			T_Damage(other, self, self->owner, dir, self->s.origin, normal, 1, 1, 0, MOD_GRAPPLE);
		}
		
		gi.positioned_sound(self->s.origin, self, CHAN_WEAPON, gi.soundindex("flyer/Flyatck2.wav"), 1, ATTN_NORM, 0);
	}
	
	VectorClear(self->velocity);
	
	self->enemy = other;
	self->owner->client->hookstate = HOOK_ON;
	
	self->think = hook_track;
	self->nextthink = level.time + 0.1;
	
	self->solid = SOLID_NOT;
}

void fire_hook (edict_t *owner, vec3_t start, vec3_t forward) 
{
	edict_t	*hook;
	trace_t tr;
	
	hook = G_Spawn();
	hook->movetype = MOVETYPE_FLYMISSILE;
	hook->solid = SOLID_BBOX;
	hook->clipmask = MASK_SHOT;
	hook->owner = owner;
	owner->client->hook = hook;
	hook->classname = "hook";
	
	vectoangles (forward, hook->s.angles);
	VectorScale(forward, 800, hook->velocity);
	
	hook->touch = hook_touch;
	
	hook->think = hook_cond_reset_think;
	hook->nextthink = level.time + FRAMETIME;
	
	gi.setmodel(hook, "");
	
	VectorCopy(start, hook->s.origin);
	VectorCopy(hook->s.origin, hook->s.old_origin);
	
	VectorClear(hook->mins);
	VectorClear(hook->maxs);
	
	hook->laser = hook_laser_start(hook);
	
	gi.linkentity(hook);
	
	tr = gi.trace (owner->s.origin, NULL, NULL, hook->s.origin, hook, MASK_SHOT);
	if (tr.fraction < 1.0)
	{
		VectorMA (hook->s.origin, -10, forward, hook->s.origin);
		hook->touch (hook, tr.ent, NULL, NULL);
	}
	
}

// this is the function called from command that initiates the hook (Hook_Fire_f)
void hook_fire (edict_t *ent) 
{
	vec3_t	forward, right;
	vec3_t	start;
	vec3_t	offset;
	
	if (ent->client->hookstate)
		return;
	
    ent->client->hookstate = HOOK_OUT;
	
	AngleVectors (ent->client->v_angle, forward, right, NULL);
	VectorSet(offset, 8, -8, ent->viewheight-8);
	P_ProjectSource (ent->client, ent->s.origin, offset, forward, right, start);
	
	VectorScale (forward, -2, ent->client->kick_origin);
	ent->client->kick_angles[0] = -1;
	
	fire_hook (ent, start, forward);
	
	if (ent->client->silencer_shots)
		gi.sound(ent, CHAN_WEAPON, gi.soundindex("flyer/Flyatck3.wav"), 0.2f, ATTN_NORM, 0);
	else
		gi.sound(ent, CHAN_WEAPON, gi.soundindex("flyer/Flyatck3.wav"), 1, ATTN_NORM, 0);
	
	PlayerNoise(ent, start, PNOISE_WEAPON);
	
}

