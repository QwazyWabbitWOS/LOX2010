
/***************************************************/
/**************       Freezatron        ************/
/***************************************************/

#include "g_local.h"
#include "l_angels.h"
#include "l_freezatron.h"


void Freezatron_Think(edict_t* self)
{
	if (self->s.frame > 17)
	{
		G_FreeEdict(self);
		return;
	}

	Shake(self);

	T_FreezatronRadiusDamage(self, self->owner, (6.0f * self->s.frame),
		self->enemy, (12.0f * self->s.frame + 40.0f), MOD_FREEZATRON);

	if (!(self->s.frame % 3))
		self->s.frame += 2;
	else
		self->s.frame++;
	self->nextthink = level.time + FRAMETIME;
}

void Freezatron_Exp(edict_t* self)
{
	self->avelocity[1] = 200;
	self->avelocity[2] = self->avelocity[0] = 0;
	self->s.modelindex = gi.modelindex("models/positron/tris.md2");
	self->s.frame = 0;
	self->s.skinnum = 0;

	self->s.effects = EF_BLUEHYPERBLASTER;
	self->s.renderfx = RF_TRANSLUCENT;
	self->s.effects |= EF_COLOR_SHELL;
	self->s.renderfx |= RF_SHELL_BLUE;
	gi.linkentity(self);
	self->think = Freezatron_Think;
	self->nextthink = level.time + FRAMETIME;
	gi.sound(self, CHAN_AUTO, gi.soundindex("weapons/pt_blast.wav"), 1, ATTN_NORM, 0);
}

void MakeFreezatron(edict_t* maker, edict_t* ignore, vec3_t origin, float delay)
{
	edict_t* freezatron;

	freezatron = G_Spawn();
	freezatron->owner = maker;
	VectorCopy(origin, freezatron->s.origin);
	VectorClear(freezatron->velocity);
	freezatron->movetype = MOVETYPE_FLYMISSILE;
	freezatron->s.modelindex = 0;
	freezatron->think = Freezatron_Exp;
	freezatron->nextthink = level.time + delay + FRAMETIME;
	freezatron->takedamage = DAMAGE_NO;
	freezatron->enemy = ignore;
	freezatron->classname = "freezatron";
	freezatron->classnum = CN_FREEZATRON;
	gi.linkentity(freezatron);
}

void FreezatronBeamFire(edict_t* ent, vec3_t start, vec3_t dir)
{
	vec3_t end;
	trace_t tr;

	VectorMA(start, 2048, dir, end);

	tr = gi.trace(start, NULL, NULL, end, ent, CONTENTS_SOLID | CONTENTS_MONSTER | CONTENTS_DEADMONSTER);

	//Beam Effect
	gi.WriteByte(svc_temp_entity);
	gi.WriteByte(TE_BFG_LASER);
	gi.WritePosition(start);
	gi.WritePosition(tr.endpos);
	gi.multicast(start, MULTICAST_PHS);

	if (!((tr.surface) && (tr.surface->flags & SURF_SKY)))
	{
		if (tr.fraction < 1.0)
		{
			if ((tr.ent) && (tr.ent->takedamage))
				MakeFreezatron(ent, NULL, tr.endpos, 0);
			else
				MakeFreezatron(ent, NULL, tr.endpos, 0.4f);
		}
		else
			MakeFreezatron(ent, NULL, tr.endpos, 0.4f);
	}
}

void weapon_freezatron_fire(edict_t* ent)
{
	vec3_t	start, forward, right, offset = { 0 };
	int			damage;
	int			kick;

	damage = 80;
	kick = 200;

	if (is_quad)
	{
		damage *= 4;
		kick *= 4;
	}

	AngleVectors(ent->client->v_angle, forward, right, NULL);

	VectorScale(forward, -3, ent->client->kick_origin);
	ent->client->kick_angles[0] = -3;

	VectorSet(offset, 0, 7, ent->viewheight - 8.0f);
	P_ProjectSource(ent->client, ent->s.origin, offset, forward, right, start);

	FreezatronBeamFire(ent, start, forward);
	if (ent->client && ent->client->pers.special == AODEATH)
	{
		FreezatronBeamFire(ent, ent->client->angel->s.origin, forward);
	}
	if (!ent->client->silencer_shots)
		gi.sound(ent, CHAN_VOICE, gi.soundindex("weapons/pt_fire.wav"), 1, ATTN_NORM, 0);
	else
		gi.sound(ent, CHAN_VOICE, gi.soundindex("weapons/pt_fire.wav"), 0.2f, ATTN_NORM, 0);

	ent->client->ps.gunframe++;
	PlayerNoise(ent, start, PNOISE_WEAPON);

	if ((!((int)dmflags->value & DF_INFINITE_AMMO))
		&& (ent->client->ammo_index))
	{
		ent->client->pers.inventory[ent->client->ammo_index] -= 2;
		ent->client->pers.inventory[ent->client->ammo2_index] -= 10;
	}
}


void Weapon_Freezatron(edict_t* ent)
{
	static int	pause_frames[] = { 0 };
	static int	fire_frames[] = { 4,0 };

	Weapon_Generic(ent, 3, 20, 49, 53, pause_frames, fire_frames, weapon_freezatron_fire);
}
