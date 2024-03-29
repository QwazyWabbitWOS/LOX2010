/*
======================================================================

FREEZE GUN

======================================================================
*/

#include "g_local.h"
#include "l_angels.h"
#include "l_freezegun.h"

void Freezer_Fire(edict_t* ent, vec3_t g_offset, int damage, qboolean hyper, int effect)
{
	vec3_t	forward, right;
	vec3_t	start;
	vec3_t	offset = { 0 };

	if (is_quad)
		damage *= 4;
	AngleVectors(ent->client->v_angle, forward, right, NULL);
	VectorSet(offset, 24, 8, ent->viewheight - 8.0f);
	VectorAdd(offset, g_offset, offset);
	P_ProjectSource(ent->client, ent->s.origin, offset, forward, right, start);

	VectorScale(forward, -2, ent->client->kick_origin);
	ent->client->kick_angles[0] = -1;

	// tell it to fire the freezer instead
	fire_freezer(ent, start, forward, damage, FREEZER_SPEED, effect);
	if (ent->client && ent->client->pers.special == AODEATH)
	{
		fire_freezer(ent, ent->client->angel->s.origin, forward, damage, FREEZER_SPEED, effect);
	}
	// send muzzle flash
	gi.WriteByte(svc_muzzleflash);
	gi.WriteShort(ent - g_edicts);
	if (hyper)
		gi.WriteByte(MZ_HYPERBLASTER | is_silenced);
	else
		gi.WriteByte(MZ_RAILGUN | is_silenced);
	gi.multicast(ent->s.origin, MULTICAST_PVS);

	PlayerNoise(ent, start, PNOISE_WEAPON);
}

void Weapon_Freezer_Fire(edict_t* ent)
{
	int		damage;

	if (deathmatch->value)
		damage = FREEZER_DAMAGE;
	else
		damage = FREEZER_DAMAGE - 15;
	Freezer_Fire(ent, vec3_origin, damage, false, EF_BLASTER);
	ent->client->ps.gunframe++;
	if (!((int)dmflags->value & DF_INFINITE_AMMO))
		ent->client->pers.inventory[ent->client->ammo_index]
		-= ent->client->pers.weapon->quantity;
}

void Weapon_Freezer(edict_t* ent)
{
	static int	pause_frames[] = { 29, 42, 57, 0 };
	static int	fire_frames[] = { 7, 0 };

	Weapon_Generic(ent, 6, 17, 57, 61, pause_frames, fire_frames, Weapon_Freezer_Fire);
}

void freezer_touch(edict_t* self, edict_t* other, cplane_t* plane, csurface_t* surf)
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
		int damage;
		int mod;

		// Set up the means of death.
		mod = MOD_FREEZE;
		if (i_fragban & WFB_FREEZEGUN)
			mod |= MOD_NOFRAG;

		// Hurt them, keep track of whether we could.
		damage = other->health;
		T_Damage(other, self, self->owner, self->velocity, self->s.origin,
			plane->normal, self->dmg, 1, DAMAGE_ENERGY, mod);
		damage -= other->health;
		gi.sound(self, CHAN_VOICE, gi.soundindex("weapons/frozen.wav"), 1,
			ATTN_NORM, 0);

		// If we hurt them without killing them, freeze them.
		if (!other->deadflag && damage > 0)
		{
			// Freeze them for this long.
			if (other->frozen)
				other->frozentime += 8.0;
			else
				other->frozentime = level.time + 4.0f;

			// Freeze them.
			other->frozen = 1;
		}
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

void fire_freezer(edict_t* self, vec3_t start, vec3_t dir, int damage, int speed, int effect)
{
	edict_t* bolt;
	trace_t	tr;

	VectorNormalize(dir);

	bolt = G_Spawn();
	VectorCopy(start, bolt->s.origin);
	VectorCopy(start, bolt->s.old_origin);
	vectoangles(dir, bolt->s.angles);
	VectorScale(dir, (float)speed, bolt->velocity);
	bolt->movetype = MOVETYPE_FLYMISSILE;
	bolt->clipmask = MASK_SHOT;
	bolt->solid = SOLID_BBOX;
	bolt->s.effects |= EF_FLAG2;
	bolt->s.effects |= EF_COLOR_SHELL;
	bolt->s.renderfx |= RF_SHELL_BLUE;
	VectorClear(bolt->mins);
	VectorClear(bolt->maxs);
	bolt->s.modelindex = gi.modelindex("models/objects/laser/tris.md2");
	bolt->s.sound = gi.soundindex("misc/lasfly.wav");
	bolt->owner = self;
	bolt->touch = freezer_touch;
	bolt->nextthink = level.time + 8000.0f / speed;
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
