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

// g_local.h -- local definitions for game module

#pragma once

#ifdef _WIN32
  #define WIN32_LEAN_AND_MEAN	//non-MFC
  #include <windows.h>
  #define _CRTDBG_MAP_ALLOC
  #include <stdlib.h>
  #include <crtdbg.h>
  #ifndef __func__
    #define __func__ __FUNCTION__ // for backward compatibility
  #endif
  _CrtMemState startup1;	// memory diagnostics, link with debug lib (/MDd)
#else
  #define OutputDebugString	//not doing Windows
#endif

#include "q_shared.h"

// define GAME_INCLUDE so that game.h does not define the
// short, server-visible gclient_t and edict_t structures,
// because we define the full size ones in this file

#define	GAME_INCLUDE
#include "game.h"

#include "g_weapon.h"

//ZOID
#include "p_menu.h"
//ZOID

#define LOX

//============================================================================

#ifndef _DEBUG
#define BUILD	"Release"
#else
#define BUILD	"Debug  "
#endif

#define VERSION	"1.12.23"	//QW// single instance of version string

// the "gameversion" client command will print this plus compile date
#define	GAMEVERSION		"WOD:LOX, "VERSION	//deathmatch
#define	GAMEVERSION_E	"CTF:LOX, "VERSION	//teamplay or CTF
#define	GAMEVERSION_C	"COOP:LOX, "VERSION	//cooperative mode

//============================================================================

// protocol bytes that can be directly added to messages
#define	svc_muzzleflash		1
#define	svc_muzzleflash2	2
#define	svc_temp_entity		3
#define	svc_layout			4
#define	svc_inventory		5
#define	svc_stufftext		11

//============================================================================

//============================================================================
// Constants for interfacing to LOX weapons and features
//============================================================================

// Weapon banning flags.
//weaponban
#define WB_SUPERBLASTER				0x00000001	//1
#define WB_SHOTGUN					0x00000002	//2
#define WB_SNIPERGUN				0x00000004	//4
#define WB_SUPERSHOTGUN				0x00000008	//8

#define WB_FREEZEGUN				0x00000010	//16
#define WB_MACHINEGUN				0x00000020	//32
#define WB_MACHINEROCKETGUN			0x00000040	//64
#define WB_BURSTMACHINEGUN			0x00000080	//128

#define WB_CHAINGUN					0x00000100	//256
#define WB_STREETSWEEPER			0x00000200	//512
#define WB_GRENADE					0x00000400	//1024
#define WB_CLUSTERGRENADE			0x00000800	//2048

#define WB_RAILBOMB					0x00001000	//4096
#define WB_PLASMAGRENADE			0x00002000	//8192
#define WB_NAPALMGRENADE			0x00004000	//16384
#define WB_SHRAPNELGRENADE			0x00008000	//32768

#define WB_GRENADELAUNCHER			0x00010000	//65536
#define WB_BAZOOKA					0x00020000	//131072
#define WB_ROCKETLAUNCHER			0x00040000	//262144
#define WB_HOMINGROCKETLAUNCHER		0x00080000	//524288

#define WB_HYPERBLASTER				0x00100000	//1048576
#define WB_PLASMARIFLE				0x00200000	//2097152
#define WB_RAILGUN					0x00400000	//4194304
#define WB_FLAMETHROWER				0x00800000	//8388608

#define WB_BFG10K					0x01000000	//16777216


// Banning flags for the WoD:LOX weapon extensions

//loxweaponban
#define LWB_AIRFIST					0x00000001 //1
#define	LWB_SWORD					0x00000002 //2
#define LWB_PULSERIFLE				0x00000004 //4
#define LWB_NAILGUN					0x00000008 //8

#define LWB_BUCKY					0x00000010 //16
#define LWB_SKIPPERHOMINGROCKET		0x00000020 //32
#define LWB_CATACLYSMDEVICE			0x00000040 //64
#define LWB_GUIDEDNUKE				0x00000080 //128

#define LWB_GUIDEDMISSILE			0x00000100 //256
#define LWB_POSITRON				0x00000200 //512
#define LWB_STICKGRENADELAUNCHER    0x00000400 //1024
#define LWB_ICECUBEGUN				0x00000800 //2048

#define LWB_DURG					0x00001000 //4096
#define LWB_LIGHTNINGGUN			0x00002000 //8192
#define	LWB_DOUBLEIMPACT			0x00004000 //16384
#define LWB_FBFG					0x00008000 //32768

#define LWB_DISINTEGRATOR			0x00010000 //65536
#define LWB_VACUUMMAKER				0x00020000 //131072
#define LWB_ENERGYVORTEX			0x00040000 //262144
#define LWB_SPIRALROCKETLAUNCHER	0x00080000 //524288

#define LWB_SONICRAILGUN			0x00100000 //1048576
#define LWB_EXPLOSIVESHOTGUN		0x00200000 //2097152
#define LWB_EXPLOSIVESUPERSHOTGUN	0x00400000 //4194304
#define LWB_EXPLOSIVEMACHINEGUN		0x00800000 //8388608

#define LWB_KAMINIT					0x01000000 //16777216
#define LWB_WALLPIERCINGRAILGUN		0x02000000 //33554432
#define LWB_NUKE					0x04000000 //67108864
#define LWB_FLOATINGMINELAUNCHER	0x08000000 //134217728

#define LWB_ANTIMATTERCANNON		0x10000000 //268435456
#define LWB_SUPERRAILSHOTGUN		0x20000000 //536870912
#define LWB_FREEZATRON				0x40000000 //1073741824

//loxtrackerban
#define	LTB_BFG10K					0x00000001 //1
#define	LTB_FBFG					0x00000002 //2
#define LTB_FREEZEGUN				0x00000004 //4
#define LTB_NAILGUN					0x00000008 //8

#define LTB_BUCKY					0x00000010 //16
#define LTB_RAILGUN					0x00000020 //32
#define LTB_ENERGYVORTEX			0x00000040 //64
#define LTB_KAMINIT					0x00000080 //128

#define LTB_SNIPERGUN				0x00000100 //256
#define LTB_POSITRON				0x00000200 //512
#define LTB_STICKGRENADELAUNCHER    0x00000400 //1024
#define LTB_ICECUBEGUN				0x00000800 //2048

#define LTB_DURG					0x00001000 //4096
#define LTB_LIGHTNINGGUN			0x00002000 //8192
#define	LTB_DOUBLEIMPACT			0x00004000 //16384
#define LTB_DISINTEGRATOR			0x00008000 //32768

#define LTB_PULSERIFLE				0x00010000 //65536
#define LTB_EXPLOSIVESHOTGUN		0x00020000 //131072
#define LTB_EXPLOSIVESUPERSHOTGUN	0x00040000 //262144
#define LTB_EXPLOSIVEMACHINEGUN		0x00080000 //524288

#define LTB_FREEZATRON				0x00100000 //1048576

// sweeper bans
// loxsweeperban 
#define LSB_SUPERBLASTERSWEEPER		0x00000001
#define LSB_FREEZERSWEEPER			0x00000002
#define LSB_GRENADESWEEPER			0x00000004
#define LSB_BAZOOKASWEEPER			0x00000008

#define LSB_RAILGUNSWEEPER			0x00000010
#define LSB_FLAMESWEEPER			0x00000020
#define LSB_DURGSWEEPER				0x00000040
#define LSB_CHUNKGUN				0x00000080

#define LSB_SNIPERSWEEPER			0x00000100
#define	LSB_EXPLOSIVECHAINGUN		0x00000200
#define LSB_EXPLOSIVESTREETSWEEPER	0x00000400
#define LSB_STICKYGRENADESWEEPER	0x00000800

// loxgrenadeban 
#define LGB_PLASMACLUSTERGRENADE	0x00000001
#define LGB_TELEGRENADE				0x00000002
#define LGB_FREEZEGRENADE			0x00000004
#define LGB_HOMERGRENADE			0x00000008

#define LGB_ROCKETGRENADE			0x00000010
#define LGB_BLINDINGGRENADE			0x00000020
#define LGB_POSITRONGRENADE			0x00000040
#define LGB_SPINNINGRAILBOMB		0x00000080

#define LGB_MIDNIGHTGRENADE			0x00000100
#define LGB_BANZAIGRENADE			0x00000200
#define LGB_FREEZATRONGRENADE		0x00000400

// Armor banning flags for the WoD:LOX armor extensions
// loxarmorban 
#define LAB_ORGANICARMOR			0x00000001
#define LAB_SLUGARMOR				0x00000002
#define LAB_CELLARMOR				0x00000004
#define LAB_SHELLARMOR				0x00000008

#define LAB_BULLETARMOR				0x00000010
#define LAB_ROCKETARMOR				0x00000020
#define LAB_GRENADEARMOR			0x00000040

// loxfeatureban 
#define LFB_CLOAK					0x00000001
#define LFB_EXPLOSIVE				0x00000002
#define LFB_INVISIBILITY			0x00000004
#define LFB_FLASHLIGHT				0x00000008

#define LFB_CHECKSTATS				0x00000010	// ban the checkstats command
#define LFB_RANGEFINDER				0x00000020
#define LFB_PLAYERID				0x00000040
#define LFB_FEIGNDEATH				0x00000080

#define LFB_ANGELODEATH				0x00000100
#define LFB_ANGELOLIFE				0x00000200
#define LFB_ANGELOMERCY				0x00000400
#define LFB_ANGELOFROST				0x00000800

#define LFB_ANGELOPLAGUE			0x00001000
#define LFB_SWEEPERFIREMODE			0x00002000
#define LFB_HEIGHT					0x00004000
#define LFB_ANGELOFLAME				0x00008000

#define LFB_ANGELOHORROR			0x00010000
#define LFB_ANGELOBLIND				0x00020000
#define LFB_AIRSTRIKE				0x00040000
#define LFB_FPH						0x00080000	// ban fph counter in hud

#define LFB_TRACKER					0x00100000	// ban trackers
#define LFB_ANGELOREVENGE			0x00200000	// ban angel of revenge
#define LFB_BATON					0x00400000
#define LFB_HOLO					0x00800000

