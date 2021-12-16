
/**************************************************/
/*           LOX Homing Rockets                   */
/**************************************************/

#include "g_local.h"
#include "x_fire.h"
#include "g_weapon.h"
#include "l_angels.h"
#include "l_nuke.h"
#include "l_positron.h"
#include "l_rockets.h"

//Skipper homing rocket
void Weapon_Skipper_Fire(edict_t* ent)
{
	vec3_t	offset = { 0 }, start, forward, right;
	int		damage;
	float	damage_radius;
	int		radius_damage;

	damage = 200 + (int)(random() * 20.0);
	radius_damage = 120;
	damage_radius = 120;
	if (is_quad)
	{
		damage *= 4;
		radius_damage *= 4;
	}

	AngleVectors(ent->client->v_angle, forward, right, NULL);
	VectorScale(forward, -2, ent->client->kick_origin);
	ent->client->kick_angles[0] = -1;
	VectorSet(offset, 8, 8, ent->viewheight - 8.0f);
	P_ProjectSource(ent->client, ent->s.origin, offset, forward, right, start);
	fire_myhoming(ent, start, forward, damage, 350, damage_radius, radius_damage);
	if (ent->client && ent->client->pers.special == AODEATH)
		fire_myhoming(ent, ent->client->angel->s.origin, forward, damage, 350, damage_radius, radius_damage);

	// send muzzle flash
	//gi.WriteByte (svc_muzzleflash);
	//gi.WriteShort (ent-g_edicts);
	//gi.WriteByte (MZ_ROCKET | is_silenced);
	//gi.multicast (ent->s.origin, MULTICAST_PVS);

	ent->client->ps.gunframe++;
	PlayerNoise(ent, start, PNOISE_WEAPON);

	if (!((int)dmflags->value & DF_INFINITE_AMMO))
		ent->client->pers.inventory[ent->client->ammo_index]
		-= ent->client->pers.weapon->quantity;
}

void Weapon_Skipper(edict_t* ent)
{
	static int	pause_frames[] = { 25, 33, 42, 50, 0 };
	static int	fire_frames[] = { 5, 0 };

	Weapon_Generic(ent, 4, 12, 50, 54, pause_frames, fire_frames, Weapon_Skipper_Fire);
}


// Fire skipper homing rocket
void fire_myhoming(edict_t* self, vec3_t start, vec3_t dir, int damage, int speed,
	float damage_radius, int radius_damage)
{
	edict_t* rocket;
	edict_t* blip = NULL;
	vec3_t blipdir = { 0 };
	float blipDot, targetDot;
	int counter = 0;

	rocket = G_Spawn();
	VectorCopy(start, rocket->s.origin);
	VectorCopy(dir, rocket->movedir);
	vectoangles(dir, rocket->s.angles);
	VectorScale(dir, (float)speed, rocket->velocity);
	rocket->movetype = MOVETYPE_FLYMISSILE;
	rocket->clipmask = MASK_SHOT;
	rocket->solid = SOLID_BBOX;
	rocket->s.effects |= EF_ROCKET;
	VectorClear(rocket->mins);
	VectorClear(rocket->maxs);
	rocket->model = "models/objects/gibs/skull/tris.md2";
	rocket->s.modelindex = gi.modelindex(rocket->model);
	rocket->owner = self;
	rocket->touch = myhoming_touch;
	rocket->enemy = NULL;

	// Make homing rocket shootable.
	rocket->movetype = MOVETYPE_FLYMISSILE;
	rocket->takedamage = DAMAGE_YES;
	VectorSet(rocket->mins, -8, -8, -8);
	VectorSet(rocket->maxs, 8, 8, 8);
	rocket->mass = 10;
	rocket->health = 20;
	rocket->max_health = 20;
	rocket->die = myhoming_die;

	rocket->dmg = 200;
	rocket->radius_dmg = 120;
	rocket->dmg_radius = 120;

	rocket->s.sound = gi.soundindex("weapons/rockfly.wav");
	rocket->classname = "homing rocket";
	rocket->classnum = CN_HOMINGROCKET;

	// Find a target to home in on.
	blip = NULL;
	targetDot = 0;
	while ((blip = findradius(blip, rocket->s.origin, 1024)) != NULL)
	{
		// See if this is the kind of blip we can home in on.
		if ((self->client->lasttarget == blip) && (counter == 0))
		{
			counter++;
			continue;
		}

		if ((self->cloaked) || (self->oldmodel))
			continue;
		if ((blip->cloaked) || (blip->oldmodel))
			continue;

		// This prevents friendly fire damage, but owner can still
		// get homed by his own rocket if he's not careful
		if (OnSameTeam(self, blip) && (blip != rocket->owner))
			continue;
		if ((homerseekowner->value == 0) && (blip == rocket->owner))
			continue;

		if (!(blip->svflags & SVF_MONSTER) && !blip->client)
			continue;
		if (!blip->takedamage)
			continue;
		if (blip->health <= 0)
			continue;
		if (!visible(rocket, blip))
			continue;
		if (!infront(rocket, blip))
			continue;	// initially look only for targets in front
		if (self->client->invisible)
			continue;
		if (blip->client == NULL)
			continue; //QW// decoys have null client id's so ignore them
		if (blip->client->invisible)
			continue;

		// Determine where the blip is in relation to us.
		VectorSubtract(blip->s.origin, rocket->s.origin, blipdir);
		blipdir[2] += 16;

		// Determine how "good" of a target it is.  (Currently, that's the
		// enemy that's most being aimed at, regardless of distance.)
		VectorNormalize(blipdir);
		blipDot = DotProduct(dir, blipdir);

		// Remember the "best" target so far.
		if (rocket->enemy == NULL || targetDot < blipDot)
		{
			rocket->enemy = blip;
			targetDot = blipDot;
			self->client->lasttarget = rocket->enemy;
		}
	}
	if (self->cloaked)
	{
		rocket->nextthink = 0;
		rocket->think = NULL;
	}
	//Did we find a target?
	else if (rocket->enemy)
	{
		// Sound a warning for the targeted one.
		gi.sound(rocket->enemy, CHAN_AUTO, gi.soundindex("misc/keytry.wav"), 1, ATTN_NORM, 0);

		// Keep tracking them.
		rocket->nextthink = level.time + FRAMETIME;
		rocket->think = myhoming_think;
	}
	else
	{
		// Let the inflictor know the rocket isn't going to home in on anyone.
		gi.sound(self, CHAN_AUTO, gi.soundindex("misc/talk1.wav"), 1, ATTN_NORM, 0);

		// No need to think.
		//rocket->nextthink = 0;
		//rocket->think = NULL;
		rocket->nextthink = level.time + FRAMETIME;
		rocket->think = myhoming_rethink;
	}

	if (self->client)
		check_dodge(self, rocket->s.origin, dir, speed);

	gi.linkentity(rocket);
}

