
#include "g_local.h"
#include "g_team.h"
#include "scanner.h"
#include "l_ar_admin.h"

typedef struct spawn_s
{
	char* name;  //the name of the entity
	void	(*spawn)(edict_t* ent); // pointer to its spawn function
} spawn_t;

void SP_item_health(edict_t* self);
void SP_item_health_small(edict_t* self);
void SP_item_health_large(edict_t* self);
void SP_item_health_mega(edict_t* self);

void SP_info_player_start(edict_t* ent);
void SP_info_player_deathmatch(edict_t* ent);
void SP_info_player_coop(edict_t* ent);
void SP_info_player_intermission(edict_t* ent);

void SP_func_plat(edict_t* ent);
void SP_func_rotating(edict_t* ent);
void SP_func_button(edict_t* ent);
void SP_func_door(edict_t* ent);
void SP_func_door_secret(edict_t* ent);
void SP_func_door_rotating(edict_t* ent);
void SP_func_water(edict_t* ent);
void SP_func_train(edict_t* ent);
void SP_func_conveyor(edict_t* self);
void SP_func_wall(edict_t* self);
void SP_func_object(edict_t* self);
void SP_func_explosive(edict_t* self);
void SP_func_timer(edict_t* self);
void SP_func_areaportal(edict_t* ent);
void SP_func_clock(edict_t* ent);
void SP_func_killbox(edict_t* ent);

void SP_trigger_always(edict_t* ent);
void SP_trigger_once(edict_t* ent);
void SP_trigger_multiple(edict_t* ent);
void SP_trigger_relay(edict_t* ent);
void SP_trigger_push(edict_t* ent);
void SP_trigger_hurt(edict_t* ent);
void SP_trigger_key(edict_t* ent);
void SP_trigger_counter(edict_t* ent);
void SP_trigger_elevator(edict_t* ent);
void SP_trigger_gravity(edict_t* ent);
void SP_trigger_monsterjump(edict_t* ent);

void SP_target_temp_entity(edict_t* ent);
void SP_target_speaker(edict_t* ent);
void SP_target_explosion(edict_t* ent);
void SP_target_changelevel(edict_t* ent);
void SP_target_secret(edict_t* ent);
void SP_target_goal(edict_t* ent);
void SP_target_splash(edict_t* ent);
void SP_target_spawner(edict_t* ent);
void SP_target_blaster(edict_t* ent);
void SP_target_crosslevel_trigger(edict_t* ent);
void SP_target_crosslevel_target(edict_t* ent);
void SP_target_laser(edict_t* self);
void SP_target_help(edict_t* ent);
void SP_target_actor(edict_t* ent);
void SP_target_lightramp(edict_t* self);
void SP_target_earthquake(edict_t* ent);
void SP_target_character(edict_t* ent);
void SP_target_string(edict_t* ent);

void SP_worldspawn(edict_t* ent);
void SP_viewthing(edict_t* ent);

void SP_light(edict_t* self);
void SP_light_mine1(edict_t* ent);
void SP_light_mine2(edict_t* ent);
void SP_info_null(edict_t* self);
void SP_info_notnull(edict_t* self);
void SP_path_corner(edict_t* self);
void SP_point_combat(edict_t* self);

void SP_misc_explobox(edict_t* self);
void SP_misc_banner(edict_t* self);
void SP_misc_banner1(edict_t* self);
void SP_misc_banner2(edict_t* self);
void SP_misc_satellite_dish(edict_t* self);
void SP_misc_actor(edict_t* self);
void SP_misc_gib_arm(edict_t* self);
void SP_misc_gib_leg(edict_t* self);
void SP_misc_gib_head(edict_t* self);
void SP_misc_insane(edict_t* self);
void SP_misc_deadsoldier(edict_t* self);
void SP_misc_viper(edict_t* self);
void SP_misc_viper_bomb(edict_t* self);
void SP_misc_bigviper(edict_t* self);
void SP_misc_strogg_ship(edict_t* self);
void SP_misc_teleporter(edict_t* self);
void SP_misc_teleporter_dest(edict_t* self);
void SP_misc_teleporter_offworld(edict_t* self);
void SP_misc_blackhole(edict_t* self);
void SP_misc_eastertank(edict_t* self);
void SP_misc_easterchick(edict_t* self);
void SP_misc_easterchick2(edict_t* self);

void SP_monster_berserk(edict_t* self);
void SP_monster_gladiator(edict_t* self);
void SP_monster_gunner(edict_t* self);
void SP_monster_infantry(edict_t* self);
void SP_monster_soldier_light(edict_t* self);
void SP_monster_soldier(edict_t* self);
void SP_monster_soldier_ss(edict_t* self);
void SP_monster_tank(edict_t* self);
void SP_monster_medic(edict_t* self);
void SP_monster_flipper(edict_t* self);
void SP_monster_chick(edict_t* self);
void SP_monster_parasite(edict_t* self);
void SP_monster_flyer(edict_t* self);
void SP_monster_brain(edict_t* self);
void SP_monster_floater(edict_t* self);
void SP_monster_hover(edict_t* self);
void SP_monster_mutant(edict_t* self);
void SP_monster_supertank(edict_t* self);
void SP_monster_boss2(edict_t* self);
void SP_monster_jorg(edict_t* self);
void SP_monster_boss3_stand(edict_t* self);

void SP_monster_commander_body(edict_t* self);

void SP_turret_breach(edict_t* self);
void SP_turret_base(edict_t* self);
void SP_turret_driver(edict_t* self);


