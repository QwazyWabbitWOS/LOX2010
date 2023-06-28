
#include "g_local.h"
#include "g_team.h"
#include "l_positron.h"
#include "l_dm_grenades.h"

void SpawnLeadExplosion(vec3_t start, edict_t* self, float damage, edict_t* ignore, float radius, int mod)
{
	edict_t* xplosion;

	xplosion = G_Spawn();
	VectorCopy(start, xplosion->s.origin);
	T_RadiusDamage(xplosion, self, damage, world, radius, mod);
	G_FreeEdict(xplosion);

}

// Some forward declarations... 
void ThrowDebris(edict_t* self, char* modelname, float speed, vec3_t origin);
void G_Spawn_Trails(int type, vec3_t start, vec3_t endpos, vec3_t origin);
void G_Spawn_Sparks(int type, vec3_t start, vec3_t movdir, vec3_t origin);
void G_Spawn_Explosion(int type, vec3_t start, vec3_t origin);
qboolean G_Within_Radius(vec3_t start, vec3_t end, float rad);
//====================================================== 
// True if Ent is valid, has client, and edict_t inuse. 
//====================================================== 
qboolean G_EntExists(edict_t* ent)
{
	return (qboolean)((ent) && (ent->client) && (ent->inuse));
}

//====================================================== 
// True if ent is not DEAD or DEAD or DEAD (and BURIED!) 
//====================================================== 
qboolean G_ClientNotDead(edict_t* ent)
{
	qboolean buried = true;
	qboolean b1 = (qboolean)(ent->client->ps.pmove.pm_type != PM_DEAD);
	qboolean b2 = (qboolean)(ent->deadflag != DEAD_DEAD);
	qboolean b3 = (qboolean)(ent->health > 0);
	return (qboolean)((b3 || b2 || b1) && (buried));
}

//====================================================== 
// True if ent is not DEAD and not just did a Respawn. 
//====================================================== 
qboolean G_ClientInGame(edict_t* ent)
{
	if (!G_EntExists(ent))
		return false;
	if (!G_ClientNotDead(ent))
		return false;
	return (qboolean)(ent->client->respawn_time + 5.0 < level.time);
}


//====================================================== 
//============== BATON WEAPON ROUTINES ================= 
//====================================================== 

//====================================================== 
void Baton_Explode(edict_t* baton)
{

	// Immediately Flag as OFF 
	baton->owner->client->baton = 0;

	// Blow up the Baton in a big fireball. 
	G_Spawn_Explosion(TE_EXPLOSION2, baton->s.origin, baton->s.origin);

	// Send small debris flying all around.. 
	ThrowDebris(baton, "models/objects/debris2/tris.md2", 1.50f, baton->s.origin);
	ThrowDebris(baton, "models/objects/debris1/tris.md2", 2.50f, baton->s.origin);
	ThrowDebris(baton, "models/objects/debris1/tris.md2", 3.50f, baton->s.origin);
	ThrowDebris(baton, "models/objects/debris1/tris.md2", 3.00f, baton->s.origin);
	ThrowDebris(baton, "models/objects/debris2/tris.md2", 1.30f, baton->s.origin);

	// Assign any Radius Damage frags to the Baton's owner.. 
	T_RadiusDamage(baton, baton->owner, baton->dmg, NULL, baton->dmg_radius, MOD_SPLASH);

	G_FreeEdict(baton); // Free the baton entity. 
}

//======================================================== 
// True if Baton's timer has expired (or owner has died!) 
//======================================================== 
qboolean Baton_Expired(edict_t* baton)
{

	if ((baton->delay < level.time) || (baton->owner->client->baton == 0))
	{
		Baton_Explode(baton);
		return true;
	}
	else
		return false;
}

//================================================================== 
// Fires beam at all visible players within 1000 unit radius 
//================================================================== 
void Baton_Think(edict_t* baton)
{
	edict_t* ent = NULL;
	trace_t tr;
	vec3_t zvec = { 0,0,0 };
	vec3_t start = { 0 }, end = { 0 };
	int i;

	if (Baton_Expired(baton))
		return; // Done! 

	baton->viewheight = 16; // required for visible(). 

	// Find ALL visible ents within 1000 unit radius... 
	for (i = 0; i < game.maxclients; i++)
	{
		ent = g_edicts + i + 1;
		if (!G_ClientInGame(ent))
			continue;
		if (ent == baton)
			continue;
		// if (ent == baton->owner)
		//	continue; // Exempt Baton's owner?? 
		if (!ent->takedamage)
			continue;
		if (!visible(baton, ent))
			continue;
		if (!G_Within_Radius(baton->s.origin, ent->s.origin, 1000))
			continue;
		// Okay, Fire at this ent!! 
		VectorCopy(baton->s.origin, start);
		VectorCopy(ent->s.origin, end);
		end[2] += 8;
		tr = gi.trace(start, ent->mins, ent->maxs, end, NULL, MASK_SHOT);
		G_Spawn_Trails(TE_BFG_LASER, start, tr.endpos, tr.endpos); // FIRE!! 
		if (G_EntExists(tr.ent) && (tr.ent->takedamage))
			T_Damage(tr.ent, baton->owner, baton->owner, zvec, start, zvec, baton->dmg, 1, 0, MOD_BATON);
	}

	baton->nextthink = level.time + 0.1f; // Re-Target in 1/10 second.. 
}

//================================================================== 
// Burn 10 sec fuse of green sparks before targeting visible ents.. 
//================================================================== 
void Baton_Fuse(edict_t* baton)
{
	vec3_t zvec = { 0,0,0 };

	if (Baton_Expired(baton))
		return;

	// Has the 10 sec fuse burned out yet? 
	if (baton->delay - level.time < 0)
	{
		baton->delay = level.time + 30.0f; // 30 sec to self destruct. 
		baton->think = Baton_Think; // Activate Laser Scan! 
		baton->nextthink = level.time + 0.1f; // Think on next frame. 
		return;
	}

	// Shower of fuse sparks... 
	G_Spawn_Sparks(TE_SCREEN_SPARKS, baton->s.origin, zvec, baton->s.origin);

	baton->nextthink = level.time + 1.0f; // Burn fuse every 1 sec.. 
}

//==========================================================

void Baton_Touch(edict_t* baton, edict_t* other, cplane_t* plane, csurface_t* surf)
{

	// Hit Sky? Turn Baton OFF and exit quietly.. 
	if (surf && surf->flags & SURF_SKY)
	{
		baton->owner->client->baton = 0;
		G_FreeEdict(baton);
		return;
	}

	// Hit a Player? then Detonate! 
	if (G_EntExists(other))
	{
		Baton_Explode(baton);
		return;
	}

	// 
	// Hit a World Surface! 
	// 

	baton->takedamage = DAMAGE_YES; // Can be killed now! 

	// Play a sound at point of Baton's impact.. 
	gi.sound(baton, CHAN_VOICE, gi.soundindex("misc/menu1.wav"), 1, ATTN_IDLE, 0);

	// Stop baton's forward motion. 
	VectorClear(baton->avelocity);
	VectorClear(baton->velocity);

	baton->delay = level.time + 10.0f; // Set 10 sec fuse timer. 

	baton->touch = NULL; // Don't touch this! 

	baton->think = Baton_Fuse;

	// Ignite the Baton's fuse on next frame.. 
	baton->nextthink = level.time + 0.1f;
}

