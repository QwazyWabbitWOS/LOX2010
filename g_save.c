
#include "g_local.h"
#include "g_team.h"

#include "l_banner.h"
#include "maplist.h"
#include "l_voting.h"
#include "l_backpack.h"
#include "l_ar_admin.h"

field_t fields[] = 
{
	{"classname", FOFS(classname), F_LSTRING},
	{"origin", FOFS(s.origin), F_VECTOR},
	{"model", FOFS(model), F_LSTRING},
	{"spawnflags", FOFS(spawnflags), F_INT},
	{"speed", FOFS(speed), F_FLOAT},
	{"accel", FOFS(accel), F_FLOAT},
	{"decel", FOFS(decel), F_FLOAT},
	{"target", FOFS(target), F_LSTRING},
	{"targetname", FOFS(targetname), F_LSTRING},
	{"pathtarget", FOFS(pathtarget), F_LSTRING},
	{"deathtarget", FOFS(deathtarget), F_LSTRING},
	{"killtarget", FOFS(killtarget), F_LSTRING},
	{"combattarget", FOFS(combattarget), F_LSTRING},
	{"message", FOFS(message), F_LSTRING},
	{"team", FOFS(team), F_LSTRING},
	{"wait", FOFS(wait), F_FLOAT},
	{"delay", FOFS(delay), F_FLOAT},
	{"random", FOFS(random), F_FLOAT},
	{"move_origin", FOFS(move_origin), F_VECTOR},
	{"move_angles", FOFS(move_angles), F_VECTOR},
	{"style", FOFS(style), F_INT},
	{"count", FOFS(count), F_INT},
	{"health", FOFS(health), F_INT},
	{"sounds", FOFS(sounds), F_INT},
	{"light", 0, F_IGNORE},
	{"dmg", FOFS(dmg), F_INT},
	{"angles", FOFS(s.angles), F_VECTOR},
	{"angle", FOFS(s.angles), F_ANGLEHACK},
	{"mass", FOFS(mass), F_INT},
	{"volume", FOFS(volume), F_FLOAT},
	{"attenuation", FOFS(attenuation), F_FLOAT},
	{"map", FOFS(map), F_LSTRING},

	// Added for WoD, for the new style of func_killbox
	{"mins", FOFS(mins), F_VECTOR},
	{"maxs", FOFS(maxs), F_VECTOR},

	// temp spawn vars -- only valid when the spawn function is called
	{"lip", STOFS(lip), F_INT, FFL_SPAWNTEMP},
	{"distance", STOFS(distance), F_INT, FFL_SPAWNTEMP},
	{"height", STOFS(height), F_INT, FFL_SPAWNTEMP},
	{"noise", STOFS(noise), F_LSTRING, FFL_SPAWNTEMP},
	{"pausetime", STOFS(pausetime), F_FLOAT, FFL_SPAWNTEMP},
	{"item", STOFS(item), F_LSTRING, FFL_SPAWNTEMP},
	{"gravity", STOFS(gravity), F_LSTRING, FFL_SPAWNTEMP},
	{"sky", STOFS(sky), F_LSTRING, FFL_SPAWNTEMP},
	{"skyrotate", STOFS(skyrotate), F_FLOAT, FFL_SPAWNTEMP},
	{"skyaxis", STOFS(skyaxis), F_VECTOR, FFL_SPAWNTEMP},
	{"minyaw", STOFS(minyaw), F_FLOAT, FFL_SPAWNTEMP},
	{"maxyaw", STOFS(maxyaw), F_FLOAT, FFL_SPAWNTEMP},
	{"minpitch", STOFS(minpitch), F_FLOAT, FFL_SPAWNTEMP},
	{"maxpitch", STOFS(maxpitch), F_FLOAT, FFL_SPAWNTEMP},
	{"nextmap", STOFS(nextmap), F_LSTRING, FFL_SPAWNTEMP},

	{NULL, 0, F_INT}
};

// -------- just for savegames ----------
// all pointer fields should be listed here, or savegames
// won't work properly (they will crash and burn).
// this wasn't just tacked on to the fields array, because
// these don't need names, we wouldn't want map fields using
// some of these, and if one were accidentally present twice
// it would double swizzle the pointer.