// Skipper homing rocket
void myhoming_rethink(edict_t* rocket)
{
	edict_t* blip = NULL;
	vec3_t blipdir = { 0 };
	float blipDot, targetDot;

	// Find a target to home in on.
	blip = NULL;
	targetDot = 0;
	while ((blip = findradius(blip, rocket->s.origin, 1024)) != NULL)
	{
		// if the owner is cloaked, invis, or dead, no homing
		if (rocket->owner->cloaked)
			continue;
		if (rocket->owner->oldmodel)
			continue;
		if (rocket->owner->client->invisible)
			continue;

		// This prevents friendly fire damage, but owner can still
		// get homed by his own rocket if he's not careful
		if (OnSameTeam(rocket, blip) && (blip != rocket->owner))
			continue;
		if ((homerseekowner->value == 0) && (blip == rocket->owner))
			continue;

		if (!(blip->svflags & SVF_MONSTER) && !blip->client)
			continue;
		if (!blip->takedamage)
			continue;
		if (blip->health <= 0)
			continue;
		if (!visible(rocket, blip))
			continue;
		//if (!infront (rocket, blip))
		//	continue;	// take a wider view, maybe owner is best target
		if (blip->cloaked)
			continue;
		if (blip->oldmodel)
			continue;
		if (blip->client == NULL)
			continue; //QW// decoys have null client id's so ignore them
		if (blip->client->invisible)
			continue;


		// Determine where the blip is in relation to us.
		VectorSubtract(blip->s.origin, rocket->s.origin, blipdir);
		blipdir[2] += 16;

		// Determine how "good" of a target it is.  (Currently, that's the
		// enemy that's most being aimed at, regardless of distance.)
		VectorNormalize(blipdir);
		blipDot = DotProduct(rocket->movedir, blipdir);

		// Remember the "best" target so far.
		if (rocket->enemy == NULL || targetDot < blipDot)
		{
			rocket->enemy = blip;
			targetDot = blipDot;
		}
	}
	if (rocket->owner->cloaked)
	{
		rocket->nextthink = 0;
		rocket->think = NULL;
	}

	//Did we find a target?
	else if (rocket->enemy)
	{
		// Sound a warning for the targeted one.
		//gi.sound (rocket->enemy, CHAN_AUTO, gi.soundindex ("misc/keytry.wav"), 1, ATTN_NORM, 0);

		// Keep tracking them.
		rocket->nextthink = level.time + FRAMETIME;
		rocket->think = myhoming_think;

	}
	else
	{
		// No need to think.
		//rocket->nextthink = 0;
		//rocket->think = NULL;
		rocket->nextthink = level.time + FRAMETIME;
		rocket->think = myhoming_rethink;
	}
	if (rocket->owner->client)
		check_dodge(rocket->owner, rocket->s.origin, rocket->movedir, (int)VectorLength(rocket->velocity));

	gi.linkentity(rocket);
}