//========================================================== 

void Fire_Baton(edict_t* ent)
{
	edict_t* baton = NULL;
	vec3_t offset = { 0,0,0 }, start = { 0,0,0 };
	vec3_t forward = { 0,0,0 }, right = { 0,0,0 };

	// Derive point of Baton's origin 
	AngleVectors(ent->client->v_angle, forward, right, NULL);
	VectorSet(offset, 8, 8, ent->viewheight - 8.0f); // Off-Handed Launch 
	P_ProjectSource_Reverse(ent->client, ent->s.origin, offset, forward, right, start);

	baton = G_Spawn();
	baton->classname = "Baton";
	baton->classnum = CN_BATON;
	baton->owner = ent;
	baton->takedamage = DAMAGE_NO; // No Damage until Touch(). 
	baton->clipmask = MASK_SHOT; // Ability to be hit by weapon's fire. 
	baton->health = 30; // Hard to target but easy to Kill 
	baton->max_health = 30;
	VectorCopy(start, baton->s.origin);
	VectorCopy(forward, baton->movedir);
	vectoangles(forward, baton->s.angles);
	VectorScale(forward, 800, baton->velocity); // Baton's toss speed = 800.. 
	baton->movetype = MOVETYPE_FLY;
	baton->solid = SOLID_BBOX;
	baton->s.effects = 0;
	VectorSet(baton->mins, -5, -5, -15); // size of bbox for touch 
	VectorSet(baton->maxs, 5, 5, 15); // size of bbox for touch 
	baton->dmg = 80; // Damage amount.. 
	baton->dmg_radius = 300; // Explosion Damage radius 
	baton->s.modelindex = gi.modelindex("models/objects/minelite/light2/tris.md2");
	baton->s.modelindex2 = 0;
	VectorSet(baton->avelocity, 0, 0, -1600); // Add some spin to baton flight.. 

	baton->touch = Baton_Touch; // Function to call upon impact 

	baton->think = NULL; // Touch will ignite fuse timer 
	baton->nextthink = 0;

	ent->client->baton = 1; // Flag baton as ON! 

	gi.linkentity(baton);
}



//  =================
//  check_dodge
//  This is a support routine used when a client is firing
//  a non-instant attack weapon.  It checks to see if a
//  monster's dodge function should be called.
//  =================
void check_dodge(edict_t* self, vec3_t start, vec3_t dir, int speed)
{
	vec3_t	end, v = { 0 };
	trace_t	tr;
	float	eta;

	// easy mode only ducks one quarter the time
	if (skill->value == 0)
	{
		if (random() > 0.25)
			return;
	}
	VectorMA(start, 8192, dir, end);
	tr = gi.trace(start, NULL, NULL, end, self, MASK_SHOT);
	if ((tr.ent) && (tr.ent->svflags & SVF_MONSTER) && (tr.ent->health > 0) && (tr.ent->monsterinfo.dodge) && infront(tr.ent, self))
	{
		VectorSubtract(tr.endpos, start, v);
		eta = (VectorLength(v) - tr.ent->maxs[0]) / speed;
		tr.ent->monsterinfo.dodge(tr.ent, self, eta);
	}
}


//=================
// Used for all impact (hit/punch/slash) attacks
//=================
qboolean fire_hit(edict_t* self, vec3_t aim, int damage, int kick)
{
	trace_t		tr;
	vec3_t		forward, right, up;
	vec3_t		v = { 0 };
	vec3_t		point;
	float		range = 0;
	vec3_t		dir = { 0 };

	//see if enemy is in range
	VectorSubtract(self->enemy->s.origin, self->s.origin, dir);
	range = VectorLength(dir);
	if (range > aim[0])
		return false;

	if (aim[1] > self->mins[0] && aim[1] < self->maxs[0])
	{
		// the hit is straight on so back the range up to the edge of their bbox
		range -= self->enemy->maxs[0];
	}
	else
	{
		// this is a side hit so adjust the "right" value out to the edge of their bbox
		if (aim[1] < 0)
			aim[1] = self->enemy->mins[0];
		else
			aim[1] = self->enemy->maxs[0];
	}

	VectorMA(self->s.origin, range, dir, point);

	tr = gi.trace(self->s.origin, NULL, NULL, point, self, MASK_SHOT);
	if (tr.fraction < 1)
	{
		if (!tr.ent->takedamage)
			return false;
		// if it will hit any client/monster then hit the one we wanted to hit
		if ((tr.ent->svflags & SVF_MONSTER) || (tr.ent->client))
			tr.ent = self->enemy;
	}

	AngleVectors(self->s.angles, forward, right, up);
	VectorMA(self->s.origin, range, forward, point);
	VectorMA(point, aim[1], right, point);
	VectorMA(point, aim[2], up, point);
	VectorSubtract(point, self->enemy->s.origin, dir);

	// do the damage
	T_Damage(tr.ent, self, self, dir, point, vec3_origin, damage, kick / 2, DAMAGE_NO_KNOCKBACK, MOD_HIT);

	if (!(tr.ent->svflags & SVF_MONSTER) && (!tr.ent->client))
		return false;

	// do our special form of knockback here
	VectorMA(self->enemy->absmin, 0.5, self->enemy->size, v);
	VectorSubtract(v, point, v);
	VectorNormalize(v);
	VectorMA(self->enemy->velocity, kick, v, self->enemy->velocity);
	if (self->enemy->velocity[2] > 0)
		self->enemy->groundentity = NULL;
	return true;
}


