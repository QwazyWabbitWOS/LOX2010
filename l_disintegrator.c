
//======================================================================
//Wyrm new weapons!!
//
//
// Disintegrator
//
//======================================================================

#include "g_local.h"
#include "l_disintegrator.h"

void fire_Disintegrator (edict_t *self, vec3_t start, vec3_t dir, int damage, int speed, edict_t *enemy)
{
	edict_t	*bolt;
	trace_t	tr;
	
	VectorNormalize (dir);
	
	bolt = G_Spawn();
	VectorCopy (start, bolt->s.origin);
	VectorCopy (start, bolt->s.old_origin);
	vectoangles2 (dir, bolt->s.angles);
	VectorScale (dir, (float)speed, bolt->velocity);
	bolt->movetype = MOVETYPE_FLYMISSILE;
	bolt->clipmask = MASK_SHOT;
	bolt->solid = SOLID_BBOX;
	bolt->speed = (float)speed;
	bolt->s.effects = EF_TRACKER;
	bolt->s.sound = gi.soundindex ("weapons/rockfly.wav");
	VectorClear (bolt->mins);
	VectorClear (bolt->maxs);
	bolt->s.modelindex = gi.modelindex ("models/objects/rocket/tris.md2");
	bolt->touch = Disintegrator_touch;
	bolt->enemy = enemy;
	bolt->owner = self;
	bolt->dmg = damage;
	bolt->classname = "Disintegrator";
	bolt->classnum = CN_DISINTEGRATOR;
	gi.linkentity (bolt);
	
	if (enemy)
	{
		bolt->nextthink = level.time + 0.1f;
		bolt->think = Disintegrator_fly;
	}
	else
	{
		bolt->nextthink = level.time + 10;
		bolt->think = G_FreeEdict;
	}
	
	if (self->client)
		check_dodge (self, bolt->s.origin, dir, speed);
	
	tr = gi.trace (self->s.origin, NULL, NULL, bolt->s.origin, bolt, MASK_SHOT);
	if (tr.fraction < 1.0)
	{
		VectorMA (bolt->s.origin, -10, dir, bolt->s.origin);
		bolt->touch (bolt, tr.ent, NULL, NULL);
	}
}

void Weapon_Disintegrator (edict_t *ent)
{
	static int	pause_frames[]	= {0};
	static int	fire_frames[]   = {6, 12, 0};
	
	Weapon_Generic (ent, DIS_ACTIVATE_LAST, DIS_FIRE_LAST, DIS_IDLE_LAST, 
		DIS_DEACTIVATE_LAST, pause_frames, fire_frames, weapon_Disintegrator_fire);
}

void Disintegrator_pain_daemon_think (edict_t *self)
{
	static vec3_t	pain_normal = { 0, 0, 1 };
	int				hurt;
	
	if (!self->inuse)
		return;
	
	if ((level.time - self->timestamp) > TRACKER_DAMAGE_TIME)
	{
		if (!self->enemy->client)
			self->enemy->s.effects &= ~EF_TRACKERTRAIL;
		G_FreeEdict (self);
	}
	else
	{
		if (self->enemy->health > 0)
		{
			//gi.dprintf("ouch %x\n", self);
			T_Damage (self->enemy, self, self->owner, vec3_origin, self->enemy->s.origin, pain_normal,
				self->dmg, 0, TRACKER_DAMAGE_FLAGS, MOD_DISRUPTOR);
			
			// if we kill the player, we'll be removed.
			if (self->inuse)
			{
				// if we killed a monster, gib them.
				if (self->enemy->health < 1)
				{
					if (self->enemy->gib_health)
						hurt = - self->enemy->gib_health;
					else
						hurt = 500;
					
					//gi.dprintf("non-player killed. ensuring gib!  %d\n", hurt);
					T_Damage (self->enemy, self, self->owner, vec3_origin, self->enemy->s.origin,
						pain_normal, hurt, 0, TRACKER_DAMAGE_FLAGS, MOD_DISRUPTOR);
				}
				
				if (self->enemy->client)
					self->enemy->client->tracker_pain_framenum = (float)(level.framenum + 1);
				else
					self->enemy->s.effects |= EF_TRACKERTRAIL;
				
				self->nextthink = level.time + FRAMETIME;
			}
		}
		else
		{
			if (!self->enemy->client)
				self->enemy->s.effects &= ~EF_TRACKERTRAIL;
			G_FreeEdict (self);
		}
	}
}

