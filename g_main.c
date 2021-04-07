
#include "g_local.h"

#include "l_voting.h"
#include "l_ar_admin.h"
#include "maplist.h"

// teamplay definitions
#include "g_team.h"

game_locals_t	game;
level_locals_t	level;
game_import_t	gi;
game_export_t	globals;
spawn_temp_t	st;

int vwep_index;
int	sm_meat_index;
int	snd_fry;
int meansOfDeath;

edict_t		*g_edicts;

cvar_t	*deathmatch;
cvar_t	*coop;
cvar_t	*dmflags;
cvar_t	*skill;
cvar_t	*noscore;
cvar_t	*fraglimit;
cvar_t	*timelimit;
cvar_t	*scoreboardtime;

cvar_t	*spectator_password;
cvar_t	*needpass;
cvar_t	*password;
cvar_t	*maxclients;
cvar_t	*maxspectators;
cvar_t	*maxentities;
cvar_t	*g_select_empty;
cvar_t	*dedicated;
cvar_t	*developer;

cvar_t	*filterban;

cvar_t	*sv_maxvelocity;
cvar_t	*sv_gravity;

cvar_t	*rollspeed;
cvar_t	*rollangle;
cvar_t	*gun_x;
cvar_t	*gun_y;
cvar_t	*gun_z;

cvar_t	*run_pitch;
cvar_t	*run_roll;
cvar_t	*bob_up;
cvar_t	*bob_pitch;
cvar_t	*bob_roll;

cvar_t	*sv_cheats;

cvar_t	*flood_msgs;
cvar_t	*flood_persecond;
cvar_t	*flood_waitdelay;

cvar_t	*weaponban;
cvar_t	*featureban;
cvar_t	*fragban;
cvar_t  *loxweaponban;
cvar_t	*loxfeatureban;
cvar_t	*loxgrenadeban;
cvar_t	*loxarmorban;
cvar_t  *loxsweeperban;
cvar_t  *loxtrackerban;
cvar_t  *loxturretban;
cvar_t	*p_pickup_regen; //QW// adjustable probability of pickup enhancements
cvar_t	*p_pickup_vampiric;
cvar_t	*p_pickup_invulnerability;
cvar_t	*p_pickup_organic_armor;
cvar_t	*p_pickup_slug_armor;
cvar_t	*p_pickup_cell_armor;
cvar_t	*p_pickup_shell_armor;
cvar_t	*p_pickup_bullet_armor;
cvar_t	*p_pickup_rocket_armor;
cvar_t	*p_pickup_grenade_armor;

//QW//
//These were in dm_flags in LOX 1.12.1 thru 1.12.18
cvar_t	*revenge_factor;
cvar_t	*anticamper;
cvar_t	*highfragger;
cvar_t	*weaponheat;
//QW//

cvar_t	*revengereward;
cvar_t	*highfraggerreward;
cvar_t	*sv_bestplayer;
cvar_t	*max_cats;		//QW// maximum cataclysm devices per player per level
cvar_t	*zbot_check;	//QW// enable/disable internal zbot checking
cvar_t	*console_chat;

//QW//
cvar_t	*debugmodels;		// set for model messages in console
cvar_t	*ext_devt;			// set to survey maps for extinction mode
cvar_t	*staticlights;		// set static map lighting mode
cvar_t	*max_trackers;		// maximum number of trackers allowed
cvar_t	*max_turrets;		// maximum number of turrets allowed
cvar_t	*max_lasertrips;	// maximum number of laser tripwires allowed
cvar_t	*max_floatingmines; // maximum number of floating mines allowed
cvar_t	*kami_frozen;		// allow player to go kamikaze if frozen
cvar_t	*campertime;		// time allowed for camping if ANTICAMPER is on
cvar_t	*dumpents;			// create ent files if set
cvar_t	*custom_ents;		// use custom ent files if available
cvar_t	*matchplay;			// boolean to decide if we want a match countdown
cvar_t	*homerseekowner;	// boolean to allow homers to seek owners (default = 1)
cvar_t	*logstats;			// Enable player stats summary to qconsole.log (default = 1)
cvar_t	*newscoreboard;		// Use the new DM scoreboard
cvar_t	*respawn_protect;	// admin configurable respawn protection
cvar_t	*g_rockets_takedamage; // admin config. guided rocket damage
cvar_t	*g_nukes_takedamage;	// admin config. guided nukes take damage
cvar_t	*push_kill_award;		// push/pulling can score a point
cvar_t	*push_kill_delay;		// window for frag point if pushkill award
cvar_t	*autoweaponselect;	//enable/disable auto select on weapon pickup in coop
cvar_t	*fast_weapon_change;	//enable/disable fast weapon change
//cvar_t	*passwd;