// Skipper homing rocket
void myhoming_think(edict_t* ent)
{
	vec_t speed;
	vec3_t targetdir = { 0 };
	static int seen;
	edict_t* blip = NULL;
	vec3_t blipdir = { 0 };
	float blipDot, targetDot;

	// Nudge our direction toward our target.
	VectorSubtract(ent->enemy->s.origin, ent->s.origin, targetdir);
	targetdir[2] += 16;
	VectorNormalize(targetdir);
	VectorScale(targetdir, 0.2f, targetdir);
	VectorAdd(targetdir, ent->movedir, targetdir);
	VectorNormalize(targetdir);
	VectorCopy(targetdir, ent->movedir);
	vectoangles(targetdir, ent->s.angles);
	speed = VectorLength(ent->velocity);
	VectorScale(targetdir, speed, ent->velocity);
	if (seen > 8) seen = 8;
	if (seen < 0) seen = 8;
	seen -= 1;
	speed -= 5;

	VectorScale(targetdir, speed, ent->velocity);
	if (seen < 1)
	{
		ent->model = "models/objects/gibs/skull/tris.md2";
		ent->s.modelindex = gi.modelindex(ent->model);
		ent->s.sound = gi.soundindex("weapons/rockfly.wav");
		ent->think = myhoming_rethink;
	}
	else
	{
		ent->model = NULL;
		ent->s.modelindex = 0;
		ent->s.sound = 0;
	}

	if (speed < 10)
	{
		speed = 500;
		VectorScale(targetdir, speed, ent->velocity);
		ent->model = NULL;
		ent->s.modelindex = 0;
		ent->s.sound = 0;
		ent->nextthink = level.time + FRAMETIME;
		ent->think = myhoming_rethink;
	}

	blip = NULL;
	targetDot = 0;

	while ((blip = findradius(blip, ent->s.origin, 1024)) != NULL)
	{
		// if the owner is cloaked, invis, or dead, no homing
		if (ent->owner->cloaked)
			continue;
		if (ent->owner->oldmodel)
			continue;
		if (ent->owner->client->invisible)
			continue;

		// This prevents friendly fire damage, but owner can still
		// get homed by his own rocket if he's not careful
		if (OnSameTeam(ent->owner, blip) && (blip != ent->owner))
			continue;
		if ((homerseekowner->value == 0) && (blip == ent->owner))
			continue;

		// See if this is the kind of blip we can home in on.
		if (!(blip->svflags & SVF_MONSTER) && (!blip->client))
			continue;
		if (!blip->takedamage)
			continue;
		if (blip->health <= 0)
			continue;
		if (blip->cloaked)
			continue;
		if (!visible(ent, blip))
			continue;
		if (ent->owner->client->invisible)
			continue;
		if (blip->client == NULL)
			continue; //QW// decoys have null client id's so ignore them
		if (blip->client->invisible)
			continue;
		if (!infront(ent, blip))
			continue;		// pick targets in front first

		// Determine where the blip is in relation to us.
		VectorSubtract(blip->s.origin, ent->s.origin, blipdir);
		blipdir[2] += 16;

		// Determine how "good" a target it is.  (Currently, that's the
		// enemy that's most being aimed at, regardless of distance.)
		VectorNormalize(blipdir);
		blipDot = DotProduct(targetdir, blipdir);

		// Remember the "best" target so far.
		if (ent->enemy == NULL || targetDot < blipDot)
		{
			ent->enemy = blip;
			targetDot = blipDot;
		}
	}
	//	if (speed < 100.0) speed = speed * 3;
	ent->nextthink = level.time + speed / 9000.0f;
	myhoming_think2(ent);
	myhoming_think2(ent);
	myhoming_think2(ent);
	myhoming_think2(ent);

	gi.linkentity(ent);
}

