// g_combat.c
//
#include "g_local.h"

// teamplay definitions
#include "g_team.h"

/*
============
CanDamage

Returns true if the inflictor can directly damage the target.  Used for
explosions and melee attacks.
============
*/
qboolean CanDamage(edict_t* targ, edict_t* inflictor)
{
	vec3_t	dest = { 0 };
	trace_t	trace;

	// bmodels need special checking because their origin is 0,0,0
	if (targ->movetype == MOVETYPE_PUSH)
	{
		VectorAdd(targ->absmin, targ->absmax, dest);
		VectorScale(dest, 0.5, dest);
		trace = gi.trace(inflictor->s.origin, vec3_origin, vec3_origin, dest, inflictor, MASK_SOLID);
		if (trace.fraction == 1.0)
			return true;
		if (trace.ent == targ)
			return true;
		return false;
	}

	trace = gi.trace(inflictor->s.origin, vec3_origin, vec3_origin, targ->s.origin, inflictor, MASK_SOLID);
	if (trace.fraction == 1.0)
		return true;

	VectorCopy(targ->s.origin, dest);
	dest[0] += 15.0;
	dest[1] += 15.0;
	trace = gi.trace(inflictor->s.origin, vec3_origin, vec3_origin, dest, inflictor, MASK_SOLID);
	if (trace.fraction == 1.0)
		return true;

	VectorCopy(targ->s.origin, dest);
	dest[0] += 15.0;
	dest[1] -= 15.0;
	trace = gi.trace(inflictor->s.origin, vec3_origin, vec3_origin, dest, inflictor, MASK_SOLID);
	if (trace.fraction == 1.0)
		return true;

	VectorCopy(targ->s.origin, dest);
	dest[0] -= 15.0;
	dest[1] += 15.0;
	trace = gi.trace(inflictor->s.origin, vec3_origin, vec3_origin, dest, inflictor, MASK_SOLID);
	if (trace.fraction == 1.0)
		return true;

	VectorCopy(targ->s.origin, dest);
	dest[0] -= 15.0;
	dest[1] -= 15.0;
	trace = gi.trace(inflictor->s.origin, vec3_origin, vec3_origin, dest, inflictor, MASK_SOLID);
	if (trace.fraction == 1.0)
		return true;


	return false;
}


/*
============
Killed
============
*/
void Killed(edict_t* targ, edict_t* inflictor, edict_t* attacker, int damage, vec3_t point)
{
	// Was this the Chamber which just got killed? 
	if (Q_stricmp(targ->classname, "Chamber") == 0)
	{
		// Flag to detonate on next frame. 
		targ->owner->client->chamber = 0;
		//	Chamber_Explode(targ->owner->client->thechamber);
		// Give the attacker 5 frags for killing the Chamber.. 
		if (G_EntExists(attacker) && (attacker != targ->owner))
		{
			gi.centerprintf(attacker, "5 Frags for destroying Chamber!\n");
			attacker->client->resp.score += 5;
		}
		return;
	}

	// Was this the Baton which just got killed? 
	if (Q_stricmp(targ->classname, "Baton") == 0)
	{
		// Flag Baton_Think() to detonate on next frame. 
		targ->owner->client->baton = 0;
		// Give the attacker 2 frags for killing the Baton.. 
		if (G_EntExists(attacker) && (attacker != targ->owner))
		{
			gi.centerprintf(attacker, "2 Frags for destroying Baton!\n");
			attacker->client->resp.score += 2;
		}
		return;
	}

	if (targ->health < -999)
		targ->health = -999;

	targ->enemy = attacker;

	if ((targ->svflags & SVF_MONSTER) && (targ->deadflag != DEAD_DEAD))
	{
		//		targ->svflags |= SVF_DEADMONSTER;	// now treat as a different content type
		if (!(targ->monsterinfo.aiflags & AI_GOOD_GUY))
		{
			level.killed_monsters++;
			if (coop->value && attacker->client)
				attacker->client->resp.score++;

			// medics won't heal monsters that they kill themselves
			// attacker->classname must not be NULL
			if (attacker->classname && strcmp(attacker->classname, "monster_medic") == 0)
				targ->owner = attacker;
		}
	}

	if (targ->movetype == MOVETYPE_PUSH || targ->movetype == MOVETYPE_STOP || targ->movetype == MOVETYPE_NONE)
	{	// doors, triggers, etc
		targ->die(targ, inflictor, attacker, damage, point);
		return;
	}

	if ((targ->svflags & SVF_MONSTER) && (targ->deadflag != DEAD_DEAD))
	{
		targ->touch = NULL;
		monster_death_use(targ);
	}

	targ->die(targ, inflictor, attacker, damage, point);
}