//=================
// fire_lead
//
//  This is an internal support routine used for bullet/pellet based weapons.
//=================
static void fire_lead(edict_t* self, vec3_t start, vec3_t aimdir, int damage, int kick, int te_impact, int hspread, int vspread, int mod)
{
	trace_t		tr;
	vec3_t		dir, forward, right, up, end, water_start = { 0 };
	float		r, u;
	qboolean	water = false;
	int			content_mask = MASK_SHOT | MASK_WATER;

	tr = gi.trace(self->s.origin, NULL, NULL, start, self, MASK_SHOT);
	if (!(tr.fraction < 1.0))
	{
		vectoangles(aimdir, dir);
		AngleVectors(dir, forward, right, up);

		r = crandom() * hspread;
		u = crandom() * vspread;
		VectorMA(start, 8192, forward, end);
		VectorMA(end, r, right, end);
		VectorMA(end, u, up, end);

		if (gi.pointcontents(start) & MASK_WATER)
		{
			water = true;
			VectorCopy(start, water_start);
			content_mask &= ~MASK_WATER;
		}

		tr = gi.trace(start, NULL, NULL, end, self, content_mask);

		// see if we hit water
		if (tr.contents & MASK_WATER)
		{
			int		color;

			water = true;
			VectorCopy(tr.endpos, water_start);

			if (!VectorCompare(start, tr.endpos))
			{
				if (tr.contents & CONTENTS_WATER)
				{
					if (strcmp(tr.surface->name, "*brwater") == 0)
						color = SPLASH_BROWN_WATER;
					else
						color = SPLASH_BLUE_WATER;
				}
				else if (tr.contents & CONTENTS_SLIME)
					color = SPLASH_SLIME;
				else if (tr.contents & CONTENTS_LAVA)
					color = SPLASH_LAVA;
				else
					color = SPLASH_UNKNOWN;

				if (color != SPLASH_UNKNOWN)
				{
					gi.WriteByte(svc_temp_entity);
					gi.WriteByte(TE_SPLASH);
					gi.WriteByte(8); //count
					gi.WritePosition(tr.endpos);
					gi.WriteDir(tr.plane.normal);
					gi.WriteByte(color);
					gi.multicast(tr.endpos, MULTICAST_PVS);
				}

				// change bullet's course when it enters water
				VectorSubtract(end, start, dir);
				vectoangles(dir, dir);
				AngleVectors(dir, forward, right, up);
				r = crandom() * hspread * 2;
				u = crandom() * vspread * 2;
				VectorMA(water_start, 8192, forward, end);
				VectorMA(end, r, right, end);
				VectorMA(end, u, up, end);
			}

			// re-trace ignoring water this time
			tr = gi.trace(water_start, NULL, NULL, end, self, MASK_SHOT);
		}
	}

	// send gun puff / flash
	if (!((tr.surface) && (tr.surface->flags & SURF_SKY)))
	{
		if (tr.fraction < 1.0)
		{
			if (tr.ent->takedamage)
			{
				T_Damage(tr.ent, self, self, aimdir, tr.endpos, tr.plane.normal, damage, kick, DAMAGE_BULLET, mod);
			}
			else
			{
				if (tr.surface && strncmp(tr.surface->name, "sky", 3) != 0)
				{
					gi.WriteByte(svc_temp_entity);
					gi.WriteByte(te_impact);
					gi.WritePosition(tr.endpos);
					gi.WriteDir(tr.plane.normal);
					gi.multicast(tr.endpos, MULTICAST_PVS);

					if (self->client)
						PlayerNoise(self, tr.endpos, PNOISE_IMPACT);
				}
			}
		}
	}

	// if went through water, determine where the end and make a bubble trail
	if (water)
	{
		vec3_t	pos;

		VectorSubtract(tr.endpos, water_start, dir);
		VectorNormalize(dir);
		VectorMA(tr.endpos, -2, dir, pos);
		if (gi.pointcontents(pos) & MASK_WATER)
			VectorCopy(pos, tr.endpos);
		else
			tr = gi.trace(pos, NULL, NULL, water_start, tr.ent, MASK_WATER);

		VectorAdd(water_start, tr.endpos, pos);
		VectorScale(pos, 0.5, pos);

		gi.WriteByte(svc_temp_entity);
		gi.WriteByte(TE_BUBBLETRAIL);
		gi.WritePosition(water_start);
		gi.WritePosition(tr.endpos);
		gi.multicast(pos, MULTICAST_PVS);
	}
}


//=================
// fire_bullet//
//
//  Fires a single round.  Used for machinegun and chaingun.  Would be fine for
//  pistols, rifles, etc....
//  =================
void fire_bullet(edict_t* self, vec3_t start, vec3_t aimdir, int damage, int kick, int hspread, int vspread, int mod)
{
	fire_lead(self, start, aimdir, damage, kick, TE_GUNSHOT, hspread, vspread, mod);
}

/***************************************************/
/**************  Explosive Machinegun **************/
/***************************************************/

//=================
//fire_explosive_lead
//
//This is an internal support routine used for explosive bullet/pellet based weapons.
//=================
void fire_explosive_lead(edict_t* self, vec3_t start, vec3_t aimdir, int damage, int kick, int te_impact, int hspread, int vspread, int mod)
{
	trace_t		tr;
	vec3_t		dir;
	vec3_t		forward, right, up;
	vec3_t		end;
	float		r;
	float		u;
	vec3_t		water_start = { 0 };
	qboolean	water = false;
	int			content_mask = MASK_SHOT | MASK_WATER;

	tr = gi.trace(self->s.origin, NULL, NULL, start, self, MASK_SHOT);
	if (!(tr.fraction < 1.0))
	{
		vectoangles(aimdir, dir);
		AngleVectors(dir, forward, right, up);

		r = crandom() * hspread;
		u = crandom() * vspread;
		VectorMA(start, 8192, forward, end);
		VectorMA(end, r, right, end);
		VectorMA(end, u, up, end);

		if (gi.pointcontents(start) & MASK_WATER)
		{
			water = true;
			VectorCopy(start, water_start);
			content_mask &= ~MASK_WATER;
		}

		tr = gi.trace(start, NULL, NULL, end, self, content_mask);

		// see if we hit water
		if (tr.contents & MASK_WATER)
		{
			int		color;

			water = true;
			VectorCopy(tr.endpos, water_start);

			if (!VectorCompare(start, tr.endpos))
			{
				if (tr.contents & CONTENTS_WATER)
				{
					if (strcmp(tr.surface->name, "*brwater") == 0)
						color = SPLASH_BROWN_WATER;
					else
						color = SPLASH_BLUE_WATER;
				}
				else if (tr.contents & CONTENTS_SLIME)
					color = SPLASH_SLIME;
				else if (tr.contents & CONTENTS_LAVA)
					color = SPLASH_LAVA;
				else
					color = SPLASH_UNKNOWN;

				if (color != SPLASH_UNKNOWN)
				{
					gi.WriteByte(svc_temp_entity);
					gi.WriteByte(TE_SPLASH); //type
					gi.WriteByte(8);  //count
					gi.WritePosition(tr.endpos);
					gi.WriteDir(tr.plane.normal);
					gi.WriteByte(color);
					gi.multicast(tr.endpos, MULTICAST_PVS);
				}

				// change bullet's course when it enters water
				VectorSubtract(end, start, dir);
				vectoangles(dir, dir);
				AngleVectors(dir, forward, right, up);
				r = crandom() * hspread * 2;
				u = crandom() * vspread * 2;
				VectorMA(water_start, 8192, forward, end);
				VectorMA(end, r, right, end);
				VectorMA(end, u, up, end);
			}

			// re-trace ignoring water this time
			tr = gi.trace(water_start, NULL, NULL, end, self, MASK_SHOT);
		}
	}

	// send gun puff / flash
	if (!((tr.surface) && (tr.surface->flags & SURF_SKY)))
	{
		if (tr.fraction < 1.0)
		{
			if (tr.ent->takedamage)
			{
				gi.WriteByte(svc_temp_entity);
				gi.WriteByte(te_impact);
				gi.WritePosition(tr.endpos);
				gi.WriteDir(tr.plane.normal);
				gi.multicast(tr.endpos, MULTICAST_PVS);

				T_Damage(tr.ent, self, self, aimdir, tr.endpos, tr.plane.normal, 8, kick, DAMAGE_BULLET, mod);
			}
			else
			{
				if (tr.surface && strncmp(tr.surface->name, "sky", 3) != 0)
				{
					gi.WriteByte(svc_temp_entity);
					gi.WriteByte(te_impact);
					gi.WritePosition(tr.endpos);
					gi.WriteDir(tr.plane.normal);
					gi.multicast(tr.endpos, MULTICAST_PVS);

					if (self->client)
						PlayerNoise(self, tr.endpos, PNOISE_IMPACT);
				}
			}
			SpawnLeadExplosion(tr.endpos, self, damage, world, damage + 40, mod);
		}
	}

	// if went through water, determine where the end and make a bubble trail
	if (water)
	{
		vec3_t	pos;

		VectorSubtract(tr.endpos, water_start, dir);
		VectorNormalize(dir);
		VectorMA(tr.endpos, -2, dir, pos);
		if (gi.pointcontents(pos) & MASK_WATER)
			VectorCopy(pos, tr.endpos);
		else
			tr = gi.trace(pos, NULL, NULL, water_start, tr.ent, MASK_WATER);

		VectorAdd(water_start, tr.endpos, pos);
		VectorScale(pos, 0.5, pos);

		gi.WriteByte(svc_temp_entity);
		gi.WriteByte(TE_BUBBLETRAIL);
		gi.WritePosition(water_start);
		gi.WritePosition(tr.endpos);
		gi.multicast(pos, MULTICAST_PVS);
	}
}


