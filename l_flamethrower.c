/*
======================================================================

FLAMETHROWER (used in the railgun family)

======================================================================
*/

#include "g_local.h"
#include "m_player.h"
#include "l_angels.h"
#include "x_fire.h"


void fire_rg (edict_t *ent, vec3_t g_offset, int damage, qboolean hyper, int effect)
{
	vec3_t	forward, right;
	vec3_t	start;
	vec3_t	offset;
	vec3_t	direct_damage = {6, 9, 25};
	vec3_t	radius_damage = {6, 4, 25};

	if (is_quad)
		damage *= 4;
	AngleVectors (ent->client->v_angle, forward, right, NULL);
	VectorSet(offset, 24, 8, ent->viewheight - 8.0f);
	VectorAdd (offset, g_offset, offset);
	P_ProjectSource (ent->client, ent->s.origin, offset, forward, right, start);

	VectorScale (forward, -2, ent->client->kick_origin);
	ent->client->kick_angles[0] = -1;

	// ow, my flesh, its burning!
	PBM_FireFlamer (ent, start, forward, 1200, 70, direct_damage, radius_damage, 100, 50);
	if (ent->client && ent->client->pers.special == AODEATH)
	{
		PBM_FireFlamer (ent, ent->client->angel->s.origin, forward, 1200, 70, direct_damage, radius_damage, 100, 50);
	}
	// send muzzle flash
	gi.WriteByte (svc_muzzleflash);
	gi.WriteShort (ent-g_edicts);
	if (hyper)
		gi.WriteByte (MZ_HYPERBLASTER | is_silenced);
	else
		gi.WriteByte (MZ_HYPERBLASTER | is_silenced);

	gi.multicast (ent->s.origin, MULTICAST_PVS);

	PlayerNoise(ent, start, PNOISE_WEAPON);
}

void weapon_railgun_fire (edict_t *ent)
{
	int		damage;

	if (!ent || !ent->client)
		return;

	if (deathmatch->value)
		damage = 10;
	else
		damage = 20;
	fire_rg (ent, vec3_origin, damage, false, EF_ROCKET);
	if (ent->client && ent->client->pers.special == AODEATH)
	{
		fire_rg (ent, ent->client->angel->s.origin, damage, false, EF_ROCKET);
	}
	ent->client->ps.gunframe++;
	if (! ((int)dmflags->value & DF_INFINITE_AMMO))
		ent->client->pers.inventory[ent->client->ammo_index]
			-= ent->client->pers.weapon->quantity;
}

void Weapon_Railgun (edict_t *ent)
{
	static int	pause_frames[]	= {34, 51, 59, 0};
	static int	fire_frames[]	= {4, 5, 0};

	Weapon_Generic (ent, 3, 5, 56, 61, pause_frames, fire_frames, weapon_railgun_fire);
}
