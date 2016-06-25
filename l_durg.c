
//*********************
//		Durg
//*********************

#include "g_local.h"
#include "l_angels.h"
#include "l_durg.h"

/*
=================
fire_durg
=================
*/
void fire_durg (edict_t *self, vec3_t start, vec3_t aimdir, int damage, int kick)
{
	vec3_t		from, end;
	trace_t		tr;
	edict_t		*ignore;
	int			mask,i, index;
	qboolean	water;
	int mod;
	
	// Set up the means of death.
	mod = MOD_RAILGUN2;
	
	VectorMA (start, 8192, aimdir, end);
	VectorCopy (start, from);
	ignore = self;
	water = QFALSE;
	mask = MASK_SHOT|CONTENTS_SLIME|CONTENTS_LAVA;
	while (ignore)
	{
		tr = gi.trace (from, NULL, NULL, end, ignore, mask);
		
		if (tr.contents & (CONTENTS_SLIME|CONTENTS_LAVA))
		{
			mask &= ~(CONTENTS_SLIME|CONTENTS_LAVA);
			water = QTRUE;
		}
		else
		{
			if ((tr.ent->svflags & SVF_MONSTER) || (tr.ent->client))
				ignore = tr.ent;
			else
				ignore = NULL;
			
			if ((tr.ent != self) && (tr.ent->takedamage))
			{
				T_Damage (tr.ent, self, self, aimdir, tr.endpos, tr.plane.normal,
					0, 1, 0, mod);
				if (tr.ent->client != NULL && tr.ent->health > 0)
				{
					TossClientWeapon(tr.ent);
					index = ITEM_INDEX (tr.ent->client->pers.weapon);
					if ((tr.ent->client->pers.weapon != NULL) &&
						(tr.ent->client->pers.weapon != &gI_weapon_blaster ||
						tr.ent->client->pers.weapon != &gI_weapon_superblaster ||
						tr.ent->client->pers.weapon != &gI_weapon_mace ||
						tr.ent->client->pers.weapon != &gI_weapon_flaregun ||
						tr.ent->client->pers.weapon != &gI_weapon_antiflaregun))
						tr.ent->client->pers.inventory[index]--;
					NoAmmoWeaponChange(tr.ent);
				}
			}
		}
		
		VectorCopy (tr.endpos, from);
	}
	
	// send gun puff / flash
	for (i=0; i<10; i++)
	{
		gi.WriteByte (svc_temp_entity);
		gi.WriteByte (TE_BUBBLETRAIL);//TE_RAILTRAIL);
		
		gi.WritePosition (start);
		gi.WritePosition (tr.endpos);
		gi.multicast (self->s.origin, MULTICAST_PHS);
		//gi.multicast (start, MULTICAST_PHS);
	}
	if (water)
	{
		for (i=0; i<10; i++)
		{
			gi.WriteByte (svc_temp_entity);
			gi.WriteByte (TE_BUBBLETRAIL);
			gi.WritePosition (start);
			gi.WritePosition (tr.endpos);
			gi.multicast (tr.endpos, MULTICAST_PHS);
		}
	}
	
	if (self->client)
		PlayerNoise(self, tr.endpos, PNOISE_IMPACT);
}

void weapon_durg_fire (edict_t *ent)
{
	vec3_t		start;
	vec3_t		forward, right;
	vec3_t		offset;
	int			damage;
	int			kick;
	
	// Same "extreme" damage in SP game *and* deathmatch :)
	damage = 0;
	kick = 2;
	
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
	fire_durg (ent, start, forward, damage, kick);
	if (ent->client && ent->client->pers.special == AODEATH)
	{
		fire_durg (ent, ent->client->angel->s.origin, forward, damage, kick);
	}
	// send muzzle flash
	gi.WriteByte (svc_muzzleflash);
	gi.WriteShort (ent-g_edicts);
	gi.WriteByte (MZ_RAILGUN | is_silenced);
	gi.multicast (ent->s.origin, MULTICAST_PVS);
	
	ent->client->ps.gunframe++;
	PlayerNoise(ent, start, PNOISE_WEAPON);
	
	if (! ((int)dmflags->value & DF_INFINITE_AMMO))
		ent->client->pers.inventory[ent->client->ammo_index]
		-= ent->client->pers.weapon->quantity;
}


void Weapon_Durg (edict_t *ent)
{
	static int	pause_frames[]	= {56, 0};
	static int	fire_frames[]	= {4, 0};
	
	Weapon_Generic (ent, 3, 18, 56, 61, pause_frames, fire_frames, weapon_durg_fire);
}