field_t		savefields[] =
{
	{"", FOFS(classname), F_LSTRING},
	{"", FOFS(target), F_LSTRING},
	{"", FOFS(targetname), F_LSTRING},
	{"", FOFS(killtarget), F_LSTRING},
	{"", FOFS(team), F_LSTRING},
	{"", FOFS(pathtarget), F_LSTRING},
	{"", FOFS(deathtarget), F_LSTRING},
	{"", FOFS(combattarget), F_LSTRING},
	{"", FOFS(model), F_LSTRING},
	{"", FOFS(map), F_LSTRING},
	{"", FOFS(message), F_LSTRING},

	{"", FOFS(client), F_CLIENT},
	{"", FOFS(item), F_ITEM},

	{"", FOFS(goalentity), F_EDICT},
	{"", FOFS(movetarget), F_EDICT},
	{"", FOFS(enemy), F_EDICT},
	{"", FOFS(oldenemy), F_EDICT},
	{"", FOFS(activator), F_EDICT},
	{"", FOFS(groundentity), F_EDICT},
	{"", FOFS(teamchain), F_EDICT},
	{"", FOFS(teammaster), F_EDICT},
	{"", FOFS(owner), F_EDICT},
	{"", FOFS(mynoise), F_EDICT},
	{"", FOFS(mynoise2), F_EDICT},
	{"", FOFS(target_ent), F_EDICT},
	{"", FOFS(chain), F_EDICT},

	{NULL, 0, F_INT}
};

field_t		levelfields[] =
{
	{"", LLOFS(changemap), F_LSTRING},

	{"", LLOFS(sight_client), F_EDICT},
	{"", LLOFS(sight_entity), F_EDICT},
	{"", LLOFS(sound_entity), F_EDICT},
	{"", LLOFS(sound2_entity), F_EDICT},

	{NULL, 0, F_INT}
};

field_t		clientfields[] =
{
	{"", CLOFS(pers.weapon), F_ITEM},
	{"", CLOFS(pers.lastweapon), F_ITEM},
	{"", CLOFS(newweapon), F_ITEM},

	{NULL, 0, F_INT}
};

void SetUpHeatBar(void)
{
	game.heat_bar[0]  = "\200\203\201\201\201\201\201\201\201\201\201\201\201\201\201\202";
	game.heat_bar[1]  = "\200\201\203\201\201\201\201\201\201\201\201\201\201\201\201\202";
	game.heat_bar[2]  = "\200\201\201\203\201\201\201\201\201\201\201\201\201\201\201\202";
	game.heat_bar[3]  = "\200\201\201\201\203\201\201\201\201\201\201\201\201\201\201\202";
	game.heat_bar[4]  = "\200\201\201\201\201\203\201\201\201\201\201\201\201\201\201\202";
	game.heat_bar[5]  = "\200\201\201\201\201\201\203\201\201\201\201\201\201\201\201\202";
	game.heat_bar[6]  = "\200\201\201\201\201\201\201\203\201\201\201\201\201\201\201\202";
	game.heat_bar[7]  = "\200\201\201\201\201\201\201\201\203\201\201\201\201\201\201\202";
	game.heat_bar[8]  = "\200\201\201\201\201\201\201\201\201\203\201\201\201\201\201\202";
	game.heat_bar[9]  = "\200\201\201\201\201\201\201\201\201\201\203\201\201\201\201\202";
	game.heat_bar[10] = "\200\201\201\201\201\201\201\201\201\201\201\203\201\201\201\202";
	game.heat_bar[11] = "\200\201\201\201\201\201\201\201\201\201\201\201\203\201\201\202";
	game.heat_bar[12] = "\200\201\201\201\201\201\201\201\201\201\201\201\201\203\201\202";
	game.heat_bar[13] = "\200\201\201\201\201\201\201\201\201\201\201\201\201\201\203\202";
	game.curr_heat_bar = game.heat_bar[0];
	game.curr_heat_bar_counter = 0;
}

int initialization;	// initialization in progress flag

/*
============
InitGame

This will be called when the dll is first loaded, which
only happens when a new game is started or a save game
is loaded.
============
*/

cvar_t *version;