#define LFB_CHAMBER					0x01000000	// not sure if the chamber even works
#define LFB_REGENHEALTH				0x02000000	// ban regeneration health
#define LFB_ANGELOUNFREEZE			0x04000000	// ban angel of unfreeze
#define LFB_VAMPIHEALTH				0x08000000

#define LFB_ANGELOENERGY			0x10000000
#define LFB_TURRET					0x20000000	// ban turrets
#define LFB_NUMTRKR					0x40000000	//inhibit tracker counter in hud when set
#define LFB_NUMTRRT					0x80000000	//inhibit turret counter in hud when set

// Frag banning flags 
#define WFB_CATACLYSMDEVICE			0x00000001	//1
#define WFB_SUPERBLASTER			0x00000002	//2
#define WFB_FREEZATRONGRENADE		0x00000004	//4
#define WFB_SUPERRAILSHOTGUN		0x00000008	//8

#define WFB_FREEZEGUN				0x00000010	//16
#define WFB_MACHINEGUN				0x00000020	//32
#define WFB_MACHINEROCKETGUN		0x00000040	//64
#define WFB_BURSTMACHINEGUN			0x00000080	//128

#define WFB_CHAINGUN				0x00000100	//256
#define WFB_SWEEPERS				0x00000200	//512
#define WFB_GRENADE					0x00000400	//1024
#define WFB_CLUSTERGRENADE			0x00000800	//2048

#define WFB_RAILBOMB				0x00001000	//4096
#define WFB_PLASMAGRENADE			0x00002000	//8192
#define WFB_NAPALMGRENADE			0x00004000	//16384
#define WFB_SHRAPNELGRENADE			0x00008000	//32768

#define WFB_FREEZEGRENADE			0x00010000	//65536
#define WFB_BAZOOKASWEEPER			0x00020000	//131072
#define WFB_ROCKETLAUNCHER			0x00040000	//262144
#define WFB_HOMINGROCKETLAUNCHER	0x00080000	//524288

#define WFB_BLINDINGGRENADE			0x00100000	//1048576
#define WFB_PLASMARIFLE				0x00200000	//2097152
#define WFB_POSITRONGRENADE			0x00400000	//4194304
#define WFB_FLAMETHROWER			0x00800000	//8388608

#define WFB_BFG10K					0x01000000	//16777216
#define WFB_PLASMACLUSTERGRENADE	0x02000000
#define WFB_HOMERGRENADE			0x04000000
#define WFB_TELEFRAG				0x08000000

#define WFB_KAMIKAZE				0x10000000
#define WFB_HOOK					0x20000000
#define WFB_CHUNKGUN				0x40000000
#define WFB_ROCKETGRENADE			0x80000000

// Feature banning flags.
//featureban
#define FB_BANDOLIER				0x00000001
#define FB_AMMOPACK					0x00000002
#define FB_SILENCER					0x00000004
#define FB_REBREATHER				0x00000008

#define FB_ENVIROSUIT				0x00000010
#define FB_ADRENALINE				0x00000020
#define FB_MEGAHEALTH				0x00000040
#define FB_POWERARMOR				0x00000080

#define FB_INVULNERABILITY			0x00000100
#define FB_DECOY					0x00000200
#define FB_HOOK						0x00000400
#define FB_ID						0x00000800

#define FB_KAMIKAZE					0x00001000
#define FB_LASERSIGHT				0x00002000
#define FB_TRIPLASER				0x00004000
#define FB_NIGHTVISION				0x00008000

#define FB_PUSHPULL					0x00010000
#define FB_SCANNER					0x00020000
#define FB_ZOOM						0x00040000
#define FB_JETPACK					0x00080000

//==================================================================

// view pitching times
#define DAMAGE_TIME		0.5f
#define	FALL_TIME		0.3f


// edict->spawnflags
// these are set with checkboxes on each entity in the map editor
#define	SPAWNFLAG_NOT_EASY			0x00000100
#define	SPAWNFLAG_NOT_MEDIUM		0x00000200
#define	SPAWNFLAG_NOT_HARD			0x00000400
#define	SPAWNFLAG_NOT_DEATHMATCH	0x00000800
#define	SPAWNFLAG_NOT_COOP			0x00001000

// edict->flags
#define	FL_FLY					0x00000001
#define	FL_SWIM					0x00000002	// implied immunity to drowning
#define FL_IMMUNE_LASER			0x00000004
#define	FL_INWATER				0x00000008
#define	FL_GODMODE				0x00000010
#define	FL_NOTARGET				0x00000020
#define FL_IMMUNE_SLIME			0x00000040
#define FL_IMMUNE_LAVA			0x00000080
#define	FL_PARTIALGROUND		0x00000100	// not all corners are valid
#define	FL_WATERJUMP			0x00000200	// player jumping out of water
#define	FL_TEAMSLAVE			0x00000400	// not the first on the team
#define FL_NO_KNOCKBACK			0x00000800
#define FL_POWER_ARMOR			0x00001000	// power armor (if any) is active
#define FL_RESPAWN				0x80000000	// used for item respawning
#define FL_FROZEN				0x00004000	// for ice cube gun
#define FL_WASFLYING			0x00008000	// for ice cube gun

#define	FRAMETIME		0.1f

// memory tags to allow dynamic memory to be cleaned up
#define	TAG_GAME	765		// clear when unloading the dll
#define	TAG_LEVEL	766		// clear when loading a new level


#define MELEE_DISTANCE	80

#define BODY_QUEUE_SIZE		8

// sweeper fire mode booleans //QW//
typedef enum sweepermode_n
{
	SFM_LOW,
	SFM_HIGH
} bool_n;

#ifdef _WIN32
//weapon heat parameters for sweepers //QW//
#define WEPHEAT_HIGH	7	// heatup rate in high firing rate
#define WEPHEAT_LOW		10	// heatup rate in low firing rate
#define WEPHEAT_COOLING	1.0	// cooldown rate when not firing
#else
// for some strange reason some linux servers will run
// the weapon heat up faster than windows servers.
// Use this to adjust the heating down for them.
#define WEPHEAT_HIGH	7	// heatup rate in high firing rate
#define WEPHEAT_LOW		10	// heatup rate in low firing rate
#define WEPHEAT_COOLING	1.0	// cooldown rate when not firing
#endif

typedef enum damage_n
{
	DAMAGE_NO,
	DAMAGE_YES,			// will take damage if hit
	DAMAGE_AIM			// auto targeting recognizes this
} damage_t;

typedef enum weaponstate_n
{
	WEAPON_READY, 
	WEAPON_ACTIVATING,
	WEAPON_DROPPING,
	WEAPON_FIRING
} weaponstate_t;

typedef enum ammo_n
{
	AMMO_BULLETS,
	AMMO_SHELLS,
	AMMO_ROCKETS,
	AMMO_GRENADES,
	AMMO_CELLS,
	AMMO_SLUGS
} ammo_t;


//deadflag
#define DEAD_NO					0
#define DEAD_DYING				1
#define DEAD_DEAD				2
#define DEAD_RESPAWNABLE		3

//range
#define RANGE_MELEE				0
#define RANGE_NEAR				1
#define RANGE_MID				2
#define RANGE_FAR				3

//gib types
#define GIB_ORGANIC				0
#define GIB_METALLIC			1

//monster ai flags
#define AI_STAND_GROUND			0x00000001
#define AI_TEMP_STAND_GROUND	0x00000002
#define AI_SOUND_TARGET			0x00000004
#define AI_LOST_SIGHT			0x00000008
#define AI_PURSUIT_LAST_SEEN	0x00000010
#define AI_PURSUE_NEXT			0x00000020
#define AI_PURSUE_TEMP			0x00000040
#define AI_HOLD_FRAME			0x00000080
#define AI_GOOD_GUY				0x00000100
#define AI_BRUTAL				0x00000200
#define AI_NOSTEP				0x00000400
#define AI_DUCKED				0x00000800
#define AI_COMBAT_POINT			0x00001000
#define AI_MEDIC				0x00002000
#define AI_RESURRECTING			0x00004000

//monster attack state
#define AS_STRAIGHT				1
#define AS_SLIDING				2
#define	AS_MELEE				3
#define	AS_MISSILE				4

// armor types
#define ARMOR_NONE				0
#define ARMOR_JACKET			1
#define ARMOR_COMBAT			2
#define ARMOR_BODY				3
#define ARMOR_SHARD				4

// power armor types
#define POWER_ARMOR_NONE		0
#define POWER_ARMOR_SCREEN		1
#define POWER_ARMOR_SHIELD		2

// handedness values
#define RIGHT_HANDED			0
#define LEFT_HANDED				1
#define CENTER_HANDED			2


// game.serverflags values
#define SFL_CROSS_TRIGGER_1		0x00000001
#define SFL_CROSS_TRIGGER_2		0x00000002
#define SFL_CROSS_TRIGGER_3		0x00000004
#define SFL_CROSS_TRIGGER_4		0x00000008
#define SFL_CROSS_TRIGGER_5		0x00000010
#define SFL_CROSS_TRIGGER_6		0x00000020
#define SFL_CROSS_TRIGGER_7		0x00000040
#define SFL_CROSS_TRIGGER_8		0x00000080
#define SFL_CROSS_TRIGGER_MASK	0x000000ff


// noise types for PlayerNoise
#define PNOISE_SELF				0
#define PNOISE_WEAPON			1
#define PNOISE_IMPACT			2


// edict->movetype values
typedef enum movetype_n
{
MOVETYPE_NONE,			// never moves
MOVETYPE_NOCLIP,		// origin and angles change with no interaction
MOVETYPE_PUSH,			// no clip to world, push on box contact
MOVETYPE_STOP,			// no clip to world, stops on box contact

MOVETYPE_WALK,			// gravity
MOVETYPE_STEP,			// gravity, special edge handling
MOVETYPE_FLY,
MOVETYPE_TOSS,			// gravity
MOVETYPE_FLYMISSILE,	// extra size to monsters
MOVETYPE_BOUNCE,
MOVETYPE_BOUNCE_NO_STOP,
MOVETYPE_BOUNCE_MORE,
MOVETYPE_FLYRICOCHET,
MOVETYPE_BOUNCEFLY,
MOVETYPE_FREEZE,       // player freeze, used for Zaero Camera
MOVETYPE_FALLFLOAT,		// falls down slopes and floats in water
MOVETYPE_RIDE,                   // basically won't move unless it rides on a MOVETYPE_PUSH entity
//Wyrm new movetype
MOVETYPE_STUCK,
MOVETYPE_HOMING
} movetype_t;