// Skipper homing rocket
void myhoming_think2(edict_t* ent)
{
	vec_t speed;
	vec3_t targetdir = { 0,0,0 };
	edict_t* blip = NULL;
	vec3_t blipdir = { 0 };
	float blipDot, targetDot;

	blip = NULL;
	targetDot = 0;
	while ((blip = findradius(blip, ent->s.origin, 1024)) != NULL)
	{
		// if the owner is cloaked, invis, or dead, no homing
		if (ent->owner->cloaked)
			continue;
		if (ent->owner->oldmodel)
			continue;
		if (ent->owner->client->invisible)
			continue;
		if (blip->cloaked)
			continue;

		// This prevents friendly fire damage, but owner can still
		// get homed by his own rocket if he's not careful
		if (OnSameTeam(ent->owner, blip) && (blip != ent->owner))
			continue;
		if ((homerseekowner->value == 0) && (blip == ent->owner))
			continue;

		// See if this is the kind of blip we can home in on.
		if (!(blip->svflags & SVF_MONSTER) && !blip->client)
			continue;
		if (!blip->takedamage)
			continue;
		if (blip->health <= 0)
			continue;
		if (!visible(ent, blip))
			continue;
		if (!infront(ent, blip))
			continue;
		if (ent->owner->client->invisible)
			continue;
		if (blip->client == NULL)
			continue; //QW// decoys have null client id's so ignore them
		if (blip->client->invisible)
			continue;

		// Determine where the blip is in relation to us.
		VectorSubtract(blip->s.origin, ent->s.origin, blipdir);
		blipdir[2] += 16;

		// Determine how "good" of a target it is.  (Currently, that's the
		// enemy that's most being aimed at, regardless of distance.)
		VectorNormalize(blipdir);
		blipDot = DotProduct(targetdir, blipdir);

		// Remember the "best" target so far.
		if (ent->enemy == NULL || targetDot < blipDot)
		{
			ent->enemy = blip;
			targetDot = blipDot;
		}
	}
	// Nudge our direction toward our target.
	VectorSubtract(ent->enemy->s.origin, ent->s.origin, targetdir);
	targetdir[2] += 16;
	VectorNormalize(targetdir);
	VectorScale(targetdir, 0.2f, targetdir);
	VectorAdd(targetdir, ent->movedir, targetdir);
	VectorNormalize(targetdir);
	VectorCopy(targetdir, ent->movedir);
	vectoangles(targetdir, ent->s.angles);
	speed = VectorLength(ent->velocity);
	VectorScale(targetdir, speed, ent->velocity);
}

// Skipper homing rocket explode
void myhoming_explode(edict_t* ent)
{
	vec3_t		origin;
	int mod;

	// Set up the means of death.
	mod = MOD_H_SPLASH;
	if (i_fragban & WFB_HOMINGROCKETLAUNCHER)
		mod |= MOD_NOFRAG;

	if (ent->owner->client)
		PlayerNoise(ent->owner, ent->s.origin, PNOISE_IMPACT);

	// calculate position for the explosion entity
	VectorMA(ent->s.origin, -0.02f, ent->velocity, origin);

	ent->takedamage = DAMAGE_NO;
	T_RadiusDamage(ent, ent->owner, (float)ent->radius_dmg, ent->enemy,
		ent->dmg_radius, mod);
	gi.WriteByte(svc_temp_entity);

	if (ent->waterlevel)
		gi.WriteByte(TE_ROCKET_EXPLOSION_WATER);
	else
		gi.WriteByte(TE_ROCKET_EXPLOSION);

	gi.WritePosition(origin);
	gi.multicast(ent->s.origin, MULTICAST_PHS);

	G_FreeEdict(ent);
}

// Skipper
void myhoming_die(edict_t* self, edict_t* inflictor, edict_t* attacker, int damage, vec3_t point)
{
	// Don't take any more damage.
	self->takedamage = DAMAGE_NO;
	// Give them credit for shooting us out of the sky, by not hurting them.
	self->enemy = attacker;

	// Blow up.
	self->think = myhoming_explode;
	self->nextthink = level.time + FRAMETIME;
}

// Skipper
void myhoming_touch(edict_t* ent, edict_t* other, cplane_t* plane, csurface_t* surf)
{
	int mod;
	int	n;

	//if (other == ent->owner)
	//	return;

	if (surf && (surf->flags & SURF_SKY))
	{
		G_FreeEdict(ent);
		return;
	}

	if (other->takedamage)
	{
		// Set up the means of death.
		mod = MOD_HOMING;
		if (i_fragban & WFB_HOMINGROCKETLAUNCHER)
			mod |= MOD_NOFRAG;

		T_Damage(other, ent, ent->owner, ent->velocity, ent->s.origin,
			plane->normal, ent->dmg, 120, 0, mod);
	}
	else
	{
		// don't throw any debris in net games
		if (!deathmatch->value && !coop->value)
		{
			if ((surf) && !(surf->flags & (SURF_WARP | SURF_TRANS33 | SURF_TRANS66 | SURF_FLOWING)))
			{
				n = rand() % 5;
				while (n--)
					ThrowDebris(ent, "models/objects/debris2/tris.md2", 2, ent->s.origin);
			}
		}
	}
	// Now make the rocket explode.
	ent->enemy = other;
	myhoming_explode(ent);
}