spawn_t	spawns[] = {
	{"item_health", SP_item_health},
	{"item_health_small", SP_item_health_small},
	{"item_health_large", SP_item_health_large},
	{"item_health_mega", SP_item_health_mega},

	{"info_player_start", SP_info_player_start},
	{"info_player_deathmatch", SP_info_player_deathmatch},
	{"info_player_coop", SP_info_player_coop},
	{"info_player_intermission", SP_info_player_intermission},
	//ZOID
	{"info_player_team1", SP_info_player_team1},
	{"info_player_team2", SP_info_player_team2},
	//ZOID

	{"func_plat", SP_func_plat},
	{"func_button", SP_func_button},
	{"func_door", SP_func_door},
	{"func_door_secret", SP_func_door_secret},
	{"func_door_rotating", SP_func_door_rotating},
	{"func_rotating", SP_func_rotating},
	{"func_train", SP_func_train},
	{"func_water", SP_func_water},
	{"func_conveyor", SP_func_conveyor},
	{"func_areaportal", SP_func_areaportal},
	{"func_clock", SP_func_clock},
	{"func_wall", SP_func_wall},
	{"func_object", SP_func_object},
	{"func_timer", SP_func_timer},
	{"func_explosive", SP_func_explosive},
	{"func_killbox", SP_func_killbox},

	{"trigger_always", SP_trigger_always},
	{"trigger_once", SP_trigger_once},
	{"trigger_multiple", SP_trigger_multiple},
	{"trigger_relay", SP_trigger_relay},
	{"trigger_push", SP_trigger_push},
	{"trigger_hurt", SP_trigger_hurt},
	{"trigger_key", SP_trigger_key},
	{"trigger_counter", SP_trigger_counter},
	{"trigger_elevator", SP_trigger_elevator},
	{"trigger_gravity", SP_trigger_gravity},
	{"trigger_monsterjump", SP_trigger_monsterjump},

	{"target_temp_entity", SP_target_temp_entity},
	{"target_speaker", SP_target_speaker},
	{"target_explosion", SP_target_explosion},
	{"target_changelevel", SP_target_changelevel},
	{"target_secret", SP_target_secret},
	{"target_goal", SP_target_goal},
	{"target_splash", SP_target_splash},
	{"target_spawner", SP_target_spawner},
	{"target_blaster", SP_target_blaster},
	{"target_crosslevel_trigger", SP_target_crosslevel_trigger},
	{"target_crosslevel_target", SP_target_crosslevel_target},
	{"target_laser", SP_target_laser},
	{"target_help", SP_target_help},
	{"target_actor", SP_target_actor},
	{"target_lightramp", SP_target_lightramp},
	{"target_earthquake", SP_target_earthquake},
	{"target_character", SP_target_character},
	{"target_string", SP_target_string},

	{"worldspawn", SP_worldspawn},
	{"viewthing", SP_viewthing},

	{"light", SP_light},
	{"light_mine1", SP_light_mine1},
	{"light_mine2", SP_light_mine2},
	{"info_null", SP_info_null},
	{"func_group", SP_info_null},
	{"info_notnull", SP_info_notnull},
	{"path_corner", SP_path_corner},
	{"point_combat", SP_point_combat},

	{"misc_explobox", SP_misc_explobox},
	{"misc_banner", SP_misc_banner},
	{"misc_banner1", SP_misc_banner1},
	{"misc_banner2", SP_misc_banner2},
	//ZOID
	{"misc_ctf_banner", SP_misc_ctf_banner},
	{"misc_ctf_small_banner", SP_misc_ctf_small_banner},
	//ZOID
	{"misc_satellite_dish", SP_misc_satellite_dish},
	{"misc_actor", SP_misc_actor},
	{"misc_gib_arm", SP_misc_gib_arm},
	{"misc_gib_leg", SP_misc_gib_leg},
	{"misc_gib_head", SP_misc_gib_head},
	{"misc_insane", SP_misc_insane},
	{"misc_deadsoldier", SP_misc_deadsoldier},
	{"misc_viper", SP_misc_viper},
	{"misc_viper_bomb", SP_misc_viper_bomb},
	{"misc_bigviper", SP_misc_bigviper},
	{"misc_strogg_ship", SP_misc_strogg_ship},
	{"misc_teleporter", SP_misc_teleporter},
	{"misc_teleporter_dest", SP_misc_teleporter_dest},
	{"misc_offworld_teleporter", SP_misc_teleporter_offworld},
	//ZOID
	{"trigger_teleport", SP_trigger_teleport},
	{"info_teleport_destination", SP_info_teleport_destination},
	//ZOID
	{"misc_blackhole", SP_misc_blackhole},
	{"misc_eastertank", SP_misc_eastertank},
	{"misc_easterchick", SP_misc_easterchick},
	{"misc_easterchick2", SP_misc_easterchick2},

	{"monster_berserk", SP_monster_berserk},
	{"monster_gladiator", SP_monster_gladiator},
	{"monster_gunner", SP_monster_gunner},
	{"monster_infantry", SP_monster_infantry},
	{"monster_soldier_light", SP_monster_soldier_light},
	{"monster_soldier", SP_monster_soldier},
	{"monster_soldier_ss", SP_monster_soldier_ss},
	{"monster_tank", SP_monster_tank},
	{"monster_tank_commander", SP_monster_tank},
	{"monster_medic", SP_monster_medic},
	{"monster_flipper", SP_monster_flipper},
	{"monster_chick", SP_monster_chick},
	{"monster_parasite", SP_monster_parasite},
	{"monster_flyer", SP_monster_flyer},
	{"monster_brain", SP_monster_brain},
	{"monster_floater", SP_monster_floater},
	{"monster_hover", SP_monster_hover},
	{"monster_mutant", SP_monster_mutant},
	{"monster_supertank", SP_monster_supertank},
	{"monster_boss2", SP_monster_boss2},
	{"monster_boss3_stand", SP_monster_boss3_stand},
	{"monster_jorg", SP_monster_jorg},

	{"monster_commander_body", SP_monster_commander_body},

	{"turret_breach", SP_turret_breach},
	{"turret_base", SP_turret_base},
	{"turret_driver", SP_turret_driver},

	{NULL, NULL}
};

/*
===============
* ED_CallSpawn
*
* Finds the spawn function for the entity and calls it
===============
*/
void ED_CallSpawn(edict_t* ent)
{
	spawn_t* s;
	gitem_t* item;

	if (!ent->classname)
	{
		gi.dprintf("%s: NULL classname\n", __func__);
		return;
	}

	// check item spawn functions
	item = FindItemByClassname(ent->classname);
	if (item != NULL)
	{	// found it
		SpawnItem(ent, item);
		return;
	}

	// check normal spawn functions
	for (s = spawns; s->name; s++)
	{
		if (!strcmp(s->name, ent->classname))
		{	// found it
			s->spawn(ent);
			return;
		}
	}
	gi.dprintf("\"%s\" doesn't have a spawn function\n", ent->classname);
}

