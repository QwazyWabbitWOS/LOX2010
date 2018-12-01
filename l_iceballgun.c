
/***************************************************/
/**************      Freeze gun       **************/
/***************************************************/

#include "g_local.h"
#include "x_fire.h"
#include "l_iceballgun.h"

//ice functions
void ThrowIce (edict_t *self)
{
	vec3_t	forward, right, start, offset;
	
	if (!self->client->iceball)
		return;
	
	AngleVectors (self->client->v_angle, forward, right, NULL);
	VectorSet(offset, 20 + self->maxs[0], 4, self->viewheight - 4.0f);
	P_ProjectSource (self->client, self->s.origin, offset, forward, right, start);
	
	VectorCopy(start, self->client->iceball->s.origin);
	VectorScale(forward, 800, self->client->iceball->velocity);
	VectorCopy(self->client->iceball->velocity, self->client->iceball->movedir);
	
	self->client->iceball->prethink = NULL;
	self->client->iceball->think = G_FreeEdict;
	self->client->iceball->nextthink = level.time + 2;
	
	if (self->client)
		check_dodge (self, self->client->iceball->s.origin, forward, 800);
	
	self->client->iceball = NULL;
	//repeat ball
	self->client->machinegun_shots = 0;
}

void IcePosFromPlayer (edict_t *ent, float size, vec3_t output)
{
	vec3_t	forward, right, offset;
	
	AngleVectors (ent->client->v_angle, forward, right, NULL);
	VectorSet(offset, 20 + size, 4, ent->viewheight - 4.0f);
	P_ProjectSource (ent->client, ent->s.origin, offset, forward, right, output);
	
}

void IceThink (edict_t *self)
{
	vec3_t start;
	
	IcePosFromPlayer(self->owner, self->maxs[0], start);
	VectorCopy( start , self->s.origin);
	VectorCopy(self->movedir, self->velocity);
}

void UnfreezeMonster (edict_t *self)
{
	int n;
	
	//just unfreeze the monster
	self->flags &= ~FL_FROZEN;
	//sound
	gi.sound (self, CHAN_AUTO, gi.soundindex ("weapons/shatter.wav"), 1, ATTN_IDLE, 0);
	
	n = rand() % 4 + 1;
	while (n--)
		ThrowDebris (self, "models/objects/debris2/tris.md2", 1, self->s.origin);
	
	n = rand() % 3 + 1;
	while (n--)
		ThrowDebris (self, "models/objects/debris1/tris.md2", 1, self->s.origin);
	
	if (self->flags & FL_WASFLYING)
	{
		self->flags &= ~FL_WASFLYING;
		self->flags |= FL_FLY;
		self->movetype = MOVETYPE_STEP;
	}
	
	self->think = self->reallythink;
	self->reallythink = NULL;
	
	if (self->think != NULL)
		self->think(self);
}

void AttachIceThink (edict_t *self)
{
	vec3_t spot;
	
	if (!(self->owner->flags & FL_FROZEN))
	{
		G_FreeEdict(self);
		return;
	}
	
	if (gi.pointcontents (self->s.origin) & MASK_WATER)
	{
		if (self->owner->client)
			self->reallynextthink = level.time + FRAMETIME;
		else
			if (self->owner->think)
				self->owner->nextthink = level.time + FRAMETIME;
	}
	
	// I will use some PM fire stuff to attach ice
	
	PBM_FireSpot (spot, self->owner);
	
	VectorCopy (spot, self->s.origin);
	VectorCopy (self->owner->velocity, self->velocity);
	gi.linkentity (self);
	
	self->think = AttachIceThink;
	self->nextthink = level.time + FRAMETIME;
}

void AttachIce (edict_t *self)
{
	edict_t *icecube;
	
	icecube = G_Spawn();
	icecube->owner = self;
	icecube->solid = SOLID_NOT;
	VectorClear(icecube->velocity);
	icecube->s.modelindex = gi.modelindex("models/icecube/tris.md2");
	icecube->s.frame = 1;
	icecube->s.renderfx = RF_TRANSLUCENT;
	icecube->s.effects = EF_FLAG2;
	
	AttachIceThink(icecube);
	gi.linkentity(icecube);
}

