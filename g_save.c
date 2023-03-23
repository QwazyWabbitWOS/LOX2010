
#include "g_local.h"
#include "g_team.h"

#include "l_banner.h"
#include "maplist.h"
#include "l_voting.h"
#include "l_backpack.h"
#include "l_ar_admin.h"

field_t fields[] =
{
	{"classname", FOFS(classname), F_LSTRING, 0},
	{"origin", FOFS(s.origin), F_VECTOR, 0},
	{"model", FOFS(model), F_LSTRING, 0},
	{"spawnflags", FOFS(spawnflags), F_INT, 0},
	{"speed", FOFS(speed), F_FLOAT, 0},
	{"accel", FOFS(accel), F_FLOAT, 0},
	{"decel", FOFS(decel), F_FLOAT, 0},
	{"target", FOFS(target), F_LSTRING, 0},
	{"targetname", FOFS(targetname), F_LSTRING, 0},
	{"pathtarget", FOFS(pathtarget), F_LSTRING, 0},
	{"deathtarget", FOFS(deathtarget), F_LSTRING, 0},
	{"killtarget", FOFS(killtarget), F_LSTRING, 0},
	{"combattarget", FOFS(combattarget), F_LSTRING, 0},
	{"message", FOFS(message), F_LSTRING, 0},
	{"team", FOFS(team), F_LSTRING, 0},
	{"wait", FOFS(wait), F_FLOAT, 0},
	{"delay", FOFS(delay), F_FLOAT, 0},
	{"random", FOFS(random), F_FLOAT, 0},
	{"move_origin", FOFS(move_origin), F_VECTOR, 0},
	{"move_angles", FOFS(move_angles), F_VECTOR, 0},
	{"style", FOFS(style), F_INT, 0},
	{"count", FOFS(count), F_INT, 0},
	{"health", FOFS(health), F_INT, 0},
	{"sounds", FOFS(sounds), F_INT, 0},
	{"light", 0, F_IGNORE, 0},
	{"dmg", FOFS(dmg), F_INT, 0},
	{"angles", FOFS(s.angles), F_VECTOR, 0},
	{"angle", FOFS(s.angles), F_ANGLEHACK, 0},
	{"mass", FOFS(mass), F_INT, 0},
	{"volume", FOFS(volume), F_FLOAT, 0},
	{"attenuation", FOFS(attenuation), F_FLOAT, 0},
	{"map", FOFS(map), F_LSTRING, 0},

	// Added for WoD, for the new style of func_killbox
	{"mins", FOFS(mins), F_VECTOR, 0},
	{"maxs", FOFS(maxs), F_VECTOR, 0},

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

	{0, 0, 0, 0}	// End of list. ED_ParseField relies on this marker. (f->name)
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
	{"", FOFS(classname), F_LSTRING, 0},
	{"", FOFS(target), F_LSTRING, 0},
	{"", FOFS(targetname), F_LSTRING, 0},
	{"", FOFS(killtarget), F_LSTRING, 0},
	{"", FOFS(team), F_LSTRING, 0},
	{"", FOFS(pathtarget), F_LSTRING, 0},
	{"", FOFS(deathtarget), F_LSTRING, 0},
	{"", FOFS(combattarget), F_LSTRING, 0},
	{"", FOFS(model), F_LSTRING, 0},
	{"", FOFS(map), F_LSTRING, 0},
	{"", FOFS(message), F_LSTRING, 0},

	{"", FOFS(client), F_CLIENT, 0},
	{"", FOFS(item), F_ITEM, 0},

	{"", FOFS(goalentity), F_EDICT, 0},
	{"", FOFS(movetarget), F_EDICT, 0},
	{"", FOFS(enemy), F_EDICT, 0},
	{"", FOFS(oldenemy), F_EDICT, 0},
	{"", FOFS(activator), F_EDICT, 0},
	{"", FOFS(groundentity), F_EDICT, 0},
	{"", FOFS(teamchain), F_EDICT, 0},
	{"", FOFS(teammaster), F_EDICT, 0},
	{"", FOFS(owner), F_EDICT, 0},
	{"", FOFS(mynoise), F_EDICT, 0},
	{"", FOFS(mynoise2), F_EDICT, 0},
	{"", FOFS(target_ent), F_EDICT, 0},
	{"", FOFS(chain), F_EDICT, 0},

	{NULL, 0, F_INT, 0}
};

