
/***************************************************/
/**************       Positron        **************/
/***************************************************/

#include "g_local.h"
#include "l_angels.h"
#include "l_positron.h"

void Shake (edict_t *self)
{
	edict_t *selected;
	vec3_t dist;
	float distance;
	int i;
	
	for (i=0 ; i<maxclients->value ; i++)
	{
		selected = g_edicts + 1 + i;
		if (!selected->inuse)
			continue;
		
		if (visible(self, selected))
		{
			VectorSubtract(selected->s.origin, self->s.origin, dist);
			if ((distance=VectorLength(dist)) <= 500)
				selected->client->kick_angles[0] = crandom() * (10 - distance * 0.02f);
		}
	}
}


void Positron_Think (edict_t *self)
{
	if (self->s.frame > 17)
	{
		G_FreeEdict (self);
		return;
	}

	Shake (self);
	T_RadiusDamage(self, self->owner, (12.0f * self->s.frame), self->enemy, (12.0f * self->s.frame + 40.0f), MOD_POSITRON);
	//   T_RadiusDamage (self,self.owner,(8 * self.frame),world);
	
	if (!(self->s.frame % 3))
		self->s.frame += 2;
	else
		self->s.frame++;
	self->nextthink = level.time + FRAMETIME;
}

void Positron_Exp (edict_t *self)
{
	self->avelocity[1] = 200;
	self->avelocity[2] = self->avelocity[0] = 0;
	self->s.modelindex = gi.modelindex("models/positron/tris.md2");
	self->s.frame = 0;
	self->s.skinnum = 0;
	self->s.effects = EF_HYPERBLASTER;
	self->s.renderfx = RF_TRANSLUCENT;
	gi.linkentity(self);
	self->think = Positron_Think;
	self->nextthink = level.time + FRAMETIME;
	gi.sound (self, CHAN_AUTO, gi.soundindex ("weapons/pt_blast.wav"), 1, ATTN_NORM, 0);
}

void MakePositron (edict_t *maker, edict_t *ignore, vec3_t origin, float delay)
{
	edict_t *positron;
	
	positron = G_Spawn();
	positron->owner = maker;
	VectorCopy(origin, positron->s.origin);
	VectorClear (positron->velocity);
	positron->movetype = MOVETYPE_FLYMISSILE;
	positron->s.modelindex = 0;
	positron->think = Positron_Exp;
	positron->nextthink = level.time + delay + FRAMETIME;
	positron->takedamage = DAMAGE_NO;
	positron->enemy = ignore;
	positron->classname = "positron";
	positron->classnum = CN_POSITRON;
	gi.linkentity (positron);
}

void PositronBeamFire(edict_t *ent, vec3_t start, vec3_t dir)
{
	vec3_t end;
	trace_t tr;
	
	VectorMA (start, 2048, dir, end);
	
	tr = gi.trace (start, NULL, NULL, end, ent, CONTENTS_SOLID|CONTENTS_MONSTER|CONTENTS_DEADMONSTER);
	
	//Beam Effect
	gi.WriteByte (svc_temp_entity);
	gi.WriteByte (TE_BFG_LASER);
	gi.WritePosition (start);
	gi.WritePosition (tr.endpos);
	gi.multicast (start, MULTICAST_PHS);
	
	if (!((tr.surface) && (tr.surface->flags & SURF_SKY)))
	{
		if (tr.fraction < 1.0)
		{
			if ((tr.ent) && (tr.ent->takedamage))
				MakePositron(ent, NULL, tr.endpos, 0);
			else
				MakePositron(ent, NULL, tr.endpos, 0.4f);
		}
		else
			MakePositron(ent, NULL, tr.endpos, 0.4f);
	}
}

void weapon_positron_fire (edict_t *ent)
{
	vec3_t	start, forward, right, offset;
	int			damage = 80;
	int			kick = 200;

	if (is_quad)
	{
		damage *= 4;
		kick *= 4;
	}

	AngleVectors (ent->client->v_angle, forward, right, NULL);
	
	VectorScale (forward, -3, ent->client->kick_origin);
	ent->client->kick_angles[0] = -3;
	
	VectorSet(offset, 0, 7,  ent->viewheight - 8.0f);
	P_ProjectSource (ent->client, ent->s.origin, offset, forward, right, start);
	
	PositronBeamFire(ent, start, forward);
	if (ent->client && ent->client->pers.special == AODEATH)
	{
		PositronBeamFire (ent, ent->client->angel->s.origin, forward);
	}
	if (ent->client && !ent->client->silencer_shots)
		gi.sound(ent, CHAN_VOICE, gi.soundindex("weapons/pt_fire.wav"), 1, ATTN_NORM, 0);
	else
		gi.sound(ent, CHAN_VOICE, gi.soundindex("weapons/pt_fire.wav"), 0.2f, ATTN_NORM, 0);
	
	ent->client->ps.gunframe++;
	PlayerNoise(ent, start, PNOISE_WEAPON);
	
	if ((! ( (int)dmflags->value & DF_INFINITE_AMMO ) )
		&&(ent->client->ammo_index))
	{
		ent->client->pers.inventory[ent->client->ammo_index] -= 2;
		ent->client->pers.inventory[ent->client->ammo2_index] -= 10;
	}
}


void Weapon_Positron (edict_t *ent)
{
	static int	pause_frames[]	= {0};
	static int	fire_frames[]	= {4,0};

	Weapon_Generic (ent, 3, 20, 49, 53, pause_frames, fire_frames, weapon_positron_fire);
}
