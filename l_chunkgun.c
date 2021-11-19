
//****************************
//		Chunk gun
//****************************


#include "g_local.h"
#include "m_player.h"
#include "l_angels.h"

//#define SFM_CHUNKGUN  // used to compile high/low sweeper fire mode chunkgun

void chunk_touch(edict_t* self, edict_t* other, cplane_t* plane, csurface_t* surf)
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
		T_Damage(other, self, self->owner, self->velocity, self->s.origin, plane->normal, self->dmg, 1, DAMAGE_BULLET, self->count);
		if (other->health > 0)
		{
			other->plaguetime = (int)(level.time + 6.0f);
			other->plaguedamage = 1;
			other->damagecauser = self->owner;
		}
	}
	else
	{
		if (plane)
		{
			gi.WriteByte(svc_temp_entity);
			gi.WriteByte(TE_GUNSHOT);
			//gi.WriteByte (damage);
			gi.WritePosition(self->s.origin);
			gi.WriteDir(plane->normal);
			gi.multicast(self->s.origin, MULTICAST_PVS);
		}
	}

	//G_FreeEdict (self);
}

void fire_chunk(edict_t* self, vec3_t start, vec3_t dir, int damage, int speed, int mod)
{
	edict_t* chunk;
	vec3_t	forward, right, up;
	char* models[11] = { "models/objects/gibs/arm/tris.md2","models/objects/gibs/bone/tris.md2",
		"models/objects/gibs/bone2/tris.md2","models/objects/gibs/chest/tris.md2",
		"models/objects/gibs/gear/tris.md2","models/objects/gibs/head/tris.md2",
		"models/objects/gibs/head2/tris.md2","models/objects/gibs/leg/tris.md2",
		"models/objects/gibs/skull/tris.md2","models/objects/gibs/sm_meat/tris.md2",
		"models/objects/gibs/sm_metal/tris.md2" };

	char* themodel;
	int thenumber;

	thenumber = (int)(random() * 100);
	thenumber = thenumber % 11;
	themodel = models[thenumber];
	VectorNormalize(dir);

	AngleVectors(dir, forward, right, up);

	chunk = G_Spawn();


	VectorCopy(start, chunk->s.origin);
	VectorCopy(start, chunk->s.old_origin);
	vectoangles(dir, chunk->s.angles);
	VectorScale(dir, (float)speed, chunk->velocity);

	//VectorCopy (start, chunk->s.origin);
	//	VectorScale (forward, speed, chunk->velocity);

	VectorMA(chunk->velocity, 200 + crandom() * 10.0f, up, chunk->velocity);
	VectorMA(chunk->velocity, crandom() * 10.0f, right, chunk->velocity);
	VectorSet(chunk->avelocity, 300, 300, 300);

	chunk->movetype = MOVETYPE_BOUNCE;
	chunk->clipmask = MASK_SHOT;
	chunk->solid = SOLID_BBOX;
	chunk->s.effects |= EF_FLIES;

	VectorClear(chunk->mins);
	VectorClear(chunk->maxs);

	chunk->s.modelindex = gi.modelindex(themodel);
	chunk->enemy = chunk->owner = self;
	chunk->touch = chunk_touch;
	chunk->nextthink = level.time + 5;
	chunk->think = G_FreeEdict;
	chunk->dmg = damage;
	chunk->count = mod;
	chunk->classname = "chunk";
	chunk->classnum = CN_CHUNK;

	if (self->client)
		check_dodge(self, chunk->s.origin, dir, speed);

	gi.linkentity(chunk);
}

void Chunkgun_Fire(edict_t* ent)
{
	int	i;
	vec3_t		start, forward, right, angles = { 0 };
	int			damage = 10;
	int			kick = 2;
	vec3_t		offset = { 0 };

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
	fire_chunk(ent, start, forward, damage, 500, MOD_PLAGUE);
	if (ent->client->sweeperfiremode == SFM_HIGH)
		ent->client->curr_heat += WEPHEAT_HIGH;
	else
		ent->client->curr_heat += WEPHEAT_LOW;

	if (ent->client && ent->client->pers.special == AODEATH)
	{
		fire_chunk(ent, ent->client->angel->s.origin, forward, damage, 500, MOD_PLAGUE);
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

void weapon_chunkgun_fire(edict_t* ent)
{
	vec3_t		start, forward, right, offset = { 0 };
	int			damage = 10;
	int			kick = 2;
	int			mod;

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
	mod = MOD_PLAGUE;
	if (i_fragban & WFB_CHUNKGUN)
		mod |= MOD_NOFRAG;

	if (deathmatch->value)
	{
		fire_chunk(ent, start, forward, damage, 500, MOD_PLAGUE);
		if (ent->client && ent->client->pers.special == AODEATH)
		{
			fire_chunk(ent, ent->client->angel->s.origin, forward, damage, 500, MOD_PLAGUE);
		}
	}
	else
	{
		fire_chunk(ent, start, forward, damage, 500, MOD_PLAGUE);
		if (ent->client && ent->client->pers.special == AODEATH)
		{
			fire_chunk(ent, ent->client->angel->s.origin, forward, damage, 500, MOD_PLAGUE);
		}
	}
	// send muzzle flash
	gi.WriteByte(svc_muzzleflash);
	gi.WriteShort(ent - g_edicts);
	gi.WriteByte(MZ_MACHINEGUN | is_silenced);
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

//QW//
#ifdef SFM_CHUNKGUN		//if sweeper fire mode controls chunkgun

void Weapon_Chunkgun(edict_t* ent)
{
	if (ent->client->sweeperfiremode == SFM_HIGH)
	{
		static int	pause_frames[] = { 38, 43, 51, 61, 0 };
		static int	fire_frames[] = { 5, 7, 9, 11, 13, 15, 17, 19, 21, 0 };
		Weapon_Generic(ent, 4, 31, 61, 64, pause_frames, fire_frames,
			weapon_chunkgun_fire);
	}
	else if (ent->client->sweeperfiremode == SFM_LOW)
	{
		static int	pause_frames[] = { 38, 43, 51, 61, 0 };
		static int	fire_frames[] = { 5, 9, 13, 17, 21, 0 };
		Weapon_Generic(ent, 4, 31, 61, 64, pause_frames, fire_frames,
			weapon_chunkgun_fire);
	}
}

#else		//old method was fast anyway

void Weapon_Chunkgun(edict_t* ent)
{
	static int	pause_frames[] = { 38, 43, 51, 61, 0 };
	static int	fire_frames[] = { 5, 7, 9, 11, 13, 15, 17, 19, 21, 0 };

	// Hacked-up chaingun spec
	Weapon_Generic(ent, 4, 31, 61, 64, pause_frames, fire_frames,
		weapon_chunkgun_fire);
}

#endif //SFM_CHUNKGUN
