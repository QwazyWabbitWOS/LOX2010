
#include "g_local.h"

#include "l_disintegrator.h"
#include "l_energyvortex.h"
#include "l_freezegun.h"
#include "l_kaminit.h"
#include "l_lightninggun.h"
#include "l_nailgun.h"
#include "l_nuke.h"
#include "l_nukegrenade.h"

// teamplay definitions
#include "g_team.h"

void Use_GrenadeWeapon (edict_t *ent, gitem_t *inv);
void Weapon_Blaster (edict_t *ent);
void Weapon_Shotgun (edict_t *ent);
void Weapon_SuperShotgun (edict_t *ent);
void Weapon_Machinegun (edict_t *ent); // rockets
void Weapon_Machine (edict_t *ent); // original
void Weapon_Chaingun (edict_t *ent);
void Weapon_HyperBlaster (edict_t *ent);
void Weapon_RocketLauncher (edict_t *ent);
void Weapon_SuperBlaster (edict_t *ent);
void Weapon_Grenade (edict_t *ent);
void Weapon_GrenadeLauncher (edict_t *ent);
void Weapon_Railgun (edict_t *ent);
void Weapon_SonicRailgun (edict_t *ent);
void Weapon_Railgun2 (edict_t *ent);
void Weapon_Sniper (edict_t *ent);
void Weapon_BFG (edict_t *ent);
void Weapon_FBFG (edict_t *ent);
void Weapon_Plasma (edict_t *ent);
void Weapon_Homing (edict_t *ent);
void Weapon_Streetsweeper (edict_t *ent);
void Weapon_ExplosiveStreetsweeper (edict_t *ent);
void Weapon_Bazooka (edict_t *ent);
void Weapon_Railgunsweeper(edict_t *ent);
void Weapon_Bazookasweeper(edict_t *ent);
void Weapon_SuperBlastersweeper(edict_t *ent);
void Weapon_Freezersweeper(edict_t *ent);
void Weapon_Grenadesweeper(edict_t *ent);
void Weapon_Flamethrower (edict_t *ent);
void Weapon_Skipper (edict_t *ent);
void Weapon_GuidedMissiles(edict_t *ent);
void Weapon_Freezegun(edict_t *ent);
void Weapon_Positron(edict_t *ent);
void Weapon_Freezatron(edict_t *ent);
void Weapon_Airfist(edict_t *ent);
void Weapon_PulseRifle (edict_t *ent);
void Weapon_Sword (edict_t *ent);
void Weapon_Bucky(edict_t *ent);
void Weapon_Flaregun (edict_t *ent);
void Weapon_AntiFlaregun (edict_t *ent);
void Weapon_StickingGrenadeLauncher (edict_t *ent);
void Weapon_Freezegun (edict_t *ent);
void Weapon_Chunkgun (edict_t *ent);
void Weapon_Mace (edict_t *ent);
void Weapon_Durg(edict_t *ent);
void Weapon_DGSuperShotgun (edict_t *ent);
void Weapon_VacuumMaker (edict_t *ent);
void Weapon_SpiralRocketLauncher (edict_t *ent);
void Weapon_Snipersweeper (edict_t *ent);
void Weapon_ExplosiveSuperShotgun(edict_t *ent);
void Weapon_ExplosiveChaingun(edict_t *ent);
void Weapon_ExplosiveShotgun(edict_t *ent);
void Weapon_ExplosiveMachingun(edict_t *ent);
void Weapon_StickyGrenadesweeper(edict_t *ent);
//void Weapon_Kaminit(edict_t *ent);
void Weapon_FloatingMineLauncher (edict_t *ent);
void Weapon_AntimatterCannon(edict_t *ent);
void Weapon_SuperRailShotgun(edict_t *ent);

gitem_armor_t jacketarmor_info	= { 25,  50, .30f, .00f, ARMOR_JACKET};
gitem_armor_t combatarmor_info	= { 50, 100, .60f, .30f, ARMOR_COMBAT};
gitem_armor_t bodyarmor_info	= {100, 200, .80f, .60f, ARMOR_BODY};

int	jacket_armor_index;
int	combat_armor_index;
int	body_armor_index;
int	power_screen_index;
int	power_shield_index;

#define HEALTH_IGNORE_MAX	1
#define HEALTH_TIMED		2

void Use_Quad (edict_t *ent, gitem_t *item);
static float	quad_drop_timeout_hack;

void Use_Jet ( edict_t *ent, gitem_t *item )
{
	ValidateSelectedItem ( ent );
	
	/*jetpack in inventory but no fuel time? must be one of the give all/give
	jetpack cheats, so put fuel in*/
	if ( ent->client->Jet_remaining == 0 )
		ent->client->Jet_remaining = 700;
	
	if ( Jet_Active(ent) ) 
		ent->client->Jet_framenum = 0; 
	else
		ent->client->Jet_framenum = level.framenum + ent->client->Jet_remaining;
	
	/* The On/Off Sound taken from the invulnerability */
	gi.sound( ent, CHAN_ITEM, gi.soundindex("items/protect.wav"), 0.8f, ATTN_NORM, 0 );
	
	/* this is the sound played when flying. To hear this sound 
	   immediately we play it here the first time */
	gi.sound ( ent, CHAN_AUTO, gi.soundindex("hover/hovidle1.wav"), 0.8f, ATTN_NORM, 0 );
}


//======================================================================

/*
===============
GetItemByIndex
===============
*/
gitem_t	*GetItemByIndex (int index)
{
	if (index == 0 || index >= game.num_items)
		return NULL;
	
	return itemlist[index];
}


/*
===============
FindItemByClassname

  ===============
*/
gitem_t	*FindItemByClassname (char *classname)
{
	int		i;
	gitem_t	*it;
	
	for (i=0 ; i<game.num_items ; i++)
	{
		it = itemlist[i];
		if (!it->classname)
			continue;
		if (!Q_stricmp(it->classname, classname))
			return it;
	}
	
	return NULL;
}

//======================================================================

void DoRespawn(edict_t* ent)
{
	if (ent == NULL)
	{
		gi.dprintf("WARNING: NULL ent passed to %s\n", __func__);
		return;
	}

	if (ent->team)
	{
		edict_t* master;
		unsigned count;
		unsigned choice;

		master = ent->teammaster;
		if (master == NULL)
			return;

		//ZOID
		//in ctf, when we are weapons stay, only the master of a team of weapons
		//is spawned
		if (ctf->value && ((int)dmflags->value & DF_WEAPONS_STAY) &&
			master->item && (master->item->flags & IT_WEAPON))
		{
			ent = master;
		}
		else
		{
			count = 0;
			for (ent = master; ent; ent = ent->chain)
				count++;

			choice = count ? rand() % count : 0;

			count = 0;
			for (ent = master; count < choice; ent = ent->chain)
				count++;
		}
	}

	if (ent)
	{
		ent->svflags &= ~SVF_NOCLIENT;
		ent->solid = SOLID_TRIGGER;
		gi.linkentity(ent);

		// send an effect
		ent->s.event = EV_ITEM_RESPAWN;
	}
}

void SetRespawn (edict_t *ent, float delay)
{
	ent->flags |= FL_RESPAWN;
	ent->svflags |= SVF_NOCLIENT;
	ent->solid = SOLID_NOT;
	ent->nextthink = level.time + delay;
	ent->think = DoRespawn;
	VectorCopy (ent->pos1, ent->s.origin);		// Restore origin/angles (if item
	VectorCopy (ent->pos2, ent->s.angles);		// got knocked around)
	gi.linkentity (ent);
}


//======================================================================

qboolean Pickup_Powerup (edict_t *ent, edict_t *other)
{
	int		quantity;
	
	quantity = other->client->pers.inventory[ITEM_INDEX(ent->item)];
	if ((skill->value == 1 && quantity >= 2) || (skill->value >= 2 && quantity >= 1))
		return false;
	
	if ((coop->value) && (ent->item->flags & IT_STAY_COOP) && (quantity > 0))
		return false;
	
	other->client->pers.inventory[ITEM_INDEX(ent->item)]++;
	
	/*ATTILA begin*/ 
	if  (ent->item == &gI_item_jetpack)  
	{
		other->client->pers.inventory[ITEM_INDEX(ent->item)] = 1;
		other->client->Jet_remaining = 700;
		
		/* if deathmatch-flag instant use is set, switch off the jetpack, the
		item->use function will turn it on again immediately */
		if ( (int)dmflags->value & DF_INSTANT_ITEMS )
			other->client->Jet_framenum = 0;
		
		/* otherwise update the burn out time if jetpack is activated */    
		else if ( Jet_Active(other) )
			other->client->Jet_framenum = level.framenum
			+ other->client->Jet_remaining;
	} 
	/*ATTILA end*/
	
	if (deathmatch->value)
	{
		if (!(ent->spawnflags & DROPPED_ITEM) )
			SetRespawn (ent, ent->item->quantity);
		if (((int)dmflags->value & DF_INSTANT_ITEMS)
			|| ((ent->item->use == Use_Quad)
			&& (ent->spawnflags & DROPPED_PLAYER_ITEM)))
		{
			if ((ent->item->use == Use_Quad) && (ent->spawnflags & DROPPED_PLAYER_ITEM))
				quad_drop_timeout_hack = (ent->nextthink - level.time) / FRAMETIME;
			ent->item->use (other, ent->item);
		}
	}
	
	return true;
}

void Drop_General (edict_t *ent, gitem_t *item)
{
	Drop_Item (ent, item);
	ent->client->pers.inventory[ITEM_INDEX(item)]--;
	
	ValidateSelectedItem (ent);
}


//======================================================================
qboolean Pickup_Adrenaline (edict_t *ent, edict_t *other)
{
	if (!deathmatch->value)
		other->max_health += 1;
	
	if (other->health < other->max_health)
		other->health = other->max_health;
	
	if (!(ent->spawnflags & DROPPED_ITEM) && (deathmatch->value))
		SetRespawn (ent, ent->item->quantity);
	
	return true;
}

qboolean Pickup_AncientHead (edict_t *ent, edict_t *other)
{
	other->max_health += 2;
	
	if (!(ent->spawnflags & DROPPED_ITEM) && (deathmatch->value))
		SetRespawn (ent, ent->item->quantity);
	
	return true;
}

qboolean Pickup_Bandolier (edict_t *ent, edict_t *other)
{
	gitem_t	*item;
	int		index;
	
	if (other->client->pers.max_bullets < 250)
		other->client->pers.max_bullets = 250;
	if (other->client->pers.max_shells < 150)
		other->client->pers.max_shells = 150;
	if (other->client->pers.max_cells < 250)
		other->client->pers.max_cells = 250;
	if (other->client->pers.max_slugs < 400)
		other->client->pers.max_slugs = 400;
	
	item = &gI_ammo_bullets;
	if (item)
	{
		index = ITEM_INDEX(item);
		other->client->pers.inventory[index] += item->quantity * 2;
		if (other->client->pers.inventory[index] > other->client->pers.max_bullets)
			other->client->pers.inventory[index] = other->client->pers.max_bullets;
	}
	
	item = &gI_ammo_shells;
	if (item)
	{
		index = ITEM_INDEX(item);
		other->client->pers.inventory[index] += item->quantity * 2;
		if (other->client->pers.inventory[index] > other->client->pers.max_shells)
			other->client->pers.inventory[index] = other->client->pers.max_shells;
	}
	
	if (!(ent->spawnflags & DROPPED_ITEM) && (deathmatch->value))
		SetRespawn (ent, ent->item->quantity);
	
	return true;
}

qboolean Pickup_Pack (edict_t *ent, edict_t *other)
{
	gitem_t	*item;
	int		index;
	
	if (other->client->pers.max_bullets < 300)
		other->client->pers.max_bullets = 300;
	if (other->client->pers.max_shells < 200)
		other->client->pers.max_shells = 200;
	if (other->client->pers.max_rockets < 100)
		other->client->pers.max_rockets = 100;
	if (other->client->pers.max_grenades < 100)
		other->client->pers.max_grenades = 100;
	if (other->client->pers.max_cells < 300)
		other->client->pers.max_cells = 300;
	if (other->client->pers.max_slugs < 100)
		other->client->pers.max_slugs = 100;
	
	item = &gI_ammo_bullets;
	if (item)
	{
		index = ITEM_INDEX(item);
		other->client->pers.inventory[index] += item->quantity * 2;
		if (other->client->pers.inventory[index] > other->client->pers.max_bullets)
			other->client->pers.inventory[index] = other->client->pers.max_bullets;
	}
	
	item = &gI_ammo_shells;
	if (item)
	{
		index = ITEM_INDEX(item);
		other->client->pers.inventory[index] += item->quantity * 2;
		if (other->client->pers.inventory[index] > other->client->pers.max_shells)
			other->client->pers.inventory[index] = other->client->pers.max_shells;
	}
	
	item = &gI_ammo_cells;
	if (item)
	{
		index = ITEM_INDEX(item);
		other->client->pers.inventory[index] += item->quantity * 2;
		if (other->client->pers.inventory[index] > other->client->pers.max_cells)
			other->client->pers.inventory[index] = other->client->pers.max_cells;
	}
	
	item = &gI_ammo_grenades;
	if (item && !(i_weaponban & WB_GRENADE))
	{
		index = ITEM_INDEX(item);
		other->client->pers.inventory[index] += item->quantity * 2;
		if (other->client->pers.inventory[index] > other->client->pers.max_grenades)
			other->client->pers.inventory[index] = other->client->pers.max_grenades;
	}
	
	item = &gI_ammo_rockets;
	if (item)
	{
		index = ITEM_INDEX(item);
		other->client->pers.inventory[index] += item->quantity * 2;
		if (other->client->pers.inventory[index] > other->client->pers.max_rockets)
			other->client->pers.inventory[index] = other->client->pers.max_rockets;
	}
	
	item = &gI_ammo_slugs;
	if (item)
	{
		index = ITEM_INDEX(item);
		other->client->pers.inventory[index] += item->quantity * 2;
		if (other->client->pers.inventory[index] > other->client->pers.max_slugs)
			other->client->pers.inventory[index] = other->client->pers.max_slugs;
	}
	
	if (!(ent->spawnflags & DROPPED_ITEM) && (deathmatch->value))
		SetRespawn (ent, ent->item->quantity);
	
	return true;
}

//======================================================================

void Use_Quad (edict_t *ent, gitem_t *item)
{
	float		timeout;
	
	ent->client->pers.inventory[ITEM_INDEX(item)]--;
	ValidateSelectedItem (ent);
	
	if (quad_drop_timeout_hack)
	{
		timeout = quad_drop_timeout_hack;
		quad_drop_timeout_hack = 0;
	}
	else
	{
		timeout = 300;
	}
	
	if (ent->client->quad_framenum > level.framenum)
		ent->client->quad_framenum += timeout;
	else
		ent->client->quad_framenum = level.framenum + timeout;
	
	gi.sound(ent, CHAN_ITEM, gi.soundindex("items/damage.wav"), 1, ATTN_NORM, 0);
}

//======================================================================

void Use_Breather (edict_t *ent, gitem_t *item)
{
	ent->client->pers.inventory[ITEM_INDEX(item)]--;
	ValidateSelectedItem (ent);
	
	if (ent->client->breather_framenum > level.framenum)
		ent->client->breather_framenum += 300.0;
	else
		ent->client->breather_framenum = level.framenum + 300.0;
	
	//	gi.sound(ent, CHAN_ITEM, gi.soundindex("items/damage.wav"), 1, ATTN_NORM, 0);
}

//======================================================================

void Use_Envirosuit (edict_t *ent, gitem_t *item)
{
	ent->client->pers.inventory[ITEM_INDEX(item)]--;
	ValidateSelectedItem (ent);
	
	if (ent->client->enviro_framenum > level.framenum)
		ent->client->enviro_framenum += 300;
	else
		ent->client->enviro_framenum = level.framenum + 300.0;
	
	//	gi.sound(ent, CHAN_ITEM, gi.soundindex("items/damage.wav"), 1, ATTN_NORM, 0);
}

//======================================================================

void Use_Invulnerability (edict_t *ent, gitem_t *item)
{
	ent->client->pers.inventory[ITEM_INDEX(item)]--;
	ValidateSelectedItem (ent);
	
	if (ent->client->invincible_framenum > level.framenum)
		ent->client->invincible_framenum += 300;	// use for 30 seconds
	else
		ent->client->invincible_framenum = level.framenum + 300.0;
	
	gi.sound(ent, CHAN_ITEM, gi.soundindex("items/protect.wav"), 1, ATTN_NORM, 0);
}

//======================================================================

void Use_Silencer (edict_t *ent, gitem_t *item)
{
	ent->client->pers.inventory[ITEM_INDEX(item)]--;
	ValidateSelectedItem (ent);
	ent->client->silencer_shots += 30;
	
	//	gi.sound(ent, CHAN_ITEM, gi.soundindex("items/damage.wav"), 1, ATTN_NORM, 0);
}

//======================================================================

qboolean Pickup_Key (edict_t *ent, edict_t *other)
{
	if (coop->value)
	{
		if (strcmp(ent->classname, "key_power_cube") == 0)
		{
			if (other->client->pers.power_cubes & ((ent->spawnflags & 0x0000ff00)>> 8))
				return false;
			other->client->pers.inventory[ITEM_INDEX(ent->item)]++;
			other->client->pers.power_cubes |= ((ent->spawnflags & 0x0000ff00) >> 8);
		}
		else
		{
			if (other->client->pers.inventory[ITEM_INDEX(ent->item)])
				return false;
			other->client->pers.inventory[ITEM_INDEX(ent->item)] = 1;
		}
		return true;
	}
	other->client->pers.inventory[ITEM_INDEX(ent->item)]++;
	return true;
}

//======================================================================

qboolean Add_Ammo (edict_t *ent, gitem_t *item, int count)
{
	int			index;
	int			max;
	
	if (!ent->client)
		return false;
	
	if (item->tag == AMMO_BULLETS)
		max = ent->client->pers.max_bullets;
	else if (item->tag == AMMO_SHELLS)
		max = ent->client->pers.max_shells;
	else if (item->tag == AMMO_ROCKETS)
		max = ent->client->pers.max_rockets;
	else if (item->tag == AMMO_GRENADES)
	{
		max = ent->client->pers.max_grenades;
		
		// Make sure they have the "launcher" for all the different types,
		// except the ones that have been banned.
		if (!(i_weaponban & WB_GRENADE))
		{
			if (!(i_weaponban & WB_CLUSTERGRENADE))
				ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_clustergrenade)] = 1;
			if (!(i_weaponban & WB_RAILBOMB))
				ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_railbomb)] = 1;
			if (!(i_weaponban & WB_PLASMAGRENADE))
				ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_plasmagrenade)] = 1;
			if (!(i_weaponban & WB_NAPALMGRENADE))
				ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_napalmgrenade)] = 1;
			if (!(i_weaponban & WB_SHRAPNELGRENADE))
				ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_shrapnelgrenade)] = 1;
			if (!(i_loxweaponban & LWB_CATACLYSMDEVICE))
				ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_cataclysm)] = 1;
			if (!(i_loxgrenadeban & LGB_PLASMACLUSTERGRENADE))
				ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_plasmaclustergrenade)] = 1;
			if (!(i_loxgrenadeban & LGB_TELEGRENADE))
				ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_telegrenade)] = 1;
			if (!(i_loxgrenadeban & LGB_FREEZEGRENADE))
				ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_freezegrenade)] = 1;
			if (!(i_loxgrenadeban & LGB_HOMERGRENADE))
				ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_homergrenade)] = 1;
			if (!(i_loxgrenadeban & LGB_ROCKETGRENADE))
				ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_rocketgrenade)] = 1;
			if (!(i_loxgrenadeban & LGB_BLINDINGGRENADE))
				ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_blindinggrenade)] = 1;
			if (!(i_loxgrenadeban & LGB_POSITRONGRENADE))
				ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_positrongrenade)] = 1;
			if (!(i_loxgrenadeban & LGB_MIDNIGHTGRENADE))
				ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_midnightgrenade)] = 1;
			if (!(i_loxgrenadeban & LGB_FREEZATRONGRENADE))
				ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_freezatrongrenade)] = 1;
			if (!(i_loxgrenadeban & LGB_BANZAIGRENADE))
				ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_banzaigrenade)] = 1;
			if (!(i_loxgrenadeban & LGB_SPINNINGRAILBOMB))
				ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_spinningrailbomb)] = 1;
		}
	}
	else if (item->tag == AMMO_CELLS)
		max = ent->client->pers.max_cells;
	else if (item->tag == AMMO_SLUGS)
		max = ent->client->pers.max_slugs;
	else
		return false;
	
	index = ITEM_INDEX(item);
	
	if (ent->client->pers.inventory[index] == max)
		return false;
	
	ent->client->pers.inventory[index] += count;
	
	if (ent->client->pers.inventory[index] > max)
		ent->client->pers.inventory[index] = max;
	
	return true;
}