//class enumerations to substitute for class names
typedef enum classnum_n
{
	CN_NOCLASS,
	CN_AODEATH,		//angel of death
	CN_AOLIFE,		//angel of life
	CN_AOMERCY,		//angel of mercy
	CN_AOFROST,		//angel of frost	
	CN_AOPLAGUE,	//angel of plague
	CN_AOFLAME,		//angel of flame
	CN_AOHORROR,	//angel of horror
	CN_AOBLIND,		//angel of blindness
	CN_AOREVENGE,	//angel of revenge
	CN_AOUNFREEZE,	//angel of unfreeze
	CN_AOENERGY,	//angel of energy
	CN_MAGE,		//mage?
	CN_TRACKER,
	CN_TURRET,
	CN_CHAMBER,
	CN_BATON,
	CN_DECOY,
	CN_DEBRIS,
	CN_GRENADE,
	CN_PLASMAEXPLOSION,
	CN_PLASMA,
	CN_ROCKET,
	CN_BFGBLAST,
	CN_BOLT,
	CN_SUPERSHOT,
	CN_BARREL,
	CN_POSITRON,
	CN_FREEZATRON,
	CN_FLARE,
	CN_ANTIFLARE,
	CN_BUCKEYBALL,
	CN_SPIKE,
	CN_PULSE,
	CN_FLOATINGMINE,
	CN_NUKE,
	CN_CHASECAM,
	CN_CHUNK,
	CN_DISINTEGRATOR,
	CN_NAPALM,
	CN_SHRAPNEL,
	CN_FREEZEEXPLOSION,
	CN_FLAME,
	CN_ICE,
	CN_GUIDEDNUKE,
	CN_GUIDEDROCKET,
	CN_HOMINGROCKET,
	CN_POSITRONEXPLOSION,
	CN_FREEZATRONEXPLOSION,
	CN_CATACLYSMEXPLOSION,
	CN_HANDGRENADE,
	CN_LASERTRIP_GRENADE,
	CN_VORTEXBLAST,
	CN_PACK,
	CN_PLAYER
} classnum_t;	// class numbers

typedef struct gitem_armor_s
{
	int		base_count;
	int		max_count;
	float	normal_protection;
	float	energy_protection;
	int		armor;
} gitem_armor_t;


// gitem_t->flags
#define IT_WEAPON		1		// use makes active weapon
#define IT_AMMO			2
#define IT_ARMOR		4
#define IT_NOT_DM		8		// Doesn't appear in DM, so don't send configstring
#define IT_STAY_COOP	16
#define IT_KEY			32
#define IT_POWERUP		64
#define IT_ALTWEAPON	128	// for weapon switching (see Cmd_Use_f())

// gitem_t->weapmodel for weapons indicates model index
#define WEAP_BLASTER			1 
#define WEAP_SHOTGUN			2 
#define WEAP_SUPERSHOTGUN		3 
#define WEAP_MACHINEGUN			4 
#define WEAP_CHAINGUN			5 
#define WEAP_GRENADES			6 
#define WEAP_GRENADELAUNCHER	7 
#define WEAP_ROCKETLAUNCHER		8 
#define WEAP_HYPERBLASTER		9 
#define WEAP_RAILGUN			10
#define WEAP_BFG				11

typedef struct gitem_s
{
	char* classname;	// spawning name
	qboolean(*pickup)(struct edict_s* ent, struct edict_s* other);
	void		(*use)(struct edict_s* ent, struct gitem_s* item);
	void		(*drop)(struct edict_s* ent, struct gitem_s* item);
	void		(*weaponthink)(struct edict_s* ent);
	char*		pickup_sound;
	char*		world_model;
	int			world_model_flags;
	char*		view_model;

	// client side info
	char*	icon;
	char*	pickup_name;		// for printing on pickup
	int		count_width;		// number of digits to display by icon

	const int		quantity;	// for ammo how much, for weapons how much is used per shot
	struct gitem_s* ammo;		// for weapons
	int		flags;				// IT_* flags

	void*	info;
	int		tag;

	char* precaches;		// string of all models, sounds, and images this item will use

	int		item_index;		// where this is in itemlist[]
} gitem_t;


char statusbar[1390];	// this is the status bar (HUD) buffer for DM and CTF //QW//

//
// this structure is left intact through an entire game
// it should be initialized at dll load time, and read/written to
// the server.ssv file for savegames
//
typedef struct game_locals_s
{
	char		helpmessage1[512];
	char		helpmessage2[512];
	int			helpchanged;	// flash F1 icon if non 0, play sound
								// and increment only if 1, 2, or 3

	gclient_t	*clients;		// [maxclients]

	// can't store spawnpoint in level, because
	// it would get overwritten by the savegame restore
	char		spawnpoint[512];	// needed for coop respawns

	// store latched cvars here that we want to get at often
	int			maxclients;
	int			maxentities;
	int			num_clients;
	int			peak_edicts;	// max num_edicts used

	// cross level triggers
	int			serverflags;

	// items
	int			num_items;

	qboolean	autosaved;

	char*		hudnames[5];
	int			revengereward;		// default set to 1
	int			highfraggeraward;	// default set to 2
	char*		statusbar;			// this serves as status bar in DM and CTF //QW//
	char*		heat_bar[14];
	char*		curr_heat_bar;
	int			curr_heat_bar_counter;
} game_locals_t;


//
// this structure is cleared as each map is entered
// it is read/written to the level.sav file for savegames
//
typedef struct level_locals_s
{
	int			framenum;
	float		time;

	char		level_name[MAX_QPATH];	// the descriptive name (Outer Base, etc)
	char		mapname[MAX_QPATH];		// the server name (base1, etc)
	char		nextmap[MAX_QPATH];		// go here when fraglimit is hit
	char		forcemap[MAX_QPATH];	// map to use on elections

	// intermission state
	float		intermissiontime;		// time the intermission was started
	char		*changemap;
	int			exitintermission;
	vec3_t		intermission_origin;
	vec3_t		intermission_angle;

	edict_t		*sight_client;	// changed once each frame for coop games

	edict_t		*sight_entity;
	int			sight_entity_framenum;
	edict_t		*sound_entity;
	int			sound_entity_framenum;
	edict_t		*sound2_entity;
	int			sound2_entity_framenum;

	int			pic_health;

	int			total_secrets;
	int			found_secrets;

	int			total_goals;
	int			found_goals;

	int			total_monsters;
	int			killed_monsters;

	edict_t		*current_entity;	// entity running from G_RunFrame
	int			body_que;			// dead bodies

	int			power_cubes;		// ugly necessity for coop

	// for night-day system
	char	    lights[2];		// a modifiable string
	int			lightcount1;
	int			lightcount2;
	int			updown;			// 0 up, 1 down
	int			cycletime;	// server ticks per light level

	// The time/frags from round 1.  (Needed for teamplay.)
	float		roundTimelimit;
	int			roundFraglimit;
} level_locals_t;


// spawn_temp_t is only used to hold entity field values that
// can be set from the editor, but aren't actualy present
// in edict_t during gameplay
typedef struct spawn_temp_s
{
	// world vars
	char		*sky;
	float		skyrotate;
	vec3_t		skyaxis;
	char		*nextmap;

	int			lip;
	int			distance;
	int			height;
	char		*noise;
	float		pausetime;
	char		*item;
	char		*gravity;	// this is entity gravity not server gravity

	float		minyaw;
	float		maxyaw;
	float		minpitch;
	float		maxpitch;
} spawn_temp_t;


typedef struct moveinfo_s
{
	// fixed data
	vec3_t		start_origin;
	vec3_t		start_angles;
	vec3_t		end_origin;
	vec3_t		end_angles;

	int			sound_start;
	int			sound_middle;
	int			sound_end;

	float		accel;
	float		speed;
	float		decel;
	float		distance;

	float		wait;
	// state data
	int			state;
	vec3_t		dir;
	float		current_speed;
	float		move_speed;
	float		next_speed;
	float		remaining_distance;
	float		decel_distance;
	void		(*endfunc)(edict_t *);
} moveinfo_t;


typedef struct mframe_s
{
	void	(*aifunc)(edict_t *self, float dist);
	float	dist;
	void	(*thinkfunc)(edict_t *self);
} mframe_t;

typedef struct mmove_s
{
	int			firstframe;
	int			lastframe;
	mframe_t	*frame;
	void		(*endfunc)(edict_t *self);
} mmove_t;

typedef struct monsterinfo_s
{
	mmove_t		*currentmove;
	int			aiflags;
	int			nextframe;
	float		scale;

	void		(*stand)(edict_t *self);
	void		(*idle)(edict_t *self);
	void		(*search)(edict_t *self);
	void		(*walk)(edict_t *self);
	void		(*run)(edict_t *self);
	void		(*dodge)(edict_t *self, edict_t *other, float eta);
	void		(*attack)(edict_t *self);
	void		(*melee)(edict_t *self);
	void		(*sight)(edict_t *self, edict_t *other);
	qboolean	(*checkattack)(edict_t *self);

	float		pausetime;
	float		attack_finished;

	vec3_t		saved_goal;
	float		search_time;
	float		trail_time;
	vec3_t		last_sighting;
	int			attack_state;
	int			lefty;
	float		idle_time;
	int			linkcount;

	int			power_armor_type;
	int			power_armor_power;
} monsterinfo_t;

extern	game_locals_t	game;
extern	level_locals_t	level;
extern	game_import_t	gi;
extern	game_export_t	globals;
extern	spawn_temp_t	st;

extern	int	vwep_index;
extern	int	sm_meat_index;
extern	int	snd_fry;

