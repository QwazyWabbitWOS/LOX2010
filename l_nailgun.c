
//********************
//		Nailgun
//********************

#include "g_local.h"
#include "l_angels.h"
#include "m_player.h"


// RAFAEL
void spike_touch(edict_t* self, edict_t* other, cplane_t* plane, csurface_t* surf)
{
	if (other == self->owner)
		return;

	if (surf && (surf->flags & SURF_SKY))
	{
		G_FreeEdict(self);
		return;
	}

	if (self->enemy->client)
		PlayerNoise(self->enemy, self->s.origin, PNOISE_IMPACT);

	if (other->takedamage)
	{
		T_Damage(other, self,
			self->owner, self->velocity,
			self->s.origin, plane->normal,
			self->dmg, 1, DAMAGE_BULLET, self->count);

	}
	else
	{
		if (plane)
		{
			gi.WriteByte(svc_temp_entity);
			gi.WriteByte(TE_GUNSHOT);
			//      gi.WriteByte (damage);
			gi.WritePosition(self->s.origin);
			gi.WriteDir(plane->normal);
			gi.multicast(self->s.origin, MULTICAST_PVS);
		}
	}

	G_FreeEdict(self);
}


// RAFAEL
void fire_nail(edict_t* self, vec3_t start, vec3_t dir, int damage, int speed, int mod)
{
	edict_t* spike;
	trace_t tr;

	VectorNormalize(dir);

	spike = G_Spawn();
	VectorCopy(start, spike->s.origin);
	VectorCopy(start, spike->s.old_origin);
	vectoangles(dir, spike->s.angles);
	VectorScale(dir, (float)speed, spike->velocity);
	spike->movetype = MOVETYPE_FLYMISSILE;
	spike->clipmask = MASK_SHOT;
	spike->solid = SOLID_BBOX;
	VectorClear(spike->mins);
	VectorClear(spike->maxs);
	spike->s.modelindex = gi.modelindex("models/drainer/tris.md2");
	spike->enemy = spike->owner = self;
	spike->touch = spike_touch;
	spike->nextthink = level.time + 6;
	spike->think = G_FreeEdict;
	spike->dmg = damage;
	spike->count = mod;
	spike->classname = "spike";
	spike->classnum = CN_SPIKE;
	gi.linkentity(spike);

	if (self->client)
		check_dodge(self, spike->s.origin, dir, speed);

	tr = gi.trace(self->s.origin, NULL, NULL, spike->s.origin, spike, MASK_SHOT);
	if (tr.fraction < 1.0)
	{
		VectorMA(spike->s.origin, -10, dir, spike->s.origin);
		spike->touch(spike, tr.ent, NULL, NULL);
	}

}