void InitGame (void)
{

#ifdef	_WIN32
	_CrtMemCheckpoint(&startup1);
#endif

	gi.dprintf ("==== InitGame (LOX "VERSION" "__DATE__") ====\n");
	gi.dprintf (loxbanner);
	version = gi.cvar ("version", "", CVAR_NOSET);
	if (version)
		gi.dprintf ("LOX detected engine: %s\n",version->string);
	Log_Time ();

	srand(time(NULL)); //seed the rng

	game.hudnames[0] = "WOD";
	game.hudnames[1] = "WOD:LOX1";
	game.hudnames[2] = "WOD:LOX2";
	game.hudnames[3] = "BLANK";
	game.hudnames[4] = "PLAYER";
	gun_x = gi.cvar ("gun_x", "0", 0);
	gun_y = gi.cvar ("gun_y", "0", 0);
	gun_z = gi.cvar ("gun_z", "0", 0);

	rollspeed = gi.cvar ("rollspeed", "200", 0);
	rollangle = gi.cvar ("rollangle", "2", 0);
	sv_maxvelocity = gi.cvar ("sv_maxvelocity", "2500", 0);
	sv_gravity = gi.cvar ("gravity", "800", 0);

	// noset vars  (set from command line or server config file)
	dedicated = gi.cvar ("dedicated", "0", CVAR_NOSET);
	gamedir = gi.cvar ("gamedir", "", CVAR_NOSET);

	// latched vars (changed when game next restarts)

	sv_cheats = gi.cvar ("cheats", "0", CVAR_SERVERINFO | CVAR_LATCH);
	gi.cvar ("gamename", GAMEVERSION , CVAR_SERVERINFO | CVAR_LATCH);
	gi.cvar ("gamedate", __DATE__ , CVAR_SERVERINFO | CVAR_LATCH);

	maxclients = gi.cvar ("maxclients", "12", CVAR_SERVERINFO | CVAR_LATCH);
	maxspectators = gi.cvar ("maxspectators", "12", CVAR_SERVERINFO);
	deathmatch = gi.cvar ("deathmatch", "0", CVAR_LATCH);
	ctf = gi.cvar ("ctf", "0", CVAR_LATCH);
	coop = gi.cvar ("coop", "0", CVAR_LATCH);
	skill = gi.cvar ("skill", "1", CVAR_LATCH);
	noscore = gi.cvar ("noscore", "0", CVAR_LATCH); // for the new scoreboard logic
	maxentities = gi.cvar ("maxentities", "1024", CVAR_LATCH);
	custom_ents = gi.cvar ("custom_ents", "0", 0);		//QW// allow custom ents in DM
	ext_devt = gi.cvar ("ext_devt", "0", CVAR_LATCH);	//QW// set this for extinction development
	staticlights = gi.cvar ("staticlights", "0", CVAR_LATCH);	//QW// 1 = no dynamic map lights.

	if (coop->value)
		gi.cvar_forceset ("gamename", GAMEVERSION_C);	// show coop mode version string

	motdfile = gi.cvar ("motdfile", "motd.txt", 0);

	// change anytime vars
	dmflags = gi.cvar ("dmflags", "0", CVAR_SERVERINFO);
	fraglimit = gi.cvar ("fraglimit", "0", CVAR_SERVERINFO);
	timelimit = gi.cvar ("timelimit", "0", CVAR_SERVERINFO);
	password = gi.cvar ("password", "", CVAR_USERINFO);
	spectator_password = gi.cvar ("spectator_password", "", CVAR_USERINFO);
	needpass = gi.cvar ("needpass", "0", CVAR_SERVERINFO);
	filterban = gi.cvar ("filterban", "1", 0);
	scoreboardtime = gi.cvar ("scoreboardtime", "15", 0);

	g_select_empty = gi.cvar ("g_select_empty", "0", CVAR_ARCHIVE);

	run_pitch = gi.cvar ("run_pitch", "0.002", 0);
	run_roll = gi.cvar ("run_roll", "0.005", 0);
	bob_up  = gi.cvar ("bob_up", "0.005", 0);
	bob_pitch = gi.cvar ("bob_pitch", "0.002", 0);
	bob_roll = gi.cvar ("bob_roll", "0.002", 0);

	debugmodels = gi.cvar ("debugmodels", "0", 0); //QW// server side runtime fix for debug models
	max_trackers = gi.cvar ("maxtrackers", "3", 0); //QW//
	max_turrets = gi.cvar ("maxturrets", "3", 0); //TheLopez//
	max_lasertrips = gi.cvar ("maxlasertrips", "4", 0); //QW//
	max_floatingmines = gi.cvar ("maxfloatingmines", "7", 0); //QW//
	kami_frozen = gi.cvar ("kamifrozen", "0", 0); // allow kami if frozen
	homerseekowner = gi.cvar ("homerseekowner", "1", 0);	//QW// homing rockets seek owners
	newscoreboard = gi.cvar ("newscoreboard", "0", 0);	//QW// use the new DM Scoreboard
	g_rockets_takedamage = gi.cvar ("g_rockets_takedamage", "0", 0);	//QW// admin option, guided rocket damage
	g_nukes_takedamage = gi.cvar ("g_nukes_takedamage", "0", 0);	//QW// admin option, guided nuke damage
	push_kill_award = gi.cvar ("push_kill_award", "0", 0);	//QW// set to number of points awarded for pushkill
	push_kill_delay = gi.cvar ("push_kill_delay", "3.0", 0);	//QW// window from push to death
	autoweaponselect = gi.cvar ("autoweaponselect", "1", 0);	//QW// for coop mode
	fast_weapon_change = gi.cvar ("fast_weapon_change", "0", 0);	//QW// for fast weapon change mode
	max_cats = gi.cvar ("max_cats", "0", 0);	//QW// for max cats per player per level
	zbot_check = gi.cvar ("zbot_check", "0", 0);	//QW// enable/disable internal zbot checking

	//cvars pertaining to stats logs
	logstats = gi.cvar ("logstats", "1", 0);		//QW// allow logging player statistics
	logarstats = gi.cvar ("logarstats", "0", 0);	//QW// allow logging Altered Realm player statistics
	statsfile = gi.cvar ("statsfile", "stats.log", 0); //QW// the name of the AR stats file
	statsfile_rename = gi.cvar ("statsfile_rename", "1", 0); //QW// 0 = never, 1 = daily, 2 = weekly, 3 = monthly
	logfile_name = gi.cvar ("logfile_name", "server.log", 0); //QW// the name of the server log file (clients use qconsole.log)
	logfile_rename = gi.cvar ("logfile_rename", "2", 0); //QW// 0 = never, 1 = daily, 2 = weekly, 3 = monthly

	// the DM respawn invulnerability (defaults to 5 seconds)
	respawn_protect = gi.cvar("respawn_protect", "5", 0);	//QW// allow admin to control respawn invincibility time

	campertime = gi.cvar("campertime", "20", 0);		//QW// camping time limit 
	matchplay = gi.cvar("matchplay", "0", 0);			//QW// matchplay makes game do countdown timer on each map

	nightdaytime  = gi.cvar("nightdaytime","0",0);	//QW// minutes for a day-night cycle
	minlevel = gi.cvar("minlevel", "0", 0);		//QW// lowest light level to apply (0 | 1 | 2)
	
	// flood control
	flood_msgs = gi.cvar ("flood_msgs", "4", 0);
	flood_persecond = gi.cvar ("flood_persecond", "4", 0);
	flood_waitdelay = gi.cvar ("flood_waitdelay", "10", 0);

	// weapon/feature banning
	// beware large values, see UpdateBans().
	weaponban = gi.cvar ("weaponban", "0", CVAR_SERVERINFO);
	featureban = gi.cvar ("featureban", "0", CVAR_SERVERINFO);
	fragban = gi.cvar ("fragban", "0", CVAR_SERVERINFO);
	loxweaponban = gi.cvar("loxweaponban", "0", CVAR_SERVERINFO);
	loxfeatureban = gi.cvar("loxfeatureban","0",CVAR_SERVERINFO);
	loxgrenadeban = gi.cvar("loxgrenadeban","0",0);
	loxarmorban = gi.cvar("loxarmorban","0",0);
	loxsweeperban = gi.cvar("loxsweeperban","0",0);
	loxtrackerban = gi.cvar("loxtrackerban","0",CVAR_SERVERINFO);
	loxturretban = gi.cvar("loxturretban","0",CVAR_SERVERINFO);

	//QW// toss backpack full of weps and ammo on player death
	backpacktoss = gi.cvar("backpacktoss","0", 0);

	//QW// default probabilities of pickups for regen, organic, etc.
	// 10% is 1000/10000 (10 in 100)
	p_pickup_regen = gi.cvar("p_pickup_regen","1000",0);
	p_pickup_vampiric = gi.cvar("p_pickup_vampiric","1000",0);
	p_pickup_invulnerability = gi.cvar("p_pickup_invulnerability","1000",0);
	p_pickup_organic_armor = gi.cvar("p_pickup_organic_armor","1000",0);
	p_pickup_slug_armor = gi.cvar("p_pickup_slug_armor","1000",0);
	p_pickup_cell_armor = gi.cvar("p_pickup_cell_armor","1000",0);
	p_pickup_shell_armor = gi.cvar("p_pickup_shell_armor","1000",0);
	p_pickup_bullet_armor = gi.cvar("p_pickup_bullet_armor","1000",0);
	p_pickup_rocket_armor = gi.cvar("p_pickup_rocket_armor","1000",0);
	p_pickup_grenade_armor = gi.cvar("p_pickup_grenade_armor","1000",0);

	//some nice small values again
	revenge_factor = gi.cvar("revenge_factor","0", 0);
	anticamper = gi.cvar("anticamper","0", 0);
	highfragger = gi.cvar("highfragger","0", 0);
	weaponheat = gi.cvar("weaponheat", "0", 0);
	revengereward = gi.cvar("revengereward","1", 0);
	highfraggerreward = gi.cvar("highfraggerreward","2", 0);
	sv_bestplayer = gi.cvar("sv_bestplayer","1",0); 
	console_chat = gi.cvar("console_chat", "1", 0);

	Maplist_InitVars();	// initialize the maplist management Cvars
	Voting_InitVars();

	initialization = true;
	UpdateBans();
	initialization = false;

	// items
	InitItems();

	Com_sprintf (game.helpmessage1, sizeof(game.helpmessage1), "");
	Com_sprintf (game.helpmessage2, sizeof(game.helpmessage2), "");

	// initialize all entities for this game
	game.maxentities = maxentities->value;
	g_edicts =  gi.TagMalloc (game.maxentities * sizeof(g_edicts[0]), TAG_GAME);
	globals.edicts = g_edicts;
	globals.max_edicts = game.maxentities;
	gi.dprintf("%s entities allocated. (%i bytes)\n", maxentities->string, game.maxentities * sizeof(g_edicts[0]));
	gi.dprintf("Size of an entity: %i bytes\n", sizeof(g_edicts[0]));

	// initialize all clients for this game
	game.maxclients = maxclients->value;
	game.clients = gi.TagMalloc (game.maxclients * sizeof(game.clients[0]), TAG_GAME);
	globals.num_edicts = game.maxclients + 1;
	game.peak_edicts = globals.num_edicts;
	gi.dprintf("Space for %s clients allocated. (%i bytes)\n", maxclients->string, game.maxclients * sizeof(game.clients[0]));
	gi.dprintf("Bytes per client: %i\n", sizeof(game.clients[0]));

	game.highfraggeraward = highfraggerreward->value;
	game.revengereward = revengereward->value;

	CTFInit();
	MatchInit();
	SetUpHeatBar();
}

