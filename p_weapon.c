/*
Copyright (C) 1997-2001 Id Software, Inc.

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/

#include "g_local.h"
#include "m_player.h"
#include "l_angels.h"
#include "l_rockets.h"
#include "x_fbomb.h"

// darKMajick definitions
#include "l_dm_grenades.h"

//QW// Globals
qboolean is_quad;
byte     is_silenced;
//QW//

void SetSweeperMode(edict_t* ent);

#define DEFAULT_EXBULLET_HSPREAD  500
#define DEFAULT_EXBULLET_VSPREAD  600

void P_ProjectSource(gclient_t* client, vec3_t point, vec3_t distance, vec3_t forward, vec3_t right, vec3_t result)
{
	vec3_t	_distance = { 0 };

	VectorCopy(distance, _distance);
	if (client->pers.hand == LEFT_HANDED)
		_distance[1] *= -1;
	else if (client->pers.hand == CENTER_HANDED)
		_distance[1] = 0;
	G_ProjectSource(point, _distance, forward, right, result);
}

/*
===============
PlayerNoise

Each player can have two noise objects associated with it:
a personal noise (jumping, pain, weapon firing), and a weapon
target noise (bullet wall impacts)

Monsters that don't directly see the player can move
to a noise in hopes of seeing the player from there.
===============
*/
void PlayerNoise(edict_t* who, vec3_t where, int type)
{
	edict_t* noise;

	if (type == PNOISE_WEAPON)
	{
		if (who->client->silencer_shots)
		{
			who->client->silencer_shots--;
			return;
		}
	}

	if (deathmatch->value)
		return;

	if (who->flags & FL_NOTARGET)
		return;


	if (!who->mynoise)
	{
		noise = G_Spawn();
		noise->classname = "player_noise";
		VectorSet(noise->mins, -8, -8, -8);
		VectorSet(noise->maxs, 8, 8, 8);
		noise->owner = who;
		noise->svflags = SVF_NOCLIENT;
		who->mynoise = noise;

		noise = G_Spawn();
		noise->classname = "player_noise";
		VectorSet(noise->mins, -8, -8, -8);
		VectorSet(noise->maxs, 8, 8, 8);
		noise->owner = who;
		noise->svflags = SVF_NOCLIENT;
		who->mynoise2 = noise;
	}

	if (type == PNOISE_SELF || type == PNOISE_WEAPON)
	{
		noise = who->mynoise;
		level.sound_entity = noise;
		level.sound_entity_framenum = level.framenum;
	}
	else // type == PNOISE_IMPACT
	{
		noise = who->mynoise2;
		level.sound2_entity = noise;
		level.sound2_entity_framenum = level.framenum;
	}

	VectorCopy(where, noise->s.origin);
	VectorSubtract(where, noise->maxs, noise->absmin);
	VectorAdd(where, noise->maxs, noise->absmax);
	noise->teleport_time = level.time;
	gi.linkentity(noise);
}

void Weapon_Recoil(edict_t* self, float recoil)
{
	vec3_t	end, forward;
	vec3_t	dir = { 0 };
	trace_t tr;

	if (self->client) // only players get recoil
	{
		AngleVectors(self->client->v_angle, forward, NULL, NULL);

		// check if near a wall
		VectorAdd(self->s.origin, self->client->ps.viewoffset, dir);
		VectorMA(dir, 64, forward, end);
		tr = gi.trace(dir, NULL, NULL, end, self, MASK_SOLID);

		// Enhance recoil if we're hitting a close wall or floor
		if (tr.fraction != 1) // 1 means we didn't hit anything
			recoil += recoil * (1 - tr.fraction) * 3; // more recoil on hit

		if (self->groundentity)
		{
			self->groundentity = NULL;
			self->s.origin[2]++;
		}

		//apply recoil
		VectorMA(self->velocity, -recoil, forward, self->velocity);
	}
}

qboolean Pickup_Weapon(edict_t* ent, edict_t* other)
{
	int			index;
	gitem_t* item;	// the item we touched
	gitem_t* item1, * item2, * item3, * item4, * item5, \
		* item6, * item7, * item8, * item9, * item10, * item11, \
		* item12, * item13, * item14, * item15;	//inventory items
	gitem_t* ammo;

	item = ent->item;

	// HACK to use when editing maps for custom entities or Extinction.
	// we print the location of the weapon once to the console when we touch it.
	if (ext_devt->value && other->client->last_touched_weapon != item)
	{
		gi.cprintf(other, PRINT_HIGH, "Weapon %s at [%i,%i,%i]\n", item->pickup_name,
			(int)ent->s.origin[0], (int)ent->s.origin[1],
			(int)ent->s.origin[2]);
		other->client->last_touched_weapon = item; //trap door interlock
	}

	// Determine what weapon was picked up and what weapons will be added to the
	// inventory, depending on weapon banning.

	item1 = item2 = item3 = item4 = item5 = item6 = item7 = item8 = item9 = item10 = item11 = item12 = item13 = item14 = item15 = NULL;

	if (item == &gI_weapon_shotgun ||
		item == &gI_weapon_sniper ||
		item == &gI_weapon_airfist ||
		item == &gI_weapon_sword ||
		item == &gI_weapon_explosiveshotgun ||
		item == &gI_weapon_superrailshotgun)
	{
		if (!(i_weaponban & WB_SHOTGUN))
			item1 = &gI_weapon_shotgun;
		else
			item = &gI_weapon_sniper;
		if (!(i_weaponban & WB_SNIPERGUN))
			item2 = &gI_weapon_sniper;
		if (!(i_loxweaponban & LWB_AIRFIST))
			item3 = &gI_weapon_airfist;
		if (!(i_loxweaponban & LWB_SWORD))
			item4 = &gI_weapon_sword;
		if (!(i_loxweaponban & LWB_EXPLOSIVESHOTGUN))
			item5 = &gI_weapon_explosiveshotgun;
		if (!(i_loxweaponban & LWB_SUPERRAILSHOTGUN))
			item6 = &gI_weapon_superrailshotgun;
	}

	else if (item == &gI_weapon_supershotgun ||
		item == &gI_weapon_freezer ||
		item == &gI_weapon_explosivesupershotgun ||
		item == &gI_weapon_superrailshotgun)
	{
		if (!(i_weaponban & WB_SUPERSHOTGUN))
			item1 = &gI_weapon_supershotgun;
		else
			item = &gI_weapon_freezer;
		if (!(i_weaponban & WB_FREEZEGUN))
			item2 = &gI_weapon_freezer;
		if (!(i_loxweaponban & LWB_EXPLOSIVESUPERSHOTGUN))
			item3 = &gI_weapon_explosivesupershotgun;
		if (!(i_loxweaponban & LWB_SUPERRAILSHOTGUN))
			item4 = &gI_weapon_superrailshotgun;
	}

	else if (item == &gI_weapon_machinegun ||
		item == &gI_weapon_machine ||
		item == &gI_weapon_machinerocketgun ||
		item == &gI_weapon_pulserifle ||
		item == &gI_weapon_nailgun ||
		item == &gI_weapon_explosivemachinegun)
	{
		if (!(i_weaponban & WB_MACHINEROCKETGUN))
			item1 = &gI_weapon_machinegun;
		else
			item = &gI_weapon_machine;
		if (!(i_weaponban & WB_MACHINEGUN))
			item2 = &gI_weapon_machine;
		if (!(i_loxweaponban & LWB_PULSERIFLE))
			item3 = &gI_weapon_pulserifle;
		if (!(i_loxweaponban & LWB_NAILGUN))
			item4 = &gI_weapon_nailgun;
		if (!(i_loxweaponban & LWB_EXPLOSIVEMACHINEGUN))
			item5 = &gI_weapon_explosivemachinegun;
		if (!(i_loxweaponban & WB_MACHINEROCKETGUN))
			item6 = &gI_weapon_machinerocketgun;
	}

	else if (item == &gI_weapon_chaingun ||
		item == &gI_weapon_streetsweeper ||
		item == &gI_weapon_superblastersweeper ||
		item == &gI_weapon_freezersweeper ||
		item == &gI_weapon_grenadesweeper ||
		item == &gI_weapon_bazookasweeper ||
		item == &gI_weapon_railgunsweeper ||
		item == &gI_weapon_chunkgun ||
		item == &gI_weapon_flamesweeper ||
		item == &gI_weapon_snipersweeper ||
		item == &gI_weapon_explosivechaingun ||
		item == &gI_weapon_explosivestreetsweeper ||
		item == &gI_weapon_stickygrenadesweeper)
	{
		if (!(i_weaponban & WB_CHAINGUN))
			item1 = &gI_weapon_chaingun;
		else
			item = &gI_weapon_streetsweeper;
		if (!(i_weaponban & WB_STREETSWEEPER))
			item2 = &gI_weapon_streetsweeper;
		if (!(i_loxsweeperban & LSB_SUPERBLASTERSWEEPER))
			item3 = &gI_weapon_superblastersweeper;
		if (!(i_loxsweeperban & LSB_FREEZERSWEEPER))
			item4 = &gI_weapon_freezersweeper;
		if (!(i_loxsweeperban & LSB_GRENADESWEEPER))
			item5 = &gI_weapon_grenadesweeper;
		if (!(i_loxsweeperban & LSB_BAZOOKASWEEPER))
			item6 = &gI_weapon_bazookasweeper;
		if (!(i_loxsweeperban & LSB_RAILGUNSWEEPER))
			item7 = &gI_weapon_railgunsweeper;
		if (!(i_loxsweeperban & LSB_CHUNKGUN))
			item8 = &gI_weapon_chunkgun;
		if (!(i_loxsweeperban & LSB_FLAMESWEEPER))
			item9 = &gI_weapon_flamesweeper;
		if (!(i_loxsweeperban & LSB_SNIPERSWEEPER))
			item10 = &gI_weapon_snipersweeper;
		if (!(i_loxsweeperban & LSB_EXPLOSIVECHAINGUN))
			item11 = &gI_weapon_explosivechaingun;
		if (!(i_loxsweeperban & LSB_EXPLOSIVESTREETSWEEPER))
			item12 = &gI_weapon_explosivestreetsweeper;
		if (!(i_loxsweeperban & LSB_STICKYGRENADESWEEPER))
			item13 = &gI_weapon_stickygrenadesweeper;
	}

	else if (item == &gI_weapon_grenadelauncher ||
		item == &gI_weapon_bazooka ||
		item == &gI_weapon_bucky ||
		item == &gI_weapon_stickinggrenadelauncher ||
		item == &gI_weapon_doubleimpact ||
		item == &gI_weapon_floatingminelauncher)
	{
		if (!(i_weaponban & WB_GRENADELAUNCHER))
			item1 = &gI_weapon_grenadelauncher;
		else
			item = &gI_weapon_bazooka;
		if (!(i_weaponban & WB_BAZOOKA))
			item2 = &gI_weapon_bazooka;
		if (!(i_loxweaponban & LWB_BUCKY))
			item3 = &gI_weapon_bucky;
		if (!(i_loxweaponban & LWB_STICKGRENADELAUNCHER))
			item4 = &gI_weapon_stickinggrenadelauncher;
		if (!(i_loxweaponban & LWB_DOUBLEIMPACT))
			item5 = &gI_weapon_doubleimpact;
		if (!(i_loxweaponban & LWB_FLOATINGMINELAUNCHER))
			item6 = &gI_weapon_floatingminelauncher;
	}

	else if (item == &gI_weapon_rocketlauncher ||
		item == &gI_weapon_homing ||
		item == &gI_weapon_skipper ||
		item == &gI_weapon_guided ||
		item == &gI_weapon_spiralrocketlauncher ||
		item == &gI_weapon_guidednuke)
	{
		if (!(i_weaponban & WB_ROCKETLAUNCHER))
			item1 = &gI_weapon_rocketlauncher;
		else
			item = &gI_weapon_homing;
		if (!(i_weaponban & WB_HOMINGROCKETLAUNCHER))
			item2 = &gI_weapon_homing;
		if (!(i_loxweaponban & LWB_SKIPPERHOMINGROCKET))
			item3 = &gI_weapon_skipper;
		if (!(i_loxweaponban & LWB_GUIDEDMISSILE))
			item4 = &gI_weapon_guided;
		if (!(i_loxweaponban & LWB_SPIRALROCKETLAUNCHER))
			item5 = &gI_weapon_spiralrocketlauncher;
		if (!(i_loxweaponban & LWB_GUIDEDNUKE))
			item6 = &gI_weapon_guidednuke;
	}

	else if (item == &gI_weapon_hyperblaster ||
		item == &gI_weapon_plasma ||
		item == &gI_weapon_positron ||
		item == &gI_weapon_freezatron ||
		item == &gI_weapon_icecubegun ||
		item == &gI_weapon_lightninggun ||
		item == &gI_weapon_disintegrator)
	{
		if (!(i_weaponban & WB_HYPERBLASTER))
			item1 = &gI_weapon_hyperblaster;
		else
			item = &gI_weapon_plasma;
		if (!(i_weaponban & WB_PLASMARIFLE))
			item2 = &gI_weapon_plasma;
		if (!(i_loxweaponban & LWB_POSITRON))
			item3 = &gI_weapon_positron;
		if (!(i_loxweaponban & LWB_FREEZATRON))
			item4 = &gI_weapon_freezatron;
		if (!(i_loxweaponban & LWB_ICECUBEGUN))
			item5 = &gI_weapon_icecubegun;
		if (!(i_loxweaponban & LWB_LIGHTNINGGUN))
			item6 = &gI_weapon_lightninggun;
		if (!(i_loxweaponban & LWB_DISINTEGRATOR))
			item7 = &gI_weapon_disintegrator;
	}

	else if (item == &gI_weapon_railgun ||
		item == &gI_weapon_railgun2 ||
		item == &gI_weapon_durg ||
		item == &gI_weapon_sonicrailgun ||
		item == &gI_weapon_kaminit ||
		item == &gI_weapon_wallpiercingrailgun ||
		item == &gI_weapon_nuke ||
		item == &gI_weapon_antimattercannon)
	{
		if (!(i_weaponban & WB_FLAMETHROWER))
			item1 = &gI_weapon_railgun;
		else
			item = &gI_weapon_railgun2;
		if (!(i_weaponban & WB_RAILGUN))
			item2 = &gI_weapon_railgun2;
		if (!(i_loxweaponban & LWB_DURG))
			item4 = &gI_weapon_durg;
		if (!(i_loxweaponban & LWB_SONICRAILGUN))
			item5 = &gI_weapon_sonicrailgun;
		if (!(i_loxweaponban & LWB_KAMINIT))
			item6 = &gI_weapon_kaminit;
		if (!(i_loxweaponban & LWB_WALLPIERCINGRAILGUN))
			item7 = &gI_weapon_wallpiercingrailgun;
		if (!(i_loxweaponban & LWB_NUKE))
			item8 = &gI_weapon_nuke;
		if (!(i_loxweaponban & LWB_ANTIMATTERCANNON))
			item9 = &gI_weapon_antimattercannon;
	}

	else if (item == &gI_weapon_bfg ||
		item == &gI_weapon_fbfg ||
		item == &gI_weapon_vacuummaker ||
		item == &gI_weapon_energyvortex)
	{
		if (!(i_weaponban & WB_BFG10K))
			item1 = &gI_weapon_bfg;
		if (!(i_loxweaponban & LWB_FBFG))
			item2 = &gI_weapon_fbfg;
		if (!(i_loxweaponban & LWB_VACUUMMAKER))
			item3 = &gI_weapon_vacuummaker;
		if (!(i_loxweaponban & LWB_ENERGYVORTEX))
			item4 = &gI_weapon_energyvortex;
	}


	index = ITEM_INDEX(item);
	ammo = item->ammo;

	if ((((int)(dmflags->value) & DF_WEAPONS_STAY) || coop->value)
		&& other->client->pers.inventory[index])
	{
		if (!(ent->spawnflags & (DROPPED_ITEM | DROPPED_PLAYER_ITEM)))
			return false;	// leave the weapon for others to pickup
		if (ammo && !Add_Ammo(other, ammo, 0))
			return false;	// gun *and* full ammo already
	}

	// Now add the weapons to their inventory.
	if (item1)
		other->client->pers.inventory[ITEM_INDEX(item1)]++;
	if (item2)
		other->client->pers.inventory[ITEM_INDEX(item2)]++;
	if (item3)
		other->client->pers.inventory[ITEM_INDEX(item3)]++;
	if (item4)
		other->client->pers.inventory[ITEM_INDEX(item4)]++;
	if (item5)
		other->client->pers.inventory[ITEM_INDEX(item5)]++;
	if (item6)
		other->client->pers.inventory[ITEM_INDEX(item6)]++;
	if (item7)
		other->client->pers.inventory[ITEM_INDEX(item7)]++;
	if (item8)
		other->client->pers.inventory[ITEM_INDEX(item8)]++;
	if (item9)
		other->client->pers.inventory[ITEM_INDEX(item9)]++;
	if (item10)
		other->client->pers.inventory[ITEM_INDEX(item10)]++;
	if (item11)
		other->client->pers.inventory[ITEM_INDEX(item11)]++;
	if (item12)
		other->client->pers.inventory[ITEM_INDEX(item12)]++;
	if (item13)
		other->client->pers.inventory[ITEM_INDEX(item13)]++;
	if (item14)
		other->client->pers.inventory[ITEM_INDEX(item14)]++;
	if (item15)
		other->client->pers.inventory[ITEM_INDEX(item15)]++;

	if (!(ent->spawnflags & DROPPED_ITEM))
	{
		if (ammo)
		{
			// give them some ammo with it
			if ((int)dmflags->value & DF_INFINITE_AMMO)
				Add_Ammo(other, ammo, 1000);
			else
				Add_Ammo(other, ammo, ammo->quantity * 2);
		}

		if (!(ent->spawnflags & DROPPED_PLAYER_ITEM))
		{
			if (deathmatch->value)
			{
				if ((int)(dmflags->value) & DF_WEAPONS_STAY)
				{
					// If the weapon hasn't been moved, do a quick in-place respawn,
					// otherwise do a more visual respawn.
					if (VectorCompare(ent->pos1, ent->s.origin)
						&& VectorCompare(ent->pos2, ent->s.angles))
						ent->flags |= FL_RESPAWN;
					else
						SetRespawn(ent, 2 * FRAMETIME);
				}
				else
					SetRespawn(ent, 30);
			}
			if (coop->value)
				ent->flags |= FL_RESPAWN;
		}
	}

	if (other->client->pers.weapon != item &&
		(other->client->pers.inventory[index] == 1) &&
		((!deathmatch->value && autoweaponselect->value) || other->client->pers.weapon == &gI_weapon_blaster))
		other->client->newweapon = item;

	return true;
}