/*******************************/
/******Super Nailgun************/
/*******************************/
void SuperNailgun_Fire(edict_t* ent)
{
	int			i;
	vec3_t		start, forward, right, up, offset = { 0 };
	float		r, u;
	int			damage = 18;
	int			kick = 2;

	if (!ent || !ent->client)
		return;

	if (ent->client->ps.gunframe == 5)
		gi.sound(ent, CHAN_AUTO, gi.soundindex("weapons/chngnu1a.wav"), 1, ATTN_IDLE, 0);

	if ((ent->client->ps.gunframe == 14) && !(ent->client->buttons & BUTTON_ATTACK))
	{
		ent->client->ps.gunframe = 32;
		ent->client->weapon_sound = 0;
		return;
	}
	else if ((ent->client->ps.gunframe == 21) && (ent->client->buttons & BUTTON_ATTACK)
		&&
		((ent->client->pers.inventory[ent->client->ammo_index] > 1) ||
			(!ent->client->ammo_index))
		)
	{
		ent->client->ps.gunframe = 15;
	}
	else
	{
		ent->client->ps.gunframe++;
	}

	if (ent->client->ps.gunframe == 22)
	{
		ent->client->weapon_sound = 0;
		gi.sound(ent, CHAN_AUTO, gi.soundindex("weapons/chngnd1a.wav"), 1, ATTN_IDLE, 0);
	}
	else
	{
		ent->client->weapon_sound = gi.soundindex("weapons/chngnl1a.wav");
	}

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

	if (ent->client->ammo_index &&
		(ent->client->pers.inventory[ent->client->ammo_index] < 2))
	{
		if (level.time >= ent->pain_debounce_time)
		{
			gi.sound(ent, CHAN_VOICE, gi.soundindex("weapons/noammo.wav"), 1, ATTN_NORM, 0);
			ent->pain_debounce_time = level.time + 1;
		}
		NoAmmoWeaponChange(ent);
		return;
	}

	if (is_quad)
	{
		damage *= 4;
		kick *= 4;
	}

	for (i = 0; i < 3; i++)
	{
		ent->client->kick_origin[i] = crandom() * 0.35f;
		ent->client->kick_angles[i] = crandom() * 0.7f;
	}

	// get start / end positions
	AngleVectors(ent->client->v_angle, forward, right, up);
	r = 7 + crandom() * 4;
	u = crandom() * 4 - 1;
	VectorSet(offset, 0, r, u + ent->viewheight - 8.0f);
	P_ProjectSource(ent->client, ent->s.origin, offset, forward, right, start);

	fire_nail(ent, start, forward, damage, 1000, MOD_SUPERNAIL);
	if (ent->client && ent->client->pers.special == AODEATH)
	{
		fire_nail(ent, ent->client->angel->s.origin, forward, damage, 1000, MOD_SUPERNAIL);
	}
	// send muzzle flash
	gi.WriteByte(svc_muzzleflash);
	gi.WriteShort(ent - g_edicts);
	gi.WriteByte((MZ_CHAINGUN1) | is_silenced);
	gi.multicast(ent->s.origin, MULTICAST_PVS);

	PlayerNoise(ent, start, PNOISE_WEAPON);

	if ((!((int)dmflags->value & DF_INFINITE_AMMO))
		&& (ent->client->ammo_index))
		ent->client->pers.inventory[ent->client->ammo_index] -= 2;
}

void Weapon_SuperNailgun(edict_t* ent)
{
	static int	pause_frames[] = { 38, 43, 51, 61, 0 };
	static int	fire_frames[] = { 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 0 };

	Weapon_Generic(ent, 4, 31, 61, 64, pause_frames, fire_frames, SuperNailgun_Fire);
}

void Nailgun_Fire(edict_t* ent)
{
	int	i;
	vec3_t	start, forward, right, angles = { 0 }, offset = { 0 };
	int		damage = 18;
	int		kick = 2;

	if (is_quad)
	{
		damage *= 4;
		kick *= 4;
	}

	for (i = 1; i < 3; i++)
	{
		ent->client->kick_origin[i] = crandom() * 0.35f;
		ent->client->kick_angles[i] = crandom() * 0.7f;
	}
	ent->client->kick_origin[0] = crandom() * 0.35f;
	ent->client->kick_angles[0] = 0;

	// get start / end positions
	VectorAdd(ent->client->v_angle, ent->client->kick_angles, angles);
	AngleVectors(angles, forward, right, NULL);
	VectorSet(offset, 0, 8, ent->viewheight - 8.0f);
	P_ProjectSource(ent->client, ent->s.origin, offset, forward, right, start);
	fire_nail(ent, start, forward, damage, 1000, MOD_NAIL);
	if (ent->client && ent->client->pers.special == AODEATH)
	{
		fire_nail(ent, ent->client->angel->s.origin, forward, damage, 1000, MOD_NAIL);
	}
	gi.WriteByte(svc_muzzleflash);
	gi.WriteShort(ent - g_edicts);
	gi.WriteByte(MZ_MACHINEGUN | is_silenced);
	gi.multicast(ent->s.origin, MULTICAST_PVS);

	PlayerNoise(ent, start, PNOISE_WEAPON);

	if ((!((int)dmflags->value & DF_INFINITE_AMMO))
		&& (ent->client->ammo_index))
		ent->client->pers.inventory[ent->client->ammo_index] -= 2;

	ent->client->ps.gunframe++;
}

void Weapon_Nailgun(edict_t* ent)
{
	static int	pause_frames[] = { 23, 45, 0 };
	static int  fire_frames[] = { 4, 0 };

	Weapon_Generic(ent, 3, 5, 45, 49, pause_frames, fire_frames, Nailgun_Fire);
}