/* 
* For LOX versions 1.12.6 and up.
* The cvars contain both string and float values received from
* here in the initialization code, from the server config file or thru rcon commands. 
*
* The original game only used 16 bits or so of integer values for bitmasks like dmflags.
* Larger mods like LOX extended the idea to 32 bits but floats break fidelity
* at 25 bits, a float of 16777216.000000 or int value of 16777217 (0x01000001).
* Using double for values would have been a wiser choice in the long
* run but when Q2 was designed I don't think the hardware was up for it.
* 
* Large values of floats don't map properly to bitmap banning so we convert the strings
* to unsigned long here so we can use them later.
*
* The original code had tests like (int)loxweaponban->value & LWB_GUIDEDNUKE 
* and this simply didn't work for 32-bit values like 0x7fffffff when 
* converted inside the cvars. 
*
* Here we grab the cvar string member and convert it nice and neat. 
* 
* Freebie: Using strtoul() allows us to use decimal, octal or 
* hex values in the commands. If the user uses decimal values
* the Quake2 core will put it in the cvar string and float members, but if it's
* entered as octal or hex it only puts it in as a string. Remember to always
* use the string member when doing these and things will work out just fine.
*/
// Worst-case execution time of this function is 12 uS 
// on a 2.8GHz P4 on Windows XP. Average was 3 or 4 uS 
// so it costs almost nothing to run it every server frame.