// means of death
#define MOD_UNKNOWN			0
#define MOD_BLASTER			1
#define MOD_SHOTGUN			2
#define MOD_SSHOTGUN		3
#define MOD_MACHINEGUN		4
#define MOD_CHAINGUN		5
#define MOD_GRENADE			6
#define MOD_G_SPLASH		7
#define MOD_ROCKET			8
#define MOD_R_SPLASH		9
#define MOD_HYPERBLASTER	10
#define MOD_RAILGUN			11
#define MOD_BFG_LASER		12
#define MOD_BFG_BLAST		13
#define MOD_BFG_EFFECT		14
#define MOD_HANDGRENADE		15
#define MOD_HG_SPLASH		16
#define MOD_WATER			17
#define MOD_SLIME			18
#define MOD_LAVA			19
#define MOD_CRUSH			20
#define MOD_TELEFRAG		21
#define MOD_FALLING			22
#define MOD_SUICIDE			23
#define MOD_HELD_GRENADE	24
#define MOD_EXPLOSIVE		25
#define MOD_BARREL			26
#define MOD_BOMB			27
#define MOD_EXIT			28
#define MOD_SPLASH			29
#define MOD_TARGET_LASER	30
#define MOD_TRIGGER_HURT	31
#define MOD_HIT				32
#define MOD_TARGET_BLASTER	33
#define MOD_NAPALM			34
#define MOD_PROXIMITY		35
#define MOD_CATA			36
#define MOD_CLUSTER			37
#define MOD_RAILBOMB		38
#define MOD_PLASMAG			39
#define MOD_GRAPPLE			40
#define MOD_SB				41
#define MOD_FGRENADE		42
#define MOD_TRIPWIRE		43
#define MOD_KAMIKAZE		44
#define MOD_SNIPER			45
#define MOD_PLASMAGUN		46
#define MOD_H_SPLASH		47
#define MOD_HOMING			48
#define MOD_SHRAPNEL		49
#define MOD_SHRAPG			50
#define MOD_MACHINE			51
#define MOD_RAILGUN2		52
#define MOD_FREEZE			53
#define MOD_STREETSWEEP		54
#define MOD_BOLTTHROWER		55
#define MOD_SUPERBLASTERSWEEPER 56
#define MOD_FREEZERSWEEPER	57
#define MOD_GRENADESWEEPER	58
#define MOD_BAZOOKASWEEPER	59
#define MOD_RAILGUNSWEEPER	60
#define MOD_TELEPORTGRENADE 61
#define MOD_PLASMACLUSTERGRENADE 62
#define MOD_AIRFIST			63
#define MOD_POSITRON		64
#define MOD_SWORD			65
#define MOD_BUCKY			66
#define MOD_DISCHARGE		68
#define MOD_LIGHTNING		69
#define MOD_SUPERNAIL		70
#define MOD_NAIL			71
#define MOD_ROCKETGRENADE   72
#define MOD_HOMERGRENADE    73
#define MOD_EGG				74
#define MOD_FIRE_SPLASH		75
#define MOD_STICKINGGRENADES 76
#define MOD_PLAGUE			 77
#define MOD_BATON			78
#define MOD_DISRUPTOR		79
#define MOD_VACUUM			80
#define MOD_VORTEX			81
#define MOD_CHAMBER			82
#define MOD_EXPLOSIVESHOTGUN	83
#define MOD_EXPLOSIVESSHOTGUN	84
#define MOD_EXPLOSIVECHAINGUN	85
#define MOD_EXPLOSIVESTREETSWEEPER	86
#define MOD_NUKE				87
#define MOD_ANTIMATTERCANNON	88
#define MOD_GUIDEDNUKE			89
#define MOD_FBFG_BLAST			90
#define MOD_SUPERRAILSHOTGUN	91
#define MOD_FREEZATRON			92
#define MOD_EXPLOSIVEMACHINEGUN 93
#define MOD_PUSHPULL			94
#define MOD_HELD_NUKE			95
#define MOD_NUKE_BLAST			96

#define MOD_TRACKER_FIRE	0x00010000
#define MOD_TURRET_FIRE		0x00020000

#define MOD_FRIENDLY_FIRE	0x04000000
#define MOD_NOFRAG			0x08000000

extern	int	meansOfDeath;

#define q_offsetof(t, m)    ((size_t)&((t *)0)->m)

extern	edict_t			*g_edicts;

#define FOFS(x)     q_offsetof(edict_t, x)
#define STOFS(x)    q_offsetof(spawn_temp_t, x)
#define	LLOFS(x)    q_offsetof(level_locals_t, x)
#define	CLOFS(x)    q_offsetof(gclient_t, x)

#define random()	((rand () & 0x7fff) / ((float)0x7fff))
#define crandom()	(2.0f * (random() - 0.5f))

extern	cvar_t	*maxentities;
extern	cvar_t	*deathmatch;
extern	cvar_t	*coop;
extern	cvar_t	*dmflags;
extern	cvar_t	*skill;
extern	cvar_t	*noscore;
extern	cvar_t	*fraglimit;
extern	cvar_t	*timelimit;
extern	cvar_t	*scoreboardtime;
extern	cvar_t	*cycle_always;
extern	cvar_t	*spectator_password;
extern	cvar_t	*needpass;
extern	cvar_t	*g_select_empty;
extern	cvar_t	*dedicated;
extern  cvar_t	*developer;
extern	cvar_t	*password;
extern	cvar_t	*filterban;
extern	cvar_t	*sv_gravity;
extern	cvar_t	*sv_maxvelocity;

extern	cvar_t	*gun_x, *gun_y, *gun_z;
extern	cvar_t	*rollspeed;
extern	cvar_t	*rollangle;

extern	cvar_t	*run_pitch;
extern	cvar_t	*run_roll;
extern	cvar_t	*bob_up;
extern	cvar_t	*bob_pitch;
extern	cvar_t	*bob_roll;

extern	cvar_t	*sv_cheats;
extern	cvar_t	*maxclients;
extern	cvar_t	*maxspectators;

extern	cvar_t	*flood_msgs;
extern	cvar_t	*flood_persecond;
extern	cvar_t	*flood_waitdelay;

extern	cvar_t	*gamedir;
extern	cvar_t	*basedir;
extern	cvar_t	*motdfile;

extern	cvar_t	*teamplay;
extern	cvar_t	*matchplay;

extern	cvar_t	*weaponban;
extern	cvar_t	*featureban;
extern	cvar_t	*fragban;
extern	cvar_t  *loxweaponban;
extern	cvar_t	*loxfeatureban;
extern	cvar_t	*loxgrenadeban;
extern	cvar_t	*loxarmorban;
extern	cvar_t	*loxsweeperban;
extern	cvar_t	*loxtrackerban;	//QW//
extern	cvar_t	*loxturretban;	//TheLopez//
extern	cvar_t	*p_pickup_regen;//QW//
extern	cvar_t	*p_pickup_vampiric;
extern	cvar_t	*p_pickup_invulnerability;
extern	cvar_t	*p_pickup_organic_armor;
extern	cvar_t	*p_pickup_slug_armor;
extern	cvar_t	*p_pickup_cell_armor;
extern	cvar_t	*p_pickup_shell_armor;
extern	cvar_t	*p_pickup_bullet_armor;
extern	cvar_t	*p_pickup_rocket_armor;
extern	cvar_t	*p_pickup_grenade_armor;
extern	cvar_t	*revenge_factor;
extern	cvar_t	*anticamper;
extern	cvar_t	*highfragger;
extern	cvar_t	*weaponheat;
extern	cvar_t	*revengereward;
extern	cvar_t	*highfraggerreward;
extern	cvar_t	*sv_bestplayer; 
extern	cvar_t	*push_kill_award; 
extern	cvar_t	*push_kill_delay;
extern	cvar_t	*autoweaponselect; //QW// for coop pickup_weapon
extern	cvar_t	*fast_weapon_change;	//QW//enable/disable fast weapon change
extern	cvar_t  *peak_edicts;		//QW// the max number of edicts used in the game to date
extern	cvar_t	*max_cats;		//QW// maximum cataclysm devices per player per level
extern	cvar_t	*zbot_check;	//QW// enable/disable internal zbot checking
extern	cvar_t	*console_chat;
extern	cvar_t	*flashlightmode; //QW/ mode for flashlight code.
//QW//

// these contain the conversion of the strings in the corresponding
// cvars so we can bitwise AND them with the weapon ban constants.
// they are defined in g_main.c
extern unsigned long i_weaponban;
extern unsigned long i_featureban;
extern unsigned long i_fragban;
extern unsigned long i_loxweaponban;
extern unsigned long i_loxfeatureban;
extern unsigned long i_loxgrenadeban;
extern unsigned long i_loxarmorban;
extern unsigned long i_loxsweeperban;
extern unsigned long i_loxtrackerban;
extern unsigned long i_loxturretban;

//QwazyWabbit's cvars
extern	cvar_t	*debugmodels;		// 1 = print debug_models messages
extern	cvar_t	*ext_devt;			// set to survey maps for extinction mode
extern	cvar_t	*staticlights;		// set static map lighting mode
extern	cvar_t	*max_trackers;		// number of trackers a player can launch
extern	cvar_t	*max_turrets;		// number of turrets a player can launch
extern	cvar_t	*max_lasertrips;	// number of lasers a player can attach
extern	cvar_t	*max_floatingmines;	// number of floating mines a player can launch
extern	cvar_t	*kami_frozen;		// can we kaminit a frozen player?

extern	cvar_t	*campertime;		// time allowed for camping if ANTICAMPER is on
extern	cvar_t	*dumpents;			// create ent files if set
extern	cvar_t	*custom_ents;		// use custom ent files if available
extern	cvar_t	*homerseekowner;	// boolean to allow homers to seek owners (default = 1)
extern	cvar_t	*logstats;			// boolean to enable player stats log (default = 1)
extern	cvar_t	*newscoreboard;		// Use the new DM scoreboard
extern	cvar_t	*respawn_protect;	// admin configurable respawn protection
extern	cvar_t	*g_rockets_takedamage; // admin config. guided rocket damage
extern	cvar_t	*g_nukes_takedamage;	// admin config. guided nukes take damage

// controls for day-night cycling
extern	cvar_t	*nightdaytime;		// duration of a "day"
extern	cvar_t	*minlevel;			// lowest light level to apply

