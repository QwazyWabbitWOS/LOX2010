/*
======================================================================

FLOATING MINE LAUNCHER

======================================================================
*/

#include "g_local.h"

#include "m_player.h"
#include "l_angels.h"

// darKMajick definitions
#include "l_dm_grenades.h"

void weapon_floatingminelauncher_fire(edict_t* ent)
{
	vec3_t	offset = { 0 };
	vec3_t	forward, right;
	vec3_t	start;
	int		damage = 120;
	float	radius;

	ent->client->mine = 1;

	if (ent->client->pers.inventory[ent->client->ammo_index]
		< ent->client->dM_ammoCost)
	{

		// No grenades, switch to something else.
		if (level.time >= ent->pain_debounce_time)
		{
			gi.sound(ent, CHAN_VOICE, gi.soundindex("weapons/noammo.wav"), 1, ATTN_NORM, 0);
			ent->pain_debounce_time = level.time + 1;
		}
		NoAmmoWeaponChange(ent);
		ent->client->ps.gunframe = /* FRAME_IDLE_FIRST */ 17;
		return;
	}


	radius = damage + 40.0f;
	if (is_quad)
		damage *= 4;

	VectorSet(offset, 8, 8, ent->viewheight - 8.0f);
	AngleVectors(ent->client->v_angle, forward, right, NULL);
	P_ProjectSource(ent->client, ent->s.origin, offset, forward, right, start);

	VectorScale(forward, -2, ent->client->kick_origin);
	ent->client->kick_angles[0] = -1;

	// Here we had a restriction on floating mine types.
	//	ent->client->dM_grenade = DM_NORMALGRENADE;

	/* Now we allow all darKMajick types except teleport grenades in floaters.
	The floating teleports didn't kill very well and the owner ended up getting
	jerked back to the site whenever someone detonated one.
	*/
	if (ent->client->dM_grenade == DM_TELEGRENADE)
		ent->client->dM_grenade = DM_NORMALGRENADE;  //QW//

	fire_grenade_dM(ent, start, forward, damage, 500, 2.5, radius,
		ent->client->dM_grenade,
		/* held */ false,
		/* bazookad */ true);

	if (!is_silenced)
	{
		gi.WriteByte(svc_muzzleflash);
		gi.WriteShort(ent - g_edicts);
		gi.WriteByte(MZ_ROCKET | is_silenced);
		gi.multicast(ent->s.origin, MULTICAST_PVS);
	}
	else
	{
		gi.WriteByte(svc_muzzleflash2);
		gi.WriteShort(ent - g_edicts);
		gi.WriteByte(MZ2_CHICK_ROCKET_1);
		gi.multicast(start, MULTICAST_PVS);
	}

	ent->client->ps.gunframe++;

	PlayerNoise(ent, start, PNOISE_WEAPON);

	if (!((int)dmflags->value & DF_INFINITE_AMMO))
		ent->client->pers.inventory[ent->client->ammo_index]
		-= ent->client->dM_ammoCost;
}

void Weapon_FloatingMineLauncher(edict_t* ent)
{
	static int	pause_frames[] = { 34, 51, 59, 0 };
	static int	fire_frames[] = { 6, 0 };

	Weapon_Generic(ent,
		5,	//frame activate last
		16,	//frame fire last
		59, //frame idle last
		64, //frame deactivate last
		pause_frames,
		fire_frames,
		weapon_floatingminelauncher_fire);
}