void UpdateBans (void)
{

//	START_PERFORMANCE_TIMER;

	if (weaponban->modified || initialization) 
	{
		i_weaponban = strtoul(weaponban->string, NULL,0);
		weaponban->modified = false;
	}
	
	if (featureban->modified || initialization)
	{
		i_featureban = strtoul(featureban->string, NULL,0);
		featureban->modified = false;
	}
	
	if (fragban->modified || initialization)
	{
		i_fragban = strtoul(fragban->string, NULL,0);
		fragban->modified = false;
	}
	
	if (loxweaponban->modified || initialization)
	{
		i_loxweaponban = strtoul(loxweaponban->string, NULL,0);
		loxweaponban->modified = false;
	}
	
	if (loxfeatureban->modified || initialization)
	{
		i_loxfeatureban = strtoul(loxfeatureban->string, NULL,0);
		loxfeatureban->modified = false;
	}
	
	if (loxgrenadeban->modified || initialization)
	{
		i_loxgrenadeban = strtoul(loxgrenadeban->string, NULL,0);
		loxgrenadeban->modified = false;
	}
	
	if (loxarmorban->modified || initialization)
	{
		i_loxarmorban = strtoul(loxarmorban->string, NULL,0);
		loxarmorban->modified = false;
	}
	
	if (loxsweeperban->modified || initialization)
	{
		i_loxsweeperban = strtoul(loxsweeperban->string, NULL,0);
		loxsweeperban->modified = false;
	}
	
	if (loxtrackerban->modified || initialization)
	{
		i_loxtrackerban = strtoul(loxtrackerban->string, NULL,0);
		loxtrackerban->modified = false;
	}
	
	if (loxturretban->modified || initialization)
	{
		i_loxturretban = strtoul(loxturretban->string, NULL,0);
		loxturretban->modified = false;
	}

	//QW// bounds check for armor/regen/organic upgrade probabilities
	// allowed range is 0 to 50% probability of obtaining enhancements.
	// Out of bounds configuration causes reset to default 10%
	
	if (p_pickup_regen->value > 5000 || p_pickup_regen->value < 0)
		p_pickup_regen->value = 1000;
	
	if (p_pickup_vampiric->value > 5000 || p_pickup_vampiric->value < 0)
		p_pickup_vampiric->value = 1000;

	if (p_pickup_invulnerability->value > 5000 || p_pickup_invulnerability->value < 0)
		p_pickup_invulnerability->value = 1000;

	if (p_pickup_organic_armor->value > 5000 || p_pickup_organic_armor->value < 0)
		p_pickup_organic_armor->value = 100;
	
	if (p_pickup_slug_armor->value > 5000 || p_pickup_slug_armor->value < 0)
		p_pickup_slug_armor->value = 1000;

	if (p_pickup_cell_armor->value > 5000 || p_pickup_cell_armor->value < 0)
		p_pickup_cell_armor->value = 1000;

	if (p_pickup_shell_armor->value > 5000 || p_pickup_shell_armor->value < 0)
		p_pickup_shell_armor->value = 1000;
	
	if (p_pickup_bullet_armor->value > 5000 || p_pickup_bullet_armor->value < 0)
		p_pickup_bullet_armor->value = 1000;
	
	if (p_pickup_rocket_armor->value > 5000 || p_pickup_rocket_armor->value < 0)
		p_pickup_rocket_armor->value = 1000;
	
	if (p_pickup_grenade_armor->value > 5000 || p_pickup_grenade_armor->value < 0)
		p_pickup_grenade_armor->value = 1000;

	if (respawn_protect->modified || initialization)
	{
		//limit value ranges allowed for the respawn protection
		if (respawn_protect->value <= 0) gi.cvar_set("respawn_protect","0");
		if (respawn_protect->value >= 6) gi.cvar_set("respawn_protect","6");
		respawn_protect->modified = false;
	}

//	STOP_PERFORMANCE_TIMER(__func__);
}