// these contain the conversion of the strings in the corresponding
// cvars so we can bitwise AND them with the weapon ban constants.

unsigned long i_weaponban;
unsigned long i_featureban;
unsigned long i_fragban;
unsigned long i_loxweaponban;
unsigned long i_loxfeatureban;
unsigned long i_loxgrenadeban;
unsigned long i_loxarmorban;
unsigned long i_loxsweeperban;
unsigned long i_loxtrackerban;
unsigned long i_loxturretban;


void ClientThink (edict_t *ent, usercmd_t *cmd);
qboolean ClientConnect (edict_t *ent, char *userinfo);
void ClientUserinfoChanged (edict_t *ent, char *userinfo);
void ClientDisconnect (edict_t *ent);
void ClientBegin (edict_t *ent);
void ClientCommand (edict_t *ent);
//void RunEntity (edict_t *ent);
void WriteGame (char *filename, qboolean autosave);
void ReadGame (char *filename);
void WriteLevel (char *filename);
void ReadLevel (char *filename);
void InitGame (void);
void G_RunFrame (void);
void LogPlayerStats(void);


// HACK
int HACK_modelindex (char *name);
int (*oldmodelindex) (char *name);
void HACK_SpawnEntities (char *mapname, char *entities, char *spawnpoint);

//===================================================================


void ShutdownGame (void)
{
	gi.dprintf ("==== ShutdownGame (LOX) ====\n");
	gi.FreeTags (TAG_LEVEL);
	gi.FreeTags (TAG_GAME);

#ifdef _WIN32
	OutputDebugString("ShutdownGame() was called.\n");
	_CrtMemDumpAllObjectsSince(&startup1);
	_CrtMemDumpStatistics (&startup1);
	OutputDebugString("Leak dump begin.\n");
	_CrtDumpMemoryLeaks();
	OutputDebugString("Leak dump end.\n");
#endif
}


/*
=================
GetGameAPI

Returns a pointer to the structure with all entry points
and global variables
=================
*/
game_export_t *GetGameAPI (game_import_t *import)
{
	gi = *import;

	// HACK
	oldmodelindex = gi.modelindex;
	gi.modelindex = HACK_modelindex;

	globals.apiversion = GAME_API_VERSION;
	globals.Init = InitGame;
	globals.Shutdown = ShutdownGame;
	globals.SpawnEntities = HACK_SpawnEntities;

	globals.WriteGame = WriteGame;
	globals.ReadGame = ReadGame;
	globals.WriteLevel = WriteLevel;
	globals.ReadLevel = ReadLevel;

	globals.ClientThink = ClientThink;
	globals.ClientConnect = ClientConnect;
	globals.ClientUserinfoChanged = ClientUserinfoChanged;
	globals.ClientDisconnect = ClientDisconnect;
	globals.ClientBegin = ClientBegin;
	globals.ClientCommand = ClientCommand;

	globals.RunFrame = G_RunFrame;

	globals.ServerCommand = ServerCommand;

	globals.edict_size = sizeof(edict_t);

	return &globals;
}

#ifndef GAME_HARD_LINKED
// this is only here so the functions in q_shared.c and q_shwin.c can link
void Sys_Error (char *error, ...)
{
	va_list		argptr;
	char		text[1024];

	va_start (argptr, error);
	vsprintf (text, error, argptr);
	va_end (argptr);

	gi.error (ERR_FATAL, "%s", text);
}