//HACK

#define world	(&g_edicts[0])

// item spawnflags
#define ITEM_TRIGGER_SPAWN		0x00000001
#define ITEM_NO_TOUCH			0x00000002
// 6 bits reserved for editor flags
// 8 bits used as power cube id bits for coop games
#define DROPPED_ITEM			0x00010000
#define	DROPPED_PLAYER_ITEM		0x00020000
#define ITEM_TARGETS_USED		0x00040000

//
// fields are needed for spawning from the entity string
// and saving / loading games
//
#define FFL_SPAWNTEMP		1
#define FFL_NOSPAWN			2

//field types 
typedef enum fieldtype_n
{
	F_INT, 
	F_FLOAT,
	F_LSTRING,			// string on disk, pointer in memory, TAG_LEVEL
	F_GSTRING,			// string on disk, pointer in memory, TAG_GAME
	F_VECTOR,
	F_ANGLEHACK,
	F_EDICT,			// index on disk, pointer in memory
	F_ITEM,				// index on disk, pointer in memory
	F_CLIENT,			// index on disk, pointer in memory
	F_FUNCTION,
	F_MMOVE,
	F_IGNORE
} fieldtype_t;

typedef struct field_s
{
	char	*name;
	int		ofs;
	fieldtype_t	type;
	int		flags;
} field_t;

int	jacket_armor_index;
int	combat_armor_index;
int	body_armor_index;
int	power_screen_index;
int	power_shield_index;

extern	gitem_armor_t	jacketarmor_info;//	= { 25,  50, .30, .00, ARMOR_JACKET};
extern	gitem_armor_t	combatarmor_info;//	= { 50, 100, .60, .30, ARMOR_COMBAT};
extern	gitem_armor_t	bodyarmor_info;//	= {100, 200, .80, .60, ARMOR_BODY};
qboolean is_quad;
byte     is_silenced;

extern	field_t fields[];
extern	gitem_t	*itemlist[];
extern	gitem_t	*itemlistSorted[];

extern gitem_t gI_ammo_bullets;
extern gitem_t gI_ammo_cells;
extern gitem_t gI_ammo_grenades;
extern gitem_t gI_ammo_rockets;
extern gitem_t gI_ammo_shells;
extern gitem_t gI_ammo_slugs;

extern gitem_t gI_item_adrenaline;
extern gitem_t gI_item_ancient_head;
extern gitem_t gI_item_armor_body;
extern gitem_t gI_item_armor_combat;
extern gitem_t gI_item_armor_jacket;
extern gitem_t gI_item_armor_shard;
extern gitem_t gI_item_bandolier;
extern gitem_t gI_item_breather;
extern gitem_t gI_item_enviro;
extern gitem_t gI_item_invulnerability;
extern gitem_t gI_item_jetpack;
extern gitem_t gI_item_pack;
extern gitem_t gI_item_power_screen;
extern gitem_t gI_item_power_shield;
extern gitem_t gI_item_quad;
extern gitem_t gI_item_silencer;

extern gitem_t gI_weapon_airfist;
extern gitem_t gI_weapon_bfg;
extern gitem_t gI_weapon_blaster;
extern gitem_t gI_weapon_flaregun;
extern gitem_t gI_weapon_antiflaregun;
extern gitem_t gI_weapon_mace;
extern gitem_t gI_weapon_pulserifle;
extern gitem_t gI_weapon_shotgun;
extern gitem_t gI_weapon_supershotgun;
extern gitem_t gI_weapon_superrailshotgun;
extern gitem_t gI_weapon_machine;
extern gitem_t gI_weapon_machinegun;
extern gitem_t gI_weapon_freezer;
extern gitem_t gI_weapon_machinerocketgun;
extern gitem_t gI_weapon_explosivesupershotgun;
extern gitem_t gI_weapon_superfreezer;
extern gitem_t gI_weapon_chaingun;
extern gitem_t gI_weapon_explosivechaingun;
extern gitem_t gI_weapon_streetsweeper;
extern gitem_t gI_weapon_explosivestreetsweeper;
extern gitem_t gI_weapon_railgunsweeper;
extern gitem_t gI_weapon_rocketsweeper;
extern gitem_t gI_weapon_bazookasweeper;
extern gitem_t gI_weapon_grenadesweeper;
extern gitem_t gI_weapon_grenadesweeper;
extern gitem_t gI_weapon_superblastersweeper;
extern gitem_t gI_weapon_freezersweeper;
extern gitem_t gI_weapon_flamesweeper;
extern gitem_t gI_weapon_snipersweeper;
extern gitem_t gI_weapon_stickygrenadesweeper;
extern gitem_t gI_weapon_explosivemachinegun;
extern gitem_t gI_weapon_kaminit;
extern gitem_t gI_weapon_wallpiercingrailgun;
extern gitem_t gI_weapon_sword;
extern gitem_t gI_weapon_icecubegun;
extern gitem_t gI_weapon_chunkgun;
extern gitem_t gI_weapon_clustergrenade;
extern gitem_t gI_weapon_railbomb;
extern gitem_t gI_weapon_plasmagrenade;
extern gitem_t gI_weapon_napalmgrenade;
extern gitem_t gI_weapon_shrapnelgrenade;
extern gitem_t gI_weapon_cataclysm;
extern gitem_t gI_weapon_plasmaclustergrenade;
extern gitem_t gI_weapon_telegrenade;
extern gitem_t gI_weapon_freezegrenade;
extern gitem_t gI_weapon_homergrenade;
extern gitem_t gI_weapon_rocketgrenade;
extern gitem_t gI_weapon_blindinggrenade;
extern gitem_t gI_weapon_positrongrenade;
extern gitem_t gI_weapon_freezatrongrenade;
extern gitem_t gI_weapon_midnightgrenade;
extern gitem_t gI_weapon_banzaigrenade;
extern gitem_t gI_weapon_spinningrailbomb;
extern gitem_t gI_weapon_grenadelauncher;
extern gitem_t gI_weapon_stickinggrenadelauncher;
extern gitem_t gI_weapon_bucky;
extern gitem_t gI_weapon_bazooka;
extern gitem_t gI_weapon_rocketlauncher;
extern gitem_t gI_weapon_spiralrocketlauncher;
extern gitem_t gI_weapon_homing;
extern gitem_t gI_weapon_skipper;
extern gitem_t gI_weapon_disintegrator;
extern gitem_t gI_weapon_positron;
extern gitem_t gI_weapon_freezatron;
extern gitem_t gI_weapon_icecubegun;
extern gitem_t gI_weapon_lightninggun;
extern gitem_t gI_weapon_nailgun;
extern gitem_t gI_weapon_guided;
extern gitem_t gI_weapon_hyperblaster;
extern gitem_t gI_weapon_superblaster;
extern gitem_t gI_weapon_railgun;
extern gitem_t gI_weapon_railgun2;
extern gitem_t gI_weapon_sonicrailgun;
extern gitem_t gI_weapon_nuke;
extern gitem_t gI_weapon_durg;
extern gitem_t gI_weapon_fbfg;
extern gitem_t gI_weapon_sniper;
extern gitem_t gI_weapon_plasma;
extern gitem_t gI_weapon_doubleimpact;
extern gitem_t gI_weapon_vacuummaker;
extern gitem_t gI_weapon_energyvortex;
extern gitem_t gI_weapon_explosiveshotgun;
extern gitem_t gI_weapon_floatingminelauncher;
extern gitem_t gI_weapon_antimattercannon;
extern gitem_t gI_weapon_guidednuke;
extern gitem_t gI_weapon_nukegrenades; //n00k//

extern gitem_t gI_key_data_cd;
extern gitem_t gI_key_power_cube;
extern gitem_t gI_key_pyramid;
extern gitem_t gI_key_data_spinner;
extern gitem_t gI_key_pass;
extern gitem_t gI_key_blue_key;
extern gitem_t gI_key_red_key;
extern gitem_t gI_key_commander_head;
extern gitem_t gI_key_airstrike_target;
extern gitem_t gI_item_health;

// damage flags
#define DAMAGE_RADIUS			0x00000001	// damage was indirect
#define DAMAGE_NO_ARMOR			0x00000002	// armour does not protect from this damage
#define DAMAGE_ENERGY			0x00000004	// damage is from an energy based weapon
#define DAMAGE_NO_KNOCKBACK		0x00000008	// do not affect velocity, just view angles
#define DAMAGE_BULLET			0x00000010  // damage is from a bullet (used for ricochets)
#define DAMAGE_NO_PROTECTION	0x00000020  // armor, shields, invulnerability, and godmode have no effect

//ROGUE
#define DAMAGE_DESTROY_ARMOR	0x00000040	// damage is done to armor and health.
#define DAMAGE_NO_REG_ARMOR		0x00000080	// damage skips regular armor
#define DAMAGE_NO_POWER_ARMOR	0x00000100	// damage skips power armor
//ROGUE

#define DEFAULT_BULLET_HSPREAD	300
#define DEFAULT_BULLET_VSPREAD	500
#define DEFAULT_SHOTGUN_HSPREAD	1000
#define DEFAULT_SHOTGUN_VSPREAD	500
#define DEFAULT_DEATHMATCH_SHOTGUN_COUNT	12
#define DEFAULT_SHOTGUN_COUNT	12
#define DEFAULT_SSHOTGUN_COUNT	20


//============================================================================

// client_t->anim_priority
#define	ANIM_BASIC		0		// stand / run
#define	ANIM_WAVE		1
#define	ANIM_JUMP		2
#define	ANIM_PAIN		3
#define	ANIM_ATTACK		4
#define	ANIM_DEATH		5
#define	ANIM_REVERSE	6

//============================================================================