/*
================
SpawnDamage
================
*/
void SpawnDamage(int type, vec3_t origin, vec3_t normal, int damage)
{
	if (damage > 255)
		damage = 255;
	gi.WriteByte(svc_temp_entity);
	gi.WriteByte(type);
	//	gi.WriteByte (damage);
	gi.WritePosition(origin);
	gi.WriteDir(normal);
	gi.multicast(origin, MULTICAST_PVS);
}


/*
============
T_Damage

targ		entity that is being damaged
inflictor	entity that is causing the damage
attacker	entity that caused the inflictor to damage targ
	example: targ=monster, inflictor=rocket, attacker=player

dir			direction of the attack
point		point at which the damage is being inflicted
normal		normal vector from that point
damage		amount of damage being inflicted
knockback	force to be applied against targ as a result of the damage

dflags		these flags are used to control how T_Damage works
	DAMAGE_RADIUS			damage was indirect (from a nearby explosion)
	DAMAGE_NO_ARMOR			armor does not protect from this damage
	DAMAGE_ENERGY			damage is from an energy based weapon
	DAMAGE_NO_KNOCKBACK		do not affect velocity, just view angles
	DAMAGE_BULLET			damage is from a bullet (used for ricochets)
	DAMAGE_NO_PROTECTION	kills godmode, armor, everything
============
*/
static int CheckPowerArmor(edict_t* ent, vec3_t point, vec3_t normal, int damage, int dflags)
{
	gclient_t* client;
	int			save;
	int			power_armor_type;
	int			index;
	int			damagePerCell;
	int			pa_te_type;
	int			power;
	int			power_used;
	vec3_t		vec = { 0 };
	vec3_t		forward;
	float		dot;

	if (!damage) return 0;

	client = ent->client;

	if (dflags & DAMAGE_NO_ARMOR) return 0;

	index = power = 0;

	if (client)
	{
		power_armor_type = PowerArmorType(ent);
		if (power_armor_type != POWER_ARMOR_NONE)
		{
			index = ITEM_INDEX(&gI_ammo_cells);
			power = client->pers.inventory[index];
		}
	}
	else if (ent->svflags & SVF_MONSTER)
	{
		power_armor_type = ent->monsterinfo.power_armor_type;
		power = ent->monsterinfo.power_armor_power;
	}
	else
		return 0;

	if (power_armor_type == POWER_ARMOR_NONE) return 0;
	if (!power) return 0;

	if (power_armor_type == POWER_ARMOR_SCREEN)
	{
		// only works if damage point is in front
		AngleVectors(ent->s.angles, forward, NULL, NULL);
		VectorSubtract(point, ent->s.origin, vec);
		VectorNormalize(vec);
		dot = DotProduct(vec, forward);
		if (dot <= 0.3)	return 0;

		damagePerCell = 1;
		pa_te_type = TE_SCREEN_SPARKS;
		damage = damage / 3;
	}
	else
	{
		damagePerCell = 1; // power armor is weaker in CTF
		pa_te_type = TE_SHIELD_SPARKS;
		damage = (2 * damage) / 3;
	}

	save = power * damagePerCell;
	if (save > damage)
		save = damage;

	SpawnDamage(pa_te_type, point, normal, save);
	ent->powerarmor_time = level.time + 0.2f;

	power_used = save / damagePerCell;

	if (client)
		client->pers.inventory[index] -= power_used;
	else
		ent->monsterinfo.power_armor_power -= power_used;
	return save;
}

static int CheckArmor(edict_t* ent, vec3_t point, vec3_t normal, int damage, int te_sparks, int dflags)
{
	gclient_t* client;
	int			save;
	int			index;
	gitem_t* armor;

	if (!damage)
		return 0;

	client = ent->client;

	if (!client)
		return 0;

	if (dflags & DAMAGE_NO_ARMOR)
		return 0;

	index = ArmorIndex(ent);
	if (!index)
		return 0;

	armor = GetItemByIndex(index);

	if (dflags & DAMAGE_ENERGY)
		save = (int)ceilf(((gitem_armor_t*)armor->info)->energy_protection * damage);
	else
		save = (int)ceilf(((gitem_armor_t*)armor->info)->normal_protection * damage);
	if (save >= client->pers.inventory[index])
		save = client->pers.inventory[index];

	if (!save)
		return 0;

	client->pers.inventory[index] -= save;
	SpawnDamage(te_sparks, point, normal, save);

	return save;
}