qboolean Pickup_Ammo (edict_t *ent, edict_t *other)
{
	//int			oldcount;
	int			count;
	qboolean	weapon;
	
	weapon = (ent->item->flags & IT_WEAPON);
	if ( (weapon) && ( (int)dmflags->value & DF_INFINITE_AMMO ) )
		count = 1000;
	else if (ent->count)
		count = ent->count;
	else
		count = ent->item->quantity;
	
	//oldcount = other->client->pers.inventory[ITEM_INDEX(ent->item)];
	
	if (!Add_Ammo (other, ent->item, count))
		return false;
	
		/*
		This, apparently, is the source of that fucking "switch to grenades every
		goddamn time you run over them, whether or not you're holding a superior
		weapon" bug.
		if (weapon && !oldcount)
		{
		if (other->client->pers.weapon != ent->item
		&& (!deathmatch->value
		|| other->client->pers.weapon == &gI_weapon_blaster))
		{
		other->client->newweapon = ent->item;
		}
		}
	*/
	
	if (!(ent->spawnflags & (DROPPED_ITEM | DROPPED_PLAYER_ITEM)) && (deathmatch->value))
		SetRespawn (ent, 30);
	return true;
}

void Drop_Ammo (edict_t *ent, gitem_t *item)
{
	edict_t	*dropped;
	int		index;
	
	index = ITEM_INDEX(item);
	dropped = Drop_Item (ent, item);
	if (ent->client->pers.inventory[index] >= item->quantity)
		dropped->count = item->quantity;
	else
		dropped->count = ent->client->pers.inventory[index];
	
	if (ent->client->pers.weapon && 
		ent->client->pers.weapon->tag == AMMO_GRENADES &&
		item->tag == AMMO_GRENADES &&
		ent->client->pers.inventory[index] - dropped->count <= 0)
	{
		gi.cprintf (ent, PRINT_HIGH, "Can't drop current weapon\n");
		G_FreeEdict(dropped);
		return;
	}
	
	ent->client->pers.inventory[index] -= dropped->count;
	ValidateSelectedItem (ent);
}


//======================================================================

void MegaHealth_think (edict_t *self)
{
	if (self->owner->health > self->owner->max_health)
	{
		self->nextthink = level.time + 1;
		self->owner->health -= 1;
		return;
	}
	
	if (!(self->spawnflags & DROPPED_ITEM) && (deathmatch->value))
		SetRespawn (self, 20);
	else
		G_FreeEdict (self);
}

// returns true if we picked up a health item.
// here, ent is the item touched and other is the player
qboolean Pickup_Health (edict_t *ent, edict_t *other)
{
	// if already maxed out we don't pick up
	if (!(ent->style & HEALTH_IGNORE_MAX))
		if (other->health >= other->max_health)
			return false;

	//QW// 
	// if player is healthy and item is 25 points
	// or more no pickup
	//ZOID
	if (other->health >= 250 && ent->count > 25)
		return false;
	//ZOID

	// we're picking it up
	other->health += ent->count;

	//ZOID
	if (other->health > 250 && ent->count > 25)
		other->health = 250;
	//ZOID

	if (!(ent->style & HEALTH_IGNORE_MAX))
	{
		if (other->health > other->max_health)
			other->health = other->max_health;
	}

	// activate the mega timer
	if (ent->style & HEALTH_TIMED)
	{
		ent->think = MegaHealth_think;
		ent->nextthink = level.time + 5;
		ent->owner = other;
		ent->flags |= FL_RESPAWN;
		ent->svflags |= SVF_NOCLIENT;
		ent->solid = SOLID_NOT;
	}
	else
	{
		if (!(ent->spawnflags & DROPPED_ITEM) && (deathmatch->value))
			SetRespawn (ent, 30);
	}

	// we picked up the item
	return true;
}

//======================================================================

int ArmorIndex (edict_t *ent)
{
	if (!ent->client)
		return 0;
	
	if (ent->client->pers.inventory[jacket_armor_index] > 0)
		return jacket_armor_index;
	
	if (ent->client->pers.inventory[combat_armor_index] > 0)
		return combat_armor_index;
	
	if (ent->client->pers.inventory[body_armor_index] > 0)
		return body_armor_index;
	
	return 0;
}

qboolean Pickup_Armor (edict_t *ent, edict_t *other)
{
	int				old_armor_index;
	gitem_armor_t	*oldinfo;
	gitem_armor_t	*newinfo;
	int				newcount;
	float			salvage;
	int				salvagecount;
	
	// get info on new armor
	newinfo = (gitem_armor_t *)ent->item->info;
	
	old_armor_index = ArmorIndex (other);
	
	// handle armor shards specially
	if (ent->item->tag == ARMOR_SHARD)
	{
		if (!old_armor_index)
			other->client->pers.inventory[jacket_armor_index] = 2;
		else
			other->client->pers.inventory[old_armor_index] += 2;
	}
	
	// if player has no armor, just use it
	else if (!old_armor_index)
	{
		// do we already have the organic armor or the slug armor?
		if (!(i_loxarmorban & LAB_ORGANICARMOR) && other->client->loxarmor != 1)
		{
			int num = abs((int)(crandom() * 10000.0));
			if (num < (int)p_pickup_organic_armor->value && other->client->organicarmor == 0)
			{
				other->client->organicarmor = 1;
				other->client->loxarmor = 1;
				gi.cprintf(other, PRINT_HIGH, "Organic Armor Picked up\n");
				
			}
		}
		
		if (!(i_loxarmorban & LAB_SLUGARMOR) && other->client->loxarmor != 1 )
		{
			int num = abs((int)(crandom() * 10000.0));
			if (num < (int)p_pickup_slug_armor->value && other->client->slugarmor != 1)
			{
				other->client->slugarmor = 1;
				other->client->loxarmor = 1;
				gi.cprintf(other, PRINT_HIGH, "Slug Armor Picked up\n");
			}
		}
		
		if (!(i_loxarmorban & LAB_CELLARMOR) && other->client->loxarmor != 1)
		{
			int num = abs((int)(crandom() * 10000.0));
			if (num < (int)p_pickup_cell_armor->value && other->client->cellarmor != 1)
			{
				other->client->cellarmor = 1;
				other->client->loxarmor = 1;
				gi.cprintf(other, PRINT_HIGH, "Cell Armor Picked up\n");
			}
		}
		
		if (!(i_loxarmorban & LAB_SHELLARMOR) && other->client->loxarmor != 1)
		{
			int num = abs((int)(crandom() * 10000.0));
			if (num < (int)p_pickup_shell_armor->value && other->client->shellarmor != 1)
			{
				other->client->shellarmor = 1;
				other->client->loxarmor = 1;
				gi.cprintf(other, PRINT_HIGH, "Shell Armor Picked up\n");
			}
		}
		
		if (!(i_loxarmorban & LAB_BULLETARMOR) && other->client->loxarmor != 1)
		{
			int num = abs((int)(crandom() * 10000.0));
			if (num < (int)p_pickup_bullet_armor->value && other->client->bulletarmor != 1)
			{
				other->client->bulletarmor = 1;
				other->client->loxarmor = 1;
				gi.cprintf(other, PRINT_HIGH, "Bullet Armor Picked up\n");
			}
		}
		
		if (!(i_loxarmorban & LAB_ROCKETARMOR) && other->client->loxarmor != 1)
		{
			int num = abs((int)(crandom() * 10000.0));
			if (num < (int)p_pickup_rocket_armor->value && other->client->rocketarmor != 1)
			{
				other->client->rocketarmor = 1;
				other->client->loxarmor = 1;
				gi.cprintf(other, PRINT_HIGH, "Rocket Armor Picked up\n");
			}
		}
		
		if (!(i_loxarmorban & LAB_GRENADEARMOR) && other->client->loxarmor != 1)
		{
			int num = abs((int)(crandom() * 10000.0));
			if (num < (int)p_pickup_grenade_armor->value && other->client->grenadearmor != 1)
			{
				other->client->grenadearmor = 1;
				other->client->loxarmor = 1;
				gi.cprintf(other, PRINT_HIGH, "Grenade Armor Picked up\n");
			}
		}
		
		other->client->pers.inventory[ITEM_INDEX(ent->item)] = newinfo->base_count;
	}
	
	// use the better armor
	else
	{
		if (!(i_loxarmorban & LAB_ORGANICARMOR) && other->client->loxarmor != 1)
		{
			int num = abs((int)(crandom() * 10000.0));
			if (num < (int)p_pickup_organic_armor->value && other->client->organicarmor == 0)
			{
				other->client->organicarmor = 1;
				other->client->loxarmor = 1;
				gi.cprintf(other, PRINT_HIGH, "Organic Armor Picked up\n");
				
			}
		}
		
		if (!(i_loxarmorban & LAB_SLUGARMOR) && other->client->loxarmor != 1 )
		{
			int num = abs((int)(crandom() * 10000.0));
			if (num < (int)p_pickup_slug_armor->value && other->client->slugarmor != 1)
			{
				other->client->slugarmor = 1;
				other->client->loxarmor = 1;
				gi.cprintf(other, PRINT_HIGH, "Slug Armor Picked up\n");
			}
		}
		
		if (!(i_loxarmorban & LAB_CELLARMOR) && other->client->loxarmor != 1)
		{
			int num = abs((int)(crandom() * 10000.0));
			if (num < (int)p_pickup_cell_armor->value && other->client->cellarmor != 1)
			{
				other->client->cellarmor = 1;
				other->client->loxarmor = 1;
				gi.cprintf(other, PRINT_HIGH, "Cell Armor Picked up\n");
			}
		}
		
		if (!(i_loxarmorban & LAB_SHELLARMOR) && other->client->loxarmor != 1)
		{
			int num = abs((int)(crandom() * 10000.0));
			if (num < (int)p_pickup_shell_armor->value && other->client->shellarmor != 1)
			{
				other->client->shellarmor = 1;
				other->client->loxarmor = 1;
				gi.cprintf(other, PRINT_HIGH, "Shell Armor Picked up\n");
			}
		}
		if (!(i_loxarmorban & LAB_BULLETARMOR) && other->client->loxarmor != 1)
		{
			int num = abs((int)(crandom() * 10000.0));
			if (num < (int)p_pickup_bullet_armor->value && other->client->bulletarmor != 1)
			{
				other->client->bulletarmor = 1;
				other->client->loxarmor = 1;
				gi.cprintf(other, PRINT_HIGH, "Bullet Armor Picked up\n");
			}
		}
		
		if (!(i_loxarmorban & LAB_ROCKETARMOR) && other->client->loxarmor != 1)
		{
			int num = abs((int)(crandom() * 10000.0));
			if (num < (int)p_pickup_rocket_armor->value && other->client->rocketarmor != 1)
			{
				other->client->rocketarmor = 1;
				other->client->loxarmor = 1;
				gi.cprintf(other, PRINT_HIGH, "Rocket Armor Picked up\n");
			}
		}
		
		if (!(i_loxarmorban & LAB_GRENADEARMOR) && other->client->loxarmor != 1)
		{
			int num = abs((int)(crandom() * 10000.0));
			if (num < (int)p_pickup_grenade_armor->value && other->client->grenadearmor != 1)
			{
				other->client->grenadearmor = 1;
				other->client->loxarmor = 1;
				gi.cprintf(other, PRINT_HIGH, "Grenade Armor Picked up\n");
			}
		}
		//		other->client->pers.inventory[ITEM_INDEX(ent->item)] = newinfo->base_count;
		
		// get info on old armor
		if (old_armor_index == jacket_armor_index)
			oldinfo = &jacketarmor_info;
		else if (old_armor_index == combat_armor_index)
			oldinfo = &combatarmor_info;
		else // (old_armor_index == body_armor_index)
			oldinfo = &bodyarmor_info;
		
		if (newinfo->normal_protection > oldinfo->normal_protection)
		{
			// calc new armor values
			salvage = oldinfo->normal_protection / newinfo->normal_protection;
			salvagecount = (int)salvage * other->client->pers.inventory[old_armor_index];
			newcount = newinfo->base_count + salvagecount;
			if (newcount > newinfo->max_count)
				newcount = newinfo->max_count;
			
			// zero count of old armor so it goes away
			other->client->pers.inventory[old_armor_index] = 0;
			
			// change armor to new item with computed value
			other->client->pers.inventory[ITEM_INDEX(ent->item)] = newcount;
		}
		else
		{
			// calc new armor values
			salvage = newinfo->normal_protection / oldinfo->normal_protection;
			salvagecount = (int)salvage * newinfo->base_count;
			newcount = other->client->pers.inventory[old_armor_index] + salvagecount;
			if (newcount > oldinfo->max_count)
				newcount = oldinfo->max_count;
			
			// if we're already maxed out then we don't need the new armor
			if (other->client->pers.inventory[old_armor_index] >= newcount)
				return false;
			
			// update current armor value
			other->client->pers.inventory[old_armor_index] = newcount;
		}
	}
	
	if (!(ent->spawnflags & DROPPED_ITEM) && (deathmatch->value))
		SetRespawn (ent, 20);
	
	return true;
}

//======================================================================

int PowerArmorType (edict_t *ent)
{
	if (!ent->client)
		return POWER_ARMOR_NONE;
	
	if (!(ent->flags & FL_POWER_ARMOR))
		return POWER_ARMOR_NONE;
	
	if (ent->client->pers.inventory[power_shield_index] > 0)
		return POWER_ARMOR_SHIELD;
	
	if (ent->client->pers.inventory[power_screen_index] > 0)
		return POWER_ARMOR_SCREEN;
	
	return POWER_ARMOR_NONE;
}

void Use_PowerArmor (edict_t *ent, gitem_t *item)
{
	int		index;
	
	if (ent->flags & FL_POWER_ARMOR)
	{
		ent->flags &= ~FL_POWER_ARMOR;
		gi.sound(ent, CHAN_AUTO, gi.soundindex("misc/power2.wav"), 1, ATTN_NORM, 0);
	}
	else
	{
		index = ITEM_INDEX(&gI_ammo_cells);
		if (!ent->client->pers.inventory[index])
		{
			gi.cprintf (ent, PRINT_HIGH, "No cells for power armor.\n");
			return;
		}
		ent->flags |= FL_POWER_ARMOR;
		gi.sound(ent, CHAN_AUTO, gi.soundindex("misc/power1.wav"), 1, ATTN_NORM, 0);
	}
}

qboolean Pickup_PowerArmor (edict_t *ent, edict_t *other)
{
	int		quantity;
	
	quantity = other->client->pers.inventory[ITEM_INDEX(ent->item)];
	
	other->client->pers.inventory[ITEM_INDEX(ent->item)]++;
	
	if (deathmatch->value)
	{
		if (!(ent->spawnflags & DROPPED_ITEM) )
			SetRespawn (ent, ent->item->quantity);
		// auto-use for DM only if we didn't already have one
		if (!quantity)
			ent->item->use (other, ent->item);
	}
	
	return true;
}

void Drop_PowerArmor (edict_t *ent, gitem_t *item)
{
	
	if ((ent->flags & FL_POWER_ARMOR) && (ent->client->pers.inventory[ITEM_INDEX(item)] == 1))
		Use_PowerArmor (ent, item);
	
	Drop_General (ent, item);
}

//======================================================================

/*
===============
Touch_Item
===============
*/
// ent is item touched, other is player
void Touch_Item (edict_t *ent, edict_t *other, cplane_t *plane, csurface_t *surf)
{
	qboolean	taken;
	
	if (!other->client)
		return;
	if (other->health < 1)
		return;		// dead people can't pickup
	if (ent->item == NULL)
		return;
	if (!ent->item->pickup)
		return;		// not a grabbable item?
	if (ent->holo == 1 && ent->owner == other)
		return;
	if (ent->holo == 1)
	{
		ent->owner->client->holo = NULL;
		G_FreeEdict (ent);
		return;
	}
	
	taken = ent->item->pickup(ent, other);
	
	if (taken)
	{
		if (ent->item->pickup == Pickup_Health) // we picked it up
		{
			// if valid client and we don't have a sweet item already
			// and if we picked up more than 9 health points then
			// give them a chance to get regen or vampiric health. 
			if (other->client != NULL && other->client->loxhealth == 0 && ent->count > 9)
			{
				if (other->client->loxhealth != 1 
					&& !(i_loxfeatureban & LFB_REGENHEALTH)
					&& other->client->regenhealth == 0 
					&& other->client->vampihealth == 0)
				{
					int num = abs((int)(crandom() * 10000.0));
					//gi.cprintf(other, PRINT_HIGH, "Computed random number for regen: %i\n", num);
					if (num < (int)p_pickup_regen->value && other->client->regenhealth == 0)
					{
						other->client->regenhealth = 1;
						other->client->loxhealth = 1;
						other->max_health = 400;
						gi.cprintf(other, PRINT_HIGH, "Regeneration Health Box Picked up\n");
					}
				}
				if (other->client->loxhealth != 1 
					&& !(i_loxfeatureban & LFB_VAMPIHEALTH)
					&& other->client->vampihealth == 0 
					&& other->client->regenhealth == 0)
				{
					int num = abs((int) (crandom() * 10000.0));
					//gi.cprintf(other, PRINT_HIGH, "Computed random number for vampiric: %i\n", num);
					if (num < (int)p_pickup_vampiric->value && other->client->vampihealth == 0)
					{
						other->client->vampihealth = 1;
						other->client->loxhealth = 1;
						other->max_health = 1000;
						gi.cprintf(other, PRINT_HIGH, "Vampiric Health Box Picked up\n");
					}
				}
			}
		}
		// flash the screen
		other->client->bonus_alpha = 0.25;	
		
		// show icon and name on status bar
		other->client->ps.stats[STAT_PICKUP_ICON] = gi.imageindex(ent->item->icon);
		other->client->ps.stats[STAT_PICKUP_STRING] = CS_ITEMS+ITEM_INDEX(ent->item);
		other->client->pickup_msg_time = level.time + 3.0;
		
		// change selected item
		if (ent->item->use)
			other->client->pers.selected_item = other->client->ps.stats[STAT_SELECTED_ITEM] = ITEM_INDEX(ent->item);
		
		if (ent->item->pickup == Pickup_Health)
		{
			
			if (ent->count == 2)
				gi.sound(other, CHAN_ITEM, gi.soundindex("items/s_health.wav"), 1, ATTN_NORM, 0);
			else if (ent->count == 10)
				gi.sound(other, CHAN_ITEM, gi.soundindex("items/n_health.wav"), 1, ATTN_NORM, 0);
			else if (ent->count == 25)
				gi.sound(other, CHAN_ITEM, gi.soundindex("items/l_health.wav"), 1, ATTN_NORM, 0);
			else // (ent->count == 100)
				gi.sound(other, CHAN_ITEM, gi.soundindex("items/m_health.wav"), 1, ATTN_NORM, 0);
		}
		else if (ent->item->pickup_sound)
		{
			gi.sound(other, CHAN_ITEM, gi.soundindex(ent->item->pickup_sound), 1, ATTN_NORM, 0);
		}
	}
	
	if (!(ent->spawnflags & ITEM_TARGETS_USED))
	{
		G_UseTargets (ent, other);
		ent->spawnflags |= ITEM_TARGETS_USED;
	}
	
	if (!taken)
		return;
	
	if (!((coop->value) &&  (ent->item->flags & IT_STAY_COOP)) || (ent->spawnflags & (DROPPED_ITEM | DROPPED_PLAYER_ITEM)))
	{
		if (ent->flags & FL_RESPAWN)
			ent->flags &= ~FL_RESPAWN;
		else
			G_FreeEdict (ent);
	}
}

