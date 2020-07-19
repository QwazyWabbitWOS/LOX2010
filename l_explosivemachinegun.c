/*
======================================================================

Explosive Machinegun

======================================================================
*/

#include "g_local.h"
#include "m_player.h"
#include "l_angels.h"


void ExplosiveMachinegun_Fire (edict_t *ent)
{
	int	i;
	vec3_t		start;
	vec3_t		forward, right;
	vec3_t		angles, offset;
	int			damage = 8;
	int			kick = 2;
	int			mod;

	if (!ent || !ent->client)
		return;

	if (!(ent->client->buttons & BUTTON_ATTACK) &&
	((ent->client->burstfire_count > 2) ||
	(!ent->client->burstfire_count)))
	{ 
		ent->client->machinegun_shots=0;
		ent->client->burstfire_count=0;
		ent->client->ps.gunframe++;
		return;
	}

	if (ent->client->burstfire_count < 3)
	{
		if (ent->client->ps.gunframe == 5)
			ent->client->ps.gunframe = 4;
		else
			ent->client->ps.gunframe = 5;
	}
	if (ent->client->pers.inventory[ent->client->ammo_index] < 1)
	{
		ent->client->ps.gunframe = 6;
		if (level.time >= ent->pain_debounce_time)
		{
			gi.sound(ent, CHAN_VOICE, gi.soundindex("weapons/noammo.wav"), 1, ATTN_NORM, 0);
			ent->pain_debounce_time = level.time + 1;
		}
		ent->client->burstfire_count=0;

		NoAmmoWeaponChange (ent);
		return;
	}

	if (is_quad)
	{
		damage *= 4;
		kick *= 4;
	}

	for (i=1 ; i<3 ; i++)
	{
		ent->client->kick_origin[i] = crandom() * 0.35f;
		ent->client->kick_angles[i] = crandom() * 0.7f;
	}
	ent->client->kick_origin[0] = crandom() * 0.35f;
	ent->client->kick_angles[0] = ent->client->machinegun_shots * -1.5f;

	// raise the gun as it is firing
	if (!deathmatch->value && !ent->client->pers.fire_mode)
	{
		ent->client->machinegun_shots++;
		if (ent->client->machinegun_shots > 9)
			ent->client->machinegun_shots = 9;
	}

	// get start / end positions
	VectorAdd (ent->client->v_angle, ent->client->kick_angles, angles);
	AngleVectors (angles, forward, right, NULL);
	VectorSet(offset, 0, 8, ent->viewheight - 8.0f);
	P_ProjectSource (ent->client, ent->s.origin, offset, forward, right, start);
	switch (ent->client->pers.fire_mode) 
	{ 
		// Fire burst
		case 1:
			// Set up the means of death.
			mod = MOD_EXPLOSIVEMACHINEGUN;
			if (i_fragban & WFB_BURSTMACHINEGUN)
				mod |= MOD_NOFRAG;

			ent->client->burstfire_count++;
			if (ent->client->burstfire_count < 4)
			{
				fire_explosive_lead (ent, start, forward, damage*4, kick/2,TE_BLASTER,
					DEFAULT_BULLET_HSPREAD/2, DEFAULT_BULLET_VSPREAD/2, mod);
				if (ent->client && ent->client->pers.special == AODEATH)
				{
					fire_explosive_lead (ent, ent->client->angel->s.origin, forward, damage*4, kick/2,TE_BLASTER,
					DEFAULT_BULLET_HSPREAD/2, DEFAULT_BULLET_VSPREAD/2, mod);
				}
				gi.WriteByte (svc_muzzleflash);
				gi.WriteShort (ent-g_edicts);
				gi.WriteByte (MZ_MACHINEGUN | is_silenced);
				gi.multicast (ent->s.origin, MULTICAST_PVS);
				PlayerNoise(ent, start, PNOISE_WEAPON);
				if (! ((int)dmflags->value & DF_INFINITE_AMMO))
					ent->client->pers.inventory[ent->client->ammo_index] -= 2;
			} 
			else if (ent->client->burstfire_count > 6) 
				ent->client->burstfire_count=0;
			break;

		// Fire Fully Automatic
		case 0:
		default:
			// Set up the means of death.
			mod = MOD_EXPLOSIVEMACHINEGUN;
			if (i_fragban & WFB_MACHINEGUN)
				mod |= MOD_NOFRAG;

			fire_explosive_lead (ent, start, forward, damage, kick,TE_BLASTER, DEFAULT_BULLET_HSPREAD,
				DEFAULT_BULLET_VSPREAD, mod);
			if (ent->client && ent->client->pers.special == AODEATH)
			{
				fire_explosive_lead (ent, ent->client->angel->s.origin, forward, damage, kick,TE_BLASTER, DEFAULT_BULLET_HSPREAD,
				DEFAULT_BULLET_VSPREAD, mod);
			}
			gi.WriteByte (svc_muzzleflash);
			gi.WriteShort (ent-g_edicts);
			gi.WriteByte (MZ_MACHINEGUN | is_silenced);
			gi.multicast (ent->s.origin, MULTICAST_PVS);
			PlayerNoise(ent, start, PNOISE_WEAPON);
			if (! ((int)dmflags->value & DF_INFINITE_AMMO))
				ent->client->pers.inventory[ent->client->ammo_index]
					-= ent->client->pers.weapon->quantity;
			break;
	}

	if (!ent->deadflag && ent->s.modelindex == 255)
	{
		// ### Hentai ### BEGIN

		ent->client->anim_priority = ANIM_ATTACK;
		if (ent->client->ps.pmove.pm_flags & PMF_DUCKED)
		{
			ent->s.frame = FRAME_crattak1 - (int) (random()+0.25);
			ent->client->anim_end = FRAME_crattak9;
		}
		else
		{
			ent->s.frame = FRAME_attack1 - (int) (random()+0.25);
			ent->client->anim_end = FRAME_attack8;
		}

		// ### Hentai ### END
	}
}

void Weapon_ExplosiveMachingun (edict_t *ent)
{
	static int	pause_frames[]	= {23, 45, 0};
	static int	fire_frames[]	= {4, 5, 0};

	Weapon_Generic (ent, 3, 5, 45, 49, pause_frames, fire_frames, ExplosiveMachinegun_Fire);
}

