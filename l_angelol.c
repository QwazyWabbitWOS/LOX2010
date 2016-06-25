
//****************************************************
//		Angel of Life
//****************************************************


#include "g_local.h"
#include "x_fire.h"
#include "l_angels.h"

void aol_finditem (edict_t *ent);
int aol_needitem (edict_t *ent, edict_t *it);

void AOL_Think (edict_t *ent)
{
	vec3_t	offset,vel,len;
	trace_t tr;
	
	ent->groundentity = NULL;
	
	if (ent->owner->client->angel != ent || ent->owner->client->pers.special != AOLIFE)
	{
		G_FreeEdict (ent);
		return;
	}
	
	vectoangles (ent->velocity, ent->s.angles);
	
	tr = gi.trace (ent->s.origin, NULL, NULL, ent->owner->s.origin, NULL, MASK_SHOT);
	if (tr.ent != ent->owner)
	{
		VectorAdd (ent->owner->s.origin, ent->move_origin, offset);
		VectorCopy (offset, ent->s.origin);
	}
	
	// new offset?
	if (rand()%10 < 1 || ent->move_origin[0] == 0)
	{
		VectorSet (offset, rand()%100 - 50.0f, rand()%100 - 50.0f, (float)(rand()%50));
		VectorCopy (offset, ent->move_origin);
	}
	
	// set velocity
	if (!(ent->goalentity) && ent->owner->health > 0)
		aol_finditem(ent);
	
	if (ent->goalentity && ent->owner->health > 0)
	{
		VectorSubtract (ent->owner->s.origin, ent->goalentity->s.origin, offset);
		
		if (!ent->goalentity->inuse)
		{
			ent->goalentity = NULL;
		}
		else if (VectorLength (offset) > 300)
		{
			ent->goalentity = NULL;
		}
		else if (ent->goalentity->svflags & SVF_NOCLIENT)
		{
			ent->goalentity = NULL;
		}
		else
		{
			VectorSubtract (ent->s.origin, ent->goalentity->s.origin, len);
			if (VectorLength (len) < 10)
			{
				int			i, num;
				edict_t		*touch[MAX_EDICTS], *hit;
				
				num = gi.BoxEdicts (ent->absmin, ent->absmax, touch, MAX_EDICTS, AREA_TRIGGERS);
				
				//				ent->goalentity = NULL;
				//				ent->enemy = NULL;
				
				for (i=0 ; i<num ; i++)
				{
					hit = touch[i];
					if (hit != ent->goalentity)
						continue;
					if (!hit->inuse)
						continue;
					if (!hit->touch)
						continue;
					if (ent->owner->health <= 0)
						continue;
					
					hit->touch (hit, ent->owner, NULL, NULL);
				}
				ent->goalentity = NULL;
				ent->enemy = NULL;
			}
			
			if (ent->goalentity == NULL)
				return;
			VectorSubtract (ent->goalentity->s.origin, ent->s.origin, vel);
			VectorScale (vel, 7, vel);
			VectorCopy (vel, ent->velocity);
		}
	}
	
	if (!(ent->goalentity) || ent->owner->health <= 0)
	{
		VectorAdd (ent->owner->s.origin, ent->move_origin, offset);
		VectorSubtract (offset, ent->s.origin, vel);
		VectorScale (vel, 2, vel);
		VectorAdd (vel, ent->velocity, ent->velocity);
	}
	
	ent->nextthink = level.time + FRAMETIME;
}

void aol_finditem (edict_t *ent)
{
	edict_t	*cur;
	trace_t	tr;
	
	cur = findradius (world, ent->owner->s.origin, 300);
	
	while (cur != NULL)
	{
		if (cur->item && cur != ent->enemy && aol_needitem (ent->owner, cur))
		{
			//gi.cprintf (ent->owner, PRINT_HIGH, "aol found an item, %s\n", cur->classname);
			tr = gi.trace (cur->s.origin, ent->mins, ent->maxs, ent->s.origin, ent, MASK_SOLID);
			if (tr.fraction == 1.0)
			{
				//gi.cprintf (ent->owner, PRINT_HIGH, "ent: %s, targ: %s\n", tr.ent->classname, cur->classname);
				ent->goalentity = cur;
				return;
			}
		}
		cur = findradius (cur, ent->owner->s.origin, 300);
	}
	return;
}