/*
===============
ChangeWeapon

The old weapon has been dropped all the way, so make the new one
current
===============
*/

// ### Hentai ### BEGIN
void ShowGun(edict_t* ent)
{
	char heldmodel[128];

	if (!ent->client->pers.weapon)
	{
		ent->client->ps.gunindex = 0;		// WI: seems to be missing?
		ent->s.modelindex2 = 0;
		gi.dprintf("ShowGun: Oops! Weapon Index missing! %s\n", ent->client->pers.netname);
		return;
	}

	ent->s.modelindex2 = 255;
	strcpy(heldmodel, "#");	//safe
	strcat(heldmodel, ent->client->pers.weapon->icon);
	strcat(heldmodel, ".md2");

	// held model is mapped onto the player skinnum
	int n = (gi.modelindex(heldmodel) - vwep_index) << 8;
	ent->s.skinnum &= 0xFF;
	ent->s.skinnum |= n;
	//DbgPrintf("ShowGun: %s index 0x%x\n", heldmodel, n);
}
// ### Hentai ### END

void ChangeWeapon(edict_t* ent)
{
	if (ent->client->grenade_time)
	{
		ent->client->grenade_time = level.time;
		ent->client->weapon_sound = 0;
		weapon_grenade_fire(ent, false);
		ent->client->grenade_time = 0;
	}

	ent->client->pers.lastweapon = ent->client->pers.weapon;
	ent->client->pers.weapon = ent->client->newweapon;
	ent->client->newweapon = NULL;
	ent->client->machinegun_shots = 0;

	if (ent->client->pers.weapon && ent->client->pers.weapon->ammo)
		ent->client->ammo_index = ITEM_INDEX(ent->client->pers.weapon->ammo);
	else
		ent->client->ammo_index = 0;

	if (!ent->client->pers.weapon || ent->s.modelindex != 255 || ent->deadflag) // ### Hentai ### 
	{	// dead, or not on client, so VWep animations could do wacky things
		ent->client->ps.gunindex = 0;
		ent->s.modelindex2 = 0;
		return;
	}

	ent->client->weaponstate = WEAPON_ACTIVATING;
	ent->client->ps.gunframe = 0;
	//Wyrm: chasecam & turret
	//if ((!ent->client->chasetoggle)&&(!ent->client->onturret))
	//	ent->client->ps.gunindex = gi.modelindex(ent->client->pers.weapon->view_model);
	ent->client->ps.gunindex = gi.modelindex(ent->client->pers.weapon->view_model);

	// ### Hentai ### BEGIN
	ent->client->anim_priority = ANIM_PAIN;
	if (ent->client->ps.pmove.pm_flags & PMF_DUCKED)
	{
		ent->s.frame = FRAME_crpain1;
		ent->client->anim_end = FRAME_crpain4;
	}
	else
	{
		ent->s.frame = FRAME_pain301;
		ent->client->anim_end = FRAME_pain304;

	}

	SetSweeperMode(ent);

	ShowGun(ent);

	// ### Hentai ### END
}


// is sweeper the selected weapon?
void SetSweeperMode(edict_t* ent)
{
	gitem_t* item;

	item = ent->client->pers.weapon;

	if (item == &gI_weapon_chaingun ||
		item == &gI_weapon_streetsweeper ||
		item == &gI_weapon_superblastersweeper ||
		item == &gI_weapon_freezersweeper ||
		item == &gI_weapon_grenadesweeper ||
		item == &gI_weapon_bazookasweeper ||
		item == &gI_weapon_railgunsweeper ||
		item == &gI_weapon_chunkgun ||
		item == &gI_weapon_flamesweeper ||
		item == &gI_weapon_snipersweeper ||
		item == &gI_weapon_explosivechaingun ||
		item == &gI_weapon_explosivestreetsweeper ||
		item == &gI_weapon_stickygrenadesweeper)

		ent->client->sweeperactive = true;
	else
		ent->client->sweeperactive = false;

}
/*
=================
NoAmmoWeaponChange
=================
*/
void NoAmmoWeaponChange(edict_t* ent)
{
	// Plasma rifle
	if (ent->client->pers.inventory[ITEM_INDEX(gI_weapon_plasma.ammo)]
		>= gI_weapon_plasma.quantity
		&& ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_plasma)])
	{
		ent->client->newweapon = &gI_weapon_plasma;
	}

	// Streetsweeper
	else if (ent->client->pers.inventory[ITEM_INDEX(gI_weapon_streetsweeper.ammo)]
		>= gI_weapon_streetsweeper.quantity
		&& ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_streetsweeper)])
	{
		ent->client->newweapon = &gI_weapon_streetsweeper;
	}

	// Explosive streetsweeper
	else if (ent->client->pers.inventory[ITEM_INDEX(gI_weapon_explosivestreetsweeper.ammo)]
		>= gI_weapon_explosivestreetsweeper.quantity
		&& ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_explosivestreetsweeper)])
	{
		ent->client->newweapon = &gI_weapon_explosivestreetsweeper;
	}

	// Railgun sweeper
	else if (ent->client->pers.inventory[ITEM_INDEX(gI_weapon_railgunsweeper.ammo)]
		>= gI_weapon_railgunsweeper.quantity
		&& ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_railgunsweeper)])
	{
		ent->client->newweapon = &gI_weapon_railgunsweeper;
	}

	// Bazooka sweeper
	else if (ent->client->pers.inventory[ITEM_INDEX(gI_weapon_bazookasweeper.ammo)]
		>= gI_weapon_bazookasweeper.quantity
		&& ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_bazookasweeper)])
	{
		ent->client->newweapon = &gI_weapon_bazookasweeper;
	}

	// Sniper sweeper
	else if (ent->client->pers.inventory[ITEM_INDEX(gI_weapon_snipersweeper.ammo)]
		>= gI_weapon_snipersweeper.quantity
		&& ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_snipersweeper)])
	{
		ent->client->newweapon = &gI_weapon_snipersweeper;
	}

	// Superblaster sweeper
	else if (ent->client->pers.inventory[ITEM_INDEX(gI_weapon_superblastersweeper.ammo)]
		>= gI_weapon_superblastersweeper.quantity
		&& ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_superblastersweeper)])
	{
		ent->client->newweapon = &gI_weapon_superblastersweeper;
	}

	// Grenade sweeper
	else if (ent->client->pers.inventory[ITEM_INDEX(gI_weapon_grenadesweeper.ammo)]
		>= gI_weapon_grenadesweeper.quantity
		&& ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_grenadesweeper)])
	{
		ent->client->newweapon = &gI_weapon_grenadesweeper;
	}

	// Freezer sweeper
	else if (ent->client->pers.inventory[ITEM_INDEX(gI_weapon_freezersweeper.ammo)]
		>= gI_weapon_freezersweeper.quantity
		&& ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_freezersweeper)])
	{
		ent->client->newweapon = &gI_weapon_freezersweeper;
	}

	// Flame sweeper
	else if (ent->client->pers.inventory[ITEM_INDEX(gI_weapon_flamesweeper.ammo)]
		>= gI_weapon_flamesweeper.quantity
		&& ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_flamesweeper)])
	{
		ent->client->newweapon = &gI_weapon_flamesweeper;
	}

	// Superblaster
	else if (ent->client->pers.inventory[ITEM_INDEX(gI_weapon_superblaster.ammo)]
		>= gI_weapon_superblaster.quantity
		&& ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_superblaster)])
	{
		ent->client->newweapon = &gI_weapon_superblaster;
	}

	// Chunkgun
	else if (ent->client->pers.inventory[ITEM_INDEX(gI_weapon_chunkgun.ammo)]
		>= gI_weapon_chunkgun.quantity
		&& ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_chunkgun)])
	{
		ent->client->newweapon = &gI_weapon_chunkgun;
	}

	// Sticky grenade sweeper
	else if (ent->client->pers.inventory[ITEM_INDEX(gI_weapon_stickygrenadesweeper.ammo)]
		>= gI_weapon_stickygrenadesweeper.quantity
		&& ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_stickygrenadesweeper)])
	{
		ent->client->newweapon = &gI_weapon_stickygrenadesweeper;
	}

	// Machine rocket gun
	else if (ent->client->pers.inventory[ITEM_INDEX(gI_weapon_machinegun.ammo)]
		>= gI_weapon_machinegun.quantity
		&& ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_machinegun)])
	{
		ent->client->newweapon = &gI_weapon_machinegun;
	}

	// Nailgun
	else if (ent->client->pers.inventory[ITEM_INDEX(gI_weapon_nailgun.ammo)]
		>= gI_weapon_nailgun.quantity
		&& ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_nailgun)])
	{
		ent->client->newweapon = &gI_weapon_nailgun;
	}

	// Explosive machinegun
	else if (ent->client->pers.inventory[ITEM_INDEX(gI_weapon_explosivemachinegun.ammo)]
		>= gI_weapon_explosivemachinegun.quantity
		&& ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_explosivemachinegun)])
	{
		ent->client->newweapon = &gI_weapon_explosivemachinegun;
	}

	// Pulse rifle
	else if (ent->client->pers.inventory[ITEM_INDEX(gI_weapon_pulserifle.ammo)]
		>= gI_weapon_pulserifle.quantity
		&& ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_pulserifle)])
	{
		ent->client->newweapon = &gI_weapon_pulserifle;
	}

	// Hyperblaster
	else if (ent->client->pers.inventory[ITEM_INDEX(gI_weapon_hyperblaster.ammo)]
		>= gI_weapon_hyperblaster.quantity
		&& ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_hyperblaster)])
	{
		ent->client->newweapon = &gI_weapon_hyperblaster;
	}

	// Positron
	else if (ent->client->pers.inventory[ITEM_INDEX(gI_weapon_positron.ammo)]
		>= gI_weapon_positron.quantity
		&& ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_positron)])
	{
		ent->client->newweapon = &gI_weapon_positron;
	}

	// Freezatron
	else if (ent->client->pers.inventory[ITEM_INDEX(gI_weapon_freezatron.ammo)]
		>= gI_weapon_freezatron.quantity
		&& ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_freezatron)])
	{
		ent->client->newweapon = &gI_weapon_freezatron;
	}

	// Lightning gun
	else if (ent->client->pers.inventory[ITEM_INDEX(gI_weapon_lightninggun.ammo)]
		>= gI_weapon_lightninggun.quantity
		&& ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_lightninggun)])
	{
		ent->client->newweapon = &gI_weapon_lightninggun;
	}

	// Icecube gun
	else if (ent->client->pers.inventory[ITEM_INDEX(gI_weapon_icecubegun.ammo)]
		>= gI_weapon_icecubegun.quantity
		&& ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_icecubegun)])
	{
		ent->client->newweapon = &gI_weapon_icecubegun;
	}

	// Chaingun
	else if (ent->client->pers.inventory[ITEM_INDEX(gI_weapon_chaingun.ammo)]
		>= gI_weapon_chaingun.quantity
		&& ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_chaingun)])
	{
		ent->client->newweapon = &gI_weapon_chaingun;
	}

	// Explosive Chaingun
	else if (ent->client->pers.inventory[ITEM_INDEX(gI_weapon_explosivechaingun.ammo)]
		>= gI_weapon_explosivechaingun.quantity
		&& ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_explosivechaingun)])
	{
		ent->client->newweapon = &gI_weapon_explosivechaingun;
	}

	// Flame rocket launcher
	else if (ent->client->pers.inventory[ITEM_INDEX(gI_weapon_rocketlauncher.ammo)]
		>= gI_weapon_rocketlauncher.quantity
		&& ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_rocketlauncher)])
	{
		ent->client->newweapon = &gI_weapon_rocketlauncher;
	}

	// Spiral Rocket launcher
	else if (ent->client->pers.inventory[ITEM_INDEX(gI_weapon_spiralrocketlauncher.ammo)]
		>= gI_weapon_spiralrocketlauncher.quantity
		&& ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_spiralrocketlauncher)])
	{
		ent->client->newweapon = &gI_weapon_spiralrocketlauncher;
	}

	// Homing rocketlauncher
	else if (ent->client->pers.inventory[ITEM_INDEX(gI_weapon_homing.ammo)]
		>= gI_weapon_homing.quantity
		&& ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_homing)])
	{
		ent->client->newweapon = &gI_weapon_homing;
	}

	// Skipper rocketlauncher
	else if (ent->client->pers.inventory[ITEM_INDEX(gI_weapon_skipper.ammo)]
		>= gI_weapon_skipper.quantity
		&& ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_skipper)])
	{
		ent->client->newweapon = &gI_weapon_skipper;
	}

	// Guided rocketlauncher
	else if (ent->client->pers.inventory[ITEM_INDEX(gI_weapon_guided.ammo)]
		>= gI_weapon_guided.quantity
		&& ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_guided)])
	{
		ent->client->newweapon = &gI_weapon_guided;
	}

	// Standard machinegun
	else if (ent->client->pers.inventory[ITEM_INDEX(gI_weapon_machine.ammo)]
		>= gI_weapon_machine.quantity
		&& ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_machine)])
	{
		ent->client->newweapon = &gI_weapon_machine;
	}

	// Super shotgun
	else if (ent->client->pers.inventory[ITEM_INDEX(gI_weapon_supershotgun.ammo)]
		>= gI_weapon_supershotgun.quantity
		&& ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_supershotgun)])
	{
		ent->client->newweapon = &gI_weapon_supershotgun;
	}

	// Explosive super shotgun
	else if (ent->client->pers.inventory[ITEM_INDEX(gI_weapon_explosivesupershotgun.ammo)]
		>= gI_weapon_explosivesupershotgun.quantity
		&& ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_explosivesupershotgun)])
	{
		ent->client->newweapon = &gI_weapon_explosivesupershotgun;
	}

	// Super rail shotgun
	else if (ent->client->pers.inventory[ITEM_INDEX(gI_weapon_superrailshotgun.ammo)]
		>= gI_weapon_superrailshotgun.quantity
		&& ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_superrailshotgun)])
	{
		ent->client->newweapon = &gI_weapon_superrailshotgun;
	}

	// Shotgun
	else if (ent->client->pers.inventory[ITEM_INDEX(gI_weapon_shotgun.ammo)]
		>= gI_weapon_shotgun.quantity
		&& ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_shotgun)])
	{
		ent->client->newweapon = &gI_weapon_shotgun;
	}

	// Super blaster
	else if (ent->client->pers.inventory[ITEM_INDEX(gI_weapon_superblaster.ammo)]
		>= gI_weapon_superblaster.quantity
		&& ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_superblaster)])
	{
		ent->client->newweapon = &gI_weapon_superblaster;
	}

	// Bazooka
	else if (ent->client->pers.inventory[ITEM_INDEX(gI_weapon_bazooka.ammo)]
		>= ent->client->dM_ammoCost
		&& ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_bazooka)])
	{
		ent->client->newweapon = &gI_weapon_bazooka;
	}

	// Sniper gun
	else if (ent->client->pers.inventory[ITEM_INDEX(gI_weapon_sniper.ammo)]
		>= gI_weapon_sniper.quantity
		&& ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_sniper)])
	{
		ent->client->newweapon = &gI_weapon_sniper;
	}

	// Grenade launcher
	else if (ent->client->pers.inventory[ITEM_INDEX(gI_weapon_grenadelauncher.ammo)]
		>= ent->client->dM_ammoCost
		&& ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_grenadelauncher)])
	{
		ent->client->newweapon = &gI_weapon_grenadelauncher;
	}

	// Double Impact launcher
	else if (ent->client->pers.inventory[ITEM_INDEX(gI_weapon_doubleimpact.ammo)]
		>= ent->client->dM_ammoCost
		&& ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_doubleimpact)])
	{
		ent->client->newweapon = &gI_weapon_doubleimpact;
	}

	// Floating mine launcher
	else if (ent->client->pers.inventory[ITEM_INDEX(gI_weapon_floatingminelauncher.ammo)]
		>= ent->client->dM_ammoCost
		&& ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_floatingminelauncher)])
	{
		ent->client->newweapon = &gI_weapon_floatingminelauncher;
	}

	// Buckey Ball launcher
	else if (ent->client->pers.inventory[ITEM_INDEX(gI_weapon_bucky.ammo)]
		>= ent->client->dM_ammoCost
		&& ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_bucky)])
	{
		ent->client->newweapon = &gI_weapon_bucky;
	}

	// Explosive shotgun
	else if (ent->client->pers.inventory[ITEM_INDEX(gI_weapon_explosiveshotgun.ammo)]
		>= gI_weapon_explosiveshotgun.quantity
		&& ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_explosiveshotgun)])
	{
		ent->client->newweapon = &gI_weapon_explosiveshotgun;
	}

	// Railgun
	else if (ent->client->pers.inventory[ITEM_INDEX(gI_weapon_railgun2.ammo)]
		>= gI_weapon_railgun2.quantity
		&& ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_railgun2)])
	{
		ent->client->newweapon = &gI_weapon_railgun2;
	}

	// Antimatter cannon
	else if (ent->client->pers.inventory[ITEM_INDEX(gI_weapon_antimattercannon.ammo)]
		>= gI_weapon_antimattercannon.quantity
		&& ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_antimattercannon)])
	{
		ent->client->newweapon = &gI_weapon_antimattercannon;
	}

	// Flamethrower
	else if (ent->client->pers.inventory[ITEM_INDEX(gI_weapon_railgun.ammo)]
		>= gI_weapon_railgun.quantity
		&& ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_railgun)])
	{
		ent->client->newweapon = &gI_weapon_railgun;
	}

	// Sonic railgun
	else if (ent->client->pers.inventory[ITEM_INDEX(gI_weapon_sonicrailgun.ammo)]
		>= gI_weapon_sonicrailgun.quantity
		&& ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_sonicrailgun)])
	{
		ent->client->newweapon = &gI_weapon_sonicrailgun;
	}

	// Disintegrator
	else if (ent->client->pers.inventory[ITEM_INDEX(gI_weapon_disintegrator.ammo)]
		>= gI_weapon_disintegrator.quantity
		&& ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_disintegrator)])
	{
		ent->client->newweapon = &gI_weapon_disintegrator;
	}

	// Durg
	else if (ent->client->pers.inventory[ITEM_INDEX(gI_weapon_durg.ammo)]
		>= gI_weapon_durg.quantity
		&& ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_durg)])
	{
		ent->client->newweapon = &gI_weapon_durg;
	}

	// Kaminit
	else if (ent->client->pers.inventory[ITEM_INDEX(gI_weapon_kaminit.ammo)]
		>= gI_weapon_kaminit.quantity
		&& ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_kaminit)])
	{
		ent->client->newweapon = &gI_weapon_kaminit;
	}

	// Wall piercing railgun
	else if (ent->client->pers.inventory[ITEM_INDEX(gI_weapon_wallpiercingrailgun.ammo)]
		>= gI_weapon_wallpiercingrailgun.quantity
		&& ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_wallpiercingrailgun)])
	{
		ent->client->newweapon = &gI_weapon_wallpiercingrailgun;
	}

	// Nuke
	else if (ent->client->pers.inventory[ITEM_INDEX(gI_weapon_nuke.ammo)]
		>= gI_weapon_nuke.quantity
		&& ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_nuke)])
	{
		ent->client->newweapon = &gI_weapon_nuke;
	}

	// Guided nuke
	else if (ent->client->pers.inventory[ITEM_INDEX(gI_weapon_guidednuke.ammo)]
		>= gI_weapon_guidednuke.quantity
		&& ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_guidednuke)])
	{
		ent->client->newweapon = &gI_weapon_guidednuke;
	}


	// No other choice, give them the blaster.
	else
		ent->client->newweapon = &gI_weapon_blaster;
}