//=========================================================

void WriteField1 (FILE *f, field_t *field, byte *base)
{
	void		*p;
	int			len;
	int			index;

	p = (void *)(base + field->ofs);
	switch (field->type)
	{
	case F_INT:
	case F_FLOAT:
	case F_ANGLEHACK:
	case F_VECTOR:
	case F_IGNORE:
		break;

	case F_LSTRING:
	case F_GSTRING:
		if ( *(char **)p )
			len = strlen(*(char **)p) + 1;
		else
			len = 0;
		*(int *)p = len;
		break;
	case F_EDICT:
		if ( *(edict_t **)p == NULL)
			index = -1;
		else
			index = *(edict_t **)p - g_edicts;
		*(int *)p = index;
		break;
	case F_CLIENT:
		if ( *(gclient_t **)p == NULL)
			index = -1;
		else
			index = *(gclient_t **)p - game.clients;
		*(int *)p = index;
		break;
	case F_ITEM:
		if ( *(edict_t **)p == NULL)
			index = -1;
		else
			index = ITEM_INDEX (*(gitem_t **)p);
		*(int *)p = index;
		break;

	default:
		gi.error ("WriteEdict: unknown field type");
	}
}

void WriteField2 (FILE *f, field_t *field, byte *base)
{
	int			len;
	void		*p;

	p = (void *)(base + field->ofs);
	switch (field->type)
	{
	case F_LSTRING:
	case F_GSTRING:
		if ( *(char **)p )
		{
			len = strlen(*(char **)p) + 1;
			fwrite (*(char **)p, len, 1, f);
		}
		break;
	
	default: 
		break;
	}
}