/*
=============
ED_NewString
=============
*/
char* ED_NewString(char* string)
{
	char* newb, * new_p;
	int		i, length;

	length = (int)strlen(string) + 1;

	newb = gi.TagMalloc(length, TAG_LEVEL);

	new_p = newb;

	for (i = 0; i < length; i++)
	{
		if (string[i] == '\\' && i < length - 1)
		{
			i++;
			if (string[i] == 'n')
				*new_p++ = '\n';
			else
				*new_p++ = '\\';
		}
		else
			*new_p++ = string[i];
	}

	return newb;
}




/*
===============
* ED_ParseField
*
* Takes a key/value pair and sets the binary values
* in an edict
===============
*/
void ED_ParseField(char* key, char* value, edict_t* ent)
{
	field_t* f;
	byte* b;
	float	v;
	vec3_t	vec = { 0 };

	for (f = fields; f->name; f++)
	{
		if (!Q_stricmp(f->name, key))
		{	// found it
			if (f->flags & FFL_SPAWNTEMP)
				b = (byte*)&st;
			else
				b = (byte*)ent;

			switch (f->type)
			{
			case F_LSTRING:
				*(char**)(b + f->ofs) = ED_NewString(value);
				break;
			case F_VECTOR:
				if (sscanf(value, "%f %f %f", &vec[0], &vec[1], &vec[2]) != 3) {
					gi.dprintf("WARNING: Vector field incomplete in %s, map: %s, field: %s\n", __func__, level.mapname, f->name);
				}
				((float*)(b + f->ofs))[0] = vec[0];
				((float*)(b + f->ofs))[1] = vec[1];
				((float*)(b + f->ofs))[2] = vec[2];
				break;
			case F_INT:
				*(int*)(b + f->ofs) = atoi(value);
				break;
			case F_FLOAT:
				*(float*)(b + f->ofs) = atof(value);
				break;
			case F_ANGLEHACK:
				v = atof(value);
				((float*)(b + f->ofs))[0] = 0;
				((float*)(b + f->ofs))[1] = v;
				((float*)(b + f->ofs))[2] = 0;
				break;
			case F_IGNORE:
				break;
			default:
				break;
			}
			return;
		}
	}
	gi.dprintf("%s: %s is not a field\n", __func__, key);
}

/*
* ====================
* ED_ParseEdict
*
* Parses an edict out of the given string, returning the new position
* ed should be a properly initialized empty edict.
* ====================
*/
char* ED_ParseEdict(char* data, edict_t* ent)
{
	qboolean	init;
	char		keyname[256];
	char* com_token;

	init = false;
	memset(&st, 0, sizeof(st));

	// go through all the dictionary pairs
	while (1)
	{
		// parse key
		com_token = COM_Parse(&data);
		if (com_token[0] == '}')
			break;
		if (!data)
			GameError("%s: EOF without closing brace", __func__);

		strncpy(keyname, com_token, sizeof(keyname) - 1);

		// parse value	
		com_token = COM_Parse(&data);
		if (!data)
			GameError("%s: EOF without closing brace", __func__);

		if (com_token[0] == '}')
			GameError("%s: closing brace without data", __func__);

		init = true;

		// keynames with a leading underscore are used for utility comments,
		// and are immediately discarded by quake
		if (keyname[0] == '_')
			continue;

		ED_ParseField(keyname, com_token, ent);
	}

	if (!init)
		memset(ent, 0, sizeof(*ent));

	return data;
}


/*
* ================
* G_FindTeams
*
* Chain together all entities with a matching team field.
*
* All but the first will have the FL_TEAMSLAVE flag set.
* All but the last will have the teamchain field set to the next one
* ================
*/
void G_FindTeams(void)
{
	edict_t* e, * e2, * chain;
	int		i, j;
	int		c, c2;

	c = 0;
	c2 = 0;
	for (i = 1, e = g_edicts + i; i < globals.num_edicts; i++, e++)
	{
		if (!e->inuse)
			continue;
		if (!e->team)
			continue;
		if (e->flags & FL_TEAMSLAVE)
			continue;
		chain = e;
		e->teammaster = e;
		c++;
		c2++;
		for (j = i + 1, e2 = e + 1; j < globals.num_edicts; j++, e2++)
		{
			if (!e2->inuse)
				continue;
			if (!e2->team)
				continue;
			if (e2->flags & FL_TEAMSLAVE)
				continue;
			if (!strcmp(e->team, e2->team))
			{
				c2++;
				chain->teamchain = e2;
				e2->teammaster = e;
				chain = e2;
				e2->flags |= FL_TEAMSLAVE;
			}
		}
	}

	gi.dprintf("%i teams with %i entities\n", c, c2);
}

// Define this to make SpawnEntities() output a version of the read entities
// containing everything that wasn't freed.  (A game-development thing.)
//#define DUMPENTS

