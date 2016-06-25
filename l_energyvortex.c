
//======================================================================
//  Energy Vortex
//======================================================================

#include "g_local.h"
#include "l_energyvortex.h"
#include "l_airfist.h"

void energyvortex_explode (edict_t *self)
{
	T_RadiusDamage(self, self->enemy, 200, NULL, 100, MOD_VORTEX);
	
	gi.WriteByte (svc_temp_entity);
	gi.WriteByte (TE_ROCKET_EXPLOSION);
	gi.WritePosition (self->s.origin);
	gi.multicast (self->s.origin, MULTICAST_PHS);
	
	G_FreeEdict(self);
}

void energyvortex_active_touch (edict_t *self, edict_t *other, cplane_t *plane, csurface_t *surf)
{
	if (surf && (surf->flags & SURF_SKY))
	{
		G_FreeEdict (self);
		return;
	}
	
	// core explosion - prevents firing it into the wall/floor
	if (other->takedamage)
	{
		T_Damage (other, self, self->enemy, self->velocity, self->s.origin, plane->normal, 200, 0, 0, MOD_VORTEX);
		
		if (self->enemy->client)
			PlayerNoise(self->enemy, self->s.origin, PNOISE_IMPACT);
		
		gi.sound (self, CHAN_VOICE, gi.soundindex ("weapons/bfg__x1b.wav"), 1, ATTN_NORM, 0);
	}
}


void energyvortex_think (edict_t *self)
{
	vec3_t dir, delta;
	float percent, distance;
	float max_mass = 800; //max mass that can be moved
	float actual_mass;
	edict_t *ent;
	int strength = 110;
	float inRange = 760;
	
	if (!(self->s.effects & EF_TRAP))
	{
		if (self->groundentity)
		{
			while (self->s.angles[0] >= 360)
				self->s.angles[0] -= 360;
			
			while (self->s.angles[0] <= -360)
				self->s.angles[0] += 360;
			
			if (self->s.angles[0] > 0)
			{
				self->s.angles[0] -= 20;
				if (self->s.angles[0] < 0) self->s.angles[0] = 0;
			}
			else if (self->s.angles[0] < 0)
			{
				self->s.angles[0] += 20;
				if (self->s.angles[0] > 0) self->s.angles[0] = 0;
			}
			
			while (self->s.angles[2] >= 360)
				self->s.angles[2] -= 360;
			
			while (self->s.angles[2] <= -360)
				self->s.angles[2] += 360;
			
			if (self->s.angles[2] > 0)
			{
				self->s.angles[2] -= 20;
				if (self->s.angles[2] < 0) self->s.angles[2] = 0;
			}
			else if (self->s.angles[2] < 0)
			{
				self->s.angles[2] += 20;
				if (self->s.angles[2] > 0) self->s.angles[2] = 0;
			}
		}
		else
		{
			VectorCopy(self->velocity, dir);
			VectorNormalize (dir);
			vectoangles(dir, self->s.angles); //Wyrm: adjust angle!!
			self->s.angles[0] -= 90;
		}
		
		if (self->timestamp < level.time)
		{
			self->s.effects = EF_TRAP|EF_ROTATE;
			self->touch = energyvortex_active_touch;
			self->timestamp = level.time + 3.2f;
			self->owner = NULL;
			gi.sound(self, CHAN_AUTO, gi.soundindex("grav_new.wav"), 1, ATTN_NORM, 0);
		}
	}
	else
	{
		//pull stuff
		ent = NULL;
		while ((ent = horn_findradius(ent, self->s.origin, inRange)) != NULL)
		{
			if (visible(self,ent) &&
				//horn_infront(self, ent)&&
				ent != self)
			{
				if ((ent->svflags & SVF_DEADMONSTER) 
					&& (ent->movetype == MOVETYPE_NONE))
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
					
					VectorSubtract(ent->s.origin, self->s.origin, delta);
					distance = VectorLength(delta);
					if (distance <= 32)
						self->touch(self, ent, NULL, NULL);
					
					percent = (inRange - distance) / inRange;
					
					if (ent->groundentity)
					{
						ent->groundentity = NULL;
						ent->s.origin[2]++;
						if (!ent->client)
						{
							if (delta[2] > 0)
								delta[2] = delta[2] / -2;
							VectorScale (delta, 0.7f, delta);
							if (delta[2] > -50)
								delta[2] = -50;
						}
					}
					
					VectorNormalize(delta);
					
					if (ent->mass < 100) actual_mass = 100;
					else actual_mass = ent->mass;
					
					VectorScale(delta, percent * strength * (max_mass / actual_mass), delta);
					
					VectorMA(ent->velocity, -1, delta, ent->velocity);
				}
			}
		}
		
		if (self->timestamp < level.time)
		{
			energyvortex_explode (self);
			return;
		}
	}
	
	self->think = energyvortex_think;
	self->nextthink = level.time + FRAMETIME;
}