void Com_Printf (char *msg, ...)
{
	va_list		argptr;
	char		text[1024];

	va_start (argptr, msg);
	vsprintf (text, msg, argptr);
	va_end (argptr);

	gi.dprintf ("%s", text);
}

#endif

//======================================================================


/*
=================
ClientEndServerFrames
=================
*/
void ClientEndServerFrames (void)
{
	int		i;
	edict_t	*ent;

	// calc the player views now that all pushing
	// and damage has been added
	for (i=0 ; i < maxclients->value ; i++)
	{
		ent = g_edicts + 1 + i;
		if (!ent->inuse || !ent->client)
			continue;
		ClientEndServerFrame (ent);
	}
}

/*
=================
CreateTargetChangeLevel

Returns the created target changelevel
=================
*/
edict_t *CreateTargetChangeLevel(char *map)
{
	edict_t *ent;

	ent = G_Spawn ();
	ent->classname = "target_changelevel";
	Com_sprintf(level.nextmap, sizeof(level.nextmap), "%s", map);
	ent->map = level.nextmap;
	return ent;
}

/*
=================
EndDMLevel

The timelimit or fraglimit has been exceeded
=================
*/
void EndDMLevel (void)
{
	edict_t	*ent;
	
	ARLog_Stats("END: %s\n", level.mapname); //QW// For ARSG
	
	// go to round 2
	if (ctf->value && TeamplayCheckRound1())
	{
		TeamplayStartRound2();
		return;
	}
	
	// stay on same level flag
	else if ((int)dmflags->value & DF_SAME_LEVEL)
		ent = CreateTargetChangeLevel(level.mapname);
	
	// get the next one out of the maplist
	else if (Maplist_Next())
		ent = CreateTargetChangeLevel(level.nextmap);
	
	// go to a specific map
	else if (level.nextmap[0])
		ent = CreateTargetChangeLevel(level.nextmap);
	
	// search for a changelevel within the current map.
	// this is for the single-player or coop game.
	else
	{
		ent = G_Find (NULL, FOFS(classname), "target_changelevel");
		if (!ent)
			// the map designer didn't include a changelevel,
			// so create a fake ent that goes back to the same level
			ent = CreateTargetChangeLevel(level.mapname);
	}
	
	Log_Time();	//QW// post the time & date to the log
	if (logstats->value)
		LogPlayerStats();
	
	gi.dprintf ("Map is changing to %s\n", level.nextmap);

	ARLog_Stats("START: %s\n", level.nextmap);

	Voting_KillVoting();	// in case we had a vote in progress at the end of level

	BeginIntermission (ent);
}


// Prints a human readable summary of player statistics
//  in the log at the end of the level. //QW//
void LogPlayerStats(void)
{
	
	int	i;
	int j[4] = { 0 };
	int	count = 0;
	edict_t	*player;
	int time_in = 0;
	
	player = g_edicts;
	gi.dprintf("-------------------------------------------\n");
	gi.dprintf("Level Name: %s\n",level.level_name);
	gi.dprintf("Map name: %s\n",level.mapname);
	gi.dprintf("Fraglimit: %s\n",fraglimit->string);
	gi.dprintf("Timelimit: %s\n",timelimit->string);
	
	for (i=0 ; i < game.maxclients ; i++)
	{
		// move to player edict
		player++;
		if (!player->inuse)
			continue;
		count++;
		time_in = 1 + (level.framenum - player->client->resp.enterframe)/10; // +1 sec. fudge
		j[0] = player->client->pers.ipAddr & 0xff;
		j[1] = (player->client->pers.ipAddr & 0xff00) / 0x100;
		j[2] = (player->client->pers.ipAddr & 0xff0000) / 0x10000;
		j[3] = (player->client->pers.ipAddr & 0xff000000) / 0x1000000;
		gi.dprintf("%d:  Player: %s Deaths: %d Score: %d\n    IP address: %d.%d.%d.%d\n    Team: %d Ping: %d FPH: %d\n    Time in map: %i secs.\n",
			i + 1, player->client->pers.netname,
			player->client->resp.deaths,
			player->client->resp.score,
			j[0],j[1],j[2],j[3],
			player->client->resp.ctf_team,
			player->client->ping,
			player->client->resp.fph,
			time_in);
	}
	gi.dprintf("Total players: %i\n",count);
	gi.dprintf("-------------------------------------------\n");
	
	// Do it again in ARSG format
	player = g_edicts;
	for (i=0; i < game.maxclients; i++)
	{
		player++;
		if (!player->inuse)
			continue;
		ARLog_Stats("STAT: %s\\score\\%d\\deaths\\%d\\fph\\%d\\team\\%d\\regenhealth\\%d\\vampihealth\\%d\\organicarmor\\%d\n",
			player->client->pers.netname,
			player->client->resp.score,
			player->client->resp.deaths,
			player->client->resp.fph,
			player->client->resp.ctf_team,
			player->client->regenhealth,
			player->client->vampihealth,
			player->client->organicarmor);
	}
}

