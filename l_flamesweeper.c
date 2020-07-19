#include "g_local.h"
#include "l_angels.h"
#include "x_fire.h"

#define FRAME_attack1         	46
#define FRAME_attack8         	53
#define FRAME_crattak1        	160
#define FRAME_crattak9        	168

/*------------------------------------------------------/ New Code /--------
//  This adjusts a vector's angles with the spread given.
//
//  NOTE:  'spread' vector fields --
//         [0] = PITCH = Vertical spread.
//         [1] = YAW   = Horizontal spread.
//         [2] = ROLL  = No effect (should always be 0).
//------------------------------------------------------------------------*/
void PBM_FireAngleSpread (vec3_t spread, vec3_t dir)
{
	vec3_t  v;
	
	vectoangles (dir, v);
	
	v[PITCH] += (random() - 0.5f) * spread[PITCH];
	v[YAW]   += (random() - 0.5f) * spread[YAW];
	v[ROLL]  += (random() - 0.5f) * spread[ROLL];
	
	AngleVectors (v, dir, NULL, NULL);
	VectorNormalize (dir);
}

/*------------------------------------------------------/ New Code /--------
//  The fat fireball impacts and burns stuff up.
//------------------------------------------------------------------------*/
void PBM_FlameThrowerTouch(edict_t *self, edict_t *other, cplane_t *plane, csurface_t *surf)
{
	if (surf && (surf->flags & SURF_SKY))
	{       G_FreeEdict (self);
	return;
	}
	
	if (PBM_InWater(self))
	{       PBM_BecomeSteam(self);
	return;
	}
	
	/* Burn! */
	PBM_BurnRadius (self, self->dmg_radius, self->pos2, other);
	
	/* Calculate position for the explosion entity. */
	VectorNormalize (self->velocity);
	VectorMA (self->s.origin, -8, self->velocity, self->s.origin);
	gi.linkentity (self);
	
	if (other->takedamage)
	{
		if (other->flags & FL_IMMUNE_LAVA)
		{	vec3_t	reduced = {0, 0, 0};
		reduced[1] = self->pos1[1];
		LPBM_BurnDamage (other, self, self->s.origin, reduced, 0, MOD_RAILGUN);
		}
		else
			LPBM_BurnDamage (other, self, self->s.origin, self->pos1, 0, MOD_RAILGUN);
	}
	else
	{       if ((rand() % 100) < self->count)
	PBM_EasyFireDrop (self);
	}
	
	/* Stop the flame. */
	self->touch        = NULL;
	self->solid        = SOLID_NOT;
	self->takedamage   = DAMAGE_NO;
	self->clipmask     = 0;
	self->movetype     = MOVETYPE_NONE;
	VectorClear (self->velocity);
	gi.linkentity (self);
}

/*------------------------------------------------------/ New Code /--------
//  This animates the fireball/explosion.  The magic numbers in this
//  function represent the frame numbers in id's r_explode md2.
//  While thinking, the shot will burn nearby entities, except its
//  owner; thus making the shot an area effect weapon.
//------------------------------------------------------------------------*/
void PBM_FlameThrowerThink (edict_t *self)
{
	if (++self->s.frame > 6)
	{	G_FreeEdict (self);
	return;
	}
	
	/* Put out the fire if it hits water. */
	if (gi.pointcontents(self->s.origin) & MASK_WATER)
	{       PBM_BecomeSteam(self);
	return;
	}
	
	PBM_BurnRadius (self, self->dmg_radius, self->pos2, self->owner);
	
	//self->s.skinnum++;
	
	self->nextthink = level.time + FRAMETIME;
}