void Disintegrator_pain_daemon_spawn (edict_t *owner, edict_t *enemy, int damage)
{
	edict_t	 *daemon;
	
	if (enemy == NULL)
		return;
	
	daemon = G_Spawn();
	daemon->classname = "pain daemon";
	daemon->think = Disintegrator_pain_daemon_think;
	daemon->nextthink = level.time + FRAMETIME;
	daemon->timestamp = level.time;
	daemon->owner = owner;
	daemon->enemy = enemy;
	daemon->dmg = damage;
}

void Disintegrator_explode (edict_t *self, cplane_t *plane)
{
	vec3_t	dir;
	
	if (!plane)
		VectorClear (dir);
	else
		VectorScale (plane->normal, 256, dir);
	
	gi.WriteByte (svc_temp_entity);
	gi.WriteByte (TE_TRACKER_EXPLOSION);
	gi.WritePosition (self->s.origin);
	gi.multicast (self->s.origin, MULTICAST_PVS);
	
	//	gi.sound (self, CHAN_VOICE, gi.soundindex ("weapons/disrupthit.wav"), 1, ATTN_NORM, 0);
	//	tracker_boom_spawn(self->s.origin);
	
	G_FreeEdict (self);
}

void Disintegrator_touch (edict_t *self, edict_t *other, cplane_t *plane, csurface_t *surf)
{
	float	damagetime;
	
	if (other == self->owner)
		return;
	
	if (surf && (surf->flags & SURF_SKY))
	{
		G_FreeEdict (self);
		return;
	}
	
	if (self->client)
		PlayerNoise(self->owner, self->s.origin, PNOISE_IMPACT);
	
	if (other->takedamage)
	{
		if ((other->svflags & SVF_MONSTER) || other->client)
		{
			if (other->health > 0)		// knockback only for living creatures
			{
				// PMM - kickback was times 4 .. reduced to 3
				// now this does no damage, just knockback
				T_Damage (other, self, self->owner, self->velocity, self->s.origin, plane->normal,
					/* self->dmg */ 0, (self->dmg*3), TRACKER_IMPACT_FLAGS, MOD_DISRUPTOR);
					
					if (!(other->flags & (FL_FLY|FL_SWIM)))
						other->velocity[2] += 140;
					
					damagetime = ((float)self->dmg)*FRAMETIME;
					damagetime = damagetime / TRACKER_DAMAGE_TIME;
					//				gi.dprintf ("damage is %f\n", damagetime);
					
					Disintegrator_pain_daemon_spawn (self->owner, other, (int)damagetime);
			}
			else						// lots of damage (almost autogib) for dead bodies
			{
				T_Damage (other, self, self->owner, self->velocity, self->s.origin, plane->normal,
					self->dmg*4, (self->dmg*3), TRACKER_IMPACT_FLAGS, MOD_DISRUPTOR);
			}
		}
		else	// full damage in one shot for inanimate objects
		{
			T_Damage (other, self, self->owner, self->velocity, self->s.origin, plane->normal,
				self->dmg, (self->dmg*3), TRACKER_IMPACT_FLAGS, MOD_DISRUPTOR);
		}
	}
	
	Disintegrator_explode (self, plane);
	return;
}

