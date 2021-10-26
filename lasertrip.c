#include "g_local.h"
#include "g_team.h"

// darKMajick definitions
#include "l_dm_grenades.h"

void weapon_lasertrip_die (edict_t *self, edict_t *inflictor,
						   edict_t *attacker, int damage, vec3_t point)
{
	// Make the "laser tripwire shot off the wall" sound.
	gi.sound (self, CHAN_WEAPON,
		gi.soundindex ("flyer/flydeth1.wav"), 1, ATTN_NORM, 0);
	
	// Blow up the grenade
	self->think = BecomeExplosion1;
	self->nextthink = level.time + FRAMETIME;
	
	// Shut off the laser
	self->activator->think = G_FreeEdict;
	self->activator->nextthink = level.time + FRAMETIME;
	if (self->owner->client->lasertrips > 0) 
		self->owner->client->lasertrips--;  //QW//
}

static void weapon_lasertrip_think (edict_t *self)
{
	edict_t	*ignore;
	vec3_t	start, end, point, last_movedir;
	trace_t	tr;
	int		count;
	
	if (self->spawnflags & FL_RESPAWN)
		count = 8;
	else
		count = 4;
	
	if (self->enemy)
	{
		_VectorCopy (self->movedir, last_movedir);
		VectorMA (self->enemy->absmin, 0.5, self->enemy->size, point);
		VectorSubtract (point, self->s.origin, self->movedir);
		VectorNormalize (self->movedir);
		if (!VectorCompare(self->movedir, last_movedir))
			self->spawnflags |= FL_RESPAWN;
	}
	
	ignore = self;
	_VectorCopy (self->s.origin, start);
	VectorMA (start, 2048, self->movedir, end);
	
	for (;;)
	{
		tr = gi.trace (start, NULL, NULL, end, ignore,
			CONTENTS_SOLID|CONTENTS_MONSTER|CONTENTS_DEADMONSTER);
		
		if (!tr.ent)
			break;
		
		// clean up tripwires when player respawns or quits
		// laser is owned by grenade is owned by player (sheesh)
		if (self->owner->owner != NULL && self->owner->owner->client != NULL)  // catch null owner
		{
			if (self->owner->owner->client->lasertrips == 0 || self->owner->owner->inuse == 0) 
			{
				self->think = G_FreeEdict;
				if (self->owner->inuse)
				{
					G_FreeEdict(self->owner);		// just delete them
				}
				break;
			}
		}
		
		// blow up if someone triggers us
		if ((tr.ent->svflags & SVF_MONSTER) || tr.ent->client)
		{
			// Make the "laser tripwire firing" sound.
			gi.sound (self->owner, CHAN_WEAPON,
				gi.soundindex ("flyer/flysght1.wav"), 1, ATTN_NORM, 0);
			
			// Set off the grenade.
			self->owner->takedamage = DAMAGE_NO;
			self->owner->think = Grenade_Explode_dM;
			self->owner->nextthink = level.time + FRAMETIME;
			
			// Shut off the laser
			self->think = G_FreeEdict;
		}
		
		// if we hit something that's not a monster or player or is immune to lasers,
		// we're done
		if (!(tr.ent->svflags & SVF_MONSTER) && (!tr.ent->client))
		{
			if (self->spawnflags & FL_RESPAWN)
			{
				self->spawnflags &= ~FL_RESPAWN;
				gi.WriteByte (svc_temp_entity);
				gi.WriteByte (TE_LASER_SPARKS);
				gi.WriteByte (count);
				gi.WritePosition (tr.endpos);
				gi.WriteDir (tr.plane.normal);
				gi.WriteByte (self->s.skinnum & 255);
				gi.multicast (tr.endpos, MULTICAST_PVS);
			}
			break;
		}
		
		ignore = tr.ent;
		VectorCopy (tr.endpos, start);
	}

	VectorCopy (tr.endpos, self->s.old_origin);
	self->nextthink = level.time + FRAMETIME;
}

static void weapon_lasertrip_on (edict_t *self)
{
	if (!self->activator)
		self->activator = self;
	self->spawnflags |= 0x80000001;
	self->svflags &= ~SVF_NOCLIENT;
	weapon_lasertrip_think (self);
}

static void pre_weapon_lasertrip_think (edict_t *self)
{
	weapon_lasertrip_on (self);
	self->think = weapon_lasertrip_think;
}