void T_FreezeRadiusDamage (edict_t *inflictor, edict_t *attacker, float damage, edict_t *ignore, float radius, int mod)
{
	int		points;
	edict_t	*ent = NULL;
	vec3_t	v, dir;
	
	while ((ent = findradius(ent, inflictor->s.origin, radius)) != NULL)
	{
		if (ent == ignore)
			continue;
		if (!( (ent->svflags & SVF_MONSTER) || (ent->client) ))
			continue;
		if (ent->health <= 0)
			continue;
		if (!ent->takedamage)
			continue;
		if (ent->flags & FL_FROZEN)
			continue;
		
		VectorAdd (ent->mins, ent->maxs, v);
		VectorMA (ent->s.origin, 0.5, v, v);
		VectorSubtract (inflictor->s.origin, v, v);
		points = (int) (damage - 0.85f * VectorLength (v));
		if (ent == attacker)
			points = points / 2;
		
		if (points > 0)
		{
			if (CanDamage (ent, inflictor))
			{
				VectorSubtract (ent->s.origin, inflictor->s.origin, dir);
				T_Damage (ent, inflictor, attacker, dir, inflictor->s.origin, vec3_origin, points, points, DAMAGE_RADIUS|DAMAGE_NO_ARMOR|DAMAGE_NO_KNOCKBACK, mod);
				
				if (ent->health == 1)
				{
					ent->flags |= FL_FROZEN;
					gi.sound (ent, CHAN_AUTO, gi.soundindex ("weapons/frozen.wav"), 1, ATTN_NORM, 0);
					//attach cube
					AttachIce(ent);
					if (ent->svflags & SVF_MONSTER)
					{
						//kill his sounds
						ent->s.sound = 0;
						
						ent->reallythink = ent->think;
						ent->think = UnfreezeMonster;
						ent->nextthink = level.time + 4 + 2*random();
						if (ent->flags & FL_FLY)
						{
							ent->flags |= FL_WASFLYING;
							ent->groundentity = NULL;
							ent->flags &= ~FL_FLY;
							ent->movetype = MOVETYPE_TOSS;
						}
					}
					else if (ent->client) /*is a client*/
					{
						//kill sounds
						ent->s.sound = 0;
						ent->client->weapon_sound = 0;
						
						ent->reallynextthink = level.time + 4 + 2*random();
						//save v_angle
						//VectorCopy(ent->client->v_angle, ent->monsterinfo.startangles);
						//launch his ball.
						ThrowIce(ent);
					}
				}
			}
		}
	}
}

void IceTouch (edict_t *ent, edict_t *other, cplane_t *plane, csurface_t *surf)
{
	if (other == ent->owner)
	{
		if (other->client) {
			if (!other->client->iceball)
				VectorCopy(ent->movedir, ent->velocity);
		}
		return;
	}

	if (surf && (surf->flags & SURF_SKY))
	{
		G_FreeEdict (ent);
		return;
	}

	//do some fun stuff

	T_FreezeRadiusDamage(ent, ent->owner, ent->dmg + 0.0f, NULL, ent->dmg + 40.0f, MOD_FREEZE);

	if (ent->owner->client)
		if (ent->owner->client->iceball == ent)
		{
			ent->owner->client->iceball = NULL;
			ent->owner->client->machinegun_shots = 1;
		}

		ent->think = G_FreeEdict;
		ent->nextthink = level.time + 0.1f;
		ent->solid = SOLID_NOT;
		ent->s.event = EV_PLAYER_TELEPORT; //fog!
}

