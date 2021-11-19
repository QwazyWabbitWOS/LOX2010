
//======================================================================
//  Vacuum Maker
//======================================================================

#include "g_local.h"
#include "l_vacuummaker.h"
#include "l_airfist.h"

void fire_vacuummaker(edict_t* self, vec3_t start, vec3_t dir, int damage, int speed, float damage_radius)
{
	vec3_t delta = { 0 };
	float percent, distance;
	float max_mass = VAC_MAX_MASS; //max mass that can be moved
	float actual_mass;
	edict_t* ent;
	int strength = VAC_STRENGTH;
	float inRange = VAC_INRANGE;

	//pull stuff
	ent = NULL;
	while ((ent = horn_findradius(ent, start, inRange)) != NULL)
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
				if (distance <= 48)
				{
					if (ent->takedamage)
						T_Damage(ent, self, self, self->velocity, start, dir, damage, 0, 0, MOD_VACUUM);
					if (ent->touch)
						ent->touch(ent, self, NULL, NULL);
				}

				percent = (inRange - distance) / inRange;


				if (ent->groundentity)
				{
					ent->groundentity = NULL;
					ent->s.origin[2]++;
					if (!ent->client)
					{
						if (delta[2] > 0)
							delta[2] = delta[2] / -2;
						VectorScale(delta, 0.7f, delta);
						if (delta[2] > -80)
							delta[2] = -80;
					}
				}

				VectorNormalize(delta);

				if (ent->mass < 100.0f) actual_mass = 100.0f;
				else actual_mass = ent->mass;

				VectorScale(delta, percent * strength * (max_mass / actual_mass), delta);

				VectorMA(ent->velocity, -1, delta, ent->velocity);
			}
		}
	}

	gi.WriteByte(svc_temp_entity);
	gi.WriteByte(TE_SPARKS);
	gi.WritePosition(start);
	gi.WriteDir(dir);
	gi.multicast(start, MULTICAST_PVS);
}

void weapon_vacuummaker_fire(edict_t* ent)
{
	vec3_t	offset = { 0 }, start, forward, right, up, dir;
	int		damage;
	float	damage_radius = 1200;

	if (deathmatch->value)
		damage = VAC_DAMAGE;
	else
		damage = 2 * VAC_DAMAGE;

	if ((ent->client->ps.gunframe == 11) ||
		((!ent->client->pers.inventory[ent->client->ammo_index]) && (ent->client->ammo_index))
		)
	{
		ent->client->ps.gunframe = 32;
		return;
	}

	if (ent->client->ps.gunframe == 9)
	{
		if (!ent->client->silencer_shots)
			gi.sound(ent, CHAN_VOICE, gi.soundindex("vacuum.wav"), 1, ATTN_NORM, 0);
		else
			gi.sound(ent, CHAN_VOICE, gi.soundindex("vacuum.wav"), 0.2f, ATTN_NORM, 0);
	}

	if (is_quad)
		damage *= 4;

	AngleVectors(ent->client->v_angle, forward, right, up);

	VectorScale(forward, -2, ent->client->kick_origin);

	// make a big pitch kick with an inverse fall
	/*
	ent->client->v_dmg_pitch = -40;
	ent->client->v_dmg_roll = crandom()*8;
	ent->client->v_dmg_time = level.time + DAMAGE_TIME;
	*/

	VectorSet(offset, 8, 8, ent->viewheight - 8.0f);
	P_ProjectSource(ent->client, ent->s.origin, offset, forward, right, start);


	VectorMA(up, 4, forward, dir);
	VectorNormalize(dir);

	fire_vacuummaker(ent, start, dir, damage, VAC_SPEED, damage_radius);

	ent->client->ps.gunframe++;

	PlayerNoise(ent, start, PNOISE_WEAPON);

	if (ent->client->ps.gunframe == 10)
		if ((!((int)dmflags->value & DF_INFINITE_AMMO))
			&& (ent->client->ammo_index))
			ent->client->pers.inventory[ent->client->ammo_index] -= 1;

	if ((ent->client->ps.gunframe == 11) &&
		((ent->client->pers.inventory[ent->client->ammo_index]) ||
			(!ent->client->ammo_index))
		&& (ent->client->buttons & BUTTON_ATTACK))
		ent->client->ps.gunframe = 9;
}

void Weapon_VacuumMaker(edict_t* ent)
{
	static int	pause_frames[] = { 39, 45, 50, 55, 0 };
	static int	fire_frames[] = { 9, 10, 11, 0 };

	Weapon_Generic(ent, 8, 32, 55, 58, pause_frames, fire_frames, weapon_vacuummaker_fire);
}
