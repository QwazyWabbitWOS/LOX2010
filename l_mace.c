
/***************************************************/
/**************       airfist         **************/
/***************************************************/

#include "g_local.h"
#include "l_airfist.h"
#include "l_angels.h"

/*
=================
mace_horn_findradius

 Returns entities that have origins within a spherical area

findradius (origin, radius)
=================
*/
static edict_t* mace_horn_findradius(edict_t* from, vec3_t org, float rad)
{
	vec3_t	eorg = { 0 };
	int		j;

	if (!from)
		from = g_edicts;
	else
		from++;
	for (; from < &g_edicts[globals.num_edicts]; from++)
	{
		if (!from->inuse)
			continue;
		for (j = 0; j < 3; j++)
			eorg[j] = org[j] - (from->s.origin[j] + (from->mins[j] + from->maxs[j]) * 0.5f);
		if (VectorLength(eorg) > rad)
			continue;

		if (from->item)	// don't push power-ups and weps
			continue;

		return from;
	}
	return NULL;
}

qboolean mace_horn_infront(edict_t* self, edict_t* other)
{
	vec3_t	vec = { 0 }, forward;
	float	dot;

	AngleVectors(self->s.angles, forward, NULL, NULL);
	VectorSubtract(other->s.origin, self->s.origin, vec);
	VectorNormalize(vec);
	dot = DotProduct(vec, forward);

	if (dot > 0.8)
		return true;
	return false;
}

void fire_mace(edict_t* self, vec3_t start, vec3_t aimdir, int inDamage, int strength, float inRange, float recoil)
{
	vec3_t	end, delta = { 0 }, forward, right, up;
	float percent, distance;
	float max_mass = 800; //max mass that can be moved
	float actual_mass;
	edict_t* ent;
	float eSpeed;
	int ldmg;
	int num_traces;

	AngleVectors(aimdir, forward, right, up);

	ent = NULL;
	while ((ent = mace_horn_findradius(ent, start, inRange)) != NULL)
	{

		if (visible(self, ent) &&
			horn_infront(self, ent) &&
			ent != self)
		{
			if ((ent->svflags & SVF_DEADMONSTER) && (ent->movetype == MOVETYPE_NONE))
			{
				ent->movetype = MOVETYPE_TOSS;
				ent->s.origin[2] += 10;
			}

			if (ent->movetype != MOVETYPE_NONE &&
				ent->movetype != MOVETYPE_NOCLIP &&
				ent->movetype != MOVETYPE_PUSH &&
				ent->movetype != MOVETYPE_STOP &&
				ent->classnum != CN_BOLT &&
				strcmp(ent->classname, "item_flag_team1") &&
				strcmp(ent->classname, "item_flag_team2")
				)
			{

				VectorSubtract(ent->s.origin, start, delta);
				distance = VectorLength(delta);

				percent = (inRange - distance) / inRange;


				if (ent->groundentity)
				{
					ent->groundentity = NULL;
					ent->s.origin[2]++;

					if (delta[2] < 0)
						delta[2] = delta[2] / -2;
					VectorScale(delta, 0.7f, delta);
					if (delta[2] < 100)
						delta[2] = 100;
				}

				VectorNormalize(delta);

				if (!ent->mass) actual_mass = 100;
				else actual_mass = ent->mass;

				VectorScale(delta, percent * strength * (max_mass / actual_mass), delta);

				if (ent->movetype == MOVETYPE_FLYMISSILE)
				{

					eSpeed = VectorLength(ent->velocity);
					VectorNormalize(delta);
					VectorScale(delta, eSpeed, ent->velocity);
				}
				else
				{

					VectorMA(ent->velocity, 1, delta, ent->velocity);

					//do some damage! :P

					if (ent->takedamage &&
						ent->classnum == CN_BARREL &&
						(!(ent->flags & FL_FROZEN))
						)
					{
						// calculate the damage amount
						ldmg = (int)percent * inDamage;

						T_Damage(ent, self, self, vec3_origin, ent->s.origin, vec3_origin, ldmg, 0, DAMAGE_NO_KNOCKBACK, MOD_AIRFIST);
						if (ent->client != NULL)
						{
							ent->client->alpha = 1.0;
							ent->client->alphatime = 100;
						}
					}
				}
			}
		}
	}

	Weapon_Recoil(self, recoil);

	//effect!
	gi.sound(self, CHAN_RELIABLE + CHAN_WEAPON, gi.soundindex("chick/chkatck2.wav"), 1, ATTN_NORM, 0);

	num_traces = 0;

	while (num_traces < 45)
	{
		VectorMA(start, 170, forward, end);
		VectorMA(end, 35 * crandom(), right, end);
		VectorMA(end, 35 * crandom(), up, end);

		gi.WriteByte(svc_temp_entity);
		gi.WriteByte(TE_BUBBLETRAIL);
		gi.WritePosition(start);
		gi.WritePosition(end);
		gi.multicast(start, MULTICAST_PHS);
		num_traces++;
	}
}

void weapon_mace_fire(edict_t* ent)
{
	vec3_t	start, up, forward, right, offset = { 0 };
	int		damage = 2;
	int		kick = 8;

	if (!ent || !ent->client)
		return;

	if (ent->client->ps.gunframe == 12)
	{
		if (ent->client->buttons & BUTTON_ATTACK)
			ent->client->ps.gunframe = 6;
		else
		{
			gi.sound(ent, CHAN_WEAPON, gi.soundindex("weapons/noammo.wav"), 1, ATTN_NORM, 0);
			ent->client->machinegun_shots = 0;
			ent->client->ps.gunframe++;
		}
		return;
	}

	AngleVectors(ent->client->v_angle, forward, right, up);

	VectorScale(forward, -2, ent->client->kick_origin);
	ent->client->kick_angles[0] = -2;

	VectorSet(offset, 0, 8, ent->viewheight - 8.0f);
	P_ProjectSource(ent->client, ent->s.origin, offset, forward, right, start);

	if (is_quad)
	{
		damage *= 4;
		kick *= 4;
	}


	if (ent->client->machinegun_shots >= 5)
	{
		gi.sound(ent, CHAN_VOICE, gi.soundindex("gunner/gunatck3.wav"), 1, ATTN_NORM, 0);
	}
	else
	{
		if (deathmatch->value)
		{
			fire_mace(ent, start, ent->client->v_angle, damage, 150, 200, 200);
			if (ent->client && ent->client->pers.special == AODEATH)
			{
				fire_mace(ent, ent->client->angel->s.origin, ent->client->v_angle, damage, 150, 200, 200);
			}
		}
		else
		{
			fire_mace(ent, start, ent->client->v_angle, damage / 2, 150, 200, 200);
			if (ent->client && ent->client->pers.special == AODEATH)
			{
				fire_mace(ent, ent->client->angel->s.origin, ent->client->v_angle, damage / 2, 150, 200, 200);
			}
		}
	}

	//  ent->client->machinegun_shots++;
	ent->client->ps.gunframe++;
	PlayerNoise(ent, start, PNOISE_WEAPON);

}

void Weapon_Mace(edict_t* ent)
{
	static int	pause_frames[] = { 25, 33, 42, 50, 0 };
	static int	fire_frames[] = { 5, 0 };

	Weapon_Generic(ent, 4, 8, 52, 55, pause_frames, fire_frames, weapon_mace_fire);
}