/*
* ==============
* SpawnEntities
*
* Creates a server's entity / program execution context by
* parsing textual entity definitions out of an ent file.
* ==============
*/
void SpawnEntities(char* mapname, char* entities, char* spawnpoint)
{
	edict_t* ent;
	int      inhibit;
	char* com_token;
	int      i;
	float    skill_level;

	// for the dumpents feature
	char* entstart = NULL;
	FILE* f = NULL;
	char szFile[MAX_QPATH];

	if (dumpents->value)
	{
		// Create the pathname to the new entity file.
		Com_sprintf(szFile, sizeof(szFile), "%s/ent/new-%s.ent", gamedir->string, mapname);

		// Try to open it.
		f = fopen(szFile, "wb");
		if (!f)
			GameError("%s DUMPENTS: couldn't open %s for writing\n", __func__, szFile);
	}

	skill_level = floor(skill->value);
	if (skill_level < 0)
		skill_level = 0;
	if (skill_level > 3)
		skill_level = 3;
	if (skill->value != skill_level)
		gi.cvar_forceset("skill", va("%f", skill_level));

	SaveClientData();

	gi.FreeTags(TAG_LEVEL);

	memset(&level, 0, sizeof(level));
	memset(g_edicts, 0, game.maxentities * sizeof(g_edicts[0]));

	strncpy(level.mapname, mapname, sizeof(level.mapname) - 1);
	strncpy(game.spawnpoint, spawnpoint, sizeof(game.spawnpoint) - 1);

	// set client fields on player ents
	for (i = 0; i < game.maxclients; i++)
		g_edicts[i + 1].client = game.clients + i;

	ent = NULL;
	inhibit = 0;

	//QW// custom_ents is automatically 1 in CTF mode.
	if (custom_ents->value)
		entities = LoadEntFile(mapname, entities);

	// parse ents
	while (1)
	{
		// keep track of where the entity started.
		entstart = entities;

		// parse the opening brace	
		com_token = COM_Parse(&entities);
		if (!entities)
			break;
		if (com_token[0] != '{') {
			GameError("LOX %s: found %s when expecting {", __func__, com_token);
		}

		if (!ent)
			ent = g_edicts;
		else
			ent = G_Spawn();

		entities = ED_ParseEdict(entities, ent);

		// yet another map hack
		if (ent && !Q_stricmp(level.mapname, "command") &&
			!Q_stricmp(ent->classname, "trigger_once") &&
			!Q_stricmp(ent->model, "*27"))
			ent->spawnflags &= ~SPAWNFLAG_NOT_HARD;

		// remove things (except the world) from different skill levels or deathmatch
		if (ent != g_edicts)
		{
			if (deathmatch->value)
			{
				if (ent->spawnflags & SPAWNFLAG_NOT_DEATHMATCH)
				{
					G_FreeEdict(ent);
					inhibit++;
					continue;
				}
			}
			else
			{
				if ( /* ((coop->value) && (ent->spawnflags & SPAWNFLAG_NOT_COOP)) || */
					((skill->value == 0) && (ent->spawnflags & SPAWNFLAG_NOT_EASY)) ||
					((skill->value == 1) && (ent->spawnflags & SPAWNFLAG_NOT_MEDIUM)) ||
					(((skill->value == 2) || (skill->value == 3)) && (ent->spawnflags & SPAWNFLAG_NOT_HARD))
					)
				{
					G_FreeEdict(ent);
					inhibit++;
					continue;
				}
			}

			ent->spawnflags &= ~(SPAWNFLAG_NOT_EASY | SPAWNFLAG_NOT_MEDIUM | SPAWNFLAG_NOT_HARD | SPAWNFLAG_NOT_COOP | SPAWNFLAG_NOT_DEATHMATCH);
		}

		ED_CallSpawn(ent);

		// If this entity survived, print it.
		if (dumpents->value && ent->inuse)
		{
			int entsize;
			int nWritten;

			// Move past the end of the line.
			while (*entities == 12 || *entities == 15)
				entities++;

			// Calculate the size of the entity, in characters.
			entsize = entities - entstart;

			// Write it.
			nWritten = (int)fwrite(entstart, sizeof(char), entsize, f);
			if (nWritten != entsize)
				GameError("SpawnEntities DUMPENTS: couldn't write to file\n");
		}
	}

	if (dumpents->value)
		fclose(f);

	gi.dprintf("%i entities inhibited\n", inhibit);

	G_FindTeams();

	PlayerTrail_Init();
	ARLog_Stats("MAP: %s\n", level.mapname);

}


// ===================================================================
// Unified HUD initialization by QwazyWabbit
// ===================================================================

// cursor positioning
//  xl <value>      x-left side
//  xr <value>      x-right side
//  yb <value>      y-bottom
//  yt <value>      y-top
//  xv <value>      x-value
//  yv <value>      y-value

// drawing
//  statpic <name>
//  pic <stat>                Draw a pic from a stat number
//  picn <name>               Draw a pic from a name string
//  num <fieldwidth> <stat>   Print a number of specified width digits
//  anum <stat>               Print the ammo count, 3 digits max, blink if low ammo
//  hnum <stat>               Print the health count, 3 digits max, blink if low health
//  rnum <stat>               Print the armor count, 3 digits max, blink if low armor

//  client <x> <y> <index> <score> <ping> <time>    Draw a deathmatch client score block
//  cstring <string> <x> <y>    Print a centered string
//  cstring2 <string> <x> <y>   Print a highlighted centered string
//  string <x> <y> <string>     Print a string at x,y
//  string2 <x> <y> <string>    Print a highlighted string
//  stat_string <stat>          Print a string based on index stat from client's configstrings
//                              Used to display the item player just picked up.
//
// control
//  if <stat>               conditionally draw a number
//  ifeq <stat> <value>     not implemented
//  ifbit <stat> <value>    not implemented
//  endif                   ends an "if" block

// //QW//
// In all cases <stat> is the integer representing the status message item
// to be presented. I've used manifest constants to keep the messages
// properly enumerated across the game modes and they are defined in
// q_shared.h. The code originally had these scattered about in
// different places but it mostly used magic numbers in the messages and that just
// made it hard to figure out exactly what was happening. They defined the constants
// and then didn't use them. Later authors were overlapping them in their
// mods or pulling in overlapping stat items from other mods and this creates
// HUD conflicts. ENUM or #DEFINE, then use them. Please.

// Positioning:
// Position 0,0 is the center of the screen with x going negative to the right.
// Positioning is based on a 640 x 480 screen, scaled to the actual video
// width and height of the player's current screen resolution.
// xv, yv is relative to the center origin.
// xl is plus counts from origin left justified.
// xr is minus counts from origin right justified.
// yb is minus counts from bottom. (yb -10 is bottom line)
// yt is plus counts from 0 at top.