// generic for all rocket types
void SpawnExplosion(vec3_t start, edict_t* self, float damage, edict_t* ignore, float radius, int mod)
{
	edict_t* xplosion;

	xplosion = G_Spawn();
	VectorCopy(start, xplosion->s.origin);
	T_RadiusDamage(xplosion, self, damage, world, radius, mod);
	G_FreeEdict(xplosion);
}

// homing rocket
void homing_explode(edict_t* ent)
{
	vec3_t		origin;
	int mod;

	// Set up the means of death.
	mod = MOD_H_SPLASH;
	if (i_fragban & WFB_HOMINGROCKETLAUNCHER)
		mod |= MOD_NOFRAG;

	if (ent->owner->client)
		PlayerNoise(ent->owner, ent->s.origin, PNOISE_IMPACT);

	// calculate position for the explosion entity
	VectorMA(ent->s.origin, -0.02f, ent->velocity, origin);

	ent->takedamage = DAMAGE_NO;
	T_RadiusDamage(ent, ent->owner, (float)ent->radius_dmg, ent->enemy,
		(float)ent->dmg_radius, mod);

	gi.WriteByte(svc_temp_entity);
	if (ent->waterlevel)
		gi.WriteByte(TE_ROCKET_EXPLOSION_WATER);
	else
		gi.WriteByte(TE_ROCKET_EXPLOSION);
	gi.WritePosition(origin);
	gi.multicast(ent->s.origin, MULTICAST_PHS);

	G_FreeEdict(ent);
}

// homing rocket
void homing_think(edict_t* ent)
{
	vec_t speed;
	vec3_t targetdir = { 0 };
	edict_t* blip = NULL;
	vec3_t blipdir = { 0 };
	float blipDot, targetDot;

	// Nudge our direction toward our target.
	VectorSubtract(ent->enemy->s.origin, ent->s.origin, targetdir);
	targetdir[2] += 16;
	VectorNormalize(targetdir);
	VectorScale(targetdir, 0.2f, targetdir);
	VectorAdd(targetdir, ent->movedir, targetdir);
	VectorNormalize(targetdir);
	VectorCopy(targetdir, ent->movedir);
	vectoangles(targetdir, ent->s.angles);
	speed = VectorLength(ent->velocity);
	speed -= 3;
	ent->dmg -= 3;
	VectorScale(targetdir, speed, ent->velocity);

	blip = NULL;
	targetDot = 0;
	while ((blip = findradius(blip, ent->s.origin, 1024)) != NULL)
	{
		// This prevents friendly fire damage, but owner can still
		// get homed by his own rocket if he's not careful
		if (OnSameTeam(ent->owner, blip) && (blip != ent->owner))
			continue;
		if ((homerseekowner->value == 0) && (blip == ent->owner))
			continue;

		// See if this is the kind of blip we can home in on.
		if (blip->cloaked)
			continue;
		if (blip->oldmodel)
			continue;
		if (!(blip->svflags & SVF_MONSTER) && !blip->client)
			continue;
		if (!blip->takedamage)
			continue;
		if (blip->health <= 0)
			continue;
		if (!visible(ent, blip))
			continue;
		if (!infront(ent, blip))
			continue;

		// Determine where the blip is in relation to us.
		VectorSubtract(blip->s.origin, ent->s.origin, blipdir);
		blipdir[2] += 16;

		// Determine how "good" of a target it is.  (Currently, that's the
		// enemy that's most being aimed at, regardless of distance.)
		VectorNormalize(blipdir);
		blipDot = DotProduct(targetdir, blipdir);

		// Remember the "best" target so far.
		if (ent->enemy == NULL || targetDot < blipDot)
		{
			ent->enemy = blip;
			targetDot = blipDot;
		}
	}
	if (speed < 100) ent->think = homing_explode;
	ent->nextthink = level.time + speed / 9000.0f;
	homing_think2(ent);
	homing_think2(ent);
	homing_think2(ent);
	homing_think2(ent);
	gi.linkentity(ent);
}