void M_ReactToDamage(edict_t* targ, edict_t* attacker)
{
	if (!(attacker->client) && !(attacker->svflags & SVF_MONSTER))
		return;

	if (attacker == targ || attacker == targ->enemy)
		return;

	// dead monsters, like misc_deadsoldier, don't have AI functions, but 
	// M_ReactToDamage might still be called on them
	if (targ->svflags & SVF_DEADMONSTER)
		return;

	// if we are a good guy monster and our attacker is a player
	// or another good guy, do not get mad at them
	if (targ->monsterinfo.aiflags & AI_GOOD_GUY)
	{
		if (attacker->client || (attacker->monsterinfo.aiflags & AI_GOOD_GUY))
			return;
	}

	// we now know that we are not both good guys

	// if attacker is a client, get mad at them because he's good and we're not
	if (attacker->client)
	{
		targ->monsterinfo.aiflags &= ~AI_SOUND_TARGET;

		// this can only happen in coop (both new and old enemies are clients)
		// only switch if can't see the current enemy
		if (targ->enemy && targ->enemy->client)
		{
			if (visible(targ, targ->enemy))
			{
				targ->oldenemy = attacker;
				return;
			}
			targ->oldenemy = targ->enemy;
		}
		targ->enemy = attacker;
		if (!(targ->monsterinfo.aiflags & AI_DUCKED))
			FoundTarget(targ);
		return;
	}

	// it's the same base (walk/swim/fly) type and a different classname and it's not a tank
	// (they spray too much), get mad at them
	if (((targ->flags & (FL_FLY | FL_SWIM)) == (attacker->flags & (FL_FLY | FL_SWIM))) &&
		(strcmp(targ->classname, attacker->classname) != 0) &&
		(strcmp(attacker->classname, "monster_tank") != 0) &&
		(strcmp(attacker->classname, "monster_supertank") != 0) &&
		(strcmp(attacker->classname, "monster_makron") != 0) &&
		(strcmp(attacker->classname, "monster_jorg") != 0))
	{
		if (targ->enemy && targ->enemy->client)
			targ->oldenemy = targ->enemy;
		targ->enemy = attacker;
		if (!(targ->monsterinfo.aiflags & AI_DUCKED))
			FoundTarget(targ);
	}
	// if they *meant* to shoot us, then shoot back
	else if (attacker->enemy == targ)
	{
		if (targ->enemy && targ->enemy->client)
			targ->oldenemy = targ->enemy;
		targ->enemy = attacker;
		if (!(targ->monsterinfo.aiflags & AI_DUCKED))
			FoundTarget(targ);
	}
	// otherwise get mad at whoever they are mad at (help our buddy) unless it is us!
	else if (attacker->enemy && attacker->enemy != targ)
	{
		if (targ->enemy && targ->enemy->client)
			targ->oldenemy = targ->enemy;
		targ->enemy = attacker->enemy;
		if (!(targ->monsterinfo.aiflags & AI_DUCKED))
			FoundTarget(targ);
	}
}

qboolean CheckTeamDamage(edict_t* targ, edict_t* attacker)
{
	//ZOID
	if (ctf->value && targ->client && attacker->client)
		if (targ->client->resp.ctf_team == attacker->client->resp.ctf_team &&
			targ != attacker)
			return true;
	//ZOID

	return false;
}

//************************************
//      Reflect Spawnkill Damage
//************************************

void ReflectDamage(edict_t* targ, edict_t* inflictor, edict_t* attacker, vec3_t dir,
	vec3_t point, vec3_t normal, int damage, int knockback,
	int dflags, int mod)
{
	//QW// this will put damage on attacker for hitting a player in respawn.
	/* The intent is for damage reflection if player is firing on new-spawner
		and for new spawner to pay a price if he comes out of spawn firing
		like a maniac under protection too. The price for shooting a spawner
		should probably be higher than being a spawning shooter.
		*/
}