/* //QW//
A standard HUD character (conchar) is 8 screen units wide. The xl and xr origins are with
respect to the left and right borders. Add/subract 2 units to keep a little space between
the edge and the chars. For example, the string "FPH" is 3 chars wide, 3 * 8 = 24 units.
If using xr, add 2 and negate, giving -26 from the right edge as the start of the
string on the screen, "Range" is (-1)(5 * 8 + 2) = -42.
Using xl, just use an origin of 2 to space a string 2 units from the border so it looks nice.
*/

// //QW//
// The big HUD characters for the counters are 16 units wide but
// their origin is already offset by 2.
// They are positioned at 0 on the left and at (n * 16)-2 on the right
// when n is the number of digits you want to display.
//
// Printing big chars on the left is problematic, they are right-justfied inside their
// block so printing them on the left will gap them from the edge when the value doesn't
// fill the full range of digits. Negative signs will probably be clipped if you don't
// allow an extra space for them.
// You should layout for signed values on the right side or midline where negative
// values aren't a problem.
// Layout for unsigned on the left if you don't mind having right-justfied
// blocks there. Allow space only for the number of digits you expect to
// display to keep the HUD packet as small as possible.

// I think I first saw an integrated HUD as a function in the CTC mod.
// I took it a little further by integrating the game modes and collecting
// the stat constants in one place.

/* //QW//
Big HUD chars are 24 units tall and conchars are 8 for a total of 32 plus
vertical spacing of 3 units to make it 35 units for a per-line increment
on the big chars. I use a 25 unit vertical offset for the conchars label
below it. This seems to give a nice uniform leading between lines.
*/

// ===================================================================

// This function is executed once in SP_worldspawn
void CreateStatusBar(void)
{
	char Bar[256]; // temporary storage
	size_t s;

	memset(statusbar, 0, sizeof(statusbar)); // the actual status bar

	sprintf(Bar, "if %d yb -24 xv 0 hnum xv 50 pic %d ", STAT_HEALTH_ICON, STAT_HEALTH_ICON);
	strcat(statusbar, Bar);

	sprintf(Bar, "if %d xv 100 anum xv 150 pic %d endif ", STAT_AMMO_ICON, STAT_AMMO_ICON);
	strcat(statusbar, Bar);

	sprintf(Bar, "if %d xv 200 rnum xv 250 pic %d endif ", STAT_ARMOR_ICON, STAT_ARMOR_ICON);
	strcat(statusbar, Bar);

	sprintf(Bar, "if %d xv 296 pic %d endif ", STAT_SELECTED_ICON, STAT_SELECTED_ICON);
	strcat(statusbar, Bar);

	sprintf(Bar, "yb -50 if %d xv 0 pic %d xv 26 yb -42 stat_string %d yb -50 endif ",
		STAT_PICKUP_ICON, STAT_PICKUP_ICON, STAT_PICKUP_STRING);
	strcat(statusbar, Bar);

	// ctf bar had xv at 262, test this //QW//
	sprintf(Bar, "if %d xv 246 num 2 %d xv 296 pic %d endif ",
		STAT_TIMER_ICON, STAT_TIMER, STAT_TIMER_ICON);
	strcat(statusbar, Bar);

	sprintf(Bar, "if %d xv 148 pic %d endif ", STAT_HELPICON, STAT_HELPICON);
	strcat(statusbar, Bar);

	sprintf(Bar, "xr -50 yt 2 num 3 %d ", STAT_FRAGS); //player frag count
	strcat(statusbar, Bar);

	if (ctf->value)
	{
		//red team pad captures
		sprintf(Bar, "yb -102 if %d xr -26 pic %d endif xr -62 num 2 %d ",
			STAT_CTF_TEAM1_PIC, STAT_CTF_TEAM1_PIC, STAT_CTF_TEAM1_CAPS);
		strcat(statusbar, Bar);

		//red joined overlay
		sprintf(Bar, "if %d yb -104 xr -28 pic %d endif ",
			STAT_CTF_JOINED_TEAM1_PIC, STAT_CTF_JOINED_TEAM1_PIC); //player frag count
		strcat(statusbar, Bar);

		//blue team pad captures
		sprintf(Bar, "yb -75 if %d xr -26 pic %d endif xr -62 num 2 %d ",
			STAT_CTF_TEAM2_PIC, STAT_CTF_TEAM2_PIC, STAT_CTF_TEAM2_CAPS);
		strcat(statusbar, Bar);

		//blue joined overlay
		sprintf(Bar, "if %d yb -77 xr -28 pic %d endif ",
			STAT_CTF_JOINED_TEAM2_PIC, STAT_CTF_JOINED_TEAM2_PIC); //player frag count
		strcat(statusbar, Bar);

		//uncaptured spawnpoints
		sprintf(Bar, "if %d yb -50 xr -62 num 2 %d endif ",
			STAT_CTF_NOTCAPPED, STAT_CTF_NOTCAPPED);
		strcat(statusbar, Bar);
	}

	// player ID
	sprintf(Bar, "if %d xv 30 yv 220 stat_string %d endif ", STAT_PLAYER, STAT_PLAYER);
	strcat(statusbar, Bar);

	// player frag per hour
	sprintf(Bar, "if %d xr -66 yt 30 num 4 %d xr -26 yt 55 string FPH endif ", STAT_FPH, STAT_FPH);
	strcat(statusbar, Bar);

	// looks like a score digit is 16 screen units wide.
	// rangefinder (-66)(-42)
	sprintf(Bar, "if %d xr -66 yt 65 num 4 %d xr -42 yt 90 string Range endif ", STAT_RANGEFINDER, STAT_RANGEFINDER);
	strcat(statusbar, Bar);

	// player Height (-66)(-50) lines are spaced 35 units.
	sprintf(Bar, "if %d xr -66 yt 100 num 4 %d xr -50 yt 125 string Height endif ", STAT_HEIGHT, STAT_HEIGHT);
	strcat(statusbar, Bar);

	// player number of TRACKERS deployed
	sprintf(Bar, "if %d xr -34 yt 135 num 2 %d xr -66 yt 160 string Trackers endif ", STAT_NUMTRACKER, STAT_NUMTRACKER);
	strcat(statusbar, Bar);

	// player number of TURRETS deployed
	sprintf(Bar, "if %d xr -34 yt 170 num 2 %d xr -58 yt 195 string Turrets endif ", STAT_NUMTURRET, STAT_NUMTURRET);
	strcat(statusbar, Bar);

	// player location
	sprintf(Bar, "if %d xv 60 yv 140 stat_string %d endif ", STAT_LOCATION, STAT_LOCATION);
	strcat(statusbar, Bar);

	//DbgPrintf("%s size: %lu\n", statusbar, strlen(statusbar));
	// Make sure we don't blow anything out of the water
	s = strlen(statusbar);
	if (s > sizeof(statusbar))
	{
		gi.dprintf("%s: Statusbar too big %d\n", __func__, strlen(statusbar)); //to the log
		statusbar[1023] = 0;
	}
}