// Homing rocket
void homing_think2(edict_t* ent)
{
	vec_t speed;
	vec3_t targetdir = { 0,0,0 };
	edict_t* blip = NULL;
	vec3_t blipdir = { 0 };
	float blipDot, targetDot;

	blip = NULL;
	targetDot = 0;
	while ((blip = findradius(blip, ent->s.origin, 1024)) != NULL)
	{

		if (blip->cloaked)
			continue;
		if (blip->oldmodel)
			continue;
		// This prevents friendly fire damage, but owner can still
		// get homed by his own rocket if he's not careful
		if (OnSameTeam(ent->owner, blip) && (blip != ent->owner))
			continue;
		if ((homerseekowner->value == 0) && (blip == ent->owner))
			continue;

		if (!(blip->svflags & SVF_MONSTER) && !blip->client)
			continue;
		if (!blip->takedamage)
			continue;
		if (blip->health <= 0)
			continue;
		if (!visible(ent, blip))
			continue;
		if (!infront(ent, blip))
			continue;

		// Determine where the blip is in relation to us.
		VectorSubtract(blip->s.origin, ent->s.origin, blipdir);
		blipdir[2] += 16;

		// Determine how "good" of a target it is.  (Currently, that's the
		// enemy that's most being aimed at, regardless of distance.)
		VectorNormalize(blipdir);
		blipDot = DotProduct(targetdir, blipdir);

		// Remember the "best" target so far.
		if (ent->enemy == NULL || targetDot < blipDot)
		{
			ent->enemy = blip;
			targetDot = blipDot;
		}
	}
	// Nudge our direction toward our target.
	VectorSubtract(ent->enemy->s.origin, ent->s.origin, targetdir);
	targetdir[2] += 16;
	VectorNormalize(targetdir);
	VectorScale(targetdir, 0.2f, targetdir);
	VectorAdd(targetdir, ent->movedir, targetdir);
	VectorNormalize(targetdir);
	VectorCopy(targetdir, ent->movedir);
	vectoangles(targetdir, ent->s.angles);
	speed = VectorLength(ent->velocity);
	VectorScale(targetdir, speed, ent->velocity);
}

void homing_die(edict_t* self, edict_t* inflictor, edict_t* attacker, int damage,
	vec3_t point)
{
	// Don't take any more damage.
	self->takedamage = DAMAGE_NO;

	// Give them credit for shooting us out of the sky, by not hurting them.
	self->enemy = attacker;

	// Blow up.
	self->think = homing_explode;
	self->nextthink = level.time + FRAMETIME;
}

void homing_touch(edict_t* ent, edict_t* other, cplane_t* plane, csurface_t* surf)
{
	int mod;
	int	n;

	//	if (other == ent->owner)
		//	return;

	if (surf && (surf->flags & SURF_SKY))
	{
		G_FreeEdict(ent);
		return;
	}

	if (other->takedamage)
	{

		// Set up the means of death.
		mod = MOD_HOMING;
		if (i_fragban & WFB_HOMINGROCKETLAUNCHER)
			mod |= MOD_NOFRAG;

		T_Damage(other, ent, ent->owner, ent->velocity, ent->s.origin,
			plane->normal, ent->dmg, 120, 0, mod);
	}
	else
	{
		// don't throw any debris in net games
		if (!deathmatch->value && !coop->value)
		{
			if ((surf) && !(surf->flags & (SURF_WARP | SURF_TRANS33 | SURF_TRANS66 | SURF_FLOWING)))
			{
				n = rand() % 5;
				while (n--)
					ThrowDebris(ent, "models/objects/debris2/tris.md2", 2, ent->s.origin);
			}
		}
	}

	// Now make the rocket explode.
	ent->enemy = other;
	homing_explode(ent);
}