void Disintegrator_fly (edict_t *self)
{
	vec3_t	dest;
	vec3_t	dir;
	vec3_t	center;
	
	if ((!self->enemy) || (!self->enemy->inuse) || (self->enemy->health < 1))
	{
		Disintegrator_explode (self, NULL);
		return;
	}
	/*
	VectorCopy (self->enemy->s.origin, dest);
	if (self->enemy->client)
	dest[2] += self->enemy->viewheight;
	*/
	// PMM - try to hunt for center of enemy, if possible and not client
	if (self->enemy->client)
	{
		VectorCopy (self->enemy->s.origin, dest);
		dest[2] += self->enemy->viewheight;
	}
	// paranoia
	else if (VectorCompare(self->enemy->absmin, vec3_origin) || VectorCompare(self->enemy->absmax, vec3_origin))
	{
		VectorCopy (self->enemy->s.origin, dest);
	}
	else
	{
		VectorMA (vec3_origin, 0.5, self->enemy->absmin, center);
		VectorMA (center, 0.5, self->enemy->absmax, center);
		VectorCopy (center, dest);
	}
	
	VectorSubtract (dest, self->s.origin, dir);
	VectorNormalize (dir);
	vectoangles2 (dir, self->s.angles);
	VectorScale (dir, self->speed, self->velocity);
	VectorCopy(dest, self->monsterinfo.saved_goal);
	
	self->nextthink = level.time + 0.1f;
}

void weapon_Disintegrator_fire (edict_t *self)
{
	vec3_t		forward, right, start, end, offset, mins, maxs;
	edict_t		*enemy;
	trace_t		tr;
	int			damage;
	
	if (self->client->ps.gunframe == 12)
	{
		gi.sound(self, CHAN_AUTO, gi.soundindex("weapons/hyprbd1a.wav"), 1, ATTN_NORM, 0);
		self->client->weapon_sound = 0;
		self->client->ps.gunframe++;
		return;
	}
	
	self->client->weapon_sound = gi.soundindex("weapons/hyprbl1a.wav");
	
	// PMM - felt a little high at 25
	/*
	if (deathmatch->value)
	damage = 30;
	else
	*/
	
	damage = DIS_DAMAGE;	//defined in l_disintegrator.h
	
	if (is_quad)
		damage *= 4;            //pgm
	
	VectorSet(mins, -16, -16, -16);
	VectorSet(maxs, 16, 16, 16);
	AngleVectors (self->client->v_angle, forward, right, NULL);
	VectorSet(offset, 24, 8, self->viewheight - 8.0f);
	P_ProjectSource (self->client, self->s.origin, offset, forward, right, start);
	
	// FIXME - can we shorten this? do we need to?
	VectorMA (start, 8192, forward, end);
	enemy = NULL;
	//PMM - doing two traces .. one point and one box.
	tr = gi.trace (start, vec3_origin, vec3_origin, end, self, MASK_SHOT);
	if (tr.ent != world)
	{
		if (tr.ent->svflags & SVF_MONSTER || tr.ent->client || tr.ent->takedamage)
		{
			if (tr.ent->health > 0)
				enemy = tr.ent;
		}
	}
	else
	{
		tr = gi.trace (start, mins, maxs, end, self, MASK_SHOT);
		if (tr.ent != world)
		{
			if (tr.ent->svflags & SVF_MONSTER || tr.ent->client || tr.ent->takedamage)
			{
				if (tr.ent->health > 0)
					enemy = tr.ent;
			}
		}
	}
	
	VectorScale (forward, -2, self->client->kick_origin);
	self->client->kick_angles[0] = -1;
	
	fire_Disintegrator (self, start, forward, damage, DIS_SPEED, enemy);
	/*
	// send muzzle flash
	gi.WriteByte (svc_muzzleflash);
	gi.WriteShort (self-g_edicts);
	gi.WriteByte (MZ_TRACKER);
	gi.multicast (self->s.origin, MULTICAST_PVS);
	*/
	PlayerNoise(self, start, PNOISE_WEAPON);
	
	self->client->ps.gunframe++;
	if ((! ( (int)dmflags->value & DF_INFINITE_AMMO ) )
		&&(self->client->ammo_index))
		self->client->pers.inventory[self->client->ammo_index] -= self->client->pers.weapon->quantity;
}