//======================================================================

void Drop_Temp_Touch (edict_t *ent, edict_t *other, cplane_t *plane, csurface_t *surf)
{
	if (other == ent->owner)
		return;
	
	Touch_Item (ent, other, plane, surf);
}

static void drop_make_touchable (edict_t *ent)
{
	ent->touch = Touch_Item;
	if (deathmatch->value)
	{
		ent->nextthink = level.time + 30;
		ent->think = G_FreeEdict;
	}
}

edict_t *Drop_Item (edict_t *ent, gitem_t *item)
{
	edict_t	*dropped;
	vec3_t	forward, right;
	vec3_t	offset;
	
	dropped = G_Spawn();
	
	dropped->classname = item->classname;
	dropped->item = item;
	dropped->spawnflags = DROPPED_ITEM;
	dropped->s.effects = item->world_model_flags;
	dropped->s.renderfx = RF_GLOW;
	VectorSet (dropped->mins, -15, -15, -15);
	VectorSet (dropped->maxs, 15, 15, 15);
	gi.setmodel (dropped, dropped->item->world_model);
	dropped->solid = SOLID_TRIGGER;
	dropped->movetype = MOVETYPE_TOSS;  
	dropped->touch = Drop_Temp_Touch;
	dropped->owner = ent;
	
	if (ent->client)
	{
		trace_t	trace;
		
		AngleVectors (ent->client->v_angle, forward, right, NULL);
		VectorSet(offset, 24, 0, -16);
		G_ProjectSource (ent->s.origin, offset, forward, right, dropped->s.origin);
		trace = gi.trace (ent->s.origin, dropped->mins, dropped->maxs,
			dropped->s.origin, ent, CONTENTS_SOLID);
		VectorCopy (trace.endpos, dropped->s.origin);
	}
	else
	{
		AngleVectors (ent->s.angles, forward, right, NULL);
		VectorCopy (ent->s.origin, dropped->s.origin);
	}
	
	VectorScale (forward, 100, dropped->velocity);
	dropped->velocity[2] = 300;
	
	dropped->think = drop_make_touchable;
	dropped->nextthink = level.time + 1;
	
	gi.linkentity (dropped);
	
	return dropped;
}

void Use_Item (edict_t *ent, edict_t *other, edict_t *activator)
{
	ent->svflags &= ~SVF_NOCLIENT;
	ent->use = NULL;
	
	if (ent->spawnflags & ITEM_NO_TOUCH)
	{
		ent->solid = SOLID_BBOX;
		ent->touch = NULL;
	}
	else
	{
		ent->solid = SOLID_TRIGGER;
		ent->touch = Touch_Item;
	}
	
	gi.linkentity (ent);
}

//======================================================================

/*
================
Drop_To_Floor
================
*/
void Drop_To_Floor (edict_t *ent)
{
	trace_t		tr;
	vec3_t		dest;
	float		*v;
	
	v = tv(-15,-15,-15);
	VectorCopy (v, ent->mins);
	v = tv(15,15,15);
	VectorCopy (v, ent->maxs);
	
	if (ent->model)
		gi.setmodel (ent, ent->model);
	else
		gi.setmodel (ent, ent->item->world_model);
	ent->solid = SOLID_TRIGGER;
	ent->movetype = MOVETYPE_TOSS;  
	ent->touch = Touch_Item;
	
	v = tv(0,0,-128);
	VectorAdd (ent->s.origin, v, dest);
	
	tr = gi.trace (ent->s.origin, ent->mins, ent->maxs, dest, ent, MASK_SOLID);
	if (tr.startsolid)
	{
		gi.dprintf ("droptofloor: %s startsolid at %s\n", ent->classname, vtos(ent->s.origin));
		G_FreeEdict (ent);
		return;
	}
	
	VectorCopy (tr.endpos, ent->s.origin);
	
	// Save the item's origin/angles, so they can be restored if it gets
	// knocked around.
	VectorCopy (ent->s.origin, ent->pos1);
	VectorCopy (ent->s.angles, ent->pos2);
	
	if (ent->team)
	{
		ent->flags &= ~FL_TEAMSLAVE;
		ent->chain = ent->teamchain;
		ent->teamchain = NULL;
		
		ent->svflags |= SVF_NOCLIENT;
		ent->solid = SOLID_NOT;
		if (ent == ent->teammaster)
		{
			ent->nextthink = level.time + FRAMETIME;
			ent->think = DoRespawn;
		}
	}
	
	if (ent->spawnflags & ITEM_NO_TOUCH)
	{
		ent->solid = SOLID_BBOX;
		ent->touch = NULL;
		ent->s.effects &= ~EF_ROTATE;
		ent->s.renderfx &= ~RF_GLOW;
	}
	
	if (ent->spawnflags & ITEM_TRIGGER_SPAWN)
	{
		ent->svflags |= SVF_NOCLIENT;
		ent->solid = SOLID_NOT;
		ent->use = Use_Item;
	}
	
	gi.linkentity (ent);
}


/*
===============
PrecacheItem

  Precaches all data needed for a given item.
  This will be called for each item spawned in a level,
  and for each item in each client's inventory.
  ===============
*/
void PrecacheItem (gitem_t *it)
{
	char	*s, *start;
	char	data[MAX_QPATH];
	int		len;
	gitem_t	*ammo;
	
	if (!it)
		return;
	
	if (it->pickup_sound)
		gi.soundindex (it->pickup_sound);
	if (it->world_model)
		gi.modelindex (it->world_model);
	if (it->view_model)
		gi.modelindex (it->view_model);
	if (it->icon)
		gi.imageindex (it->icon);
	
	// parse everything for its ammo
	if (it->ammo)
	{
		ammo = it->ammo;
		if (ammo != it)
			PrecacheItem (ammo);
	}
	
	// parse the space seperated precache string for other items
	s = it->precaches;
	if (!s || !s[0])
		return;
	
	while (*s)
	{
		start = s;
		while (*s && *s != ' ')
			s++;
		
		len = s-start;
		if (len >= MAX_QPATH - 1 || len < 5)
			gi.error ("PrecacheItem: %s has bad precache string", it->classname);
		memcpy (data, start, len);
		data[len] = 0;
		if (*s)
			s++;
		
		// determine type based on extension
		if (!strcmp(data+len-3, "md2"))
			gi.modelindex (data);
		else if (!strcmp(data+len-3, "sp2"))
			gi.modelindex (data);
		else if (!strcmp(data+len-3, "wav"))
			gi.soundindex (data);
		if (!strcmp(data+len-3, "pcx"))
			gi.imageindex (data);
	}
}

/*
============
SpawnItem

  Sets the clipping size and plants the object on the floor.
  
	Items can't be immediately dropped to floor, because they might
	be on an entity that hasn't spawned yet.
	============
*/
void SpawnItem (edict_t *ent, gitem_t *item)
{

	UpdateBans();	//make sure all the i_vars are current
	
	// Some items have been banned.
	if (item == &gI_item_bandolier
		&& (i_featureban & FB_BANDOLIER))
	{
		G_FreeEdict (ent);
		return;
	}
	else if (item == &gI_item_pack
		&& (i_featureban & FB_AMMOPACK))
	{
		G_FreeEdict (ent);
		return;
	}
	else if (item == &gI_item_silencer
		&& (i_featureban & FB_SILENCER))
	{
		G_FreeEdict (ent);
		return;
	}
	else if (item == &gI_item_breather
		&& (i_featureban & FB_REBREATHER))
	{
		G_FreeEdict (ent);
		return;
	}
	else if (item == &gI_item_enviro
		&& (i_featureban & FB_ENVIROSUIT))
	{
		G_FreeEdict (ent);
		return;
	}
	else if (item == &gI_item_adrenaline
		&& (i_featureban & FB_ADRENALINE))
	{
		G_FreeEdict (ent);
		return;
	}
	else if (item == &gI_item_health && ent->count == 100
		&& (i_featureban & FB_MEGAHEALTH))
	{
		G_FreeEdict (ent);
		return;
	}
	else if ((item == &gI_item_power_screen || item == &gI_item_power_shield)
		&& (i_featureban & FB_POWERARMOR))
	{
		G_FreeEdict (ent);
		return;
	}
	else if (item == &gI_item_invulnerability
		&& (i_featureban & FB_INVULNERABILITY))
	{
		G_FreeEdict (ent);
		return;
	}
	else if (item == &gI_item_jetpack
		&& (i_featureban & FB_JETPACK))
	{
		G_FreeEdict (ent);
		return;
	}
	else if (item == &gI_weapon_shotgun
		&& (i_weaponban & WB_SHOTGUN)
		&& (i_loxweaponban & LWB_EXPLOSIVESHOTGUN)
		&& (i_weaponban & WB_SNIPERGUN)
		&& (i_loxweaponban & LWB_AIRFIST)
		&& (i_loxweaponban & LWB_SWORD))
	{
		G_FreeEdict (ent);
		return;
	}
	else if (item == &gI_weapon_supershotgun
		&& (i_weaponban & WB_SUPERSHOTGUN)
		&& (i_weaponban & WB_FREEZEGUN)
		&& (i_loxweaponban & LWB_EXPLOSIVESUPERSHOTGUN)
		&& (i_loxweaponban & LWB_SUPERRAILSHOTGUN))
	{
		G_FreeEdict (ent);
		return;
	}
	else if (item == &gI_weapon_machinegun
		&& (i_weaponban & WB_MACHINEGUN)
		&& (i_weaponban & WB_MACHINEROCKETGUN)
		&& (i_weaponban & WB_BURSTMACHINEGUN)
		&& (i_loxweaponban & LWB_PULSERIFLE)
		&& (i_loxweaponban & LWB_NAILGUN)
		&& (i_loxweaponban & LWB_EXPLOSIVEMACHINEGUN))
	{
		G_FreeEdict (ent);
		return;
	}
	else if (item == &gI_weapon_chaingun
		&& (i_weaponban & WB_CHAINGUN)
		&& (i_weaponban & WB_STREETSWEEPER)
		&& (i_loxsweeperban & LSB_SUPERBLASTERSWEEPER)
		&& (i_loxsweeperban & LSB_FREEZERSWEEPER)
		&& (i_loxsweeperban & LSB_GRENADESWEEPER)
		&& (i_loxsweeperban & LSB_BAZOOKASWEEPER)
		&& (i_loxsweeperban & LSB_RAILGUNSWEEPER)
		&& (i_loxsweeperban & LSB_CHUNKGUN)
		&& (i_loxsweeperban & LSB_FLAMESWEEPER)
		&& (i_loxsweeperban & LSB_SNIPERSWEEPER)
		&& (i_loxsweeperban & LSB_EXPLOSIVECHAINGUN)
		&& (i_loxsweeperban & LSB_EXPLOSIVESTREETSWEEPER)
		&& (i_loxsweeperban & LSB_STICKYGRENADESWEEPER))
	{
		G_FreeEdict (ent);
		return;
	}
	else if (item == &gI_weapon_grenadelauncher
		&& ((i_weaponban & WB_GRENADE)
		|| ((i_weaponban & WB_GRENADELAUNCHER)
		&& (i_weaponban & WB_BAZOOKA) 
		&& (i_loxweaponban & LWB_BUCKY)
		&& (i_loxweaponban & LWB_STICKGRENADELAUNCHER)
		&& (i_loxweaponban & LWB_DOUBLEIMPACT)
		&& (i_loxweaponban & LWB_FLOATINGMINELAUNCHER) )))
	{
		G_FreeEdict (ent);
		return;
	}
	else if (item == &gI_weapon_rocketlauncher
		&& (i_weaponban & WB_ROCKETLAUNCHER)
		&& (i_weaponban & WB_HOMINGROCKETLAUNCHER)
		&& (i_loxweaponban & LWB_SKIPPERHOMINGROCKET)
		&& (i_loxweaponban & LWB_GUIDEDMISSILE)
		&& (i_loxweaponban & LWB_SPIRALROCKETLAUNCHER)
		&& (i_loxweaponban & LWB_GUIDEDNUKE))
	{
		G_FreeEdict (ent);
		return;
	}
	else if (item == &gI_weapon_hyperblaster
		&& (i_weaponban & WB_HYPERBLASTER)
		&& (i_weaponban & WB_PLASMARIFLE)
		&& (i_loxweaponban & LWB_POSITRON)
		&& (i_loxweaponban & LWB_FREEZATRON)
		&& (i_loxweaponban & LWB_LIGHTNINGGUN)
		&& (i_loxweaponban & LWB_ICECUBEGUN))
	{
		G_FreeEdict (ent);
		return;
	}
	else if (item == &gI_weapon_railgun
		&& (i_weaponban & WB_RAILGUN)
		&& (i_weaponban & WB_FLAMETHROWER)
		&& (i_loxweaponban & LWB_DURG)
		&& (i_loxweaponban & LWB_SONICRAILGUN)
		&& (i_loxweaponban & LWB_KAMINIT)
		&& (i_loxweaponban & LWB_WALLPIERCINGRAILGUN)
		&& (i_loxweaponban & LWB_ANTIMATTERCANNON)
		&& (i_loxweaponban & LWB_NUKE))
	{
		G_FreeEdict (ent);
		return;
	}
	else if (item == &gI_weapon_bfg
		&& (i_weaponban & WB_BFG10K)
		&& (i_loxweaponban & LWB_FBFG)
		&& (i_loxweaponban & LWB_VACUUMMAKER)
		&& (i_loxweaponban & LWB_ENERGYVORTEX))
	{
		G_FreeEdict (ent);
		return;
	}
	else if (item == &gI_ammo_shells
		&& (i_weaponban & WB_SHOTGUN)
		&& (i_weaponban & WB_SNIPERGUN)
		&& (i_weaponban & WB_SUPERSHOTGUN)
		&& (i_weaponban & WB_STREETSWEEPER))
	{
		G_FreeEdict (ent);
		return;
	}
	else if (item == &gI_ammo_bullets
		&& (i_weaponban & WB_MACHINEGUN)
		&& (i_weaponban & WB_MACHINEROCKETGUN)
		&& (i_weaponban & WB_BURSTMACHINEGUN)
		&& (i_weaponban & WB_CHAINGUN))
	{
		G_FreeEdict (ent);
		return;
	}
	else if (item == &gI_ammo_grenades
		&& (i_weaponban & WB_GRENADE))
	{
		G_FreeEdict (ent);
		return;
	}
	else if (item == &gI_ammo_cells
		&& (i_weaponban & WB_SUPERBLASTER)
		&& (i_weaponban & WB_FREEZEGUN)
		&& (i_weaponban & WB_HYPERBLASTER)
		&& (i_weaponban & WB_PLASMARIFLE)
		&& (i_weaponban & WB_BFG10K)
		&& (i_featureban & FB_TRIPLASER))
	{
		G_FreeEdict (ent);
		return;
	}
	else if (item == &gI_ammo_rockets
		&& (i_weaponban & WB_ROCKETLAUNCHER)
		&& (i_weaponban & WB_HOMINGROCKETLAUNCHER))
	{
		G_FreeEdict (ent);
		return;
	}
	else if (item == &gI_ammo_slugs
		&& (i_weaponban & WB_FLAMETHROWER)
		&& (i_weaponban & WB_RAILGUN)
		&& (i_loxweaponban & LWB_DURG))
	{
		G_FreeEdict (ent);
		return;
	}
	
	if (ent->spawnflags)
	{
		if (strcmp(ent->classname, "key_power_cube") != 0)
		{
			ent->spawnflags = 0;
			gi.dprintf("%s at %s has invalid spawnflags set\n", ent->classname, vtos(ent->s.origin));
		}
	}
	
	// some items will be prevented in deathmatch
	if (deathmatch->value)
	{
		if ( (int)dmflags->value & DF_NO_ARMOR )
		{
			if (item->pickup == Pickup_Armor || item->pickup == Pickup_PowerArmor)
			{
				G_FreeEdict (ent);
				return;
			}
		}
		if ( (int)dmflags->value & DF_NO_ITEMS )
		{
			if (item->pickup == Pickup_Powerup)
			{
				G_FreeEdict (ent);
				return;
			}
		}
		if ( (int)dmflags->value & DF_NO_HEALTH )
		{
			if (item->pickup == Pickup_Health || item->pickup == Pickup_Adrenaline || item->pickup == Pickup_AncientHead)
			{
				G_FreeEdict (ent);
				return;
			}
		}
		if ( (int)dmflags->value & DF_INFINITE_AMMO )
		{
			if  (item->flags == IT_AMMO)
			{
				G_FreeEdict (ent);
				return;
			}
		}
	}
	
	PrecacheItem (item);
	
	if (coop->value && (strcmp(ent->classname, "key_power_cube") == 0))
	{
		ent->spawnflags |= (1 << (8 + level.power_cubes));
		level.power_cubes++;
	}
	
	// don't let them drop items that stay in a coop game
	if ((coop->value) && (item->flags & IT_STAY_COOP))
	{
		item->drop = NULL;
	}
	
	ent->item = item;
	ent->nextthink = level.time + 2 * FRAMETIME;    // items start after other solids
	ent->think = Drop_To_Floor;
	ent->s.effects = item->world_model_flags;
	ent->s.renderfx = RF_GLOW;
	if (ent->model)
		gi.modelindex (ent->model);
}

//======================================================================

gitem_t gI_item_null1 =
{
	NULL
};	// leave index 0 alone

//
// ARMOR
//

/*QUAKED item_armor_body (.3 .3 1) (-16 -16 -16) (16 16 16)
*/
gitem_t gI_item_armor_body =
{
	"item_armor_body", 
		Pickup_Armor,
		NULL,
		NULL,
		NULL,
		"misc/ar1_pkup.wav",
		"models/items/armor/body/tris.md2", EF_ROTATE,
		NULL,
		/* icon */		"i_bodyarmor",
		/* pickup */	"Body Armor",
		/* width */		3,
		0,
		NULL,
		IT_ARMOR,
		&bodyarmor_info,
		ARMOR_BODY,
		/* precache */ ""
};

/*QUAKED item_armor_combat (.3 .3 1) (-16 -16 -16) (16 16 16)
*/
gitem_t gI_item_armor_combat =
{
	"item_armor_combat", 
		Pickup_Armor,
		NULL,
		NULL,
		NULL,
		"misc/ar1_pkup.wav",
		"models/items/armor/combat/tris.md2", EF_ROTATE,
		NULL,
		/* icon */		"i_combatarmor",
		/* pickup */	"Combat Armor",
		/* width */		3,
		0,
		NULL,
		IT_ARMOR,
		&combatarmor_info,
		ARMOR_COMBAT,
		/* precache */ ""
};

/*QUAKED item_armor_jacket (.3 .3 1) (-16 -16 -16) (16 16 16)
*/
gitem_t gI_item_armor_jacket =
{
	"item_armor_jacket", 
		Pickup_Armor,
		NULL,
		NULL,
		NULL,
		"misc/ar1_pkup.wav",
		"models/items/armor/jacket/tris.md2", EF_ROTATE,
		NULL,
		/* icon */		"i_jacketarmor",
		/* pickup */	"Jacket Armor",
		/* width */		3,
		0,
		NULL,
		IT_ARMOR,
		&jacketarmor_info,
		ARMOR_JACKET,
		/* precache */ ""
};

/*QUAKED item_armor_shard (.3 .3 1) (-16 -16 -16) (16 16 16)
*/
gitem_t gI_item_armor_shard =
{
	"item_armor_shard", 
		Pickup_Armor,
		NULL,
		NULL,
		NULL,
		"misc/ar2_pkup.wav",
		"models/items/armor/shard/tris.md2", EF_ROTATE,
		NULL,
		/* icon */		"i_jacketarmor",
		/* pickup */	"Armor Shard",
		/* width */		3,
		0,
		NULL,
		IT_ARMOR,
		NULL,
		ARMOR_SHARD,
		/* precache */ ""
};