/*
=================
Think_Weapon

Called by ClientBeginServerFrame and ClientThink
=================
*/
void Think_Weapon(edict_t* ent)
{
	// if just died, put the weapon away
	if (ent->health <= 0)
	{
		ent->client->newweapon = NULL;
		ChangeWeapon(ent);
	}

	// call active weapon think routine
	if (ent->client->pers.weapon && ent->client->pers.weapon->weaponthink)
	{
		is_quad = (ent->client->quad_framenum > level.framenum);
		if (ent->client->silencer_shots)
			is_silenced = MZ_SILENCED;
		else
			is_silenced = 0;
		ent->client->pers.weapon->weaponthink(ent);
	}
}


//================
//  Make the weapon ready if there is ammo
//================
void Use_Weapon(edict_t* ent, gitem_t* item)
{
	int			ammo_index;
	gitem_t* ammo_item;

	// see if we're already using it
	if (item == ent->client->pers.weapon)
		return;

	if (item->ammo && !g_select_empty->value && !(item->flags & IT_AMMO))
	{
		ammo_item = item->ammo;
		ammo_index = ITEM_INDEX(ammo_item);

		if (!ent->client->pers.inventory[ammo_index])
		{
			gi.cprintf(ent, PRINT_HIGH, "No %s for %s.\n", ammo_item->pickup_name, item->pickup_name);
			return;
		}

		if (ent->client->pers.inventory[ammo_index] < item->quantity)
		{
			gi.cprintf(ent, PRINT_HIGH, "Not enough %s for %s.\n", ammo_item->pickup_name, item->pickup_name);
			return;
		}
	}

	// change to this weapon when down
	ent->client->newweapon = item;
	if (item == &gI_weapon_chaingun ||
		item == &gI_weapon_streetsweeper ||
		item == &gI_weapon_superblastersweeper ||
		item == &gI_weapon_freezersweeper ||
		item == &gI_weapon_grenadesweeper ||
		item == &gI_weapon_bazookasweeper ||
		item == &gI_weapon_railgunsweeper ||
		item == &gI_weapon_chunkgun ||
		item == &gI_weapon_flamesweeper ||
		item == &gI_weapon_snipersweeper ||
		item == &gI_weapon_explosivechaingun ||
		item == &gI_weapon_explosivestreetsweeper ||
		item == &gI_weapon_stickygrenadesweeper)
	{
		if (weaponheat->value)
			ent->client->heat_active = true;
		else
			ent->client->heat_active = false;
	}
	else
		ent->client->heat_active = false;
}



/*
================
Drop_Weapon
================
*/
void Drop_Weapon(edict_t* ent, gitem_t* item)
{
	gitem_t* altitem = NULL;
	int index;
	int altindex = 0;

	if ((int)(dmflags->value) & DF_WEAPONS_STAY)
		return;

	// Find the counterpart to this weapon.
	if (item == &gI_weapon_shotgun)
		altitem = &gI_weapon_explosiveshotgun;
	else if (item == &gI_weapon_explosiveshotgun)
		altitem = &gI_weapon_sniper;
	else if (item == &gI_weapon_sniper)
		altitem = &gI_weapon_airfist;
	else if (item == &gI_weapon_airfist)
		altitem = &gI_weapon_shotgun;
	else if (item == &gI_weapon_supershotgun)
		altitem = &gI_weapon_superrailshotgun;
	else if (item == &gI_weapon_superrailshotgun)
		altitem = &gI_weapon_explosivesupershotgun;
	else if (item == &gI_weapon_explosivesupershotgun)
		altitem = &gI_weapon_freezer;
	else if (item == &gI_weapon_freezer)
		altitem = &gI_weapon_supershotgun;
	else if (item == &gI_weapon_machinegun)
		altitem = &gI_weapon_machine;
	else if (item == &gI_weapon_machine)
		altitem = &gI_weapon_pulserifle;
	else if (item == &gI_weapon_pulserifle)
		altitem = &gI_weapon_explosivemachinegun;
	else if (item == &gI_weapon_explosivemachinegun)
		altitem = &gI_weapon_nailgun;
	else if (item == &gI_weapon_nailgun)
		altitem = &gI_weapon_machinegun;
	else if (item == &gI_weapon_chaingun)
		altitem = &gI_weapon_explosivechaingun;
	else if (item == &gI_weapon_explosivechaingun)
		altitem = &gI_weapon_streetsweeper;
	else if (item == &gI_weapon_streetsweeper)
		altitem = &gI_weapon_explosivestreetsweeper;
	else if (item == &gI_weapon_explosivestreetsweeper)
		altitem = &gI_weapon_superblastersweeper;
	else if (item == &gI_weapon_superblastersweeper)
		altitem = &gI_weapon_freezersweeper;
	else if (item == &gI_weapon_freezersweeper)
		altitem = &gI_weapon_flamesweeper;
	else if (item == &gI_weapon_flamesweeper)
		altitem = &gI_weapon_grenadesweeper;
	else if (item == &gI_weapon_grenadesweeper)
		altitem = &gI_weapon_snipersweeper;
	else if (item == &gI_weapon_snipersweeper)
		altitem = &gI_weapon_bazookasweeper;
	else if (item == &gI_weapon_bazookasweeper)
		altitem = &gI_weapon_railgunsweeper;
	else if (item == &gI_weapon_railgunsweeper)
		altitem = &gI_weapon_stickygrenadesweeper;
	else if (item == &gI_weapon_stickygrenadesweeper)
		altitem = &gI_weapon_chunkgun;
	else if (item == &gI_weapon_chunkgun)
		altitem = &gI_weapon_chaingun;
	else if (item == &gI_weapon_grenadelauncher)
		altitem = &gI_weapon_doubleimpact;
	else if (item == &gI_weapon_doubleimpact)
		altitem = &gI_weapon_bucky;
	else if (item == &gI_weapon_bucky)
		altitem = &gI_weapon_floatingminelauncher;
	else if (item == &gI_weapon_floatingminelauncher)
		altitem = &gI_weapon_bazooka;
	else if (item == &gI_weapon_bazooka)
		altitem = &gI_weapon_stickinggrenadelauncher;
	else if (item == &gI_weapon_stickinggrenadelauncher)
		altitem = &gI_weapon_grenadelauncher;
	else if (item == &gI_weapon_rocketlauncher)
		altitem = &gI_weapon_spiralrocketlauncher;
	else if (item == &gI_weapon_spiralrocketlauncher)
		altitem = &gI_weapon_homing;
	else if (item == &gI_weapon_homing)
		altitem = &gI_weapon_skipper;
	else if (item == &gI_weapon_skipper)
		altitem = &gI_weapon_guided;
	else if (item == &gI_weapon_guided)
		altitem = &gI_weapon_guidednuke;
	else if (item == &gI_weapon_guidednuke)
		altitem = &gI_weapon_rocketlauncher;
	else if (item == &gI_weapon_hyperblaster)
		altitem = &gI_weapon_plasma;
	else if (item == &gI_weapon_plasma)
		altitem = &gI_weapon_positron;
	else if (item == &gI_weapon_positron)
		altitem = &gI_weapon_freezatron;
	else if (item == &gI_weapon_freezatron)
		altitem = &gI_weapon_icecubegun;
	else if (item == &gI_weapon_icecubegun)
		altitem = &gI_weapon_disintegrator;
	else if (item == &gI_weapon_disintegrator)
		altitem = &gI_weapon_lightninggun;
	else if (item == &gI_weapon_lightninggun)
		altitem = &gI_weapon_hyperblaster;
	else if (item == &gI_weapon_railgun)
		altitem = &gI_weapon_railgun2;
	else if (item == &gI_weapon_railgun2)
		altitem = &gI_weapon_sonicrailgun;
	else if (item == &gI_weapon_sonicrailgun)
		altitem = &gI_weapon_durg;
	else if (item == &gI_weapon_durg)
		altitem = &gI_weapon_kaminit;
	else if (item == &gI_weapon_kaminit)
		altitem = &gI_weapon_wallpiercingrailgun;
	else if (item == &gI_weapon_wallpiercingrailgun)
		altitem = &gI_weapon_antimattercannon;
	else if (item == &gI_weapon_antimattercannon)
		altitem = &gI_weapon_nuke;
	else if (item == &gI_weapon_nuke)
		altitem = &gI_weapon_railgun;
	else if (item == &gI_weapon_bfg)
		altitem = &gI_weapon_energyvortex;
	else if (item == &gI_weapon_energyvortex)
		altitem = &gI_weapon_vacuummaker;
	else if (item == &gI_weapon_vacuummaker)
		altitem = &gI_weapon_fbfg;
	else if (item == &gI_weapon_fbfg)
		altitem = &gI_weapon_fbfg;
	// Get their indexes.
	index = ITEM_INDEX(item);
	if (altitem)
		altindex = ITEM_INDEX(altitem);

	// Decrease their inventory of the dropped weapon.  If they have an equal
	// number of the counterpart weapon, decrease that too.
	if (ent->client->pers.inventory[index] == ent->client->pers.inventory[altindex])
		ent->client->pers.inventory[altindex]--;
	ent->client->pers.inventory[index]--;

	// Finally, drop the weapon.  Don't drop an alternate weapon -- make it a
	// normal weapon.
	/* if (item->flags & IT_ALTWEAPON)
		Drop_Item (ent, altitem);
	else */
	Drop_Item(ent, item);

	// If they dropped the current weapon, switch.
	if ((item == ent->client->pers.weapon && !ent->client->pers.inventory[index])
		|| (altitem == ent->client->pers.weapon && !ent->client->pers.inventory[altindex]))
	{
		NoAmmoWeaponChange(ent);
	}
}


/*
================
Weapon_Generic

A generic function to handle the basics of weapon thinking
================
*/
#define FRAME_FIRE_FIRST		(FRAME_ACTIVATE_LAST + 1)
#define FRAME_IDLE_FIRST		(FRAME_FIRE_LAST + 1)
#define FRAME_DEACTIVATE_FIRST	(FRAME_IDLE_LAST + 1)