void energyvortex_touch (edict_t *self, edict_t *other, cplane_t *plane, csurface_t *surf)
{
	if (other == self->owner)
		return;
	
	if (surf && (surf->flags & SURF_SKY))
	{
		G_FreeEdict (self);
		return;
	}
}


void fire_energyvortex (edict_t *self, vec3_t start, vec3_t dir, int damage, int speed, float damage_radius)
{
	edict_t *vortex;
	
	vortex = G_Spawn();
	VectorCopy (start, vortex->s.origin);
	VectorCopy (dir, vortex->movedir);
	vectoangles (dir, vortex->s.angles);
	VectorScale (dir, (float) speed, vortex->velocity);
	vortex->movetype = MOVETYPE_TOSS;
	vortex->clipmask = MASK_SHOT;
	//vortex->solid = SOLID_TRIGGER;
	vortex->solid = SOLID_BBOX;
	vortex->s.effects |= EF_BLASTER;
	//VectorClear (vortex->mins);
	//VectorClear (vortex->maxs);
	VectorClear(vortex->mins);
	VectorClear(vortex->maxs);
	//VectorSet(vortex->mins, -16, -16, -8);
	//VectorSet(vortex->maxs, 16, 16, 24);
	vortex->s.modelindex = gi.modelindex ("models/items/keys/pyramid/tris.md2");
	vortex->enemy = vortex->owner = self;
	vortex->touch = energyvortex_touch;
	vortex->nextthink = level.time + 8000/speed;
	vortex->think = G_FreeEdict;
	vortex->radius_dmg = damage;
	vortex->dmg_radius = damage_radius;
	vortex->classname = "vortex blast";
	vortex->classnum = CN_VORTEXBLAST;
	//vortex->think = energyvortex_think;
	//vortex->nextthink = level.time + FRAMETIME;
	vortex->timestamp = level.time + 2;
	vortex->takedamage = DAMAGE_NO;
	energyvortex_think(vortex);
	vortex->teammaster = vortex;
	vortex->teamchain = NULL;
	
	if (self->client)
		check_dodge (self, vortex->s.origin, dir, speed);
	
	gi.linkentity (vortex);
	
}

void weapon_energyvortex_fire (edict_t *ent)
{
	vec3_t	offset, start;
	vec3_t  forward, right, up, dir;
	int		damage;
	float	damage_radius = 1000;
	
	if (deathmatch->value)
		damage = 200;
	else
		damage = 500;
	
	if (ent->client->ps.gunframe == 9)
	{
		if (!ent->client->silencer_shots)
			gi.sound(ent, CHAN_VOICE, gi.soundindex("weapons/bfg__f1y.wav"), 1, ATTN_NORM, 0);
		else
			gi.sound(ent, CHAN_VOICE, gi.soundindex("weapons/bfg__f1y.wav"), 0.2f, ATTN_NORM, 0);
		
		ent->client->ps.gunframe++;
		
		//PlayerNoise(ent, start, PNOISE_WEAPON);
		return;
	}
	
	// cells can go down during windup (from power armor hits), so
	// check again and abort firing if we don't have enough now
	if ((ent->client->ammo_index)&&(ent->client->pers.inventory[ent->client->ammo_index] < 100))
	{
		ent->client->ps.gunframe++;
		return;
	}
	
	if (is_quad)
		damage *= 4;
	
	AngleVectors (ent->client->v_angle, forward, right, up);
	
	VectorScale (forward, -2, ent->client->kick_origin);
	
	// make a big pitch kick with an inverse fall
	/*
	ent->client->v_dmg_pitch = -40;
	ent->client->v_dmg_roll = crandom() * 8;
	ent->client->v_dmg_time = level.time + DAMAGE_TIME;
	*/
	
	VectorSet(offset, 8, 8, ent->viewheight - 8.0f);
	P_ProjectSource (ent->client, ent->s.origin, offset, forward, right, start);
	
	
	VectorMA(up, 4, forward, dir);
	VectorNormalize(dir);
	
	fire_energyvortex (ent, start, dir, damage, 500, damage_radius);
	
	ent->client->ps.gunframe++;
	
	PlayerNoise(ent, start, PNOISE_WEAPON);
	
	if ((! ( (int)dmflags->value & DF_INFINITE_AMMO ) )
		&&(ent->client->ammo_index))
		ent->client->pers.inventory[ent->client->ammo_index] -= 100;
}

void Weapon_EnergyVortex (edict_t *ent)
{
	static int	pause_frames[]	= {39, 45, 50, 55, 0};
	static int	fire_frames[]	= {9, 17, 0};
	
	Weapon_Generic (ent, 8, 32, 55, 58, pause_frames, fire_frames, weapon_energyvortex_fire);
}