/*QUAKED item_power_screen (.3 .3 1) (-16 -16 -16) (16 16 16)
*/
gitem_t gI_item_power_screen =
{
	"item_power_screen", 
		Pickup_PowerArmor,
		Use_PowerArmor,
		Drop_PowerArmor,
		NULL,
		"misc/ar3_pkup.wav",
		"models/items/armor/screen/tris.md2", EF_ROTATE,
		NULL,
		/* icon */		"i_powerscreen",
		/* pickup */	"Power Screen",
		/* width */		0,
		60,
		NULL,
		IT_ARMOR,
		NULL,
		0,
		/* precache */ ""
};

/*QUAKED item_power_shield (.3 .3 1) (-16 -16 -16) (16 16 16)
*/
gitem_t gI_item_power_shield =
{
	"item_power_shield",
		Pickup_PowerArmor,
		Use_PowerArmor,
		Drop_PowerArmor,
		NULL,
		"misc/ar3_pkup.wav",
		"models/items/armor/shield/tris.md2", EF_ROTATE,
		NULL,
		/* icon */		"i_powershield",
		/* pickup */	"Power Shield",
		/* width */		0,
		60,
		NULL,
		IT_ARMOR,
		NULL,
		0,
		/* precache */ "misc/power2.wav misc/power1.wav"
};


//
// WEAPONS 
//

/* weapon_blaster (.3 .3 1) (-16 -16 -16) (16 16 16)
always owned, never in the world
*/
gitem_t gI_weapon_blaster =
{
	"weapon_blaster", 
		NULL,
		Use_Weapon,
		NULL,
		Weapon_Blaster,
		"misc/w_pkup.wav",
		NULL, 0,
		"models/weapons/v_blast/tris.md2",
		/* icon */		"w_blaster",
		/* pickup */	"Blaster",
		0,
		0,
		NULL,
		IT_WEAPON|IT_STAY_COOP,
		NULL,
		0,
		/* precache */ "weapons/blastf1a.wav misc/lasfly.wav"
};

/* weapon_blaster (.3 .3 1) (-16 -16 -16) (16 16 16)
always owned, never in the world
*/
gitem_t gI_weapon_mace =
{
	"weapon_mace", 
		NULL,
		Use_Weapon,
		NULL,
		Weapon_Mace,
		"misc/w_pkup.wav",
		NULL, 0,
		"models/weapons/v_blast/tris.md2",
		/* icon */		"w_blaster",
		/* pickup */	"Mace",
		0,
		0,
		NULL,
		IT_WEAPON|IT_STAY_COOP,
		NULL,
		0,
		/* precache */ "weapons/blastf1a.wav misc/lasfly.wav"
};

/* weapon_flaregun (.3 .3 1) (-16 -16 -16) (16 16 16)
always owned, never in the world
*/
gitem_t gI_weapon_flaregun	= {
	"weapon_flaregun", 
		Pickup_Weapon,
		Use_Weapon,
		Drop_Weapon,
		Weapon_Flaregun,
		"misc/w_pkup.wav",
		"models/weapons/g_blast/tris.md2", EF_ROTATE,
		"models/weapons/v_blast/tris.md2",
		/* icon */		"w_blaster",
		/* pickup */    "Flaregun",
		0,
		0,
		NULL,
		IT_WEAPON|IT_STAY_COOP,
		NULL,
		0,
		/* precache */ "weapons/blastf1a.wav misc/lasfly.wav"
};

/* weapon_antiflaregun (.3 .3 1) (-16 -16 -16) (16 16 16)
always owned, never in the world
*/
gitem_t gI_weapon_antiflaregun	= {
	"weapon_antiflaregun", 
		Pickup_Weapon,
		Use_Weapon,
		Drop_Weapon,
		Weapon_AntiFlaregun,
		"misc/w_pkup.wav",
		"models/weapons/g_blast/tris.md2", EF_ROTATE,
		"models/weapons/v_blast/tris.md2",
		/* icon */		"w_blaster",
		/* pickup */    "Anti-Flaregun",
		0,
		0,
		NULL,
		IT_WEAPON|IT_STAY_COOP,
		NULL,
		0,
		/* precache */ "weapons/blastf1a.wav misc/lasfly.wav"
};


/*QUAKED weapon_airfist (.3 .3 1) (-16 -16 -16) (16 16 16)
*/
gitem_t gI_weapon_airfist = 
{ 
	"weapon_airfist", 
		NULL,
		Use_Weapon,
		NULL,
		Weapon_Airfist,
		"misc/w_pkup.wav",
		"models/weapons/g_shotg/tris.md2", EF_ROTATE,
		"models/weapons/v_shotg/tris.md2",
		/* icon */		"w_shotgun",
		/* pickup */	"Airfist",
		0,
		1,
		&gI_ammo_shells,
		IT_WEAPON|IT_STAY_COOP,
		NULL,
		0,
		/* precache */ "weapons/shotgf1b.wav weapons/shotgr1b.wav"
};

/*QUAKED weapon_shotgun (.3 .3 1) (-16 -16 -16) (16 16 16)
*/
gitem_t gI_weapon_shotgun =
{
	"weapon_shotgun", 
		Pickup_Weapon,
		Use_Weapon,
		Drop_Weapon,
		Weapon_Shotgun,
		"misc/w_pkup.wav",
		"models/weapons/g_shotg/tris.md2", EF_ROTATE,
		"models/weapons/v_shotg/tris.md2",
		/* icon */		"w_shotgun",
		/* pickup */	"Shotgun",
		0,
		1,
		&gI_ammo_shells,
		IT_WEAPON|IT_STAY_COOP,
		NULL,
		0,
		/* precache */ "weapons/shotgf1b.wav weapons/shotgr1b.wav"
};

gitem_t gI_weapon_explosiveshotgun =
{
	"weapon_explosiveshotgun", 
		Pickup_Weapon,
		Use_Weapon,
		Drop_Weapon,
		Weapon_ExplosiveShotgun,
		"misc/w_pkup.wav",
		"models/weapons/g_shotg/tris.md2", EF_ROTATE,
		"models/weapons/v_shotg/tris.md2",
		/* icon */		"w_shotgun",
		/* pickup */	"Explosive Shotgun",
		0,
		1,
		&gI_ammo_shells,
		IT_WEAPON|IT_STAY_COOP,
		NULL,
		0,
		/* precache */ "weapons/shotgf1b.wav weapons/shotgr1b.wav"
};

gitem_t gI_weapon_sword = 
{		
	"weapon_sword",
		NULL,
		Use_Weapon,                             //How to use
		NULL,
		Weapon_Sword,                           //What the function is
		"misc/w_pkup.wav",
		"models/weapons/g_sword/tris.md2",EF_ROTATE,
		"models/weapons/v_sword/tris.md2",      //The models stuff
		"w_blaster",                                    //Icon to be used
		"Sword",                                        //Pickup name
		0,		
		0,
		NULL,
		IT_WEAPON,
		NULL,
		0,
		"models/weapons/v_sword/tris.md2 models/weapons/g_sword/tris.md2 weapons/blastf1a.wav misc/lasfly.wav" //The sound of the blaster
								//This is precached
};

/*QUAKED weapon_supershotgun (.3 .3 1) (-16 -16 -16) (16 16 16)
*/
gitem_t gI_weapon_supershotgun =
{
	"weapon_supershotgun", 
		Pickup_Weapon,
		Use_Weapon,
		Drop_Weapon,
		Weapon_SuperShotgun,
		"misc/w_pkup.wav",
		"models/weapons/g_shotg2/tris.md2", EF_ROTATE,
		"models/weapons/v_shotg2/tris.md2",
		/* icon */		"w_sshotgun",
		/* pickup */	"Super Shotgun",
		0,
		2,
		&gI_ammo_shells,
		IT_WEAPON|IT_STAY_COOP,
		NULL,
		0,
		/* precache */ "weapons/sshotf1b.wav"
};


/*QUAKED weapon_supershotgun (.3 .3 1) (-16 -16 -16) (16 16 16)
*/
gitem_t gI_weapon_superrailshotgun =
{
	"weapon_superrailshotgun", 
		Pickup_Weapon,
		Use_Weapon,
		Drop_Weapon,
		Weapon_SuperRailShotgun,
		"misc/w_pkup.wav",
		"models/weapons/g_shotg2/tris.md2", EF_ROTATE,
		"models/weapons/v_shotg2/tris.md2",
		/* icon */		"w_sshotgun",
		/* pickup */	"Super Rail Shotgun",
		0,
		10, //ammo cost
		&gI_ammo_slugs,
		IT_WEAPON|IT_STAY_COOP,
		NULL,
		0,
		/* precache */ "weapons/sshotf1b.wav"
};

gitem_t gI_weapon_explosivesupershotgun =
{
	"weapon_explosivesupershotgun", 
		Pickup_Weapon,
		Use_Weapon,
		Drop_Weapon,
		Weapon_ExplosiveSuperShotgun,
		"misc/w_pkup.wav",
		"models/weapons/g_shotg2/tris.md2", EF_ROTATE,
		"models/weapons/v_shotg2/tris.md2",
		/* icon */		"w_sshotgun",
		/* pickup */	"Explosive Super Shotgun",
		0,
		2,
		&gI_ammo_shells,
		IT_WEAPON|IT_STAY_COOP,
		NULL,
		0,
		/* precache */ "weapons/sshotf1b.wav"
};

/*gitem_t gI_weapon_egggun = {
"weapon_egggun", 
Pickup_Weapon,
Use_Weapon,
Drop_Weapon,
Weapon_Egggun,
"misc/w_pkup.wav",
NULL, 0,
"models/weapons/v_egggun/tris.md2",
"models/weapons/w_egggun",
"Egggun",
0,
0,
NULL,
IT_WEAPON|IT_STAY_COOP,
NULL,
0,
"" //precache
};*/

/*QUAKED weapon_machine (.3 .3 1) (-16 -16 -16) (16 16 16)
*/
gitem_t gI_weapon_machine =
{
	"weapon_machine", 
		Pickup_Weapon,
		Use_Weapon,
		Drop_Weapon,
		Weapon_Machine,
		"misc/w_pkup.wav",
		"models/weapons/g_machn/tris.md2", EF_ROTATE,
		"models/weapons/v_machn/tris.md2",
		/* icon */		"w_machinegun",
		/* pickup */	"Standard Machinegun",
		0,
		1,
		&gI_ammo_bullets,
		IT_WEAPON|IT_ALTWEAPON|IT_STAY_COOP,
		NULL,
		0,
		/* precache */ "weapons/machgf1b.wav weapons/machgf2b.wav weapons/machgf3b.wav weapons/machgf4b.wav weapons/machgf5b.wav"
};

/*QUAKED weapon_machine (.3 .3 1) (-16 -16 -16) (16 16 16)
*/
gitem_t gI_weapon_machinerocketgun =
{
	"weapon_machinerocketgun", 
		Pickup_Weapon,
		Use_Weapon,
		Drop_Weapon,
		Weapon_Machine,
		"misc/w_pkup.wav",
		"models/weapons/g_machn/tris.md2", EF_ROTATE,
		"models/weapons/v_machn/tris.md2",
		/* icon */		"w_machinegun",
		/* pickup */	"Machine Rocket Gun",
		0,
		1,
		&gI_ammo_bullets,
		IT_WEAPON|IT_ALTWEAPON|IT_STAY_COOP,
		NULL,
		0,
		/* precache */ "weapons/machgf1b.wav weapons/machgf2b.wav weapons/machgf3b.wav weapons/machgf4b.wav weapons/machgf5b.wav"
};

/*QUAKED weapon_nailgun (.3 .3 1) (-16 -16 -16) (16 16 16)
*/
gitem_t gI_weapon_nailgun =	{
	"weapon_nailgun", 
		Pickup_Weapon,
		Use_Weapon,
		Drop_Weapon,
        Weapon_Nailgun,
		"misc/w_pkup.wav",
		"models/weapons/g_machn/tris.md2", EF_ROTATE,
		"models/weapons/v_machn/tris.md2",
		/* icon */		"w_machinegun",
		/* pickup */    "Nailgun",
		0,
		1,
		&gI_ammo_bullets,
		IT_WEAPON|IT_ALTWEAPON|IT_STAY_COOP,
		NULL,
		0,
		/* precache */ "models/nail/tris.md2 weapons/machgf1b.wav weapons/machgf2b.wav weapons/machgf3b.wav weapons/machgf4b.wav weapons/machgf5b.wav",
};

/*QUAKED weapon_explosivemachinegun (.3 .3 1) (-16 -16 -16) (16 16 16)
*/
gitem_t gI_weapon_explosivemachinegun =	{
	"weapon_explosivemachinegun", 
		Pickup_Weapon,
		Use_Weapon,
		Drop_Weapon,
        Weapon_ExplosiveMachingun,
		"misc/w_pkup.wav",
		"models/weapons/g_machn/tris.md2", EF_ROTATE,
		"models/weapons/v_machn/tris.md2",
		/* icon */		"w_machinegun",
		/* pickup */    "Explosive Machinegun",
		0,
		1,
		&gI_ammo_bullets,
		IT_WEAPON|IT_ALTWEAPON|IT_STAY_COOP,
		NULL,
		0,
		/* precache */ "weapons/machgf1b.wav weapons/machgf2b.wav weapons/machgf3b.wav weapons/machgf4b.wav weapons/machgf5b.wav",
		
		
};
//Wyrm: new weapon from rogue
/*QUAKED weapon_pulserifle (.3 .3 1) (-16 -16 -16) (16 16 16)
*/
gitem_t gI_weapon_pulserifle =
{
	"weapon_pulserifle", 
		Pickup_Weapon,
		Use_Weapon,
		Drop_Weapon,
		Weapon_PulseRifle,
		"misc/w_pkup.wav",
		//"models/weapons/g_machn/tris.md2", EF_ROTATE,
		//"models/weapons/v_machn/tris.md2",
		"models/weapons/g_buzzsw/tris.md2", EF_ROTATE,
		"models/weapons/v_buzzsw/tris.md2",
		/* icon */		"w_machinegun",
		/* pickup */	"Pulserifle",
		0,
		2,
		&gI_ammo_cells,
		IT_WEAPON|IT_STAY_COOP,
		NULL,
		0,
		/* precache */ "weapons/machgf1b.wav weapons/machgf2b.wav weapons/machgf3b.wav weapons/machgf4b.wav weapons/machgf5b.wav weapons/pulse1.wav",
};

/*QUAKED weapon_machinegun (.3 .3 1) (-16 -16 -16) (16 16 16)
*/
gitem_t gI_weapon_machinegun =
{
	"weapon_machinegun", 
		Pickup_Weapon,
		Use_Weapon,
		Drop_Weapon,
		Weapon_Machinegun,
		"misc/w_pkup.wav",
		"models/weapons/g_machn/tris.md2", EF_ROTATE,
		"models/weapons/v_machn/tris.md2",
		/* icon */		"w_machinegun",
		/* pickup */	"Machinegun",
		0,
		2,
		&gI_ammo_bullets,
		IT_WEAPON|IT_STAY_COOP,
		NULL,
		0,
		/* precache */ "weapons/machgf1b.wav weapons/machgf2b.wav weapons/machgf3b.wav weapons/machgf4b.wav weapons/machgf5b.wav"
};

/*QUAKED weapon_freezer (.3 .3 1) (-16 -16 -16) (16 16 16)
*/
gitem_t gI_weapon_freezer =
{
	"weapon_freezer", 
		Pickup_Weapon,
		Use_Weapon,
		Drop_Weapon,
		Weapon_Freezer,
		"misc/w_pkup.wav",
		"models/weapons/g_shotg2/tris.md2", EF_ROTATE,
		"models/weapons/v_shotg2/tris.md2",
		/* icon */		"w_sshotgun",
		/* pickup */	"Freezer",
		0,
		10,
		&gI_ammo_cells,
		IT_WEAPON|IT_ALTWEAPON|IT_STAY_COOP,
		NULL,
		0,
		/* precache */ "weapons/blastf1a.wav misc/lasfly.wav sound/weapons/frozen.wav"
};


/*QUAKED weapon_chaingun (.3 .3 1) (-16 -16 -16) (16 16 16)
*/
gitem_t gI_weapon_chaingun =
{
	"weapon_chaingun", 
		Pickup_Weapon,
		Use_Weapon,
		Drop_Weapon,
		Weapon_Chaingun,
		"misc/w_pkup.wav",
		"models/weapons/g_chain/tris.md2", EF_ROTATE,
		"models/weapons/v_chain/tris.md2",
		/* icon */		"w_chaingun",
		/* pickup */	"Chaingun",
		0,
		1,
		&gI_ammo_bullets,
		IT_WEAPON|IT_STAY_COOP,
		NULL,
		0,
		/* precache */ "weapons/chngnu1a.wav weapons/chngnl1a.wav weapons/machgf3b.wav` weapons/chngnd1a.wav"
};

gitem_t gI_weapon_explosivechaingun =
{
	"weapon_explosivechaingun", 
		Pickup_Weapon,
		Use_Weapon,
		Drop_Weapon,
		Weapon_ExplosiveChaingun,
		"misc/w_pkup.wav",
		"models/weapons/g_chain/tris.md2", EF_ROTATE,
		"models/weapons/v_chain/tris.md2",
		/* icon */		"w_chaingun",
		/* pickup */	"Explosive Chaingun",
		0,
		1,
		&gI_ammo_bullets,
		IT_WEAPON|IT_STAY_COOP,
		NULL,
		0,
		/* precache */ "weapons/chngnu1a.wav weapons/chngnl1a.wav weapons/machgf3b.wav` weapons/chngnd1a.wav"
};


/*QUAKED weapon_streetsweeper (.3 .3 1) (-16 -16 -16) (16 16 16)
*/
gitem_t gI_weapon_streetsweeper =
{
	"weapon_streetsweeper", 
		Pickup_Weapon,
		Use_Weapon,
		Drop_Weapon,
		Weapon_Streetsweeper,
		"misc/w_pkup.wav",
		"models/weapons/g_chain/tris.md2", EF_ROTATE,
		"models/weapons/v_chain/tris.md2",
		/* icon */		"w_chaingun",
		/* pickup */	"Streetsweeper",
		0,
		1,
		&gI_ammo_shells,
		IT_WEAPON|IT_ALTWEAPON|IT_STAY_COOP,
		NULL,
		0,
		/* precache */ "weapons/chngnu1a.wav weapons/shotgf1b.wav weapons/shotgr1b.wav weapons/chngnd1a.wav"
};
/*QUAKED weapon_streetsweeper (.3 .3 1) (-16 -16 -16) (16 16 16)
*/
gitem_t gI_weapon_explosivestreetsweeper =
{
	"weapon_explosivestreetsweeper", 
		Pickup_Weapon,
		Use_Weapon,
		Drop_Weapon,
		Weapon_ExplosiveStreetsweeper,
		"misc/w_pkup.wav",
		"models/weapons/g_chain/tris.md2", EF_ROTATE,
		"models/weapons/v_chain/tris.md2",
		/* icon */		"w_chaingun",
		/* pickup */	"Explosive Streetsweeper",
		0,
		1,
		&gI_ammo_shells,
		IT_WEAPON|IT_ALTWEAPON|IT_STAY_COOP,
		NULL,
		0,
		/* precache */ "weapons/chngnu1a.wav weapons/shotgf1b.wav weapons/shotgr1b.wav weapons/chngnd1a.wav"
};

/*QUAKED weapon_streetsweeper (.3 .3 1) (-16 -16 -16) (16 16 16)
*/
gitem_t gI_weapon_stickygrenadesweeper =
{
	"weapon_stickygrenadesweeper", 
		Pickup_Weapon,
		Use_Weapon,
		Drop_Weapon,
		Weapon_StickyGrenadesweeper,
		"misc/w_pkup.wav",
		"models/weapons/g_chain/tris.md2", EF_ROTATE,
		"models/weapons/v_chain/tris.md2",
		/* icon */		"w_chaingun",
		/* pickup */	"Sticky Grenadesweeper",
		0,
		1,
		&gI_ammo_grenades,
		IT_WEAPON|IT_ALTWEAPON|IT_STAY_COOP,
		NULL,
		0,
		/* precache */ "weapons/chngnu1a.wav weapons/shotgf1b.wav weapons/shotgr1b.wav weapons/chngnd1a.wav"
};