int aol_needitem (edict_t *ent, edict_t *it)
{
	int index = ITEM_INDEX(it->item);
	
	if (it->svflags & SVF_NOCLIENT) return QFALSE;
	
	if (it->item->pickup == Pickup_Health && (ent->health < ent->max_health || it->style & 1))
		return QTRUE;
	
	if (it->item->pickup == Pickup_Weapon)
	{
		if (((int)(dmflags->value) & DF_WEAPONS_STAY) && ent->client->pers.inventory[index])
		{
			//if (!(it->spawnflags & (DROPPED_ITEM | DROPPED_PLAYER_ITEM) ) )
			return QFALSE;	// leave the weapon for others to pickup
		}
		
		if (it->touch == Drop_Temp_Touch) return QFALSE;
		if ((it->spawnflags & (DROPPED_ITEM | DROPPED_PLAYER_ITEM))) return QFALSE;
		
		// map-spawned fundamental weapons
		// If any of these are banned, the angel will ignore them and you
		// will have to run over the weapon in order to pick up the associated
		// LOX weapons and ammo.
		// This was added to prevent the angel from zoning out of it's orbit
		// when a base weapon was banned. It would be a very rare configuration
		// to have any of these weapons banned, but the case is now covered, otherwise
		// the angel goes off on a track and never comes back.
		if ((it->item == &gI_weapon_shotgun && (i_weaponban & WB_SHOTGUN)) ||
			(it->item == &gI_weapon_supershotgun && (i_weaponban & WB_SUPERSHOTGUN)) ||
			(it->item == &gI_weapon_machinegun && (i_weaponban & WB_MACHINEGUN)) ||
			(it->item == &gI_weapon_chaingun && (i_weaponban & WB_CHAINGUN)) ||
			(it->item == &gI_weapon_grenadelauncher && (i_weaponban & WB_GRENADELAUNCHER)) ||
			(it->item == &gI_weapon_rocketlauncher && (i_weaponban & WB_ROCKETLAUNCHER)) ||
			(it->item == &gI_weapon_hyperblaster && (i_weaponban & WB_HYPERBLASTER)) ||
			(it->item == &gI_weapon_railgun && (i_weaponban & WB_RAILGUN)) ||
			(it->item == &gI_weapon_bfg && (i_weaponban & WB_BFG10K)))
			return QFALSE; // the base weapon was banned so we ignore it
		return QTRUE; // all other weapons get picked up
	}
	
	if (it->item->pickup == Pickup_Powerup)	return QFALSE;
	if (it->item->pickup == Pickup_Adrenaline) return QTRUE;
	if (it->item->pickup == Pickup_AncientHead) return QTRUE;
	if (it->item->pickup == Pickup_Bandolier) return QTRUE;
	if (it->item->pickup == Pickup_Pack) return QTRUE;
	if (it->item->pickup == Pickup_Key) return QTRUE;
	
	if (it->item->pickup == Pickup_Ammo)
	{
		int max = 0, index = 0;
		
		if (it->item->tag == AMMO_BULLETS)
			max = ent->client->pers.max_bullets;
		else if (it->item->tag == AMMO_SHELLS)
			max = ent->client->pers.max_shells;
		else if (it->item->tag == AMMO_ROCKETS)
			max = ent->client->pers.max_rockets;
		else if (it->item->tag == AMMO_GRENADES)
			max = ent->client->pers.max_grenades;
		else if (it->item->tag == AMMO_CELLS)
			max = ent->client->pers.max_cells;
		else if (it->item->tag == AMMO_SLUGS)
			max = ent->client->pers.max_slugs;
		else
			return QFALSE;
		
		index = ITEM_INDEX(it->item);
		
		if (ent->client->pers.inventory[index] >= max)
			return QFALSE;
		
		if (it->spawnflags & DROPPED_ITEM)
			return QFALSE;
		
		return QTRUE;
	}
	
	if (it->item->pickup == Pickup_Armor)
	{
		int old_armor_index = ArmorIndex(ent);
		if (it->item->tag == ARMOR_SHARD) 
			return QTRUE;
		else if (old_armor_index == 0) 
			return QTRUE;
		else
		{
			gitem_armor_t	*oldinfo;
			gitem_armor_t	*newinfo;
			
			newinfo = (gitem_armor_t *)it->item->info;
			if (old_armor_index == jacket_armor_index)
				oldinfo = &jacketarmor_info;
			else if (old_armor_index == combat_armor_index)
				oldinfo = &combatarmor_info;
			else // (old_armor_index == body_armor_index)
				oldinfo = &bodyarmor_info;
			
			if (newinfo->normal_protection > oldinfo->normal_protection)
			{
				return QTRUE;
			}
			
			// if we're already maxed out then we don't need the new armor
			if (ent->client->pers.inventory[old_armor_index] >= oldinfo->max_count)
				return QFALSE;
		}
	}
	return QFALSE;
}
