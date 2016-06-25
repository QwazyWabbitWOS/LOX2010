
/***************************************************/
/************       antiflaregun        ************/
/***************************************************/

#include "g_local.h"

void AntiFlaregun_Stick(edict_t *ent, edict_t *other)
{
	
	if (ent->movetype == MOVETYPE_STUCK)
		return;
	
	ent->movetype = MOVETYPE_STUCK;
	VectorClear(ent->velocity);
	VectorClear(ent->avelocity);
	ent->stuckentity = other;
	VectorCopy(other->s.origin,ent->oldstuckorigin);
	VectorCopy(other->s.angles,ent->oldstuckangles);
}

/*
* antiflare_think
*/

void antiflare_think(edict_t *self)
{
	// self->timestamp is 15 seconds after the antiflare was spawned.
	//
	if (self->lastheight > self->s.origin[2])
	{
		VectorScale (self->movedir, 200, self->velocity);
		self->gravity = 0.10f;
	}

	self->s.effects |= EF_GREENGIB;
	self->s.effects |= EF_TRACKER;

	self->lastheight = self->s.origin[2];
	
	if ( level.time > self->timestamp )
		G_FreeEdict( self );
	
	// We're still active, so lets see if we need to shoot some sparks.
	//
	else
	{
		//If we're in water, we won't spark. Otherwise we will.
		//
		if (gi.pointcontents(self->s.origin) & MASK_WATER)
		{
			// We're in water, so let's not glow anymore.
			//
			self->s.effects = 0;
			self->s.sound = 0;
			self->s.renderfx = 0;
		}
		//Check Blindness?
	}
	
	//move to angles...
	if (VectorCompare(self->velocity, vec3_origin))
	{
		vec3_t dir;
		
		VectorCopy(self->velocity, dir);
		VectorNormalize(dir);
		vectoangles (dir, self->s.angles);
	}
	else
		self->s.angles[PITCH] = self->s.angles[ROLL] = 0;
	
	// We'll think again in .1 seconds
	//
	self->nextthink = level.time + 0.1f;
}

void antiflare_touch( edict_t *ent, edict_t *other, cplane_t *plane, csurface_t *surf )
{
	if (other == ent->owner)
		return;
	if (surf && (surf->flags & SURF_SKY))
	{
		G_FreeEdict (ent);
		return;
	}
	
	AntiFlaregun_Stick(ent, other);
}

void fire_antiflaregun (edict_t *self, vec3_t start, vec3_t aimdir, int damage, int speed)
{
	edict_t *antiflare;
	vec3_t dir, forward, right, up;
	
	vectoangles (aimdir, dir);
	AngleVectors (dir, forward, right, up);
	
	antiflare = G_Spawn();
	VectorCopy (start, antiflare->s.origin);
	VectorScale (aimdir, (float) speed, antiflare->velocity);
	VectorClear (antiflare->avelocity);
	VectorCopy(dir, antiflare->s.angles);
	antiflare->movetype = MOVETYPE_BOUNCE;
	antiflare->clipmask = MASK_SHOT;
	antiflare->solid = SOLID_BBOX;
	
	antiflare->s.effects |= EF_GREENGIB;
	antiflare->s.effects |= EF_TRACKER;

	VectorClear (antiflare->mins);
	VectorClear (antiflare->maxs);
	
	antiflare->s.modelindex = gi.modelindex ("models/objects/grenade2/tris.md2");
	antiflare->owner = self;
	antiflare->touch = antiflare_touch;
	antiflare->nextthink = self->nextthink = level.time + 0.1f;	//FRAMETIME;
	antiflare->think = antiflare_think;
	antiflare->dmg = damage;
	antiflare->classname = "antiflare";
	antiflare->classnum = CN_ANTIFLARE;
	
	antiflare->s.sound = gi.soundindex("weapons/flare.wav");
	
	antiflare->timestamp = level.time + 60.0f; //live for 60 seconds
	antiflare->lastheight = antiflare->s.origin[2];
	gi.linkentity (antiflare);
}

void Weapon_AntiFlaregun_Fire (edict_t *ent)
{
	int damage = 10;
	vec3_t	offset, start, forward, right;
	
	if (is_quad)
		damage *= 4;
	
	AngleVectors (ent->client->v_angle, forward, right, NULL);
	
	VectorSet(offset, 24, 8, ent->viewheight - 8.0f);
	P_ProjectSource (ent->client, ent->s.origin, offset, forward, right, start);
	
	fire_antiflaregun (ent, start, forward, damage, 1000);
	
	ent->client->ps.gunframe++;
}

void Weapon_AntiFlaregun (edict_t *ent)
{
	static int	pause_frames[]	= {19, 32, 0};
	static int	fire_frames[]	= {5, 0};
	
	Weapon_Generic (ent, 4, 8, 52, 55, pause_frames, fire_frames, Weapon_AntiFlaregun_Fire);
}
