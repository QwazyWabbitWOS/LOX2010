#include "g_local.h"
#include "l_bucky.h"
#include "l_angels.h"

/*
=================
fire_buckyball
=================
*/

void BuckyBall_Explode(edict_t* ent)
{
	vec3_t		origin, dir = { 0 }, v = { 0 };
	int		mod;
	float	points;


	if (ent->owner->client)
		PlayerNoise(ent->owner, ent->s.origin, PNOISE_IMPACT);

	//FIXME: if we are onground then raise our Z just a bit since we are a point?

	if (ent->enemy)
	{

		VectorAdd(ent->enemy->mins, ent->enemy->maxs, v);
		VectorMA(ent->enemy->s.origin, 0.5, v, v);
		VectorSubtract(ent->s.origin, v, v);
		points = ent->dmg - 0.5f * VectorLength(v);
		points = points * 0.45f;
		VectorSubtract(ent->enemy->s.origin, ent->s.origin, dir);
		mod = MOD_BUCKY;
		T_Damage(ent->enemy, ent, ent->owner, dir, ent->s.origin, vec3_origin, (int)points, (int)points, DAMAGE_RADIUS, mod);
	}

	mod = MOD_BUCKY;

	T_RadiusDamage(ent, ent->owner, (float)ent->dmg, ent->enemy, (float)ent->dmg_radius, mod);

	VectorMA(ent->s.origin, -0.02f, ent->velocity, origin);
	gi.WriteByte(svc_temp_entity);
	if (ent->waterlevel)
	{
		if (ent->groundentity)
			gi.WriteByte(TE_GRENADE_EXPLOSION_WATER);
		else
			gi.WriteByte(TE_ROCKET_EXPLOSION_WATER);
	}
	else
	{
		if (ent->groundentity)
			gi.WriteByte(TE_GRENADE_EXPLOSION);
		else
			gi.WriteByte(TE_ROCKET_EXPLOSION);
	}
	gi.WritePosition(origin);
	gi.multicast(ent->s.origin, MULTICAST_PHS);

	ent->enemy = NULL;

	if (ent->count == 5)
		G_FreeEdict(ent);
	else
	{
		//Tornar a posar vel
		VectorCopy(ent->velocity, dir);
		VectorNormalize(dir);
		VectorScale(dir, (float)ent->health, ent->velocity);
	}

	ent->count++;
}

void BuckyBall_Touch(edict_t* ent, edict_t* other, cplane_t* plane, csurface_t* surf)
{
	if (other == ent->owner)
		return;

	if (surf && (surf->flags & SURF_SKY))
	{
		G_FreeEdict(ent);
		return;
	}

#if 0
	if (!other->takedamage)
	{
		if (ent->spawnflags & 1)
		{
			if (random() > 0.5)
				gi.sound(ent, CHAN_VOICE, gi.soundindex("weapons/hgrenb1a.wav"), 1, ATTN_NORM, 0);
			else
				gi.sound(ent, CHAN_VOICE, gi.soundindex("weapons/hgrenb2a.wav"), 1, ATTN_NORM, 0);
		}
		else
		{
			gi.sound(ent, CHAN_VOICE, gi.soundindex("weapons/grenlb1b.wav"), 1, ATTN_NORM, 0);
		}
		return;
	}
#endif
	if (other->takedamage)
		ent->enemy = other;

	BuckyBall_Explode(ent);
}

void BuckyBall_OnGroundThink(edict_t* ent)
{
	//Check if onground. If so, explode
	if (ent->groundentity || !ent->velocity[2])
	{
		BuckyBall_Explode(ent);
		ent->nextthink = level.time + 5 * FRAMETIME;
	}
	else
	{
		ent->think = BuckyBall_Think;
		ent->nextthink = level.time + FRAMETIME;
	}
}

void BuckyBall_Think(edict_t* ent)
{
	//Check if onground. If so, prepare explode
	if (ent->groundentity || !ent->velocity[2])
	{
		ent->think = BuckyBall_OnGroundThink;
		ent->nextthink = level.time + 5 * FRAMETIME;
	}
	else
		ent->nextthink = level.time + FRAMETIME;
}