void Weapon_Generic(edict_t* ent, int FRAME_ACTIVATE_LAST, int FRAME_FIRE_LAST,
	int FRAME_IDLE_LAST, int FRAME_DEACTIVATE_LAST,
	int* pause_frames, int* fire_frames,
	void (*fire)(edict_t* ent))
{
	int		n;

	if (ent->deadflag || ent->s.modelindex != 255) { // VWep animations screw up corpses
		return;
	}

	if (ent->client->weaponstate == WEAPON_DROPPING)
	{
		if (ent->client->ps.gunframe == FRAME_DEACTIVATE_LAST)
		{
			ChangeWeapon(ent);
			return;
		}
		// ### Hentai ### BEGIN
		else if ((FRAME_DEACTIVATE_LAST - ent->client->ps.gunframe) == 4)
		{
			ent->client->anim_priority = ANIM_REVERSE;
			if (ent->client->ps.pmove.pm_flags & PMF_DUCKED)
			{
				ent->s.frame = FRAME_crpain4 + 1;
				ent->client->anim_end = FRAME_crpain1;
			}
			else
			{
				ent->s.frame = FRAME_pain304 + 1;
				ent->client->anim_end = FRAME_pain301;

			}
		}
		// ### Hentai ### END

		ent->client->ps.gunframe++;
		return;
	}

	if (ent->client->weaponstate == WEAPON_ACTIVATING) {
		if (fast_weapon_change->value) {
			ent->client->ps.gunframe = FRAME_ACTIVATE_LAST;
		}
		if (ent->client->ps.gunframe == FRAME_ACTIVATE_LAST)
		{
			ent->client->weaponstate = WEAPON_READY;
			ent->client->ps.gunframe = FRAME_IDLE_FIRST;
			return;
		}

		ent->client->ps.gunframe++;
		return;
	}

	if ((ent->client->newweapon) && (ent->client->weaponstate != WEAPON_FIRING))
	{
		ent->client->weaponstate = WEAPON_DROPPING;
		if (fast_weapon_change->value) {
			ChangeWeapon(ent);
			return;
		}
		else {
			ent->client->ps.gunframe = FRAME_DEACTIVATE_FIRST;
		}
		if ((FRAME_DEACTIVATE_LAST - FRAME_DEACTIVATE_FIRST) < 4)
		{
			ent->client->anim_priority = ANIM_REVERSE;
			if (ent->client->ps.pmove.pm_flags & PMF_DUCKED)
			{
				ent->s.frame = FRAME_crpain4 + 1;
				ent->client->anim_end = FRAME_crpain1;
			}
			else
			{
				ent->s.frame = FRAME_pain304 + 1;
				ent->client->anim_end = FRAME_pain301;

			}
		}
		// ### Hentai ### END

		return;
	}

	if (ent->client->weaponstate == WEAPON_READY)
	{
		if (((ent->client->latched_buttons | ent->client->buttons) & BUTTON_ATTACK))
		{
			ent->client->latched_buttons &= ~BUTTON_ATTACK;

			if ((!ent->client->ammo_index) ||
				(ent->client->pers.inventory[ent->client->ammo_index]
					>= ent->client->pers.weapon->quantity))
			{
				ent->client->ps.gunframe = FRAME_FIRE_FIRST;
				ent->client->weaponstate = WEAPON_FIRING;

				// ### Hentai ### BEGIN
				// start the animation
				ent->client->anim_priority = ANIM_ATTACK;
				if (ent->client->ps.pmove.pm_flags & PMF_DUCKED)
				{
					ent->s.frame = FRAME_crattak1 - 1;
					ent->client->anim_end = FRAME_crattak9;
				}
				else
				{
					ent->s.frame = FRAME_attack1 - 1;
					ent->client->anim_end = FRAME_attack8;
				}
				// ### Hentai ### END
			}
			else
			{
				if (level.time >= ent->pain_debounce_time)
				{
					gi.sound(ent, CHAN_VOICE, gi.soundindex("weapons/noammo.wav"), 1, ATTN_NORM, 0);
					ent->pain_debounce_time = level.time + 1;
				}
				NoAmmoWeaponChange(ent);
			}
		}
		else
		{
			if (ent->client->ps.gunframe == FRAME_IDLE_LAST)
			{
				ent->client->ps.gunframe = FRAME_IDLE_FIRST;
				return;
			}

			if (pause_frames)
			{
				for (n = 0; pause_frames[n]; n++)
				{
					if (ent->client->ps.gunframe == pause_frames[n])
					{
						if (rand() & 15)
							return;
					}
				}
			}

			ent->client->ps.gunframe++;
			return;
		}
	}

	if (ent->client->weaponstate == WEAPON_FIRING)
	{
		for (n = 0; fire_frames[n]; n++)
		{
			if (ent->client->ps.gunframe == fire_frames[n])
			{
				if (ent->client->quad_framenum > level.framenum)
					gi.sound(ent, CHAN_ITEM, gi.soundindex("items/damage3.wav"), 1, ATTN_NORM, 0);

				fire(ent);
				break;
			}
		}

		if (!fire_frames[n]) {
			ent->client->ps.gunframe++;
			if (ent->client->newweapon && fast_weapon_change->value) {
				ChangeWeapon(ent);
				return;
			}
		}
		else {
			if (ent->client->newweapon && fast_weapon_change->value) {
				ChangeWeapon(ent);
				return;
			}
		}

		if (ent->client->ps.gunframe == FRAME_IDLE_FIRST + 1)
			ent->client->weaponstate = WEAPON_READY;
	}
}


/*
======================================================================

GRENADES

======================================================================
*/

#define GRENADE_TIMER		3.0f
#define GRENADE_MINSPEED	400.0f
#define GRENADE_MAXSPEED	800.0f

void weapon_grenade_fire(edict_t* ent, qboolean held)
{
	vec3_t	offset = { 0 };
	vec3_t	forward, right;
	vec3_t	start;
	int		damage = 125;
	float	timer;
	int		speed;
	float	radius;
	int		typ;

	radius = damage + 40.0f;
	if (is_quad)
		damage *= 4;

	VectorSet(offset, 8, 8, ent->viewheight - 8.0);
	AngleVectors(ent->client->v_angle, forward, right, NULL);
	P_ProjectSource(ent->client, ent->s.origin, offset, forward, right, start);

	timer = ent->client->grenade_time - level.time;
	speed = (int)(GRENADE_MINSPEED + (GRENADE_TIMER - timer) * ((GRENADE_MAXSPEED - GRENADE_MINSPEED) / GRENADE_TIMER));

	// darKMajick:
	typ = ent->client->dM_grenade;
	// fire_grenade2 (ent, start, forward, damage, speed, timer, radius);
	fire_grenade_dM(ent, start, forward, damage, speed, timer, radius, typ,
		/* held */ true, /* bazookad */ false);

	if (!ent->deadflag && ent->s.modelindex == 255)
	{
		// ### Hentai ### BEGIN
		if (ent->client->ps.pmove.pm_flags & PMF_DUCKED)
		{
			ent->client->anim_priority = ANIM_ATTACK;
			ent->s.frame = FRAME_crattak1 - 1;
			ent->client->anim_end = FRAME_crattak3;
		}
		else if (ent->s.modelindex != 255)
		{
			ent->client->anim_priority = ANIM_REVERSE;
			ent->s.frame = FRAME_wave08;
			ent->client->anim_end = FRAME_wave01;
		}
		// ### Hentai ### END
	}

	// Altered to use dM_ammoCost
	if (!((int)dmflags->value & DF_INFINITE_AMMO))
		ent->client->pers.inventory[ent->client->ammo_index]
		-= ent->client->dM_ammoCost;

	ent->client->grenade_time = level.time + 1.0;
}

void Weapon_Grenade(edict_t* ent)
{
	if ((ent->client->newweapon) && (ent->client->weaponstate == WEAPON_READY))
	{
		ChangeWeapon(ent);
		return;
	}

	if (ent->client->weaponstate == WEAPON_ACTIVATING)
	{
		ent->client->weaponstate = WEAPON_READY;
		ent->client->ps.gunframe = 16;
		return;
	}

	if (ent->client->weaponstate == WEAPON_READY)
	{
		if (((ent->client->latched_buttons | ent->client->buttons) & BUTTON_ATTACK))
		{
			ent->client->latched_buttons &= ~BUTTON_ATTACK;
			if (ent->client->pers.inventory[ent->client->ammo_index]
				>= ent->client->dM_ammoCost)
			{
				// Start throwing the grenade.
				ent->client->ps.gunframe = 1;
				ent->client->weaponstate = WEAPON_FIRING;
				ent->client->grenade_time = 0;
			}
			else
			{
				// No grenades, switch to something else.
				if (level.time >= ent->pain_debounce_time)
				{
					gi.sound(ent, CHAN_VOICE, gi.soundindex("weapons/noammo.wav"), 1, ATTN_NORM, 0);
					ent->pain_debounce_time = level.time + 1;
				}
				NoAmmoWeaponChange(ent);
			}
			return;
		}

		if ((ent->client->ps.gunframe == 29) || (ent->client->ps.gunframe == 34) || (ent->client->ps.gunframe == 39) || (ent->client->ps.gunframe == 48))
		{
			if (rand() & 15)
				return;
		}

		if (++ent->client->ps.gunframe > 48)
			ent->client->ps.gunframe = 16;
		return;
	}

	if (ent->client->weaponstate == WEAPON_FIRING)
	{
		if (ent->client->ps.gunframe == 5)
			gi.sound(ent, CHAN_WEAPON, gi.soundindex("weapons/hgrena1b.wav"), 1, ATTN_NORM, 0);

		if (ent->client->ps.gunframe == 11)
		{
			if (!ent->client->grenade_time)
			{
				ent->client->grenade_time = level.time + GRENADE_TIMER + 0.2;
				ent->client->weapon_sound = gi.soundindex("weapons/hgrenc1b.wav");
			}

			// they waited too long, detonate it in their hand
			if (!ent->client->grenade_blew_up && level.time >= ent->client->grenade_time)
			{
				ent->client->weapon_sound = 0;
				weapon_grenade_fire(ent, true);
				ent->client->grenade_blew_up = true;
			}

			if (ent->client->buttons & BUTTON_ATTACK)
				return;

			if (ent->client->grenade_blew_up)
			{
				if (level.time >= ent->client->grenade_time)
				{
					ent->client->ps.gunframe = 15;
					ent->client->grenade_blew_up = false;
				}
				else
				{
					return;
				}
			}
		}

		if (ent->client->ps.gunframe == 12)
		{
			ent->client->weapon_sound = 0;
			weapon_grenade_fire(ent, false);
		}

		if ((ent->client->ps.gunframe == 15) && (level.time < ent->client->grenade_time))
			return;

		ent->client->ps.gunframe++;

		if (ent->client->ps.gunframe == 16)
		{
			ent->client->grenade_time = 0;
			ent->client->weaponstate = WEAPON_READY;
		}
	}
}

void Use_GrenadeWeapon(edict_t* ent, gitem_t* item)
{
	// First, see if we can use this special grenade type.
	Use_Weapon(ent, item);

	// Set it up for use.  (Let them select grenades even if they can't use
	// them.  They'll NoAmmoWeaponChange() immediately.)

	// Make them use the grenades.
	if (item != &gI_ammo_grenades)
		ent->client->newweapon = &gI_ammo_grenades;

	// Set the type of grenade to use.
	if (item == &gI_weapon_plasmaclustergrenade)
		ent->client->dM_grenade = DM_PLASMACLUSTER;
	else if (item == &gI_weapon_telegrenade)
		ent->client->dM_grenade = DM_TELEGRENADE;
	else if (item == &gI_weapon_freezegrenade)
		ent->client->dM_grenade = DM_FREEZEGRENADE;
	else if (item == &gI_weapon_homergrenade)
		ent->client->dM_grenade = DM_HOMERGRENADE;
	else if (item == &gI_weapon_rocketgrenade)
		ent->client->dM_grenade = DM_ROCKETGRENADE;
	else if (item == &gI_weapon_blindinggrenade)
		ent->client->dM_grenade = DM_BLINDINGGRENADE;
	else if (item == &gI_weapon_positrongrenade)
		ent->client->dM_grenade = DM_POSITRONGRENADE;
	else if (item == &gI_weapon_freezatrongrenade)
		ent->client->dM_grenade = DM_FREEZATRONGRENADE;
	else if (item == &gI_weapon_clustergrenade)
		ent->client->dM_grenade = DM_CLUSTERGRENADE;
	else if (item == &gI_weapon_railbomb)
		ent->client->dM_grenade = DM_RAILBOMB;
	else if (item == &gI_weapon_plasmagrenade)
		ent->client->dM_grenade = DM_PLASMAGRENADE;
	else if (item == &gI_weapon_napalmgrenade)
		ent->client->dM_grenade = DM_NAPALMGRENADE;
	else if (item == &gI_weapon_shrapnelgrenade)
		ent->client->dM_grenade = DM_SHRAPNELGRENADE;
	else if (item == &gI_weapon_cataclysm)
		ent->client->dM_grenade = DM_CATACLYSM;
	else if (item == &gI_weapon_midnightgrenade)
		ent->client->dM_grenade = DM_MIDNIGHTGRENADE;
	else if (item == &gI_weapon_banzaigrenade)
		ent->client->dM_grenade = DM_BANZAIGRENADE;
	else if (item == &gI_weapon_spinningrailbomb)
		ent->client->dM_grenade = DM_SPINNINGRAILBOMB;

	// If we get lost, recover.
	else
	{
		ent->client->dM_grenade = DM_NORMALGRENADE;
		ent->client->dM_ammoCost = 1;
	}

	// Set the ammo usage.
	if (ent->client->dM_grenade != 0)
		ent->client->dM_ammoCost = item->quantity;
}

/*
======================================================================

GRENADE LAUNCHER

======================================================================
*/

void weapon_grenadelauncher_fire(edict_t* ent)
{
	vec3_t	offset = { 0 };
	vec3_t	forward, right;
	vec3_t	start;
	int		damage = 120;
	float	radius, timer = 2.5;
	int		speed = 700; //initial speed of the grenade

	ent->client->mine = 0;
	if (ent->client->explosive != NULL)
	{
		Grenade_Explode_dM(ent->client->explosive);
		ent->client->expactive = 1;
	}

	if (ent->client->pers.inventory[ent->client->ammo_index]
		< ent->client->dM_ammoCost)
	{
		// No grenades, switch to something else.
		if (level.time >= ent->pain_debounce_time)
		{
			gi.sound(ent, CHAN_VOICE, gi.soundindex("weapons/noammo.wav"), 1, ATTN_NORM, 0);
			ent->pain_debounce_time = level.time + 1;
		}
		NoAmmoWeaponChange(ent);
		ent->client->ps.gunframe = /* FRAME_IDLE_FIRST */ 17;
		return;
	}

	radius = damage + 40.0;
	if (is_quad)
		damage *= 4;

	VectorSet(offset, 8, 8, ent->viewheight - 8.0);
	AngleVectors(ent->client->v_angle, forward, right, NULL);
	P_ProjectSource(ent->client, ent->s.origin, offset, forward, right, start);

	VectorScale(forward, -2, ent->client->kick_origin);
	ent->client->kick_angles[0] = -1;

	if (ent->client->dM_grenade == DM_NORMALGRENADE)
	{
		fire_flamegrenade(ent, start, forward, damage, speed, timer, radius);
		if (ent->client && ent->client->pers.special == AODEATH)
		{
			fire_flamegrenade(ent, ent->client->angel->s.origin, forward, damage, speed, timer, radius);
		}
	}
	else if (ent->client->expactive == 1)
	{
		fire_grenade_dM(ent, start, forward, damage, speed, 100, radius,
			ent->client->dM_grenade, /* held */ false, /* bazookad */ false);
	}
	else
	{
		fire_grenade_dM(ent, start, forward, damage, speed, timer, radius,
			ent->client->dM_grenade, /* held */ false, /* bazookad */ false);
		if (ent->client && ent->client->pers.special == AODEATH)
		{
			fire_grenade_dM(ent, ent->client->angel->s.origin, forward, damage, speed, timer, radius,
				ent->client->dM_grenade, /* held */ false, /* bazookad */ false);
		}
	}

	gi.WriteByte(svc_muzzleflash);
	gi.WriteShort(ent - g_edicts);
	gi.WriteByte(MZ_GRENADE | is_silenced);
	gi.multicast(ent->s.origin, MULTICAST_PVS);

	ent->client->ps.gunframe++;

	PlayerNoise(ent, start, PNOISE_WEAPON);

	if (!((int)dmflags->value & DF_INFINITE_AMMO))
		ent->client->pers.inventory[ent->client->ammo_index]
		-= ent->client->dM_ammoCost;

}