void ReadField (FILE *f, field_t *field, byte *base)
{
	void		*p;
	int			len;
	int			index;

	p = (void *)(base + field->ofs);
	switch (field->type)
	{
	case F_INT:
	case F_FLOAT:
	case F_ANGLEHACK:
	case F_VECTOR:
	case F_IGNORE:
		break;

	case F_LSTRING:
		len = *(int *)p;
		if (!len)
			*(char **)p = NULL;
		else
		{
			*(char **)p = gi.TagMalloc (len, TAG_LEVEL);
			fread (*(char **)p, len, 1, f);
		}
		break;
	case F_GSTRING:
		len = *(int *)p;
		if (!len)
			*(char **)p = NULL;
		else
		{
			*(char **)p = gi.TagMalloc (len, TAG_GAME);
			fread (*(char **)p, len, 1, f);
		}
		break;
	case F_EDICT:
		index = *(int *)p;
		if ( index == -1 )
			*(edict_t **)p = NULL;
		else
			*(edict_t **)p = &g_edicts[index];
		break;
	case F_CLIENT:
		index = *(int *)p;
		if ( index == -1 )
			*(gclient_t **)p = NULL;
		else
			*(gclient_t **)p = &game.clients[index];
		break;
	case F_ITEM:
		index = *(int *)p;
		if ( index == -1 )
			*(gitem_t **)p = NULL;
		else
			*(gitem_t **)p = itemlist[index];
		break;

	default:
		gi.error ("ReadEdict: unknown field type");
	}
}

//=========================================================

/*
==============
WriteClient

All pointer variables (except function pointers) must be handled specially.
==============
*/
void WriteClient (FILE *f, gclient_t *client)
{
	field_t		*field;
	gclient_t	temp;
	
	// all of the ints, floats, and vectors stay as they are
	temp = *client;

	// change the pointers to lengths or indexes
	for (field=clientfields ; field->name ; field++)
	{
		WriteField1 (f, field, (byte *)&temp);
	}

	// write the block
	fwrite (&temp, sizeof(temp), 1, f);

	// now write any allocated data following the edict
	for (field=clientfields ; field->name ; field++)
	{
		WriteField2 (f, field, (byte *)client);
	}
}

/*
==============
ReadClient

All pointer variables (except function pointers) must be handled specially.
==============
*/
void ReadClient (FILE *f, gclient_t *client)
{
	field_t		*field;

	fread (client, sizeof(*client), 1, f);

	for (field=clientfields ; field->name ; field++)
	{
		ReadField (f, field, (byte *)client);
	}
}

/*
============
WriteGame

This will be called whenever the game goes to a new level,
and when the user explicitly saves the game.

Game information include cross level data, like multi level
triggers, help computer info, and all client states.

A single player death will automatically restore from the
last save position.
============
*/
void WriteGame (char *filename, qboolean autosave)
{
	FILE	*f;
	int		i;
	char	str[16];

	if (!autosave)
		SaveClientData ();

	f = fopen (filename, "wb");
	if (!f) {
		gi.error ("Couldn't open %s", filename);
		exit(1); // never gets here
	}
	memset (str, 0, sizeof(str));
	strcpy (str, __DATE__);	//safe, no user input
	fwrite (str, sizeof(str), 1, f);

	game.autosaved = autosave;
	fwrite (&game, sizeof(game), 1, f);
	game.autosaved = false;

	for (i=0 ; i<game.maxclients ; i++)
		WriteClient (f, &game.clients[i]);

	fclose (f);
}

void ReadGame (char *filename)
{
	FILE	*f;
	int		i;
	char	str[16];

	gi.FreeTags (TAG_GAME);

	f = fopen (filename, "rb");
	if (!f) {
		gi.error ("Couldn't open %s", filename);
		exit(1); // never gets here
	}
	fread (str, sizeof(str), 1, f);
	if (Q_stricmp (str, __DATE__))
	{
		fclose (f);
		gi.error ("Savegame from an older version.\n");
	}

	g_edicts =  gi.TagMalloc (game.maxentities * sizeof(g_edicts[0]), TAG_GAME);
	globals.edicts = g_edicts;

	fread (&game, sizeof(game), 1, f);
	game.clients = gi.TagMalloc (game.maxclients * sizeof(game.clients[0]), TAG_GAME);
	for (i=0 ; i<game.maxclients ; i++)
		ReadClient (f, &game.clients[i]);

	fclose (f);
}

//==========================================================


/*
==============
WriteEdict

All pointer variables (except function pointers) must be handled specially.
==============
*/
void WriteEdict (FILE *f, edict_t *ent)
{
	field_t		*field;
	edict_t		temp;

	// all of the ints, floats, and vectors stay as they are
	temp = *ent;

	// change the pointers to lengths or indexes
	for (field=savefields ; field->name ; field++)
	{
		WriteField1 (f, field, (byte *)&temp);
	}

	// write the block
	fwrite (&temp, sizeof(temp), 1, f);

	// now write any allocated data following the edict
	for (field=savefields ; field->name ; field++)
	{
		WriteField2 (f, field, (byte *)ent);
	}

}