void PlaceLaserTripwire (edict_t *ent)
{
	edict_t *laser, *grenade;
	vec3_t forward, clientp, wallp;
	trace_t tr;
	gitem_t *it;
	int	type;

	int  laser_colour[] =
	{
		0xf2f2f0f0,             // red
		0xd0d1d2d3,             // green
		0xf3f3f1f1,             // blue
		0xdcdddedf,             // yellow
		0xe0e1e2e3              // bitty yellow strobe
	};
	
	// No laser tripwires when you're dead or frozen.
	if ((ent->deadflag) || (ent->frozen))
		return;
	
	// Or when you're a ghost.
	if (ctf->value && ent->movetype == MOVETYPE_NOCLIP && ent->solid == SOLID_NOT)
		return;
	
	// Or if laser tripwires (or grenades) have been banned.
	if ((i_featureban & FB_TRIPLASER)
		|| (i_weaponban & WB_GRENADE))
		return;
	
	// Or if it's been too soon since the last tripwire this player placed.
	if (level.time < ent->client->tripwire_debounce_time)
		return;
	
	// cells for laser ?
	if (ent->client->pers.inventory[ITEM_INDEX(&gI_ammo_cells)] < 5)
	{
		gi.cprintf(ent, PRINT_HIGH, "Not enough cells to set a laser tripwire.\n");
		return;
	}
	
	//QW// Limit the number of tripwires the user can set
	if (max_lasertrips->value > 10) gi.cvar_set("max_lasertrips", "10");  //set a reasonable hard coded limit.
	if (max_lasertrips->value < 1) gi.cvar_set("max_lasertrips", "1");  //use weaponban to disable them
	if (ent->client->lasertrips >= max_lasertrips->value)
	{
		gi.cprintf(ent,PRINT_HIGH, "Too many tripwires activated\n");
		return;
	}
	
	//The server will crash if telenade tripwires are set on a teleport pad and someone trips on them.  
	//Quick fix: set type to plasma for all tripwires if they have teleport selected.
	if (ent->client->dM_grenade == DM_TELEGRENADE) 
		ent->client->dM_grenade = DM_PLASMACLUSTER;  //QW//
	
	// get grenade type for the bomb.
	it = itemlist[ITEM_INDEX (&gI_ammo_grenades) + ent->client->dM_grenade];
	
	// enough grenades?
	if (ent->client->pers.inventory[ITEM_INDEX(&gI_ammo_grenades)]
		< ent->client->dM_ammoCost)
	{
		if (it == &gI_ammo_grenades)
			gi.cprintf(ent, PRINT_HIGH, "No grenades for laser tripwire.\n");
		else
			gi.cprintf(ent, PRINT_HIGH, "Not enough grenades for %s "
			"laser tripwire.\n", it->pickup_name);
		return;
	}
	
	type = ent->client->dM_grenade;

	// Setup "little look" to close wall
	VectorCopy (ent->s.origin, clientp);
	clientp[2] += ent->viewheight;
	
	// Cast along view angle
	AngleVectors (ent->client->v_angle, forward, NULL, NULL);
	
	// Setup end point
	wallp[0] = clientp[0] + forward[0] * 100;
	wallp[1] = clientp[1] + forward[1] * 100;
	wallp[2] = clientp[2] + forward[2] * 100;  
	
	// trace
	tr = gi.trace (clientp, NULL, NULL, wallp, ent, MASK_SOLID);
	
	// Line complete ? (ie. no collision)
	if (tr.fraction == 1.0)
	{
		gi.cprintf (ent, PRINT_HIGH, "Too far from wall.\n");
		return;
	}
	
	// Hit sky (or nothing)?
	if (!tr.surface || tr.surface->flags & SURF_SKY)
		return;
	
	// -------------
	// Setup grenade
	// -------------
	grenade = G_Spawn();
	
	VectorClear (grenade->mins);
	VectorClear (grenade->maxs);
	VectorCopy (tr.endpos, grenade->s.origin);
	vectoangles (tr.plane.normal, grenade->s.angles);
	
	grenade->movetype = MOVETYPE_NONE;
	grenade->clipmask = MASK_SHOT;
	grenade->solid = SOLID_NOT;
	grenade->s.modelindex = gi.modelindex ("models/objects/grenade2/tris.md2");
	grenade->classname = "lasertrip_grenade";
	grenade->classnum = CN_LASERTRIP_GRENADE;
	grenade->owner = ent;
	grenade->think = NULL;
	grenade->nextthink = 0;
	
	// Make the grenade react to radius damage, i.e. people can try to blow
	// them up.
	grenade->solid = SOLID_BBOX;
	VectorSet (grenade->mins, -8, -8, -8);
	VectorSet (grenade->maxs, 8, 8, 8);
	grenade->takedamage = DAMAGE_YES;
	grenade->clipmask = MASK_SOLID;
	grenade->health = 20;
	grenade->max_health = 20;
	grenade->dmg = 0;	// these will get set according to the type
	grenade->dmg_radius = 0; // when the grenade explodes
	grenade->die = weapon_lasertrip_die;
	grenade->dm_type = type;

	switch (type)
	{
	case DM_RAILBOMB:
		grenade->s.effects |= EF_COLOR_SHELL;
		grenade->s.renderfx |= RF_SHELL_BLUE;
		grenade->s.effects |= EF_GRENADE;
		break;
		
	case DM_PLASMAGRENADE:
		grenade->s.effects |= EF_COLOR_SHELL;
		grenade->s.renderfx |= RF_SHELL_GREEN;
		grenade->s.effects |= EF_BFG;
		break;
		
	case DM_NAPALMGRENADE:
		grenade->s.effects |= EF_COLOR_SHELL;
		grenade->s.renderfx |= RF_SHELL_RED;
		grenade->s.effects |= EF_ROCKET;
		break;
		
	case DM_CATACLYSM:
		grenade->s.effects |= EF_COLOR_SHELL;
		grenade->s.renderfx |= RF_SHELL_BLUE;
		grenade->s.renderfx |= RF_SHELL_GREEN;
		grenade->s.renderfx |= RF_SHELL_RED;
		grenade->s.effects |= EF_ROCKET;
		break;
		
	default:	// all others
		grenade->s.effects |= EF_GRENADE;
		break;
	}

	gi.linkentity (grenade);
	
	// MAJOR HACK TO USE THE LASERTRIP GRENADE FEATURE FOR POINT LOCATIONALS.
	if (ext_devt->value)
	{
		grenade->s.modelindex = gi.modelindex ("models/objects/grenade2/tris.md2");
		grenade->think = G_FreeEdict;
		grenade->nextthink = level.time + 3;
		gi.cprintf (ent, PRINT_HIGH, "Point is [%i,%i,%i]",
			(int)tr.endpos[0], (int)tr.endpos[1], (int)tr.endpos[2]);
		if (tr.ent && strcmp (ent->classname, "worldspawn") != 0)
		{
			gi.cprintf (ent, PRINT_HIGH, ", classname %s", tr.ent->classname);
			if (tr.ent->model)
				gi.cprintf (ent, PRINT_HIGH, ", model %s", tr.ent->model);
		}
		gi.cprintf (ent, PRINT_HIGH, "\n");
		return;
	}
	
	// -----------
	// Setup laser
	// -----------
	laser = G_Spawn();
	
	laser -> movetype      = MOVETYPE_NONE;
	laser -> solid         = SOLID_NOT;
	laser -> s.renderfx    = RF_BEAM|RF_TRANSLUCENT;
	laser -> s.modelindex  = 1;    // must be non-zero
	laser -> s.sound       = gi.soundindex ("world/laser.wav");
	laser -> classname     = "lasertrip_laser";
	laser -> s.frame       = 2;    // beam diameter
	laser -> owner         = grenade;
	laser -> s.skinnum     = laser_colour[((int) (random() * 1000)) % 5];
	
	// Set orgin of laser to point of contact with wall
	VectorCopy (tr.endpos,laser->s.origin);
	
	// convert normal at point of contact to laser angles
	vectoangles (tr.plane.normal,laser -> s.angles);
	
	// setup laser movedir (projection of laser)
	G_SetMovedir (laser->s.angles, laser->movedir);
	
	VectorSet (laser->mins, -8, -8, -8);
	VectorSet (laser->maxs, 8, 8, 8);
	
	// link to world
	gi.linkentity (laser);
	
	// link the grenade to the laser
	grenade->activator = laser;
	
	// start off
	target_laser_off (laser);
	
	// but make it automatically come on
	laser->nextthink = level.time + 2;	//two second delay
	laser->think = pre_weapon_lasertrip_think;
	
	// Deduct the ammo.
	if (!((int)dmflags->value & DF_INFINITE_AMMO))
	{
		ent->client->pers.inventory[ITEM_INDEX(&gI_ammo_cells)] -= 5;
		ent->client->pers.inventory[ITEM_INDEX(&gI_ammo_grenades)]
			-= ent->client->dM_ammoCost;
	}
	
	ent->client->lasertrips++;   //QW// increment the semaphore, we allow up to max_lasertrips per user
	
	if (it == &gI_ammo_grenades)
		gi.cprintf (ent, PRINT_MEDIUM, "Laser tripwire attached.\n");
	else
		gi.cprintf (ent, PRINT_MEDIUM, "%s laser tripwire attached.\n",
		it->pickup_name);
	
	// Make the "laser tripwire set" sound.
	gi.sound (ent, CHAN_WEAPON, gi.soundindex ("world/fusein.wav"), 1,
		ATTN_NORM, 0);
	
	// Keep them from setting another laser tripwire for a few seconds.
	ent->client->tripwire_debounce_time = level.time + 1.5f;
}