// client data that stays across multiple level loads
typedef struct client_persistent_s
{
	char		userinfo[MAX_INFO_STRING];
	char		netname[16];
	int			hand;

	qboolean	connected;			// a loadgame will leave valid entities that
									// just don't have a connection yet

	// values saved and restored from edicts when changing levels
	int			health;
	int			max_health;
	qboolean		powerArmorActive;

	int			selected_item;
	int			inventory[MAX_ITEMS];

	// ammo capacities
	int			max_bullets;
	int			max_shells;
	int			max_rockets;
	int			max_grenades;
	int			max_cells;
	int			max_slugs;

	gitem_t		*weapon;		// the current weapon
	gitem_t		*lastweapon;
	gitem_t		*lastweapons[10];
	gitem_t		*reallastweapon;

	int			power_cubes;	// used for tracking the cubes in coop games
	int			score;			// for calculating total unit score in coop games
	
	char zbotBuf[24];				// For Lithium ZBot detection
	// Don't add any fields before zbotBuf[], or the precompiled ZbotCheck.o
	// will get confused and start overwriting the wrong things.

	int			game_helpchanged;
	int			helpchanged;

	qboolean	spectator;			// client is a spectator

	int	scanner_active;	// bit 0 is on/off, bit 1 is just changed bit
	qboolean fire_mode; // Muce: 0 for FA and 1 for BF

	unsigned int ipAddr;		// Their IP address when they first connect

	int		special;

	// David Martin - LBIND variables
	char		ondeath[MAX_INFO_STRING];
	char		onrespawn[MAX_INFO_STRING];
	char		onenemydeath[MAX_INFO_STRING];
//	int			effects;	// used to hold values, for ent->s.effects during 
							// feature creation, set in game.
//	int			renderfx;	// used to hold values, for ent->s.renderfx during 
							// feature creation, set in game. 
	// David Martin - LBIND variables

} client_persistent_t;



// client data that stays across deathmatch respawns
typedef struct client_respawn_s
{
	client_persistent_t	coop_respawn;	// what to set client->pers to on a respawn
	int			enterframe;			// level.framenum the client entered the game
	float		entertime;			// level.time the client entered the game
	int			score;				// frags, etc
	int			deaths;				// how many times he's been killed
	vec3_t		cmd_angles;			// angles sent over in the last command
	int			game_helpchanged;
	int			helpchanged;

	qboolean	spectator;		// client is a spectator
	qboolean	voted;			// for elections
	int			votes_started;

//ZOID
	int			ctf_team;		// CTF team
	int			ctf_state;
	qboolean	id_state;
//ZOID
	
	// for zbotcheck
	short angles[2][2];
	int tog;
	int jitter;
	float jitter_time;
	float jitter_last;

	// HUD variables
	int		playeridactive;
	int		rangefinderactive;
	int		heightactive;
	int		fph_active;		//whether we HUD the FPH
	int		fph;			//player's FPH
	int		trackercountactive; // to display HUD tracker count
	int		turretcountactive; // to display HUD turret count
	int		locationactive;		// display current location and bearing
	int		inspect;		//trace to surface and display its texture name in HUD

} client_respawn_t;

// this structure is cleared on each PutClientInServer(),
// except for 'client->pers'
struct gclient_s
{
	// known to server
	player_state_t	ps;				// communicated by server to clients
	int				ping;

	// private to game
	client_persistent_t	pers;
	client_respawn_t	resp;
	pmove_state_t		old_pmove;	// for detecting out-of-pmove changes

	qboolean	showscores;			// set layout stat
	qboolean	showinventory;		// set layout stat
	qboolean	showhelp;
	qboolean	showhelpicon;

	int			ammo_index;

	int			buttons;
	int			oldbuttons;
	int			latched_buttons;

	qboolean	weapon_thunk;

	gitem_t		*newweapon;
	gitem_t		*last_touched_weapon;	//QW// only for extinction map development

	// sum up damage over an entire frame, so
	// shotgun blasts give a single big kick
	int			damage_armor;		// damage absorbed by armor
	int			damage_parmor;		// damage absorbed by power armor
	int			damage_blood;		// damage taken out of health
	int			damage_knockback;	// impact damage
	vec3_t		damage_from;		// origin for vector calculation

	float		killer_yaw;			// when dead, look at killer

	weaponstate_t	weaponstate;
	vec3_t		kick_angles;	// weapon kicks
	vec3_t		kick_origin;
	float		v_dmg_roll, v_dmg_pitch, v_dmg_time;	// damage kicks
	float		fall_time, fall_value;		// for view drop on fall
	float		damage_alpha;
	float		bonus_alpha;
	vec3_t		damage_blend;
	vec3_t		v_angle;			// aiming direction
	float		bobtime;			// so off-ground doesn't change it
	vec3_t		oldviewangles;
	vec3_t		oldvelocity;

	float		next_drown_time;
	int			old_waterlevel;
	int			breather_sound;

	int			machinegun_shots;	// for weapon raising


	// animation vars
	int			anim_end;
	int			anim_priority;
	qboolean	anim_duck;
	qboolean	anim_run;

	// powerup timers
	float		quad_framenum;
	float		invincible_framenum;
	float		breather_framenum;
	float		enviro_framenum;

	qboolean	grenade_blew_up;
	float		grenade_time;	//time between grenade tosses?
	int			silencer_shots;
	int			weapon_sound;

	float		pickup_msg_time;

	float		flood_locktill;		// locked from talking
	float		flood_when[10];		// when messages were said
	int			flood_whenhead;		// head pointer for when said

	float		respawn_time;		// can respawn when time > this

	int		hookstate;
	int		num_hooks;		//QW// deployed hook count
	edict_t	*hook;			//QW// for lithium laser hook

	int burstfire_count; // Muce: to keep track of BF

	/*ATTILA begin*/ 
	float Jet_framenum;		// burn out time when jet is activated
	float Jet_remaining;		// remaining fuel time
	float Jet_next_think;
	/*ATTILA end*/

	// dM
   int         dM_grenade;
   int         dM_ammoCost;
	// dM

	// JDB: new variable for lowlight vision 4/4/98
	qboolean lowlight;

	//SBOF: chasecam variables
	int chasetoggle;
	edict_t *chasecam;
	edict_t *oldplayer;

	/* WonderSlug --Added For Kamikaze Mode */ 
	qboolean kamikaze_mode;
	float kamikaze_framenum; 
	float kamikaze_timeleft; 
	/* WonderSlug End */

//ZOID
	qboolean	inmenu;				// in menu
	pmenuhnd_t	*menu;				// current menu
	edict_t		*chase_target;
	qboolean	update_chase;
//ZOID

	float		tripwire_debounce_time;
	edict_t     *lasttarget;
    edict_t     *missile;
	int			onturret;
	int         ammo2_index;
	edict_t		*iceball;

	// explosive
	edict_t		*explosive;
	int			expactive;

	// cloak 
	char		*modelbefore;
	int			cloaktime;
	float		cloak_time;
	qboolean	invisible;
	
	// flashlight
	edict_t		*flashlight;
	int			flashtype;
	
	// revenge
	edict_t		*killedby;

	// feign
	qboolean	feign;
	qboolean	raising_up;
	int			oldweaponindex;
	int			oldgunindex;

	// pointer to angel entity
	edict_t		*angel;
	

	float		fragpermin;
	// New armor
	int			organicarmor;
	int			slugarmor;
	int			cellarmor;
	int			shellarmor;
	int			bulletarmor;
	int			rocketarmor;
	int			grenadearmor;
	int			loxarmor;

	int			regenhealth;
	int			vampihealth;
	int			loxhealth;

	// anti camper stuff
	vec3_t		lastposition;
	float		lastpositiontime;
	int			warningtime;

	// sweeper stuff
	int			sweeperfiremode;

	// angel of horror
	float horrortime;

	float alpha;
	float alphatime;

    int			airstrike_called;
	vec3_t      airstrike_entry;
    float		airstrike_time;
	int			airstrike_grentype;

	// for cataclysm device limitation
	int		cats_used; // zero on new level, we count them per player here.

	// David Martin - If and when we decide to implement the POV feature for 
	// trackers and turrets these must be changed to be pointers to the actual 
	// trackers and turrets. Just an idea, but I feel that it will be needed.
	int			tracker;
	int			turret;

	int baton; // 1=ON, 0=OFF 

//ROGUE disruptor
	float		tracker_pain_framenum;

	int			scannerx;
	int			scannery;
	edict_t		*holo;
	int			chamber; // 1=ON, 0=OFF 
	edict_t		*thechamber;

	// David Martin - LBIND variables
	/* David Martin - Moved to client_persistent_t - 07-07-05
	char		*ondeath;
	char		*onrespawn;
	char		*onenemydeath;
	int			effects;	// used to hold values, for ent->s.effects during 
							// feature creation, set in game.
	int			renderfx;	// used to hold values, for ent->s.renderfx during 
							// feature creation, set in game. 
	*/
	// David Martin - LBIND variables

// PM:  New fields
    float		flash_alpha;
    float		flash_fade;
// PM
	float		curr_heat;
	int			already_updated;
	int			next_update;
	int			heat_active;
	int			mine;
	int			mine_count;
	int			guidedMissileFired;
	int			lasertrips;		//QW// the laser trip count
	qboolean	sweeperactive;
	pmenu_t		*devmenu;		// this is a pointer to the menu
								// the menu will be allocated in PMenu_Open
	int			scoreboardnum;
	int			leader;
};	

struct edict_s
{
	entity_state_t	s;
	struct gclient_s	*client;	// NULL if not a player
									// the server expects the first part
									// of gclient_s to be a player_state_t
									// but the rest of it is opaque
	qboolean	inuse;
	int			linkcount;

	// FIXME: move these fields to a server private sv_entity_t
	link_t		area;				// linked to a division node or leaf
	
	int			num_clusters;		// if -1, use headnode instead
	int			clusternums[MAX_ENT_CLUSTERS];
	int			headnode;			// unused if num_clusters != -1
	int			areanum, areanum2;

	//================================

	int			svflags;
	vec3_t		mins, maxs;
	vec3_t		absmin, absmax, size;
	solid_t		solid;
	int			clipmask;
	edict_t		*owner;
	

	// DO NOT MODIFY ANYTHING ABOVE THIS, THE SERVER
	// EXPECTS THE FIELDS IN THAT ORDER!

	//================================
	edict_t		*other;
	int			movetype;
	int			flags;

	char		*model;
	float		freetime;			// sv.time when the object was freed
	
	//
	// only used locally in game, not by server
	//
	char		*message;
	char		*classname;	// the name of the entity class
	int			classnum;	// the number of the entity class to use instead of strings
	int			spawnflags;