// Compute the effect of the damage on the players.
void T_Damage(edict_t* targ, edict_t* inflictor, edict_t* attacker, vec3_t dir,
	vec3_t point, vec3_t normal, int damage, int knockback,
	int dflags, int mod)
{
	gclient_t* client;
	int			take;
	int			save;
	int			asave;
	int			psave;
	int			te_sparks;

	if (!targ->inuse || !targ->takedamage)
		return;

	if (Q_stricmp(targ->classname, "Tracker") == 0)
	{
		targ->health = targ->health - damage;
		if (targ->health <= 0)
			targ->trackerstarttime = level.time;
		return;
	}

	if (Q_stricmp(targ->classname, "Turret") == 0)
	{
		targ->health = targ->health - damage;
		if (targ->health <= 0)
			targ->turretstarttime = level.time;
		return;
	}

	if (!targ->takedamage)
		return;


	if (targ->client != NULL && targ->client->slugarmor == 1)
	{
		if (attacker->client != NULL
			&& attacker->client->pers.weapon != NULL
			&& attacker->client->pers.weapon->ammo != NULL
			&& Q_stricmp(attacker->client->pers.weapon->ammo->classname, "ammo_slugs") == 0)
		{
			SpawnDamage(TE_SHIELD_SPARKS, point, normal, damage);
			targ->powerarmor_time = level.time + 0.2f;
			return;
		}
	}

	if (targ->client != NULL && targ->client->cellarmor == 1)
	{
		if (attacker->client != NULL
			&& attacker->client->pers.weapon != NULL
			&& attacker->client->pers.weapon->ammo != NULL
			&& Q_stricmp(attacker->client->pers.weapon->ammo->classname, "ammo_cells") == 0)
		{
			SpawnDamage(TE_SHIELD_SPARKS, point, normal, damage);
			targ->powerarmor_time = level.time + 0.2f;
			return;
		}
	}

	if (targ->client != NULL && targ->client->shellarmor == 1)
	{
		if (attacker->client != NULL
			&& attacker->client->pers.weapon != NULL
			&& attacker->client->pers.weapon->ammo != NULL
			&& Q_stricmp(attacker->client->pers.weapon->ammo->classname, "ammo_shells") == 0)
		{
			SpawnDamage(TE_SHIELD_SPARKS, point, normal, damage);
			targ->powerarmor_time = level.time + 0.2f;
			return;
		}
	}

	if (targ->client != NULL && targ->client->bulletarmor == 1)
	{
		if (attacker->client != NULL
			&& attacker->client->pers.weapon != NULL
			&& attacker->client->pers.weapon->ammo != NULL
			&& Q_stricmp(attacker->client->pers.weapon->ammo->classname, "ammo_bullets") == 0)
		{
			SpawnDamage(TE_SHIELD_SPARKS, point, normal, damage);
			targ->powerarmor_time = level.time + 0.2f;
			return;
		}
	}

	if (targ->client != NULL && targ->client->rocketarmor == 1)
	{
		if (attacker->client != NULL
			&& attacker->client->pers.weapon != NULL
			&& attacker->client->pers.weapon->ammo != NULL
			&& Q_stricmp(attacker->client->pers.weapon->ammo->classname, "ammo_rockets") == 0)
		{
			SpawnDamage(TE_SHIELD_SPARKS, point, normal, damage);
			targ->powerarmor_time = level.time + 0.2f;
			return;
		}
	}

	if (targ->client != NULL && targ->client->grenadearmor == 1)
	{
		if (attacker->client != NULL
			&& attacker->client->pers.weapon != NULL
			&& attacker->client->pers.weapon->ammo != NULL
			&& Q_stricmp(attacker->client->pers.weapon->ammo->classname, "ammo_grenades") == 0)
		{
			SpawnDamage(TE_SHIELD_SPARKS, point, normal, damage);
			targ->powerarmor_time = level.time + 0.2f;
			return;
		}
	}

	if (attacker->client != NULL && attacker->client->vampihealth == 1)
	{
		if (attacker->health < 1000)
			attacker->health = attacker->health + (int)(damage * 0.40);
		else
			attacker->health = 1000;
	}

	// friendly fire avoidance
	// if enabled you can't hurt teammates (but you can hurt yourself)
	// knockback still occurs
	if (targ != attacker)
	{
		if (coop->value
			|| (deathmatch->value
				&& (((int)(dmflags->value) & (DF_MODELTEAMS | DF_SKINTEAMS))
					|| ctf->value)))
		{
			if (OnSameTeam(targ, attacker))
			{
				if ((int)(dmflags->value) & DF_NO_FRIENDLY_FIRE)
					damage = 0;
				else
					mod |= MOD_FRIENDLY_FIRE;
			}
		}
	}
	meansOfDeath = mod;

	// easy mode takes half damage
	if (skill->value == 0 && deathmatch->value == 0 && targ->client)
	{
		damage /= 2;
		if (!damage)
			damage = 1;
	}

	client = targ->client;

	if (dflags & DAMAGE_BULLET)
		te_sparks = TE_BULLET_SPARKS;
	else
		te_sparks = TE_SPARKS;

	VectorNormalize(dir);

	// bonus damage for surprising a monster
	if (!(dflags & DAMAGE_RADIUS) && (targ->svflags & SVF_MONSTER) && (attacker->client) && (!targ->enemy) && (targ->health > 0))
		damage *= 2;

	if (targ->flags & FL_NO_KNOCKBACK)
		knockback = 0;

	// figure momentum add
	if (!(dflags & DAMAGE_NO_KNOCKBACK))
	{
		if ((knockback) && (targ->movetype != MOVETYPE_NONE) && (targ->movetype != MOVETYPE_BOUNCE) && (targ->movetype != MOVETYPE_PUSH) && (targ->movetype != MOVETYPE_STOP))
		{
			vec3_t	kvel;
			float	mass;

			if (targ->mass < 50.0)
				mass = 50.0;
			else
				mass = targ->mass;

			if (targ->client && attacker == targ)
				VectorScale(dir, 1600.0f * knockback / mass, kvel);	// the rocket jump hack...
			else
				VectorScale(dir, 500.0f * knockback / mass, kvel);

			VectorAdd(targ->velocity, kvel, targ->velocity);
		}
	}

	take = damage;
	save = 0;

	// check for godmode
	if ((targ->flags & FL_GODMODE) && !(dflags & DAMAGE_NO_PROTECTION))
	{
		take = 0;
		save = damage;
		SpawnDamage(te_sparks, point, normal, save);
	}

	// check for invincibility
	if ((client && client->invincible_framenum > level.framenum) && !(dflags & DAMAGE_NO_PROTECTION))
	{
		if (targ->pain_debounce_time < level.time)
		{
			gi.sound(targ, CHAN_ITEM, gi.soundindex("items/protect4.wav"), 1, ATTN_NORM, 0);
			targ->pain_debounce_time = level.time + 2;
		}
		take = 0; // target player is invincible
		save = damage;

		//QW// Player is spawning and we reflect the damage back to shooter
		ReflectDamage(targ, inflictor, attacker, dir,
			point, normal, damage, knockback,
			dflags, mod);
	}

	//team armor protect
	if (ctf->value && targ->client && attacker->client &&
		targ->client->resp.ctf_team == attacker->client->resp.ctf_team &&
		targ != attacker && ((int)dmflags->value & DF_ARMOR_PROTECT))
	{
		psave = asave = 0;
	}
	else
	{
		psave = CheckPowerArmor(targ, point, normal, take, dflags);
		take -= psave;

		asave = CheckArmor(targ, point, normal, take, te_sparks, dflags);
		take -= asave;
	}

	//treat cheat/powerup savings the same as armor
	asave += save;

	// team damage avoidance
	if (!(dflags & DAMAGE_NO_PROTECTION) && CheckTeamDamage(targ, attacker))
		return;

	// do the damage
	if (take)
	{
		if ((targ->svflags & SVF_MONSTER) || (client))
			SpawnDamage(TE_BLOOD, point, normal, take);
		else
			SpawnDamage(te_sparks, point, normal, take);


		targ->health = targ->health - take;

		if (targ->health <= 0)
		{
			if ((targ->svflags & SVF_MONSTER) || (client))
				targ->flags |= FL_NO_KNOCKBACK;
			Killed(targ, inflictor, attacker, take, point);
			return;
		}
	}

	if (targ->svflags & SVF_MONSTER)
	{
		M_ReactToDamage(targ, attacker);
		if (!(targ->monsterinfo.aiflags & AI_DUCKED) && (take))
		{
			targ->pain(targ, attacker, (float)knockback, take);
			// nightmare mode monsters don't go into pain frames often
			if (skill->value == 3)
				targ->pain_debounce_time = level.time + 5;
		}
	}
	else if (client)
	{
		if (!(targ->flags & FL_GODMODE) && (take))
			targ->pain(targ, attacker, (float)knockback, take);
	}
	else if (take)
	{
		if (targ->pain)
			targ->pain(targ, attacker, (float)knockback, take);
	}

	// add to the damage inflicted on a player this frame
	// the total will be turned into screen blends and view angle kicks
	// at the end of the frame
	if (client)
	{
		client->damage_parmor += psave;
		client->damage_armor += asave;
		client->damage_blood += take;
		client->damage_knockback += knockback;
		VectorCopy(point, client->damage_from);
	}
}


