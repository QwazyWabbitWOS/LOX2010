/*
======================================================================

SUPERBLASTER SWEEPER

======================================================================
*/

#include "g_local.h"
#include "m_player.h"
#include "l_angels.h"

void weapon_superblastersweeper_fire(edict_t* ent)
{
	vec3_t		start;
	vec3_t		forward, right;
	vec3_t		offset = { 0 };
	int			damage;
	int			kick = 8;
	int			mod;

	if (deathmatch->value)
		damage = 50;
	else
		damage = 150;

	AngleVectors(ent->client->v_angle, forward, right, NULL);
	VectorScale(forward, -2, ent->client->kick_origin);
	ent->client->kick_angles[0] = -2;
	VectorSet(offset, 0, 8, ent->viewheight - 8.0f);
	P_ProjectSource(ent->client, ent->s.origin, offset, forward, right, start);

	if (is_quad)
	{
		damage *= 4;
		kick *= 4;
	}

	if (!(ent->client->pers.inventory[ent->client->ammo_index]))
	{
		// Stop firing.
		ent->client->ps.gunframe = 22;
		ent->client->weapon_sound = 0;

		// Make the streetsweeper-noammo sound.
		gi.sound(ent, CHAN_AUTO, gi.soundindex("weapons/noammo.wav"), 1, ATTN_IDLE, 0);

		// Switch to some other weapon.
		NoAmmoWeaponChange(ent);

		return;
	}
	else if (ent->client->weaponstate == WEAPON_FIRING
		&& ent->client->ps.gunframe == 21
		&& (ent->client->buttons & BUTTON_ATTACK)
		&& ent->client->pers.inventory[ent->client->ammo_index])
	{
		// Keep firing.
		ent->client->ps.gunframe = 15;
	}
	else if (ent->client->weaponstate == WEAPON_FIRING
		&& ent->client->ps.gunframe >= 5
		&& ent->client->ps.gunframe <= 21
		&& !(ent->client->buttons & BUTTON_ATTACK))
	{
		// Stop firing.
		ent->client->ps.gunframe = 22;
		ent->client->weapon_sound = 0;

		// Make the streetsweeper-winddown sound.
		gi.sound(ent, CHAN_AUTO, gi.soundindex("weapons/chngnd1a.wav"), 1, ATTN_IDLE, 0);
	}
	else
	{
		if (ent->client->ps.gunframe == 5)
		{
			// Start firing.

			// Make the streetsweeper-windup sound.
			gi.sound(ent, CHAN_AUTO, gi.soundindex("weapons/chngnu1a.wav"), 1, ATTN_IDLE, 0);
		}
		else if (ent->client->ps.gunframe >= 15
			&& ent->client->ps.gunframe <= 21)
		{
			// Now that the wind-up sound has finished, play the "during" sound
			// continuously until they stop firing.
			ent->client->weapon_sound = gi.soundindex("weapons/chngnl1a.wav");
		}

		ent->client->ps.gunframe++;
	}

	if (!ent->deadflag && ent->s.modelindex == 255)
	{
		// ### Hentai ### BEGIN

		ent->client->anim_priority = ANIM_ATTACK;
		if (ent->client->ps.pmove.pm_flags & PMF_DUCKED)
		{
			ent->s.frame = FRAME_crattak1 - (ent->client->ps.gunframe & 1);
			ent->client->anim_end = FRAME_crattak9;
		}
		else
		{
			ent->s.frame = FRAME_attack1 - (ent->client->ps.gunframe & 1);
			ent->client->anim_end = FRAME_attack8;
		}

		// ### Hentai ### END
	}

	// Set up the means of death.
	mod = MOD_SUPERBLASTERSWEEPER;
	if (i_fragban & WFB_SWEEPERS)	//frag ban on sweepers means no points awarded
		mod |= MOD_NOFRAG;

	if (deathmatch->value)
	{
		fire_super(ent, start, forward, damage, 1000, EF_BLASTER /* , hyper */);
		if (ent->client->sweeperfiremode == SFM_HIGH)
			ent->client->curr_heat += WEPHEAT_HIGH;
		else
			ent->client->curr_heat += WEPHEAT_LOW;

		if (ent->client && ent->client->pers.special == AODEATH)
			fire_super(ent, ent->client->angel->s.origin, forward, damage, 1000, EF_BLASTER /* , hyper */);
	}
	else
	{
		fire_super(ent, start, forward, damage, 1000, EF_BLASTER /* , hyper */);
		if (ent->client && ent->client->pers.special == AODEATH)
			fire_super(ent, ent->client->angel->s.origin, forward, damage, 1000, EF_BLASTER /* , hyper */);
	}
	// send muzzle flash
	gi.WriteByte(svc_muzzleflash);
	gi.WriteShort(ent - g_edicts);
	gi.WriteByte(MZ_BLASTER | is_silenced);
	gi.multicast(ent->s.origin, MULTICAST_PVS);

	PlayerNoise(ent, start, PNOISE_WEAPON);

	if (!ent->deadflag && ent->s.modelindex == 255)
	{
		// ### Hentai ### BEGIN

		ent->client->anim_priority = ANIM_ATTACK;
		if (ent->client->ps.pmove.pm_flags & PMF_DUCKED)
		{
			ent->s.frame = FRAME_crattak1 - (int)(random() + 0.25);
			ent->client->anim_end = FRAME_crattak9;
		}
		else
		{
			ent->s.frame = FRAME_attack1 - (int)(random() + 0.25);
			ent->client->anim_end = FRAME_attack8;
		}

		// ### Hentai ### END
	}

	if (!((int)dmflags->value & DF_INFINITE_AMMO))
		ent->client->pers.inventory[ent->client->ammo_index]
		-= ent->client->pers.weapon->quantity;
}

void Weapon_SuperBlastersweeper(edict_t* ent)
{
	if (ent->client->sweeperfiremode == SFM_HIGH)
	{
		static int	pause_frames[] = { 38, 43, 51, 61, 0 };
		static int	fire_frames[] = { 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 0 };
		Weapon_Generic(ent, 4, 31, 61, 64, pause_frames, fire_frames, weapon_superblastersweeper_fire);
	}
	else if (ent->client->sweeperfiremode == SFM_LOW)
	{
		static int	fire_frames[] = { 5, 7, 9, 11, 13, 15, 17, 19, 21, 0 };
		static int	pause_frames[] = { 38, 43, 51, 61, 0 };
		Weapon_Generic(ent, 4, 31, 61, 64, pause_frames, fire_frames,
			weapon_superblastersweeper_fire);
	}
}