void Weapon_GrenadeLauncher(edict_t* ent)
{
	static int	pause_frames[] = { 34, 51, 59, 0 };
	static int	fire_frames[] = { 6, 0 };

	Weapon_Generic(ent, 5, 16, 59, 64, pause_frames, fire_frames, weapon_grenadelauncher_fire);
}

void weapon_bazooka_fire(edict_t* ent)
{
	vec3_t	offset = { 0 };
	vec3_t	forward, right;
	vec3_t	start;
	int		damage = 120;
	int		radius;

	ent->client->mine = 0;
	if (ent->client->pers.inventory[ent->client->ammo_index]
		< ent->client->dM_ammoCost)
	{
		// No grenades, switch to something else.
		if (level.time >= ent->pain_debounce_time)
		{
			gi.sound(ent, CHAN_VOICE, gi.soundindex("weapons/noammo.wav"), 1, ATTN_NORM, 0);
			ent->pain_debounce_time = level.time + 1;
		}
		NoAmmoWeaponChange(ent);
		ent->client->ps.gunframe = /* FRAME_IDLE_FIRST */ 17;
		return;
	}

	radius = damage + 40;
	if (is_quad)
		damage *= 4;

	VectorSet(offset, 8, 8, ent->viewheight - 8.0);
	AngleVectors(ent->client->v_angle, forward, right, NULL);
	P_ProjectSource(ent->client, ent->s.origin, offset, forward, right, start);

	VectorScale(forward, -2, ent->client->kick_origin);
	ent->client->kick_angles[0] = -1;

	fire_grenade_dM(ent, start, forward, damage, 500, 2.5, radius,
		ent->client->dM_grenade, /* held */ false, /* bazookad */ true);
	if (ent->client && ent->client->pers.special == AODEATH)
	{
		fire_grenade_dM(ent, ent->client->angel->s.origin, forward, (float)damage, 500, 2.5, radius,
			ent->client->dM_grenade, /* held */ false, /* bazookad */ true);
	}

	if (!is_silenced)
	{
		gi.WriteByte(svc_muzzleflash);
		gi.WriteShort(ent - g_edicts);
		gi.WriteByte(MZ_ROCKET | is_silenced);
		gi.multicast(ent->s.origin, MULTICAST_PVS);
	}
	else
	{
		gi.WriteByte(svc_muzzleflash2);
		gi.WriteShort(ent - g_edicts);
		gi.WriteByte(MZ2_CHICK_ROCKET_1);
		gi.multicast(start, MULTICAST_PVS);
	}

	ent->client->ps.gunframe++;

	PlayerNoise(ent, start, PNOISE_WEAPON);

	if (!((int)dmflags->value & DF_INFINITE_AMMO))
		ent->client->pers.inventory[ent->client->ammo_index]
		-= ent->client->dM_ammoCost;
}

void Weapon_Bazooka(edict_t* ent)
{
	// WOWIE!
/*	if (ent->client->dM_grenade == DM_CATACLYSM)
	{
		static int	pause_frames[]	= {7, 0};
		static int	fire_frames[]	= {6, 0};

		Weapon_Generic (ent,
			5,	//frame activate last
			6,	//frame fire last
			59,	//frame idle last
			64,	//frame deactivate last
			pause_frames,
			fire_frames,
			weapon_bazooka_fire);
	}
*/
	static int	pause_frames[] = { 34, 51, 59, 0 };
	static int	fire_frames[] = { 6, 0 };

	Weapon_Generic(ent,
		5,	//frame activate last
		16,	//frame fire last
		59,	//frame idle last
		64,	//frame deactivate last
		pause_frames,
		fire_frames,
		weapon_bazooka_fire);
}

/*
======================================================================

ROCKET

======================================================================
*/

void Weapon_RocketLauncher_Fire(edict_t* ent)
{
	vec3_t	offset = { 0 };
	vec3_t	start;
	vec3_t	forward, right;
	int		damage;
	float	damage_radius = 120;
	int		radius_damage = 120;
	int		speed = 700;

	damage = 100 + (int)(random() * 20.0);
	if (is_quad)
	{
		damage *= 4;
		radius_damage *= 4;
	}

	AngleVectors(ent->client->v_angle, forward, right, NULL);

	VectorScale(forward, -2, ent->client->kick_origin);
	ent->client->kick_angles[0] = -1;

	VectorSet(offset, 8, 8, ent->viewheight - 8.0);
	P_ProjectSource(ent->client, ent->s.origin, offset, forward, right, start);
	fire_flamerocket(ent, start, forward, damage, speed, damage_radius, radius_damage);
	if (ent->client && ent->client->pers.special == AODEATH)
	{
		fire_flamerocket(ent, ent->client->angel->s.origin, forward, damage, speed, damage_radius, radius_damage);
	}
	// send muzzle flash
	if (is_silenced)
	{
		gi.WriteByte(svc_muzzleflash);
		gi.WriteShort(ent - g_edicts);
		gi.WriteByte(MZ_ROCKET | is_silenced);
		gi.multicast(ent->s.origin, MULTICAST_PVS);
	}
	else
	{
		gi.WriteByte(svc_muzzleflash2);
		gi.WriteShort(ent - g_edicts);
		gi.WriteByte(MZ2_CHICK_ROCKET_1);
		gi.multicast(start, MULTICAST_PVS);
	}

	ent->client->ps.gunframe++;

	PlayerNoise(ent, start, PNOISE_WEAPON);

	if (!((int)dmflags->value & DF_INFINITE_AMMO))
		ent->client->pers.inventory[ent->client->ammo_index]
		-= ent->client->pers.weapon->quantity;
}

void Weapon_RocketLauncher(edict_t* ent)
{
	static int	pause_frames[] = { 25, 33, 42, 50, 0 };
	static int	fire_frames[] = { 5, 0 };

	Weapon_Generic(ent, 4, 12, 50, 54, pause_frames, fire_frames, Weapon_RocketLauncher_Fire);
}


/*
======================================================================

BLASTER / HYPERBLASTER

======================================================================
*/

void Blaster_Fire(edict_t* ent, vec3_t g_offset, int damage, qboolean hyper, int effect)
{
	vec3_t	forward, right;
	vec3_t	start;
	vec3_t	offset = { 0 };

	if (is_quad)
		damage *= 4;
	AngleVectors(ent->client->v_angle, forward, right, NULL);
	VectorSet(offset, 24, 8, ent->viewheight - 8.0f);
	VectorAdd(offset, g_offset, offset);
	P_ProjectSource(ent->client, ent->s.origin, offset, forward, right, start);

	VectorScale(forward, -2, ent->client->kick_origin);
	ent->client->kick_angles[0] = -1;

	fire_blaster(ent, start, forward, damage, 1000, effect /* , hyper */);
	if (ent->client && ent->client->pers.special == AODEATH)
	{
		fire_blaster(ent, ent->client->angel->s.origin, forward, damage, 1000, effect /* , hyper */);
	}
	// send muzzle flash
	gi.WriteByte(svc_muzzleflash);
	gi.WriteShort(ent - g_edicts);
	if (hyper)
		gi.WriteByte(MZ_HYPERBLASTER | is_silenced);
	else
		gi.WriteByte(MZ_BLASTER | is_silenced);
	gi.multicast(ent->s.origin, MULTICAST_PVS);

	PlayerNoise(ent, start, PNOISE_WEAPON);
}


void Weapon_Blaster_Fire(edict_t* ent)
{
	int		damage;
	if (deathmatch->value)
		damage = 15;
	else
		damage = 10;

	// ### Hentai ### BEGIN

	Blaster_Fire(ent, vec3_origin, damage, false, EF_BLASTER);

	ent->client->anim_priority = ANIM_ATTACK;
	if (ent->client->ps.pmove.pm_flags & PMF_DUCKED)
	{
		ent->s.frame = FRAME_crattak1 - 1;
		ent->client->anim_end = FRAME_crattak9;
	}
	else
	{
		ent->s.frame = FRAME_attack1 - 1;
		ent->client->anim_end = FRAME_attack8;
	}

	// ### Hentai ### END

	ent->client->ps.gunframe++;
}

void Weapon_Blaster(edict_t* ent)
{
	static int	pause_frames[] = { 25, 33, 42, 50, 0 };
	static int	fire_frames[] = { 5, 0 };

	Weapon_Generic(ent, 4, 6, 52, 55, pause_frames, fire_frames, Weapon_Blaster_Fire);
}

void Weapon_HyperBlaster_Fire(edict_t* ent)
{
	float	rotation;
	vec3_t	offset = { 0 };
	int		effect;
	int		damage;

	ent->client->weapon_sound = gi.soundindex("weapons/hyprbl1a.wav");

	if (!(ent->client->buttons & BUTTON_ATTACK))
	{
		ent->client->ps.gunframe++;
	}
	else
	{
		if (!ent->client->pers.inventory[ent->client->ammo_index])
		{
			if (level.time >= ent->pain_debounce_time)
			{
				gi.sound(ent, CHAN_VOICE, gi.soundindex("weapons/noammo.wav"), 1, ATTN_NORM, 0);
				ent->pain_debounce_time = level.time + 1;
			}
			NoAmmoWeaponChange(ent);
		}
		else
		{
			rotation = (ent->client->ps.gunframe - 5.0) * 2.0 * M_PI / 6.0;
			offset[0] = -4 * sin(rotation);
			offset[1] = 0;
			offset[2] = 4 * cos(rotation);

			// Don't give every shot a yellow glow, otherwise the client will
			// get too much rendering lag.
			if ((ent->client->ps.gunframe == 6) || (ent->client->ps.gunframe == 9))
				effect = EF_HYPERBLASTER;
			else
				effect = 0;

			if (deathmatch->value)
				damage = 15;
			else
				damage = 20;

			Blaster_Fire(ent, offset, damage, true, effect);

			if (!ent->deadflag && ent->s.modelindex == 255)
			{
				// ### Hentai ### BEGIN

				ent->client->anim_priority = ANIM_ATTACK;
				if (ent->client->ps.pmove.pm_flags & PMF_DUCKED)
				{
					ent->s.frame = FRAME_crattak1 - 1;
					ent->client->anim_end = FRAME_crattak9;
				}
				else
				{
					ent->s.frame = FRAME_attack1 - 1;
					ent->client->anim_end = FRAME_attack8;
				}

				// ### Hentai ### END
			}

			if (!((int)dmflags->value & DF_INFINITE_AMMO))
				ent->client->pers.inventory[ent->client->ammo_index]
				-= ent->client->pers.weapon->quantity;
		}

		ent->client->ps.gunframe++;
		if (ent->client->ps.gunframe == 12 && ent->client->pers.inventory[ent->client->ammo_index])
			ent->client->ps.gunframe = 6;
	}

	if (ent->client->ps.gunframe == 12)
	{
		gi.sound(ent, CHAN_AUTO, gi.soundindex("weapons/hyprbd1a.wav"), 1, ATTN_NORM, 0);
		ent->client->weapon_sound = 0;
	}
}

void Weapon_HyperBlaster(edict_t* ent)
{
	static int	pause_frames[] = { 0 };
	static int	fire_frames[] = { 6, 7, 8, 9, 10, 11, 0 };

	Weapon_Generic(ent, 5, 20, 49, 53, pause_frames, fire_frames, Weapon_HyperBlaster_Fire);
}
/*
======================================================================

MACHINEGUN / CHAINGUN

======================================================================
*/
void Machine_Fire(edict_t* ent)
{
	int	i;
	vec3_t		start;
	vec3_t		forward, right;
	vec3_t		angles = { 0 };
	int			damage = 8;
	int			kick = 2;
	vec3_t		offset = { 0 };
	int			mod;

	if (!ent->client)
		return;

	if (!(ent->client->buttons & BUTTON_ATTACK) &&
		((ent->client->burstfire_count > 2) ||
			(!ent->client->burstfire_count)))
	{
		ent->client->machinegun_shots = 0;
		ent->client->burstfire_count = 0;
		ent->client->ps.gunframe++;
		return;
	}

	if (ent->client->burstfire_count < 3)
	{
		if (ent->client->ps.gunframe == 5)
			ent->client->ps.gunframe = 4;
		else
			ent->client->ps.gunframe = 5;
	}
	if (ent->client->pers.inventory[ent->client->ammo_index] < 1)
	{
		ent->client->ps.gunframe = 6;
		if (level.time >= ent->pain_debounce_time)
		{
			gi.sound(ent, CHAN_VOICE, gi.soundindex("weapons/noammo.wav"), 1, ATTN_NORM, 0);
			ent->pain_debounce_time = level.time + 1;
		}
		ent->client->burstfire_count = 0;

		NoAmmoWeaponChange(ent);
		return;
	}

	if (is_quad)
	{
		damage *= 4;
		kick *= 4;
	}

	for (i = 1; i < 3; i++)
	{
		ent->client->kick_origin[i] = crandom() * 0.35;
		ent->client->kick_angles[i] = crandom() * 0.7;
	}
	ent->client->kick_origin[0] = crandom() * 0.35;
	ent->client->kick_angles[0] = ent->client->machinegun_shots * -1.5;

	// raise the gun as it is firing
	if (!deathmatch->value && !ent->client->pers.fire_mode)
	{
		ent->client->machinegun_shots++;
		if (ent->client->machinegun_shots > 9)
			ent->client->machinegun_shots = 9;
	}

	// get start / end positions
	VectorAdd(ent->client->v_angle, ent->client->kick_angles, angles);
	AngleVectors(angles, forward, right, NULL);
	VectorSet(offset, 0, 8, ent->viewheight - 8.0);
	P_ProjectSource(ent->client, ent->s.origin, offset, forward, right, start);
	switch (ent->client->pers.fire_mode)
	{
		// Fire burst
	case 1:
		// Set up the means of death.
		mod = MOD_MACHINE;
		if (i_fragban & WFB_BURSTMACHINEGUN)
			mod |= MOD_NOFRAG;

		ent->client->burstfire_count++;
		if (ent->client->burstfire_count < 4)
		{
			fire_bullet(ent, start, forward, damage * 4, kick / 2,
				DEFAULT_BULLET_HSPREAD / 2, DEFAULT_BULLET_VSPREAD / 2, mod);
			if (ent->client && ent->client->pers.special == AODEATH)
			{
				fire_bullet(ent, ent->client->angel->s.origin, forward, damage * 4, kick / 2,
					DEFAULT_BULLET_HSPREAD / 2, DEFAULT_BULLET_VSPREAD / 2, mod);
			}
			gi.WriteByte(svc_muzzleflash);
			gi.WriteShort(ent - g_edicts);
			gi.WriteByte(MZ_MACHINEGUN | is_silenced);
			gi.multicast(ent->s.origin, MULTICAST_PVS);
			PlayerNoise(ent, start, PNOISE_WEAPON);
			if (!((int)dmflags->value & DF_INFINITE_AMMO))
				ent->client->pers.inventory[ent->client->ammo_index] -= 2;
		}
		else if (ent->client->burstfire_count > 6)
			ent->client->burstfire_count = 0;
		break;

		// Fire Fully Automatic
	case 0:
	default:
		// Set up the means of death.
		mod = MOD_MACHINE;
		if (i_fragban & WFB_MACHINEGUN)
			mod |= MOD_NOFRAG;

		fire_bullet(ent, start, forward, damage, kick, DEFAULT_BULLET_HSPREAD,
			DEFAULT_BULLET_VSPREAD, mod);
		if (ent->client && ent->client->pers.special == AODEATH)
		{
			fire_bullet(ent, ent->client->angel->s.origin, forward, damage, kick, DEFAULT_BULLET_HSPREAD,
				DEFAULT_BULLET_VSPREAD, mod);
		}
		gi.WriteByte(svc_muzzleflash);
		gi.WriteShort(ent - g_edicts);
		gi.WriteByte(MZ_MACHINEGUN | is_silenced);
		gi.multicast(ent->s.origin, MULTICAST_PVS);
		PlayerNoise(ent, start, PNOISE_WEAPON);
		if (!((int)dmflags->value & DF_INFINITE_AMMO))
			ent->client->pers.inventory[ent->client->ammo_index]
			-= ent->client->pers.weapon->quantity;
		break;
	}

	if (!ent->deadflag && ent->s.modelindex == 255)
	{
		// ### Hentai ### BEGIN
		ent->client->anim_priority = ANIM_ATTACK;
		if (ent->client->ps.pmove.pm_flags & PMF_DUCKED)
		{
			ent->s.frame = FRAME_crattak1 - (int)(random() + 0.25);
			ent->client->anim_end = FRAME_crattak9;
		}
		else
		{
			ent->s.frame = FRAME_attack1 - (int)(random() + 0.25);
			ent->client->anim_end = FRAME_attack8;
		}
		// ### Hentai ### END
	}
}