// end HUD initialization

// ===================================================================

/*
* QUAKED worldspawn (0 0 0) ?
*
* Only used for the world.
* "sky"	environment map name
* "skyaxis"	vector axis for rotating sky
* "skyrotate"	speed of rotation in degrees/second
* "sounds"	music cd track number
* "gravity"	800 is default gravity
* "message"	text to print at user logon
*/
void SP_worldspawn(edict_t* ent)
{
	ent->movetype = MOVETYPE_PUSH;
	ent->solid = SOLID_BSP;
	ent->inuse = true;			// since the world doesn't use G_Spawn()
	ent->s.modelindex = 1;		// world model is always index 1
	//---------------
	game.statusbar = statusbar;
	// reserve some spots for dead player bodies for coop / deathmatch
	InitBodyQue();

	// set configstrings for items
	SetItemNames();

	if (st.nextmap)
		strncpy(level.nextmap, st.nextmap, sizeof(level.nextmap) - 1);

	// make some data visible to the server

	if (ent->message && ent->message[0])
	{
		gi.configstring(CS_NAME, ent->message);
		strncpy(level.level_name, ent->message, sizeof level.level_name - 1);
	}
	else
		strncpy(level.level_name, level.mapname, sizeof level.level_name);

	if (st.sky && st.sky[0])
		gi.configstring(CS_SKY, st.sky);
	else
		gi.configstring(CS_SKY, "unit1_");

	gi.configstring(CS_SKYROTATE, va("%f", st.skyrotate));

	gi.configstring(CS_SKYAXIS, va("%f %f %f",
		st.skyaxis[0], st.skyaxis[1], st.skyaxis[2]));

	gi.configstring(CS_CDTRACK, va("%i", ent->sounds));

	gi.configstring(CS_MAXCLIENTS, va("%i", (int)(maxclients->value)));

	// Unified status bar for all modes. //QW//
	CreateStatusBar();	// fill the statusbar string
	gi.configstring(CS_STATUSBAR, statusbar);

	// Add the ctf specific stuff
	if (ctf->value)
	{
		//precaches
		gi.imageindex("ctfsb1");
		gi.imageindex("ctfsb2");
		gi.imageindex("i_ctf1");
		gi.imageindex("i_ctf2");
		gi.imageindex("i_ctf1d");
		gi.imageindex("i_ctf2d");
		gi.imageindex("i_ctf1t");
		gi.imageindex("i_ctf2t");
		gi.imageindex("i_ctfj");
	}

	//---------------

	// help icon for statusbar
	gi.imageindex("i_help");
	level.pic_health = gi.imageindex("i_health");
	gi.imageindex("help");
	gi.imageindex("field_3");

	if (!st.gravity)
		gi.cvar_set("gravity", "800");
	else
		gi.cvar_set("gravity", st.gravity);

	snd_fry = gi.soundindex("player/fry.wav");	// standing in lava / slime

	PrecacheItem(&gI_weapon_blaster);

	gi.soundindex("player/lava1.wav");
	gi.soundindex("player/lava2.wav");

	gi.soundindex("misc/pc_up.wav");
	gi.soundindex("misc/talk1.wav");

	gi.soundindex("misc/udeath.wav");

	// gibs
	gi.soundindex("items/respawn1.wav");

	// sexed sounds
	gi.soundindex("*death1.wav");
	gi.soundindex("*death2.wav");
	gi.soundindex("*death3.wav");
	gi.soundindex("*death4.wav");
	gi.soundindex("*fall1.wav");
	gi.soundindex("*fall2.wav");
	gi.soundindex("*gurp1.wav");		// drowning damage
	gi.soundindex("*gurp2.wav");
	gi.soundindex("*jump1.wav");		// player jump
	gi.soundindex("*pain25_1.wav");
	gi.soundindex("*pain25_2.wav");
	gi.soundindex("*pain50_1.wav");
	gi.soundindex("*pain50_2.wav");
	gi.soundindex("*pain75_1.wav");
	gi.soundindex("*pain75_2.wav");
	gi.soundindex("*pain100_1.wav");
	gi.soundindex("*pain100_2.wav");

	//-------------------

	gi.soundindex("player/gasp1.wav");		// gasping for air
	gi.soundindex("player/gasp2.wav");		// head breaking surface, not gasping

	gi.soundindex("player/watr_in.wav");	// feet hitting water
	gi.soundindex("player/watr_out.wav");	// feet leaving water

	gi.soundindex("player/watr_un.wav");	// head going underwater

	gi.soundindex("player/u_breath1.wav");
	gi.soundindex("player/u_breath2.wav");

	gi.soundindex("items/pkup.wav");		// bonus item pickup
	gi.soundindex("world/land.wav");		// landing thud
	gi.soundindex("misc/h2ohit1.wav");		// landing splash

	gi.soundindex("items/damage.wav");
	gi.soundindex("items/protect.wav");
	gi.soundindex("items/protect4.wav");
	gi.soundindex("weapons/noammo.wav");

	gi.soundindex("infantry/inflies1.wav");

	sm_meat_index = gi.modelindex("models/objects/gibs/sm_meat/tris.md2");
	gi.modelindex("models/objects/gibs/arm/tris.md2");
	gi.modelindex("models/objects/gibs/bone/tris.md2");
	gi.modelindex("models/objects/gibs/bone2/tris.md2");
	gi.modelindex("models/objects/gibs/chest/tris.md2");
	gi.modelindex("models/objects/gibs/skull/tris.md2");
	gi.modelindex("models/objects/gibs/head2/tris.md2");

	vwep_index = gi.modelindex("#w_blaster.md2") - 1;
	gi.modelindex("#w_shotgun.md2");
	gi.modelindex("#w_sshotgun.md2");
	gi.modelindex("#w_machinegun.md2");
	gi.modelindex("#w_chaingun.md2");
	gi.modelindex("#a_grenades.md2");
	gi.modelindex("#w_glauncher.md2");
	gi.modelindex("#w_rlauncher.md2");
	gi.modelindex("#w_hyperblaster.md2");
	gi.modelindex("#w_railgun.md2");
	gi.modelindex("#w_bfg.md2");
	gi.modelindex("#w_grapple.md2");

	// Load in the fire.
	//gi.modelindex ("fire.md2");

	// "Laser-tripwire set" sound
	gi.soundindex("world/fusein.wav");
	// "Laser-tripwire fire" sound
	gi.soundindex("flyer/flysght1.wav");
	// "Laser-tripwire shot off the wall" sound
	gi.soundindex("flyer/flydeth1.wav");

	//
	// Setup light animation tables. 'a' is total darkness, 'z' is doublebright.
	//

	if (staticlights->value)
	{
		// Abbreviated light tables (since lights are freed anyway).
		gi.configstring(CS_LIGHTS + 0, "m");
		gi.configstring(CS_LIGHTS + 1, "m");
		gi.configstring(CS_LIGHTS + 2, "m");
		gi.configstring(CS_LIGHTS + 3, "m");
		gi.configstring(CS_LIGHTS + 4, "m");
		gi.configstring(CS_LIGHTS + 5, "m");
		gi.configstring(CS_LIGHTS + 6, "m");
		gi.configstring(CS_LIGHTS + 7, "m");
		gi.configstring(CS_LIGHTS + 8, "m");
		gi.configstring(CS_LIGHTS + 9, "m");
		gi.configstring(CS_LIGHTS + 10, "m");
		gi.configstring(CS_LIGHTS + 11, "m");
		gi.configstring(CS_LIGHTS + 63, "a");
	}
	else
	{
		// 0 normal
		gi.configstring(CS_LIGHTS + 0, "m");

		// 1 FLICKER (first variety)
		gi.configstring(CS_LIGHTS + 1, "mmnmmommommnonmmonqnmmo");

		// 2 SLOW STRONG PULSE
		gi.configstring(CS_LIGHTS + 2, "abcdefghijklmnopqrstuvwxyzyxwvutsrqponmlkjihgfedcba");

		// 3 CANDLE (first variety)
		gi.configstring(CS_LIGHTS + 3, "mmmmmaaaaammmmmaaaaaabcdefgabcdefg");

		// 4 FAST STROBE
		gi.configstring(CS_LIGHTS + 4, "mamamamamama");

		// 5 GENTLE PULSE 1
		gi.configstring(CS_LIGHTS + 5, "jklmnopqrstuvwxyzyxwvutsrqponmlkj");

		// 6 FLICKER (second variety)
		gi.configstring(CS_LIGHTS + 6, "nmonqnmomnmomomno");

		// 7 CANDLE (second variety)
		gi.configstring(CS_LIGHTS + 7, "mmmaaaabcdefgmmmmaaaammmaamm");

		// 8 CANDLE (third variety)
		gi.configstring(CS_LIGHTS + 8, "mmmaaammmaaammmabcdefaaaammmmabcdefmmmaaaa");

		// 9 SLOW STROBE (fourth variety)
		gi.configstring(CS_LIGHTS + 9, "aaaaaaaazzzzzzzz");

		// 10 FLUORESCENT FLICKER
		gi.configstring(CS_LIGHTS + 10, "mmamammmmammamamaaamammma");

		// 11 SLOW PULSE NOT FADE TO BLACK
		gi.configstring(CS_LIGHTS + 11, "abcdefghijklmnopqrrqponmlkjihgfedcba");

		// styles 32-62 are assigned by the light program for switchable lights

		// 63 testing
		gi.configstring(CS_LIGHTS + 63, "a");
	}

	gi.imageindex(PIC_SCANNER_TAG);
	gi.imageindex(PIC_DOT_TAG);
	gi.imageindex(PIC_INVDOT_TAG);
	gi.imageindex(PIC_QUADDOT_TAG);
	gi.imageindex(PIC_UP_TAG);
	gi.imageindex(PIC_DOWN_TAG);
	//	gi.imageindex (PIC_FARDOT_TAG);
	//	gi.imageindex (PIC_FARTHERDOT_TAG);
	//	gi.imageindex (PIC_FARTHESTDOT_TAG);
}


