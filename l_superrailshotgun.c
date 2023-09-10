
//*****************************
//		Super Rail Shotgun
//*****************************

#include "g_local.h"

void fire_superrailshotgun(edict_t* self, vec3_t start, vec3_t aimdir, int damage, int kick)
{
	vec3_t		forward, right, up, from = { 0 }, end, dir;
	trace_t		tr;
	edict_t* ignore;
	int			mask, mod;
	qboolean	water;
	float	r, u;

	if (!self || !self->client)
		return;

	// Set up the means of death.
	mod = MOD_SUPERRAILSHOTGUN;
	// no point awarded if fragbanned
	if (i_fragban & WFB_SUPERRAILSHOTGUN)
		mod |= MOD_NOFRAG;

	VectorMA(start, 8192, aimdir, end);
	VectorCopy(start, from);
	ignore = self;
	water = false;
	mask = MASK_SHOT | CONTENTS_SLIME | CONTENTS_LAVA;

	while (ignore)
	{
		vectoangles(aimdir, dir);
		AngleVectors(dir, forward, right, up);

		r = crandom() * 2000;
		u = crandom() * 800;
		VectorMA(start, 8192, forward, end);
		VectorMA(end, r, right, end);
		VectorMA(end, u, up, end);
		tr = gi.trace(from, NULL, NULL, end, ignore, mask);

		if (tr.contents & (CONTENTS_SLIME | CONTENTS_LAVA))
		{
			mask &= ~(CONTENTS_SLIME | CONTENTS_LAVA);
			water = true;
		}
		else
		{
			if ((tr.ent->svflags & SVF_MONSTER) || (tr.ent->client))
				ignore = tr.ent;
			else
				ignore = NULL;

			if (tr.ent != self && tr.ent->takedamage)
				T_Damage(tr.ent, self, self, aimdir, tr.endpos, tr.plane.normal, damage, 1, 0, mod);
		}

		VectorCopy(tr.endpos, from);
	}

	// send gun puff / flash
	gi.WriteByte(svc_temp_entity);
	gi.WriteByte(TE_RAILTRAIL);

	gi.WritePosition(start);
	gi.WritePosition(tr.endpos);
	gi.multicast(self->s.origin, MULTICAST_PHS);

	gi.WriteByte(svc_temp_entity);
	gi.WriteByte(TE_BUBBLETRAIL);

	gi.WritePosition(start);
	gi.WritePosition(tr.endpos);
	gi.multicast(self->s.origin, MULTICAST_PHS);

	if (water)
	{
		gi.WriteByte(svc_temp_entity);
		gi.WriteByte(TE_RAILTRAIL);
		gi.WritePosition(start);
		gi.WritePosition(tr.endpos);
		gi.multicast(tr.endpos, MULTICAST_PHS);
	}

	if (self->client)
		PlayerNoise(self, tr.endpos, PNOISE_IMPACT);
}

void weapon_superrailshotgun_fire(edict_t* ent)
{
	vec3_t		start, forward, right, offset = { 0 }, v = { 0 };
	int			damage;
	int			kick;
	int			i;

	// Same "extreme" damage in SP game *and* deathmatch :)
	damage = 40;
	kick = 2;

	if (is_quad)
	{
		damage *= 4;
		kick *= 4;
	}

	AngleVectors(ent->client->v_angle, forward, right, NULL);

	VectorScale(forward, -3, ent->client->kick_origin);
	ent->client->kick_angles[0] = -3;

	VectorSet(offset, 0, 7, ent->viewheight - 8.0f);
	P_ProjectSource(ent->client, ent->s.origin, offset, forward, right, start);

	v[PITCH] = ent->client->v_angle[PITCH];
	v[ROLL] = ent->client->v_angle[ROLL];
	v[YAW] = ent->client->v_angle[YAW] - 10;
	AngleVectors(v, forward, NULL, NULL);

	for (i = 0; i < 5; i++)	// on the left
		fire_superrailshotgun(ent, start, forward, damage, kick);

	v[YAW] = ent->client->v_angle[YAW] + 10;
	AngleVectors(v, forward, NULL, NULL);

	for (i = 0; i < 5; i++)	// on the right
		fire_superrailshotgun(ent, start, forward, damage, kick);

	// send muzzle flash
	gi.WriteByte(svc_muzzleflash);
	gi.WriteShort(ent - g_edicts);
	gi.WriteByte(MZ_RAILGUN | MZ_SILENCED);
	gi.multicast(ent->s.origin, MULTICAST_PVS);

	ent->client->ps.gunframe++;
	PlayerNoise(ent, start, PNOISE_WEAPON);

	if (!((int)dmflags->value & DF_INFINITE_AMMO))
		ent->client->pers.inventory[ent->client->ammo_index]
		-= ent->client->pers.weapon->quantity;
}


void Weapon_SuperRailShotgun(edict_t* ent)
{
	static int	pause_frames[] = { 56, 0 };
	static int	fire_frames[] = { 4, 0 };

	Weapon_Generic(ent, 3, 18, 56, 61, pause_frames, fire_frames, weapon_superrailshotgun_fire);
}