/*QUAKED weapon_superblastersweeper (.3 .3 1) (-16 -16 -16) (16 16 16)
*/
gitem_t gI_weapon_superblastersweeper =
{
	"weapon_superblastersweeper", 
		Pickup_Weapon,
		Use_Weapon,
		Drop_Weapon,
		Weapon_SuperBlastersweeper,
		"misc/w_pkup.wav",
		"models/weapons/g_chain/tris.md2", EF_ROTATE,
		"models/weapons/v_chain/tris.md2",
		/* icon */		"w_chaingun",
		/* pickup */	"SuperBlastersweeper",
		0,
		1,
		&gI_ammo_cells,
		IT_WEAPON|IT_ALTWEAPON|IT_STAY_COOP,
		NULL,
		0,
		/* precache */ "weapons/chngnu1a.wav weapons/shotgf1b.wav weapons/shotgr1b.wav weapons/chngnd1a.wav"
};

gitem_t gI_weapon_grenadesweeper =
{
	"weapon_grenadesweeper", 
		Pickup_Weapon,
		Use_Weapon,
		Drop_Weapon,
		Weapon_Grenadesweeper,
		"misc/w_pkup.wav",
		"models/weapons/g_chain/tris.md2", EF_ROTATE,
		"models/weapons/v_chain/tris.md2",
		/* icon */		"w_chaingun",
		/* pickup */	"Grenadesweeper",
		0,
		1,
		&gI_ammo_grenades,
		IT_WEAPON|IT_ALTWEAPON|IT_STAY_COOP,
		NULL,
		0,
		/* precache */ "weapons/chngnu1a.wav weapons/shotgf1b.wav weapons/shotgr1b.wav weapons/chngnd1a.wav"
};

gitem_t gI_weapon_chunkgun =
{
	"weapon_chunkgun", 
		Pickup_Weapon,
		Use_Weapon,
		Drop_Weapon,
		Weapon_Chunkgun,
		"misc/w_pkup.wav",
		"models/weapons/g_chain/tris.md2", EF_ROTATE,
		"models/weapons/v_chain/tris.md2",
		/* icon */		"w_chaingun",
		/* pickup */	"Chunkgun",
		0,
		1,
		&gI_ammo_cells,
		IT_WEAPON|IT_ALTWEAPON|IT_STAY_COOP,
		NULL,
		0,
		/* precache */ "weapons/chngnu1a.wav weapons/shotgf1b.wav weapons/shotgr1b.wav weapons/chngnd1a.wav"
};

gitem_t gI_weapon_freezersweeper =
{
	"weapon_freezersweeper", 
		Pickup_Weapon,
		Use_Weapon,
		Drop_Weapon,
		Weapon_Freezersweeper,
		"misc/w_pkup.wav",
		"models/weapons/g_chain/tris.md2", EF_ROTATE,
		"models/weapons/v_chain/tris.md2",
		/* icon */		"w_chaingun",
		/* pickup */	"Freezersweeper",
		0,
		1,
		&gI_ammo_cells,
		IT_WEAPON|IT_ALTWEAPON|IT_STAY_COOP,
		NULL,
		0,
		/* precache */ "weapons/chngnu1a.wav weapons/shotgf1b.wav weapons/shotgr1b.wav weapons/chngnd1a.wav"
};


gitem_t gI_weapon_flamesweeper =
{
	"weapon_flamesweeper", 
		Pickup_Weapon,
		Use_Weapon,
		Drop_Weapon,
		Weapon_Flamethrower,
		"misc/w_pkup.wav",
		"models/weapons/g_chain/tris.md2", EF_ROTATE,
		"models/weapons/v_chain/tris.md2",
		/* icon */		"w_chaingun",
		/* pickup */	"Flamesweeper",
		0,
		1,
		&gI_ammo_cells,
		IT_WEAPON|IT_ALTWEAPON|IT_STAY_COOP,
		NULL,
		0,
		/* precache */ "weapons/chngnu1a.wav weapons/shotgf1b.wav weapons/shotgr1b.wav weapons/chngnd1a.wav"
};

/*QUAKED weapon_bazookasweeper (.3 .3 1) (-16 -16 -16) (16 16 16)
*/
gitem_t gI_weapon_bazookasweeper =
{
	"weapon_bazookasweeper", 
		Pickup_Weapon,
		Use_Weapon,
		Drop_Weapon,
		Weapon_Bazookasweeper,
		"misc/w_pkup.wav",
		"models/weapons/g_chain/tris.md2", EF_ROTATE,
		"models/weapons/v_chain/tris.md2",
		/* icon */		"w_chaingun",
		/* pickup */	"Bazookasweeper",
		0,
		1,
		&gI_ammo_grenades,
		IT_WEAPON|IT_ALTWEAPON|IT_STAY_COOP,
		NULL,
		0,
		/* precache */ "weapons/chngnu1a.wav weapons/shotgf1b.wav weapons/shotgr1b.wav weapons/chngnd1a.wav"
};


/*QUAKED weapon_railgunsweeper (.3 .3 1) (-16 -16 -16) (16 16 16)
*/
gitem_t gI_weapon_railgunsweeper =
{
	"weapon_railgunsweeper", 
		Pickup_Weapon,
		Use_Weapon,
		Drop_Weapon,
		Weapon_Railgunsweeper,
		"misc/w_pkup.wav",
		"models/weapons/g_chain/tris.md2", EF_ROTATE,
		"models/weapons/v_chain/tris.md2",
		/* icon */		"w_chaingun",
		/* pickup */	"Railgunsweeper",
		0,
		1,
		&gI_ammo_slugs,
		IT_WEAPON|IT_ALTWEAPON|IT_STAY_COOP,
		NULL,
		0,
		/* precache */ "weapons/chngnu1a.wav weapons/shotgf1b.wav weapons/shotgr1b.wav weapons/chngnd1a.wav"
};

/*QUAKED weapon_snipersweeper (.3 .3 1) (-16 -16 -16) (16 16 16)
*/
gitem_t gI_weapon_snipersweeper =
{
	"weapon_snipersweeper", 
		Pickup_Weapon,
		Use_Weapon,
		Drop_Weapon,
		Weapon_Snipersweeper,
		"misc/w_pkup.wav",
		"models/weapons/g_chain/tris.md2", EF_ROTATE,
		"models/weapons/v_chain/tris.md2",
		/* icon */		"w_chaingun",
		/* pickup */	"Snipersweeper",
		0,
		5,
		&gI_ammo_shells,
		IT_WEAPON|IT_ALTWEAPON|IT_STAY_COOP,
		NULL,
		0,
		/* precache */ "weapons/chngnu1a.wav weapons/shotgf1b.wav weapons/shotgr1b.wav weapons/chngnd1a.wav"
};


/*QUAKED ammo_grenades (.3 .3 1) (-16 -16 -16) (16 16 16)
*/
gitem_t gI_ammo_grenades =
{
	"ammo_grenades",
		Pickup_Ammo,
		Use_GrenadeWeapon,
		Drop_Ammo,
		Weapon_Grenade,
		"misc/am_pkup.wav",
		"models/items/ammo/grenades/medium/tris.md2", 0,
		"models/weapons/v_handgr/tris.md2",
		/* icon */		"a_grenades",
		/* pickup */	"Grenades",
		/* width */		3,
		5 /* 1 */,
		&gI_ammo_grenades,
		IT_AMMO|IT_WEAPON,
		NULL,
		AMMO_GRENADES,
		/* precache */ "weapons/hgrent1a.wav weapons/hgrena1b.wav weapons/hgrenc1b.wav weapons/hgrenb1a.wav weapons/hgrenb2a.wav "
};

/*QUAKED weapon_clustergrenade (.3 .3 1) (-16 -16 -16) (16 16 16)
*/
gitem_t gI_weapon_blindinggrenade =
{
	"weapon_blindinggrenade",
		NULL,
		Use_GrenadeWeapon,
		NULL,
		NULL,
		"misc/am_pkup.wav",
		"models/items/ammo/grenades/medium/tris.md2", 0,
		"models/weapons/v_handgr/tris.md2",
		/* icon */		"a_grenades",
		/* pickup */	"Blinding Grenade",
		/* width */		3,
		5 /* 1 */,
		&gI_ammo_grenades,
		IT_WEAPON,
		NULL,
		0,
		/* precache */ "weapons/hgrent1a.wav weapons/hgrena1b.wav weapons/hgrenc1b.wav weapons/hgrenb1a.wav weapons/hgrenb2a.wav "
};

/*QUAKED weapon_clustergrenade (.3 .3 1) (-16 -16 -16) (16 16 16)
*/
gitem_t gI_weapon_clustergrenade =
{
	"weapon_clustergrenade",
		NULL,
		Use_GrenadeWeapon,
		NULL,
		NULL,
		"misc/am_pkup.wav",
		"models/items/ammo/grenades/medium/tris.md2", 0,
		"models/weapons/v_handgr/tris.md2",
		/* icon */		"a_grenades",
		/* pickup */	"Cluster Grenade",
		/* width */		3,
		5 /* 1 */,
		&gI_ammo_grenades,
		IT_WEAPON,
		NULL,
		0,
		/* precache */ "weapons/hgrent1a.wav weapons/hgrena1b.wav weapons/hgrenc1b.wav weapons/hgrenb1a.wav weapons/hgrenb2a.wav "
};

/*QUAKED weapon_railbomb (.3 .3 1) (-16 -16 -16) (16 16 16)
*/
gitem_t gI_weapon_railbomb =
{
	"weapon_railbomb",
		NULL,
		Use_GrenadeWeapon,
		NULL,
		NULL,
		"misc/am_pkup.wav",
		"models/items/ammo/grenades/medium/tris.md2", 0,
		"models/weapons/v_handgr/tris.md2",
		/* icon */		"a_grenades",
		/* pickup */	"Rail Bomb",
		/* width */		3,
		5 /* 1 */,
		&gI_ammo_grenades,
		IT_WEAPON,
		NULL,
		0,
		/* precache */ "weapons/hgrent1a.wav weapons/hgrena1b.wav weapons/hgrenc1b.wav weapons/hgrenb1a.wav weapons/hgrenb2a.wav "
};

/*QUAKED weapon_plasmagrenade (.3 .3 1) (-16 -16 -16) (16 16 16)
*/
gitem_t gI_weapon_plasmagrenade =
{
	"weapon_plasmagrenade",
		NULL,
		Use_GrenadeWeapon,
		NULL,
		NULL,
		"misc/am_pkup.wav",
		"models/items/ammo/grenades/medium/tris.md2", 0,
		"models/weapons/v_handgr/tris.md2",
		/* icon */		"a_grenades",
		/* pickup */	"Plasma Grenade",
		/* width */		3,
		5 /* 1 */,
		&gI_ammo_grenades,
		IT_WEAPON,
		NULL,
		0,
		/* precache */ "weapons/hgrent1a.wav weapons/hgrena1b.wav weapons/hgrenc1b.wav weapons/hgrenb1a.wav weapons/hgrenb2a.wav "
};

/*QUAKED weapon_napalmgrenade (.3 .3 1) (-16 -16 -16) (16 16 16)
*/
gitem_t gI_weapon_napalmgrenade =
{
	"weapon_napalmgrenade",
		NULL,
		Use_GrenadeWeapon,
		NULL,
		NULL,
		"misc/am_pkup.wav",
		"models/items/ammo/grenades/medium/tris.md2", 0,
		"models/weapons/v_handgr/tris.md2",
		/* icon */		"a_grenades",
		/* pickup */	"Napalm Grenade",
		/* width */		3,
		5 /* 1 */,
		&gI_ammo_grenades,
		IT_WEAPON,
		NULL,
		0,
		/* precache */ "weapons/hgrent1a.wav weapons/hgrena1b.wav weapons/hgrenc1b.wav weapons/hgrenb1a.wav weapons/hgrenb2a.wav "
};

/*QUAKED weapon_shrapnelgrenade (.3 .3 1) (-16 -16 -16) (16 16 16)
*/
gitem_t gI_weapon_shrapnelgrenade =
{
	"weapon_shrapnelgrenade",
		NULL,
		Use_GrenadeWeapon,
		NULL,
		NULL,
		"misc/am_pkup.wav",
		"models/items/ammo/grenades/medium/tris.md2", 0,
		"models/weapons/v_handgr/tris.md2",
		/* icon */		"a_grenades",
		/* pickup */	"Shrapnel Grenade",
		/* width */		3,
		5 /* 1 */,
		&gI_ammo_grenades,
		IT_WEAPON,
		NULL,
		0,
		/* precache */ "weapons/hgrent1a.wav weapons/hgrena1b.wav weapons/hgrenc1b.wav weapons/hgrenb1a.wav weapons/hgrenb2a.wav "
};

/*QUAKED weapon_cataclysm (.3 .3 1) (-16 -16 -16) (16 16 16)
*/
gitem_t gI_weapon_cataclysm =
{
	"weapon_cataclysmdevice",
		NULL,
		Use_GrenadeWeapon,
		NULL,
		NULL,
		"misc/am_pkup.wav",
		"models/items/ammo/grenades/medium/tris.md2", 0,
		"models/weapons/v_handgr/tris.md2",
		/* icon */		"a_grenades",
		/* pickup */	"Cataclysm Device",
		/* width */		3,
		10 /* 1 */,
		&gI_ammo_grenades,
		IT_WEAPON,
		NULL,
		0,
		/* precache */ "weapons/hgrent1a.wav weapons/hgrena1b.wav weapons/hgrenc1b.wav weapons/hgrenb1a.wav weapons/hgrenb2a.wav "
};

/*QUAKED weapon_plasmaclustergrenade (.3 .3 1) (-16 -16 -16) (16 16 16)
*/
gitem_t gI_weapon_plasmaclustergrenade =
{
	"weapon_plasmaclustergrenade",
		NULL,
		Use_GrenadeWeapon,
		NULL,
		NULL,
		"misc/am_pkup.wav",
		"models/items/ammo/grenades/medium/tris.md2", 0,
		"models/weapons/v_handgr/tris.md2",
		/* icon */		"a_grenades",
		/* pickup */	"Plasma Cluster Grenade",
		/* width */		3,
		5 /* 1 */,
		&gI_ammo_grenades,
		IT_WEAPON,
		NULL,
		0,
		/* precache */ "weapons/hgrent1a.wav weapons/hgrena1b.wav weapons/hgrenc1b.wav weapons/hgrenb1a.wav weapons/hgrenb2a.wav "
};

/*QUAKED weapon_telegrenade (.3 .3 1) (-16 -16 -16) (16 16 16)
*/
gitem_t gI_weapon_telegrenade =
{
	"weapon_teleportgrenade",
		NULL,
		Use_GrenadeWeapon,
		NULL,
		NULL,
		"misc/am_pkup.wav",
		"models/items/ammo/grenades/medium/tris.md2", 0,
		"models/weapons/v_handgr/tris.md2",
		/* icon */		"a_grenades",
		/* pickup */	"Teleport Grenade",
		/* width */		3,
		5 /* 1 */,
		&gI_ammo_grenades,
		IT_WEAPON,
		NULL,
		0,
		/* precache */ "weapons/hgrent1a.wav weapons/hgrena1b.wav weapons/hgrenc1b.wav weapons/hgrenb1a.wav weapons/hgrenb2a.wav "
};

/*QUAKED weapon_freezegrenade (.3 .3 1) (-16 -16 -16) (16 16 16)
*/
gitem_t gI_weapon_freezegrenade =
{
	"weapon_freezegrenade",
		NULL,
		Use_GrenadeWeapon,
		NULL,
		NULL,
		"misc/am_pkup.wav",
		"models/items/ammo/grenades/medium/tris.md2", 0,
		"models/weapons/v_handgr/tris.md2",
		/* icon */		"a_grenades",
		/* pickup */	"Freeze Grenade",
		/* width */		3,
		5 /* 1 */,
		&gI_ammo_grenades,
		IT_WEAPON,
		NULL,
		0,
		/* precache */ "weapons/hgrent1a.wav weapons/hgrena1b.wav weapons/hgrenc1b.wav weapons/hgrenb1a.wav weapons/hgrenb2a.wav "
};

/*QUAKED weapon_homergrenade (.3 .3 1) (-16 -16 -16) (16 16 16)
*/
gitem_t gI_weapon_homergrenade =
{
	"weapon_homergrenade",
		NULL,
		Use_GrenadeWeapon,
		NULL,
		NULL,
		"misc/am_pkup.wav",
		"models/items/ammo/grenades/medium/tris.md2", 0,
		"models/weapons/v_handgr/tris.md2",
		/* icon */		"a_grenades",
		/* pickup */	"Homer Grenade",
		/* width */		3,
		5 /* 1 */,
		&gI_ammo_grenades,
		IT_WEAPON,
		NULL,
		0,
		/* precache */ "weapons/hgrent1a.wav weapons/hgrena1b.wav weapons/hgrenc1b.wav weapons/hgrenb1a.wav weapons/hgrenb2a.wav "
};

/*QUAKED weapon_rocketgrenade (.3 .3 1) (-16 -16 -16) (16 16 16)
*/
gitem_t gI_weapon_rocketgrenade =
{
	"weapon_rocketgrenade",
		NULL,
		Use_GrenadeWeapon,
		NULL,
		NULL,
		"misc/am_pkup.wav",
		"models/items/ammo/grenades/medium/tris.md2", 0,
		"models/weapons/v_handgr/tris.md2",
		/* icon */		"a_grenades",
		/* pickup */	"Rocket Grenade",
		/* width */		3,
		5 /* 1 */,
		&gI_ammo_grenades,
		IT_WEAPON,
		NULL,
		0,
		/* precache */ "weapons/hgrent1a.wav weapons/hgrena1b.wav weapons/hgrenc1b.wav weapons/hgrenb1a.wav weapons/hgrenb2a.wav "
};

/*QUAKED weapon_positrongrenade (.3 .3 1) (-16 -16 -16) (16 16 16)
*/
gitem_t gI_weapon_positrongrenade =
{
	"weapon_positrongrenade",
		NULL,
		Use_GrenadeWeapon,
		NULL,
		NULL,
		"misc/am_pkup.wav",
		"models/items/ammo/grenades/medium/tris.md2", 0,
		"models/weapons/v_handgr/tris.md2",
		/* icon */		"a_grenades",
		/* pickup */	"Positron Grenade",
		/* width */		3,
		10/* 1 */,
		&gI_ammo_grenades,
		IT_WEAPON,
		NULL,
		0,
		/* precache */ "weapons/hgrent1a.wav weapons/hgrena1b.wav weapons/hgrenc1b.wav weapons/hgrenb1a.wav weapons/hgrenb2a.wav "
};

/*QUAKED weapon_midnightgrenade (.3 .3 1) (-16 -16 -16) (16 16 16)
*/
gitem_t gI_weapon_midnightgrenade =
{
	"weapon_midnightgrenade",
		NULL,
		Use_GrenadeWeapon,
		NULL,
		NULL,
		"misc/am_pkup.wav",
		"models/items/ammo/grenades/medium/tris.md2", 0,
		"models/weapons/v_handgr/tris.md2",
		/* icon */		"a_grenades",
		/* pickup */	"Midnight Grenade",
		/* width */		3,
		10/* 1 */,
		&gI_ammo_grenades,
		IT_WEAPON,
		NULL,
		0,
		/* precache */ "weapons/hgrent1a.wav weapons/hgrena1b.wav weapons/hgrenc1b.wav weapons/hgrenb1a.wav weapons/hgrenb2a.wav "
};