void fire_homing(edict_t* self, vec3_t start, vec3_t dir, int damage, int speed,
	float damage_radius, int radius_damage)
{
	edict_t* rocket;
	edict_t* blip = NULL;
	vec3_t blipdir = { 0 };
	float blipDot, targetDot;
	int counter = 0;

	rocket = G_Spawn();
	VectorCopy(start, rocket->s.origin);
	VectorCopy(dir, rocket->movedir);
	vectoangles(dir, rocket->s.angles);
	VectorScale(dir, (float)speed, rocket->velocity);
	rocket->movetype = MOVETYPE_FLYMISSILE;
	rocket->clipmask = MASK_SHOT;
	rocket->solid = SOLID_BBOX;
	rocket->s.effects |= EF_ROCKET;
	VectorClear(rocket->mins);
	VectorClear(rocket->maxs);
	rocket->model = "models/objects/rocket/tris.md2";
	rocket->s.modelindex = gi.modelindex(rocket->model);
	rocket->owner = self;
	rocket->touch = homing_touch;
	rocket->enemy = NULL;

	// Make homing rocket shootable.
	rocket->movetype = MOVETYPE_FLYRICOCHET;//MOVETYPE_FLYMISSILE;
	rocket->takedamage = DAMAGE_YES;
	//rocket->svflags |= SVF_MONSTER;
	//rocket->clipmask |= CONTENTS_MONSTERCLIP;
	VectorSet(rocket->mins, -8, -8, -8);
	VectorSet(rocket->maxs, 8, 8, 8);
	rocket->mass = 10;
	rocket->health = 20;
	rocket->max_health = 20;
	rocket->die = homing_die;

	rocket->dmg = damage;
	rocket->radius_dmg = 120;
	rocket->dmg_radius = 120;
	rocket->s.sound = gi.soundindex("weapons/rockfly.wav");
	rocket->classname = "homing rocket";
	rocket->classnum = CN_HOMINGROCKET;

	// Find a target to home in on.
	blip = NULL;
	targetDot = 0;
	while ((blip = findradius(blip, rocket->s.origin, 1024)) != NULL)
	{
		// See if this is the kind of blip we can home in on.
		if ((self->client->lasttarget == blip) && (counter == 0))
		{
			counter++;
			continue;
		}

		if (!self->oldmodel)
			continue;
		if (blip->cloaked)
			continue;
		if (blip->oldmodel)
			continue;

		// This prevents friendly fire damage, but owner can still
		// get homed by his own rocket if he's not careful
		if (OnSameTeam(self, blip) && (blip != self->owner))
			continue;
		if ((homerseekowner->value == 0) && (blip == self->owner))
			continue;

		if (!(blip->svflags & SVF_MONSTER) && !blip->client)
			continue;
		if (blip == rocket->owner)
			continue;
		if (!blip->takedamage)
			continue;
		if (blip->health <= 0)
			continue;
		if (!visible(rocket, blip))
			continue;
		if (!infront(rocket, blip))
			continue;
		if (self->client->invisible)
			continue;
		if (blip->client == NULL)
			continue;  //QW// decoys have null client id's so ignore them

		// Determine where the blip is in relation to us.
		VectorSubtract(blip->s.origin, rocket->s.origin, blipdir);
		blipdir[2] += 16;

		// Determine how "good" of a target it is.  (Currently, that's the
		// enemy that's most being aimed at, regardless of distance.)
		VectorNormalize(blipdir);
		blipDot = DotProduct(dir, blipdir);

		// Remember the "best" target so far.
		if (rocket->enemy == NULL || targetDot < blipDot)
		{
			rocket->enemy = blip;
			targetDot = blipDot;
			self->client->lasttarget = rocket->enemy;
		}
	}

	if (self->cloaked)
	{
		rocket->nextthink = 0;
		rocket->think = NULL;
	}
	//Did we find a target?
	else if (rocket->enemy)
	{
		// Sound a warning for the targeted one.
		if (rocket->enemy == rocket->owner)
			gi.sound(rocket->enemy, CHAN_AUTO, gi.soundindex("misc/uhoh.wav"), 1, ATTN_NORM, 0);
		else
			gi.sound(rocket->enemy, CHAN_AUTO, gi.soundindex("misc/keytry.wav"), 1, ATTN_NORM, 0);

		// Keep tracking them.
		rocket->nextthink = level.time + FRAMETIME;
		rocket->think = homing_think;
	}

	else
	{
		// Let the inflictor know the rocket isn't going to home in on anyone.
		gi.sound(self, CHAN_AUTO, gi.soundindex("misc/talk1.wav"), 1, ATTN_NORM, 0);

		// No need to think.
		//rocket->nextthink = 0;
		//rocket->think = NULL;
		rocket->nextthink = level.time + FRAMETIME;
		rocket->think = homing_rethink;
	}

	if (self->client)
		check_dodge(self, rocket->s.origin, dir, speed);

	gi.linkentity(rocket);
}