void Weapon_Machine(edict_t* ent)
{
	static int	pause_frames[] = { 23, 45, 0 };
	static int	fire_frames[] = { 4, 5, 0 };

	Weapon_Generic(ent, 3, 5, 45, 49, pause_frames, fire_frames, Machine_Fire);
}

void Machinegun_Fire(edict_t* ent)
{
	vec3_t	offset = { 0 };
	vec3_t	start;
	vec3_t	forward, right;
	int		damage;
	float	damage_radius;
	int		radius_damage;

	if (!ent->client)
		return;

	damage = 15 + (int)(random() * 10.0);
	radius_damage = 15 + (int)(random() * 10.0);
	damage_radius = 20;
	if (is_quad)
	{
		damage *= 4;
		radius_damage *= 4;
	}

	if (!(ent->client->buttons & BUTTON_ATTACK))
	{
		ent->client->machinegun_shots = 0;
		ent->client->ps.gunframe++;
		return;
	}

	if (ent->client->ps.gunframe == 5)
		ent->client->ps.gunframe = 4;
	else
		ent->client->ps.gunframe = 5;

	AngleVectors(ent->client->v_angle, forward, right, NULL);

	VectorScale(forward, -2, ent->client->kick_origin);
	ent->client->kick_angles[0] = -1;

	VectorSet(offset, 8, 8, ent->viewheight - 8.0);
	P_ProjectSource(ent->client, ent->s.origin, offset, forward, right, start);

	fire_mr(ent, start, forward, damage, 1000, damage_radius, radius_damage);

	if (ent->client && ent->client->pers.special == AODEATH)
	{
		fire_mr(ent, ent->client->angel->s.origin, forward, damage, 1000, damage_radius, radius_damage);
	}
	// send muzzle flash
	gi.WriteByte(svc_muzzleflash);
	gi.WriteShort(ent - g_edicts);
	gi.WriteByte(MZ_CHAINGUN1 | is_silenced);
	gi.multicast(ent->s.origin, MULTICAST_PVS);

	ent->client->ps.gunframe++;

	PlayerNoise(ent, start, PNOISE_WEAPON);

	if (!ent->deadflag && ent->s.modelindex == 255)
	{
		// ### Hentai ### BEGIN

		ent->client->anim_priority = ANIM_ATTACK;
		if (ent->client->ps.pmove.pm_flags & PMF_DUCKED)
		{
			ent->s.frame = FRAME_crattak1 - (int)(random() + 0.25);
			ent->client->anim_end = FRAME_crattak9;
		}
		else
		{
			ent->s.frame = FRAME_attack1 - (int)(random() + 0.25);
			ent->client->anim_end = FRAME_attack8;
		}

		// ### Hentai ### END
	}

	if (!((int)dmflags->value & DF_INFINITE_AMMO))
		ent->client->pers.inventory[ent->client->ammo_index]
		-= ent->client->pers.weapon->quantity;
}

void Weapon_Machinegun(edict_t* ent)
{
	static int	pause_frames[] = { 23, 45, 0 };
	static int	fire_frames[] = { 4, 5, 0 };

	Weapon_Generic(ent, 3, 4, 45, 49, pause_frames, fire_frames, Machinegun_Fire);
}

void Chaingun_Fire(edict_t* ent)
{
	int			i;
	int			shots;
	vec3_t		start;
	vec3_t		forward, right, up;
	float		r, u;
	vec3_t		offset = { 0 };
	int			damage;
	int			kick = 2;
	int			mod;

	if (!ent->client)
		return;

	if (deathmatch->value)
		damage = 6;
	else
		damage = 8;

	if (is_quad)
	{
		damage *= 4;
		kick *= 4;
	}

	if (ent->client->ps.gunframe == 5)
		gi.sound(ent, CHAN_AUTO, gi.soundindex("weapons/chngnu1a.wav"), 1, ATTN_IDLE, 0);

	if ((ent->client->ps.gunframe == 14) && !(ent->client->buttons & BUTTON_ATTACK))
	{
		ent->client->ps.gunframe = 32;
		ent->client->weapon_sound = 0;
		return;
	}
	else if ((ent->client->ps.gunframe == 21) && (ent->client->buttons & BUTTON_ATTACK)
		&& ent->client->pers.inventory[ent->client->ammo_index])
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

	if (!ent->deadflag && ent->s.modelindex == 255)
	{
		// ### Hentai ### BEGIN

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

		// ### Hentai ### END
	}

	if (ent->client->ps.gunframe <= 9)
		shots = 1;
	else if (ent->client->ps.gunframe <= 14)
	{
		if (ent->client->buttons & BUTTON_ATTACK)
			shots = 2;
		else
			shots = 1;
	}
	else
		shots = 3;

	if (ent->client->pers.inventory[ent->client->ammo_index] < shots)
		shots = ent->client->pers.inventory[ent->client->ammo_index];

	if (!shots)
	{
		if (level.time >= ent->pain_debounce_time)
		{
			gi.sound(ent, CHAN_VOICE, gi.soundindex("weapons/noammo.wav"), 1, ATTN_NORM, 0);
			ent->pain_debounce_time = level.time + 1;
		}
		NoAmmoWeaponChange(ent);
		return;
	}

	for (i = 0; i < 3; i++)
	{
		ent->client->kick_origin[i] = crandom() * 0.35;
		ent->client->kick_angles[i] = crandom() * 0.7;
	}

	// Set up the means of death.
	mod = MOD_CHAINGUN;
	if (i_fragban & WFB_CHAINGUN)
		mod |= MOD_NOFRAG;

	for (i = 0; i < shots; i++)
	{
		// get start / end positions
		AngleVectors(ent->client->v_angle, forward, right, up);
		r = 7 + crandom() * 4;
		u = crandom() * 4;
		VectorSet(offset, 0, r, u + ent->viewheight - 8.0);
		P_ProjectSource(ent->client, ent->s.origin, offset, forward, right, start);

		fire_bullet(ent, start, forward, damage, kick, DEFAULT_BULLET_HSPREAD,
			DEFAULT_BULLET_VSPREAD, mod);
		if (ent->client && ent->client->pers.special == AODEATH)
		{
			fire_bullet(ent, ent->client->angel->s.origin, forward, damage, kick, DEFAULT_BULLET_HSPREAD,
				DEFAULT_BULLET_VSPREAD, mod);
		}
	}

	// send muzzle flash
	gi.WriteByte(svc_muzzleflash);
	gi.WriteShort(ent - g_edicts);
	gi.WriteByte((MZ_CHAINGUN1 + shots - 1) | is_silenced);
	gi.multicast(ent->s.origin, MULTICAST_PVS);

	PlayerNoise(ent, start, PNOISE_WEAPON);

	if (!ent->deadflag && ent->s.modelindex == 255)
	{
		// ### Hentai ### BEGIN

		ent->client->anim_priority = ANIM_ATTACK;
		if (ent->client->ps.pmove.pm_flags & PMF_DUCKED)
		{
			ent->s.frame = FRAME_crattak1 - (int)(random() + 0.25);
			ent->client->anim_end = FRAME_crattak9;
		}
		else
		{
			ent->s.frame = FRAME_attack1 - (int)(random() + 0.25);
			ent->client->anim_end = FRAME_attack8;
		}

		// ### Hentai ### END
	}

	if (!((int)dmflags->value & DF_INFINITE_AMMO))
		ent->client->pers.inventory[ent->client->ammo_index] -= shots;
}


void Weapon_Chaingun(edict_t* ent)
{
	static int	pause_frames[] = { 38, 43, 51, 61, 0 };
	static int	fire_frames[] = { 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 0 };

	Weapon_Generic(ent, 4, 31, 61, 64, pause_frames, fire_frames, Chaingun_Fire);
}

void ExplosiveChaingun_Fire(edict_t* ent)
{
	int			i;
	int			shots;
	vec3_t		start;
	vec3_t		forward, right, up;
	float		r, u;
	vec3_t		offset = { 0 };
	int			damage;
	int			kick = 2;
	int			mod;

	if (!ent->client)
		return;

	if (deathmatch->value)
		damage = 6;
	else
		damage = 8;

	if (ent->client->ps.gunframe == 5)
		gi.sound(ent, CHAN_AUTO, gi.soundindex("weapons/chngnu1a.wav"), 1, ATTN_IDLE, 0);

	if ((ent->client->ps.gunframe == 14) && !(ent->client->buttons & BUTTON_ATTACK))
	{
		ent->client->ps.gunframe = 32;
		ent->client->weapon_sound = 0;
		return;
	}
	else if ((ent->client->ps.gunframe == 21) && (ent->client->buttons & BUTTON_ATTACK)
		&& ent->client->pers.inventory[ent->client->ammo_index])
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

	if (!ent->deadflag && ent->s.modelindex == 255)
	{
		// ### Hentai ### BEGIN

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

		// ### Hentai ### END
	}

	if (ent->client->ps.gunframe <= 9)
		shots = 1;
	else if (ent->client->ps.gunframe <= 14)
	{
		if (ent->client->buttons & BUTTON_ATTACK)
			shots = 2;
		else
			shots = 1;
	}
	else
		shots = 3;

	if (ent->client->pers.inventory[ent->client->ammo_index] < shots)
		shots = ent->client->pers.inventory[ent->client->ammo_index];

	if (!shots)
	{
		if (level.time >= ent->pain_debounce_time)
		{
			gi.sound(ent, CHAN_VOICE, gi.soundindex("weapons/noammo.wav"), 1, ATTN_NORM, 0);
			ent->pain_debounce_time = level.time + 1;
		}
		NoAmmoWeaponChange(ent);
		return;
	}

	if (is_quad)
	{
		damage *= 4;
		kick *= 4;
	}

	for (i = 0; i < 3; i++)
	{
		ent->client->kick_origin[i] = crandom() * 0.35;
		ent->client->kick_angles[i] = crandom() * 0.7;
	}

	// Set up the means of death.
	mod = MOD_EXPLOSIVECHAINGUN;
	if (i_fragban & WFB_SWEEPERS)	//frag ban on sweepers means no points awarded
		mod |= MOD_NOFRAG;

	for (i = 0; i < shots; i++)
	{
		// get start / end positions
		AngleVectors(ent->client->v_angle, forward, right, up);
		r = 7 + crandom() * 4;
		u = crandom() * 4;
		VectorSet(offset, 0, r, u + ent->viewheight - 8.0);
		P_ProjectSource(ent->client, ent->s.origin, offset, forward, right, start);

		fire_explosive_bullet(ent, start, forward, damage, kick, DEFAULT_BULLET_HSPREAD,
			DEFAULT_BULLET_VSPREAD, mod);
		if (ent->client->sweeperfiremode == SFM_HIGH)
			ent->client->curr_heat += WEPHEAT_HIGH;
		else
			ent->client->curr_heat += WEPHEAT_LOW;


		if (ent->client && ent->client->pers.special == AODEATH)
		{
			fire_explosive_bullet(ent, ent->client->angel->s.origin, forward, damage, kick, DEFAULT_BULLET_HSPREAD, DEFAULT_BULLET_VSPREAD, mod);
		}
	}

	// send muzzle flash
	gi.WriteByte(svc_muzzleflash);
	gi.WriteShort(ent - g_edicts);
	gi.WriteByte((MZ_CHAINGUN1 + shots - 1) | is_silenced);
	gi.multicast(ent->s.origin, MULTICAST_PVS);

	PlayerNoise(ent, start, PNOISE_WEAPON);

	if (!ent->deadflag && ent->s.modelindex == 255)
	{
		// ### Hentai ### BEGIN

		ent->client->anim_priority = ANIM_ATTACK;
		if (ent->client->ps.pmove.pm_flags & PMF_DUCKED)
		{
			ent->s.frame = FRAME_crattak1 - (int)(random() + 0.25);
			ent->client->anim_end = FRAME_crattak9;
		}
		else
		{
			ent->s.frame = FRAME_attack1 - (int)(random() + 0.25);
			ent->client->anim_end = FRAME_attack8;
		}

		// ### Hentai ### END
	}

	if (!((int)dmflags->value & DF_INFINITE_AMMO))
		ent->client->pers.inventory[ent->client->ammo_index] -= shots;
}


void Weapon_ExplosiveChaingun(edict_t* ent)
{
	static int	pause_frames[] = { 38, 43, 51, 61, 0 };
	static int	fire_frames[] = { 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 0 };

	Weapon_Generic(ent, 4, 31, 61, 64, pause_frames, fire_frames, ExplosiveChaingun_Fire);
}

void weapon_streetsweeper_fire(edict_t* ent)
{
	vec3_t		start;
	vec3_t		forward, right;
	vec3_t		offset = { 0 };
	int			damage = 4;
	int			kick = 8;
	int			mod;

	AngleVectors(ent->client->v_angle, forward, right, NULL);

	VectorScale(forward, -2, ent->client->kick_origin);
	ent->client->kick_angles[0] = -2;

	VectorSet(offset, 0, 8, ent->viewheight - 8.0);
	P_ProjectSource(ent->client, ent->s.origin, offset, forward, right, start);

	if (is_quad)
	{
		damage *= 4;
		kick *= 4;
	}

	if (!(ent->client->pers.inventory[ent->client->ammo_index]))
	{
		// Stop firing.
		ent->client->ps.gunframe = 22;
		ent->client->weapon_sound = 0;

		// Make the streetsweeper-noammo sound.
		gi.sound(ent, CHAN_AUTO, gi.soundindex("weapons/noammo.wav"), 1, ATTN_IDLE, 0);

		// Switch to some other weapon.
		NoAmmoWeaponChange(ent);

		return;
	}
	else if (ent->client->weaponstate == WEAPON_FIRING
		&& ent->client->ps.gunframe == 21
		&& (ent->client->buttons & BUTTON_ATTACK)
		&& ent->client->pers.inventory[ent->client->ammo_index])
	{
		// Keep firing.
		ent->client->ps.gunframe = 15;
	}
	else if (ent->client->weaponstate == WEAPON_FIRING
		&& ent->client->ps.gunframe >= 5
		&& ent->client->ps.gunframe <= 21
		&& !(ent->client->buttons & BUTTON_ATTACK))
	{
		// Stop firing.
		ent->client->ps.gunframe = 22;
		ent->client->weapon_sound = 0;

		// Make the streetsweeper-winddown sound.
		gi.sound(ent, CHAN_AUTO, gi.soundindex("weapons/chngnd1a.wav"), 1, ATTN_IDLE, 0);
	}
	else
	{
		if (ent->client->ps.gunframe == 5)
		{
			// Start firing.

			// Make the streetsweeper-windup sound.
			gi.sound(ent, CHAN_AUTO, gi.soundindex("weapons/chngnu1a.wav"), 1, ATTN_IDLE, 0);
		}
		else if (ent->client->ps.gunframe >= 15
			&& ent->client->ps.gunframe <= 21)
		{
			// Now that the wind-up sound has finished, play the "during" sound
			// continuously until they stop firing.
			ent->client->weapon_sound = gi.soundindex("weapons/chngnl1a.wav");
		}

		ent->client->ps.gunframe++;
	}

	if (!ent->deadflag && ent->s.modelindex == 255)
	{
		// ### Hentai ### BEGIN

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

		// ### Hentai ### END
	}

	// Set up the means of death.
	mod = MOD_STREETSWEEP;
	if (i_fragban & WFB_SWEEPERS)	//frag ban on sweepers means no points awarded
		mod |= MOD_NOFRAG;

	if (deathmatch->value)
	{
		fire_shotgun(ent, start, forward, damage, kick, 500, 500,
			DEFAULT_DEATHMATCH_SHOTGUN_COUNT, mod);
		if (ent->client && ent->client->pers.special == AODEATH)
		{
			fire_shotgun(ent, ent->client->angel->s.origin, forward, damage, kick,
				500, 500, DEFAULT_DEATHMATCH_SHOTGUN_COUNT, mod);
		}
	}
	else
	{
		fire_shotgun(ent, start, forward, damage, kick, 500, 500,
			DEFAULT_SHOTGUN_COUNT, mod);
		if (ent->client && ent->client->pers.special == AODEATH)
		{
			fire_shotgun(ent, ent->client->angel->s.origin, forward, damage, kick,
				500, 500, DEFAULT_SHOTGUN_COUNT, mod);
		}
	}
	// send muzzle flash
	gi.WriteByte(svc_muzzleflash);
	gi.WriteShort(ent - g_edicts);
	gi.WriteByte(MZ_SHOTGUN | is_silenced);
	gi.multicast(ent->s.origin, MULTICAST_PVS);

	PlayerNoise(ent, start, PNOISE_WEAPON);

	if (!ent->deadflag && ent->s.modelindex == 255)
	{
		// ### Hentai ### BEGIN

		ent->client->anim_priority = ANIM_ATTACK;
		if (ent->client->ps.pmove.pm_flags & PMF_DUCKED)
		{
			ent->s.frame = FRAME_crattak1 - (int)(random() + 0.25);
			ent->client->anim_end = FRAME_crattak9;
		}
		else
		{
			ent->s.frame = FRAME_attack1 - (int)(random() + 0.25);
			ent->client->anim_end = FRAME_attack8;
		}

		// ### Hentai ### END
	}

	if (!((int)dmflags->value & DF_INFINITE_AMMO))
		ent->client->pers.inventory[ent->client->ammo_index]
		-= ent->client->pers.weapon->quantity;
}