	float		timestamp;

	float		angle;			// set in qe3, -1 = up, -2 = down
	char		*target;
	char		*targetname;
	char		*killtarget;
	char		*team;
	char		*pathtarget;
	char		*deathtarget;
	char		*combattarget;
	edict_t		*target_ent;

	float		speed, accel, decel;
	vec3_t		movedir;
	vec3_t		pos1, pos2;

	vec3_t		velocity;
	vec3_t		avelocity;
	float		mass;
	float		air_finished;
	float		gravity;		// per entity gravity multiplier (1.0 is normal)
								// use for lowgrav artifact, e.g., flares

	edict_t		*goalentity;
	edict_t		*movetarget;
	float		yaw_speed;
	float		ideal_yaw;

	float		nextthink;
	float		nextthink4;		// from Paril's wall hider detection
	void		(*think4)(edict_t *self);

	void		(*prethink) (edict_t *ent);
	void		(*think)(edict_t *self);
	void		(*blocked)(edict_t *self, edict_t *other);	//move to moveinfo?
	void		(*touch)(edict_t *self, edict_t *other, cplane_t *plane, csurface_t *surf);
	void		(*use)(edict_t *self, edict_t *other, edict_t *activator);
	void		(*pain)(edict_t *self, edict_t *other, float kick, int damage);
	void		(*die)(edict_t *self, edict_t *inflictor, edict_t *attacker, int damage, vec3_t point);


	float		touch_debounce_time;		// are all these legit?  do we need more/less of them?
	float		pain_debounce_time;
	float		damage_debounce_time;
	float		fly_sound_debounce_time;	//move to clientinfo
	float		last_move_time;

	int			health;
	int			max_health;
	int			gib_health;
	int			deadflag;
	float		show_hostile;

	float		powerarmor_time;

	char		*map;			// target_changelevel

	int			viewheight;		// height above origin where eyesight is determined
	int			takedamage;
	int			dmg;		// damage inflicted
	int			radius_dmg;
	float		dmg_radius;
	int			dm_type;		// darKMajick grenade type
	int			sounds;			//make this a spawntemp var?
	int			count;

	edict_t		*chain;
	edict_t		*enemy;
	edict_t		*oldenemy;
	edict_t		*activator;
	edict_t		*groundentity;
	int			groundentity_linkcount;
	edict_t		*teamchain;
	edict_t		*teammaster;

	edict_t		*mynoise;		// can go in client only
	edict_t		*mynoise2;

	int			noise_index;
	int			noise_index2;
	float		volume;
	float		attenuation;

	// timing variables
	float		wait;
	float		delay;			// before firing targets
	float		random;

	float		teleport_time;

	int			watertype;
	int			waterlevel;

	vec3_t		move_origin;
	vec3_t		move_angles;

	// move this to clientinfo?
	int			light_level;

	int			style;			// also used as areaportal number

	gitem_t		*item;			// for bonus items

	// common data blocks
	moveinfo_t		moveinfo;
	monsterinfo_t	monsterinfo;

	// *************************
	// freezing data
	int frozen;
	int unfrozen;
	float	frozentime;
	// ************************

	//ThirdPerson Perspective Stuff
	int		thirdperson;	// To tell the server if that person is in 3rd person mode
	edict_t	*clone;			// This is what the player sees
	int		currentweapon;	// This is for the weapon model info when in 3rd person mode
	float		thirdoffx;		// So the player can move the camera around
	float		thirdoffz;

	 edict_t *decoy;
	 edict_t *creator;

	 //SBOF: Chasecam variables
	int chasedist1;
	int chasedist2;

	edict_t		*hook_target;
	edict_t		*laser;		//QW// lithium laser hook

	float       burnout;
	edict_t     *burner;

	edict_t		*lasersight;

	qboolean	oldmodel;	
	qboolean	cloaked;

	// flaregun
	float		lastheight;
	
	// Stick Grenade
	edict_t		*stuckentity;
	vec3_t		oldstuckorigin;
	vec3_t		oldstuckangles;

	// Icecube Gun
    void		(*reallythink)(edict_t *self);
    float		reallynextthink;
	int			fireflags;

	int			plaguetime;
	int			plaguedamagecount;
	int			plaguedamage;
	edict_t		*damagecauser;

	edict_t		*master;

	// Tracker info
	int			trackerammo;
	char		*trackerammotype;
	float		trackerstarttime;

	// Turret info
	int			turretammo;
	char		*turretammotype;
	float		turretstarttime;

	int			decoytime;
	float		spamtimer;
	int			holo;

	int			SonicDamage;	//Will: For the Sonic Rail

	//	Floating grenade code
	int			fireCounter;
	float		floatTime;
	float		floatRiseTime;
	float		floatRiseRate;
	void		(*floatthink)(edict_t *self);
	qboolean    canFloat;

	int      packitems[MAX_ITEMS]; // New for Backpack (Maj.Bitch)
};


//============================================================================
// Here we have a fine collection of prototypes that probably belong elsewhere
//============================================================================
//
// g_ent.c
//
// Phlem - new for off-world teleport and entity loading
char *LoadEntFile(char *mapname, char *entities); 
// Phlem - end new for off-world teleport and entity loading

//
// g_cmds.c
//
void Cmd_Help_f (edict_t *ent);
void Cmd_Score_f (edict_t *ent);

//
// g_items.c
//
#define	ITEM_INDEX(x) ((x)->item_index)

void PrecacheItem (gitem_t *it);
void InitItems (void);
void SetItemNames (void);
gitem_t	*FindItem (char *pickup_name);
gitem_t	*FindItemByClassname (char *classname);
edict_t *Drop_Item (edict_t *ent, gitem_t *item);
void SetRespawn (edict_t *ent, float delay);
void SpawnItem (edict_t *ent, gitem_t *item);
int ArmorIndex (edict_t *ent);
int PowerArmorType (edict_t *ent);
gitem_t	*GetItemByIndex (int index);
qboolean Add_Ammo (edict_t *ent, gitem_t *item, int count);
void Touch_Item (edict_t *ent, edict_t *other, cplane_t *plane, csurface_t *surf);
void Drop_Temp_Touch (edict_t *ent, edict_t *other, cplane_t *plane, csurface_t *surf);

//
// s_cam.c
//
void ChasecamRemove(edict_t *ent);

//
// p_weapon.c
//
void	ChangeWeapon (edict_t *ent);
void	Think_Weapon (edict_t *ent);
void	Use_Weapon (edict_t *ent, gitem_t *inv);
void	Drop_Weapon (edict_t *ent, gitem_t *inv);
// ### Hentai ### BEGIN 
void	ShowGun(edict_t *ent);
// ###	Hentai ### END

//
// g_spawn.c
//
void SpawnEntities (char *mapname, char *entities, char *spawnpoint);
void CreateStatusBar(void);

//
// g_utils.c
//
qboolean	KillBox (edict_t *ent);
void	G_ProjectSource (vec3_t point, vec3_t distance, vec3_t forward, vec3_t right, vec3_t result);
edict_t *G_Find (edict_t *from, int fieldofs, char *match);
edict_t *findradius (edict_t *from, vec3_t org, float rad);
edict_t *G_PickTarget (char *targetname);
void	G_UseTargets (edict_t *ent, edict_t *activator);
void	G_SetMovedir (vec3_t angles, vec3_t movedir);

void	G_InitEdict (edict_t *e);
edict_t	*G_Spawn (void);
void	G_FreeEdict (edict_t *e);

void	G_TouchTriggers (edict_t *ent);
void	G_TouchSolids (edict_t *ent);

char	*G_CopyString (char *in);
void StuffCmd(edict_t *ent, char *text);

float	*tv (float x, float y, float z);
char	*vtos (vec3_t v);

float vectoyaw (vec3_t vec);
void vectoangles (vec3_t vec, vec3_t angles);

//
// g_combat.c
//
qboolean OnSameTeam (edict_t *ent1, edict_t *ent2);
qboolean CanDamage (edict_t *targ, edict_t *inflictor);
qboolean CheckTeamDamage (edict_t *targ, edict_t *attacker);
void T_Damage (edict_t *targ, edict_t *inflictor, edict_t *attacker, vec3_t dir, vec3_t point, vec3_t normal, int damage, int knockback, int dflags, int mod);
void T_RadiusDamage (edict_t *inflictor, edict_t *attacker, float damage, edict_t *ignore, float radius, int mod);
void T_FreezatronRadiusDamage (edict_t *inflictor, edict_t *attacker, float damage, edict_t *ignore, float radius, int mod); // Added by TheLopez
void T_WaterRadiusDamage (edict_t *inflictor, edict_t *attacker, float damage, edict_t *ignore, float radius, int mod);

//
// g_monster.c
//
void monster_fire_bullet (edict_t *self, vec3_t start, vec3_t dir, int damage, int kick, int hspread, int vspread, int flashtype);
void monster_fire_shotgun (edict_t *self, vec3_t start, vec3_t aimdir, int damage, int kick, int hspread, int vspread, int count, int flashtype);
void monster_fire_blaster (edict_t *self, vec3_t start, vec3_t dir, int damage, int speed, int flashtype, int effect);
void monster_fire_grenade (edict_t *self, vec3_t start, vec3_t aimdir, int damage, int speed, int flashtype);
void monster_fire_rocket (edict_t *self, vec3_t start, vec3_t dir, int damage, int speed, int flashtype);
void monster_fire_railgun (edict_t *self, vec3_t start, vec3_t aimdir, int damage, int kick, int flashtype);
void monster_fire_bfg (edict_t *self, vec3_t start, vec3_t aimdir, int damage, int speed, int kick, float damage_radius, int flashtype);
void M_droptofloor (edict_t *ent);
void monster_think (edict_t *self);
void walkmonster_start (edict_t *self);
void swimmonster_start (edict_t *self);
void flymonster_start (edict_t *self);
void AttackFinished (edict_t *self, float time);
void monster_death_use (edict_t *self);
void M_CatagorizePosition (edict_t *ent);
qboolean M_CheckAttack (edict_t *self);
void M_FliesOff(edict_t* self);
void M_FliesOn(edict_t* self);
void M_FlyCheck (edict_t *self);
void M_CheckGround (edict_t *ent);

