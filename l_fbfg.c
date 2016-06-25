//======================================================================
//  BFG10K
//======================================================================


#include "g_local.h"
#include "g_team.h"
#include "l_angels.h"
#include "l_fbfg.h"

/*
=================
fire_fbfg
=================
*/
void fbfg_explode (edict_t *self)
{
	edict_t	*ent;
	float	points;
	vec3_t	v;
	float	dist;
	int mod;
	
	// Set up the means of death.
	mod = MOD_BFG_EFFECT;
	if (i_fragban & WFB_BFG10K)
		mod |= MOD_NOFRAG;
	
	if (self->s.frame == 0)
	{
		// the BFG effect
		ent = NULL;
		while ((ent = findradius (ent, self->s.origin, self->dmg_radius)) != NULL)
		{
			if (!ent->takedamage)
				continue;
			if (ent == self->owner)
				continue;
			if (!CanDamage (ent, self))
				continue;
			if (!CanDamage (ent, self->owner))
				continue;
			
			VectorAdd (ent->mins, ent->maxs, v);
			VectorMA (ent->s.origin, 0.5, v, v);
			VectorSubtract (self->s.origin, v, v);
			dist = VectorLength(v);
			points = (float) (self->radius_dmg * (1.0 - sqrt(dist/self->dmg_radius)));
			if (ent == self->owner)
				points = points * 0.5f;
			
			gi.WriteByte (svc_temp_entity);
			gi.WriteByte (TE_BFG_EXPLOSION);
			gi.WritePosition (ent->s.origin);
			gi.multicast (ent->s.origin, MULTICAST_PHS);
			T_Damage (ent, self, self->owner, self->velocity, ent->s.origin,
				vec3_origin, (int)points, 0, DAMAGE_ENERGY, mod);
		}
	}
	
	self->nextthink = level.time + FRAMETIME;
	self->s.frame++;
	if (self->s.frame == 5)
		self->think = G_FreeEdict;
}

void fbfg_touch (edict_t *self, edict_t *other, cplane_t *plane, csurface_t *surf)
{
	int mod;
	
	if (other == self->owner)
		return;
	
	if (surf && (surf->flags & SURF_SKY))
	{
		G_FreeEdict (self);
		return;
	}
	
	if (self->owner->client)
		PlayerNoise(self->owner, self->s.origin, PNOISE_IMPACT);
	
	// Set up the means of death.
	mod = MOD_FBFG_BLAST;
	if (i_fragban & WFB_BFG10K)
		mod |= MOD_NOFRAG;
	
	// core explosion - prevents firing it into the wall/floor
	if (other->takedamage)
		T_Damage (other, self, self->owner, self->velocity, self->s.origin,
		plane->normal, 200, 0, 0, mod);
	T_RadiusDamage (self, self->owner, 200, other, 20, mod);
	
	gi.sound (self, CHAN_VOICE, gi.soundindex ("weapons/bfg__x1b.wav"), 1, ATTN_NORM, 0);
	self->solid = SOLID_NOT;
	self->touch = NULL;
	VectorMA (self->s.origin, -1 * FRAMETIME, self->velocity, self->s.origin);
	VectorClear (self->velocity);
	self->s.modelindex = gi.modelindex ("sprites/s_bfg2.sp2");
	self->s.frame = 0;
	self->s.sound = 0;
	self->s.effects &= ~EF_ANIM_ALLFAST;
	self->think = fbfg_explode;
	self->nextthink = level.time + FRAMETIME;
	self->enemy = other;
	
	gi.WriteByte (svc_temp_entity);
	gi.WriteByte (TE_BFG_BIGEXPLOSION);
	gi.WritePosition (self->s.origin);
	gi.multicast (self->s.origin, MULTICAST_PVS);
}


void fbfg_think (edict_t *self)
{
	edict_t	*ent;
	edict_t	*ignore;
	vec3_t	point,dir,start,end;
	int		dmg;
	trace_t	tr;
	int mod;
	
	// Set up the means of death.
	mod = MOD_BFG_LASER;
	if (i_fragban & WFB_BFG10K)
		mod |= MOD_NOFRAG;
	
	if (deathmatch->value)
		dmg = 5;
	else
		dmg = 10;
	
	ent = NULL;
	while ((ent = findradius(ent, self->s.origin, 256)) != NULL)
	{
		if (ent == self)
			continue;
		if (ent == self->owner)
			continue;
		if (!ent->takedamage)
			continue;
		if (!(ent->svflags & SVF_MONSTER)
			&& (!ent->client) 
			&& (strcmp(ent->classname, "misc_explobox") != 0))
			continue;
		
		//ZOID
		//don't target teammates in CTF or COOP (//QW//)
		if ((ctf->value || coop->value) && ent->client &&
			self->owner->client &&
			ent->client->resp.ctf_team == self->owner->client->resp.ctf_team)
			continue;
		//ZOID
		
		VectorMA (ent->absmin, 0.5, ent->size, point);
		
		VectorSubtract (point, self->s.origin, dir);
		VectorNormalize (dir);
		
		ignore = self;
		VectorCopy (self->s.origin, start);
		VectorMA (start, 2048, dir, end);
		for (;;)
		{
			tr = gi.trace (start, NULL, NULL, end, ignore, CONTENTS_SOLID|CONTENTS_MONSTER|CONTENTS_DEADMONSTER);
			
			if (!tr.ent)
				break;
			
			// hurt it if we can
			if ((tr.ent->takedamage) && !(tr.ent->flags & FL_IMMUNE_LASER) && (tr.ent != self->owner))
				//	T_Damage (tr.ent, self, self->owner, dir, tr.endpos, vec3_origin,
				//		dmg, 1, DAMAGE_ENERGY, mod);
			{
				ent->frozentime = level.time + 8.0f;
				// Freeze them.
				ent->frozen = 1;
			}
			// if we hit something that's not a monster or player we're done
			if (!(tr.ent->svflags & SVF_MONSTER) && (!tr.ent->client))
			{
				gi.WriteByte (svc_temp_entity);
				gi.WriteByte (TE_LASER_SPARKS);
				gi.WriteByte (4);
				gi.WritePosition (tr.endpos);
				gi.WriteDir (tr.plane.normal);
				gi.WriteByte (self->s.skinnum);
				gi.multicast (tr.endpos, MULTICAST_PVS);
				break;
			}
			
			ignore = tr.ent;
			VectorCopy (tr.endpos, start);
		}
		
		gi.WriteByte (svc_temp_entity);
		gi.WriteByte (TE_BFG_LASER);
		gi.WritePosition (self->s.origin);
		gi.WritePosition (tr.endpos);
		gi.multicast (self->s.origin, MULTICAST_PHS);
	}
	
	self->nextthink = level.time + FRAMETIME;
}