#if 0
// Precache Foxman's song list.
Foxman_precache()
{
	FILE* in;
	char buffer[MAX_QPATH], * res;

	// Open the songlist file.
	gi.dprintf("Preparing to read songlist...\n");
	sprintf(buffer, "./%s/songlist.txt", gamedir->string);
	in = fopen(buffer, "rt");
	if (in == NULL)
	{
		gi.dprintf("No songlist -- can't open ./%s/songlist.txt\n",
			gamedir->string);
	}
	else
	{
		gi.dprintf("Reading songlist...\n");

		// Precache each song in the list.
		while ((res = fgets(buffer, sizeof(buffer), in)) != NULL)
		{
			// Chop the newline(s) from the end of the string.
			res = buffer + strlen(buffer) - 1;
			while (res >= buffer && (*res == 10 || *res == 13))
			{
				*res = 0; res--;
			}

			// Precache this sound.
			gi.soundindex(res);
		}

		fclose(in);
	}
}
#endif


//====================================================== 
//========== Spawn Temp Entity Functions =============== 
//====================================================== 

//	Spawns (type) Splash with {count} particles of {color} at {start} moving 
//	in {direction} and Broadcasts to all in Potentially Visible Set from 
//	vector (origin) 

//	  TE_LASER_SPARKS - Splash particles obey gravity 
//	  TE_WELDING_SPARKS - Splash particles with flash of light at {origin} 
//	  TE_SPLASH - Randomly shaded shower of particles 
//

