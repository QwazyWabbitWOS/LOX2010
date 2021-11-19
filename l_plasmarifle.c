/*
======================================================================

PLASMA RIFLE

======================================================================
*/

#include "g_local.h"
#include "m_player.h"
#include "l_angels.h"

void Plasma_Fire(edict_t* ent, vec3_t g_offset, int damage,
	qboolean hyper, int effect)
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

#if 0

	fire_plasma(ent, start, forward, damage, 1200, effect);
	if (ent->client && ent->client->pers.special == AODEATH)
	{
		fire_plasma(ent, ent->client->angel->s.origin, forward, damage, 1200, effect);
	}

#else
	{
		fire_bolt(ent, start, forward, damage);
		if (ent->client && ent->client->pers.special == AODEATH)
		{
			fire_bolt(ent, ent->client->angel->s.origin, forward, damage);
		}
	}
#endif

	// send muzzle flash
	gi.WriteByte(svc_muzzleflash);
	gi.WriteShort(ent - g_edicts);
	if (hyper)
		gi.WriteByte(MZ_HYPERBLASTER | is_silenced);
	else
		gi.WriteByte(MZ_BLASTER | is_silenced);
	gi.multicast(ent->s.origin, MULTICAST_PVS);

	PlayerNoise(ent, start, PNOISE_WEAPON);
}


void Weapon_Plasma_Fire(edict_t* ent)
{
	int		damage;

	if (deathmatch->value)
		damage = 15;
	else
		damage = 20;

	Plasma_Fire(ent, vec3_origin, damage, false, EF_BFG);
	ent->client->ps.gunframe++;
	if (!((int)dmflags->value & DF_INFINITE_AMMO))
		ent->client->pers.inventory[ent->client->ammo_index]
		-= ent->client->pers.weapon->quantity;
}

#if 0

void Weapon_Plasma(edict_t* ent)
{
	static int	pause_frames[] = { 25, 33, 42, 50, 0 };
	static int	fire_frames[] = { 6, 7, 0 };

	// As long as they're holding down the fire button, fire.
	// (That's why we fire in frame 7; to keep it from going idle.)
	if (ent->client->weaponstate == WEAPON_FIRING
		&& ent->client->ps.gunframe == 7)
	{
		if (!ent->client->newweapon
			&& (ent->client->buttons & BUTTON_ATTACK)
			&& ent->client->pers.inventory[ITEM_INDEX(gI_weapon_plasma.ammo)]
			>= gI_weapon_plasma.quantity)
		{
			// Turn back the clock.
			ent->client->ps.gunframe = 6;
		}
		else
		{
			// Move the clock forward.
			ent->client->ps.gunframe = 8;
		}
	}

	// Then act like a normal weapon.
	Weapon_Generic(ent, 5, 7, 59, 64, pause_frames, fire_frames,
		Weapon_Plasma_Fire);
}

#else

void Weapon_Plasma(edict_t* ent)
{
	static int	pause_frames[] = { 0 };
	static int	fire_frames[] = { 6, 7, 8, 9, 10, 11, 0 };

	// As long as they're holding down the fire button, fire.
	// (That's why we fire in frame 7; to keep it from going idle.)
	if (ent->client->weaponstate == WEAPON_FIRING
		&& ent->client->ps.gunframe == 7)
	{
		if (!ent->client->newweapon
			&& (ent->client->buttons & BUTTON_ATTACK)
			&& ent->client->pers.inventory[ITEM_INDEX(gI_weapon_plasma.ammo)]
			>= gI_weapon_plasma.quantity)
		{
			// Turn back the clock.
			ent->client->ps.gunframe = 6;
		}
		else
		{
			// Move the clock forward.
			ent->client->ps.gunframe = 12;
		}
	}

	// Then act like a normal weapon.
	Weapon_Generic(ent, 5, 20, 49, 53, pause_frames, fire_frames,
		Weapon_Plasma_Fire);
}

#endif

void fire_bolt(edict_t* self, vec3_t start, vec3_t aimdir, int damage)
{
	vec3_t		from = { 0 };
	vec3_t		end;
	trace_t		tr;
	int			mod;

	VectorNormalize(aimdir);
	VectorMA(start, 8192, aimdir, end);
	VectorCopy(start, from);
	tr = gi.trace(from, NULL, NULL, end, self, MASK_SHOT | MASK_WATER);

	if ((tr.ent != self) && (tr.ent->takedamage))
	{
		// Set up the means of death.
		mod = MOD_PLASMAGUN;
		if (i_fragban & WFB_PLASMARIFLE)
			mod |= MOD_NOFRAG;

		T_Damage(tr.ent, self, self, aimdir, tr.endpos, tr.plane.normal,
			damage, 0, DAMAGE_ENERGY, mod);
	}

	VectorCopy(tr.endpos, from);

	// draw bolt
	gi.WriteByte(svc_temp_entity);
	gi.WriteByte(TE_BFG_LASER);
	gi.WritePosition(start);
	gi.WritePosition(tr.endpos);
	gi.multicast(self->s.origin, MULTICAST_PVS);

	if (self->client)
		PlayerNoise(self, tr.endpos, PNOISE_IMPACT);
}

//end plasma