/*QUAKED weapon_freezatrongrenade (.3 .3 1) (-16 -16 -16) (16 16 16)
*/
gitem_t gI_weapon_freezatrongrenade =
{
	"weapon_freezatrongrenade",
		NULL,
		Use_GrenadeWeapon,
		NULL,
		NULL,
		"misc/am_pkup.wav",
		"models/items/ammo/grenades/medium/tris.md2", 0,
		"models/weapons/v_handgr/tris.md2",
		/* icon */		"a_grenades",
		/* pickup */	"Freezatron Grenade",
		/* width */		3,
		10/* 1 */,
		&gI_ammo_grenades,
		IT_WEAPON,
		NULL,
		0,
		/* precache */ "weapons/hgrent1a.wav weapons/hgrena1b.wav weapons/hgrenc1b.wav weapons/hgrenb1a.wav weapons/hgrenb2a.wav "
};

/*QUAKED weapon_banzaigrenade (.3 .3 1) (-16 -16 -16) (16 16 16)
*/
gitem_t gI_weapon_banzaigrenade =
{
	"weapon_banzaigrenade",
		NULL,
		Use_GrenadeWeapon,
		NULL,
		NULL,
		"misc/am_pkup.wav",
		"models/items/ammo/grenades/medium/tris.md2", 0,
		"models/weapons/v_handgr/tris.md2",
		/* icon */		"a_grenades",
		/* pickup */	"Banzai Grenade",
		/* width */		3,
		10/* 1 */,
		&gI_ammo_grenades,
		IT_WEAPON,
		NULL,
		0,
		/* precache */ "weapons/hgrent1a.wav weapons/hgrena1b.wav weapons/hgrenc1b.wav weapons/hgrenb1a.wav weapons/hgrenb2a.wav "
};


/*QUAKED weapon_spinningrailbomb (.3 .3 1) (-16 -16 -16) (16 16 16)
*/
gitem_t gI_weapon_spinningrailbomb =
{
	"weapon_spinningrailbomb",
		NULL,
		Use_GrenadeWeapon,
		NULL,
		NULL,
		"misc/am_pkup.wav",
		"models/items/ammo/grenades/medium/tris.md2", 0,
		"models/weapons/v_handgr/tris.md2",
		/* icon */		"a_grenades",
		/* pickup */	"Spinning Rail Grenade",
		/* width */		3,
		10/* 1 */,
		&gI_ammo_grenades,
		IT_WEAPON,
		NULL,
		0,
		/* precache */ "weapons/hgrent1a.wav weapons/hgrena1b.wav weapons/hgrenc1b.wav weapons/hgrenb1a.wav weapons/hgrenb2a.wav "
};

/*QUAKED ammo_nukegrenades (.3 .3 1) (-16 -16 -16) (16 16 16)
n00k
*/
gitem_t gI_weapon_nukegrenades =
{
	"ammo_nukegrenades",
		Pickup_Ammo,
		Use_Weapon,
		Drop_Ammo,
		Weapon_NukeGrenade,
		"misc/am_pkup.wav",
		"models/items/ammo/grenades/bang/tris.md2", 0,
		"models/weapons/v_nukegr/tris.md2",
		/* icon */		"i_nuke",
		/* pickup */	"Nuke Grenades",
		/* width */		3,
		1,
		&gI_ammo_grenades,
		IT_AMMO|IT_WEAPON,
		NULL,
		0,
		/* precache */ "weapons/hgrent1a.wav weapons/hgrena1b.wav weapons/hgrenc1b.wav weapons/hgrenb1a.wav weapons/hgrenb2a.wav misc/bigtele.wav",
};

/*QUAKED weapon_grenadelauncher (.3 .3 1) (-16 -16 -16) (16 16 16)
*/
gitem_t gI_weapon_grenadelauncher =
{
	"weapon_grenadelauncher",
		Pickup_Weapon,
		Use_Weapon,
		Drop_Weapon,
		Weapon_GrenadeLauncher,
		"misc/w_pkup.wav",
		"models/weapons/g_launch/tris.md2", EF_ROTATE,
		"models/weapons/v_launch/tris.md2",
		/* icon */		"w_glauncher",
		/* pickup */	"Grenade Launcher",
		0,
		1,
		&gI_ammo_grenades,
		IT_WEAPON|IT_STAY_COOP,
		NULL,
		0,
		/* precache */ "models/objects/grenade/tris.md2 weapons/grenlf1a.wav weapons/grenlr1b.wav weapons/grenlb1b.wav"
};

/*QUAKED weapon_stickinggrenadelauncher (.3 .3 1) (-16 -16 -16) (16 16 16)
*/
gitem_t gI_weapon_stickinggrenadelauncher = 	{
	"weapon_stickinggrenadelauncher",
		Pickup_Weapon,
		Use_Weapon,
		Drop_Weapon,
		Weapon_StickingGrenadeLauncher,
		"misc/w_pkup.wav",
		"models/weapons/g_launch/tris.md2", EF_ROTATE,
		"models/weapons/v_launch/tris.md2",
		/* icon */		"w_glauncher",
		/* pickup */    "Sticking Grenades",
		0,
		1,
		&gI_ammo_grenades,
		IT_WEAPON|IT_STAY_COOP,
		NULL,
		0,
		/* precache */ "models/objects/hgproxy/tris.md2 sound/misc/giggle1.wav sound/misc/giggle2.wav sound/misc/giggle3.wav sound/misc/giggle4.wav sound/misc/giggle5.wav sound/misc/giggle6.wav sound/misc/giggle10.wav sound/misc/giggle7.wav sound/misc/giggle8.wav sound/misc/giggle9.wav sound/misc/giggle10.wav"
};

/* weapon_bucky (.3 .3 1) (-16 -16 -16) (16 16 16)
always owned, never in the world
*/
gitem_t gI_weapon_bucky =
{
    "weapon_bucky", 
		Pickup_Weapon,
		Use_Weapon,
		Drop_Weapon,
		Weapon_Bucky,
		"misc/w_pkup.wav",
		"models/weapons/g_launch/tris.md2", EF_ROTATE,
		"models/weapons/v_launch/tris.md2",
		/* icon */		"w_glauncher",
		/* pickup */	"Buckyball Launcher",
		0,
		1,
		&gI_ammo_grenades,
		IT_WEAPON|IT_STAY_COOP,
		NULL,
		0,
		/* precache */ "models/objects/hgproxy/tris.md2 sound/misc/giggle1.wav sound/misc/giggle2.wav sound/misc/giggle3.wav sound/misc/giggle4.wav sound/misc/giggle5.wav sound/misc/giggle6.wav sound/misc/giggle10.wav sound/misc/giggle7.wav sound/misc/giggle8.wav sound/misc/giggle9.wav sound/misc/giggle10.wav"
};


/*QUAKED weapon_bazooka (.3 .3 1) (-16 -16 -16) (16 16 16)
*/
gitem_t gI_weapon_floatingminelauncher =
{
	"weapon_floatingminelauncher",
		Pickup_Weapon,
		Use_Weapon,
		Drop_Weapon,
		Weapon_FloatingMineLauncher,
		"misc/w_pkup.wav",
		"models/weapons/g_launch/tris.md2", EF_ROTATE,
		"models/weapons/v_launch/tris.md2",
		/* icon */		"w_glauncher",
		/* pickup */	"Floating Mine Launcher",
		0,
		3,
		&gI_ammo_grenades,
		IT_WEAPON|IT_ALTWEAPON|IT_STAY_COOP,
		NULL,
		0,
		/* precache */ "models/objects/grenade/tris.md2 weapons/grenlf1a.wav weapons/grenlr1b.wav weapons/grenlb1b.wav sound/flyer/flyidle1.wav"
};

/*QUAKED weapon_bazooka (.3 .3 1) (-16 -16 -16) (16 16 16)
*/
gitem_t gI_weapon_bazooka =
{
	"weapon_bazooka",
		Pickup_Weapon,
		Use_Weapon,
		Drop_Weapon,
		Weapon_Bazooka,
		"misc/w_pkup.wav",
		"models/weapons/g_launch/tris.md2", EF_ROTATE,
		"models/weapons/v_launch/tris.md2",
		/* icon */		"w_glauncher",
		/* pickup */	"Bazooka",
		0,
		1,
		&gI_ammo_grenades,
		IT_WEAPON|IT_ALTWEAPON|IT_STAY_COOP,
		NULL,
		0,
		/* precache */ "models/objects/grenade/tris.md2 weapons/grenlf1a.wav weapons/grenlr1b.wav weapons/grenlb1b.wav sound/flyer/flyidle1.wav"
};

/*QUAKED weapon_doubleimpact (.3 .3 1) (-16 -16 -16) (16 16 16)
*/
gitem_t gI_weapon_doubleimpact =	{
	"weapon_doubleimpact", 
		Pickup_Weapon,
		Use_Weapon,
		Drop_Weapon,
        Weapon_DGSuperShotgun,
		"misc/w_pkup.wav",
		"models/weapons/g_shotg2/tris.md2", EF_ROTATE,
		"models/weapons/v_shotg2/tris.md2",
		/* icon */		"w_sshotgun",
		/* pickup */    "Double Impact",
		0,
		2,
        &gI_ammo_grenades,
		IT_WEAPON|IT_STAY_COOP,
		NULL,
		0,
		/* precache */ "weapons/sshotf1b.wav"
};

/*QUAKED weapon_rocketlauncher (.3 .3 1) (-16 -16 -16) (16 16 16)
*/
gitem_t gI_weapon_rocketlauncher =
{
	"weapon_rocketlauncher",
		Pickup_Weapon,
		Use_Weapon,
		Drop_Weapon,
		Weapon_RocketLauncher,
		"misc/w_pkup.wav",
		"models/weapons/g_rocket/tris.md2", EF_ROTATE,
		"models/weapons/v_rocket/tris.md2",
		/* icon */		"w_rlauncher",
		/* pickup */	"Rocket Launcher",
		0,
		1,
		&gI_ammo_rockets,
		IT_WEAPON|IT_STAY_COOP,
		NULL,
		0,
		/* precache */ "models/objects/rocket/tris.md2 weapons/rockfly.wav weapons/rocklf1a.wav weapons/rocklr1b.wav models/objects/debris2/tris.md2"
};

gitem_t gI_weapon_spiralrocketlauncher =
{
	"weapon_spiralrocketlauncher",
		Pickup_Weapon,
		Use_Weapon,
		Drop_Weapon,
		Weapon_SpiralRocketLauncher,
		"misc/w_pkup.wav",
		"models/weapons/g_rocket/tris.md2", EF_ROTATE,
		"models/weapons/v_rocket/tris.md2",
		/* icon */		"w_rlauncher",
		/* pickup */	"Spiral Rocket Launcher",
		0,
		3,
		&gI_ammo_rockets,
		IT_WEAPON|IT_STAY_COOP,
		NULL,
		0,
		/* precache */ "models/objects/rocket/tris.md2 weapons/rockfly.wav weapons/rocklf1a.wav weapons/rocklr1b.wav models/objects/debris2/tris.md2"
};

/*QUAKED weapon_homing (.3 .3 1) (-16 -16 -16) (16 16 16)
*/
gitem_t gI_weapon_homing = {
	"weapon_homingrocketlauncher",
		Pickup_Weapon,
		Use_Weapon,
		Drop_Weapon,
		Weapon_Homing,
		"misc/w_pkup.wav",
		"models/weapons/g_rocket/tris.md2", EF_ROTATE,
		"models/weapons/v_rocket/tris.md2",
		/* icon */		"w_rlauncher",
		/* pickup */	"Homing Rocket Launcher",
		0,
		1,
		&gI_ammo_rockets,
		IT_WEAPON|IT_ALTWEAPON|IT_STAY_COOP,
		NULL,
		0,
		/* precache */ "models/objects/rocket/tris.md2 weapons/rockfly.wav weapons/rocklf1a.wav weapons/rocklr1b.wav models/objects/debris2/tris.md2"
};

/*QUAKED weapon_skipper (.3 .3 1) (-16 -16 -16) (16 16 16)
*/
gitem_t gI_weapon_skipper =
{
	"weapon_skipperhomingrocket",
		Pickup_Weapon,
		Use_Weapon,
		Drop_Weapon,
		Weapon_Skipper,
		"misc/w_pkup.wav",
		"models/weapons/g_rocket/tris.md2", EF_ROTATE,
		"models/weapons/v_rocket/tris.md2",
		/* icon */		"w_rlauncher",
		/* pickup */	"Skipper Homing Rocket",//My Homing Rocket",
		0,
		1,
		&gI_ammo_rockets,
		IT_WEAPON|IT_ALTWEAPON|IT_STAY_COOP,
		NULL,
		0,
		/* precache */ "models/objects/rocket/tris.md2 weapons/rockfly.wav weapons/rocklf1a.wav weapons/rocklr1b.wav models/objects/debris2/tris.md2"
};

/*QUAKED weapon_guided (.3 .3 1) (-16 -16 -16) (16 16 16)
*/
gitem_t gI_weapon_guided =
{
	"weapon_guidedmissile",
		Pickup_Weapon,
		Use_Weapon,
		Drop_Weapon,
		Weapon_GuidedMissiles,
		"misc/w_pkup.wav",
		"models/weapons/g_rocket/tris.md2", EF_ROTATE,
		"models/weapons/v_rocket/tris.md2",
		/* icon */		"w_rlauncher",
		/* pickup */	"Guided Missile",//Guided Homing Rocket",
		0,
		1,
		&gI_ammo_rockets,
		IT_WEAPON|IT_ALTWEAPON|IT_STAY_COOP,
		NULL,
		0,
		/* precache */ "models/objects/rocket/tris.md2 weapons/rockfly.wav weapons/rocklf1a.wav weapons/rocklr1b.wav models/objects/debris2/tris.md2"
};


/*QUAKED weapon_guidednuke (.3 .3 1) (-16 -16 -16) (16 16 16)
*/
gitem_t gI_weapon_guidednuke =
{
	"weapon_guidednuke",
		Pickup_Weapon,
		Use_Weapon,
		Drop_Weapon,
		Weapon_GuidedNuke,
		"misc/w_pkup.wav",
		"models/weapons/g_rocket/tris.md2", EF_ROTATE,
		"models/weapons/v_rocket/tris.md2",
		/* icon */		"w_rlauncher",
		/* pickup */	"Guided Nuke",//Guided Homing Rocket",
		0,
		10,
		&gI_ammo_rockets,
		IT_WEAPON|IT_ALTWEAPON|IT_STAY_COOP,
		NULL,
		0,
		/* precache */ "models/objects/rocket/tris.md2 weapons/rockfly.wav weapons/rocklf1a.wav weapons/rocklr1b.wav models/objects/debris2/tris.md2"
};

/*QUAKED weapon_lightninggun (.3 .3 1) (-16 -16 -16) (16 16 16)
*/
gitem_t gI_weapon_lightninggun =	{
	"weapon_lightninggun",
		Pickup_Weapon,
		Use_Weapon,
		Drop_Weapon,
		Weapon_LightningGun,
		"misc/w_pkup.wav",
		"models/weapons/g_hyperb/tris.md2", EF_ROTATE,
		"models/weapons/v_hyperb/tris.md2",
		/* icon */		"w_hyperblaster",
		/* pickup */	"Lightning Gun",
		0,
		5,
		&gI_ammo_cells,
		IT_WEAPON|IT_STAY_COOP,
		NULL,
		0,
		/* precache */ "light/lhit.wav light/lstart.wav models/proj/lightning/tris.md2"
};

/*QUAKED weapon_antimattercannon (.3 .3 1) (-16 -16 -16) (16 16 16)
*/
gitem_t gI_weapon_antimattercannon =	{
	"weapon_antimattercannon",
		Pickup_Weapon,
		Use_Weapon,
		Drop_Weapon,
		Weapon_AntimatterCannon,
		"misc/w_pkup.wav",
		"models/weapons/g_rail/tris.md2", EF_ROTATE,
		"models/weapons/v_rail/tris.md2",
		/* icon */		"w_railgun",
		/* pickup */	"Antimatter Cannon",
		0,
		10,
		&gI_ammo_slugs,
		IT_WEAPON|IT_STAY_COOP,
		NULL,
		0,
		/* precache */ "light/lhit.wav light/lstart.wav",
};

/*QUAKED weapon_positron (.3 .3 1) (-16 -16 -16) (16 16 16)
*/
gitem_t gI_weapon_positron = {
	"weapon_positron", 
		Pickup_Weapon,
		Use_Weapon,
		Drop_Weapon,
		Weapon_Positron,
		"misc/w_pkup.wav",
		"models/weapons/g_hyperb/tris.md2", EF_ROTATE,
		"models/weapons/v_hyperb/tris.md2",
		/* icon */		"w_hyperblaster",
		/* pickup */	"Positron",
		0,
		5,
		&gI_ammo_cells,
		IT_WEAPON|IT_STAY_COOP,
		NULL,
		0,
		/* precache */ "weapons/hyprbu1a.wav weapons/hyprbl1a.wav weapons/hyprbf1a.wav weapons/hyprbd1a.wav misc/lasfly.wav"
};

/*QUAKED weapon_freezatron (.3 .3 1) (-16 -16 -16) (16 16 16)
*/
gitem_t gI_weapon_freezatron = {
	"weapon_freezatron", 
		Pickup_Weapon,
		Use_Weapon,
		Drop_Weapon,
		Weapon_Freezatron,
		"misc/w_pkup.wav",
		"models/weapons/g_hyperb/tris.md2", EF_ROTATE,
		"models/weapons/v_hyperb/tris.md2",
		/* icon */		"w_hyperblaster",
		/* pickup */	"Freezatron",
		0,
		5,
		&gI_ammo_cells,
		IT_WEAPON|IT_STAY_COOP,
		NULL,
		0,
		/* precache */ "weapons/hyprbu1a.wav weapons/hyprbl1a.wav weapons/hyprbf1a.wav weapons/hyprbd1a.wav misc/lasfly.wav"
};

/*QUAKED weapon_hyperblaster (.3 .3 1) (-16 -16 -16) (16 16 16)
*/
gitem_t gI_weapon_hyperblaster = {
	"weapon_hyperblaster", 
		Pickup_Weapon,
		Use_Weapon,
		Drop_Weapon,
		Weapon_HyperBlaster,
		"misc/w_pkup.wav",
		"models/weapons/g_hyperb/tris.md2", EF_ROTATE,
		"models/weapons/v_hyperb/tris.md2",
		/* icon */		"w_hyperblaster",
		/* pickup */	"HyperBlaster",
		0,
		1,
		&gI_ammo_cells,
		IT_WEAPON|IT_STAY_COOP,
		NULL,
		0,
		/* precache */ "weapons/hyprbu1a.wav weapons/hyprbl1a.wav weapons/hyprbf1a.wav weapons/hyprbd1a.wav misc/lasfly.wav"
};

/*QUAKED weapon_icecubegun (.3 .3 1) (-16 -16 -16) (16 16 16)
*/
gitem_t gI_weapon_icecubegun = {
	"weapon_icecubegun", 
		Pickup_Weapon,
		Use_Weapon,
		Drop_Weapon,
		Weapon_Freezegun,
		"misc/w_pkup.wav",
		"models/weapons/g_hyperb/tris.md2", EF_ROTATE,
		"models/weapons/v_hyperb/tris.md2",
		/* icon */		"w_hyperblaster",
		/* pickup */	"Icecube Gun",
		0,
		1,
		&gI_ammo_cells,
		IT_WEAPON|IT_STAY_COOP,
		NULL,
		0,
		/* precache */ "weapons/hyprbu1a.wav weapons/hyprbl1a.wav weapons/hyprbf1a.wav weapons/hyprbd1a.wav misc/lasfly.wav"
};


