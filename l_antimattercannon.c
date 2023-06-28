
//****************************************************
//		Antimatter 
//****************************************************

#include "g_local.h"
#include "l_antimattercannon.h"

void BombExplosion(edict_t* self)
{
	T_RadiusDamage(self, self->owner, (float)self->dmg, self->enemy, self->dmg_radius, MOD_ANTIMATTERCANNON);
	gi.WriteByte(svc_temp_entity);
	if (self->waterlevel)
		gi.WriteByte(TE_ROCKET_EXPLOSION_WATER);
	else
		gi.WriteByte(TE_ROCKET_EXPLOSION);

	gi.WritePosition(self->s.origin);
	gi.multicast(self->s.origin, MULTICAST_PHS);
	G_FreeEdict(self);
}

void fire_asha(edict_t* self, vec3_t start, vec3_t aimdir, int damage, int kick, int hspread, int vspread, int mod)
{
	trace_t		tr;
	vec3_t		dir, forward, right, up, end;
	float		r, u;
	vec3_t		water_start = { 0 };
	qboolean	water = false;
	int			content_mask = MASK_SHOT | MASK_WATER;
	qboolean    is_onturret = false;

	//eo! Turret encara no posat! FIXME
	//#if 0
	//	if (self->client)
	//		is_onturret = (self->client->onturret > 0);
	//	else
	//		if (self->flags & FL_TEAMSLAVE)
	//			is_onturret=true;
	//#endif

	if (!is_onturret)
		tr = gi.trace(self->s.origin, NULL, NULL, start, self, MASK_SHOT);
	else
		tr = gi.trace(start, NULL, NULL, start, self, MASK_SHOT);

	if (!(tr.fraction < 1.0))
	{
		vectoangles(aimdir, dir);
		AngleVectors(dir, forward, right, up);

		r = crandom() * hspread;
		u = crandom() * vspread;
		VectorMA(start, 8192, forward, end);
		VectorMA(end, r, right, end);
		VectorMA(end, u, up, end);

		if (gi.pointcontents(start) & MASK_WATER)
		{
			water = true;
			VectorCopy(start, water_start);
			content_mask &= ~MASK_WATER;
		}

		tr = gi.trace(start, NULL, NULL, end, self, content_mask);

		// see if we hit water
		if (tr.contents & MASK_WATER)
		{
			int		color;

			water = true;
			VectorCopy(tr.endpos, water_start);

			if (!VectorCompare(start, tr.endpos))
			{
				if (tr.contents & CONTENTS_WATER)
				{
					if (strcmp(tr.surface->name, "*brwater") == 0)
						color = SPLASH_BROWN_WATER;
					else
						color = SPLASH_BLUE_WATER;
				}
				else if (tr.contents & CONTENTS_SLIME)
					color = SPLASH_SLIME;
				else if (tr.contents & CONTENTS_LAVA)
					color = SPLASH_LAVA;
				else
					color = SPLASH_UNKNOWN;

				if (color != SPLASH_UNKNOWN)
				{
					gi.WriteByte(svc_temp_entity);
					gi.WriteByte(TE_SPLASH);	// type
					gi.WriteByte(8);	// count
					gi.WritePosition(tr.endpos);
					gi.WriteDir(tr.plane.normal);
					gi.WriteByte(color);
					gi.multicast(tr.endpos, MULTICAST_PVS);
				}

				// change bullet's course when it enters water
				VectorSubtract(end, start, dir);
				vectoangles(dir, dir);
				AngleVectors(dir, forward, right, up);
				r = crandom() * hspread * 2;
				u = crandom() * vspread * 2;
				VectorMA(water_start, 8192, forward, end);
				VectorMA(end, r, right, end);
				VectorMA(end, u, up, end);
			}

			// re-trace ignoring water this time
			tr = gi.trace(water_start, NULL, NULL, end, self, MASK_SHOT);
		}
	}

	// send gun puff / flash
	if (!((tr.surface) && (tr.surface->flags & SURF_SKY)))
	{
		if (tr.fraction < 1.0)
		{
			edict_t* bomb;
			vec3_t bdest;

			if (tr.ent->takedamage)
			{
				T_Damage(tr.ent, self, self, aimdir, tr.endpos, tr.plane.normal, damage, kick, 0, mod);
			}

			VectorMA(tr.endpos, -4, aimdir, bdest);
			bomb = G_Spawn();
			bomb->think = BombExplosion;
			bomb->dmg = 80;
			bomb->enemy = tr.ent;
			bomb->dmg_radius = 100;
			bomb->nextthink = level.time + 0.1f + random() * 0.3f;
			VectorCopy(bdest, bomb->s.origin);
			bomb->svflags = SVF_NOCLIENT;
			bomb->owner = self;
			gi.linkentity(bomb);
		}
	}

	// if went through water, determine where the end and make a bubble trail
	if (water)
	{
		vec3_t	pos;

		VectorSubtract(tr.endpos, water_start, dir);
		VectorNormalize(dir);
		VectorMA(tr.endpos, -2, dir, pos);
		if (gi.pointcontents(pos) & MASK_WATER)
			VectorCopy(pos, tr.endpos);
		else
			tr = gi.trace(pos, NULL, NULL, water_start, tr.ent, MASK_WATER);

		VectorAdd(water_start, tr.endpos, pos);
		VectorScale(pos, 0.5, pos);

		gi.WriteByte(svc_temp_entity);
		gi.WriteByte(TE_BUBBLETRAIL);
		gi.WritePosition(water_start);
		gi.WritePosition(tr.endpos);
		gi.multicast(pos, MULTICAST_PVS);
	}
}

void weapon_asha_fire(edict_t* ent)
{
	vec3_t		start;
	vec3_t		forward, right;
	vec3_t		offset = { 0 };
	int			damage;
	int			kick;
	int                     count;

	damage = 80;
	kick = 200;

	if (is_quad)
	{
		damage *= 4;
		kick *= 4;
	}

	AngleVectors(ent->client->v_angle, forward, right, NULL);

	VectorScale(forward, -3, ent->client->kick_origin);
	ent->client->kick_angles[0] = -3;

	if (ent->groundentity)
	{
		ent->groundentity = NULL;
		ent->s.origin[2]++;
	}

	VectorMA(ent->velocity, -300, forward, ent->velocity);
	VectorSet(offset, 0, 7, ent->viewheight - 8.0f);
	P_ProjectSource(ent->client, ent->s.origin, offset, forward, right, start);

	for (count = 1; count <= 6; count++)
		fire_asha(ent, start, forward, damage, kick, 1310, 655, MOD_ANTIMATTERCANNON);

	// send muzzle flash
	gi.WriteByte(svc_muzzleflash);
	gi.WriteShort(ent - g_edicts);
	gi.WriteByte(MZ_RAILGUN | is_silenced);
	gi.multicast(ent->s.origin, MULTICAST_PVS);
	ent->client->ps.gunframe++;
	PlayerNoise(ent, start, PNOISE_WEAPON);

	if ((!((int)dmflags->value & DF_INFINITE_AMMO)) && (ent->client->ammo_index))
		ent->client->pers.inventory[ent->client->ammo_index] -= 3;
}


void Weapon_AntimatterCannon(edict_t* ent)
{
	static int	pause_frames[] = { 56, 0 };
	static int	fire_frames[] = { 4, 0 };

	Weapon_Generic(ent, 3, 18, 56, 61, pause_frames, fire_frames, weapon_asha_fire);
}