//=================
//fire_explosive_bullet
//
//  Fires a single round.  Used for machinegun and chaingun.  Would be fine for
//  pistols, rifles, etc....
//=================
void fire_explosive_bullet(edict_t* self, vec3_t start, vec3_t aimdir, int damage, int kick, int hspread, int vspread, int mod)
{
	fire_explosive_lead(self, start, aimdir, damage, kick, TE_BLASTER, hspread, vspread, mod);
}

//=================
//  fire_shotgun
//  
// Shoots shotgun pellets.  Used by shotgun and super shotgun.
//=================
void fire_explosive_shotgun(edict_t* self, vec3_t start, vec3_t aimdir, int damage, int kick, int te_impact, int hspread, int vspread, int count, int mod)
{
	int		i;

	for (i = 0; i < count; i++)
		fire_explosive_lead(self, start, aimdir, damage, kick, TE_BLASTER, hspread, vspread, mod);
}

//=================
//  fire_shotgun
//  
// Shoots shotgun pellets.  Used by shotgun and super shotgun.
//=================
void fire_shotgun(edict_t* self, vec3_t start, vec3_t aimdir, int damage, int kick, int hspread, int vspread, int count, int mod)
{
	int		i;

	for (i = 0; i < count; i++)
		fire_lead(self, start, aimdir, damage, kick, TE_SHOTGUN, hspread, vspread, mod);
}

//=================
//  fire_blaster
//  
//	Fires a single blaster bolt.  Used by the blaster and hyper blaster.
//=================
void blaster_touch(edict_t* self, edict_t* other, cplane_t* plane, csurface_t* surf)
{
	if (other == self->owner)
		return;

	if (surf && (surf->flags & SURF_SKY))
	{
		G_FreeEdict(self);
		return;
	}

	if (self->owner->client)
		PlayerNoise(self->owner, self->s.origin, PNOISE_IMPACT);

	if (other->takedamage)
	{
		int mod;

		// Set up the means of death.
		mod = MOD_BLASTER;

		T_Damage(other, self, self->owner, self->velocity, self->s.origin,
			plane->normal, self->dmg, 1, DAMAGE_ENERGY, mod);
	}
	else
	{
		// return;
		gi.WriteByte(svc_temp_entity);
		gi.WriteByte(TE_BLASTER);
		gi.WritePosition(self->s.origin);
		if (!plane)
			gi.WriteDir(vec3_origin);
		else
			gi.WriteDir(plane->normal);
		gi.multicast(self->s.origin, MULTICAST_PVS);
	}

	G_FreeEdict(self);
}

void fire_blaster(edict_t* self, vec3_t start, vec3_t dir, int damage, int speed, int effect)
{
	edict_t* bolt;
	trace_t tr;
	int laser_colour[] =
	{
		0xf2f2f0f0,	     // red
		0xf3f3f1f1,	     // blue
	};

	VectorNormalize(dir);

	// Only change hand blaster effect
	if (effect & EF_BLASTER)
	{
		bolt = G_Spawn();
		bolt->svflags = SVF_DEADMONSTER;
		VectorCopy(start, bolt->s.origin);
		vectoangles(dir, bolt->s.angles);
		VectorScale(dir, speed, bolt->velocity);
		VectorAdd(start, bolt->velocity, bolt->s.old_origin);
		bolt->clipmask = MASK_SHOT;

		bolt->movetype = MOVETYPE_FLYMISSILE;
		bolt->solid = SOLID_BBOX;
		bolt->s.renderfx |= RF_BEAM | RF_TRANSLUCENT;
		bolt->s.modelindex = 1;
		bolt->owner = self;
		bolt->classname = "bolt";
		bolt->classnum = CN_BOLT;
		bolt->s.frame = 3;

		if (ctf->value && self->client)
		{
			// Team colors for blaster lasers.
			if (self->client->resp.ctf_team == CTF_TEAM1)
				bolt->s.skinnum = laser_colour[0];
			else
				bolt->s.skinnum = laser_colour[1];
		}
		else
			bolt->s.skinnum = laser_colour[((int)(random() * 1500)) % 2];

		VectorClear(bolt->mins);
		VectorClear(bolt->maxs);

		bolt->touch = blaster_touch;
		bolt->nextthink = level.time + 4;
		bolt->think = G_FreeEdict;
		bolt->dmg = damage;

		gi.linkentity(bolt);

		if (self->client)
			check_dodge(self, bolt->s.origin, dir, speed);

		tr = gi.trace(self->s.origin, NULL, NULL, bolt->s.origin, bolt, MASK_SHOT);
		if (tr.fraction < 1.0)
		{
			VectorMA(bolt->s.origin, -10, dir, bolt->s.origin);
			bolt->touch(bolt, tr.ent, NULL, NULL);
		}

		return;
	}

	bolt = G_Spawn();
	bolt->svflags = SVF_DEADMONSTER;
	VectorCopy(start, bolt->s.origin);
	VectorCopy(start, bolt->s.old_origin);
	vectoangles(dir, bolt->s.angles);
	VectorScale(dir, speed, bolt->velocity);
	bolt->movetype = MOVETYPE_FLYMISSILE;
	bolt->clipmask = MASK_SHOT;
	bolt->solid = SOLID_BBOX;
	bolt->s.effects |= effect;
	VectorClear(bolt->mins);
	VectorClear(bolt->maxs);
	bolt->s.modelindex = gi.modelindex("models/objects/laser/tris.md2");
	bolt->s.sound = gi.soundindex("misc/lasfly.wav");
	bolt->owner = self;
	bolt->touch = blaster_touch;
	bolt->nextthink = level.time + 2;
	bolt->think = G_FreeEdict;
	bolt->dmg = damage;
	bolt->classname = "bolt";
	bolt->classnum = CN_BOLT;
	gi.linkentity(bolt);

	if (self->client)
		check_dodge(self, bolt->s.origin, dir, speed);

	tr = gi.trace(self->s.origin, NULL, NULL, bolt->s.origin, bolt, MASK_SHOT);
	if (tr.fraction < 1.0)
	{
		VectorMA(bolt->s.origin, -10, dir, bolt->s.origin);
		bolt->touch(bolt, tr.ent, NULL, NULL);
	}
}

void sniper_touch(edict_t* self, edict_t* other, cplane_t* plane, csurface_t* surf)
{
	if (other == self->owner)
		return;

	if (surf && (surf->flags & SURF_SKY))
	{
		G_FreeEdict(self);
		return;
	}

	if (self->owner->client)
		PlayerNoise(self->owner, self->s.origin, PNOISE_IMPACT);

	if (other->takedamage)
	{
		int mod;

		// Set up the means of death.
		mod = MOD_SNIPER;

		T_Damage(other, self, self->owner, self->velocity, self->s.origin,
			plane->normal, self->dmg, 1, 0, mod);
	}
	else
	{
		// return;
		gi.WriteByte(svc_temp_entity);
		gi.WriteByte(TE_SHOTGUN);
		gi.WritePosition(self->s.origin);
		if (!plane)
			gi.WriteDir(vec3_origin);
		else
			gi.WriteDir(plane->normal);
		gi.multicast(self->s.origin, MULTICAST_PVS);
	}

	G_FreeEdict(self);
}