/*
=================
CheckNeedPass
=================
*/
void CheckNeedPass (void)
{
	int need;

	// if password or spectator_password has changed, update needpass
	// as needed
	if (password->modified || spectator_password->modified) 
	{
		password->modified = spectator_password->modified = false;

		need = 0;

		if (*password->string && Q_stricmp(password->string, "none"))
			need |= 1;
		if (*spectator_password->string && Q_stricmp(spectator_password->string, "none"))
			need |= 2;

		gi.cvar_set("needpass", va("%d", need));
	}
}

/*
=================
CheckDMRules
Called once every server frame by G_RunFrame
=================
*/
void CheckDMRules (void)
{
	int			i;
	gclient_t	*cl;

	if (level.intermissiontime)
		return;

	if (!deathmatch->value)
		return;
	
	Voting_CheckVoting();
	UpdateBans();

	if (ctf->value)
	{
		char *winner;

		winner = CTFCheckRules();
		if (winner)
		{
			if (TeamplayCheckRound1())
				gi.bprintf (PRINT_HIGH, "The %s team wins Round 1.\n", winner);
			else
				gi.bprintf (PRINT_HIGH, "Game over.  The %s team wins.\n", winner);
			EndDMLevel ();
			return;
		}
	}

	if (timelimit->value)
	{
		float timelimitvalue = timelimit->value * 60 + level.roundTimelimit;

		if (level.time >= timelimitvalue)
		{
			gi.bprintf (PRINT_HIGH, "Timelimit hit.\n");
			EndDMLevel ();
			return;
		}
	}

	if (fraglimit->value)
	{
		int fraglimitvalue = (int) (fraglimit->value + level.roundFraglimit);

		for (i=0 ; i < maxclients->value ; i++)
		{
			cl = game.clients + i;
			if (!g_edicts[i+1].inuse)
				continue;

			if (cl->resp.score >= fraglimitvalue)
			{
				gi.bprintf (PRINT_HIGH, "Fraglimit hit.\n");
				EndDMLevel ();
				return;
			}
		}
	}
}


/*
=============
ExitLevel
=============
*/
void ExitLevel (void)
{
	int		i;
	edict_t	*ent;
	char	command [256];

	Com_sprintf (command, sizeof(command), "gamemap \"%s\"\n", level.changemap);
	gi.AddCommandString (command);
	level.changemap = NULL;
	level.exitintermission = 0;
	level.intermissiontime = 0;
	ClientEndServerFrames ();

	// clear some things before going to next level
	for (i=0 ; i < maxclients->value ; i++)
	{
		ent = g_edicts + 1 + i;
		if (!ent->inuse)
			continue;
		if (ent->health > ent->client->pers.max_health)
			ent->health = ent->client->pers.max_health;
	}

	CTFInit();
	MatchInit();

	// Forcibly rebalance the teams after each game.
	if (ctf->value && ((int)dmflags->value & DF_TEAMREBALANCE))
		TeamplayRebalanceTeams();
}

int CountConnectedClients (void)
{
	int n, count;
	edict_t *player;

	count = 0;
	for (n = 1; n <= maxclients->value; n++)
	{
		player = &g_edicts[n];
		if (!player->inuse)
			continue;
		else
			count++;
	}
	return count;
}