/*
==============
WriteLevelLocals

All pointer variables (except function pointers) must be handled specially.
==============
*/
void WriteLevelLocals (FILE *f)
{
	field_t		*field;
	level_locals_t		temp;

	// all of the ints, floats, and vectors stay as they are
	temp = level;

	// change the pointers to lengths or indexes
	for (field=levelfields ; field->name ; field++)
	{
		WriteField1 (f, field, (byte *)&temp);
	}

	// write the block
	fwrite (&temp, sizeof(temp), 1, f);

	// now write any allocated data following the edict
	for (field=levelfields ; field->name ; field++)
	{
		WriteField2 (f, field, (byte *)&level);
	}
}


/*
==============
ReadEdict

All pointer variables (except function pointers) must be handled specially.
==============
*/
void ReadEdict (FILE *f, edict_t *ent)
{
	field_t		*field;

	fread (ent, sizeof(*ent), 1, f);

	for (field=savefields ; field->name ; field++)
	{
		ReadField (f, field, (byte *)ent);
	}
}

/*
==============
ReadLevelLocals

All pointer variables (except function pointers) must be handled specially.
==============
*/
void ReadLevelLocals (FILE *f)
{
	field_t		*field;

	fread (&level, sizeof(level), 1, f);

	for (field=levelfields ; field->name ; field++)
	{
		ReadField (f, field, (byte *)&level);
	}
}

/*
=================
WriteLevel

=================
*/
void WriteLevel (char *filename)
{
	int		i;
	edict_t	*ent;
	FILE	*f;
	void	(*base)(void);

	f = fopen (filename, "wb");
	if (!f){
		gi.error ("Couldn't open %s", filename);
		exit(1); // never gets here
	}
	// write out edict size for checking
	i = sizeof(edict_t);
	fwrite (&i, sizeof(i), 1, f);

	// write out a function pointer for checking
	base = InitGame;
	fwrite (&base, sizeof(base), 1, f);

	// write out level_locals_t
	WriteLevelLocals (f);

	// write out all the entities
	for (i=0 ; i < globals.num_edicts ; i++)
	{
		ent = &g_edicts[i];
		if (!ent->inuse)
			continue;
		fwrite (&i, sizeof(i), 1, f);
		WriteEdict (f, ent);
	}
	i = -1;
	fwrite (&i, sizeof(i), 1, f);

	fclose (f);
}


/*
=================
ReadLevel

SpawnEntities will already have been called on the
level the same way it was when the level was saved.

That is necessary to get the baselines
set up identically.

The server will have cleared all of the world links before
calling ReadLevel.

No clients are connected yet.
=================
*/
void ReadLevel (char *filename)
{
	int		entnum;
	FILE	*f;
	int		i;
	void	(*base)(void);
	edict_t	*ent;

	f = fopen (filename, "rb");
	if (!f) {
		gi.error ("Couldn't open %s", filename);
		exit(1); // never gets here. gi.error doesn't return
	}
	// free any dynamic memory allocated by loading the level
	// base state
	gi.FreeTags (TAG_LEVEL);

	// wipe all the entities
	memset (g_edicts, 0, game.maxentities * sizeof(g_edicts[0]));
	globals.num_edicts = maxclients->value + 1;

	// check edict size
	fread (&i, sizeof(i), 1, f);
	if (i != sizeof(edict_t))
	{
		fclose (f);
		gi.error ("ReadLevel: mismatched edict size");
	}

	// check function pointer base address
	fread (&base, sizeof(base), 1, f);
	if (base != InitGame)
	{
		fclose (f);
		gi.error ("ReadLevel: function pointers have moved");
	}

	// load the level locals
	ReadLevelLocals (f);

	// load all the entities
	for (;;)
	{
		if (fread (&entnum, sizeof(entnum), 1, f) != 1)
		{
			fclose (f);
			gi.error ("ReadLevel: failed to read entnum");
		}
		if (entnum == -1)
			break;
		if (entnum >= globals.num_edicts)
			globals.num_edicts = entnum + 1;

		ent = &g_edicts[entnum];
		ReadEdict (f, ent);

		// let the server rebuild world links for this ent
		memset (&ent->area, 0, sizeof(ent->area));
		gi.linkentity (ent);
	}

	fclose (f);

	// mark all clients as unconnected
	for (i=0 ; i < maxclients->value ; i++)
	{
		ent = &g_edicts[i+1];
		ent->client = game.clients + i;
		ent->client->pers.connected = false;
	}

	// do any load time things at this point
	for (i=0 ; i < globals.num_edicts ; i++)
	{
		ent = &g_edicts[i];

		if (!ent->inuse)
			continue;

		// fire any cross-level triggers
		if (ent->classname)
			if (strcmp(ent->classname, "target_crosslevel_target") == 0)
				ent->nextthink = level.time + ent->delay;
	}
}