field_t		levelfields[] =
{
	{"", LLOFS(changemap), F_LSTRING, 0},

	{"", LLOFS(sight_client), F_EDICT, 0},
	{"", LLOFS(sight_entity), F_EDICT, 0},
	{"", LLOFS(sound_entity), F_EDICT, 0},
	{"", LLOFS(sound2_entity), F_EDICT, 0},

	{NULL, 0, F_INT, 0}
};

field_t		clientfields[] =
{
	{"", CLOFS(pers.weapon), F_ITEM, 0},
	{"", CLOFS(pers.lastweapon), F_ITEM, 0},
	{"", CLOFS(newweapon), F_ITEM, 0},

	{NULL, 0, F_INT, 0}
};

void SetUpHeatBar(void)
{
	game.heat_bar[0] = "\200\203\201\201\201\201\201\201\201\201\201\201\201\201\201\202";
	game.heat_bar[1] = "\200\201\203\201\201\201\201\201\201\201\201\201\201\201\201\202";
	game.heat_bar[2] = "\200\201\201\203\201\201\201\201\201\201\201\201\201\201\201\202";
	game.heat_bar[3] = "\200\201\201\201\203\201\201\201\201\201\201\201\201\201\201\202";
	game.heat_bar[4] = "\200\201\201\201\201\203\201\201\201\201\201\201\201\201\201\202";
	game.heat_bar[5] = "\200\201\201\201\201\201\203\201\201\201\201\201\201\201\201\202";
	game.heat_bar[6] = "\200\201\201\201\201\201\201\203\201\201\201\201\201\201\201\202";
	game.heat_bar[7] = "\200\201\201\201\201\201\201\201\203\201\201\201\201\201\201\202";
	game.heat_bar[8] = "\200\201\201\201\201\201\201\201\201\203\201\201\201\201\201\202";
	game.heat_bar[9] = "\200\201\201\201\201\201\201\201\201\201\203\201\201\201\201\202";
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
void InitGame(void)
{
	cvar_t* version;	//The engine version string

#ifdef	_WIN32
	_CrtMemCheckpoint(&startup1);
#endif

	gi.dprintf("==== InitGame (LOX %s %s %s) ====\n", VERSION, BUILD, __DATE__);
	gi.dprintf(loxbanner, VERSION, BUILD, __DATE__, __TIME__);
	version = gi.cvar("version", "", CVAR_NOSET);
	if (version)
		gi.dprintf("LOX detected engine: %s\n", version->string);
	Log_Time();

	srand(time(NULL)); //seed the rng

	game.hudnames[0] = "WOD";
	game.hudnames[1] = "WOD:LOX1";
	game.hudnames[2] = "WOD:LOX2";
	game.hudnames[3] = "BLANK";
	game.hudnames[4] = "PLAYER";
	gun_x = gi.cvar("gun_x", "0", 0);
	gun_y = gi.cvar("gun_y", "0", 0);
	gun_z = gi.cvar("gun_z", "0", 0);

	//FIXME: sv_ prefix is wrong for these
	sv_rollspeed = gi.cvar("sv_rollspeed", "200", 0);
	sv_rollangle = gi.cvar("sv_rollangle", "2", 0);
	sv_maxvelocity = gi.cvar("sv_maxvelocity", "2500", 0);
	sv_gravity = gi.cvar("sv_gravity", "800", 0);

	// noset vars  (set from command line or server config file)
	dedicated = gi.cvar("dedicated", "0", CVAR_NOSET);
	gamedir = gi.cvar("gamedir", "lox", CVAR_NOSET);

	// latched vars (changed when game next restarts)

	sv_cheats = gi.cvar("cheats", "0", CVAR_SERVERINFO | CVAR_LATCH);
	gi.cvar("gamename", GAMEVERSION, CVAR_SERVERINFO | CVAR_LATCH);
	gi.cvar("gamedate", __DATE__, CVAR_SERVERINFO | CVAR_LATCH);

	maxclients = gi.cvar("maxclients", "12", CVAR_SERVERINFO | CVAR_LATCH);
	maxspectators = gi.cvar("maxspectators", "12", CVAR_SERVERINFO);
	deathmatch = gi.cvar("deathmatch", "0", CVAR_LATCH);
	ctf = gi.cvar("ctf", "0", CVAR_LATCH);
	coop = gi.cvar("coop", "0", CVAR_LATCH | CVAR_SERVERINFO);
	skill = gi.cvar("skill", "3", CVAR_LATCH);
	noscore = gi.cvar("noscore", "0", CVAR_LATCH); // for the new scoreboard logic
	maxentities = gi.cvar("maxentities", "1024", CVAR_LATCH);
	dumpents = gi.cvar("dumpents", "0", 0);		//QW// create entfiles as we go
	custom_ents = gi.cvar("custom_ents", "0", 0);		//QW// allow custom ents in DM
	ext_devt = gi.cvar("ext_devt", "0", CVAR_LATCH);	//QW// set this for extinction development
	staticlights = gi.cvar("staticlights", "0", CVAR_LATCH);	//QW// 1 = no dynamic map lights.
	developer = gi.cvar("developer", "0", CVAR_LATCH);	//QW// 1 = output debug info to console/log

	if (coop->value)
		gi.cvar_forceset("gamename", GAMEVERSION_C);	// show coop mode version string

	motdfile = gi.cvar("motdfile", "motd.txt", 0);

	// change anytime vars
	dmflags = gi.cvar("dmflags", "0", CVAR_SERVERINFO);
	fraglimit = gi.cvar("fraglimit", "0", CVAR_SERVERINFO);
	timelimit = gi.cvar("timelimit", "0", CVAR_SERVERINFO);
	password = gi.cvar("password", "", CVAR_USERINFO);
	spectator_password = gi.cvar("spectator_password", "", CVAR_USERINFO);
	needpass = gi.cvar("needpass", "0", CVAR_SERVERINFO);
	filterban = gi.cvar("filterban", "1", 0);
	scoreboardtime = gi.cvar("scoreboardtime", "15", 0);
	cycle_always = gi.cvar("cycle_always", "0", 0);
	
	//QW// Selector for spiral rocket head gibs vs normal rocket
	spiral_heads = gi.cvar("spiral_heads", "0", 0);

	g_select_empty = gi.cvar("g_select_empty", "0", CVAR_ARCHIVE);

	run_pitch = gi.cvar("run_pitch", "0.002", 0);
	run_roll = gi.cvar("run_roll", "0.005", 0);
	bob_up = gi.cvar("bob_up", "0.005", 0);
	bob_pitch = gi.cvar("bob_pitch", "0.002", 0);
	bob_roll = gi.cvar("bob_roll", "0.002", 0);

	debugmodels = gi.cvar("debugmodels", "0", 0); //QW// server side runtime fix for debug models
	max_trackers = gi.cvar("maxtrackers", "3", 0); //QW//
	max_turrets = gi.cvar("maxturrets", "3", 0); //TheLopez//
	max_lasertrips = gi.cvar("maxlasertrips", "4", 0); //QW//
	max_floatingmines = gi.cvar("maxfloatingmines", "7", 0); //QW//
	kami_frozen = gi.cvar("kamifrozen", "0", 0); // allow kami if frozen
	homerseekowner = gi.cvar("homerseekowner", "1", 0);	//QW// homing rockets seek owners
	newscoreboard = gi.cvar("newscoreboard", "0", 0);	//QW// use the new DM Scoreboard
	g_rockets_takedamage = gi.cvar("g_rockets_takedamage", "0", 0);	//QW// admin option, guided rocket damage
	g_nukes_takedamage = gi.cvar("g_nukes_takedamage", "0", 0);	//QW// admin option, guided nuke damage
	push_kill_award = gi.cvar("push_kill_award", "0", 0);	//QW// set to number of points awarded for pushkill
	push_kill_delay = gi.cvar("push_kill_delay", "3.0", 0);	//QW// window from push to death
	autoweaponselect = gi.cvar("autoweaponselect", "1", 0);	//QW// for coop mode
	fast_weapon_change = gi.cvar("fast_weapon_change", "0", 0);	//QW// for fast weapon change mode
	max_cats = gi.cvar("max_cats", "0", 0);	//QW// for max cats per player per level
	zbot_check = gi.cvar("zbot_check", "0", 0);	//QW// enable/disable internal zbot checking

	//cvars pertaining to stats logs
	logstats = gi.cvar("logstats", "1", 0);		//QW// allow logging player statistics
	logarstats = gi.cvar("logarstats", "0", 0);	//QW// allow logging Altered Realm player statistics
	statsfile = gi.cvar("statsfile", "stats.log", 0); //QW// the name of the AR stats file
	statsfile_rename = gi.cvar("statsfile_rename", "1", 0); //QW// 0 = never, 1 = daily, 2 = weekly, 3 = monthly
	//cvars pertaining to server logs
	logfile = gi.cvar("logfile", "", 0);			//QW// allow server logging
	logfile_name = gi.cvar("logfile_name", "server.log", 0); //QW// the name of the server log file (clients use qconsole.log)
	logfile_rename = gi.cvar("logfile_rename", "2", 0); //QW// 0 = never, 1 = daily, 2 = weekly, 3 = monthly

	// the DM respawn invulnerability (defaults to 5 seconds)
	respawn_protect = gi.cvar("respawn_protect", "5", 0);	//QW// allow admin to control respawn invincibility time

	campertime = gi.cvar("campertime", "20", 0);		//QW// camping time limit 
	matchplay = gi.cvar("matchplay", "0", 0);			//QW// matchplay makes game do countdown timer on each map

	InitDayNightVars();

	// flood control
	flood_msgs = gi.cvar("flood_msgs", "4", 0);
	flood_persecond = gi.cvar("flood_persecond", "4", 0);
	flood_waitdelay = gi.cvar("flood_waitdelay", "10", 0);

	// weapon/feature banning
	// beware large values, see UpdateBans().
	weaponban = gi.cvar("weaponban", "0", CVAR_SERVERINFO);
	featureban = gi.cvar("featureban", "0", CVAR_SERVERINFO);
	fragban = gi.cvar("fragban", "0", CVAR_SERVERINFO);
	loxweaponban = gi.cvar("loxweaponban", "0", CVAR_SERVERINFO);
	loxfeatureban = gi.cvar("loxfeatureban", "0", CVAR_SERVERINFO);
	loxgrenadeban = gi.cvar("loxgrenadeban", "0", 0);
	loxarmorban = gi.cvar("loxarmorban", "0", 0);
	loxsweeperban = gi.cvar("loxsweeperban", "0", 0);
	loxtrackerban = gi.cvar("loxtrackerban", "0", CVAR_SERVERINFO);
	loxturretban = gi.cvar("loxturretban", "0", CVAR_SERVERINFO);

	//QW// toss backpack full of weps and ammo on player death
	backpacktoss = gi.cvar("backpacktoss", "0", 0);

	//QW// default probabilities of pickups for regen, organic, etc.
	// 10% is 1000/10000 (10 in 100)
	p_pickup_regen = gi.cvar("p_pickup_regen", "1000", 0);
	p_pickup_vampiric = gi.cvar("p_pickup_vampiric", "1000", 0);
	p_pickup_invulnerability = gi.cvar("p_pickup_invulnerability", "1000", 0);
	p_pickup_organic_armor = gi.cvar("p_pickup_organic_armor", "1000", 0);
	p_pickup_slug_armor = gi.cvar("p_pickup_slug_armor", "1000", 0);
	p_pickup_cell_armor = gi.cvar("p_pickup_cell_armor", "1000", 0);
	p_pickup_shell_armor = gi.cvar("p_pickup_shell_armor", "1000", 0);
	p_pickup_bullet_armor = gi.cvar("p_pickup_bullet_armor", "1000", 0);
	p_pickup_rocket_armor = gi.cvar("p_pickup_rocket_armor", "1000", 0);
	p_pickup_grenade_armor = gi.cvar("p_pickup_grenade_armor", "1000", 0);

	//some nice small values again
	revenge_factor = gi.cvar("revenge_factor", "0", 0);
	anticamper = gi.cvar("anticamper", "0", 0);
	highfragger = gi.cvar("highfragger", "0", 0);
	weaponheat = gi.cvar("weaponheat", "0", 0);
	revengereward = gi.cvar("revengereward", "1", 0);
	highfraggerreward = gi.cvar("highfraggerreward", "2", 0);
	sv_bestplayer = gi.cvar("sv_bestplayer", "1", 0);
	console_chat = gi.cvar("console_chat", "0", 0);

	Maplist_InitVars();	// initialize the maplist management Cvars
	Voting_InitVars();

	initialization = true;
	UpdateBans();
	initialization = false;

	// items
	InitItems();

	flashlightmode = gi.cvar("flashlightmode", "0", 0);

	Com_sprintf(game.helpmessage1, sizeof(game.helpmessage1), "");
	Com_sprintf(game.helpmessage2, sizeof(game.helpmessage2), "");

	// initialize all entities for this game
	game.maxentities = maxentities->value;
	g_edicts = gi.TagMalloc(game.maxentities * sizeof(g_edicts[0]), TAG_GAME);
	globals.edicts = g_edicts;
	globals.max_edicts = game.maxentities;
	gi.dprintf("%s entities allocated. (%i bytes)\n", maxentities->string, game.maxentities * sizeof(g_edicts[0]));
	gi.dprintf("Size of an entity: %i bytes\n", sizeof(g_edicts[0]));

	// initialize all clients for this game
	game.maxclients = maxclients->value;
	game.clients = gi.TagMalloc(game.maxclients * sizeof(game.clients[0]), TAG_GAME);
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
void UpdateBans(void)
{

	//START_PERFORMANCE_TIMER;

	if (weaponban->modified || initialization)
	{
		i_weaponban = strtoul(weaponban->string, NULL, 0);
		weaponban->modified = false;
	}

	if (featureban->modified || initialization)
	{
		i_featureban = strtoul(featureban->string, NULL, 0);
		featureban->modified = false;
	}

	if (fragban->modified || initialization)
	{
		i_fragban = strtoul(fragban->string, NULL, 0);
		fragban->modified = false;
	}

	if (loxweaponban->modified || initialization)
	{
		i_loxweaponban = strtoul(loxweaponban->string, NULL, 0);
		loxweaponban->modified = false;
	}

	if (loxfeatureban->modified || initialization)
	{
		i_loxfeatureban = strtoul(loxfeatureban->string, NULL, 0);
		loxfeatureban->modified = false;
	}

	if (loxgrenadeban->modified || initialization)
	{
		i_loxgrenadeban = strtoul(loxgrenadeban->string, NULL, 0);
		loxgrenadeban->modified = false;
	}

	if (loxarmorban->modified || initialization)
	{
		i_loxarmorban = strtoul(loxarmorban->string, NULL, 0);
		loxarmorban->modified = false;
	}

	if (loxsweeperban->modified || initialization)
	{
		i_loxsweeperban = strtoul(loxsweeperban->string, NULL, 0);
		loxsweeperban->modified = false;
	}

	if (loxtrackerban->modified || initialization)
	{
		i_loxtrackerban = strtoul(loxtrackerban->string, NULL, 0);
		loxtrackerban->modified = false;
	}

	if (loxturretban->modified || initialization)
	{
		i_loxturretban = strtoul(loxturretban->string, NULL, 0);
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
		if (respawn_protect->value <= 0) gi.cvar_set("respawn_protect", "0");
		if (respawn_protect->value >= 6) gi.cvar_set("respawn_protect", "6");
		respawn_protect->modified = false;
	}

	//STOP_PERFORMANCE_TIMER
}

//=========================================================

void WriteField1(FILE* f, field_t* field, byte* base)
{
	void* p;
	size_t		len;
	long		index;

	p = (void*)(base + field->ofs);
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
		if (*(char**)p)
			len = strlen(*(char**)p) + 1;
		else
			len = 0;
		*(int*)p = (int)len;
		break;
	case F_EDICT:
		if (*(edict_t**)p == NULL)
			index = -1;
		else
			index = *(edict_t**)p - g_edicts;
		*(int*)p = (int)index;
		break;
	case F_CLIENT:
		if (*(gclient_t**)p == NULL)
			index = -1;
		else
			index = *(gclient_t**)p - game.clients;
		*(int*)p = (int)index;
		break;
	case F_ITEM:
		if (*(edict_t**)p == NULL)
			index = -1;
		else
			index = ITEM_INDEX(*(gitem_t**)p);
		*(int*)p = index;
		break;

	default:
		GameError("WriteEdict: unknown field type");
	}
}

static void WriteField2(FILE* f, field_t* field, byte* base)
{
	size_t		len;
	void* p;

	p = (void*)(base + field->ofs);
	switch (field->type)
	{
	case F_LSTRING:
	case F_GSTRING:
		if (*(char**)p)
		{
			len = strlen(*(char**)p) + 1;
			fwrite(*(char**)p, len, 1, f);
		}
		break;

	default:
		break;
	}
}

void ReadField(FILE* f, field_t* field, byte* base)
{
	void* p;
	int			len;
	int			index;
	size_t	count;

	p = (void*)(base + field->ofs);
	switch (field->type)
	{
	case F_INT:
	case F_FLOAT:
	case F_ANGLEHACK:
	case F_VECTOR:
	case F_IGNORE:
		break;

	case F_LSTRING:
		len = *(int*)p;
		if (!len)
			*(char**)p = NULL;
		else
		{
			*(char**)p = gi.TagMalloc(len, TAG_LEVEL);
			count = fread(*(char**)p, len, 1, f);
			if (count) {
				; // don't worry, be happy
			}
		}
		break;
	case F_GSTRING:
		len = *(int*)p;
		if (!len)
			*(char**)p = NULL;
		else
		{
			*(char**)p = gi.TagMalloc(len, TAG_GAME);
			count = fread(*(char**)p, len, 1, f);
			if (count) {
				; // don't worry, be happy
			}
		}
		break;
	case F_EDICT:
		index = *(int*)p;
		if (index == -1)
			*(edict_t**)p = NULL;
		else
			*(edict_t**)p = &g_edicts[index];
		break;
	case F_CLIENT:
		index = *(int*)p;
		if (index == -1)
			*(gclient_t**)p = NULL;
		else
			*(gclient_t**)p = &game.clients[index];
		break;
	case F_ITEM:
		index = *(int*)p;
		if (index == -1)
			*(gitem_t**)p = NULL;
		else
			*(gitem_t**)p = itemlist[index];
		break;

	default:
		GameError("ReadEdict: unknown field type");
	}
}

//=========================================================

/*
==============
WriteClient

All pointer variables (except function pointers) must be handled specially.
==============
*/
void WriteClient(FILE* f, gclient_t* client)
{
	field_t* field;
	gclient_t	temp;

	// all of the ints, floats, and vectors stay as they are
	temp = *client;

	// change the pointers to lengths or indexes
	for (field = clientfields; field->name; field++)
	{
		WriteField1(f, field, (byte*)&temp);
	}

	// write the block
	fwrite(&temp, sizeof(temp), 1, f);

	// now write any allocated data following the edict
	for (field = clientfields; field->name; field++)
	{
		WriteField2(f, field, (byte*)client);
	}
}

/*
==============
ReadClient

All pointer variables (except function pointers) must be handled specially.
==============
*/
void ReadClient(FILE* f, gclient_t* client)
{
	field_t* field;
	size_t	count;

	count = fread(client, sizeof(*client), 1, f);
	if (count) {
		; // don't worry, be happy
	}

	for (field = clientfields; field->name; field++)
	{
		ReadField(f, field, (byte*)client);
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
void WriteGame(char* filename, qboolean autosave)
{
	FILE* f;
	int		i;
	char	str[16];

	if (dedicated->value)
		return;
	if (!autosave)
		SaveClientData();

	f = fopen(filename, "wb");
	if (!f)
		GameError("Couldn't open %s", filename);

	memset(str, 0, sizeof(str));
	strcpy(str, __DATE__);	//safe, no user input
	fwrite(str, sizeof(str), 1, f);

	game.autosaved = autosave;
	fwrite(&game, sizeof(game), 1, f);
	game.autosaved = false;

	for (i = 0; i < game.maxclients; i++)
		WriteClient(f, &game.clients[i]);

	fclose(f);
}

void ReadGame(char* filename)
{
	FILE* f;
	int		i;
	char	str[16] = { 0 };
	size_t	count;

	gi.FreeTags(TAG_GAME);

	if (dedicated->value)
		return;
	f = fopen(filename, "rb");
	if (!f)
		GameError("Couldn't open %s", filename);

	count = fread(str, sizeof(str), 1, f);
	if (count) {
		; // don't worry, be happy
	}
	if (Q_stricmp(str, __DATE__))
	{
		fclose(f);
		GameError("Savegame from an older version.\n");
	}

	g_edicts = gi.TagMalloc(game.maxentities * sizeof(g_edicts[0]), TAG_GAME);
	globals.edicts = g_edicts;

	count = fread(&game, sizeof(game), 1, f);
	game.clients = gi.TagMalloc(game.maxclients * sizeof(game.clients[0]), TAG_GAME);
	for (i = 0; i < game.maxclients; i++)
		ReadClient(f, &game.clients[i]);

	fclose(f);
}

//==========================================================


/*
==============
WriteEdict

All pointer variables (except function pointers) must be handled specially.
==============
*/
void WriteEdict(FILE* f, edict_t* ent)
{
	field_t* field;
	edict_t		temp;

	// all of the ints, floats, and vectors stay as they are
	temp = *ent;

	// change the pointers to lengths or indexes
	for (field = savefields; field->name; field++)
	{
		WriteField1(f, field, (byte*)&temp);
	}

	// write the block
	fwrite(&temp, sizeof(temp), 1, f);

	// now write any allocated data following the edict
	for (field = savefields; field->name; field++)
	{
		WriteField2(f, field, (byte*)ent);
	}

}

/*
==============
WriteLevelLocals

All pointer variables (except function pointers) must be handled specially.
==============
*/
void WriteLevelLocals(FILE* f)
{
	field_t* field;
	level_locals_t		temp;

	// all of the ints, floats, and vectors stay as they are
	temp = level;

	// change the pointers to lengths or indexes
	for (field = levelfields; field->name; field++)
	{
		WriteField1(f, field, (byte*)&temp);
	}

	// write the block
	fwrite(&temp, sizeof(temp), 1, f);

	// now write any allocated data following the edict
	for (field = levelfields; field->name; field++)
	{
		WriteField2(f, field, (byte*)&level);
	}
}


/*
==============
ReadEdict

All pointer variables (except function pointers) must be handled specially.
==============
*/
void ReadEdict(FILE* f, edict_t* ent)
{
	field_t* field;
	size_t	count;

	count = fread(ent, sizeof(*ent), 1, f);
	if (count) {
		; // don't worry, be happy
	}

	for (field = savefields; field->name; field++)
	{
		ReadField(f, field, (byte*)ent);
	}
}

/*
==============
ReadLevelLocals

All pointer variables (except function pointers) must be handled specially.
==============
*/
void ReadLevelLocals(FILE* f)
{
	field_t* field;
	size_t	count;

	count = fread(&level, sizeof(level), 1, f);
	if (count) {
		; // don't worry, be happy
	}
	for (field = levelfields; field->name; field++)
	{
		ReadField(f, field, (byte*)&level);
	}
}

/*
=================
WriteLevel

=================
*/
void WriteLevel(char* filename)
{
	int		i;
	edict_t* ent;
	FILE* f;
	void	(*base)(void);

	if (dedicated->value)
		return;
	f = fopen(filename, "wb");
	if (!f)
		GameError("Couldn't open %s", filename);

	// write out edict size for checking
	i = sizeof(edict_t);
	fwrite(&i, sizeof(i), 1, f);

	// write out a function pointer for checking
	base = InitGame;
	fwrite(&base, sizeof(base), 1, f);

	// write out level_locals_t
	WriteLevelLocals(f);

	// write out all the entities
	for (i = 0; i < globals.num_edicts; i++)
	{
		ent = &g_edicts[i];
		if (!ent->inuse)
			continue;
		fwrite(&i, sizeof(i), 1, f);
		WriteEdict(f, ent);
	}
	i = -1;
	fwrite(&i, sizeof(i), 1, f);

	fclose(f);
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
void ReadLevel(char* filename)
{
	int		entnum;
	FILE* f;
	int		i;
	void	(*base)(void);
	edict_t* ent;
	size_t	count;

	f = fopen(filename, "rb");
	if (!f)
		GameError("Couldn't open %s", filename);

	// free any dynamic memory allocated by loading the level
	// base state
	gi.FreeTags(TAG_LEVEL);

	// wipe all the entities
	memset(g_edicts, 0, game.maxentities * sizeof(g_edicts[0]));
	globals.num_edicts = maxclients->value + 1;

	// check edict size
	count = fread(&i, sizeof(i), 1, f);
	if (i != sizeof(edict_t))
	{
		fclose(f);
		GameError("ReadLevel: mismatched edict size");
	}

	// check function pointer base address
	count = fread(&base, sizeof(base), 1, f);
	if (base != InitGame)
	{
		fclose(f);
		GameError("ReadLevel: function pointers have moved");
	}

	// load the level locals
	ReadLevelLocals(f);

	// load all the entities
	while (1)
	{
		count = fread(&entnum, sizeof(entnum), 1, f);
		if (count != 1)
		{
			fclose(f);
			GameError("ReadLevel: failed to read entnum");
		}
		if (entnum == -1)
			break;
		if (entnum >= globals.num_edicts)
			globals.num_edicts = entnum + 1;

		ent = &g_edicts[entnum];
		ReadEdict(f, ent);

		// let the server rebuild world links for this ent
		memset(&ent->area, 0, sizeof(ent->area));
		gi.linkentity(ent);
	}

	fclose(f);

	// mark all clients as unconnected
	for (i = 0; i < maxclients->value; i++)
	{
		ent = &g_edicts[i + 1];
		ent->client = game.clients + i;
		ent->client->pers.connected = false;
	}

	// do any load time things at this point
	for (i = 0; i < globals.num_edicts; i++)
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
