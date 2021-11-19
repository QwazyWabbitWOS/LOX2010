
//************************
//      Pulse Rifle
//************************

#include "g_local.h"
#include "l_angels.h"

void pulserifle_sparks(edict_t* self)
{
	gi.WriteByte(svc_temp_entity);
	gi.WriteByte(TE_WELDING_SPARKS);
	gi.WriteByte(0);
	gi.WritePosition(self->s.origin);
	gi.WriteDir(vec3_origin);
	gi.WriteByte(0xe4 + (rand() & 3));
	gi.multicast(self->s.origin, MULTICAST_PVS);

	G_FreeEdict(self);
}

// RAFAEL
void pulserifle_touch(edict_t* self, edict_t* other, cplane_t* plane, csurface_t* surf)
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
		T_Damage(other, self, self->enemy, self->velocity, self->s.origin, plane->normal, self->dmg, 1, DAMAGE_ENERGY, MOD_BLASTER);

	}
	else
	{
		self->owner = NULL;
		//bouncing
		if (plane)
		{
			gi.WriteByte(svc_temp_entity);
			gi.WriteByte(TE_SPARKS);
			//      gi.WriteByte (damage);
			gi.WritePosition(self->s.origin);
			gi.WriteDir(plane->normal);
			gi.multicast(self->s.origin, MULTICAST_PVS);
		}
		return;
	}
	G_FreeEdict(self);
}


// RAFAEL

void fire_pulserifle(edict_t* self, vec3_t start, vec3_t dir, int damage, int speed, int effect)
{
	edict_t* pulse;
	trace_t tr;

	VectorNormalize(dir);

	pulse = G_Spawn();
	VectorCopy(start, pulse->s.origin);
	VectorCopy(start, pulse->s.old_origin);
	vectoangles(dir, pulse->s.angles);
	VectorScale(dir, (float)speed, pulse->velocity);

	pulse->movetype = MOVETYPE_BOUNCEFLY;
	pulse->clipmask = MASK_SHOT;
	pulse->solid = SOLID_BBOX;
	pulse->s.effects |= effect;

	pulse->s.renderfx |= RF_FULLBRIGHT;

	VectorClear(pulse->mins);
	VectorClear(pulse->maxs);
	pulse->s.modelindex = gi.modelindex("models/objects/buzz/tris.md2");
	pulse->s.sound = gi.soundindex("misc/lasfly.wav");
	pulse->enemy = pulse->owner = self;
	pulse->touch = pulserifle_touch;
	pulse->nextthink = level.time + 6;
	pulse->think = pulserifle_sparks;
	pulse->dmg = damage;
	pulse->dmg_radius = 100;
	pulse->classname = "pulse";
	pulse->classnum = CN_PULSE;
	gi.linkentity(pulse);

	if (self->client)
		check_dodge(self, pulse->s.origin, dir, speed);

	tr = gi.trace(self->s.origin, NULL, NULL, pulse->s.origin, pulse, MASK_SHOT);
	if (tr.fraction < 1.0)
	{
		VectorMA(pulse->s.origin, -10, dir, pulse->s.origin);
		pulse->touch(pulse, tr.ent, NULL, NULL);
	}
}


void weapon_pulserifle_fire(edict_t* ent)
{
	vec3_t	start, forward, right, offset = { 0 }, tempang = { 0 };
	int		damage, kick;

	if (!ent || !ent->client)
		return;

	if (ent->client->ps.gunframe == 5)
	{
		if (ent->client->machinegun_shots)
		{
			ent->client->kick_origin[0] = -0.2f;
			ent->client->kick_angles[0] = ent->client->machinegun_shots * -2.5f;
			ent->client->machinegun_shots--;
		}
		else
			ent->client->ps.gunframe++;

		return;
	}

	if (deathmatch->value)
	{
		// tone down for deathmatch
		damage = 50;
		kick = 60;
	}
	else
	{
		damage = 50;
		kick = 60;
	}

	if (is_quad)
	{
		damage *= 4;
		kick *= 4;
	}
	/*
	for (i=1 ; i<3 ; i++)
	{
	ent->client->kick_origin[i] = crandom() * 0.35;
	ent->client->kick_angles[i] = crandom() * 0.7;
	}
	*/
	ent->client->kick_origin[0] = -0.2f;
	ent->client->machinegun_shots = 3;
	ent->client->kick_angles[0] = ent->client->machinegun_shots * -2.5f;

	VectorCopy(ent->client->v_angle, tempang);
	tempang[YAW] += crandom();

	AngleVectors(tempang, forward, right, NULL);

	VectorScale(forward, -3, ent->client->kick_origin);
	ent->client->kick_angles[0] = -3;

	//VectorSet (offset, 0, 7, ent->viewheight - 8);
	VectorSet(offset, 16, 7, ent->viewheight - 8.0f);

	P_ProjectSource(ent->client, ent->s.origin, offset, forward, right, start);

	fire_pulserifle(ent, start, forward, damage, 500, EF_GRENADE);
	if (ent->client && ent->client->pers.special == AODEATH)
	{
		fire_pulserifle(ent, ent->client->angel->s.origin, forward, damage, 500, EF_GRENADE);
	}
	/*
	// send muzzle flash
	gi.WriteByte (svc_muzzleflash);
	gi.WriteShort (ent - g_edicts);
	gi.WriteByte (MZ_IONRIPPER | is_silenced);
	gi.multicast (ent->s.origin, MULTICAST_PVS);
	*/
	if (!ent->client->silencer_shots)
		gi.sound(ent, CHAN_VOICE, gi.soundindex("weapons/pulse1.wav"), 1, ATTN_NORM, 0);
	else
		gi.sound(ent, CHAN_VOICE, gi.soundindex("weapons/pulse1.wav"), 0.2f, ATTN_NORM, 0);

	ent->client->ps.gunframe++;
	PlayerNoise(ent, start, PNOISE_WEAPON);

	if ((!((int)dmflags->value & DF_INFINITE_AMMO))
		&& (ent->client->ammo_index))
	{
		ent->client->pers.inventory[ent->client->ammo_index] -= ent->client->pers.weapon->quantity;

		if (ent->client->pers.inventory[ent->client->ammo_index] < 0)
			ent->client->pers.inventory[ent->client->ammo_index] = 0;
	}
}

void Weapon_PulseRifle(edict_t* ent)
{
	static int pause_frames[] = { 23, 45, 0 };
	static int fire_frames[] = { 4, 5, 0 };

	Weapon_Generic(ent, 3, 5, 50, 54, pause_frames, fire_frames, weapon_pulserifle_fire);
}