/*
================
G_RunFrame

Advances the world by 0.1 seconds
================
*/
void G_RunFrame (void)
{
	int		i;
	edict_t	*ent;
	
	//START_PERFORMANCE_TIMER;
	
	DayNightCycle();	// execute the day/night cycle if enabled
	
	if (level.framenum % 10 == 0)	// every 10th frame do housekeeping
	{
		if (CheckLocalMidnight())
		{
			//rename statistics log file and console log on schedule at midnight
			RenameStatsLog();
			RenameConsoleLog();
		}
	}
	
	level.framenum++;
	level.time = level.framenum * FRAMETIME;
	
	// choose a client for monsters to target this frame
	AI_SetSightClient ();
	
	if (level.intermissiontime && (level.intermissiontime < (level.time - scoreboardtime->value)))
	{
		// exit intermissions only if we have clients
		if (CountConnectedClients())
			level.exitintermission = 1;
	}
	
	// exit intermissions
	if (level.exitintermission)
	{
		gi.dprintf("Peak edicts usage: %i\n", game.peak_edicts);
		ExitLevel ();
		return;
	}
	
	//
	// treat each object in turn
	// even the world gets a chance to think
	//
	ent = &g_edicts[0];
	for (i = 0 ; i < globals.num_edicts ; i++, ent++)
	{
		if (!ent->inuse)
			continue;
		
		level.current_entity = ent;
		
		VectorCopy (ent->s.origin, ent->s.old_origin);
		
		// if the ground entity moved, make sure we are still on it
		if ((ent->groundentity) && (ent->groundentity->linkcount != ent->groundentity_linkcount))
		{
			ent->groundentity = NULL;
			if ( !(ent->flags & (FL_SWIM|FL_FLY)) && (ent->svflags & SVF_MONSTER) )
			{
				M_CheckGround (ent);
			}
		}
		
		if (i > 0 && i <= maxclients->value)
		{
			ClientBeginServerFrame (ent);
				continue; //QW// from Paril, comment out for wall-hider detection
		}
		
		G_RunEntity (ent);
	}
	
	if (ctf->value)
	{
		// continue the countdown
		TeamplayDoCountdown();
		// see if any ghost-players can be respawned now.
		TeamplayCheckRespawn();
	}
	
	if (matchplay->value)
	{
		// continue the countdown
		MatchplayDoCountdown();
	}
	
//QW//	HighScorer();

	// see if it is time to end a deathmatch
	CheckDMRules ();
	// see if needpass needs updating
	CheckNeedPass ();
	// build the playerstate_t structures for all players
	ClientEndServerFrames ();
	
	//_STOP_PERFORMANCE_TIMER(__func__);
}

// HACK
char modelSeen[256];

int HACK_modelindex (char *name)
{
	int result;

	// First, get the result of gi.modelindex().
	result = oldmodelindex (name);

	// Log some "*Index: overflow" bugcatching info to the console, if
	// they're using the maplist feature on a dedicated DM server -

	//QW//
	// - and debugmodels is on.
	// The purpose of this hack is to allow debugging of the overflow bug of the 256 model
	// index limit in Q2. It starts the next level if we start to exceed the model limit.
	// I hacked the hack to allow the server configuration to control whether we see the
	// debug messages on the console or not.
	// The map city3.bsp hits the model limit every time even with a single player on a server.
	//QW//

	if (deathmatch->value && dedicated->value && maplist->value)
	{

		if (result >= MAX_MODELS - 1)
		{
			gi.bprintf (PRINT_HIGH, "Forced Timelimit. Model index = %d\n", result);
			EndDMLevel();
		}

		// If this model hasn't been seen before, report it & its number.
		if (!modelSeen[result])
		{
			modelSeen[result] = true;
			if (debugmodels->value) 
				gi.dprintf ("modelindex %d allocated to %s\n", result, name);
		}
	}

	// Return the model index.
	return result;
}

void
HACK_SpawnEntities (char *mapname, char *entities, char *spawnpoint)
{
	// Clear modelSeen[]
	memset (modelSeen, 0, sizeof (modelSeen));

	// Proceed.
	TeamplaySpawnEntities (mapname, entities, spawnpoint);
}