void fire_sniper(edict_t* self, vec3_t start, vec3_t dir, int damage, int speed, int effect)
{
	edict_t* bolt;
	trace_t tr;

	VectorNormalize(dir);

	bolt = G_Spawn();
	VectorCopy(start, bolt->s.origin);
	VectorCopy(start, bolt->s.old_origin);
	vectoangles(dir, bolt->s.angles);
	VectorScale(dir, speed, bolt->velocity);
	bolt->movetype = MOVETYPE_FLYMISSILE;
	bolt->clipmask = MASK_SHOT;
	bolt->solid = SOLID_BBOX;
	bolt->s.effects |= 0;
	VectorClear(bolt->mins);
	VectorClear(bolt->maxs);
	bolt->model = "models/monsters/parasite/tip/tris.md2";
	bolt->s.modelindex = gi.modelindex("models/monsters/parasite/tip/tris.md2");
	bolt->s.sound = gi.soundindex("misc/lasfly.wav");
	bolt->owner = self;
	bolt->touch = sniper_touch;
	bolt->nextthink = level.time + 2;
	bolt->think = G_FreeEdict;
	bolt->dmg = damage;
	bolt->classname = "bolt";
	bolt->classnum = CN_BOLT;
	gi.linkentity(bolt);

	if (self->client)
		check_dodge(self, bolt->s.origin, dir, speed);

	tr = gi.trace(self->s.origin, NULL, NULL, bolt->s.origin, bolt, MASK_SHOT);
	if (tr.fraction < 1.0)
	{
		VectorMA(bolt->s.origin, -10, dir, bolt->s.origin);
		bolt->touch(bolt, tr.ent, NULL, NULL);
	}
}

// plasma begin
void plasma_touch(edict_t* self, edict_t* other, cplane_t* plane, csurface_t* surf)
{
	if (other == self->owner)
		return;

	if (surf && (surf->flags & SURF_SKY))
	{
		G_FreeEdict(self);
		return;
	}

	if (self->owner->client)
		PlayerNoise(self->owner, self->s.origin, PNOISE_IMPACT);

	if (other->takedamage)
	{
		int mod;

		// Set up the means of death.
		mod = MOD_PLASMAGUN;
		if (i_fragban & WFB_PLASMARIFLE)
			mod |= MOD_NOFRAG;

		T_Damage(other, self, self->owner, self->velocity, self->s.origin,
			plane->normal, self->dmg, 1, DAMAGE_ENERGY, mod);
	}
	else
	{
		gi.WriteByte(svc_temp_entity);
		gi.WriteByte(TE_BLASTER);
		gi.WritePosition(self->s.origin);
		if (!plane)
			gi.WriteDir(vec3_origin);
		else
			gi.WriteDir(plane->normal);
		gi.multicast(self->s.origin, MULTICAST_PVS);
	}

	G_FreeEdict(self);
}

void fire_plasma(edict_t* self, vec3_t start, vec3_t dir, int damage, int speed, int effect)
{
	edict_t* bolt;
	trace_t tr;

	VectorNormalize(dir);
	bolt = G_Spawn();
	VectorCopy(start, bolt->s.origin);
	VectorCopy(start, bolt->s.old_origin);
	vectoangles(dir, bolt->s.angles);
	VectorScale(dir, speed, bolt->velocity);
	bolt->movetype = MOVETYPE_FLYMISSILE;
	bolt->clipmask = MASK_SHOT;
	bolt->solid = SOLID_BBOX;
	//bolt->s.effects |= EF_COLOR_SHELL;
	VectorClear(bolt->mins);
	VectorClear(bolt->maxs);
	// bolt->s.modelindex = gi.modelindex ("sprites/s_bfg2.sp2");
	// bolt->s.frame = 2;
	bolt->s.modelindex = gi.modelindex("models/objects/laser/tris.md2");
	bolt->s.frame = 0;
	// bolt->s.sound = gi.soundindex ("weapons/bfg__l1a.wav");
	bolt->s.sound = gi.soundindex("misc/lasfly.wav");
	bolt->s.renderfx |= RF_SHELL_GREEN;
	bolt->owner = self;
	bolt->touch = plasma_touch;
	bolt->nextthink = level.time + 2;
	bolt->think = G_FreeEdict;
	bolt->dmg = damage;
	bolt->classname = "plasma";
	bolt->classnum = CN_PLASMA;
	gi.linkentity(bolt);
	if (self->client)
		check_dodge(self, bolt->s.origin, dir, speed);

	tr = gi.trace(self->s.origin, NULL, NULL, bolt->s.origin, bolt, MASK_SHOT);
	if (tr.fraction < 1.0)
	{
		VectorMA(bolt->s.origin, -10, dir, bolt->s.origin);
		bolt->touch(bolt, tr.ent, NULL, NULL);
	}
}

//  =================
//  fire_grenade
//  =================
void Grenade_Touch(edict_t* ent, edict_t* other, cplane_t* plane, csurface_t* surf)
{
	if (other == ent->owner)
		return;
	if (surf && (surf->flags & SURF_SKY))
	{
		G_FreeEdict(ent);
		return;
	}
	if (!other->takedamage)
	{
		if (ent->spawnflags & 1)
		{
			if (random() > 0.5)
				gi.sound(ent, CHAN_VOICE, gi.soundindex("weapons/hgrenb1a.wav"), 1, ATTN_NORM, 0);
			else
				gi.sound(ent, CHAN_VOICE, gi.soundindex("weapons/hgrenb2a.wav"), 1, ATTN_NORM, 0);
		}
		else
			gi.sound(ent, CHAN_VOICE, gi.soundindex("weapons/grenlb1b.wav"), 1, ATTN_NORM, 0);
		return;
	}
	ent->enemy = other;
	Grenade_Explode(ent);
}

void fire_grenade(edict_t* self, vec3_t start, vec3_t aimdir, int damage,
	int speed, float timer, float damage_radius)
{
	edict_t* grenade;
	vec3_t	dir;
	vec3_t	forward, right, up;

	vectoangles(aimdir, dir);
	AngleVectors(dir, forward, right, up);

	grenade = G_Spawn();
	VectorCopy(start, grenade->s.origin);
	VectorScale(aimdir, speed, grenade->velocity);
	VectorMA(grenade->velocity, 200 + crandom() * 10.0f, up, grenade->velocity);
	VectorMA(grenade->velocity, crandom() * 10.0f, right, grenade->velocity);
	VectorSet(grenade->avelocity, 300, 300, 300);
	grenade->movetype = MOVETYPE_BOUNCE;
	grenade->clipmask = MASK_SHOT;
	grenade->solid = SOLID_BBOX;
	grenade->s.effects |= EF_GRENADE;
	VectorClear(grenade->mins);
	VectorClear(grenade->maxs);
	grenade->s.modelindex = gi.modelindex("models/objects/grenade/tris.md2");
	grenade->owner = self;
	grenade->touch = Grenade_Touch;
	grenade->nextthink = level.time + timer;
	grenade->think = Grenade_Explode;
	grenade->dmg = damage;
	grenade->dmg_radius = damage_radius;
	grenade->classname = "grenade";
	grenade->classnum = CN_GRENADE;
	gi.linkentity(grenade);
}

