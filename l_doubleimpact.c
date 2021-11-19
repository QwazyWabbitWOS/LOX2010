/***************************************************/
/**************    double impact      **************/
/***************************************************/

#include "g_local.h"

#include "l_angels.h"
#include "l_doubleimpact.h"

// darKMajick definitions
#include "l_dm_grenades.h"

void fire_double(edict_t* self, vec3_t start, vec3_t aimdir, int damage, int speed, float timer, float damage_radius)
{
	edict_t* grenade;
	vec3_t	dir, forward, right, up;
	int typ;

	typ = self->client->dM_grenade;
	vectoangles(aimdir, dir);
	AngleVectors(dir, forward, right, up);

	grenade = G_Spawn();
	VectorCopy(start, grenade->s.origin);
	VectorScale(aimdir, (float)speed, grenade->velocity);
	VectorMA(grenade->velocity, 200 + crandom() * 10.0f, up, grenade->velocity);
	VectorMA(grenade->velocity, crandom() * 10.0f, right, grenade->velocity);
	VectorSet(grenade->avelocity, 300, 300, 300);
	grenade->movetype = MOVETYPE_BOUNCE;
	grenade->clipmask = MASK_SHOT;
	grenade->solid = SOLID_BBOX;
	grenade->s.effects |= EF_GRENADE;
	VectorClear(grenade->mins);
	VectorClear(grenade->maxs);
	grenade->s.modelindex = gi.modelindex("models/objects/grenade2/tris.md2");
	grenade->owner = self;
	grenade->touch = Grenade_Touch;
	grenade->nextthink = level.time + timer;
	grenade->think = Grenade_Explode_dM;
	grenade->dmg = damage;
	grenade->dmg_radius = damage_radius;
	grenade->classname = "grenade";
	grenade->classnum = CN_GRENADE;
	grenade->dm_type = typ;

	SetGrenadeEffects(grenade, typ);

	if (self->client->expactive == 1)
	{
		grenade->nextthink = level.time + timer;
		grenade->think = Grenade_Explode_dM;
		self->client->explosive = grenade;
	}
	gi.linkentity(grenade);
}

void weapon_dgsupershotgun_fire(edict_t* ent)
{
	vec3_t	start, forward, right, offset = { 0 }, v = { 0 };
	int		damage = 100;

	int radius = damage + 40;

	AngleVectors(ent->client->v_angle, forward, right, NULL);

	VectorScale(forward, -2, ent->client->kick_origin);
	ent->client->kick_angles[0] = -2;

	VectorSet(offset, 0, 8, ent->viewheight - 8.0f);
	P_ProjectSource(ent->client, ent->s.origin, offset, forward, right, start);

	if (is_quad)
		damage *= 4;

	v[PITCH] = ent->client->v_angle[PITCH];
	v[YAW] = ent->client->v_angle[YAW] - 6;
	v[ROLL] = ent->client->v_angle[ROLL];
	AngleVectors(v, forward, NULL, NULL);
	if (ent->client && ent->client->pers.special == AODEATH)
	{
		fire_double(ent, ent->client->angel->s.origin, forward, damage, 600, 2 + 0.5f * random(), (float)radius);
	}
	fire_double(ent, start, forward, damage, 600, 2.0f + 0.5f * random(), (float)radius);

	v[YAW] = ent->client->v_angle[YAW] + 6;
	AngleVectors(v, forward, NULL, NULL);
	if (ent->client && ent->client->pers.special == AODEATH)
	{
		fire_double(ent, ent->client->angel->s.origin, forward, damage, 600, 2 + 0.5f * random(), (float)radius);
	}
	fire_double(ent, start, forward, damage, 600, 2 + 0.5f * random(), (float)radius);

	// send muzzle flash
	gi.WriteByte(svc_muzzleflash);
	gi.WriteShort(ent - g_edicts);
	gi.WriteByte(MZ_GRENADE | is_silenced);
	gi.multicast(ent->s.origin, MULTICAST_PVS);

	ent->client->ps.gunframe++;
	PlayerNoise(ent, start, PNOISE_WEAPON);

	if ((!((int)dmflags->value & DF_INFINITE_AMMO))
		&& (ent->client->ammo_index))
		ent->client->pers.inventory[ent->client->ammo_index] -= 2;
}

void Weapon_DGSuperShotgun(edict_t* ent)
{
	static int	pause_frames[] = { 29, 42, 57, 0 };
	static int	fire_frames[] = { 7, 0 };

	Weapon_Generic(ent, 6, 17, 57, 61, pause_frames, fire_frames, weapon_dgsupershotgun_fire);
}
