
/***************************************************/
/**************       flaregun        **************/
/***************************************************/

#include "g_local.h"
#include "g_team.h"

/*
* flare_think
*/

void flare_think(edict_t *self)
{
	// self->timestamp is 15 seconds after the flare was spawned.
	//
	if (self->lastheight > self->s.origin[2])
	{
		self->s.renderfx |= RF_SHELL_RED;
		self->s.renderfx |= RF_SHELL_BLUE;
		self->s.renderfx |= RF_SHELL_GREEN;
		VectorScale (self->movedir, 200, self->velocity);
		self->gravity = 0.10f;
	}
	self->lastheight = self->s.origin[2];
	
	if (level.time > self->timestamp)
		G_FreeEdict( self );
	
	// We're still active, so lets see if we need to shoot some sparks.
	//
	else
	{
		//If we're in water, we won't spark. Otherwise we will
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
	
	//move to angles
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

void flare_touch( edict_t *ent, edict_t *other, cplane_t *plane, csurface_t *surf )
{
	if (other == ent->owner)
		return;
	if (surf && (surf->flags & SURF_SKY))
	{
		G_FreeEdict (ent);
		return;
	}
	if (!other->takedamage)
		return;
	//T_Damage (other, ent, ent->owner, ent->velocity, ent->s.origin, plane->normal, ent->dmg, 2, DAMAGE_NO_KNOCKBACK, MOD_FIRE_SPLASH);
}

void fire_flaregun (edict_t *self, vec3_t start, vec3_t aimdir, int damage, int speed)
{
	edict_t *flare;
	vec3_t dir, forward, right, up;
	
	vectoangles (aimdir, dir);
	AngleVectors (dir, forward, right, up);
	
	flare = G_Spawn();
	VectorCopy (start, flare->s.origin);
	VectorScale (aimdir, (float) speed, flare->velocity);
	VectorClear (flare->avelocity);
	VectorCopy(dir, flare->s.angles);
	flare->movetype = MOVETYPE_BOUNCE;
	flare->clipmask = MASK_SHOT;
	flare->solid = SOLID_BBOX;
	
	flare->s.effects = EF_ROCKET; //make it glow
	flare->s.effects |= EF_COLOR_SHELL;
	flare->s.renderfx |= RF_SHELL_RED;
	
	if (ctf->value)
	{
		switch (self->client->resp.ctf_team)
		{
		case CTF_TEAM1:
			flare->s.renderfx = RF_SHELL_RED;
			break;
		case CTF_TEAM2:
			flare->s.renderfx = RF_SHELL_BLUE;
			break;
		}
	}
	
	VectorClear (flare->mins);
	VectorClear (flare->maxs);
	
	flare->s.modelindex = gi.modelindex ("models/objects/grenade2/tris.md2");
	flare->owner = self;
	flare->touch = flare_touch;
	flare->nextthink = self->nextthink = level.time + 0.1f;	//FRAMETIME;
	flare->think = flare_think;
	flare->dmg = damage;
	flare->classname = "flare";
	flare->classnum = CN_FLARE;
	
	flare->s.sound = gi.soundindex("weapons/flare.wav");
	
	flare->timestamp = level.time + 30.0f; //live for 30 seconds
	flare->lastheight = flare->s.origin[2];
	gi.linkentity (flare);
}

void Weapon_Flaregun_Fire (edict_t *ent)
{
	int damage = 10;
	vec3_t	offset, start, forward, right;
	
	if (is_quad)
		damage *= 4;
	
	AngleVectors (ent->client->v_angle, forward, right, NULL);
	
	VectorSet(offset, 24, 8, ent->viewheight - 8.0f);
	P_ProjectSource (ent->client, ent->s.origin, offset, forward, right, start);
	
	fire_flaregun (ent, start, forward, damage, 1000);
	
	ent->client->ps.gunframe++;
}

void Weapon_Flaregun (edict_t *ent)
{
	static int	pause_frames[]	= {19, 32, 0};
	static int	fire_frames[]	= {5, 0};
	
	Weapon_Generic (ent, 4, 8, 52, 55, pause_frames, fire_frames, Weapon_Flaregun_Fire);
}