void Weapon_Homing_Fire(edict_t* ent)
{
	vec3_t	offset = { 0 }, start;
	vec3_t	forward, right;
	int		damage;
	float	damage_radius;
	int		radius_damage;

	damage = 85 + (int)(random() * 20.0);
	radius_damage = 120;
	damage_radius = 120;

	if (is_quad)
	{
		damage *= 4;
		radius_damage *= 4;
	}

	AngleVectors(ent->client->v_angle, forward, right, NULL);

	VectorScale(forward, -2, ent->client->kick_origin);
	ent->client->kick_angles[0] = -1;

	VectorSet(offset, 8, 8, ent->viewheight - 8.0f);
	P_ProjectSource(ent->client, ent->s.origin, offset, forward, right, start);
	fire_homing(ent, start, forward, damage, 350, damage_radius, radius_damage);
	if (ent->client && ent->client->pers.special == AODEATH)
	{
		fire_homing(ent, ent->client->angel->s.origin, forward, damage, 350, damage_radius, radius_damage);
	}

	// send muzzle flash
	gi.WriteByte(svc_muzzleflash);
	gi.WriteShort(ent - g_edicts);
	gi.WriteByte(MZ_ROCKET | is_silenced);
	gi.multicast(ent->s.origin, MULTICAST_PVS);

	ent->client->ps.gunframe++;

	PlayerNoise(ent, start, PNOISE_WEAPON);

	if (!((int)dmflags->value & DF_INFINITE_AMMO))
		ent->client->pers.inventory[ent->client->ammo_index]
		-= ent->client->pers.weapon->quantity;
}

void Weapon_Homing(edict_t* ent)
{
	static int	pause_frames[] = { 25, 33, 42, 50, 0 };
	static int	fire_frames[] = { 5, 0 };

	Weapon_Generic(ent, 4, 12, 50, 54, pause_frames, fire_frames, Weapon_Homing_Fire);
}

void homing_rethink(edict_t* rocket)
{
	edict_t* blip = NULL;
	vec3_t blipdir = { 0 };
	float blipDot, targetDot;

	// Find a target to home in on.
	blip = NULL;
	targetDot = 0;
	while ((blip = findradius(blip, rocket->s.origin, 1024)) != NULL)
	{

		// See if this is the kind of blip we can home in on.
		// if the owner is cloaked, invis, or dead, no homing
		if (rocket->owner->cloaked)
			continue;
		if (rocket->owner->oldmodel)
			continue;
		if (rocket->owner->client->invisible)
			continue;
		if (blip->cloaked)
			continue;

		// This prevents friendly fire damage, but owner can still
		// get homed by his own rocket if he's not careful
		if (OnSameTeam(rocket->owner, blip) && (blip != rocket->owner))
			continue;
		if ((homerseekowner->value == 0) && (blip == rocket->owner))
			continue;

		if (blip->s.modelindex == 0)
			continue;
		if (!(blip->svflags & SVF_MONSTER) && !blip->client)
			continue;
		if (!blip->takedamage)
			continue;
		if (blip->health <= 0)
			continue;
		if (!visible(rocket, blip))
			continue;
		//if (!infront (rocket, blip))
		//	continue;
		if (rocket->owner->client->invisible)
			continue;
		if ((blip->client == NULL) || (blip->client->invisible))
			continue; //QW// decoys have null client id's so ignore them

		// Determine where the blip is in relation to us.
		VectorSubtract(blip->s.origin, rocket->s.origin, blipdir);
		blipdir[2] += 16;

		// Determine how "good" of a target it is.  (Currently, that's the
		// enemy that's most being aimed at, regardless of distance.)
		VectorNormalize(blipdir);
		blipDot = DotProduct(rocket->movedir, blipdir);

		// Remember the "best" target so far.
		if (rocket->enemy == NULL || targetDot < blipDot)
		{
			rocket->enemy = blip;
			targetDot = blipDot;
		}
	}

	if (rocket->owner->cloaked)
	{
		rocket->nextthink = 0;
		rocket->think = NULL;
	}
	//Did we find a target?
	else if (rocket->enemy)
	{
		// Sound a warning for the targeted one.
		if (rocket->enemy == rocket->owner)
			gi.sound(rocket->enemy, CHAN_AUTO, gi.soundindex("misc/uhoh.wav"), 1, ATTN_NORM, 0);
		else
			gi.sound(rocket->enemy, CHAN_AUTO, gi.soundindex("misc/keytry.wav"), 1, ATTN_NORM, 0);

		// Keep tracking them.
		rocket->nextthink = level.time + FRAMETIME;
		rocket->think = homing_think;

	}
	else
	{

		// No need to think.
		//rocket->nextthink = 0;
		//rocket->think = NULL;
		rocket->nextthink = level.time + FRAMETIME;
		rocket->think = homing_rethink;
	}
	if (rocket->owner->client)
		check_dodge(rocket->owner, rocket->s.origin, rocket->movedir, (int)VectorLength(rocket->velocity));

	gi.linkentity(rocket);
}