/*------------------------------------------------------/ New Code /--------
//  Create and launch a fat fireball.
//------------------------------------------------------------------------*/
void PBM_FireFlameThrower(edict_t *self, vec3_t start, vec3_t spread, vec3_t dir, int speed, vec3_t damage, vec3_t radius_damage, int rain_chance, int blast_chance)
{
	edict_t  *fireball;
	
	/* Adjust fireball direction with 'spread'. */
	PBM_FireAngleSpread (spread, dir);
	
	/* Create the fireball. */
	fireball = G_Spawn();
	fireball->s.modelindex = gi.modelindex("models/objects/r_explode/tris.md2");
	fireball->s.frame      = 2;
	fireball->s.skinnum    = 1;
	VectorClear (fireball->mins);
	VectorClear (fireball->maxs);
	VectorCopy (start, fireball->s.origin);
	vectoangles (dir, fireball->s.angles);
	VectorScale (dir, (float) speed, fireball->velocity);
	fireball->s.angles[PITCH] -= 90;
	fireball->s.angles[YAW]   += rand() % 360;
	fireball->movetype        = MOVETYPE_FLYMISSILE;
	fireball->clipmask        = MASK_SHOT;
	fireball->solid           = SOLID_BBOX;
	fireball->takedamage      = DAMAGE_NO;
	fireball->s.effects       = 0;
	fireball->s.renderfx      = RF_FULLBRIGHT;
	fireball->owner           = self;
	fireball->master          = self;
	fireball->classname       = "fire";
	fireball->touch           = PBM_FlameThrowerTouch;
	fireball->nextthink       = level.time + FRAMETIME * 2;
	fireball->think           = PBM_FlameThrowerThink;
	VectorCopy(damage, fireball->pos1);
	VectorCopy(radius_damage, fireball->pos2);
	fireball->dmg_radius      = RADIUS_FIRE_SPLASH;
	fireball->count           = rain_chance;
	fireball->dmg             = blast_chance;
	gi.linkentity (fireball);
	
	/* If fireball is spawned in liquid, it dissolves. */
	if (PBM_InWater (fireball))
	{       PBM_BecomeSteam (fireball);
	return;
	}
	
	/* Flamethrower is an area effect weapon, so don't bother dodging. */
}

/***************************************************/
/**************    FlameThrower       **************/
/***************************************************/
void flame_think (edict_t *ent)

{
	ent->nextthink = level.time + 0.1f;
	
	if (PBM_InWater(ent))
	{       PBM_BecomeSteam(ent);
	return;
	}
	
	if (ent->s.frame < 13)
	{
		ent->s.frame++;
		if (random() > .7) ent->s.frame++;
		
		//                ent->s.skinnum = (ent->s.frame - 1) / 2;
		if (ent->s.frame >= 10)
		{
			ent->s.renderfx |= RF_TRANSLUCENT;
			ent->s.renderfx &= ~RF_FULLBRIGHT;
		}
	}
	else
	{
		PBM_BecomeSmoke(ent);
		return;
	}
}

void flame_touch (edict_t *ent, edict_t *other, cplane_t *plane, csurface_t *surf)
{
	vec3_t	origin;
	
	if (other == ent->owner)
		return;
	
	if (surf && (surf->flags & SURF_SKY))
	{
		G_FreeEdict (ent);
		return;
	}
	
	if (ent->owner->client)
		PlayerNoise(ent->owner, ent->s.origin, PNOISE_IMPACT);
	
	// calculate position for the explosion entity
	VectorMA (ent->s.origin, -0.02f, ent->velocity, origin);
	
	if (other->takedamage)
	{
		T_Damage (other, ent, ent->owner, ent->velocity, ent->s.origin, plane->normal, 20, 0, 0, MOD_FIRE_SPLASH);
	}
	
	G_FreeEdict (ent);
}



void fire_flame (edict_t *self, vec3_t start, vec3_t dir, int damage, int speed)
{
	edict_t *flame;
	
	flame = G_Spawn();
	VectorCopy (start, flame->s.origin);
	VectorCopy (dir, flame->movedir);
	vectoangles (dir, flame->s.angles);
	flame->s.angles[2] = 90;
	flame->s.angles[1] -= 90;
	flame->avelocity[0] = crandom()*90;
	flame->s.angles[0] = crandom()*90;
	VectorScale (dir, (float) speed, flame->velocity);
	flame->movetype = MOVETYPE_FLYMISSILE;
	flame->clipmask = MASK_SHOT;
	flame->solid = SOLID_BBOX;
	flame->s.renderfx = RF_FULLBRIGHT;
	flame->s.effects = 0;
	VectorClear (flame->mins);
	VectorClear (flame->maxs);
	flame->s.modelindex = gi.modelindex ("models/objects/r_explode/tris.md2");
	flame->master = flame->owner = self;
	flame->touch = PBM_FireballTouch;
	flame->s.frame = 0;
	flame->s.skinnum = 1;
	//   if (random() < 0.6)
	//           flame->s.skinnum = 3;
	//   else
	//           if (random() < 0.5)
	//                   flame->s.skinnum = 2;
	//           else
	//                   flame->s.skinnum = 4;
	flame->nextthink = level.time + 0.2f;
	flame->think = flame_think;
	flame->dmg = damage;
	flame->s.sound = gi.soundindex ("weapons/rockfly.wav");
	flame->classname = "flame";
	flame->classnum = CN_FLAME;
	
	flame->pos1[0] = 6;
	flame->pos1[1] = 9;
	flame->pos1[2] = 25;
	flame->pos2[0] = 6;
	flame->pos2[1] = 4;
	flame->pos2[2] = 25;
	flame->dmg_radius = 70;
	flame->count        = 15;
	flame->dmg          = 10;
	
	
	if (self->client)
		check_dodge (self, flame->s.origin, dir, speed);
	
	gi.linkentity (flame);
	
	if (PBM_InWater(flame))
	{       PBM_BecomeSteam(flame);
	return;
	}
}