void CreateIce (edict_t *self)
{
	vec3_t  start;
	trace_t tr;
	edict_t *ice;
	
	IcePosFromPlayer(self, 4, start);
	tr = gi.trace (self->s.origin, NULL, NULL, start, self, MASK_SHOT);
	if (! ( (int)dmflags->value & DF_INFINITE_AMMO ) )
		self->client->pers.inventory[self->client->ammo_index]--;
	
	ice = G_Spawn();
	self->client->iceball = ice;
	VectorClear(ice->velocity);
	ice->movetype = MOVETYPE_FLYMISSILE;
	ice->clipmask = MASK_SHOT;
	ice->solid = SOLID_BBOX;
	ice->s.effects = EF_COLOR_SHELL|EF_FLAG2;
	ice->s.renderfx = RF_TRANSLUCENT|RF_SHELL_BLUE;
	VectorSet(ice->mins, -4, -4, -4);
	VectorSet(ice->maxs, 4, 4, 4);
	ice->s.modelindex = gi.modelindex ("models/ice/tris.md2");
	ice->owner = self;
	ice->touch = IceTouch;
	ice->s.frame = 0;
	ice->s.skinnum = 0;
	VectorClear(ice->movedir);
	ice->classname = "ice";
	ice->classnum = CN_ICE;
	ice->dmg          = 40;
	VectorCopy(tr.endpos, ice->s.origin);
	gi.linkentity (ice);
}

void Weapon_Freezegun_Fire (edict_t *ent)
{
	int		j;
	float	frame;
	int		damage = 10;
	
	ent->client->weapon_sound = gi.soundindex("weapons/hyprbl1a.wav");
	
	if (!(ent->client->buttons & BUTTON_ATTACK))
	{
		if (ent->client->iceball)
			ThrowIce(ent);
		
		ent->client->machinegun_shots = 0;
		
		ent->client->ps.gunframe++;
	}
	else
	{
		if ((ent->client->ammo_index)&&
			(! ent->client->pers.inventory[ent->client->ammo_index] ))
		{
			if (ent->client->iceball)
				ThrowIce(ent);
			
			if (level.time >= ent->pain_debounce_time)
			{
				gi.sound(ent, CHAN_VOICE, gi.soundindex("weapons/noammo.wav"), 1, ATTN_NORM, 0);
				ent->pain_debounce_time = level.time + 1;
			}
			NoAmmoWeaponChange (ent);
		}
		else
		{
			if (ent->client->iceball != NULL)
			{
				if (ent->client->iceball->s.frame < 19)
				{
					if (ent->client->machinegun_shots >= 1)
					{
						if ((! ( (int)dmflags->value & DF_INFINITE_AMMO ) )
							&&(ent->client->ammo_index))
							ent->client->pers.inventory[ent->client->ammo_index]--;
						ent->client->machinegun_shots = 0;
					}
					else
						ent->client->machinegun_shots++;
					
					//Wyrm: ctf not added yet
					//damage = CTFApplyStrength(ent, damage);
					ent->client->iceball->dmg += damage;
					
					ent->client->iceball->s.frame++;
					
					frame = (float) floor(0.6 * ent->client->iceball->s.frame);
					for (j=0 ; j <=2 ; j++)
					{
						ent->client->iceball->maxs[j] = 4 + frame;
						ent->client->iceball->mins[j] = -4 - frame;
					}
					
				}
			}
			else
			{
				if (ent->client->machinegun_shots)
				{
					if (ent->client->machinegun_shots >= 10)
						ent->client->machinegun_shots = 0;
					else
						ent->client->machinegun_shots++;
				}
				else
					CreateIce(ent);
			}
		}
		
		ent->client->ps.gunframe++;
		if (ent->client->ps.gunframe == 12 &&
			((!ent->client->ammo_index)||
			(ent->client->pers.inventory[ent->client->ammo_index]))
			)
			ent->client->ps.gunframe = 6;
	}
	
	if (ent->client->ps.gunframe == 12)
	{
		gi.sound(ent, CHAN_AUTO, gi.soundindex("weapons/hyprbd1a.wav"), 1, ATTN_NORM, 0);
		ent->client->weapon_sound = 0;
	}
	
}

void Weapon_Freezegun (edict_t *ent)
{
	static int	pause_frames[]	= {0};
	static int	fire_frames[]	= {6, 7, 8, 9, 10, 11, 0};
	
	Weapon_Generic (ent, 5, 20, 49, 53, pause_frames, fire_frames, Weapon_Freezegun_Fire);
}