//
// g_misc.c
//
void ThrowHead (edict_t *self, char *gibname, int damage, int type);
void ThrowClientHead (edict_t *self, int damage);
void ThrowGib (edict_t *self, char *gibname, int damage, int type);
void BecomeExplosion1(edict_t *self);
void BecomeExplosion2 (edict_t *self);
void misc_die (edict_t *self, edict_t *inflictor, edict_t *attacker, int damage, vec3_t point);

//
// highscorer.c
//
void HighScorer(void);

//
// jetpack.c
//
qboolean Jet_Active (edict_t *ent);
void Jet_BecomeExplosion (edict_t *ent, int damage);
void Jet_ApplyJet (edict_t *ent, usercmd_t *ucmd);
qboolean Jet_AvoidGround (edict_t *ent);

//
// g_ai.c
//
void AI_SetSightClient (void);
void ai_stand (edict_t *self, float dist);
void ai_move (edict_t *self, float dist);
void ai_walk (edict_t *self, float dist);
void ai_turn (edict_t *self, float dist);
void ai_run (edict_t *self, float dist);
void ai_charge (edict_t *self, float dist);
qboolean ai_checkattack (edict_t *self);
int range (edict_t *self, edict_t *other);
qboolean FindTarget (edict_t *self);
void FoundTarget (edict_t *self);
qboolean infront (edict_t *self, edict_t *other);
qboolean visible (edict_t *self, edict_t *other);
qboolean FacingIdeal(edict_t *self);

//
// laser.c
//
#define LASER_TIME 30
#define CELLS_FOR_LASER 20
#define LASER_DAMAGE 100
#define LASER_MOUNT_DAMAGE 50
#define LASER_MOUNT_DAMAGE_RADIUS 64

void target_laser_use (edict_t *self, edict_t *other, edict_t *activator);
void target_laser_think (edict_t *self);
void target_laser_on (edict_t *self);
void target_laser_off (edict_t *self);

//
// lasertrip.c
//
void PlaceLaserTripwire (edict_t *ent);

//
// g_ptrail.c
//
void PlayerTrail_Init (void);
void PlayerTrail_Add (vec3_t spot);
void PlayerTrail_New (vec3_t spot);
edict_t *PlayerTrail_PickFirst (edict_t *self);
edict_t *PlayerTrail_PickNext (edict_t *self);
edict_t	*PlayerTrail_LastSpot (void);

//
// defined in p_client.c
//
void respawn (edict_t *ent);
void SelectSpawnPoint (edict_t *ent, vec3_t origin, vec3_t angles);
void PutClientInServer (edict_t *ent);
void InitClientPersistent (gclient_t *client);
void InitClientResp (gclient_t *client);
void InitBodyQue (void);
void ClientBeginServerFrame (edict_t *ent);
void TossClientWeapon(edict_t *ent);

//
// g_player.c
//
void player_pain (edict_t *self, edict_t *other, float kick, int damage);
void player_die (edict_t *self, edict_t *inflictor, edict_t *attacker, int damage, vec3_t point);

//
// g_svcmds.c
//
typedef struct ipfilter_s
{
	unsigned	mask;
	unsigned	compare;
} ipfilter_t;

qboolean StringToFilter (char *s, ipfilter_t *f);
void	ServerCommand (void);
qboolean SV_FilterPacket (char *from);

//
// g_save.c
//
void UpdateBans(void);

//
// p_view.c
//
void ClientEndServerFrame (edict_t *ent);
void ThirdBegin (edict_t *ent);
void ThirdEnd (edict_t *ent);

//
// p_hud.c
//
void BeginIntermission (edict_t *targ);
void MoveClientToIntermission (edict_t *client);
void G_SetStats (edict_t *ent);
void ValidateSelectedItem (edict_t *ent);
void DM_CreateScoreboard(edict_t *ent, edict_t *killer, char *string);
void A_ScoreboardMessage (edict_t *ent, edict_t *killer, char *string);
void DeathmatchScoreboardMessage (edict_t *client, edict_t *killer);
void DeathmatchScoreboard (edict_t *ent);
void ShowHeatbar (edict_t* ent, char* string) ;

//
// p_hook.c
//
void Cmd_Hook_f (edict_t *ent);
void DropHook (edict_t *ent);
void HookGiveBinds_f (edict_t *ent);

//
// p_weapon.c
//
void P_ProjectSource (gclient_t *client, vec3_t point, vec3_t distance, vec3_t forward, vec3_t right, vec3_t result);
void PlayerNoise(edict_t *who, vec3_t where, int type);
void Weapon_Generic (edict_t *ent, int FRAME_ACTIVATE_LAST, int FRAME_FIRE_LAST, int FRAME_IDLE_LAST, int FRAME_DEACTIVATE_LAST, int *pause_frames, int *fire_frames, void (*fire)(edict_t *ent));
void weapon_grenadelauncher_fire (edict_t *ent);
void Weapon_Recoil(edict_t* self, float recoil);

//
// m_move.c
//
qboolean M_CheckBottom (edict_t *ent);
qboolean M_walkmove (edict_t *ent, float yaw, float dist);
void M_MoveToGoal (edict_t *ent, float dist);
void M_ChangeYaw (edict_t *ent);
qboolean SV_CloseEnough (edict_t *ent, edict_t *goal, float dist);

//
// g_phys.c
//
void G_RunEntity (edict_t *ent);

//
// g_main.c
//
void SaveClientData (void);
void FetchClientEntData (edict_t *ent);
void EndDMLevel (void);
int CountConnectedClients (void);

//
// g_func.c
//
void train_use (edict_t *self, edict_t *other, edict_t *activator);
void func_train_find (edict_t *self);
void train_use (edict_t *self, edict_t *other, edict_t *activator);
void func_train_find (edict_t *self);

//
// g_chase.c
//
void UpdateChaseCam(edict_t *ent);
void ChaseNext(edict_t *ent);
void ChasePrev(edict_t *ent);
void GetChaseTarget(edict_t *ent);

//
// wf_decoy.c
//
void Toggle_Decoy_f (edict_t *self);
void free_decoy (edict_t *self);

extern void CheckChasecam_Viewent(edict_t *ent);

void LaserSightThink (edict_t *self);
void SP_LaserSight(edict_t *self);

// g_weapon.c replacements
void Grenade_Explode (edict_t *ent);
void rocket_touch (edict_t *ent, edict_t *other, cplane_t *plane, csurface_t *surf);

// g_misc.c replacements
void barrel_explode (edict_t *self);

// g_target.c replacements
void target_explosion_explode (edict_t *self);
void target_earthquake_think (edict_t *self);
void target_earthquake_use (edict_t *self, edict_t *other, edict_t *activator);


// new functions
void T_ShockWave (edict_t *inflictor, float damage, float radius);
void T_ShockItems (edict_t *inflictor);
void ThrowShrapnel (edict_t *self, char *modelname, float speed, vec3_t origin);
void ThrowShrapnel2 (edict_t *self, char *modelname, float speed, vec3_t origin);
void ThrowShrapnel3 (edict_t *self, char *modelname, float speed, vec3_t origin);
void ThrowShrapnel4 (edict_t *self, char *modelname, float speed, vec3_t origin);
void make_debris (edict_t *ent);
void BigBang (edict_t *ent);
qboolean isvisible (edict_t *self, edict_t *other);


qboolean Pickup_Health (edict_t *ent, edict_t *other);
qboolean Pickup_Weapon (edict_t *ent, edict_t *other);
qboolean Pickup_Powerup (edict_t *ent, edict_t *other);
qboolean Pickup_Adrenaline (edict_t *ent, edict_t *other);
qboolean Pickup_AncientHead (edict_t *ent, edict_t *other);
qboolean Pickup_Bandolier (edict_t *ent, edict_t *other);
qboolean Pickup_Pack (edict_t *ent, edict_t *other);
qboolean Pickup_Key (edict_t *ent, edict_t *other);
qboolean Pickup_Ammo (edict_t *ent, edict_t *other);
qboolean Pickup_Armor (edict_t *ent, edict_t *other);
qboolean ZbotCheck(edict_t* ent, usercmd_t* ucmd);
void SpawnDamage (int type, vec3_t origin, vec3_t normal, int damage);
void NoAmmoWeaponChange (edict_t *ent);
void Cmd_Kill_f(edict_t *ent);
void Cmd_PlayerID(edict_t *ent);
void Cmd_Rangefinder(edict_t *ent);
void Cmd_Height(edict_t *ent);
void SV_AddBlend (float r, float g, float b, float a, float *v_blend);
void Cmd_FeignDeath(edict_t *self);

// CCH: new prototype for function called when airstrike arrives
void Think_Airstrike (edict_t *ent);
void Cmd_LogPlayers_f(edict_t *ent);
void P_ProjectSource_Reverse (gclient_t *client, vec3_t point, vec3_t distance, vec3_t forward, vec3_t right, vec3_t result);
qboolean G_ClientInGame(edict_t *ent);
void Fire_Baton(edict_t *ent);
qboolean G_EntExists(edict_t *ent);
void weapon_durg_fire (edict_t *ent);
void Grenade_Touch (edict_t *ent, edict_t *other, cplane_t *plane, csurface_t *surf);
void vectoangles2 (vec3_t value1, vec3_t angles);

// defined in g_cmds.c
void Cmd_Time_f(edict_t *ent);
void Cmd_TimeRemaining_f(edict_t *ent);
void Cmd_TimeAll_f(edict_t *ent);
void Log_Time (void);
void Cmd_SkinList_f(edict_t *ent);

// defined in l_daynight.c
void DayNightCycle(void);
void InitDayNightVars(void);

// defined in l_matchplay.c
void MatchInit(void);
void MatchplayDoCountdown (void);
void MatchplaySpawnEntities (char *mapname, char *entities, char *spawnpoint);
qboolean MatchplayCheckRound1 (void);
qboolean MatchplayCheckRound2 (void);
void MatchplayStartRound2 (void);
void MatchplayMakeObserver (edict_t *ent);
void DoMatchCountdown(edict_t *ent);



