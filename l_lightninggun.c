
/***************************************************/
/**************    lightning gun      **************/
/***************************************************/

#include "g_local.h"
#include "l_angels.h"
#include "m_player.h"

void fire_lightning (edict_t *self, vec3_t start, vec3_t dir, int damage)
{
	trace_t	tr;
	vec3_t	end, offset;
	edict_t	*ent;
	
	//Wyrm: check for viewent
	if (self->client && (self->client->chasetoggle == 1 || 
		self->client->missile)&&(self->client->oldplayer))
	{
		ent = self->client->oldplayer;
		VectorClear(offset);
	}
	else
	{
		ent = self;
		VectorSubtract(start, ent->s.origin, offset);
	}
	
	VectorMA (start, 600, dir, end);
	
	tr = gi.trace (start, NULL, NULL, end, self, MASK_SHOT| MASK_WATER);
	
	if (tr.fraction < 1.0)
	{
		if (tr.contents & MASK_WATER) // kill yourself playing with electricity and water
		{
			T_WaterRadiusDamage (self, self, 35, NULL, 80, MOD_LIGHTNING);
		}
		else if ((tr.ent != NULL)&&(tr.ent->takedamage)) //Ha tocat una entitat que es pot ferir
		{
			T_Damage (tr.ent, self, self, dir, tr.endpos, tr.plane.normal, damage, 0, DAMAGE_ENERGY, MOD_LIGHTNING);
		}
		else
		{
			// sparks
			gi.WriteByte (svc_temp_entity);
			gi.WriteByte (TE_SPARKS);
			gi.WritePosition (tr.endpos);
			gi.WriteDir (tr.plane.normal);
			gi.multicast (self->s.origin, MULTICAST_PVS);
		}
	}

	// the beam
	gi.WriteByte (svc_temp_entity);
	gi.WriteByte (TE_GRAPPLE_CABLE);
	gi.WriteShort (ent - g_edicts);
	gi.WritePosition (start);
	gi.WritePosition (tr.endpos);
	gi.WritePosition (offset);
	
	gi.multicast (ent->s.origin, MULTICAST_PVS);
}

void weapon_lightning_fire (edict_t *ent)
{
	vec3_t		start, forward, right, offset;
	int			damage;

	if (!ent || !ent->client)
		return;
	
	damage = 30;
	
	if (is_quad)
		damage *= 4;
	
	if (!(ent->client->buttons & BUTTON_ATTACK))
	{
		ent->client->ps.gunframe = 18;
		ent->client->machinegun_shots = 0;
		return;
	}
	
	if ((ent->client->ammo_index)&&
		(ent->client->pers.inventory[ent->client->ammo_index] < 1))
	{
		ent->client->ps.gunframe = 18;
		ent->client->machinegun_shots = 0;
		if (level.time >= ent->pain_debounce_time)
		{
			gi.sound(ent, CHAN_VOICE, gi.soundindex("weapons/noammo.wav"), 1, ATTN_NORM, 0);
			ent->pain_debounce_time = level.time + 1;
		}
		NoAmmoWeaponChange (ent);
		return;
	}
	
	AngleVectors (ent->client->v_angle, forward, right, NULL);
	
	VectorSet(offset, 0, 7,  ent->viewheight - 8.0f);
	P_ProjectSource (ent->client, ent->s.origin, offset, forward, right, start);
	
	VectorScale (forward, -2, ent->client->kick_origin);
	ent->client->kick_angles[0] = -1;
	
	if (gi.pointcontents (start) & MASK_WATER)
	{
		int cells;
		
		if (ent->client->ammo_index)
		{
			cells = ent->client->pers.inventory[ent->client->ammo_index];
			ent->client->pers.inventory[ent->client->ammo_index] = 0;
		}
		else
			cells = 100;
		
		T_WaterRadiusDamage (ent, ent, 35.0f * cells, NULL, 500, MOD_DISCHARGE);
	}
	else
	{
		fire_lightning (ent, start, forward, damage);
		if (ent->client && ent->client->pers.special == AODEATH)
		{
			fire_lightning (ent, ent->client->angel->s.origin, forward,damage);
		}
		if ((! ( (int)dmflags->value & DF_INFINITE_AMMO ) )
			&&(ent->client->ammo_index))
			ent->client->pers.inventory[ent->client->ammo_index]--;
	}
	
	if (!ent->client->machinegun_shots)
	{
		if (!ent->client->silencer_shots)
			gi.sound(ent, CHAN_AUTO, gi.soundindex("light/lstart.wav"), 1, ATTN_NORM, 0);
		else
			gi.sound(ent, CHAN_AUTO, gi.soundindex("light/lstart.wav"), 0.2f, ATTN_NORM, 0);
		ent->client->machinegun_shots = 1;
	}
	
	if (ent->client->machinegun_shots == 1)
	{
		if (!ent->client->silencer_shots)
			gi.sound(ent, CHAN_VOICE, gi.soundindex("light/lhit.wav"), 1, ATTN_NORM, 0);
		else
			gi.sound(ent, CHAN_VOICE, gi.soundindex("light/lhit.wav"), 0.2f, ATTN_NORM, 0);
		ent->client->machinegun_shots = 2;
	}
	else if (ent->client->machinegun_shots < 7)
		ent->client->machinegun_shots++;
	else
		ent->client->machinegun_shots = 1;
	
	//        ent->client->ps.gunframe++;
	PlayerNoise(ent, start, PNOISE_WEAPON);
	
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
}


void Weapon_LightningGun (edict_t *ent)
{
	static int	pause_frames[]	= {56, 0};
	static int	fire_frames[]	= {4, 0};
	
	Weapon_Generic (ent, 3, 18, 56, 61, pause_frames, fire_frames, weapon_lightning_fire);
}