void Weapon_Streetsweeper(edict_t* ent)
{
	static int	pause_frames[] = { 38, 43, 51, 61, 0 };
	static int	fire_frames[] = { 5, 7, 9, 11, 13, 15, 17, 19, 21, 0 };

	// Hacked-up chaingun spec
	Weapon_Generic(ent, 4, 31, 61, 64, pause_frames, fire_frames,
		weapon_streetsweeper_fire);
}

void weapon_explosivestreetsweeper_fire(edict_t* ent)
{
	vec3_t		start;
	vec3_t		forward, right;
	vec3_t		offset = { 0 };
	int			damage = 4;
	int			kick = 8;
	int			mod;

	AngleVectors(ent->client->v_angle, forward, right, NULL);

	VectorScale(forward, -2, ent->client->kick_origin);
	ent->client->kick_angles[0] = -2;

	VectorSet(offset, 0, 8, ent->viewheight - 8.0);
	P_ProjectSource(ent->client, ent->s.origin, offset, forward, right, start);

	if (is_quad)
	{
		damage *= 4;
		kick *= 4;
	}

	if (!(ent->client->pers.inventory[ent->client->ammo_index]))
	{
		// Stop firing.
		ent->client->ps.gunframe = 22;
		ent->client->weapon_sound = 0;

		// Make the streetsweeper-noammo sound.
		gi.sound(ent, CHAN_AUTO, gi.soundindex("weapons/noammo.wav"), 1, ATTN_IDLE, 0);

		// Switch to some other weapon.
		NoAmmoWeaponChange(ent);

		return;
	}
	else if (ent->client->weaponstate == WEAPON_FIRING
		&& ent->client->ps.gunframe == 21
		&& (ent->client->buttons & BUTTON_ATTACK)
		&& ent->client->pers.inventory[ent->client->ammo_index])
	{
		// Keep firing.
		ent->client->ps.gunframe = 15;
	}
	else if (ent->client->weaponstate == WEAPON_FIRING
		&& ent->client->ps.gunframe >= 5
		&& ent->client->ps.gunframe <= 21
		&& !(ent->client->buttons & BUTTON_ATTACK))
	{
		// Stop firing.
		ent->client->ps.gunframe = 22;
		ent->client->weapon_sound = 0;

		// Make the streetsweeper-winddown sound.
		gi.sound(ent, CHAN_AUTO, gi.soundindex("weapons/chngnd1a.wav"), 1, ATTN_IDLE, 0);
	}
	else
	{
		if (ent->client->ps.gunframe == 5)
		{
			// Start firing.

			// Make the streetsweeper-windup sound.
			gi.sound(ent, CHAN_AUTO, gi.soundindex("weapons/chngnu1a.wav"), 1, ATTN_IDLE, 0);
		}
		else if (ent->client->ps.gunframe >= 15
			&& ent->client->ps.gunframe <= 21)
		{
			// Now that the wind-up sound has finished, play the "during" sound
			// continuously until they stop firing.
			ent->client->weapon_sound = gi.soundindex("weapons/chngnl1a.wav");
		}

		ent->client->ps.gunframe++;
	}

	if (!ent->deadflag && ent->s.modelindex == 255)
	{
		// ### Hentai ### BEGIN

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

		// ### Hentai ### END
	}

	// Set up the means of death.
	mod = MOD_EXPLOSIVESTREETSWEEPER;
	if (i_fragban & WFB_SWEEPERS)	//frag ban on sweepers means no points awarded
		mod |= MOD_NOFRAG;

	if (deathmatch->value)
	{
		fire_explosive_shotgun(ent, start, forward, damage, kick, TE_BLASTER, 500, 500,
			DEFAULT_DEATHMATCH_SHOTGUN_COUNT, mod);
		if (ent->client->sweeperfiremode == SFM_HIGH)
			ent->client->curr_heat += WEPHEAT_HIGH;
		else
			ent->client->curr_heat += WEPHEAT_LOW;


		if (ent->client && ent->client->pers.special == AODEATH)
		{
			fire_explosive_shotgun(ent, ent->client->angel->s.origin, forward, damage, kick, TE_BLASTER,
				500, 500, DEFAULT_DEATHMATCH_SHOTGUN_COUNT, mod);
		}
	}
	else
	{
		fire_shotgun(ent, start, forward, damage, kick, 500, 500,
			DEFAULT_SHOTGUN_COUNT, mod);
		if (ent->client && ent->client->pers.special == AODEATH)
		{
			fire_shotgun(ent, ent->client->angel->s.origin, forward, damage, kick,
				500, 500, DEFAULT_SHOTGUN_COUNT, mod);
		}
	}
	// send muzzle flash
	gi.WriteByte(svc_muzzleflash);
	gi.WriteShort(ent - g_edicts);
	gi.WriteByte(MZ_SHOTGUN | is_silenced);
	gi.multicast(ent->s.origin, MULTICAST_PVS);

	PlayerNoise(ent, start, PNOISE_WEAPON);

	if (!ent->deadflag && ent->s.modelindex == 255)
	{
		// ### Hentai ### BEGIN

		ent->client->anim_priority = ANIM_ATTACK;
		if (ent->client->ps.pmove.pm_flags & PMF_DUCKED)
		{
			ent->s.frame = FRAME_crattak1 - (int)(random() + 0.25);
			ent->client->anim_end = FRAME_crattak9;
		}
		else
		{
			ent->s.frame = FRAME_attack1 - (int)(random() + 0.25);
			ent->client->anim_end = FRAME_attack8;
		}

		// ### Hentai ### END
	}

	if (!((int)dmflags->value & DF_INFINITE_AMMO))
		ent->client->pers.inventory[ent->client->ammo_index]
		-= ent->client->pers.weapon->quantity;
}

void Weapon_ExplosiveStreetsweeper(edict_t* ent)
{
	static int	pause_frames[] = { 38, 43, 51, 61, 0 };
	static int	fire_frames[] = { 5, 7, 9, 11, 13, 15, 17, 19, 21, 0 };

	// Hacked-up chaingun spec
	Weapon_Generic(ent, 4, 31, 61, 64, pause_frames, fire_frames,
		weapon_explosivestreetsweeper_fire);
}
/*
======================================================================

SHOTGUN / SUPERSHOTGUN

======================================================================
*/

void weapon_shotgun_fire(edict_t* ent)
{
	vec3_t		start;
	vec3_t		forward, right;
	vec3_t		offset = { 0 };
	int			damage = 4;
	int			kick = 8;
	int			mod;

	if (!ent->client)
		return;

	if (ent->client->ps.gunframe == 9)
	{
		ent->client->ps.gunframe++;
		return;
	}

	AngleVectors(ent->client->v_angle, forward, right, NULL);

	VectorScale(forward, -2, ent->client->kick_origin);
	ent->client->kick_angles[0] = -2;

	VectorSet(offset, 0, 8, ent->viewheight - 8.0);
	P_ProjectSource(ent->client, ent->s.origin, offset, forward, right, start);

	if (is_quad)
	{
		damage *= 4;
		kick *= 4;
	}

	// Set up the means of death.
	mod = MOD_SHOTGUN;

	if (deathmatch->value)
	{
		fire_shotgun(ent, start, forward, damage, kick, 500, 500,
			DEFAULT_DEATHMATCH_SHOTGUN_COUNT, mod);
		if (ent->client && ent->client->pers.special == AODEATH)
		{
			fire_shotgun(ent, ent->client->angel->s.origin, forward, damage, kick, 500, 500,
				DEFAULT_DEATHMATCH_SHOTGUN_COUNT, mod);
		}
	}
	else
	{
		fire_shotgun(ent, start, forward, damage, kick, 500, 500,
			DEFAULT_SHOTGUN_COUNT, mod);
		if (ent->client && ent->client->pers.special == AODEATH)
		{
			fire_shotgun(ent, ent->client->angel->s.origin, forward, damage, kick, 500, 500,
				DEFAULT_SHOTGUN_COUNT, mod);
		}
	}

	// send muzzle flash
	gi.WriteByte(svc_muzzleflash);
	gi.WriteShort(ent - g_edicts);
	gi.WriteByte(MZ_SHOTGUN | is_silenced);
	gi.multicast(ent->s.origin, MULTICAST_PVS);

	ent->client->ps.gunframe++;
	PlayerNoise(ent, start, PNOISE_WEAPON);

	if (!((int)dmflags->value & DF_INFINITE_AMMO))
		ent->client->pers.inventory[ent->client->ammo_index]
		-= ent->client->pers.weapon->quantity;
}

void Weapon_Shotgun(edict_t* ent)
{
	static int	pause_frames[] = { 22, 28, 34, 0 };
	static int	fire_frames[] = { 8, 9, 0 };

	Weapon_Generic(ent, 7, 18, 36, 39, pause_frames, fire_frames, weapon_shotgun_fire);
}

void weapon_explosiveshotgun_fire(edict_t* ent)
{
	vec3_t		start;
	vec3_t		forward, right;
	vec3_t		offset = { 0 };
	int			damage = 4;
	int			kick = 8;
	int			mod;

	if (!ent->client)
		return;

	if (ent->client->ps.gunframe == 9)
	{
		ent->client->ps.gunframe++;
		return;
	}

	AngleVectors(ent->client->v_angle, forward, right, NULL);

	VectorScale(forward, -2, ent->client->kick_origin);
	ent->client->kick_angles[0] = -2;

	VectorSet(offset, 0, 8, ent->viewheight - 8.0);
	P_ProjectSource(ent->client, ent->s.origin, offset, forward, right, start);

	if (is_quad)
	{
		damage *= 4;
		kick *= 4;
	}

	// Set up the means of death.
	mod = MOD_EXPLOSIVESHOTGUN;

	if (deathmatch->value)
	{
		fire_explosive_shotgun(ent, start, forward, damage, kick, TE_BLASTER, 500, 500,
			DEFAULT_DEATHMATCH_SHOTGUN_COUNT, mod);
		if (ent->client && ent->client->pers.special == AODEATH)
		{
			fire_explosive_shotgun(ent, ent->client->angel->s.origin, forward, damage, kick, TE_BLASTER, 500, 500,
				DEFAULT_DEATHMATCH_SHOTGUN_COUNT, mod);
		}
	}
	else
	{
		fire_explosive_shotgun(ent, start, forward, damage, kick, TE_BLASTER, 500, 500,
			DEFAULT_SHOTGUN_COUNT, mod);
		if (ent->client && ent->client->pers.special == AODEATH)
		{
			fire_explosive_shotgun(ent, ent->client->angel->s.origin, forward, damage, kick, TE_BLASTER, 500, 500,
				DEFAULT_SHOTGUN_COUNT, mod);
		}
	}

	// send muzzle flash
	gi.WriteByte(svc_muzzleflash);
	gi.WriteShort(ent - g_edicts);
	gi.WriteByte(MZ_SHOTGUN | is_silenced);
	gi.multicast(ent->s.origin, MULTICAST_PVS);

	ent->client->ps.gunframe++;
	PlayerNoise(ent, start, PNOISE_WEAPON);

	if (!((int)dmflags->value & DF_INFINITE_AMMO))
		ent->client->pers.inventory[ent->client->ammo_index]
		-= ent->client->pers.weapon->quantity;
}

void Weapon_ExplosiveShotgun(edict_t* ent)
{
	static int	pause_frames[] = { 22, 28, 34, 0 };
	static int	fire_frames[] = { 8, 9, 0 };

	Weapon_Generic(ent, 7, 18, 36, 39, pause_frames, fire_frames, weapon_explosiveshotgun_fire);
}

void weapon_supershotgun_fire(edict_t* ent)
{
	vec3_t		start;
	vec3_t		forward, right;
	vec3_t		offset = { 0 };
	vec3_t		v = { 0 };
	int			damage = 6;
	int			kick = 100;
	int			mod;

	AngleVectors(ent->client->v_angle, forward, right, NULL);

	VectorScale(forward, -2, ent->client->kick_origin);
	ent->client->kick_angles[0] = -2;

	VectorSet(offset, 0, 8, ent->viewheight - 8.0);
	P_ProjectSource(ent->client, ent->s.origin, offset, forward, right, start);

	if (is_quad)
	{
		damage *= 4;
		kick *= 4;
	}

	// Set up the means of death.
	mod = MOD_SSHOTGUN;

	v[PITCH] = ent->client->v_angle[PITCH];
	v[YAW] = ent->client->v_angle[YAW] - 5;
	v[ROLL] = ent->client->v_angle[ROLL];
	AngleVectors(v, forward, NULL, NULL);
	fire_shotgun(ent, start, forward, damage, kick, DEFAULT_SHOTGUN_HSPREAD,
		DEFAULT_SHOTGUN_VSPREAD, DEFAULT_SSHOTGUN_COUNT / 2, mod);
	if (ent->client && ent->client->pers.special == AODEATH)
	{
		fire_shotgun(ent, ent->client->angel->s.origin, forward, damage, kick, DEFAULT_SHOTGUN_HSPREAD, DEFAULT_SHOTGUN_VSPREAD, DEFAULT_SSHOTGUN_COUNT / 2, MOD_SSHOTGUN);
	}
	v[YAW] = ent->client->v_angle[YAW] + 5;
	AngleVectors(v, forward, NULL, NULL);
	fire_shotgun(ent, start, forward, damage, kick, DEFAULT_SHOTGUN_HSPREAD,
		DEFAULT_SHOTGUN_VSPREAD, DEFAULT_SSHOTGUN_COUNT / 2, mod);

	// send muzzle flash
	gi.WriteByte(svc_muzzleflash);
	gi.WriteShort(ent - g_edicts);
	gi.WriteByte(MZ_SSHOTGUN | is_silenced);
	gi.multicast(ent->s.origin, MULTICAST_PVS);

	ent->client->ps.gunframe++;
	PlayerNoise(ent, start, PNOISE_WEAPON);

	if (!((int)dmflags->value & DF_INFINITE_AMMO))
		ent->client->pers.inventory[ent->client->ammo_index]
		-= ent->client->pers.weapon->quantity;
}


void Weapon_SuperShotgun(edict_t* ent)
{
	static int	pause_frames[] = { 29, 42, 57, 0 };
	static int	fire_frames[] = { 7, 0 };

	Weapon_Generic(ent, 6, 17, 57, 61, pause_frames, fire_frames, weapon_supershotgun_fire);
}