//=================
// fire_rocket
// this is the conventional flame rocket type
//=================
void fire_rocket(edict_t* self, vec3_t start, vec3_t dir, int damage, int speed, float damage_radius, int radius_damage)
{
	edict_t* rocket;

	rocket = G_Spawn();
	VectorCopy(start, rocket->s.origin);
	VectorCopy(dir, rocket->movedir);
	vectoangles(dir, rocket->s.angles);
	VectorScale(dir, speed, rocket->velocity);
	rocket->movetype = MOVETYPE_FLYMISSILE;
	rocket->clipmask = MASK_SHOT;
	rocket->solid = SOLID_BBOX;
	rocket->s.effects |= EF_ROCKET;
	VectorClear(rocket->mins);
	VectorClear(rocket->maxs);
	rocket->s.modelindex = gi.modelindex("models/objects/rocket/tris.md2");
	rocket->owner = self;
	rocket->touch = rocket_touch;
	rocket->nextthink = level.time + 8000.0f / speed;
	rocket->think = G_FreeEdict;
	rocket->dmg = damage;
	rocket->radius_dmg = radius_damage;
	rocket->dmg_radius = damage_radius;
	rocket->s.sound = gi.soundindex("weapons/rockfly.wav");
	rocket->classname = "rocket";
	rocket->classnum = CN_ROCKET;

	if (self->client)
		check_dodge(self, rocket->s.origin, dir, speed);

	gi.linkentity(rocket);
}

/*
=================
rocket_touch

  Grenade explosion + a few glowing trails.  Creates ~15 new entities.
  =================
*/
void rocket_touch(edict_t* ent, edict_t* other, cplane_t* plane, csurface_t* surf)
{
	int		i, mod;
	vec3_t	org = { 0 }, origin;
	float	spd;

	// can't be hit by own rocket
	if (other == ent->owner)
		return;

	// can't hit sky
	if (surf && (surf->flags & SURF_SKY))
	{
		G_FreeEdict(ent);
		return;
	}

	// noise if hits other player
	if (ent->owner->client)
		PlayerNoise(ent->owner, ent->s.origin, PNOISE_IMPACT);

	// calculate position for the explosion entity
	VectorMA(ent->s.origin, -0.02f, ent->velocity, origin);

	// impact damage
	if (other->takedamage)
	{
		// Set up the means of death.
		mod = MOD_ROCKET;
		if (i_fragban & WFB_ROCKETLAUNCHER)
			mod |= MOD_NOFRAG;

		T_Damage(other, ent, ent->owner, ent->velocity, ent->s.origin,
			plane->normal, ent->dmg, 0, 0, mod);
	}

	// make some glowing shrapnel
	spd = 15.0f * ent->dmg / 200;
	for (i = 0; i < 3; i++)
	{
		org[0] = ent->s.origin[0] + crandom() * ent->size[0];
		org[1] = ent->s.origin[1] + crandom() * ent->size[1];
		org[2] = ent->s.origin[2] + crandom() * ent->size[2];
		ThrowShrapnel4(ent, "models/objects/debris2/tris.md2", spd, org);
	}

	make_debris(ent);
	mod = MOD_R_SPLASH;
	if (i_fragban & WFB_ROCKETLAUNCHER)
		mod |= MOD_NOFRAG;
	T_RadiusDamage(ent, ent->owner, (float)ent->radius_dmg, other, ent->dmg_radius,
		mod);
	T_ShockItems(ent);
	T_ShockWave(ent, 255, 1024);
	BecomeNewExplosion(ent);
}


//  ===================
//  Machine Gun Rockets 
//  ===================
void mr_touch(edict_t* ent, edict_t* other, cplane_t* plane, csurface_t* surf)
{
	vec3_t	origin;
	int		mod;

	if (other == ent->owner)
		return;

	if (surf && (surf->flags & SURF_SKY))
	{
		G_FreeEdict(ent);
		return;
	}

	if (ent->owner->client)
		PlayerNoise(ent->owner, ent->s.origin, PNOISE_IMPACT);

	// calculate position for the explosion entity
	VectorMA(ent->s.origin, -0.02f, ent->velocity, origin);

	// Set up the means of death.
	mod = MOD_MACHINEGUN;
	if (i_fragban & WFB_MACHINEROCKETGUN)
		mod |= MOD_NOFRAG;

	if (other->takedamage)
	{
		T_Damage(other, ent, ent->owner, ent->velocity, ent->s.origin,
			plane->normal, ent->dmg, 20, 20, mod);
	}

	T_RadiusDamage(ent, ent->owner, ent->radius_dmg, other, ent->dmg_radius, mod);

	gi.WriteByte(svc_temp_entity);
	if (ent->waterlevel)
		gi.WriteByte(TE_ROCKET_EXPLOSION_WATER);
	else
		gi.WriteByte(TE_ROCKET_EXPLOSION);
	gi.WritePosition(origin);
	// gi.multicast (ent->s.origin, MULTICAST_PHS);
	gi.multicast(ent->s.origin, MULTICAST_PVS);		// WI -- cut down mr impact on game

	G_FreeEdict(ent);
}

//fire machinegun rockets
void fire_mr(edict_t* self, vec3_t start, vec3_t dir, int damage, int speed,
	float damage_radius, int radius_damage)
{
	edict_t* rocket;

	rocket = G_Spawn();
	VectorCopy(start, rocket->s.origin);
	VectorCopy(dir, rocket->movedir);
	vectoangles(dir, rocket->s.angles);
	VectorScale(dir, speed, rocket->velocity);
	rocket->movetype = MOVETYPE_FLYMISSILE;
	rocket->clipmask = MASK_SHOT;
	rocket->solid = SOLID_BBOX;
	rocket->s.effects |= 0; // EF_GRENADE;
	VectorClear(rocket->mins);
	VectorClear(rocket->maxs);
	rocket->s.modelindex = gi.modelindex("models/objects/rocket/tris.md2");
	rocket->owner = self;
	rocket->touch = mr_touch;
	rocket->nextthink = level.time + 8000.0f / speed;
	rocket->think = G_FreeEdict;
	rocket->dmg = damage;
	rocket->radius_dmg = 20;
	rocket->dmg_radius = 20;
	rocket->s.sound = gi.soundindex("weapons/rockfly.wav");
	rocket->classname = "rocket";
	rocket->classnum = CN_ROCKET;

	if (self->client)
		check_dodge(self, rocket->s.origin, dir, speed);

	gi.linkentity(rocket);
}
// machine rockets end

//	=================
//  fire_super
//  
//	Fires a super blaster bolt.
//	=================
void super_touch(edict_t* self, edict_t* other, cplane_t* plane, csurface_t* surf)
{
	int		mod;

	if (other == self->owner)
		return;

	if (surf && (surf->flags & SURF_SKY))
	{
		G_FreeEdict(self);
		return;
	}

	if (self->owner->client)
		PlayerNoise(self->owner, self->s.origin, PNOISE_IMPACT);

	if (other->takedamage)
	{
		if (self->spawnflags & 1)
			mod = MOD_HYPERBLASTER;
		else
			mod = MOD_SB;
		if (i_fragban & WFB_SUPERBLASTER)
			mod |= MOD_NOFRAG;
		T_Damage(other, self, self->owner, self->velocity, self->s.origin,
			plane->normal, self->dmg, 1, DAMAGE_ENERGY, mod);
	}
	else
	{
		gi.WriteByte(svc_temp_entity);
		gi.WriteByte(TE_BLASTER);
		gi.WritePosition(self->s.origin);
		if (!plane)
			gi.WriteDir(vec3_origin);
		else
			gi.WriteDir(plane->normal);
		gi.multicast(self->s.origin, MULTICAST_PVS);
	}

	G_FreeEdict(self);
}