/***************************************************/
/**************    FlameThrower       **************/
/***************************************************/

void Flamethrower_Fire (edict_t *ent)
{
	vec3_t		start, forward, right, up, offset;
	int			damage;
	int			kick = 2;
	
	if (!ent || !ent->client)
		return;

	if (deathmatch->value)
		damage = 15;
	else
		damage = 8;
	
	if (ent->client->ps.gunframe == 5)
	{
		gi.sound(ent, CHAN_AUTO, gi.soundindex("weapons/chngnu1a.wav"), 1, ATTN_IDLE, 0);
	}
	
	if ((ent->client->ps.gunframe == 14) && !(ent->client->buttons & BUTTON_ATTACK))
	{
		ent->client->ps.gunframe = 32;
		ent->client->weapon_sound = 0;
		return;
	}
	else if ((ent->client->ps.gunframe == 21) && (ent->client->buttons & BUTTON_ATTACK)
		&&
		((ent->client->pers.inventory[ent->client->ammo_index])||
		(!ent->client->ammo_index))
		)
	{
		ent->client->ps.gunframe = 15;
	}
	else
	{
		ent->client->ps.gunframe++;
	}
	
	if (ent->client->ps.gunframe == 22)
	{
		ent->client->weapon_sound = 0;
		gi.sound(ent, CHAN_AUTO, gi.soundindex("weapons/chngnd1a.wav"), 1, ATTN_IDLE, 0);
	}
	else
	{
		ent->client->weapon_sound = gi.soundindex("weapons/chngnl1a.wav");
	}
	
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
	
	
	if ((ent->client->ammo_index)&&
		(!ent->client->pers.inventory[ent->client->ammo_index]))
	{
		if (level.time >= ent->pain_debounce_time)
		{
			gi.sound(ent, CHAN_VOICE, gi.soundindex("weapons/noammo.wav"), 1, ATTN_NORM, 0);
			ent->pain_debounce_time = level.time + 1;
		}
		NoAmmoWeaponChange (ent);
		return;
	}
	
	if (is_quad)
	{
		damage *= 4;
		kick *= 4;
	}
	// get start / end positions
	AngleVectors (ent->client->v_angle, forward, right, up);
	VectorSet(offset, 4, 4, ent->viewheight - 8.0f);
	P_ProjectSource (ent->client, ent->s.origin, offset, forward, right, start);
	
	VectorMA(start, 10, forward, start);
	
	fire_flame (ent, start, forward, damage, /*speed*/ 200);
	ent->client->curr_heat += WEPHEAT_HIGH;
	
	if (ent->client && ent->client->pers.special == AODEATH)
	{
		fire_flame (ent, ent->client->angel->s.origin, forward, damage, /*speed*/ 200);
	}
	
	if ((! ( (int)dmflags->value & DF_INFINITE_AMMO ) )
		&&(ent->client->ammo_index))
	{
		if (!ent->client->machinegun_shots)
		{
			ent->client->pers.inventory[ent->client->ammo_index]--;
			ent->client->machinegun_shots = 10;
		}
		ent->client->machinegun_shots--;
	}
}

void Weapon_Flamethrower (edict_t *ent)
{
	static int	pause_frames[]	= {38, 43, 51, 61, 0};
	static int	fire_frames[]	= {5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 0};
	
	Weapon_Generic (ent, 4, 31, 61, 64, pause_frames, fire_frames, Flamethrower_Fire);
}