void weapon_explosivesupershotgun_fire(edict_t* ent)
{
	vec3_t		start;
	vec3_t		forward, right;
	vec3_t		offset = { 0 };
	vec3_t		v = { 0 };
	int			damage = 6;
	int			kick = 100;
	int			mod;

	AngleVectors(ent->client->v_angle, forward, right, NULL);

	VectorScale(forward, -2, ent->client->kick_origin);
	ent->client->kick_angles[0] = -2;

	VectorSet(offset, 0, 8, ent->viewheight - 8.0);
	P_ProjectSource(ent->client, ent->s.origin, offset, forward, right, start);

	if (is_quad)
	{
		damage *= 4;
		kick *= 4;
	}

	// Set up the means of death.
	mod = MOD_EXPLOSIVESSHOTGUN;

	v[PITCH] = ent->client->v_angle[PITCH];
	v[YAW] = ent->client->v_angle[YAW] - 5;
	v[ROLL] = ent->client->v_angle[ROLL];
	AngleVectors(v, forward, NULL, NULL);
	fire_explosive_shotgun(ent, start, forward, damage, kick, TE_BLASTER, DEFAULT_EXBULLET_HSPREAD,
		DEFAULT_EXBULLET_VSPREAD, DEFAULT_SSHOTGUN_COUNT / 2, mod);
	if (ent->client && ent->client->pers.special == AODEATH)
	{
		fire_explosive_shotgun(ent, ent->client->angel->s.origin, forward, damage, kick, TE_BLASTER, DEFAULT_EXBULLET_HSPREAD,
			DEFAULT_EXBULLET_VSPREAD, DEFAULT_SSHOTGUN_COUNT / 2, mod);
	}
	v[YAW] = ent->client->v_angle[YAW] + 5;
	AngleVectors(v, forward, NULL, NULL);
	fire_explosive_shotgun(ent, start, forward, damage, kick, TE_BLASTER, DEFAULT_EXBULLET_HSPREAD,
		DEFAULT_EXBULLET_VSPREAD, DEFAULT_SSHOTGUN_COUNT / 2, mod);
	if (ent->client && ent->client->pers.special == AODEATH)
	{
		fire_explosive_shotgun(ent, ent->client->angel->s.origin, forward, damage, kick, TE_BLASTER, DEFAULT_EXBULLET_HSPREAD,
			DEFAULT_EXBULLET_VSPREAD, DEFAULT_SSHOTGUN_COUNT / 2, mod);
	}
	// send muzzle flash
	gi.WriteByte(svc_muzzleflash);
	gi.WriteShort(ent - g_edicts);
	gi.WriteByte(MZ_SSHOTGUN | is_silenced);
	gi.multicast(ent->s.origin, MULTICAST_PVS);

	ent->client->ps.gunframe++;
	PlayerNoise(ent, start, PNOISE_WEAPON);

	if (!((int)dmflags->value & DF_INFINITE_AMMO))
		ent->client->pers.inventory[ent->client->ammo_index]
		-= ent->client->pers.weapon->quantity;
}

void Weapon_ExplosiveSuperShotgun(edict_t* ent)
{
	static int	pause_frames[] = { 29, 42, 57, 0 };
	static int	fire_frames[] = { 7, 0 };

	Weapon_Generic(ent, 6, 17, 57, 61, pause_frames, fire_frames, weapon_explosivesupershotgun_fire);
}

/*
======================================================================

SNIPER GUN

======================================================================
*/

void Sniper_Fire(edict_t* ent, vec3_t g_offset, int damage, qboolean hyper, int effect)
{
	vec3_t	forward, right;
	vec3_t	start;
	vec3_t	offset = { 0 };

	if (is_quad)
		damage *= 4;
	AngleVectors(ent->client->v_angle, forward, right, NULL);
	VectorSet(offset, 24, 8, ent->viewheight - 8.0);
	VectorAdd(offset, g_offset, offset);
	P_ProjectSource(ent->client, ent->s.origin, offset, forward, right, start);

	VectorScale(forward, -2, ent->client->kick_origin);
	ent->client->kick_angles[0] = -1;

	fire_sniper(ent, start, forward, damage, 2500, effect);
	if (ent->client && ent->client->pers.special == AODEATH)
	{
		fire_sniper(ent, ent->client->angel->s.origin, forward, damage, 2500, effect);
	}
	// send muzzle flash
	gi.WriteByte(svc_muzzleflash);
	gi.WriteShort(ent - g_edicts);
	if (hyper)
		gi.WriteByte(MZ_HYPERBLASTER | is_silenced);
	else
		gi.WriteByte(MZ_RAILGUN | is_silenced);
	gi.multicast(ent->s.origin, MULTICAST_PVS);

	PlayerNoise(ent, start, PNOISE_WEAPON);
}


void Weapon_Sniper_Fire(edict_t* ent)
{
	int		damage;

	if (deathmatch->value)
		damage = 300;
	else
		damage = 600;
	Sniper_Fire(ent, vec3_origin, damage, false, EF_BLASTER);
	ent->client->ps.gunframe++;
	if (!((int)dmflags->value & DF_INFINITE_AMMO))
		ent->client->pers.inventory[ent->client->ammo_index]
		-= ent->client->pers.weapon->quantity;
}

void Weapon_Sniper(edict_t* ent)
{
	static int	pause_frames[] = { 25, 33, 42, 50, 0 };
	static int	fire_frames[] = { 5, 0 };

	Weapon_Generic(ent, 4, 29, 52, 55, pause_frames, fire_frames, Weapon_Sniper_Fire);
}


/*
======================================================================

RAILGUN

======================================================================
*/

void weapon_railgun2_fire(edict_t* ent)
{
	vec3_t		start;
	vec3_t		forward, right;
	vec3_t		offset = { 0 };
	int			damage;
	int			kick;

	// Same "extreme" damage in SP game *and* deathmatch :)
	damage = 150;
	kick = 250;

	if (is_quad)
	{
		damage *= 4;
		kick *= 4;
	}

	AngleVectors(ent->client->v_angle, forward, right, NULL);

	VectorScale(forward, -3, ent->client->kick_origin);
	ent->client->kick_angles[0] = -3;

	VectorSet(offset, 0, 7, ent->viewheight - 8.0);
	P_ProjectSource(ent->client, ent->s.origin, offset, forward, right, start);
	fire_rail(ent, start, forward, damage, kick);
	if (ent->client && ent->client->pers.special == AODEATH)
	{
		fire_rail(ent, ent->client->angel->s.origin, forward, damage, kick);
	}
	// send muzzle flash
	gi.WriteByte(svc_muzzleflash);
	gi.WriteShort(ent - g_edicts);
	gi.WriteByte(MZ_RAILGUN | is_silenced);
	gi.multicast(ent->s.origin, MULTICAST_PVS);

	ent->client->ps.gunframe++;
	PlayerNoise(ent, start, PNOISE_WEAPON);

	if (!((int)dmflags->value & DF_INFINITE_AMMO))
		ent->client->pers.inventory[ent->client->ammo_index]
		-= ent->client->pers.weapon->quantity;
}


void Weapon_Railgun2(edict_t* ent)
{
	static int	pause_frames[] = { 56, 0 };
	static int	fire_frames[] = { 4, 0 };

	Weapon_Generic(ent, 3, 18, 56, 61, pause_frames, fire_frames, weapon_railgun2_fire);
}

void weapon_sonicrailgun_fire(edict_t* ent)
{
	vec3_t		start;
	vec3_t		forward, right;
	vec3_t		offset = { 0 };
	int			kick = 0;

	AngleVectors(ent->client->v_angle, forward, right, NULL);
	VectorScale(forward, -3, ent->client->kick_origin);
	ent->client->kick_angles[0] = -3;	VectorSet(offset, 0, 7, ent->viewheight - 8.0f);
	P_ProjectSource(ent->client, ent->s.origin, offset, forward, right, start);

	//Will:  Added for the sonic rail, it fires when released
	if (ent->client->buttons & BUTTON_ATTACK)
	{

		if (ent->SonicDamage < 240)
		{
			if ((ent->SonicDamage % 80) == 0)
				gi.sound(ent, CHAN_WEAPON, gi.soundindex("makron/rail_up.wav"), 1, ATTN_NONE, 0);
		}
		ent->SonicDamage += 10;
		if (ent->SonicDamage > 250)
			ent->SonicDamage = 250;	//This is the maximum damage of the Sonic Rail
		if (ent->SonicDamage == 240)
			gi.cprintf(ent, PRINT_HIGH, "Sonic Rail MAX DAMAGE\n");
		if (ent->SonicDamage == 240)
			gi.sound(ent, CHAN_WEAPON, gi.soundindex("misc/comp_up.wav"), 1, ATTN_NONE, 0);
		return;


	}
	if (ent->SonicDamage == 0)
	{
		ent->SonicDamage = 1;
		return;
	}
	if (ent->SonicDamage < 70)
		ent->SonicDamage = 70;	//This sets the minimum damage of the Sonic Rail
	//Will:  end of section
	if (is_quad)
	{
		ent->SonicDamage *= 4;
		kick *= 4;
	}

	fire_rail(ent, start, forward, ent->SonicDamage, kick);	// send muzzle flash
	if (ent->client && ent->client->pers.special == AODEATH)
		fire_rail(ent, ent->client->angel->s.origin, forward, ent->SonicDamage, kick);	// send muzzle flash

	gi.WriteByte(svc_muzzleflash);
	gi.WriteShort(ent - g_edicts);
	gi.WriteByte(MZ_RAILGUN | is_silenced);
	gi.multicast(ent->s.origin, MULTICAST_PVS);
	ent->client->ps.gunframe++;
	PlayerNoise(ent, start, PNOISE_WEAPON);
	if (!((int)dmflags->value & DF_INFINITE_AMMO))
		ent->client->pers.inventory[ent->client->ammo_index]--;
	ent->SonicDamage = 0;	//Will
}

void Weapon_SonicRailgun(edict_t* ent)
{
	static int	pause_frames[] = { 56, 0 };
	static int	fire_frames[] = { 4, 0 };

	Weapon_Generic(ent, 3, 18, 56, 61, pause_frames, fire_frames, weapon_sonicrailgun_fire);
}

/*
======================================================================

BFG10K

======================================================================
*/

void weapon_bfg_fire(edict_t* ent)
{
	vec3_t	offset = { 0 };
	vec3_t	start = { 0 };
	vec3_t	forward, right;
	int		damage;
	float	damage_radius = 1000;

	if (!ent->client)
		return;

	if (deathmatch->value)
		damage = 200;
	else
		damage = 500;

	if (is_quad)
		damage *= 4;

	VectorCopy(ent->s.origin, start);
	if (ent->client->ps.gunframe == 9)
	{
		// send muzzle flash
		gi.WriteByte(svc_muzzleflash);
		gi.WriteShort(ent - g_edicts);
		gi.WriteByte(MZ_BFG | is_silenced);
		gi.multicast(ent->s.origin, MULTICAST_PVS);

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

	AngleVectors(ent->client->v_angle, forward, right, NULL);

	VectorScale(forward, -2, ent->client->kick_origin);

	// make a big pitch kick with an inverse fall
	ent->client->v_dmg_pitch = -40;
	ent->client->v_dmg_roll = crandom() * 8;
	ent->client->v_dmg_time = level.time + DAMAGE_TIME;

	VectorSet(offset, 8, 8, ent->viewheight - 8.0);
	P_ProjectSource(ent->client, ent->s.origin, offset, forward, right, start);
	fire_bfg(ent, start, forward, damage, 400, damage_radius);
	if (ent->client && ent->client->pers.special == AODEATH)
	{
		fire_bfg(ent, ent->client->angel->s.origin, forward, damage, 400, damage_radius);
	}
	ent->client->ps.gunframe++;

	PlayerNoise(ent, start, PNOISE_WEAPON);

	if (!((int)dmflags->value & DF_INFINITE_AMMO))
		ent->client->pers.inventory[ent->client->ammo_index]
		-= ent->client->pers.weapon->quantity;
}

void Weapon_BFG(edict_t* ent)
{
	static int	pause_frames[] = { 39, 45, 50, 55, 0 };
	static int	fire_frames[] = { 9, 17, 0 };

	Weapon_Generic(ent, 8, 32, 55, 58, pause_frames, fire_frames, weapon_bfg_fire);
}

/*================= Think_Airstrike
CCH: This will bring the airstrike ordinance into existence in the game
JDB: Modified 5/4/98 Called by ClientThink =================*/
void Think_Airstrike(edict_t* ent)
{

	vec3_t	start = { 0 };
	vec3_t  forward;
	vec3_t  end;
	vec3_t	targetdir = { 0 };
	trace_t tr;
	trace_t tr_2;       // find the target point
	int		i, j;

	VectorCopy(ent->s.origin, start);
	start[2] += ent->viewheight;
	AngleVectors(ent->client->v_angle, forward, NULL, NULL);
	VectorMA(start, 8192, forward, end);
	tr = gi.trace(start, NULL, NULL, end, ent, MASK_SHOT | CONTENTS_SLIME | CONTENTS_LAVA);
	// find the direction from the entry point to the target
	VectorSubtract(tr.endpos, ent->client->airstrike_entry, targetdir);

	VectorNormalize(targetdir);
	VectorAdd(ent->client->airstrike_entry, targetdir, start);
	// check we have a clear line of fire
	tr_2 = gi.trace(start, NULL, NULL, tr.endpos, ent, MASK_SHOT | CONTENTS_SLIME | CONTENTS_LAVA);

	// check to make sure we're not materializing in a solid
	if (gi.pointcontents(start) == CONTENTS_SOLID || tr_2.fraction < 1.0)
	{
		gi.cprintf(ent, PRINT_HIGH, "Airstrike intercepted en route.\n");
		gi.sound(ent, CHAN_ITEM, gi.soundindex("world/pilot1.wav"), 0.8f, ATTN_NORM, 0);

		if (ent->lasersight != NULL)
			SP_LaserSight(ent);
		return;
	}

	gi.sound(ent, CHAN_ITEM, gi.soundindex("world/pilot2.wav"), 0.9f, ATTN_NORM, 0);
	gi.sound(ent, CHAN_AUTO, gi.soundindex("world/flyby1.wav"), 0.7f, ATTN_NORM, 0);

	// fire away!	
	j = 700;

	for (i = 0; i < 12; i++)
	{

		// David Martin - 07/15/2005 - randomize where the rockets are going to hit
		// and how fast they are going to hit
		//		VectorCopy(targetdir, finalTargetdir);
		//		finalTargetdir[0] = finalTargetdir[0] * (crandom()/crandom());
		//		finalTargetdir[1] = finalTargetdir[1] * (crandom()/crandom());
		//		finalTargetdir[2] = finalTargetdir[2] * (crandom()/crandom());
		//		VectorNormalize(finalTargetdir);
		//		j = 650 + (crandom() * 25);

		if (ent->client != NULL
			&& ent->client->airstrike_grentype == DM_NORMALGRENADE) // or if no dm grenade was selected
			fire_rocket(ent, start, targetdir,
				700,	/*damage*/
				j,		/*speed*/
				300,	/*damageradius*/
				450);	/*radiusdamage*/

		else
			// use the selected dM_grenade type when airstrike commanded
			fire_grenade_dM(ent, start, targetdir,
				150,	//damage
				j,		//speed
				100,	//timer
				300,	//damage radius
				ent->client->airstrike_grentype, //type of grenade held when airstrike called
				false, // held
				true); // bazookad
		j -= 30;
	}

	if (ent->lasersight != NULL)
		SP_LaserSight(ent);
	gi.cprintf(ent, PRINT_HIGH, "Airstrike has arrived.\n");
}

// This takes a string specifying a wave file
// such as "weapon/somewep%9fire.wav" and generates
// a sound index into some weapon wave array
// that is a function of a random number between
// 1 and the digit following the %.
//
#if 0
int ParseSound(char* in)
{
	char	ret[MAX_QPATH];
	char* p;
	char	digit[2] = "";
	int		rn;

	p = ret;
	while (*in != '%' && *in != '\0')
	{
		*p++ = *in++;
	}

	if (*in == '\0')
		return gi.soundindex(ret); // didn't find a %

	in++; //skip the % token
	*digit = *in++;
	rn = (rand() % atoi(digit)) + 1;
	itoa(rn, digit, 10);
	*p++ = *digit; //insert random digit

	//finish copying the string
	while (*in != '\0')
	{
		*p++ = *in++;
	}

	*p = *in;

	return gi.soundindex(ret);
}
#endif