void fire_super(edict_t* self, vec3_t start, vec3_t dir, int damage, int speed, int effect)
{
	edict_t* bolt;
	trace_t tr;

	VectorNormalize(dir);

	// Only change hand blaster effect
	if (effect & EF_BLASTER)
	{
		bolt = G_Spawn();
		VectorCopy(start, bolt->s.origin);
		vectoangles(dir, bolt->s.angles);
		VectorScale(dir, speed, bolt->velocity);
		VectorAdd(start, bolt->velocity, bolt->s.old_origin);
		bolt->clipmask = MASK_SHOT;

		bolt->movetype = MOVETYPE_FLYMISSILE;
		bolt->solid = SOLID_BBOX;
		bolt->s.renderfx |= RF_SHELL_RED;
		bolt->s.effects |= EF_COLOR_SHELL;
		bolt->s.effects |= EF_FLAG1;
		bolt->model = "models/objects/laser/tris.md2";
		bolt->s.modelindex = gi.modelindex("models/objects/laser/tris.md2");
		bolt->owner = self;

		bolt->s.frame = 0;

		VectorSet(bolt->mins, -8, -8, -8);
		VectorSet(bolt->maxs, 8, 8, 8);

		bolt->touch = super_touch;
		bolt->nextthink = level.time + 4;
		bolt->think = G_FreeEdict;
		bolt->dmg = damage;

		gi.linkentity(bolt);

		if (self->client)
			check_dodge(self, bolt->s.origin, dir, speed);

		tr = gi.trace(self->s.origin, NULL, NULL, bolt->s.origin, bolt,
			MASK_SHOT);
		if (tr.fraction < 1.0)
		{
			VectorMA(bolt->s.origin, -10, dir, bolt->s.origin);
			bolt->touch(bolt, tr.ent, NULL, NULL);
		}
		return;
	}

	bolt = G_Spawn();
	VectorCopy(start, bolt->s.origin);
	VectorCopy(start, bolt->s.old_origin);
	vectoangles(dir, bolt->s.angles);
	VectorScale(dir, speed, bolt->velocity);
	bolt->movetype = MOVETYPE_FLYMISSILE;
	bolt->clipmask = MASK_SHOT;
	bolt->solid = SOLID_BBOX;
	bolt->s.effects |= effect;
	VectorClear(bolt->mins);
	VectorClear(bolt->maxs);
	bolt->model = "models/objects/laser/tris.md2";
	bolt->s.modelindex = gi.modelindex("models/objects/laser/tris.md2");
	bolt->s.sound = gi.soundindex("misc/lasfly.wav");
	bolt->owner = self;
	bolt->touch = blaster_touch;
	bolt->nextthink = level.time + 2;
	bolt->think = G_FreeEdict;
	bolt->dmg = damage;
	bolt->classname = "bolt";
	bolt->classnum = CN_BOLT;
	gi.linkentity(bolt);

	if (self->client)
		check_dodge(self, bolt->s.origin, dir, speed);

	tr = gi.trace(self->s.origin, NULL, NULL, bolt->s.origin, bolt, MASK_SHOT);
	if (tr.fraction < 1.0)
	{
		if (bolt == NULL)
		{
			G_FreeEdict(bolt);
			return;
		}
		VectorMA(bolt->s.origin, -10, dir, bolt->s.origin);
		bolt->touch(bolt, tr.ent, NULL, NULL);
	}
}

//  =================
//  fire_bfg
//  =================
void bfg_explode(edict_t* self)
{
	edict_t* ent;
	float	points;
	vec3_t	v = { 0 };
	float	dist;
	int mod;

	// Set up the means of death.
	mod = MOD_BFG_EFFECT;
	if (i_fragban & WFB_BFG10K)
		mod |= MOD_NOFRAG;

	if (self->s.frame == 0)
	{
		// the BFG effect
		ent = NULL;
		while ((ent = findradius(ent, self->s.origin, self->dmg_radius)) != NULL)
		{
			if (!ent->takedamage)
				continue;
			if (ent == self->owner)
				continue;
			if (!CanDamage(ent, self))
				continue;
			if (!CanDamage(ent, self->owner))
				continue;

			VectorAdd(ent->mins, ent->maxs, v);
			VectorMA(ent->s.origin, 0.5, v, v);
			VectorSubtract(self->s.origin, v, v);
			dist = VectorLength(v);
			points = self->radius_dmg * (1.0f - (float)sqrt(dist / self->dmg_radius));
			if (ent == self->owner)
				points = points * 0.5f;

			gi.WriteByte(svc_temp_entity);
			gi.WriteByte(TE_BFG_EXPLOSION);
			gi.WritePosition(ent->s.origin);
			gi.multicast(ent->s.origin, MULTICAST_PHS);
			T_Damage(ent, self, self->owner, self->velocity, ent->s.origin,
				vec3_origin, (int)points, 0, DAMAGE_ENERGY, mod);
		}
	}

	self->nextthink = level.time + FRAMETIME;
	self->s.frame++;
	if (self->s.frame == 5)
		self->think = G_FreeEdict;
}

void bfg_touch(edict_t* self, edict_t* other, cplane_t* plane, csurface_t* surf)
{
	int mod;

	if (other == self->owner)
		return;

	if (surf && (surf->flags & SURF_SKY))
	{
		G_FreeEdict(self);
		return;
	}

	if (self->owner->client)
		PlayerNoise(self->owner, self->s.origin, PNOISE_IMPACT);

	// Set up the means of death.
	mod = MOD_BFG_BLAST;
	if (i_fragban & WFB_BFG10K)
		mod |= MOD_NOFRAG;

	// core explosion - prevents firing it into the wall/floor
	if (other->takedamage)
		T_Damage(other, self, self->owner, self->velocity, self->s.origin,
			plane->normal, 200, 0, 0, mod);
	T_RadiusDamage(self, self->owner, 200, other, 100, mod);

	gi.sound(self, CHAN_VOICE, gi.soundindex("weapons/bfg__x1b.wav"), 1, ATTN_NORM, 0);
	self->solid = SOLID_NOT;
	self->touch = NULL;
	VectorMA(self->s.origin, -1 * FRAMETIME, self->velocity, self->s.origin);
	VectorClear(self->velocity);
	self->s.modelindex = gi.modelindex("sprites/s_bfg2.sp2");
	self->s.frame = 0;
	self->s.sound = 0;
	self->s.effects &= ~EF_ANIM_ALLFAST;
	self->think = bfg_explode;
	self->nextthink = level.time + FRAMETIME;
	self->enemy = other;

	gi.WriteByte(svc_temp_entity);
	gi.WriteByte(TE_BFG_BIGEXPLOSION);
	gi.WritePosition(self->s.origin);
	gi.multicast(self->s.origin, MULTICAST_PVS);
}