/*QUAKED weapon_superblaster (.3 .3 1) (-16 -16 -16) (16 16 16)
*/
gitem_t gI_weapon_superblaster =
{
	"weapon_superblaster",
		NULL,
		Use_Weapon,
		NULL,
		Weapon_SuperBlaster,
		"misc/w_pkup.wav",
		NULL, 0,
		"models/weapons/v_blast/tris.md2",
		/* icon */		"w_blaster",
		/* pickup */	"Super Blaster",
		0,
		5,
		&gI_ammo_cells,
		IT_WEAPON|IT_ALTWEAPON|IT_STAY_COOP,
		NULL,
		0,
		/* precache */ "models/objects/rocket/tris.md2 weapons/rockfly.wav weapons/rocklf1a.wav weapons/rocklr1b.wav models/objects/debris2/tris.md2 sound/weapons/tris.md2"
};
// end 4.8 super blaster

/* weapon_railgun [this is the flamethrower] 
*/
gitem_t gI_weapon_railgun =
{
	"weapon_railgun", 
		Pickup_Weapon,
		Use_Weapon,
		Drop_Weapon,
		Weapon_Railgun,
		"misc/w_pkup.wav",
		"models/weapons/g_rail/tris.md2", EF_ROTATE,
		"models/weapons/v_rail/tris.md2",
		/* icon */		"w_railgun",
		/* pickup */	"Railgun",
		0,
		1,
		&gI_ammo_slugs,
		IT_WEAPON|IT_ALTWEAPON|IT_STAY_COOP,
		NULL,
		0,
		/* precache */ "weapons/rg_hum.wav"
};

gitem_t gI_weapon_kaminit =
{
	"weapon_kaminit", 
		Pickup_Weapon,
		Use_Weapon,
		Drop_Weapon,
		Weapon_Kaminit,
		"misc/w_pkup.wav",
		"models/weapons/g_rail/tris.md2", EF_ROTATE,
		"models/weapons/v_rail/tris.md2",
		/* icon */		"w_railgun",
		/* pickup */	"Kaminit",
		0,
		1,
		&gI_ammo_slugs,
		IT_WEAPON|IT_STAY_COOP,
		NULL,
		0,
		/* precache */ "weapons/rg_hum.wav"
};

gitem_t gI_weapon_wallpiercingrailgun =
{
	"weapon_wallpiercingrailgun", 
		Pickup_Weapon,
		Use_Weapon,
		Drop_Weapon,
		Weapon_Railgun2,
		"misc/w_pkup.wav",
		"models/weapons/g_rail/tris.md2", EF_ROTATE,
		"models/weapons/v_rail/tris.md2",
		/* icon */		"w_railgun",
		/* pickup */	"Wall Piercing Railgun",
		0,
		1,
		&gI_ammo_slugs,
		IT_WEAPON|IT_ALTWEAPON|IT_STAY_COOP,
		NULL,
		0,
		/* precache */ "weapons/rg_hum.wav"
};



gitem_t gI_weapon_sonicrailgun =
{
	"weapon_sonicrailgun", 
		Pickup_Weapon,
		Use_Weapon,
		Drop_Weapon,
		Weapon_SonicRailgun,
		"misc/w_pkup.wav",
		"models/weapons/g_rail/tris.md2", EF_ROTATE,
		"models/weapons/v_rail/tris.md2",
		/* icon */		"w_railgun",
		/* pickup */	"Sonic Railgun",
		0,
		4,
		&gI_ammo_slugs,
		IT_WEAPON|IT_ALTWEAPON|IT_STAY_COOP,
		NULL,
		0,
		/* precache */ "weapons/rg_hum.wav"
};

/*QUAKED weapon_railgun2 (.3 .3 1) (-16 -16 -16) (16 16 16)
*/
//QW// this is the original railgun
// don't ask me how it got this way, but not sure I want to
// untangle this mess between the railgun and flamethrower
gitem_t gI_weapon_railgun2 =
{
	"weapon_railgun2", 
		Pickup_Weapon,
		Use_Weapon,
		Drop_Weapon,
		Weapon_Railgun2,
		"misc/w_pkup.wav",
		"models/weapons/g_rail/tris.md2", EF_ROTATE,
		"models/weapons/v_rail/tris.md2",
		/* icon */		"w_railgun",
		/* pickup */	"Railgun2",
		0,
		4,
		&gI_ammo_slugs,
		IT_WEAPON|IT_ALTWEAPON|IT_STAY_COOP,
		NULL,
		0,
		/* precache */ "weapons/rg_hum.wav"
};

//Wyrm: new weapon from Rogue
/*QUAKED weapon_disintegrator (.3 .3 1) (-16 -16 -16) (16 16 16)
*/
gitem_t gI_weapon_disintegrator = {
	"weapon_disintegrator", 
		Pickup_Weapon,
		Use_Weapon,
		Drop_Weapon,
		Weapon_Disintegrator,
		"misc/w_pkup.wav",
		"models/weapons/g_hyperb/tris.md2", EF_ROTATE,
		"models/weapons/v_hyperb/tris.md2",
		/* icon */		"w_hyperblaster",
		/* pickup */    "Disruptor",
		0,
		4,
		&gI_ammo_slugs,
		IT_WEAPON|IT_ALTWEAPON|IT_STAY_COOP,
		NULL,
		0,
		/* precache */ "weapons/hyprbu1a.wav weapons/hyprbl1a.wav weapons/hyprbf1a.wav weapons/hyprbd1a.wav misc/lasfly.wav",
};

gitem_t gI_weapon_durg =
{
	"weapon_durg", 
		Pickup_Weapon,
		Use_Weapon,
		Drop_Weapon,
		Weapon_Durg,
		"misc/w_pkup.wav",
		"models/weapons/g_rail/tris.md2", EF_ROTATE,
		"models/weapons/v_rail/tris.md2",
		/* icon */		"w_railgun",
		/* pickup */	"Durg",
		0,
		4,
		&gI_ammo_slugs,
		IT_WEAPON|IT_ALTWEAPON|IT_STAY_COOP,
		NULL,
		0,
		/* precache */ "weapons/rg_hum.wav"
};

/*QUAKED weapon_nuke (.3 .3 1) (-16 -16 -16) (16 16 16)
*/
gitem_t gI_weapon_nuke = 	{
	"weapon_nuke",
		Pickup_Weapon,
		Use_Weapon,
		Drop_Weapon,
		Weapon_Nuke,
		"misc/w_pkup.wav",
		"models/weapons/g_rail/tris.md2", EF_ROTATE,
		"models/weapons/v_rail/tris.md2",
		/* icon */		"w_railgun",
		/* pickup */    "Nuke",
		0,
		20,
		&gI_ammo_slugs,
		IT_WEAPON|IT_ALTWEAPON|IT_STAY_COOP,
		NULL,
		0,
		/* precache */ "sprites/s_bfg1.sp2 sprites/s_bfg2.sp2 sprites/s_bfg3.sp2 weapons/bfg__f1y.wav weapons/bfg__l1a.wav weapons/bfg__x1b.wav weapons/bfg_hum.wav",
};

/*QUAKED weapon_energyvortex (.3 .3 1) (-16 -16 -16) (16 16 16)
*/
gitem_t gI_weapon_energyvortex =
{
	"weapon_energyvortex",
		Pickup_Weapon,
		Use_Weapon,
		Drop_Weapon,
        Weapon_EnergyVortex,
		"misc/w_pkup.wav",
		"models/weapons/g_bfg/tris.md2", EF_ROTATE,
		"models/weapons/v_bfg/tris.md2",
		/* icon */		"w_bfg",
		/* pickup */    "Energy Vortex",
        0,                  
        100,
		&gI_ammo_cells,
		IT_WEAPON|IT_STAY_COOP,
		NULL,
		0,
		/* precache */ "models/items/keys/pyramid/tris.md2 grav_new.wav ",
};

/*QUAKED weapon_vacuummaker (.3 .3 1) (-16 -16 -16) (16 16 16)
*/
gitem_t gI_weapon_vacuummaker =	
{
	"weapon_vacuummaker",
		Pickup_Weapon,
		Use_Weapon,
		Drop_Weapon,
        Weapon_VacuumMaker,
		"misc/w_pkup.wav",
		"models/weapons/g_bfg/tris.md2", EF_ROTATE,
		"models/weapons/v_bfg/tris.md2",
		/* icon */		"w_bfg",
		/* pickup */    "Vacuum Maker",
		0,
		1,
		&gI_ammo_cells,
		IT_WEAPON|IT_STAY_COOP,
		NULL,
		0,
		/* precache */ "",
};

/*QUAKED weapon_bfg (.3 .3 1) (-16 -16 -16) (16 16 16)
*/
gitem_t gI_weapon_bfg =
{
	"weapon_bfg",
		Pickup_Weapon,
		Use_Weapon,
		Drop_Weapon,
		Weapon_BFG,
		"misc/w_pkup.wav",
		"models/weapons/g_bfg/tris.md2", EF_ROTATE,
		"models/weapons/v_bfg/tris.md2",
		/* icon */		"w_bfg",
		/* pickup */	"BFG10K",
		0,
		50,
		&gI_ammo_cells,
		IT_WEAPON|IT_STAY_COOP,
		NULL,
		0,
		/* precache */ "sprites/s_bfg1.sp2 sprites/s_bfg2.sp2 sprites/s_bfg3.sp2 weapons/bfg__f1y.wav weapons/bfg__l1a.wav weapons/bfg__x1b.wav weapons/bfg_hum.wav"
};

/*QUAKED weapon_bfg (.3 .3 1) (-16 -16 -16) (16 16 16)
*/
gitem_t gI_weapon_fbfg =
{
	"weapon_fbfg",
		Pickup_Weapon,
		Use_Weapon,
		Drop_Weapon,
		Weapon_FBFG,
		"misc/w_pkup.wav",
		"models/weapons/g_bfg/tris.md2", EF_ROTATE,
		"models/weapons/v_bfg/tris.md2",
		/* icon */		"w_bfg",
		/* pickup */	"FBFG",
		0,
		50,
		&gI_ammo_cells,
		IT_WEAPON|IT_STAY_COOP,
		NULL,
		0,
		/* precache */ "sprites/s_bfg1.sp2 sprites/s_bfg2.sp2 sprites/s_bfg3.sp2 weapons/bfg__f1y.wav weapons/bfg__l1a.wav weapons/bfg__x1b.wav weapons/bfg_hum.wav"
};

/*QUAKED weapon_sniper (.3 .3 1) (-16 -16 -16) (16 16 16)
*/
gitem_t gI_weapon_sniper =
{
	"weapon_snipergun", 
		Pickup_Weapon,
		Use_Weapon,
		Drop_Weapon,
		Weapon_Sniper,
		"misc/w_pkup.wav",
		"models/weapons/g_rail/tris.md2", EF_ROTATE,
		"models/weapons/v_rail/tris.md2",
		/* icon */		"w_railgun",
		/* pickup */	"Sniper Gun",
		0,
		5,
		&gI_ammo_shells,
		IT_WEAPON|IT_ALTWEAPON|IT_STAY_COOP,
		NULL,
		0,
		/* precache */ "weapons/rg_hum.wav models/monsters/parasite/tip/tris.md2"
};

/*QUAKED weapon_plasmarifle (.3 .3 1) (-16 -16 -16) (16 16 16)
*/
gitem_t gI_weapon_plasma =
{
	"weapon_plasmarifle", 
		Pickup_Weapon,
		Use_Weapon,
		Drop_Weapon,
		Weapon_Plasma,
		"misc/w_pkup.wav",
		"models/weapons/g_hyperb/tris.md2", EF_ROTATE,
		"models/weapons/v_hyperb/tris.md2",
		/* icon */		"w_hyperblaster",
		/* pickup */	"Plasma Rifle",
		0,
		1,
		&gI_ammo_cells,
		IT_WEAPON|IT_ALTWEAPON|IT_STAY_COOP,
		NULL,
		0,
		/* precache */ "weapons/rg_hum.wav"
};


//
// AMMO ITEMS
//

/*QUAKED ammo_shells (.3 .3 1) (-16 -16 -16) (16 16 16)
*/
gitem_t gI_ammo_shells =
{
	"ammo_shells",
		Pickup_Ammo,
		NULL,
		Drop_Ammo,
		NULL,
		"misc/am_pkup.wav",
		"models/items/ammo/shells/medium/tris.md2", 0,
		NULL,
		/* icon */		"a_shells",
		/* pickup */	"Shells",
		/* width */		3,
		15,
		NULL,
		IT_AMMO,
		NULL,
		AMMO_SHELLS,
		/* precache */ ""
};

/*QUAKED ammo_bullets (.3 .3 1) (-16 -16 -16) (16 16 16)
*/
gitem_t gI_ammo_bullets =
{
	"ammo_bullets",
		Pickup_Ammo,
		NULL,
		Drop_Ammo,
		NULL,
		"misc/am_pkup.wav",
		"models/items/ammo/bullets/medium/tris.md2", 0,
		NULL,
		/* icon */		"a_bullets",
		/* pickup */	"Bullets",
		/* width */		3,
		75,
		NULL,
		IT_AMMO,
		NULL,
		AMMO_BULLETS,
		/* precache */ ""
};

/*QUAKED ammo_cells (.3 .3 1) (-16 -16 -16) (16 16 16)
*/
gitem_t gI_ammo_cells =
{
	"ammo_cells",
		Pickup_Ammo,
		NULL,
		Drop_Ammo,
		NULL,
		"misc/am_pkup.wav",
		"models/items/ammo/cells/medium/tris.md2", 0,
		NULL,
		/* icon */		"a_cells",
		/* pickup */	"Cells",
		/* width */		3,
		75,
		NULL,
		IT_AMMO,
		NULL,
		AMMO_CELLS,
		/* precache */ ""
};

/*QUAKED ammo_rockets (.3 .3 1) (-16 -16 -16) (16 16 16)
*/
gitem_t gI_ammo_rockets =
{
	"ammo_rockets",
		Pickup_Ammo,
		NULL,
		Drop_Ammo,
		NULL,
		"misc/am_pkup.wav",
		"models/items/ammo/rockets/medium/tris.md2", 0,
		NULL,
		/* icon */		"a_rockets",
		/* pickup */	"Rockets",
		/* width */		3,
		10,
		NULL,
		IT_AMMO,
		NULL,
		AMMO_ROCKETS,
		/* precache */ ""
};

/*QUAKED ammo_slugs (.3 .3 1) (-16 -16 -16) (16 16 16)
*/
gitem_t gI_ammo_slugs =
{
	"ammo_slugs",
		Pickup_Ammo,
		NULL,
		Drop_Ammo,
		NULL,
		"misc/am_pkup.wav",
		"models/items/ammo/slugs/medium/tris.md2", 0,
		NULL,
		/* icon */		"a_slugs",
		/* pickup */	"Slugs",
		/* width */		3,
		75,
		NULL,
		IT_AMMO,
		NULL,
		AMMO_CELLS,
		/* precache */ ""
};

//
// POWERUP ITEMS
//
/*QUAKED item_jetpack (.3 .3 1) (-16 -16 -16) (16 16 16) */
gitem_t gI_item_jetpack =
{ 
	"item_quad",
		Pickup_Powerup,
		Use_Jet,
		/* No dropping function for jetpack */
		NULL,
		NULL,
		"items/pkup.wav",
		/* this will show the monster icarus instead of quad damage in the game */
		"models/monsters/hover/tris.md2",
		EF_ROTATE, NULL,
		/* icon */ "p_quad",
		/* Ok, its the quad icon on screen but who cares */
		/* pickup */	"Jetpack",	/* now we can use it with the use command */
		/* width */ 2,
		/* respawn time */ 60,
		NULL,
		IT_POWERUP,
		NULL,
		0,
		/* precache */
		"hover/hovidle1.wav items/damage.wav items/damage2.wav items/damage3.wav"
};

/*QUAKED item_quad (.3 .3 1) (-16 -16 -16) (16 16 16)
*/
gitem_t gI_item_quad =
{
	"item_oldquad", 
		Pickup_Powerup,
		Use_Quad,
		Drop_General,
		NULL,
		"items/pkup.wav",
		"models/items/quaddama/tris.md2", EF_ROTATE,
		NULL,
		/* icon */		"p_quad",
		/* pickup */	"Quad Damage",
		/* width */		2,
		/* respawn time */60,
		NULL,
		IT_POWERUP,
		NULL,
		0,
		/* precache */ "items/damage.wav items/damage2.wav items/damage3.wav"
};

/*QUAKED item_invulnerability (.3 .3 1) (-16 -16 -16) (16 16 16)
*/
gitem_t gI_item_invulnerability =
{
	"item_invulnerability",
		Pickup_Powerup,
		Use_Invulnerability,
		Drop_General,
		NULL,
		"items/pkup.wav",
		"models/items/invulner/tris.md2", EF_ROTATE,
		NULL,
		/* icon */		"p_invulnerability",
		/* pickup */	"Invulnerability",
		/* width */		2,
		300,
		NULL,
		IT_POWERUP,
		NULL,
		0,
		/* precache */ "items/protect.wav items/protect2.wav items/protect4.wav"
};

/*QUAKED item_silencer (.3 .3 1) (-16 -16 -16) (16 16 16)
*/
gitem_t gI_item_silencer =
{
	"item_silencer",
		Pickup_Powerup,
		Use_Silencer,
		Drop_General,
		NULL,
		"items/pkup.wav",
		"models/items/silencer/tris.md2", EF_ROTATE,
		NULL,
		/* icon */		"p_silencer",
		/* pickup */	"Silencer",
		/* width */		2,
		60,
		NULL,
		IT_POWERUP,
		NULL,
		0,
		/* precache */ ""
};

/*QUAKED item_breather (.3 .3 1) (-16 -16 -16) (16 16 16)
*/
gitem_t gI_item_breather =
{
	"item_breather",
		Pickup_Powerup,
		Use_Breather,
		Drop_General,
		NULL,
		"items/pkup.wav",
		"models/items/breather/tris.md2", EF_ROTATE,
		NULL,
		/* icon */		"p_rebreather",
		/* pickup */	"Rebreather",
		/* width */		2,
		60,
		NULL,
		IT_STAY_COOP|IT_POWERUP,
		NULL,
		0,
		/* precache */ "items/airout.wav"
};

/*QUAKED item_enviro (.3 .3 1) (-16 -16 -16) (16 16 16)
*/
gitem_t gI_item_enviro =
{
	"item_enviro",
		Pickup_Powerup,
		Use_Envirosuit,
		Drop_General,
		NULL,
		"items/pkup.wav",
		"models/items/enviro/tris.md2", EF_ROTATE,
		NULL,
		/* icon */		"p_envirosuit",
		/* pickup */	"Environment Suit",
		/* width */		2,
		60,
		NULL,
		IT_STAY_COOP|IT_POWERUP,
		NULL,
		0,
		/* precache */ "items/airout.wav"
};

/*QUAKED item_ancient_head (.3 .3 1) (-16 -16 -16) (16 16 16)
Special item that gives +2 to maximum health
*/
gitem_t gI_item_ancient_head =
{
	"item_ancient_head",
		Pickup_AncientHead,
		NULL,
		NULL,
		NULL,
		"items/pkup.wav",
		"models/items/c_head/tris.md2", EF_ROTATE,
		NULL,
		/* icon */		"i_fixme",
		/* pickup */	"Ancient Head",
		/* width */		2,
		60,
		NULL,
		IT_NOT_DM,
		NULL,
		0,
		/* precache */ ""
};

/*QUAKED item_adrenaline (.3 .3 1) (-16 -16 -16) (16 16 16)
gives +1 to maximum health
*/
gitem_t gI_item_adrenaline =
{
	"item_adrenaline",
		Pickup_Adrenaline,
		NULL,
		NULL,
		NULL,
		"items/pkup.wav",
		"models/items/adrenal/tris.md2", EF_ROTATE,
		NULL,
		/* icon */		"p_adrenaline",
		/* pickup */	"Adrenaline",
		/* width */		2,
		60,
		NULL,
		0,
		NULL,
		0,
		/* precache */ ""
};

/*QUAKED item_bandolier (.3 .3 1) (-16 -16 -16) (16 16 16)
*/
gitem_t gI_item_bandolier =
{
	"item_bandolier",
		Pickup_Bandolier,
		NULL,
		NULL,
		NULL,
		"items/pkup.wav",
		"models/items/band/tris.md2", EF_ROTATE,
		NULL,
		/* icon */		"p_bandolier",
		/* pickup */	"Bandolier",
		/* width */		2,
		60,
		NULL,
		0,
		NULL,
		0,
		/* precache */ ""
};