void fire_buckyball(edict_t* self, vec3_t start, vec3_t aimdir, int damage, int speed, float damage_radius)
{
	edict_t* buckyball;
	vec3_t	dir;
	vec3_t	forward, right, up;

	vectoangles(aimdir, dir);
	AngleVectors(dir, forward, right, up);

	buckyball = G_Spawn();
	VectorCopy(start, buckyball->s.origin);
	VectorScale(aimdir, (float)speed, buckyball->velocity);
	VectorMA(buckyball->velocity, 200 + crandom() * 10.0f, up, buckyball->velocity);
	VectorMA(buckyball->velocity, crandom() * 10.0f, right, buckyball->velocity);
	VectorSet(buckyball->avelocity, 300, 300, 300);
	buckyball->movetype = MOVETYPE_BOUNCE;
	buckyball->clipmask = MASK_SHOT;
	buckyball->solid = SOLID_BBOX;
	buckyball->s.effects |= EF_GRENADE;
	VectorClear(buckyball->mins);
	VectorClear(buckyball->maxs);
	buckyball->s.modelindex = gi.modelindex("models/objects/hgproxy/tris.md2");
	//       buckyball->s.modelindex = gi.modelindex ("models/items/ammo/grenades/medium/tris.md2");

	buckyball->owner = self;
	buckyball->touch = BuckyBall_Touch;
	buckyball->nextthink = level.time + FRAMETIME;
	buckyball->think = BuckyBall_Think;
	buckyball->dmg = damage;
	buckyball->dmg_radius = damage_radius;
	buckyball->classname = "buckyball";
	buckyball->classnum = CN_BUCKEYBALL;
	buckyball->takedamage = DAMAGE_NO;
	buckyball->health = 1000;
	buckyball->count = 0;

	gi.linkentity(buckyball);
}

void Weapon_Bucky_Fire(edict_t* ent)
{
	int		damage;
	vec3_t	start;
	vec3_t	forward, right;
	vec3_t	angles = { 0 };
	vec3_t	offset = { 0 };

	char* temp;
	char* temp2[10] = { "misc/giggle1.wav","misc/giggle2.wav","misc/giggle3.wav","misc/giggle4.wav",
		"misc/giggle5.wav","misc/giggle6.wav","misc/giggle7.wav","misc/giggle8.wav",
		"misc/giggle9.wav","misc/giggle10.wav" };
	damage = BUCKY_DAMAGE;

	temp = temp2[((int)(random() * 100.0)) % 10];

	// get start / end positions
	VectorAdd(ent->client->v_angle, ent->client->kick_angles, angles);
	AngleVectors(angles, forward, right, NULL);
	VectorSet(offset, 0, 8, ent->viewheight - 8.0f);
	P_ProjectSource(ent->client, ent->s.origin, offset, forward, right, start);

	fire_buckyball(ent, start, forward, damage, BUCKY_SPEED, (float)damage + 40);
	if (ent->client && ent->client->pers.special == AODEATH)
		fire_buckyball(ent, ent->client->angel->s.origin, forward, damage, BUCKY_SPEED, (float)damage + 40);

	if (!ent->client->silencer_shots)
		gi.sound(ent, CHAN_VOICE, gi.soundindex(temp), 1, ATTN_NORM, 0);
	else
		gi.sound(ent, CHAN_VOICE, gi.soundindex(temp), 0.2f, ATTN_NORM, 0);

	PlayerNoise(ent, start, PNOISE_WEAPON);

	if ((!((int)dmflags->value & DF_INFINITE_AMMO))
		&& (ent->client->ammo_index))
		ent->client->pers.inventory[ent->client->ammo_index] -= 3;
	ent->client->ps.gunframe++;
}

void Weapon_Bucky(edict_t* ent)
{
	static int	pause_frames[] = { 19, 32, 0 };
	static int	fire_frames[] = { 5, 0 };

	Weapon_Generic(ent, 4, 8, 52, 55, pause_frames, fire_frames, Weapon_Bucky_Fire);
}