void bfg_think(edict_t* self)
{
	edict_t* ent;
	edict_t* ignore;
	vec3_t	point;
	vec3_t	dir = { 0 };
	vec3_t	start = { 0 };
	vec3_t	end;
	int		dmg;
	trace_t	tr;
	int mod;

	// Set up the means of death.
	mod = MOD_BFG_LASER;
	if (i_fragban & WFB_BFG10K)
		mod |= MOD_NOFRAG;

	if (deathmatch->value)
		dmg = 5;
	else
		dmg = 10;

	ent = NULL;
	while ((ent = findradius(ent, self->s.origin, 256)) != NULL)
	{
		if (ent == self)
			continue;

		if (ent == self->owner)
			continue;

		if (!ent->takedamage)
			continue;

		if (!(ent->svflags & SVF_MONSTER) && (!ent->client) && (strcmp(ent->classname, "misc_explobox") != 0))
			continue;

		//ZOID
		//don't target teammates in CTF
		if (ctf->value && ent->client &&
			self->owner->client &&
			ent->client->resp.ctf_team == self->owner->client->resp.ctf_team)
			continue;
		//ZOID

		VectorMA(ent->absmin, 0.5, ent->size, point);

		VectorSubtract(point, self->s.origin, dir);
		VectorNormalize(dir);

		ignore = self;
		VectorCopy(self->s.origin, start);
		VectorMA(start, 2048, dir, end);
		for (;;)
		{
			tr = gi.trace(start, NULL, NULL, end, ignore, CONTENTS_SOLID | CONTENTS_MONSTER | CONTENTS_DEADMONSTER);

			if (!tr.ent)
				break;

			// hurt it if we can
			if ((tr.ent->takedamage) && !(tr.ent->flags & FL_IMMUNE_LASER) && (tr.ent != self->owner))
				T_Damage(tr.ent, self, self->owner, dir, tr.endpos, vec3_origin,
					dmg, 1, DAMAGE_ENERGY, mod);

			// if we hit something that's not a monster or player we're done
			if (!(tr.ent->svflags & SVF_MONSTER) && (!tr.ent->client))
			{
				gi.WriteByte(svc_temp_entity);
				gi.WriteByte(TE_LASER_SPARKS);
				gi.WriteByte(4);  //count
				gi.WritePosition(tr.endpos);
				gi.WriteDir(tr.plane.normal);
				gi.WriteByte(self->s.skinnum & 255);
				gi.multicast(tr.endpos, MULTICAST_PVS);
				break;
			}

			ignore = tr.ent;
			VectorCopy(tr.endpos, start);
		}

		gi.WriteByte(svc_temp_entity);
		gi.WriteByte(TE_BFG_LASER);
		gi.WritePosition(self->s.origin);
		gi.WritePosition(tr.endpos);
		gi.multicast(self->s.origin, MULTICAST_PHS);
	}

	self->nextthink = level.time + FRAMETIME;
}


void fire_bfg(edict_t* self, vec3_t start, vec3_t dir, int damage, int speed, float damage_radius)
{
	edict_t* bfg;

	bfg = G_Spawn();
	VectorCopy(start, bfg->s.origin);
	VectorCopy(dir, bfg->movedir);
	vectoangles(dir, bfg->s.angles);
	VectorScale(dir, speed, bfg->velocity);
	bfg->movetype = MOVETYPE_FLYMISSILE;
	bfg->clipmask = MASK_SHOT;
	bfg->solid = SOLID_BBOX;
	bfg->s.effects |= EF_BFG | EF_ANIM_ALLFAST;
	VectorClear(bfg->mins);
	VectorClear(bfg->maxs);
	bfg->s.modelindex = gi.modelindex("sprites/s_bfg1.sp2");
	bfg->owner = self;
	bfg->touch = bfg_touch;
	bfg->nextthink = level.time + 8000.0f / speed;
	bfg->think = G_FreeEdict;
	bfg->radius_dmg = damage;
	bfg->dmg_radius = damage_radius;
	bfg->classname = "bfg blast";
	bfg->classnum = CN_BFGBLAST;
	bfg->s.sound = gi.soundindex("weapons/bfg__l1a.wav");

	bfg->think = bfg_think;
	bfg->nextthink = level.time + FRAMETIME;
	bfg->teammaster = bfg;
	bfg->teamchain = NULL;

	if (self->client)
		check_dodge(self, bfg->s.origin, dir, speed);

	gi.linkentity(bfg);
}

//  =================
//  fire_rail
//  =================
void fire_rail(edict_t* self, vec3_t start, vec3_t aimdir, int damage, int kick)
{
	vec3_t		from = { 0 };
	vec3_t		end;
	trace_t		tr;
	edict_t* ignore;
	int			mask;
	qboolean	water;
	int n;

	if (!self)
		return;

	VectorMA(start, 8192, aimdir, end);
	VectorCopy(start, from);
	ignore = self;
	water = false;
	mask = MASK_SHOT | CONTENTS_SLIME | CONTENTS_LAVA;
	while (ignore)
	{
		tr = gi.trace(from, NULL, NULL, end, ignore, mask);

		if (tr.contents & (CONTENTS_SLIME | CONTENTS_LAVA))
		{
			mask &= ~(CONTENTS_SLIME | CONTENTS_LAVA);
			water = true;
		}
		else
		{
			if ((tr.ent->svflags & SVF_MONSTER) || (tr.ent->client))
				ignore = tr.ent;
			else
				ignore = NULL;

			if ((tr.ent != self) && (tr.ent->takedamage))
				T_Damage(tr.ent, self, self, aimdir, tr.endpos, tr.plane.normal,
					damage, kick, 0, MOD_RAILGUN2);
		}

		VectorCopy(tr.endpos, from);
	}

	// send gun puff / flash
	gi.WriteByte(svc_temp_entity);
	gi.WriteByte(TE_RAILTRAIL);
	gi.WritePosition(start);
	gi.WritePosition(tr.endpos);
	gi.multicast(self->s.origin, MULTICAST_PHS);
	if (water)
	{
		gi.WriteByte(svc_temp_entity);
		gi.WriteByte(TE_RAILTRAIL);
		gi.WritePosition(start);
		gi.WritePosition(tr.endpos);
		gi.multicast(tr.endpos, MULTICAST_PHS);
	}

	//QW// BUGNOTE: Wall piercing rail recursive call blows up server with
	// stack overflow on game maps that leak. We take advantage of the damage
	// degradation here to break out of the loop as soon as we go too far down.
	// This stops the server crash but side-effect is a few stray bubble trails
	// when hitting the limit. Non-leaky maps don't cause this problem.

	if (self->client)
	{
		PlayerNoise(self, tr.endpos, PNOISE_IMPACT);
		if (self->client->pers.weapon != NULL)
		{
			if (Q_stricmp(self->client->pers.weapon->classname, "weapon_wallpiercingrailgun") == 0)
			{
				// Take this point, and add 1, until we find an open place...
				VectorMA(tr.endpos, 10, aimdir, from);
				for (n = 0; n < 10 && damage > 20; n++)
				{
					if ((!(gi.pointcontents(from) & CONTENTS_SOLID)))
					{
						// Found a hole, fire again.
						fire_rail(self, from, aimdir, (int)(damage * 0.75), kick);
						break;
					}
					VectorMA(from, 10, aimdir, from);
				}
			}
		}
	}
}