/*QUAKED item_pack (.3 .3 1) (-16 -16 -16) (16 16 16)
*/
gitem_t gI_item_pack =
{
	"item_pack",
		Pickup_Pack,
		NULL,
		NULL,
		NULL,
		"items/pkup.wav",
		"models/items/pack/tris.md2", EF_ROTATE,
		NULL,
		/* icon */		"i_pack",
		/* pickup */	"Ammo Pack",
		/* width */		2,
		180,
		NULL,
		0,
		NULL,
		0,
		/* precache */ ""
};

//
// KEYS
//
/*QUAKED key_data_cd (0 .5 .8) (-16 -16 -16) (16 16 16)
key for computer centers
*/
gitem_t gI_key_data_cd =
{
	"key_data_cd",
		Pickup_Key,
		NULL,
		Drop_General,
		NULL,
		"items/pkup.wav",
		"models/items/keys/data_cd/tris.md2", EF_ROTATE,
		NULL,
		"k_datacd",
		"Data CD",
		2,
		0,
		NULL,
		IT_STAY_COOP|IT_KEY|IT_NOT_DM,
		NULL,
		0,
		/* precache */ ""
};

/*QUAKED key_power_cube (0 .5 .8) (-16 -16 -16) (16 16 16) TRIGGER_SPAWN NO_TOUCH
warehouse circuits
*/
gitem_t gI_key_power_cube =
{
	"key_power_cube",
		Pickup_Key,
		NULL,
		Drop_General,
		NULL,
		"items/pkup.wav",
		"models/items/keys/power/tris.md2", EF_ROTATE,
		NULL,
		"k_powercube",
		"Power Cube",
		2,
		0,
		NULL,
		IT_STAY_COOP|IT_KEY|IT_NOT_DM,
		NULL,
		0,
		/* precache */ ""
};

/*QUAKED key_pyramid (0 .5 .8) (-16 -16 -16) (16 16 16)
key for the entrance of jail3
*/
gitem_t gI_key_pyramid =
{
	"key_pyramid",
		Pickup_Key,
		NULL,
		Drop_General,
		NULL,
		"items/pkup.wav",
		"models/items/keys/pyramid/tris.md2", EF_ROTATE,
		NULL,
		"k_pyramid",
		"Pyramid Key",
		2,
		0,
		NULL,
		IT_STAY_COOP|IT_KEY|IT_NOT_DM,
		NULL,
		0,
		/* precache */ ""
};

/*QUAKED key_data_spinner (0 .5 .8) (-16 -16 -16) (16 16 16)
key for the city computer
*/
gitem_t gI_key_data_spinner =
{
	"key_data_spinner",
		Pickup_Key,
		NULL,
		Drop_General,
		NULL,
		"items/pkup.wav",
		"models/items/keys/spinner/tris.md2", EF_ROTATE,
		NULL,
		"k_dataspin",
		"Data Spinner",
		2,
		0,
		NULL,
		IT_STAY_COOP|IT_KEY|IT_NOT_DM,
		NULL,
		0,
		/* precache */ ""
};

/*QUAKED key_pass (0 .5 .8) (-16 -16 -16) (16 16 16)
security pass for the security level
*/
gitem_t gI_key_pass =
{
	"key_pass",
		Pickup_Key,
		NULL,
		Drop_General,
		NULL,
		"items/pkup.wav",
		"models/items/keys/pass/tris.md2", EF_ROTATE,
		NULL,
		"k_security",
		"Security Pass",
		2,
		0,
		NULL,
		IT_STAY_COOP|IT_KEY|IT_NOT_DM,
		NULL,
		0,
		/* precache */ ""
};

/*QUAKED key_blue_key (0 .5 .8) (-16 -16 -16) (16 16 16)
normal door key - blue
*/
gitem_t gI_key_blue_key =
{
	"key_blue_key",
		Pickup_Key,
		NULL,
		Drop_General,
		NULL,
		"items/pkup.wav",
		"models/items/keys/key/tris.md2", EF_ROTATE,
		NULL,
		"k_bluekey",
		"Blue Key",
		2,
		0,
		NULL,
		IT_STAY_COOP|IT_KEY|IT_NOT_DM,
		NULL,
		0,
		/* precache */ ""
};

/*QUAKED key_red_key (0 .5 .8) (-16 -16 -16) (16 16 16)
normal door key - red
*/
gitem_t gI_key_red_key =
{
	"key_red_key",
		Pickup_Key,
		NULL,
		Drop_General,
		NULL,
		"items/pkup.wav",
		"models/items/keys/red_key/tris.md2", EF_ROTATE,
		NULL,
		"k_redkey",
		"Red Key",
		2,
		0,
		NULL,
		IT_STAY_COOP|IT_KEY|IT_NOT_DM,
		NULL,
		0,
		/* precache */ ""
};

/*QUAKED key_commander_head (0 .5 .8) (-16 -16 -16) (16 16 16)
tank commander's head
*/
gitem_t gI_key_commander_head =
{
	"key_commander_head",
		Pickup_Key,
		NULL,
		Drop_General,
		NULL,
		"items/pkup.wav",
		"models/monsters/commandr/head/tris.md2", EF_GIB,
		NULL,
		/* icon */		"k_comhead",
		/* pickup */	"Commander's Head",
		/* width */		2,
		0,
		NULL,
		IT_STAY_COOP|IT_KEY|IT_NOT_DM,
		NULL,
		0,
		/* precache */ ""
};

/*QUAKED key_airstrike_target (0 .5 .8) (-16 -16 -16) (16 16 16)
tank commander's head
*/
gitem_t gI_key_airstrike_target =
{
	"key_airstrike_target",
		Pickup_Key,
		NULL,
		Drop_General,
		NULL,
		"items/pkup.wav",
		"models/items/keys/target/tris.md2", EF_ROTATE,
		NULL,
		/* icon */		"i_airstrike",
		/* pickup */	"Airstrike Marker",
		/* width */		2,
		0,
		NULL,
		IT_STAY_COOP|IT_KEY|IT_NOT_DM,
		NULL,
		0,
		/* precache */ ""
};

gitem_t gI_item_health =
{
	NULL,
		Pickup_Health,
		NULL,
		NULL,
		NULL,
		"items/pkup.wav",
		NULL, 0,
		NULL,
		/* icon */		"i_health",
		/* pickup */	"Health",
		/* width */		3,
		0,
		NULL,
		0,
		NULL,
		0,
		/* precache */ "items/s_health.wav items/n_health.wav items/l_health.wav items/m_health.wav"
};

gitem_t gI_item_null2 =
{
	NULL
};	// end of list marker

// Note: if you modify itemlist[], be sure to update itemlistSorted[], below.
// I apologize for the extra effort.
gitem_t *itemlist[] = 
{
	&gI_item_null1,
		
		&gI_item_armor_body,
		&gI_item_armor_combat,
		&gI_item_armor_jacket,
		&gI_item_armor_shard,
		&gI_item_power_screen,
		&gI_item_power_shield,
		
		&gI_weapon_blaster,
		&gI_weapon_flaregun,
		&gI_weapon_antiflaregun,
		&gI_weapon_superblaster,
		&gI_weapon_mace,
		&gI_weapon_shotgun,
		&gI_weapon_explosiveshotgun,
		&gI_weapon_sniper,
		&gI_weapon_supershotgun,
		&gI_weapon_superrailshotgun,
		&gI_weapon_freezer,
		&gI_weapon_explosivesupershotgun,
		&gI_weapon_machinegun,
		&gI_weapon_machine,
		&gI_weapon_machinerocketgun,
		&gI_weapon_chaingun,
		&gI_weapon_explosivechaingun,
		&gI_weapon_streetsweeper,
		&gI_weapon_explosivestreetsweeper,
		&gI_weapon_railgunsweeper,
		&gI_weapon_bazookasweeper,
		&gI_weapon_stickygrenadesweeper,
		&gI_weapon_grenadesweeper,
		&gI_weapon_superblastersweeper,
		&gI_weapon_freezersweeper,	
		&gI_weapon_flamesweeper,
		&gI_weapon_snipersweeper,
		&gI_weapon_chunkgun,
		&gI_weapon_hyperblaster,
		&gI_weapon_positron,
		&gI_weapon_freezatron,
		&gI_weapon_disintegrator,
		&gI_weapon_icecubegun,
		&gI_weapon_lightninggun,
		&gI_weapon_nailgun,
		&gI_weapon_explosivemachinegun,
		&gI_weapon_airfist,
		&gI_weapon_pulserifle,
		&gI_weapon_sword,
		&gI_ammo_grenades,
		// Grenade-selection code expects the alt. grenade types to be bracketed
		// by gI_ammo_grenades and gI_weapon_grenadelauncher, so if you need to
		// change that, you'll have to fix the grenade-selection code.
		&gI_weapon_clustergrenade,
		&gI_weapon_railbomb,
		&gI_weapon_plasmagrenade,
		&gI_weapon_napalmgrenade,
		&gI_weapon_shrapnelgrenade,
		&gI_weapon_cataclysm,
		&gI_weapon_plasmaclustergrenade,
		&gI_weapon_telegrenade,
		&gI_weapon_freezegrenade,
		&gI_weapon_homergrenade,
		&gI_weapon_rocketgrenade,
		&gI_weapon_blindinggrenade,
		&gI_weapon_positrongrenade,
		&gI_weapon_midnightgrenade,
		&gI_weapon_freezatrongrenade,
		&gI_weapon_banzaigrenade,
		&gI_weapon_spinningrailbomb,
		&gI_weapon_grenadelauncher,

		&gI_weapon_stickinggrenadelauncher,
		&gI_weapon_bucky,
		&gI_weapon_bazooka,
		&gI_weapon_floatingminelauncher,
		&gI_weapon_doubleimpact,
		&gI_weapon_rocketlauncher,
		&gI_weapon_homing,
		&gI_weapon_spiralrocketlauncher,
		&gI_weapon_guided,
		&gI_weapon_guidednuke,
		&gI_weapon_skipper,
		&gI_weapon_plasma,
		&gI_weapon_railgun,
		&gI_weapon_railgun2,
		&gI_weapon_sonicrailgun,
		&gI_weapon_wallpiercingrailgun,
		&gI_weapon_kaminit,
		&gI_weapon_nuke,
		&gI_weapon_antimattercannon,
		&gI_weapon_durg,
		&gI_weapon_bfg,
		&gI_weapon_fbfg,
		&gI_weapon_vacuummaker,
		&gI_weapon_energyvortex,
		
		&gI_ammo_shells,
		&gI_ammo_bullets,
		&gI_ammo_cells,
		&gI_ammo_rockets,
		&gI_ammo_slugs,
		
		&gI_item_jetpack,
		&gI_item_quad,
		&gI_item_invulnerability,
		&gI_item_silencer,
		&gI_item_breather,
		&gI_item_enviro,
		&gI_item_ancient_head,
		&gI_item_adrenaline,
		&gI_item_bandolier,
		&gI_item_pack,
		
		&gI_key_data_cd,
		&gI_key_power_cube,
		&gI_key_pyramid,
		&gI_key_data_spinner,
		&gI_key_pass,
		&gI_key_blue_key,
		&gI_key_red_key,
		&gI_key_commander_head,
		&gI_key_airstrike_target,
		&gI_item_health,
		
		&gI_item_null2
};

// A copy of itemlist[], except it gets qsort()ed on Q_stricmp (pickup_name)
gitem_t *itemlistSorted[] = 
{
	&gI_item_armor_body,
		&gI_item_armor_combat,
		&gI_item_armor_jacket,
		&gI_item_armor_shard,
		&gI_item_power_screen,
		&gI_item_power_shield,
		
		&gI_weapon_blaster,
		&gI_weapon_mace,
		&gI_weapon_flaregun,
		&gI_weapon_antiflaregun,
		&gI_weapon_shotgun,
		&gI_weapon_explosiveshotgun,
		&gI_weapon_supershotgun,
		&gI_weapon_superrailshotgun,
		&gI_weapon_explosivesupershotgun,
		&gI_weapon_machine,
		&gI_weapon_machinegun,
		&gI_weapon_machinerocketgun,
		&gI_weapon_freezer,
		&gI_weapon_chaingun,
		&gI_weapon_explosivechaingun,
		&gI_weapon_streetsweeper,
		&gI_weapon_explosivestreetsweeper,
		&gI_weapon_railgunsweeper,
		&gI_weapon_bazookasweeper,
		&gI_weapon_stickygrenadesweeper,
		&gI_weapon_grenadesweeper,
		&gI_weapon_superblastersweeper,
		&gI_weapon_freezersweeper,
		&gI_weapon_flamesweeper,
		&gI_weapon_snipersweeper,
		&gI_weapon_chunkgun,
		&gI_weapon_hyperblaster,
		&gI_weapon_positron,
		&gI_weapon_freezatron,
		&gI_weapon_disintegrator,
		&gI_weapon_icecubegun,
		&gI_weapon_lightninggun,
		&gI_weapon_nailgun,
		&gI_weapon_explosivemachinegun,
		&gI_weapon_airfist,
		&gI_weapon_pulserifle,
		&gI_weapon_sword,
		&gI_ammo_grenades,
		&gI_weapon_clustergrenade,
		&gI_weapon_railbomb,
		&gI_weapon_plasmagrenade,
		&gI_weapon_napalmgrenade,
		&gI_weapon_shrapnelgrenade,
		&gI_weapon_cataclysm,
		&gI_weapon_plasmaclustergrenade,
		&gI_weapon_telegrenade,
		&gI_weapon_freezegrenade,
		&gI_weapon_homergrenade,
		&gI_weapon_rocketgrenade,
		&gI_weapon_blindinggrenade,
		&gI_weapon_positrongrenade,
		&gI_weapon_midnightgrenade,
		&gI_weapon_freezatrongrenade,
		&gI_weapon_banzaigrenade,
		&gI_weapon_spinningrailbomb,
		&gI_weapon_grenadelauncher,
		&gI_weapon_stickinggrenadelauncher,
		&gI_weapon_bucky,
		&gI_weapon_bazooka,
		&gI_weapon_floatingminelauncher,
		&gI_weapon_doubleimpact,
		&gI_weapon_rocketlauncher,
		&gI_weapon_spiralrocketlauncher,
		&gI_weapon_guided,
		&gI_weapon_guidednuke,
		&gI_weapon_skipper,
		&gI_weapon_homing,
		&gI_weapon_plasma,
		&gI_weapon_superblaster,
		&gI_weapon_railgun,
		&gI_weapon_railgun2,
		&gI_weapon_sonicrailgun,
		&gI_weapon_wallpiercingrailgun,
		&gI_weapon_kaminit,
		&gI_weapon_antimattercannon,
		&gI_weapon_nuke,
		&gI_weapon_durg,
		&gI_weapon_bfg,
		&gI_weapon_fbfg,
		&gI_weapon_vacuummaker,
		&gI_weapon_energyvortex,
		&gI_weapon_sniper,
		
		&gI_ammo_shells,
		&gI_ammo_bullets,
		&gI_ammo_cells,
		&gI_ammo_rockets,
		&gI_ammo_slugs,
		
		&gI_item_jetpack,
		&gI_item_quad,
		&gI_item_invulnerability,
		&gI_item_silencer,
		&gI_item_breather,
		&gI_item_enviro,
		&gI_item_ancient_head,
		&gI_item_adrenaline,
		&gI_item_bandolier,
		&gI_item_pack,
		
		&gI_key_data_cd,
		&gI_key_power_cube,
		&gI_key_pyramid,
		&gI_key_data_spinner,
		&gI_key_pass,
		&gI_key_blue_key,
		&gI_key_red_key,
		&gI_key_commander_head,
		&gI_key_airstrike_target,
		&gI_item_health,
};


/*QUAKED item_health (.3 .3 1) (-16 -16 -16) (16 16 16)
*/
void SP_item_health (edict_t *self)
{
	if ( deathmatch->value && ((int)dmflags->value & DF_NO_HEALTH) )
	{
		G_FreeEdict (self);
		return;
	}
	
	self->model = "models/items/healing/medium/tris.md2";
	self->count = 10;
	SpawnItem (self, &gI_item_health);
	gi.soundindex ("items/n_health.wav");
}

/*QUAKED item_health_small (.3 .3 1) (-16 -16 -16) (16 16 16)
*/
void SP_item_health_small (edict_t *self)
{
	if ( deathmatch->value && ((int)dmflags->value & DF_NO_HEALTH) )
	{
		G_FreeEdict (self);
		return;
	}
	
	self->model = "models/items/healing/stimpack/tris.md2";
	self->count = 2;
	SpawnItem (self, &gI_item_health);
	self->style = HEALTH_IGNORE_MAX;
	gi.soundindex ("items/s_health.wav");
}

/*QUAKED item_health_large (.3 .3 1) (-16 -16 -16) (16 16 16)
*/
void SP_item_health_large (edict_t *self)
{
	if ( deathmatch->value && ((int)dmflags->value & DF_NO_HEALTH) )
	{
		G_FreeEdict (self);
		return;
	}
	
	self->model = "models/items/healing/large/tris.md2";
	self->count = 25;
	SpawnItem (self, &gI_item_health);
	gi.soundindex ("items/l_health.wav");
}

/*QUAKED item_health_mega (.3 .3 1) (-16 -16 -16) (16 16 16)
*/
void SP_item_health_mega (edict_t *self)
{
	if ( deathmatch->value && ((int)dmflags->value & DF_NO_HEALTH) )
	{
		G_FreeEdict (self);
		return;
	}
	
	self->model = "models/items/mega_h/tris.md2";
	self->count = 100;
	SpawnItem (self, &gI_item_health);
	gi.soundindex ("items/m_health.wav");
	self->style = HEALTH_IGNORE_MAX|HEALTH_TIMED;
}

int CompareItemsByPickup (const void *a, const void *b)
{
	return Q_stricmp ((*(gitem_t **)a)->pickup_name,
		(*(gitem_t **)b)->pickup_name);
}


void InitItems (void)
{
	int i;
	
	// set the # of items
	game.num_items = sizeof(itemlist)/sizeof(itemlist[0]) - 1;
	
	// set each item's index
	for (i = 0; i <= game.num_items; i++)
		itemlist[i]->item_index = i;
	
	// sort the items, so they can be found quickly
	qsort ((void *)itemlistSorted,
		sizeof (itemlistSorted) / sizeof (itemlistSorted[0]),
		sizeof (itemlistSorted[0]), CompareItemsByPickup);
}


/*
* ===============
*    FindItem
*
* ===============
*/
gitem_t	*FindItem (char *pickup_name)
{
	int low, high, curr, icmp;
	gitem_t *it;
	
	// Do a binary search.
	low = 0;
	high = sizeof (itemlistSorted) / sizeof (itemlistSorted[0]);
	while (low < high)
	{
		curr = (low + high) >> 1;
		it = itemlistSorted[curr];
		icmp = Q_stricmp (it->pickup_name, pickup_name);
		if (icmp > 0)
			high = curr;
		else if (icmp < 0)
			low = curr + 1;
		else
			return it;
	}
	
	// We didn't find it.
	return NULL;
}



/*
* ===============
*  SetItemNames
*
*  Called by SP_worldspawn()
* ===============
*/
void SetItemNames (void)
{
	int		i;
	gitem_t	*it;
	
	for (i=1 ; i < game.num_items ; i++)
	{
		it = itemlist[i];
		if (!(deathmatch->value) || !(it->flags & IT_NOT_DM))
			gi.configstring (CS_ITEMS + i, it->pickup_name);
	}
	
	jacket_armor_index = ITEM_INDEX (&gI_item_armor_jacket);
	combat_armor_index = ITEM_INDEX (&gI_item_armor_combat);
	body_armor_index   = ITEM_INDEX (&gI_item_armor_body);
	power_screen_index = ITEM_INDEX (&gI_item_power_screen);
	power_shield_index = ITEM_INDEX (&gI_item_power_shield);
}