//	colors: 
/*
Color Codes
0xf2f2f0f0,//red
0xd0d1d2d3,//green
0xf3f3f1f1,//blue
0xdcdddedf,//yellow
0xe0e1e2e3,//yellow strobe
0x80818283,//dark purple
0x70717273,//light blue
0x90919293,//different green
0xb0b1b2b3,//purple
0x40414243,//different red
0xe2e5e3e6,//orange
0xd0f1d3f3,//mixture
0xf2f3f0f1,//inner = red, outer = blue
0xf3f2f1f0,//inner = blue, outer = red
0xdad0dcd2,//inner = green, outer = yellow
0xd0dad2dc //inner = yellow, outer = green
*/

//====================================================== 
void G_Spawn_Splash(int type, int count, int color, vec3_t start, vec3_t movdir, vec3_t origin)
{
	gi.WriteByte(svc_temp_entity);
	gi.WriteByte(type);
	gi.WriteByte(count);
	gi.WritePosition(start);
	gi.WriteDir(movdir);
	gi.WriteByte(color);
	gi.multicast(origin, MULTICAST_PVS);
}

//====================================================== 
//====================================================== 
//====================================================== 

//	Spawns a string of successive (type) models of from record (rec_no) 
//	from (start) to (endpos) which are offset by vector (offset) and 
//	Broadcasts to all in Potentially Visible Set from vector (origin) 

//	  Type: 
//	  TE_GRAPPLE_CABLE - The grappling hook cable (NOT IN 3.19!) 
//	  TE_MEDIC_CABLE_ATTACK - NOT IMPLEMENTED IN ENGINE 
//	  TE_PARASITE_ATTACK - NOT IMPLEMENTED IN ENGINE 

//====================================================== 
void G_Spawn_Models(int type, short rec_no, vec3_t start, vec3_t endpos, vec3_t offset, vec3_t origin)
{
	gi.WriteByte(svc_temp_entity);
	gi.WriteByte(type);
	gi.WriteShort(rec_no);
	gi.WritePosition(start);
	gi.WritePosition(endpos);
	gi.WritePosition(offset);
	gi.multicast(origin, MULTICAST_PVS);
}

//====================================================== 
//====================================================== 
//====================================================== 
//	Spawns a trail of (type) from {start} to {end} and Broadcasts to all 
//	in Potentially Visible Set from vector (origin) 

//	  TE_BFG_LASER - Spawns a green laser 
//	  TE_BUBBLETRAIL - Spawns a trail of bubbles 
//	  TE_PLASMATRAIL - NOT IMPLEMENTED IN ENGINE 
//	  TE_RAILTRAIL - Spawns a blue spiral trail filled with white smoke 

//====================================================== 
void G_Spawn_Trails(int type, vec3_t start, vec3_t endpos, vec3_t origin)
{
	gi.WriteByte(svc_temp_entity);
	gi.WriteByte(type);
	gi.WritePosition(start);
	gi.WritePosition(endpos);
	gi.multicast(origin, MULTICAST_PVS);
}

//====================================================== 
//====================================================== 
//====================================================== 

//	Spawns sparks of (type) from {start} in direction of {movdir} and 
//	Broadcasts to all in Potentially Visible Set from vector (origin) 

//	  TE_BLASTER - Spawns a blaster sparks 
//	  TE_BLOOD - Spawns a spurt of red blood 
//	  TE_BULLET_SPARKS - Same as TE_SPARKS, with a bullet puff and richochet sound 
//	  TE_GREENBLOOD - NOT IMPLEMENTED - Spawns a spurt of green blood 
//	  TE_GUNSHOT - Spawns a grey splash of particles, with a bullet puff 
//	  TE_SCREEN_SPARKS - Spawns a large green/white splash of sparks 
//	  TE_SHIELD_SPARKS - Spawns a large blue/violet splash of sparks 
//	  TE_SHOTGUN - Spawns a small grey splash of spark particles, with a bullet puff 
//	  TE_SPARKS - Spawns a red/gold splash of spark particles 

//====================================================== 
void G_Spawn_Sparks(int type, vec3_t start, vec3_t movdir, vec3_t origin)
{
	gi.WriteByte(svc_temp_entity);
	gi.WriteByte(type);
	gi.WritePosition(start);
	gi.WriteDir(movdir);
	gi.multicast(origin, MULTICAST_PVS);
}

//====================================================== 
//====================================================== 
//====================================================== 

//	Spawns a (type) explosion at (start} and Broadcasts to all Potentially 
//	Visible Sets from {origin} 

//	  TE_BFG_BIGEXPLOSION - Spawns a BFG particle explosion 
//	  TE_BFG_EXPLOSION - Spawns a BFG explosion sprite 
//	  TE_BOSSTPORT - Spawns a mushroom-cloud particle effect 
//	  TE_EXPLOSION1 - Spawns a mid-air-style explosion 
//	  TE_EXPLOSION2 - Spawns a nuclear-style explosion 
//	  TE_GRENADE_EXPLOSION - Spawns a grenade explosion 
//	  TE_GRENADE_EXPLOSION_WATER - Spawns an underwater grenade explosion 
//	  TE_ROCKET_EXPLOSION - Spawns a rocket explosion 
//	  TE_ROCKET_EXPLOSION_WATER - Spawns an underwater rocket explosion 

//		Note: The last four EXPLOSION entries overlap to some degree. 
//		TE_GRENADE_EXPLOSION is the same as TE_EXPLOSION2, 
//		TE_ROCKET_EXPLOSION is the same as TE_EXPLOSION1, 
//		and both of the EXPLOSION_WATER entries are the same, visually.

//====================================================== 
void G_Spawn_Explosion(int type, vec3_t start, vec3_t origin)
{
	gi.WriteByte(svc_temp_entity);
	gi.WriteByte(type);
	gi.WritePosition(start);
	gi.multicast(origin, MULTICAST_PVS);
}