void fire_fbfg (edict_t *self, vec3_t start, vec3_t dir, int damage, int speed, float damage_radius)
{
	edict_t	*bfg;
	
	bfg = G_Spawn();
	VectorCopy (start, bfg->s.origin);
	VectorCopy (dir, bfg->movedir);
	vectoangles (dir, bfg->s.angles);
	VectorScale (dir, (float) speed, bfg->velocity);
	bfg->movetype = MOVETYPE_FLYMISSILE;
	bfg->clipmask = MASK_SHOT;
	bfg->solid = SOLID_BBOX;
	bfg->s.effects |= EF_BFG | EF_ANIM_ALLFAST;
	VectorClear (bfg->mins);
	VectorClear (bfg->maxs);
	bfg->s.modelindex = gi.modelindex ("sprites/s_bfg1.sp2");
	bfg->s.effects |= EF_COLOR_SHELL;
	bfg->s.renderfx |= RF_SHELL_BLUE;
	bfg->s.effects |= EF_BFG;
	bfg->owner = self;
	bfg->touch = fbfg_touch;
	bfg->nextthink = level.time + 8000/speed;
	bfg->think = G_FreeEdict;
	bfg->radius_dmg = damage;
	bfg->dmg_radius = damage_radius;
	bfg->classname = "bfg blast";
	bfg->classnum = CN_BFGBLAST;
	bfg->s.sound = gi.soundindex ("weapons/bfg__l1a.wav");
	bfg->think = fbfg_think;
	bfg->nextthink = level.time + FRAMETIME;
	bfg->teammaster = bfg;
	bfg->teamchain = NULL;
	
	if (self->client)
		check_dodge (self, bfg->s.origin, dir, speed);
	
	gi.linkentity (bfg);
}

void weapon_fbfg_fire (edict_t *ent)
{
	vec3_t	offset, start, forward, right;
	int		damage;
	float	damage_radius = 1000;
	
	if (deathmatch->value)
		damage = 200;
	else
		damage = 500;
	
	if (ent->client->ps.gunframe == 9)
	{
		// send muzzle flash
		gi.WriteByte (svc_muzzleflash);
		gi.WriteShort (ent-g_edicts);
		gi.WriteByte (MZ_BFG | is_silenced);
		gi.multicast (ent->s.origin, MULTICAST_PVS);
		
		ent->client->ps.gunframe++;
		
		PlayerNoise(ent, start, PNOISE_WEAPON);
		return;
	}
	
	// cells can go down during windup (from power armor hits), so
	// check again and abort firing if we don't have enough now
	if (ent->client->pers.inventory[ent->client->ammo_index] < 50)
	{
		ent->client->ps.gunframe++;
		return;
	}
	
	if (is_quad)
		damage *= 4;
	
	AngleVectors (ent->client->v_angle, forward, right, NULL);
	
	VectorScale (forward, -2, ent->client->kick_origin);
	
	// make a big pitch kick with an inverse fall
	ent->client->v_dmg_pitch = -40;
	ent->client->v_dmg_roll = crandom()*8;
	ent->client->v_dmg_time = level.time + DAMAGE_TIME;
	
	VectorSet(offset, 8, 8, ent->viewheight - 8.0f);
	P_ProjectSource (ent->client, ent->s.origin, offset, forward, right, start);
	fire_fbfg (ent, start, forward, damage, 400, damage_radius);
	if (ent->client && ent->client->pers.special == AODEATH)
	{
		fire_fbfg (ent, ent->client->angel->s.origin, forward, damage, 400, damage_radius);
	}
	ent->client->ps.gunframe++;
	
	PlayerNoise(ent, start, PNOISE_WEAPON);
	
	if (! ((int)dmflags->value & DF_INFINITE_AMMO))
		ent->client->pers.inventory[ent->client->ammo_index]
		-= ent->client->pers.weapon->quantity;
}

void Weapon_FBFG (edict_t *ent)
{
	static int	pause_frames[]	= {39, 45, 50, 55, 0};
	static int	fire_frames[]	= {9, 17, 0};
	
	Weapon_Generic (ent, 8, 32, 55, 58, pause_frames, fire_frames, weapon_fbfg_fire);
}