/*
============
T_RadiusDamage
============
*/
void T_RadiusDamage(edict_t* inflictor, edict_t* attacker, float damage,
	edict_t* ignore, float radius, int mod)
{
	float	points;
	edict_t* ent = NULL;
	vec3_t	v = { 0 };
	vec3_t	dir = { 0 };

	while ((ent = findradius(ent, inflictor->s.origin, radius)) != NULL)
	{
		if (ent == ignore)
			continue;
		if (!ent->takedamage)
			continue;

		VectorAdd(ent->mins, ent->maxs, v);
		VectorMA(ent->s.origin, 0.5, v, v);
		VectorSubtract(inflictor->s.origin, v, v);
		points = damage - 0.5f * VectorLength(v);
		if (ent == attacker)
			points = points * 0.5f;
		if (points > 0)
		{
			if (CanDamage(ent, inflictor))
			{
				VectorSubtract(ent->s.origin, inflictor->s.origin, dir);
				T_Damage(ent, inflictor, attacker, dir, inflictor->s.origin, vec3_origin, (int)points, (int)points, DAMAGE_RADIUS, mod);
				if (ent->client != NULL && ent->dmg == 12)
				{
					ent->client->alpha = 1.0;
					ent->client->alphatime = 100;
				}
			}
		}
	}
}
/*
============
T_FreezatronRadiusDamage
============
*/
void T_FreezatronRadiusDamage(edict_t* inflictor, edict_t* attacker, float damage,
	edict_t* ignore, float radius, int mod)
{
	float	points;
	edict_t* ent = NULL;
	vec3_t	v = { 0 };
	vec3_t	dir = { 0 };

	while ((ent = findradius(ent, inflictor->s.origin, radius)) != NULL)
	{
		if (ent == ignore)
			continue;
		if (!ent->takedamage)
			continue;

		VectorAdd(ent->mins, ent->maxs, v);
		VectorMA(ent->s.origin, 0.5, v, v);
		VectorSubtract(inflictor->s.origin, v, v);
		points = damage - 0.5f * VectorLength(v);
		if (ent == attacker)
			points = points * 0.5f;
		if (points > 0)
		{
			if (CanDamage(ent, inflictor))
			{
				VectorSubtract(ent->s.origin, inflictor->s.origin, dir);
				T_Damage(ent, inflictor, attacker, dir, inflictor->s.origin, vec3_origin, (int)points, (int)points, DAMAGE_RADIUS, mod);
				if (ent->client != NULL && ent->dmg == 12)
				{
					ent->client->alpha = 1.0;
					ent->client->alphatime = 100;
				}
			}
		}
		// Freeze them no matter if they took damager or not
		if (!ent->deadflag && damage > 0)
		{
			// freeze them for this long.
			if (ent->frozen && (ent->frozentime - level.time) <= 22)
				ent->frozentime += 8.0;
			else
				ent->frozentime = level.time + 4.0f;

			// Freeze them.
			ent->frozen = 1;
		}
	}
}
//Wyrm: used for plasma gun and water discharges
/*
============
T_WaterRadiusDamage
============
*/
void T_WaterRadiusDamage(edict_t* inflictor, edict_t* attacker, float damage, edict_t* ignore, float radius, int mod)
{
	float	points;
	edict_t* ent = NULL;
	vec3_t	v = { 0 };
	vec3_t	dir = { 0 };

	while ((ent = findradius(ent, inflictor->s.origin, radius)) != NULL)
	{
		if (ent == ignore)
			continue;
		if (!ent->takedamage)
			continue;

		//Enviro suit protects against discharges!
		if (ent->client)
			if (ent->client->enviro_framenum > level.framenum)
				continue;

		VectorAdd(ent->mins, ent->maxs, v);
		VectorMA(ent->s.origin, 0.5, v, v);
		VectorSubtract(inflictor->s.origin, v, v);
		points = damage - 0.5f * VectorLength(v);
		if (ent == attacker)
			points = points * 0.5f;
		if (points > 0)
			if (ent->waterlevel)
			{
				if (CanDamage(ent, inflictor))
				{
					VectorSubtract(ent->s.origin, inflictor->s.origin, dir);
					T_Damage(ent, inflictor, attacker, dir, inflictor->s.origin, vec3_origin, (int)points, (int)points, DAMAGE_RADIUS, mod);
				}
			}
	}
}
