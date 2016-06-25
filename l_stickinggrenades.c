/*
=================
stickinggrenade
=================
*/

#include "g_local.h"

// darKMajick definitions
#include "l_dm_grenades.h"

void Stick(edict_t *ent, edict_t *other)
{
	
	if (ent->movetype == MOVETYPE_STUCK)
		return;
	
	ent->movetype = MOVETYPE_STUCK;
	VectorClear(ent->velocity);
	VectorClear(ent->avelocity);
	ent->stuckentity = other;
	VectorCopy(other->s.origin,ent->oldstuckorigin);
	VectorCopy(other->s.angles,ent->oldstuckangles);
	ent->solid = SOLID_NOT;	//fix to allow player free movement

}

void StickingGrenade_Explode (edict_t *ent)
{
	vec3_t		origin;
	int			mod;
	
	
	if (ent->owner->client)
		PlayerNoise(ent->owner, ent->s.origin, PNOISE_IMPACT);
	
	mod = MOD_STICKINGGRENADES;

	//FIXME: if we are onground then raise our Z just a bit since we are a point?
	if (ent->enemy)
	{
		float	points;
		vec3_t	v;
		vec3_t	dir;
		
		VectorAdd (ent->enemy->mins, ent->enemy->maxs, v);
		VectorMA (ent->enemy->s.origin, 0.5, v, v);
		VectorSubtract (ent->s.origin, v, v);
		points = ent->dmg - 0.5f * VectorLength (v);
		VectorSubtract (ent->enemy->s.origin, ent->s.origin, dir);
		T_Damage (ent->enemy, ent, ent->owner, dir, 
				ent->s.origin, vec3_origin, (int)points, 
				(int)points, DAMAGE_RADIUS, mod);
	}
	
	T_RadiusDamage(ent, ent->owner, (float) ent->dmg, ent->enemy, (float) ent->dmg_radius, mod);
	
	VectorMA (ent->s.origin, -0.02f, ent->velocity, origin);
	gi.WriteByte (svc_temp_entity);
	if (ent->waterlevel)
	{
		if (ent->groundentity)
			gi.WriteByte (TE_GRENADE_EXPLOSION_WATER);
		else
			gi.WriteByte (TE_ROCKET_EXPLOSION_WATER);
	}
	else
	{
		if (ent->groundentity)
			gi.WriteByte (TE_GRENADE_EXPLOSION);
		else
			gi.WriteByte (TE_ROCKET_EXPLOSION);
	}
	gi.WritePosition (origin);
	gi.multicast (ent->s.origin, MULTICAST_PHS);
	
	G_FreeEdict (ent);
}

void StickingGrenade_Touch (edict_t *ent, edict_t *other, cplane_t *plane, csurface_t *surf)
{
	if (other == ent->owner)
		return;
	
	if (surf && (surf->flags & SURF_SKY))
	{
		G_FreeEdict (ent);
		return;
	}
	
	if (!other->takedamage)
	{
		if (ent->spawnflags & 1)
		{
			if (random() > 0.5)
				gi.sound (ent, CHAN_VOICE, gi.soundindex ("weapons/hgrenb1a.wav"), 1, ATTN_NORM, 0);
			else
				gi.sound (ent, CHAN_VOICE, gi.soundindex ("weapons/hgrenb2a.wav"), 1, ATTN_NORM, 0);
		}
		else
		{
			gi.sound (ent, CHAN_VOICE, gi.soundindex ("weapons/grenlb1b.wav"), 1, ATTN_NORM, 0);
		}
	}
	else
		ent->enemy = other;
	
	Stick(ent, other);
}

void fire_stickinggrenade (edict_t *self, vec3_t start, vec3_t aimdir, int damage, int speed, float timer, float damage_radius)
{
	edict_t	*grenade;
	vec3_t	dir;
	vec3_t	forward, right, up;
	int typ;
	
	if (self->classnum == CN_TRACKER) 
		typ = self->owner->client->dM_grenade;
	else 
		typ = self->client->dM_grenade;

	vectoangles (aimdir, dir);
	AngleVectors (dir, forward, right, up);
	
	grenade = G_Spawn();
	VectorCopy (start, grenade->s.origin);
	VectorScale (aimdir, (float) speed, grenade->velocity);
	VectorMA (grenade->velocity, 200 + crandom() * 10.0f, up, grenade->velocity);
	VectorMA (grenade->velocity, crandom() * 10.0f, right, grenade->velocity);
	VectorSet (grenade->avelocity, 300, 300, 300);
	grenade->movetype = MOVETYPE_BOUNCE;
	grenade->clipmask = MASK_SHOT;
	grenade->solid = SOLID_BBOX;
	grenade->s.effects |= EF_GRENADE;
	VectorClear (grenade->mins);
	VectorClear (grenade->maxs);
	grenade->s.modelindex = gi.modelindex ("models/objects/grenade/tris.md2");
	grenade->owner = self;
	grenade->touch = StickingGrenade_Touch;
	grenade->nextthink = level.time + timer;
	grenade->think = Grenade_Explode_dM;
	grenade->dmg = damage;
	grenade->dmg_radius = damage_radius;
	grenade->classname = "grenade";
	grenade->classnum = CN_GRENADE;
	grenade->dm_type = typ;
	
	SetGrenadeEffects(grenade, typ);
	
	if (self->client != NULL && self->client->expactive == 1)
	{
		grenade->nextthink = level.time + timer;
		grenade->think = Grenade_Explode_dM;
		self->client->explosive = grenade;
	}
	gi.linkentity (grenade);
}


void weapon_stickinggrenadelauncher_fire (edict_t *ent)
{
	vec3_t	start, forward, right, offset;
	int		damage = 120;
	float	radius;
	
	radius = damage + 40.0f;
	if (is_quad)
		damage *= 4;
	
	if (ent->client->explosive != NULL)
	{
		Grenade_Explode_dM (ent->client->explosive);
		ent->client->expactive = 1;
	}
	
	VectorSet(offset, 8, 8, ent->viewheight - 8.0f);
	AngleVectors (ent->client->v_angle, forward, right, NULL);
	P_ProjectSource (ent->client, ent->s.origin, offset, forward, right, start);
	
	VectorScale (forward, -2, ent->client->kick_origin);
	ent->client->kick_angles[0] = -1;

	if (ent->client->expactive == 1) 
		fire_stickinggrenade (ent, start, forward, damage, 600,100, radius);
	else 
		fire_stickinggrenade (ent, start, forward, damage, 600, 2.5, radius);
	
	gi.WriteByte (svc_muzzleflash);
	gi.WriteShort (ent-g_edicts);
	gi.WriteByte (MZ_GRENADE | is_silenced);
	gi.multicast (ent->s.origin, MULTICAST_PVS);
	
	ent->client->ps.gunframe++;
	
	PlayerNoise(ent, start, PNOISE_WEAPON);
	
	if ((!((int)dmflags->value & DF_INFINITE_AMMO))
		&&(ent->client->ammo_index))
			ent->client->pers.inventory[ent->client->ammo_index]--;
}

void Weapon_StickingGrenadeLauncher (edict_t *ent)
{
	static int	pause_frames[]	= {34, 51, 59, 0};
	static int	fire_frames[]	= {6, 0};
	
	Weapon_Generic (ent, 5, 16, 59, 64, pause_frames, fire_frames, weapon_stickinggrenadelauncher_fire);
}
