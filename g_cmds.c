
#include "g_local.h" // includes q_shared.h, game.h and p_menu.h
#include "g_team.h"
#include "m_player.h"

// interface with other modules.
#include "kamikaze.h"
#include "l_tracker.h"
#include "l_turret.h"
#include "l_angels.h"
#include "l_help.h"
#include "l_banner.h"
#include "l_voting.h"
#include "l_flashlight.h"
#include "scanner.h"
#include "l_ar_admin.h"
#include "lithhook.h"
#include "oak.h"		//inteface to Oakbot


// darKMajick definitions
#include "l_dm_grenades.h"


void Cmd_WWW_f (void);
void Cmd_cloak_f (edict_t * ent);
void Cmd_Explosive(edict_t *ent);
void Cmd_Detonate_f(edict_t *ent);
void Cmd_DevMenu_f(char *cmd, edict_t *ent);
void Cmd_Invisible(edict_t *ent);
void Cmd_Flashlight(edict_t *ent);
void Cmd_CheckStats_f (edict_t *ent);

void Cmd_Height(edict_t *ent);
void Cmd_AngelOD(edict_t *ent);
void Cmd_AngelOL(edict_t *ent);
void Cmd_AngelOM(edict_t *ent);
void Cmd_AngelOF(edict_t *ent);
void Cmd_AngelOP(edict_t *ent);
void Cmd_AngelOE(edict_t *ent);
void Cmd_AngelOH(edict_t *ent);
void Cmd_AngelOR(edict_t *ent);
void Cmd_AngelOBlind(edict_t *ent);
void Cmd_AngelOU(edict_t *ent);

void Cmd_PlayerID(edict_t *ent);
void Cmd_FPH(edict_t *ent);
void Cmd_Rangefinder(edict_t *ent);
void Cmd_SweeperFireMode(edict_t *ent);
void Cmd_Ver_f(edict_t *ent);
void Cmd_Tracker_f(edict_t *ent);
void Cmd_TrackerCounter(edict_t *ent);
void Cmd_Turret_f(edict_t *ent);
void Cmd_Baton_f(edict_t *ent); 
void Cmd_Help(edict_t *ent);
void Cmd_Holoquad_f(edict_t *ent);
void Cmd_Holoinvul_f(edict_t *ent);
void Cmd_Holobfg_f(edict_t *ent);
void Cmd_Holoweapon_f(edict_t *ent);
void Cmd_Time_f(edict_t *ent);
void Cmd_TimeRemaining_f(edict_t *ent);
void Cmd_LBind_f(edict_t *ent);
void Cmd_DropArmor_f(edict_t *ent);

void Cmd_DropArmor_f(edict_t *ent)
{
	ent->client->organicarmor = 0;
	ent->client->slugarmor = 0;
	ent->client->cellarmor = 0;
	ent->client->shellarmor = 0;
	ent->client->bulletarmor = 0;
	ent->client->rocketarmor = 0;
	ent->client->grenadearmor = 0;
	ent->client->loxarmor = 0;
}

// Function name	: stripem
// Description	    : Strip unwanted characters from a string.
// copy string p into s stripping out special chars found in string q
// size of p and s must be large enough to contain the full string

// Argument : char *s pointer to source string
// Argument : char *d pointer to destination string
// Argument : char *q pointer to array of characters to filter out

//void stripem(char *s, char *d, char *q)
//{
//	char *r;
//	
//	r = q;
//	while (*s)
//	{
//		while (*q) 
//		{
//			if (*s != *q)
//				*q++;	//next matcher
//			else 
//			{
//				s++;	//next input
//				q = r;	//reset and recompare
//			}
//		}
//		q = r;
//		*d++ = *s++;
//	}
//	*d = '\0'; // terminate the filtered string
//}

//LOX binds
void Cmd_LBind_f(edict_t *ent)
{
	
	unsigned length;
	
	if (gi.argc() < 2) 
	{
		gi.cprintf(ent, PRINT_HIGH, "usage: lbind (ondeath|onrespawn|onenemydeath) <command(s)> - if more than one command is used they must be enclosed in quotes.\n");
		return;
	}
	
	if (gi.argc() == 3 ) 
	{
		length = strlen(gi.argv(2));
		if (length > MAX_INFO_STRING) 
		{
			gi.cprintf(ent, PRINT_HIGH, "Command string is too long!\n");
			return;
		}
	}	
	
	if (Q_stricmp(gi.argv(1),"ondeath") == 0) 
	{
		if (gi.argc() == 2) 
		{
			if (strlen(ent->client->pers.ondeath) != 0)
				gi.cprintf(ent, PRINT_HIGH,"ondeath is set to: %s\n",ent->client->pers.ondeath);
			else 
				gi.cprintf(ent, PRINT_HIGH, "ondeath is not set\n");
		}
		else
			strncpy (ent->client->pers.ondeath, gi.argv(2), sizeof(ent->client->pers.ondeath)-1);
	}	
	
	if (Q_stricmp(gi.argv(1),"onrespawn") == 0) 
	{
		if (gi.argc() == 2) 
		{
			if (strlen(ent->client->pers.onrespawn) != 0)
				gi.cprintf(ent, PRINT_HIGH,"onrespawn is set to: %s\n",ent->client->pers.onrespawn);
			else 
				gi.cprintf(ent, PRINT_HIGH, "onrespawn is not set\n");
		}
		else
			strncpy (ent->client->pers.onrespawn, gi.argv(2), sizeof(ent->client->pers.onrespawn)-1);
	}
	
	if (Q_stricmp(gi.argv(1),"onenemydeath") == 0) 
	{
		if (gi.argc() == 2) 
		{
			if (strlen(ent->client->pers.onenemydeath) != 0)
				gi.cprintf(ent, PRINT_HIGH,"onenemydeath is set to: %s\n",ent->client->pers.onenemydeath);
			else 
				gi.cprintf(ent, PRINT_HIGH, "onenemydeath is not set\n");
			
		}
		else
			strncpy (ent->client->pers.onenemydeath, gi.argv(2), sizeof(ent->client->pers.onenemydeath)-1);
	}
}

/*
if (Q_stricmp(gi.argv(1),"effects") == 0)
{
	if (ent->client->pers.effects != 0)
		gi.cprintf(ent, PRINT_HIGH,"effects is set to: %i\n",ent->client->pers.effects);
	else 
		gi.cprintf(ent, PRINT_HIGH, "effects is not set\n");
}

  else
  ent->client->pers.effects = (int)atoi(gi.argv(2));
  
	else if (Q_stricmp(gi.argv(1),"renderfx") == 0) 
	{
	if (ent->client->pers.renderfx != 0)
	gi.cprintf(ent, PRINT_HIGH,"renderfx is set to: %i\n",ent->client->pers.renderfx);
	else 
	gi.cprintf(ent, PRINT_HIGH, "renderfx is not set\n");
	}
	else
	ent->client->pers.renderfx = (int)atoi(gi.argv(2));
*/

// server side skinlist command to allow users to get
// skins list from server when using clients like r1q2 that
// kill the skins command. //QW//
void Cmd_SkinList_f(edict_t *ent)
{
	int		i;
	char	*skin, *name, string[64];
	edict_t	*edict;
	
	if (ent->spamtimer >= level.time)
		return;	// anti spammage
	if (!level.intermissiontime) 
	{
		// make it all look nice
		gi.cprintf (ent, PRINT_HIGH, "\nnum name             skin");
		gi.cprintf (ent, PRINT_HIGH, "\n--- ---------------- ---------------------\n");
		for (i=0, edict=g_edicts + 1 + i; i < maxclients->value; i++, edict++)
		{
			if (!edict->inuse) 
				continue;
			skin = Info_ValueForKey(edict->client->pers.userinfo, "skin");
			name = Info_ValueForKey(edict->client->pers.userinfo, "name");
			sprintf (string,"%3i %16s %s\n", i, name, skin);
			gi.cprintf (ent, PRINT_HIGH, string);
		}
		sprintf (string, "\n");
		gi.cprintf (ent, PRINT_HIGH, string);
	}
}

// a portable time function using ANSI C functions that will work in Windows or Linux
void Cmd_Time_f(edict_t *ent)
{
	time_t tmpbuf;
	
	if (!ent->client) 
		return;
	if (ent->spamtimer >= level.time) 
		return;	// anti spammage
	time(&tmpbuf);
	gi.cprintf(ent, PRINT_MEDIUM,"Time: %s",ctime(&tmpbuf)); //time string already contains a \n.
}

// report time remaining in deathmatch level
void Cmd_TimeRemaining_f(edict_t *ent)
{
	float tr;
	
	if (!ent->client) 
		return;
	if (ent->spamtimer >= level.time) 
		return;	// anti spammage
	tr = (timelimit->value * 60 + level.roundTimelimit - level.time) / 60;  //decimal minutes
	if (timelimit->value) 
		gi.cprintf(ent, PRINT_MEDIUM,"Time remaining in match: %2.1f minutes\n", tr);
	else
		gi.cprintf(ent, PRINT_MEDIUM,"No time limit in match.");
}

// a time broadcast message
void Cmd_TimeAll_f(edict_t *ent)
{
	time_t tmpbuf;
	float tr;
	
	if (!ent->client) 
		return;
	if (ent->spamtimer >= level.time) 
		return;	// anti spammage
	time(&tmpbuf);
	tr = (timelimit->value * 60 + level.roundTimelimit - level.time) / 60;
	if (timelimit->value) 
		gi.bprintf(PRINT_CHAT, "With %2.1f minutes left in the match, it's %s", tr, ctime(&tmpbuf)); //time string has a \n
	else
		gi.bprintf(PRINT_CHAT, "With no time limit to the match, it's %s", ctime(&tmpbuf)); //time string has a \n
}

// used at GameInit or any place we want the time logged.
// Server internal use only, not a client command.
void Log_Time (void)
{
	time_t tmpbuf;
	
	time(&tmpbuf);
	gi.dprintf("%s",ctime(&tmpbuf)); //time string already contains a \n.
}

void Cmd_Holoweapon_f(edict_t *ent)
{
	edict_t *drop;
	
	if (i_loxfeatureban & LFB_HOLO) 
		return;
	if (!ent->client || ent->health <= 0) 
		return;
	
	if (ent->client->pers.weapon == &gI_weapon_blaster ||
		ent->client->pers.weapon == &gI_weapon_superblaster ||
		ent->client->pers.weapon == &gI_weapon_mace ||
		ent->client->pers.weapon == &gI_weapon_flaregun) 
		return;
	
	if (ent->client->holo != NULL) 
		G_FreeEdict(ent->client->holo);
	
	drop = Drop_Item (ent, ent->client->pers.weapon);
	drop->spawnflags |= DROPPED_PLAYER_ITEM;
	drop->touch = Touch_Item;
	drop->nextthink = level.time + 30;
	drop->think = G_FreeEdict;
	drop->owner = ent;
	ent->client->holo = drop;
	drop->holo = 1;
}

void Cmd_Holobfg_f(edict_t *ent)
{
	edict_t *drop;
	
	if (i_loxfeatureban & LFB_HOLO) 
		return;
	if (!ent->client || ent->health <= 0) 
		return;
	
	if (ent->client->holo != NULL) 
		G_FreeEdict(ent->client->holo);

	drop = Drop_Item (ent, &gI_weapon_bfg);
	drop->spawnflags |= DROPPED_PLAYER_ITEM;
	drop->touch = Touch_Item;
	drop->nextthink = level.time + 30;
	drop->think = G_FreeEdict;
	drop->owner = ent;
	ent->client->holo = drop;
	drop->holo = 1;
}

void Cmd_Holoinvul_f(edict_t *ent)
{
	edict_t *drop;
	
	if (i_loxfeatureban & LFB_HOLO)	
		return;
	if (!ent->client || ent->health <= 0) 
		return;
	
	if (ent->client->holo != NULL) 
		G_FreeEdict(ent->client->holo);

	drop = Drop_Item (ent, &gI_item_invulnerability);
	drop->spawnflags |= DROPPED_PLAYER_ITEM;
	drop->touch = Touch_Item;
	drop->nextthink = level.time + 30;
	drop->think = G_FreeEdict;
	drop->owner = ent;
	ent->client->holo = drop;
	drop->holo = 1;
}

void Cmd_Holoquad_f(edict_t *ent)
{
	edict_t *drop;
	
	if (i_loxfeatureban & LFB_HOLO)
		return;
	if (!ent->client || ent->health <= 0)
		return;
	
	if (ent->client->holo != NULL) 
		G_FreeEdict(ent->client->holo);
	
	drop = Drop_Item (ent, &gI_item_quad);
	drop->spawnflags |= DROPPED_PLAYER_ITEM;
	drop->touch = Touch_Item;
	drop->nextthink = level.time + 30;
	drop->think = G_FreeEdict;
	drop->owner = ent;
	ent->client->holo = drop;
	drop->holo = 1;
}

void Cmd_Help(edict_t *ent)
{
	
	char *query;
	
	if (gi.argc() == 1)
	{
		gi.cprintf(ent, PRINT_HIGH, "usage: man \"<command, weapon or feature>\"\n");
		return;
	}
	
	query = gi.argv(1);
	switch(query[0])
	{
		
	case 'a':
	case 'A':
		if (Q_stricmp (query, "airstrike") == 0) gi.cprintf(ent, PRINT_MEDIUM, Info_Airstrike_Text);
		else if (Q_stricmp (query, "angel") == 0) gi.cprintf(ent, PRINT_MEDIUM, Info_Angel_Text);			
		else if (Q_stricmp (query, "angels") == 0) gi.cprintf(ent, PRINT_MEDIUM, Info_Angel_Text);	
		else if (Q_stricmp (query, "angelob") == 0) gi.cprintf(ent, PRINT_MEDIUM, Info_AngelOBlind_Text);
		else if (Q_stricmp (query, "angeloblind") == 0) gi.cprintf(ent, PRINT_MEDIUM, Info_AngelOBlind_Text);
		else if (Q_stricmp (query, "angelod") == 0) gi.cprintf(ent, PRINT_MEDIUM, Info_AngelOD_Text);
		else if (Q_stricmp (query, "angeloe") == 0) gi.cprintf(ent, PRINT_MEDIUM, Info_AngelOE_Text);
		else if (Q_stricmp (query, "angelof") == 0) gi.cprintf(ent, PRINT_MEDIUM, Info_AngelOF_Text);
		else if (Q_stricmp (query, "angeloflame") == 0) gi.cprintf(ent, PRINT_MEDIUM, Info_AngelOB_Text);
		else if (Q_stricmp (query, "angeloh") == 0) gi.cprintf(ent, PRINT_MEDIUM, Info_AngelOH_Text);
		else if (Q_stricmp (query, "angelohorror") == 0) gi.cprintf(ent, PRINT_MEDIUM, Info_AngelOH_Text);
		else if (Q_stricmp (query, "angelol") == 0) gi.cprintf(ent, PRINT_MEDIUM, Info_AngelOL_Text);
		else if (Q_stricmp (query, "angelom") == 0) gi.cprintf(ent, PRINT_MEDIUM, Info_AngelOM_Text);
		else if (Q_stricmp (query, "angelop") == 0) gi.cprintf(ent, PRINT_MEDIUM, Info_AngelOP_Text);
		else if (Q_stricmp (query, "angelor") == 0) gi.cprintf(ent, PRINT_MEDIUM, Info_AngelOR_Text);
		else if (Q_stricmp (query, "angelou") == 0) gi.cprintf(ent, PRINT_MEDIUM, Info_AngelOU_Text);
		else if (Q_stricmp (query, "anticamper") == 0) gi.cprintf(ent, PRINT_MEDIUM, Info_AntiCamper_Text);
		else if (Q_stricmp (query, "anti-flaregun") == 0) gi.cprintf(ent, PRINT_MEDIUM, Info_AntiFlareGun_Text);
		break;
		
	case 'b':
	case 'B':
		if (Q_stricmp (query, "banzai grenade") == 0) gi.cprintf(ent, PRINT_MEDIUM, Info_BanzaiGrenade_Text);
		else if (Q_stricmp (query, "baton") == 0) gi.cprintf(ent, PRINT_MEDIUM, Info_Baton_Text);
		else if (Q_stricmp (query, "bazooka sweeper") == 0) gi.cprintf(ent, PRINT_MEDIUM, Info_BazookaSweeper_Text);
		else if (Q_stricmp (query, "blindness grenade") == 0) gi.cprintf(ent, PRINT_MEDIUM, Info_BlindnessGrenade_Text);
		else if (Q_stricmp (query, "bazooka") == 0) gi.cprintf(ent, PRINT_MEDIUM, Info_Bazooka_Text);
		else if (Q_stricmp (query, "buckyball") == 0) gi.cprintf(ent, PRINT_MEDIUM, Info_BuckyballLauncher_Text);
		else if (Q_stricmp (query, "buckyball launcher") == 0) gi.cprintf(ent, PRINT_MEDIUM, Info_BuckyballLauncher_Text);
		break;
		
	case 'c':
	case 'C':
		if (Q_stricmp (query, "cloak") == 0) gi.cprintf(ent, PRINT_MEDIUM, Info_Cloak_Text);
		else if (Q_stricmp (query, "checkstats") == 0) gi.cprintf(ent, PRINT_MEDIUM, Info_Checkstats_Text);
		else if (Q_stricmp (query, "chasecam") == 0) gi.cprintf(ent, PRINT_MEDIUM, Info_Chasecam_Text);
		else if (Q_stricmp (query, "chunkgun") == 0) gi.cprintf(ent, PRINT_MEDIUM, Info_ChunkGun_Text);
		else if (Q_stricmp (query, "cluster grenade") == 0) gi.cprintf(ent, PRINT_MEDIUM, Info_ClusterGrenade_Text);
		else if (Q_stricmp (query, "cataclysm device") == 0) gi.cprintf(ent, PRINT_MEDIUM, Info_CataclysmDevice_Text);
		else if (Q_stricmp (query, "cat") == 0) gi.cprintf(ent, PRINT_MEDIUM, Info_CataclysmDevice_Text);
		break;
		
	case 'd':
	case 'D':
		if (Q_stricmp (query, "drop") == 0) gi.cprintf(ent, PRINT_MEDIUM, Info_Drop_Text);
		else if (Q_stricmp (query, "decoy") == 0) gi.cprintf(ent, PRINT_MEDIUM, Info_Decoy_Text); 
		else if (Q_stricmp (query, "detonate") == 0) gi.cprintf(ent, PRINT_MEDIUM, Info_Detonate_Text);
		else if (Q_stricmp (query, "day") == 0) gi.cprintf(ent, PRINT_MEDIUM, Info_DayNight_Text);
		else if (Q_stricmp (query, "night") == 0) gi.cprintf(ent, PRINT_MEDIUM, Info_DayNight_Text);
		else if (Q_stricmp (query, "daynight") == 0) gi.cprintf(ent, PRINT_MEDIUM, Info_DayNight_Text);
		else if (Q_stricmp (query, "doubleimpact") == 0) gi.cprintf(ent, PRINT_MEDIUM, Info_DoubleImpact_Text);
		else if (Q_stricmp (query, "durggun") == 0) gi.cprintf(ent, PRINT_MEDIUM, Info_DurgGun_Text);
		else if (Q_stricmp (query, "disruptor") == 0) gi.cprintf(ent, PRINT_MEDIUM, Info_Disruptor_Text);
		break;
		
	case 'e':
	case 'E':
		if (Q_stricmp (query, "explosive") == 0) gi.cprintf(ent, PRINT_MEDIUM, Info_Explosive_Text);
		else if (Q_stricmp (query, "energy vortex") == 0) gi.cprintf(ent, PRINT_MEDIUM, Info_EnergyVortex_Text);
		break;
		
	case 'f':
	case 'F':
		if (Q_stricmp (query, "flashlight") == 0) gi.cprintf(ent, PRINT_MEDIUM, Info_Flashlight_Text);
		else if (Q_stricmp(query, "feign") == 0) gi.cprintf(ent, PRINT_MEDIUM, Info_FeignDeath_Text);
		else if (Q_stricmp(query, "fph") == 0) gi.cprintf(ent, PRINT_MEDIUM, Info_FPH_Text);
		else if (Q_stricmp (query, "firemode") == 0) gi.cprintf(ent, PRINT_MEDIUM, Info_FireMode_Text);
		else if (Q_stricmp (query, "flamesweeper") == 0) gi.cprintf(ent, PRINT_MEDIUM, Info_FlameSweeper_Text);
		else if (Q_stricmp (query, "freezersweeper") == 0) gi.cprintf(ent, PRINT_MEDIUM, Info_FreezerSweeper_Text);
		else if (Q_stricmp (query, "freezer") == 0) gi.cprintf(ent, PRINT_MEDIUM, Info_FreezeGun_Text);	
		else if (Q_stricmp (query, "freezegun") == 0) gi.cprintf(ent, PRINT_MEDIUM, Info_FreezeGun_Text);	
		else if (Q_stricmp (query, "flaregun") == 0) gi.cprintf(ent, PRINT_MEDIUM, Info_FlareGun_Text);
		else if (Q_stricmp (query, "freezegrenade") == 0) gi.cprintf(ent, PRINT_MEDIUM, Info_FreezeGrenade_Text);	
		else if (Q_stricmp (query, "flamethrower") == 0) gi.cprintf(ent, PRINT_MEDIUM, Info_FlameThrower_Text);
		else if (Q_stricmp (query, "fbfg") == 0) gi.cprintf(ent, PRINT_MEDIUM, Info_FBFG_Text);			
		else if (Q_stricmp (query, "freezatron") == 0) gi.cprintf(ent, PRINT_MEDIUM, Info_Freezatron_Text);		
		else if (Q_stricmp (query, "freezatron grenade") == 0) gi.cprintf(ent, PRINT_MEDIUM, Info_FreezatronGrenade_Text);	
		break;
		
	case 'g':
	case 'G':
		if (Q_stricmp (query, "grenadesweeper") == 0) gi.cprintf(ent, PRINT_MEDIUM, Info_GrenadeSweeper_Text);
		else if (Q_stricmp (query, "grapple") == 0) gi.cprintf(ent, PRINT_MEDIUM, Info_Hook_f_Text);
		else if (Q_stricmp (query, "grenades") == 0) gi.cprintf(ent, PRINT_MEDIUM, Info_Grenades_Text);	
		else if (Q_stricmp (query, "grenade") == 0) gi.cprintf(ent, PRINT_MEDIUM, Info_Grenades_Text);	
		else if (Q_stricmp (query, "darkmajickgrenades") == 0) gi.cprintf(ent, PRINT_MEDIUM, Info_Grenades_Text);	
		else if (Q_stricmp (query, "guided missile") == 0) gi.cprintf(ent, PRINT_MEDIUM, Info_GuidedMissile_Text);	
		break;
		
	case 'h':
	case 'H':
		if (Q_stricmp (query, "help") == 0) 
		{
			gi.cprintf(ent, PRINT_MEDIUM, Info_Man_Text1);	// help on the man functs split into 
			gi.cprintf(ent, PRINT_MEDIUM, Info_Man_Text2);	// segments because it's just too big
			gi.cprintf(ent, PRINT_MEDIUM, Info_Man_Text3);	// to be transmitted in 1 frame.
		}
		else if (Q_stricmp (query, "height") == 0) gi.cprintf(ent, PRINT_MEDIUM, Info_Height_Text);
		else if (Q_stricmp (query, "hook") == 0) gi.cprintf(ent, PRINT_MEDIUM, Info_Hook_f_Text);
		else if (Q_stricmp (query, "homer grenade") == 0) gi.cprintf(ent, PRINT_MEDIUM, Info_HomerGrenade_Text);
		else if (Q_stricmp (query, "homing rocket launcher") == 0) gi.cprintf(ent, PRINT_MEDIUM, Info_HomingRocketLauncher_Text);	
		else if (Q_stricmp (query, "holoquad") == 0) gi.cprintf(ent, PRINT_MEDIUM, Info_Holoquad_Text);
		else if (Q_stricmp (query, "holoinvul") == 0) gi.cprintf(ent, PRINT_MEDIUM, Info_Holoinvul_Text);
		else if (Q_stricmp (query, "holobfg") == 0) gi.cprintf(ent, PRINT_MEDIUM, Info_Holobfg_Text);
		else if (Q_stricmp (query, "holoweapon") == 0) gi.cprintf(ent, PRINT_MEDIUM, Info_Holoweapon_Text);
		break;
		
	case 'i':
	case 'I':
		if (Q_stricmp(query, "id") == 0) gi.cprintf(ent, PRINT_MEDIUM, Info_PlayerID_Text);
		else if (Q_stricmp(query,"invis") == 0) gi.cprintf(ent, PRINT_MEDIUM, Info_NewCloak_Text);
		else if (Q_stricmp (query, "icecube gun") == 0) gi.cprintf(ent, PRINT_MEDIUM, Info_IcecubeGun_Text);	
		break;
		
	case 'k':
	case 'K':
		if (Q_stricmp (query, "kamikaze") == 0) gi.cprintf(ent, PRINT_MEDIUM, Info_Kamikaze_Text);
		break;
		
	case 'l':
	case 'L':
		if (Q_stricmp (query, "lbind") == 0) gi.cprintf(ent, PRINT_MEDIUM, Info_LBind_Text);
		else if (Q_stricmp (query, "lox") == 0) gi.cprintf(ent, PRINT_MEDIUM, Info_LOX_Text);
		else if (Q_stricmp (query, "lsight") == 0) gi.cprintf(ent, PRINT_MEDIUM, Info_LaserSight_Text);
		else if (Q_stricmp (query, "laser") == 0) gi.cprintf(ent, PRINT_MEDIUM, Info_PlaceLaserTripwire_Text);
		else if (Q_stricmp (query, "logplayers") == 0) gi.cprintf(ent, PRINT_MEDIUM, Info_LogPlayers_Text);
		else if (Q_stricmp (query, "lightning gun") == 0) gi.cprintf(ent, PRINT_MEDIUM, Info_LightningGun_Text);
		break;
		
	case 'm':
	case 'M':
		if (Q_stricmp (query, "mace") == 0) gi.cprintf(ent, PRINT_MEDIUM, Info_Mace_Text);
		else if (Q_stricmp (query, "machine rocket gun") == 0) gi.cprintf(ent, PRINT_MEDIUM, Info_MachineRocketGun_Text);
		else if (Q_stricmp (query, "machinegun") == 0) gi.cprintf(ent, PRINT_MEDIUM, Info_MachineGun_Text);
		else if (Q_stricmp (query, "midnight grenade") == 0) gi.cprintf(ent, PRINT_MEDIUM, Info_MidnightGrenade_Text);
		break;
		
	case 'n':
	case 'N':
		if (Q_stricmp (query, "nightvision") == 0) gi.cprintf(ent, PRINT_MEDIUM, Info_Lowlight_Text);
		else if (Q_stricmp (query, "nailgun") == 0) gi.cprintf(ent, PRINT_MEDIUM, Info_Nailgun_Text);
		else if (Q_stricmp (query, "napalm grenade") == 0) gi.cprintf(ent, PRINT_MEDIUM, Info_NapalmGrenade_Text);
		break;
		
	case 'o':
	case 'O':
		if (Q_stricmp(query, "organic") == 0) gi.cprintf(ent, PRINT_MEDIUM, Info_OrganicArmor_Text);
		else if (Q_stricmp(query, "organic armor") == 0) gi.cprintf(ent, PRINT_MEDIUM, Info_OrganicArmor_Text);
		break;			
		
	case 'p':
	case 'P':
		if (Q_stricmp (query, "push") == 0) gi.cprintf(ent, PRINT_MEDIUM, Info_Tractor_Text);
		else if (Q_stricmp (query, "pull") == 0) gi.cprintf(ent, PRINT_MEDIUM, Info_Tractor_Text);
		else if (Q_stricmp(query, "playerid") == 0) gi.cprintf(ent, PRINT_MEDIUM, Info_PlayerID_Text);
		else if (Q_stricmp(query, "plasma grenade") == 0) gi.cprintf(ent, PRINT_MEDIUM, Info_PlasmaGrenade_Text);
		else if (Q_stricmp(query, "plasma cluster grenade") == 0) gi.cprintf(ent, PRINT_MEDIUM, Info_PlasmaClusterGrenade_Text);
		else if (Q_stricmp(query, "plasma rifle") == 0) gi.cprintf(ent, PRINT_MEDIUM, Info_PlasmaRifle_Text);
		else if (Q_stricmp(query, "positron") == 0) gi.cprintf(ent, PRINT_MEDIUM, Info_Positron_Text);
		else if (Q_stricmp(query, "positron grenade") == 0) gi.cprintf(ent, PRINT_MEDIUM, Info_PositronGrenade_Text);
		break;
		
	case 'r':
	case 'R':
		if (Q_stricmp (query, "range") == 0) gi.cprintf(ent, PRINT_MEDIUM, Info_Rangefinder_Text);
		else if (Q_stricmp(query, "revenge") == 0) gi.cprintf(ent, PRINT_MEDIUM, Info_Revenge_Text);
		else if (Q_stricmp (query, "railgun sweeper") == 0) gi.cprintf(ent, PRINT_MEDIUM, Info_RailgunSweeper_Text);
		else if (Q_stricmp(query, "rocket grenade") == 0) gi.cprintf(ent, PRINT_MEDIUM, Info_RocketGrenade_Text);
		else if (Q_stricmp(query, "railbomb") == 0) gi.cprintf(ent, PRINT_MEDIUM, Info_RailBomb_Text);
		break;
		
	case 's':
	case 'S':
		if (Q_stricmp (query, "scanner") == 0) gi.cprintf(ent, PRINT_MEDIUM, Info_Scanner_Text);
		else if (Q_stricmp (query, "score") == 0) gi.cprintf(ent, PRINT_MEDIUM, Info_Score_Text);
		else if (Q_stricmp (query, "shrapnel grenade") == 0) gi.cprintf(ent, PRINT_MEDIUM, Info_ShrapnelGrenade_Text);
		else if (Q_stricmp (query, "skinlist") == 0) gi.cprintf(ent, PRINT_MEDIUM, Info_Skinlist_Text);
		else if (Q_stricmp (query, "skipper homing rocket") == 0) gi.cprintf(ent, PRINT_MEDIUM, Info_SkipperHomingRocket_Text);
		else if (Q_stricmp (query, "slugarmor") == 0) gi.cprintf(ent, PRINT_MEDIUM, Info_SlugArmor_Text);
		else if (Q_stricmp (query, "snipergun") == 0) gi.cprintf(ent, PRINT_MEDIUM, Info_SniperGun_Text);
		else if (Q_stricmp (query, "streetsweeper") == 0) gi.cprintf(ent, PRINT_MEDIUM, Info_StreetSweeper_Text);
		else if (Q_stricmp (query, "spinning rail grenade") == 0) gi.cprintf(ent, PRINT_MEDIUM, Info_SpinningRailGrenade_Text);
		else if (Q_stricmp (query, "sticking grenade") == 0) gi.cprintf(ent, PRINT_MEDIUM, Info_StickingGrenade_Text);
		else if (Q_stricmp (query, "superblaster") == 0) gi.cprintf(ent, PRINT_MEDIUM, Info_Superblaster_Text);
		else if (Q_stricmp (query, "superblastersweeper") == 0) gi.cprintf(ent, PRINT_MEDIUM, Info_SuperblasterSweeper_Text);
		else if (Q_stricmp (query, "sweeper") == 0) gi.cprintf(ent, PRINT_MEDIUM, Info_Sweepers_Text);
		else if (Q_stricmp (query, "sweeperfiremode") == 0) gi.cprintf(ent, PRINT_MEDIUM, Info_SweeperFireMode_Text);
		break;
		
	case 't':
	case 'T':
		if (Q_stricmp (query, "tracker") == 0) gi.cprintf(ent, PRINT_MEDIUM, Info_Tracker_Text);
		else if (Q_stricmp (query, "teleportgrenade") == 0) gi.cprintf(ent, PRINT_MEDIUM, Info_TeleportGrenade_Text);
		else if (Q_stricmp (query, "time") == 0) gi.cprintf(ent, PRINT_MEDIUM, Info_Time_Text);
		else if (Q_stricmp (query, "timeleft") == 0) gi.cprintf(ent, PRINT_MEDIUM, Info_Timeleft_Text);
		else if (Q_stricmp (query, "timeremaining") == 0) gi.cprintf(ent, PRINT_MEDIUM, Info_Timeleft_Text);
		else if (Q_stricmp (query, "timeall") == 0) gi.cprintf(ent, PRINT_MEDIUM, Info_Timeall_Text);
		else if (Q_stricmp (query, "turret") == 0) gi.cprintf(ent, PRINT_MEDIUM, Info_Turret_Text);
		else if (Q_stricmp (query, "trackercount") == 0) gi.cprintf(ent, PRINT_MEDIUM, Info_TrackerCount_Text);
		else if (Q_stricmp (query, "turretcount") == 0) gi.cprintf(ent, PRINT_MEDIUM, Info_TurretCount_Text);
		break;
		
	case 'v':
	case 'V':
		if (Q_stricmp (query, "ver") == 0) gi.cprintf(ent, PRINT_MEDIUM, Info_Ver_Text);
		else if (Q_stricmp (query, "vacuummaker") == 0) gi.cprintf(ent, PRINT_MEDIUM, Info_VacuumMaker_Text);
		else if (Q_stricmp (query, "vote") == 0) gi.cprintf(ent, PRINT_MEDIUM, Info_Voting_Text);
		else if (Q_stricmp (query, "voting") == 0) gi.cprintf(ent, PRINT_MEDIUM, Info_Voting_Text);
		break;
		
	default:
		gi.cprintf(ent, PRINT_HIGH, "Command, person or weapon not known\n");
		break;
	}
}

//====================================================== 
// Pay the Piper to launch the Baton 
//====================================================== 
void Cmd_Baton_f(edict_t *ent) 
{ 
	
	if (i_loxfeatureban & LFB_BATON)
		return;
	if (!ent->client || ent->health <= 0)
		return;
	// Don't allow dead/respawning players to launch Baton! 
	if (!G_ClientInGame(ent))
		return; 
	
	// If not enough frags then notify ent.. 
	if (ent->client->resp.score < 1) 
		gi.centerprintf(ent, "Each Baton costs 1 frag!"); 
	else
	{ 
		// Deduct frags from ent's Score 
		ent->client->resp.score -= 1; 
		// And, fire off the Baton.. 
		Fire_Baton(ent);  
	} 
}

void Cmd_Tracker_f(edict_t *ent)
{
	
	if (i_loxfeatureban & LFB_TRACKER) 
		return;
	if (!ent->client || ent->health <= 0) 
		return;
	Tracker_Weapon_Fire (ent);
	
}

void Cmd_Turret_f(edict_t *ent)
{
	
	if (i_loxfeatureban & LFB_TURRET) 
		return;
	if (!ent->client || ent->health <= 0) 
		return;
	Turret_Weapon_Fire (ent);
	
}

void Cmd_LogPlayers_f(edict_t *ent)
{
	
	int	i,j[4];
	
	edict_t	*player = g_edicts;
	gi.dprintf("---------------------------------------\n");
	gi.dprintf("Who issued the log? %s\n",ent->client->pers.netname);
	gi.dprintf("Level Name: %s\n",level.level_name);
	gi.dprintf("Map name: %s\n",level.mapname);
	for (i=0; i < game.maxclients; i++)
	{
		// move to player edict
		player++;
		if (player->client->pers.ipAddr == 0) 
			continue;

		j[0] = player->client->pers.ipAddr & 0xff;
		j[1] = (player->client->pers.ipAddr & 0xff00) / 0x100;
		j[2] = (player->client->pers.ipAddr & 0xff0000) / 0x10000;
		j[3] = (player->client->pers.ipAddr & 0xff000000) / 0x1000000;
		gi.dprintf("%d:     name: %s\n     ip-address: %d.%d.%d.%d\n     ping: %d\n",
			i+1,player->client->pers.netname, j[0],j[1],j[2],j[3], player->client->ping);
	}
	gi.dprintf("---------------------------------------\n");
}

/*
=================
Cmd_Airstrike_f
CCH: new function to call in airstrikes
JDB: modified 5/4/98
GLJ: modified 6/11/2007 dM_grenades are used when player is holding
     grenade launcher or bazooka when this command is used, else it
	 sets the grenade type to normal and normal mode invokes the rocket
	 entity airstrike.
=================
*/
void Cmd_Airstrike_f (edict_t *ent)
{
	vec3_t  start, forward, world_up, end;
	trace_t tr;
	
	if (!ent->client || ent->health <= 0)
		return; //not if dead
	if (i_loxfeatureban & LFB_AIRSTRIKE)
		return;
	
	// David Martin - 07/15/2005 - Grenades we never want to see used in 
	// airstrikes since grenades are now used instead of rockets.
	// QW - 06/11/2007 - It was possible to launch an airstrike and then
	// command one of these weapons and bypass the protection.
	// I added the airstrike_grentype to the client so now all we
	// need to do is set it here and let Think_Airstrike take care of it.
	
	if ((ent->client->dM_grenade == DM_CLUSTERGRENADE)
		|| (ent->client->dM_grenade == DM_SHRAPNELGRENADE)
		|| (ent->client->dM_grenade == DM_CATACLYSM)
		|| (ent->client->dM_grenade == DM_HOMERGRENADE)
		|| (ent->client->dM_grenade == DM_ROCKETGRENADE)
		|| (ent->client->dM_grenade == DM_TELEGRENADE)
		|| (ent->client->dM_grenade == DM_MIDNIGHTGRENADE)
		|| (ent->client->dM_grenade == DM_BANZAIGRENADE)
		|| (ent->client->dM_grenade == DM_SPINNINGRAILBOMB)) 
	{
		ent->client->airstrike_called = 0;
		ent->client->airstrike_grentype = DM_NORMALGRENADE;
	}
	// not holding the bazooka or grenade launcher use conventional rockets
	else if ((ent->client->pers.weapon != &gI_weapon_grenadelauncher) &&
		(ent->client->pers.weapon != &gI_weapon_bazooka)) 
		ent->client->airstrike_grentype = DM_NORMALGRENADE;
	else
		ent->client->airstrike_grentype = ent->client->dM_grenade;
	
	
	// cancel airstrike if it's already been called
	if ( ent->client->airstrike_called )
	{
		if (ent->lasersight != NULL)
			SP_LaserSight(ent);
		ent->client->airstrike_called = 0;
		gi.cprintf(ent, PRINT_HIGH, "The airstrike has been called off!!\n");
		gi.sound(ent, CHAN_ITEM, gi.soundindex("world/pilot1.wav"), 0.4f, ATTN_NORM, 0);
		return;
	}
	
	// see if we're pointed at the sky
	VectorCopy(ent->s.origin, start);
	start[2] += ent->viewheight;
	AngleVectors(ent->client->v_angle, forward, NULL, NULL);
	VectorMA(start, 8192, forward, end);
	tr = gi.trace(start, NULL, NULL, end, ent, MASK_SHOT|CONTENTS_SLIME|CONTENTS_LAVA);
	if ( tr.surface && !(tr.surface->flags & SURF_SKY) )
	{
        // We hit something but it wasn't sky, so let's see if there is sky above it!
		VectorCopy(tr.endpos,start);
		VectorSet(world_up, 0, 0, 1);
		VectorMA(start, 8192, world_up, end);
		tr = gi.trace(start, NULL, NULL, end, ent, MASK_SHOT|CONTENTS_SLIME|CONTENTS_LAVA);
		if ( tr.surface && !(tr.surface->flags & SURF_SKY))  // No sky above it either!!
		{
			gi.cprintf(ent, PRINT_HIGH, "Airstrikes have to come from the sky!!!\n");
			gi.sound(ent, CHAN_ITEM, gi.soundindex("world/pilot1.wav"), 0.4f, ATTN_NORM, 0);
			return;
		}
	}
	
	// set up for the airstrike
	VectorCopy(tr.endpos, ent->client->airstrike_entry);
	ent->client->airstrike_called = 1;
	ent->client->airstrike_time = level.time + 5;
	if (ent->lasersight == NULL)
		SP_LaserSight(ent);
	gi.cprintf(ent, PRINT_HIGH, "Airstrike on it's way! Light on the target. ETA 5 seconds.\n");
	gi.sound(ent, CHAN_ITEM, gi.soundindex("world/pilot3.wav"), 0.8f, ATTN_NORM, 0);
}

// Print the LOX version info on command
void Cmd_Ver_f(edict_t *ent)
{
	gi.cprintf(ent, PRINT_HIGH, loxbanner);
}

// Toggle the sweeper fire mode
void Cmd_SweeperFireMode(edict_t *ent)
{
	
	if (i_loxfeatureban & LFB_SWEEPERFIREMODE)
		return;
	if (ent->client->sweeperfiremode == SFM_LOW)
	{
		gi.cprintf(ent, PRINT_HIGH, "Sweeper Firemode High\n");
		ent->client->sweeperfiremode = SFM_HIGH;
	}
	
	else if (ent->client->sweeperfiremode == SFM_HIGH)
	{
		gi.cprintf(ent, PRINT_HIGH, "Sweeper Firemode Low\n");
		ent->client->sweeperfiremode = SFM_LOW;
	}
}

void Cmd_FPH(edict_t *ent)
{
	
	if (i_loxfeatureban & LFB_FPH)
		return;
	
	if (ent->client->resp.fph_active == 0)
	{
		ent->client->resp.fph_active = 1;
		gi.cprintf(ent,PRINT_HIGH, "Frag Per Hour Activated\n");
	}
	else if (ent->client->resp.fph_active == 1)
	{
		ent->client->resp.fph_active = 0;
		gi.cprintf(ent,PRINT_HIGH, "Frag Per Hour Deactivated\n");
	}
}

void Cmd_Height(edict_t *ent)
{
	
	if (i_loxfeatureban & LFB_HEIGHT)
		return;
	
	if (ent->client->resp.heightactive == 0)
	{
		ent->client->resp.heightactive = 1;
		gi.cprintf(ent,PRINT_HIGH, "Height Activated\n");
	}
	else if (ent->client->resp.heightactive == 1)
	{
		ent->client->resp.heightactive = 0;
		gi.cprintf(ent,PRINT_HIGH, "Height Deactivated\n");
	}
}

void Cmd_Location_f(edict_t *ent)
{
	
	if (ent->client->resp.locationactive == 0)
	{
		ent->client->resp.locationactive = 1;
		gi.cprintf(ent,PRINT_HIGH, "Location Activated\n");
	}
	else if (ent->client->resp.locationactive == 1)
	{
		ent->client->resp.locationactive = 0;
		gi.cprintf(ent,PRINT_HIGH, "Location Deactivated\n");
	}
}

void Cmd_TrackerCounter(edict_t *ent)
{
	
	if (i_loxfeatureban & LFB_NUMTRKR)
		return;
	
	if (ent->client->resp.trackercountactive == 0)
	{
		ent->client->resp.trackercountactive = 1;
		gi.cprintf(ent,PRINT_HIGH, "Tracker Count Activated\n");
	}
	else if (ent->client->resp.trackercountactive == 1)
	{
		ent->client->resp.trackercountactive = 0;
		gi.cprintf(ent,PRINT_HIGH, "Tracker Count Deactivated\n");
	}
}

void Cmd_TurretCounter(edict_t *ent)
{
	
	if (i_loxfeatureban & LFB_NUMTRRT)
		return;
	
	if (ent->client->resp.turretcountactive == 0)
	{
		ent->client->resp.turretcountactive = 1;
		gi.cprintf(ent,PRINT_HIGH, "Turret Count Activated\n");
	}
	else if (ent->client->resp.turretcountactive == 1)
	{
		ent->client->resp.turretcountactive = 0;
		gi.cprintf(ent,PRINT_HIGH, "Turret Count Deactivated\n");
	}
}

void Cmd_Rangefinder(edict_t *ent)
{
	
	if (i_loxfeatureban & LFB_RANGEFINDER)
		return;
	
	if (ent->client->resp.rangefinderactive == 0)
	{
		ent->client->resp.rangefinderactive = 1;
		gi.cprintf(ent,PRINT_HIGH, "Rangefinder Activated\n");
	}
	else if (ent->client->resp.rangefinderactive == 1)
	{
		ent->client->resp.rangefinderactive = 0;
		gi.cprintf(ent,PRINT_HIGH, "Rangefinder Deactivated\n");
	}
}

void Cmd_PlayerID(edict_t *ent)
{
	
	if (i_loxfeatureban & LFB_PLAYERID)
		return;
	
	if (ent->client->resp.playeridactive == 0)
	{
		ent->client->resp.playeridactive = 1;
		gi.cprintf(ent, PRINT_HIGH, "Player ID Activated\n");
	}
	else if (ent->client->resp.playeridactive == 1)
	{
		ent->client->resp.playeridactive = 0;
		gi.cprintf(ent, PRINT_HIGH, "Player ID Deactivated\n");
	}
}

// Angel of Unfreeze
void Cmd_AngelOU(edict_t *ent)
{
	
	if (i_loxfeatureban & LFB_ANGELOUNFREEZE)
		return;			
	if (!ent->client || ent->health <= 0)
		return;
	
	if ((ent->client->pers.special != AOUNFREEZE) && ent->client->angel!=NULL)
	{
		G_FreeEdict(ent->client->angel);
		ent->client->angel = NULL;
		ent->client->pers.special = 0;
	}
				
	if (ent->client->angel != NULL)
	{		
		G_FreeEdict(ent->client->angel);	
		ent->client->angel = NULL;
		ent->client->pers.special = 0;
		gi.cprintf(ent, PRINT_HIGH, "Angel Of Unfreeze De-activated\n");
	}
	
	else
	{					
		ent->client->pers.special = AOUNFREEZE;	
		gi.cprintf(ent, PRINT_HIGH, "Angel Of Unfreeze Activated\n");
		Spawn_Angel(ent);
	}
}

// Angel of Revenge
void Cmd_AngelOR(edict_t *ent)
{
	
	if (i_loxfeatureban & LFB_ANGELOREVENGE)
		return;				
	if (!ent->client || ent->health <= 0)
		return;
	
	if ((ent->client->pers.special != AOREVENGE) && ent->client->angel!=NULL)
	{
		G_FreeEdict(ent->client->angel);
		ent->client->angel = NULL;
		ent->client->pers.special = 0;
	}
				
	if (ent->client->angel != NULL)
	{		
		G_FreeEdict(ent->client->angel);	
		ent->client->angel = NULL;
		ent->client->pers.special = 0;
		gi.cprintf(ent, PRINT_HIGH, "Angel Of Revenge De-activated\n");
	}
	
	else
	{					
		ent->client->pers.special = AOREVENGE;	
		gi.cprintf(ent, PRINT_HIGH, "Angel Of Revenge Activated\n");
		Spawn_Angel(ent);
	}
}

// Angel of Horror
void Cmd_AngelOH(edict_t *ent)
{
	
	if (i_loxfeatureban & LFB_ANGELOHORROR)
		return;
	if (!ent->client || ent->health <= 0)
		return;
				
	if ((ent->client->pers.special != AOHORROR) && ent->client->angel!=NULL)
	{
		G_FreeEdict(ent->client->angel);
		ent->client->angel = NULL;
		ent->client->pers.special = 0;
	}
				
	if (ent->client->angel != NULL)
	{
		G_FreeEdict(ent->client->angel);	
		ent->client->angel = NULL;
		ent->client->pers.special = 0;
		gi.cprintf(ent, PRINT_HIGH, "Angel Of Horror De-activated\n");
	}
	
	else
	{					
		ent->client->pers.special = AOHORROR;	
		gi.cprintf(ent, PRINT_HIGH, "Angel Of Horror Activated\n");
		Spawn_Angel(ent);
	}
}

// Angel of Blindness
void Cmd_AngelOBlind(edict_t *ent)
{
	
	if (i_loxfeatureban & LFB_ANGELOBLIND)
		return;
	if (!ent->client || ent->health <= 0)
		return;
				
	if ((ent->client->pers.special != AOBLIND) && ent->client->angel!=NULL)
	{
		G_FreeEdict(ent->client->angel);
		ent->client->angel = NULL;
		ent->client->pers.special = 0;
	}
				
	if (ent->client->angel != NULL)
	{		
		G_FreeEdict(ent->client->angel);	
		ent->client->angel = NULL;
		ent->client->pers.special = 0;
		gi.cprintf(ent, PRINT_HIGH, "Angel Of Blindness De-activated\n");
	}
	
	else
	{					
		ent->client->pers.special = AOBLIND;	
		gi.cprintf(ent, PRINT_HIGH, "Angel Of Blindness Activated\n");
		Spawn_Angel(ent);
	}
}

// Angel of Plague
void Cmd_AngelOP(edict_t *ent)
{
	
	if (i_loxfeatureban & LFB_ANGELOPLAGUE)
		return;
	if (!ent->client || ent->health <= 0)
		return;
				
	if ((ent->client->pers.special != AOPLAGUE) && ent->client->angel!=NULL)
	{
		G_FreeEdict(ent->client->angel);
		ent->client->angel = NULL;
		ent->client->pers.special = 0;
	}
				
	if (ent->client->angel != NULL)
	{		
		G_FreeEdict(ent->client->angel);	
		ent->client->angel = NULL;
		ent->client->pers.special = 0;
		gi.cprintf(ent, PRINT_HIGH, "Angel Of Plague De-activated\n");
	}
	
	else
	{					
		ent->client->pers.special = AOPLAGUE;	
		gi.cprintf(ent, PRINT_HIGH, "Angel Of Plague Activated\n");
		Spawn_Angel(ent);
	}
}

// Angel of Flame
void Cmd_AngelOB(edict_t *ent)
{
	
	if (i_loxfeatureban & LFB_ANGELOFLAME)
		return;
	if (!ent->client || ent->health <= 0)
		return;
				
	if ((ent->client->pers.special != AOFLAME) && ent->client->angel!=NULL)
	{
		G_FreeEdict(ent->client->angel);
		ent->client->angel = NULL;
		ent->client->pers.special = 0;
	}
				
	if (ent->client->angel != NULL)
	{		
		G_FreeEdict(ent->client->angel);	
		ent->client->angel = NULL;
		ent->client->pers.special = 0;
		gi.cprintf(ent, PRINT_HIGH, "Angel Of Flame De-activated\n");
	}
	
	else
	{					
		ent->client->pers.special = AOFLAME;	
		gi.cprintf(ent, PRINT_HIGH, "Angel Of Flame Activated\n");
		Spawn_Angel(ent);
	}
}

// Angel of Death
void Cmd_AngelOD(edict_t *ent)
{
	
	if (i_loxfeatureban & LFB_ANGELODEATH)
		return;
	if (!ent->client || ent->health <= 0)
		return;
				
	if ((ent->client->pers.special != AODEATH) && ent->client->angel!=NULL)
	{
		G_FreeEdict(ent->client->angel);
		ent->client->angel = NULL;
		ent->client->pers.special = 0;
	}
				
	if (ent->client->angel != NULL)
	{		
		G_FreeEdict(ent->client->angel);	
		ent->client->angel = NULL;
		ent->client->pers.special = 0;
		gi.cprintf(ent, PRINT_HIGH, "Angel Of Death De-activated\n");
	}
	
	else
	{					
		ent->client->pers.special = AODEATH;	
		gi.cprintf(ent, PRINT_HIGH, "Angel Of Death Activated\n");
		Spawn_Angel(ent);
	}
}

// Angel of Life
void Cmd_AngelOL(edict_t *ent)
{
	
	if (i_loxfeatureban & LFB_ANGELOLIFE)
		return;
	if (!ent->client || ent->health <= 0)
		return;
	
	if ((ent->client->pers.special != AOLIFE) && ent->client->angel!=NULL)
	{
		G_FreeEdict(ent->client->angel);
		ent->client->angel = NULL;
		ent->client->pers.special = 0;		
	}
				
	if (ent->client->angel != NULL)
	{		
		G_FreeEdict(ent->client->angel);	
		ent->client->angel = NULL;
		ent->client->pers.special = 0;
		gi.cprintf(ent, PRINT_HIGH, "Angel Of Life De-activated\n");		
	}
	
	else
	{					
		ent->client->pers.special = AOLIFE;	
		gi.cprintf(ent, PRINT_HIGH, "Angel Of Life Activated\n");
		Spawn_Angel(ent);
	}
}

//Angel of Mercy
void Cmd_AngelOM(edict_t *ent)
{
	
	if (i_loxfeatureban & LFB_ANGELOMERCY)
		return;
	if (!ent->client || ent->health <= 0)
		return;
	
	if ((ent->client->pers.special != AOMERCY) && ent->client->angel != NULL)
	{
		G_FreeEdict(ent->client->angel);
		ent->client->angel = NULL;
		ent->client->pers.special = 0;	
	}
				
	if (ent->client->angel != NULL)
	{		
		G_FreeEdict(ent->client->angel);	
		ent->client->angel = NULL;
		ent->client->pers.special = 0;
		gi.cprintf(ent, PRINT_HIGH, "Angel Of Mercy De-activated\n");		
	}
	
	else 
	{					
		ent->client->pers.special = AOMERCY;	
		gi.cprintf(ent, PRINT_HIGH, "Angel of Mercy Activated\n");
		Spawn_Angel(ent);
	}
}

// Angel of Frost
void Cmd_AngelOF(edict_t *ent)
{ 
	
	if (i_loxfeatureban & LFB_ANGELOFROST)
		return;
	if (!ent->client || ent->health <= 0)
		return;
	
	if ((ent->client->pers.special != AOFROST) && ent->client->angel!=NULL)
	{
		G_FreeEdict(ent->client->angel);
		ent->client->angel = NULL;
		ent->client->pers.special = 0;		
	}
				
	if (ent->client->angel != NULL)
	{		
		G_FreeEdict(ent->client->angel);	
		ent->client->angel = NULL;
		ent->client->pers.special = 0;
		gi.cprintf(ent, PRINT_HIGH, "Angel Of Frost De-activated\n");		
	}
	
	else
	{					
		ent->client->pers.special = AOFROST;	
		gi.cprintf(ent, PRINT_HIGH, "Angel of Frost Activated\n");
		Spawn_Angel(ent);
	}
}

// Angel of Energy
void Cmd_AngelOE(edict_t *ent)
{
	
	if (i_loxfeatureban & LFB_ANGELOENERGY)
		return;
	if (!ent->client || ent->health <= 0)
		return;
	
	if ((ent->client->pers.special != AOENERGY) && ent->client->angel!=NULL)
	{
		G_FreeEdict(ent->client->angel);
		ent->client->angel = NULL;
		ent->client->pers.special = 0;		
	}
				
	if (ent->client->angel != NULL)
	{		
		G_FreeEdict(ent->client->angel);	
		ent->client->angel = NULL;
		ent->client->pers.special = 0;
		gi.cprintf(ent, PRINT_HIGH, "Angel Of Energy De-activated\n");		
	}
	
	else
	{
		ent->client->pers.special = AOENERGY;	
		gi.cprintf(ent, PRINT_HIGH, "Angel of Energy Activated\n");
		Spawn_Angel(ent);
	}
}

//*********** FEIGN DEATH *******************
// see also ClientThink
void Cmd_FeignDeath (edict_t *self)
{
	
	if (i_loxfeatureban & LFB_FEIGNDEATH)
		return;
	//si on n'est pas en train de faire la feinte,
	//c'est que l'on veut la faire !
	
	// if already dead, no need for a 2nd one
	if (self->deadflag)
		return;	//si on est deja mort, pas besoin de l'etre 2 fois... :)
	
	if (!self->client->feign)
	{
		static int i;
		
		self->client->feign = QTRUE;
		self->client->raising_up = QFALSE;
		
		//gi.dprintf ("Feigning death\n");
		
		VectorClear (self->avelocity);
		
		self->takedamage = DAMAGE_YES;
		self->movetype = MOVETYPE_TOSS;
		
		self->client->oldweaponindex = self->s.modelindex2;
		self->client->oldgunindex = self->client->ps.gunindex;
		self->s.modelindex2 = 0;	// remove linked weapon model
		self->client->ps.gunindex = 0;
		
		self->s.angles[0] = 0;
		self->s.angles[2] = 0;
		
		self->s.sound = 0;
		self->client->weapon_sound = 0;
		
		self->maxs[2] = -8;
		self->client->ps.pmove.pm_type = PM_DEAD;
		
		
		i = (i+1)%3;
		// start a death animation
		self->client->anim_priority = ANIM_DEATH;
		
		if (self->client->ps.pmove.pm_flags & PMF_DUCKED)
		{
			self->s.frame = FRAME_crdeath1-1;
			self->client->anim_end = FRAME_crdeath5;
		}
		else switch (i)
		{
		case 0:
			self->s.frame = FRAME_death101-1;
			self->client->anim_end = FRAME_death106;
			break;
		case 1:
			self->s.frame = FRAME_death201-1;
			self->client->anim_end = FRAME_death206;
			break;
		case 2:
			self->s.frame = FRAME_death301-1;
			self->client->anim_end = FRAME_death308;
			break;
		}
		gi.sound (self, CHAN_VOICE, gi.soundindex(va("*death%i.wav", (rand()%4)+1)), 1, ATTN_NORM, 0);
		
		gi.linkentity (self);
	}
	else	//sinon, il faut se relever (if not, it's necessary to be raised)
	{
		vec3_t	mins = {-16, -16, -24};
		vec3_t	maxs = {16, 16, 32};
		
		self->client->feign = QFALSE;
		self->client->raising_up = QTRUE;
		
		self->movetype = MOVETYPE_WALK;
		self->viewheight = 22;
		self->solid = SOLID_BBOX;
		self->flags &= ~FL_NO_KNOCKBACK;
		self->svflags &= ~SVF_DEADMONSTER;
		
		VectorCopy (mins, self->mins);
		VectorCopy (maxs, self->maxs);
		VectorClear (self->velocity);
		
		self->client->anim_priority = ANIM_REVERSE;
		if (self->client->ps.pmove.pm_flags & PMF_DUCKED)
		{
			
			self->s.frame = FRAME_crdeath5+1;
			self->client->anim_end = FRAME_crdeath1;
		}
		else
		{
			self->s.frame = FRAME_death106+1;
			self->client->anim_end = FRAME_death101;
			
		}
		
		self->client->raising_up = QTRUE;
		self->s.modelindex2=self->client->oldweaponindex;
		self->client->ps.gunindex=self->client->oldgunindex; 
		
		//		self->misc_time[7] = level.time + 0.5;
		
		self->client->ps.gunindex = gi.modelindex(self->client->pers.weapon->view_model);
		//		ShowGun(self);					// ### Hentai ### special gun model
		self->s.modelindex2 = 255;		// custom gun model
		gi.linkentity (self);
	}
}

/*
=================
Cmd_CheckStats_f
CCH: New function to print all players' stats
=================
*/
void Cmd_CheckStats_f (edict_t *ent)
{
	int		i, j;
	edict_t	*player;
	char	stats[500];
	vec3_t	v;
	float	len;
	
	if (i_loxfeatureban & LFB_CHECKSTATS)
		return;	
	if (!ent->client || ent->health <= 0)
		return;
	
	// use in coop mode only
	// if (!coop->value)
	//	return;
	
	j = sprintf(stats, "            Name Health Range\n=============================\n");
	for (i=0 ; i < maxclients->value ; i++)
	{
		player = g_edicts + 1 + i;
		if (!player->inuse || !player->client)
			continue; 
		VectorSubtract (ent->s.origin, player->s.origin, v);
		len = VectorLength (v);
		j += sprintf(stats + j, "%16s %6d %5.0f\n", player->client->pers.netname, player->health, len);
		if (j > 450)
			break;
	}
	gi.centerprintf(ent, "%s", stats);
}

void Cmd_Flashlight(edict_t *ent)
{
	
	if (i_loxfeatureban & LFB_FLASHLIGHT)
		return;	
	if (!ent->client || ent->health <= 0)
		return;
	
	if (ent->client->flashlight == NULL)
	{ 
		gi.cprintf(ent, PRINT_HIGH, "Flashlight On\n");
		Use_Flashlight(ent);
	}
	else
	{
		gi.cprintf(ent, PRINT_HIGH, "Flashlight Off\n");
		nightmareResetFlashlight(ent->client->flashlight);
	}
}

void Cmd_Invisible(edict_t *ent)
{
	
	if (i_loxfeatureban & LFB_INVISIBILITY)
		return;	
	if (!ent->client || ent->health <= 0)
		return;
	
	if (ent->cloaked)
	{
		gi.cprintf(ent, PRINT_HIGH, "Cloak being used\n");
		return;
	}
	else if (!ent->client->invisible)
	{
		gi.cprintf(ent, PRINT_HIGH, "Invisibility On\n");
		ent->client->cloak_time = level.time + 30.0f;
		gi.sound(ent, CHAN_ITEM, gi.soundindex("items/protect.wav"), 1, ATTN_NORM, 0);
		ent->client->invisible = QTRUE;
	}
	else
	{
		gi.cprintf(ent, PRINT_HIGH, "Invisibility Off\n");
		ent->client->cloak_time = level.time;
		ent->client->invisible = QFALSE;
	}
}

void Cmd_WWW_f (void)
{
	gi.bprintf(PRINT_MEDIUM,"Someone is trying to cheat!@!@!@!\n");
	return;
}

void Cmd_cloak_f (edict_t *ent)
{
	if (i_loxfeatureban & LFB_CLOAK)
		return;
	if (!ent->client || ent->health <= 0)
		return;
	
	if (ent->client->invisible)
	{
		gi.cprintf(ent, PRINT_HIGH, "Invisibility being used\n");
		return;
	}
	if (!ent->cloaked)
	{
		ent->client->cloaktime = 200 + level.framenum;
		ent->cloaked = QTRUE;
		if (ent->oldmodel)
			ent->oldmodel = QFALSE;
		gi.cprintf(ent, PRINT_HIGH, "Cloaked\n");
	}
	else
	{
		ent->cloaked = QFALSE;
		ent->client->cloaktime = level.framenum;
		gi.cprintf(ent, PRINT_HIGH, "Uncloaked\n");
	}
}

void Cmd_Detonate_f(edict_t *ent)
{
	
	if (i_loxfeatureban & LFB_EXPLOSIVE)
		return;
	//QW//	if (!ent->client || ent->health <= 0)
	//	return;
	//QW// can still detonate if dead... tee hee...
	
	if (ent->client->expactive == 0)
	{
		gi.cprintf(ent, PRINT_HIGH, "Explosive Not active\n");
		return;
	}
	
	if (ent->client->explosive == NULL)
	{
		gi.cprintf(ent, PRINT_HIGH, "No explosives out\n");
		return;
	}
	
	gi.cprintf(ent, PRINT_HIGH, "Explosive Detonated\n");
	
	Grenade_Explode_dM(ent->client->explosive);
	ent->client->explosive = NULL;
}

void Cmd_Explosive(edict_t *ent)
{
	
	if (i_loxfeatureban & LFB_EXPLOSIVE)
		return;	
	if (!ent->client || ent->health <= 0)
		return;
	
	if (ent->client->expactive == 0)
	{
		gi.cprintf(ent, PRINT_HIGH, "Explosives Active\n");
		ent->client->expactive = 1;
	}
	
	else if (ent->client->expactive == 1)
	{
		gi.cprintf(ent, PRINT_HIGH, "Explosives Deactivated\n");
		ent->client->expactive = 0;
		if (ent->client->explosive != NULL)
		{
			Grenade_Explode_dM(ent->client->explosive);
			ent->client->explosive = NULL;
		}
	}
}

char *ClientTeam (edict_t *ent)
{
	char		*p;
	static char	value[512];
	
	value[0] = 0;
	
	if (!ent->client)
		return value;
	
	strncpy(value, Info_ValueForKey (ent->client->pers.userinfo, "skin"), sizeof(value)-1);
	p = strchr(value, '/');
	if (!p)
		return value;
	
	if ((int)(dmflags->value) & DF_MODELTEAMS)
	{
		*p = 0;
		return value;
	}
	return ++p;
}

qboolean OnSameTeam (edict_t *ent1, edict_t *ent2)
{
	char	ent1Team [512];
	char	ent2Team [512];
	
	if (ctf->value && ent1->client && ent2->client
		&& (ent1->client->resp.ctf_team == ent2->client->resp.ctf_team))
		return QTRUE;
	
	if ((int)(dmflags->value) & (DF_MODELTEAMS | DF_SKINTEAMS))
	{
		strncpy (ent1Team, ClientTeam (ent1), sizeof(ent1Team)-1);
		strncpy (ent2Team, ClientTeam (ent2), sizeof(ent2Team)-1);
		
		if (strcmp(ent1Team, ent2Team) == 0)
			return QTRUE;
	}
	return QFALSE;
}	

void SelectNextItem (edict_t *ent, int itflags)
{
	gclient_t	*cl;
	unsigned int i, index;
	gitem_t		*it;
	
	cl = ent->client;
	
	//ZOID
	if (cl->menu)
	{
		PMenu_Next(ent);
		return;
	}
	else if (cl->chase_target)
	{
		ChaseNext(ent);
		return;
	}
	//ZOID
	
	// scan  for the next valid one
	for (i=1 ; i<=MAX_ITEMS ; i++)
	{
		// index = (cl->pers.selected_item + i)%MAX_ITEMS;
		index = (((unsigned)cl->pers.selected_item) + i) & (MAX_ITEMS-1);
		if (!cl->pers.inventory[index])
			continue;
		it = itemlist[index];
		if (!it->use)
			continue;
		if (!(it->flags & itflags))
			continue;
		
		cl->pers.selected_item = index;
		return;
	}
	
	cl->pers.selected_item = -1;
}

void SelectPrevItem (edict_t *ent, int itflags)
{
	gclient_t	*cl;
	unsigned int i, index;
	gitem_t		*it;
	
	cl = ent->client;
	
	//ZOID
	if (cl->menu)
	{
		PMenu_Prev(ent);
		return;
	}
	else if (cl->chase_target)
	{
		ChasePrev(ent);
		return;
	}
	//ZOID
	
	// scan  for the next valid one
	for (i=1 ; i<=MAX_ITEMS ; i++)
	{
		// index = (cl->pers.selected_item + MAX_ITEMS - i)%MAX_ITEMS;
		index = (((unsigned)cl->pers.selected_item) + MAX_ITEMS - i)
			& (MAX_ITEMS-1);
		if (!cl->pers.inventory[index])
			continue;
		it = itemlist[index];
		if (!it->use)
			continue;
		if (!(it->flags & itflags))
			continue;
		
		cl->pers.selected_item = index;
		return;
	}
	
	cl->pers.selected_item = -1;
}

void ValidateSelectedItem (edict_t *ent)
{
	gclient_t	*cl;
	
	cl = ent->client;
	
	if (cl->pers.inventory[cl->pers.selected_item])
		return;		// valid
	
	SelectNextItem (ent, -1);
}


//=================================================================================


//==================
// Cmd_Give_f
//
// Give items to a client
//==================

void Cmd_Give_f (edict_t *ent)
{
	char		*name;
	gitem_t		*it;
	int			index;
	int			i;
	qboolean	give_all;
	edict_t		*it_ent;
	
	if ((deathmatch->value || coop->value) && !sv_cheats->value)
	{
		gi.cprintf (ent, PRINT_HIGH, 
			"You must run the server with '+set cheats 1' to enable this command.\n");
		return;
	}
	
	name = gi.args();
	
	if (Q_stricmp(name, "all") == 0)
		give_all = QTRUE;
	else
		give_all = QFALSE;
	
	if (give_all || Q_stricmp(gi.argv(1), "health") == 0)
	{
		if (gi.argc() == 3)
			ent->health = atoi(gi.argv(2));
		else
			ent->health = ent->max_health;
		if (!give_all)
			return;
	}
	
	if (give_all || Q_stricmp(name, "weapons") == 0)
	{
		for (i=0 ; i<game.num_items ; i++)
		{
			it = itemlist[i];
			if (!it->pickup)
				continue;
			if (!(it->flags & IT_WEAPON))
				continue;
			ent->client->pers.inventory[i] += 1;
		}
		if (!give_all)
			return;
	}
	
	if (give_all || Q_stricmp(name, "ammo") == 0)
	{
		for (i=0 ; i<game.num_items ; i++)
		{
			it = itemlist[i];
			if (!it->pickup)
				continue;
			if (!(it->flags & IT_AMMO))
				continue;
			Add_Ammo (ent, it, 1000);
		}
		if (!give_all)
			return;
	}
	
	if (give_all || Q_stricmp(name, "armor") == 0)
	{
		gitem_armor_t	*info;
		
		it = &gI_item_armor_jacket;
		ent->client->pers.inventory[ITEM_INDEX(it)] = 0;
		
		it = &gI_item_armor_combat;
		ent->client->pers.inventory[ITEM_INDEX(it)] = 0;
		
		it = &gI_item_armor_body;
		info = (gitem_armor_t *)it->info;
		ent->client->pers.inventory[ITEM_INDEX(it)] = info->max_count;
		
		if (!give_all)
			return;
	}
	
	if (give_all || Q_stricmp(name, "Power Shield") == 0)
	{
		it = &gI_item_power_shield;
		it_ent = G_Spawn();
		it_ent->classname = it->classname;
		SpawnItem (it_ent, it);
		Touch_Item (it_ent, ent, NULL, NULL);
		if (it_ent->inuse)
			G_FreeEdict(it_ent);
		
		if (!give_all)
			return;
	}
	
	if (give_all)
	{
		for (i=0 ; i<game.num_items ; i++)
		{
			it = itemlist[i];
			if (!it->pickup)
				continue;
			if (it->flags & (IT_ARMOR|IT_WEAPON|IT_AMMO))
				continue;
			ent->client->pers.inventory[i] = 1;
		}
		return;
	}
	
	it = FindItem (name);
	if (!it)
	{
		name = gi.argv(1);
		it = FindItem (name);
		if (!it)
		{
			gi.cprintf (ent, PRINT_HIGH, "unknown item\n");
			return;
		}
	}
	
	if (!it->pickup)
	{
		gi.cprintf (ent, PRINT_HIGH, "non-pickup item\n");
		return;
	}
	
	index = ITEM_INDEX(it);
	
	if (it->flags & IT_AMMO)
	{
		if (gi.argc() == 3)
			ent->client->pers.inventory[index] = atoi(gi.argv(2));
		else
			ent->client->pers.inventory[index] += it->quantity;
	}
	else
	{
		it_ent = G_Spawn();
		it_ent->classname = it->classname;
		SpawnItem (it_ent, it);
		Touch_Item (it_ent, ent, NULL, NULL);
		if (it_ent->inuse)
			G_FreeEdict(it_ent);
	}
}

//==================
//Cmd_God_f
//
// Sets client to godmode
//  
// argv(0) god
//==================
//
void Cmd_God_f (edict_t *ent)
{
	char	*msg;
	
	if ((deathmatch->value || coop->value) && !sv_cheats->value)
	{
		gi.cprintf (ent, PRINT_HIGH, "You must run the server with '+set cheats 1' to enable this command.\n");
		return;
	}
	
	ent->flags ^= FL_GODMODE;
	if (!(ent->flags & FL_GODMODE) )
		msg = "godmode OFF\n";
	else
		msg = "godmode ON\n";
	
	gi.cprintf (ent, PRINT_HIGH, msg);
}


//==================
//	Cmd_Notarget_f
//	
//  Sets client to notarget
//	  
//	argv(0) notarget
//==================
void Cmd_Notarget_f (edict_t *ent)
{
	char	*msg;
	
	if ((deathmatch->value || coop->value) && !sv_cheats->value)
	{
		gi.cprintf (ent, PRINT_HIGH, "You must run the server with '+set cheats 1' to enable this command.\n");
		return;
	}
	
	ent->flags ^= FL_NOTARGET;
	if (!(ent->flags & FL_NOTARGET) )
		msg = "notarget OFF\n";
	else
		msg = "notarget ON\n";
	
	gi.cprintf (ent, PRINT_HIGH, msg);
}


//==================
//	Cmd_Noclip_f
//	
//  argv(0) noclip
//==================
void Cmd_Noclip_f (edict_t *ent)
{
	char	*msg;
	
	if ((deathmatch->value || coop->value) && !sv_cheats->value)
	{
		gi.cprintf (ent, PRINT_HIGH, 
			"You must run the server with '+set cheats 1' to enable this command.\n");
		return;
	}
	
	if (ent->movetype == MOVETYPE_NOCLIP)
	{
		ent->movetype = MOVETYPE_WALK;
		msg = "noclip OFF\n";
	}
	else
	{
		ent->movetype = MOVETYPE_NOCLIP;
		if (ent->thirdperson) ThirdEnd(ent);
		msg = "noclip ON\n";
	}
	
	gi.cprintf (ent, PRINT_HIGH, msg);
}

//=====
//	Cmd_kamikaze_f
//	added so player cannot kamikaze when dead
//=====
void Cmd_Kamikaze_f (edict_t *ent)
{
	// No kamikazes when you're dead.
	if (ent->deadflag) 
		return;
	
	// Or when you're a ghost.
	if (ctf->value && ent->movetype == MOVETYPE_NOCLIP && ent->solid == SOLID_NOT)
		return;
	
	// Or if you're frozen.
	//QW// Configuration variable allows kamikaze if frozen
	if (ent->frozen && (int)kami_frozen->value == 0)
		return;
	
	// Or if kamikazes have been banned.
	if (i_featureban & FB_KAMIKAZE)
		return;
	
	Start_Kamikaze_Mode (ent);
}

/*
=================
Cmd_Third_f
=================
*/
void Cmd_Third_f (edict_t *ent)
{
	if (ent->deadflag)
		return;
	
	if (ent->movetype == MOVETYPE_NOCLIP)
	{
		gi.cprintf (ent, PRINT_HIGH, "Can't be in Chasecam Mode and noclip!\n");
		return;
	}
	else if (gi.pointcontents (ent->s.origin) & CONTENTS_SOLID)
	{
		gi.cprintf (ent, PRINT_HIGH, "Can't activate Chasecam Mode when in a solid!\n");
		return;
	}
	else
	{
		ent->thirdperson = 1 - ent->thirdperson;
		if (ent->thirdperson)
			ThirdBegin(ent);
		else
			ThirdEnd(ent);
	}
}

//=================
//Cmd_ThirdX_f
//=================
void Cmd_ThirdX_f (edict_t *ent)
{
	if (gi.argc() == 1)
	{
		gi.cprintf(ent, PRINT_HIGH, va("%f\n", ent->thirdoffx));
		return;
	}
	ent->thirdoffx = (float) atof(gi.argv(1));
}

//=================
//Cmd_ThirdZ_f
//=================
void Cmd_ThirdZ_f (edict_t *ent)
{
	if (gi.argc() == 1)
	{
		gi.cprintf(ent, PRINT_HIGH, va("%f\n", ent->thirdoffz));
		return;
	}
	ent->thirdoffz = (float) atof (gi.argv(1));
}

//==================
//	Cmd_Use_f
//
//  Use an inventory item
//==================
void Cmd_Use_f (edict_t *ent)
{
	int			index;
	gitem_t		*it;
	char		*s;
	
//	START_PERFORMANCE_TIMER;

	s = gi.args();
	it = FindItem (s);
	if (!it)
	{
		gi.cprintf (ent, PRINT_HIGH, "unknown item: %s\n", s);
		return;
	}
	
	if (!it->use)
	{
		gi.cprintf (ent, PRINT_HIGH, "Item is not usable.\n");
		return;
	}
	
	// Handle weapon selection specially, to account for alternate weapons as
	// well as weapon banning.  Basically, if they're already holding the normal
	// weapon and they have the alternate in their inventory, or if they don't
	// have the normal weapon, select the alternate.
	/*	if (it == &gI_weapon_blaster)
	{
	if ((it == ent->client->pers.weapon
	&& ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_superblaster)])
	|| (it != ent->client->pers.weapon
	&& !ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_blaster)]))
	it = &gI_weapon_superblaster;
}*/
	
	else if (it == &gI_weapon_blaster)
	{
		
		it = ent->client->pers.lastweapons[0];
		if ((ent->client->pers.lastweapons[0] == &gI_weapon_blaster)
			&&((it == ent->client->pers.weapon
			&& ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_superblaster)])
			|| (it != ent->client->pers.weapon
			&& !ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_blaster)])))
		{
			it = &gI_weapon_superblaster;
		}
		
		else if (((ent->client->pers.lastweapons[0] == &gI_weapon_superblaster)||
			(ent->client->pers.lastweapons[0] == &gI_weapon_blaster))
			&&((it == ent->client->pers.weapon
			&& ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_flaregun)])
			|| (it != ent->client->pers.weapon
			&& !ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_blaster)])))
		{
			it = &gI_weapon_flaregun;
		}
		else if (((ent->client->pers.lastweapons[0] == &gI_weapon_flaregun)||
			(ent->client->pers.lastweapons[0] == &gI_weapon_superblaster)||
			(ent->client->pers.lastweapons[0] == &gI_weapon_blaster))
			&&((it == ent->client->pers.weapon
			&& ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_antiflaregun)])
			|| (it != ent->client->pers.weapon
			&& !ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_blaster)])))
		{
			it = &gI_weapon_antiflaregun;
		}
		else if (((ent->client->pers.lastweapons[0] == &gI_weapon_antiflaregun)||
			(ent->client->pers.lastweapons[0] == &gI_weapon_flaregun)||
			(ent->client->pers.lastweapons[0] == &gI_weapon_superblaster)||
			(ent->client->pers.lastweapons[0] == &gI_weapon_blaster))
			&&((it == ent->client->pers.weapon
			&& ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_mace)])
			|| (it != ent->client->pers.weapon
			&& !ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_blaster)])))
		{
			it = &gI_weapon_mace;
		}
		
		else
		{
			it = &gI_weapon_blaster;
		}
		ent->client->pers.lastweapons[0] = it;
	}
	
	
	
	else if (it == &gI_weapon_shotgun)
	{
		it = ent->client->pers.lastweapons[1];
		if ((ent->client->pers.lastweapons[1] == &gI_weapon_shotgun)
			&&((it == ent->client->pers.weapon
			&& ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_sniper)])
			|| (it != ent->client->pers.weapon
			&& (!ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_shotgun)]
			|| (ent->client->pers.inventory[ITEM_INDEX(gI_weapon_shotgun.ammo)]
			< gI_weapon_sniper.quantity
			&& !g_select_empty->value)))))
			it = &gI_weapon_sniper;
		
		else if (((ent->client->pers.lastweapons[1] == &gI_weapon_sniper)||
			(ent->client->pers.lastweapons[1] == &gI_weapon_shotgun))
			&&((it == ent->client->pers.weapon
			&& ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_explosiveshotgun)])
			|| (it != ent->client->pers.weapon
			&& (!ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_shotgun)]
			|| (ent->client->pers.inventory[ITEM_INDEX(gI_weapon_shotgun.ammo)]
			< gI_weapon_shotgun.quantity
			&& !g_select_empty->value)))))
		{
			it = &gI_weapon_explosiveshotgun;
		}
		
		else if (((ent->client->pers.lastweapons[1] == &gI_weapon_explosiveshotgun)||
			(ent->client->pers.lastweapons[1] == &gI_weapon_sniper)||
			(ent->client->pers.lastweapons[1] == &gI_weapon_shotgun))
			&&((it == ent->client->pers.weapon
			&& ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_airfist)])
			|| (it != ent->client->pers.weapon
			&& (!ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_shotgun)]
			|| (ent->client->pers.inventory[ITEM_INDEX(gI_weapon_shotgun.ammo)]
			< gI_weapon_shotgun.quantity
			&& !g_select_empty->value)))))
		{
			it = &gI_weapon_airfist;
		}
		
		else
			it = &gI_weapon_shotgun;
		ent->client->pers.lastweapons[1] = it;
	}
	
	
	else if (it == &gI_weapon_supershotgun)
	{
		it = ent->client->pers.lastweapons[2];
		if ((ent->client->pers.lastweapons[2] == &gI_weapon_supershotgun)
			&&((it == ent->client->pers.weapon
			&& ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_freezer)])
			|| (it != ent->client->pers.weapon
			&& (!ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_supershotgun)]
			|| (ent->client->pers.inventory[ITEM_INDEX(gI_weapon_supershotgun.ammo)]
			< gI_weapon_supershotgun.quantity
			&& !g_select_empty->value)))))
		{
			it = &gI_weapon_freezer;
		}
		else if (((ent->client->pers.lastweapons[2] == &gI_weapon_freezer)||
			(ent->client->pers.lastweapons[2] == &gI_weapon_supershotgun))
			&&((it == ent->client->pers.weapon
			&& ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_explosivesupershotgun)])
			|| (it != ent->client->pers.weapon
			&& (!ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_explosivesupershotgun)]
			|| (ent->client->pers.inventory[ITEM_INDEX(gI_weapon_explosivesupershotgun.ammo)]
			< gI_weapon_freezer.quantity
			&& !g_select_empty->value)))))
		{
			it = &gI_weapon_explosivesupershotgun;
		}
		else if (((ent->client->pers.lastweapons[2] == &gI_weapon_explosivesupershotgun)||
			(ent->client->pers.lastweapons[2] == &gI_weapon_freezer)||
			(ent->client->pers.lastweapons[2] == &gI_weapon_supershotgun))
			&&((it == ent->client->pers.weapon
			&& ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_superrailshotgun)])
			|| (it != ent->client->pers.weapon
			&& (!ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_superrailshotgun)]
			|| (ent->client->pers.inventory[ITEM_INDEX(gI_weapon_superrailshotgun.ammo)]
			< gI_weapon_freezer.quantity
			&& !g_select_empty->value)))))
		{
			it = &gI_weapon_superrailshotgun;
		}
		
		
		else
			it =  &gI_weapon_supershotgun;
		
		ent->client->pers.lastweapons[2] = it;
	}
	
	else if (it == &gI_weapon_machinegun)
	{
		it = ent->client->pers.lastweapons[3];
		if ((ent->client->pers.lastweapons[3] == &gI_weapon_machinegun)
			&&((it == ent->client->pers.weapon
			&& ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_machine)])
			|| (it != ent->client->pers.weapon
			&& (!ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_machinegun)]
			|| (ent->client->pers.inventory[ITEM_INDEX(gI_weapon_machinegun.ammo)]
			< gI_weapon_machinegun.quantity
			&& !g_select_empty->value)))))
		{
			it = &gI_weapon_machine;
		}
		
		else if (((ent->client->pers.lastweapons[3] == &gI_weapon_machine)||
			(ent->client->pers.lastweapons[3] == &gI_weapon_machinegun))
			&&((it == ent->client->pers.weapon
			&& ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_machinerocketgun)])
			|| (it != ent->client->pers.weapon
			&& (!ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_machinegun)]
			|| (ent->client->pers.inventory[ITEM_INDEX(gI_weapon_machinegun.ammo)]
			< gI_weapon_machinegun.quantity
			&& !g_select_empty->value)))))
		{
			it = &gI_weapon_machinerocketgun;
		}
		
		else if (((ent->client->pers.lastweapons[3] == &gI_weapon_machinerocketgun)||
			(ent->client->pers.lastweapons[3] == &gI_weapon_machinerocketgun))
			&&((it == ent->client->pers.weapon
			&& ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_explosivemachinegun)])
			|| (it != ent->client->pers.weapon
			&& (!ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_machinegun)]
			|| (ent->client->pers.inventory[ITEM_INDEX(gI_weapon_machinegun.ammo)]
			< gI_weapon_machinegun.quantity
			&& !g_select_empty->value)))))
		{
			it = &gI_weapon_explosivemachinegun;
		}
		
		else if (((ent->client->pers.lastweapons[3] == &gI_weapon_explosivemachinegun) ||
			(ent->client->pers.lastweapons[3] == &gI_weapon_machine)||
			(ent->client->pers.lastweapons[3] == &gI_weapon_machinegun))
			&&((it == ent->client->pers.weapon
			&& ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_pulserifle)])
			|| (it != ent->client->pers.weapon
			&& (!ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_machinegun)]
			|| (ent->client->pers.inventory[ITEM_INDEX(gI_weapon_machinegun.ammo)]
			< gI_weapon_machinegun.quantity
			&& !g_select_empty->value)))))
		{
			it = &gI_weapon_pulserifle;
		}
		
		else if (((ent->client->pers.lastweapons[3] == &gI_weapon_pulserifle)||
			(ent->client->pers.lastweapons[3] == &gI_weapon_explosivemachinegun) ||
			(ent->client->pers.lastweapons[3] == &gI_weapon_machine)||
			(ent->client->pers.lastweapons[3] == &gI_weapon_machinegun))
			&&((it == ent->client->pers.weapon
			&& ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_nailgun)])
			|| (it != ent->client->pers.weapon
			&& (!ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_machinegun)]
			|| (ent->client->pers.inventory[ITEM_INDEX(gI_weapon_machinegun.ammo)]
			< gI_weapon_machinegun.quantity
			&& !g_select_empty->value)))))
		{
			it = &gI_weapon_nailgun;
		}
		
		else 
			it =  &gI_weapon_machinegun;
		ent->client->pers.lastweapons[3] = it;
	}
	
	else if (it == &gI_weapon_grenadelauncher)
	{
		it = ent->client->pers.lastweapons[4];
		if ((ent->client->pers.lastweapons[4] == &gI_weapon_grenadelauncher)
			&&((it == ent->client->pers.weapon
			&& ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_bazooka)])
			|| (it != ent->client->pers.weapon
			&& (!ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_grenadelauncher)]
			|| (ent->client->pers.inventory[ITEM_INDEX(gI_weapon_grenadelauncher.ammo)]
			< gI_weapon_grenadelauncher.quantity
			&& !g_select_empty->value)))))
		{
			it = &gI_weapon_bazooka;
		}
		
		else if (((ent->client->pers.lastweapons[4] == &gI_weapon_bazooka)||
			(ent->client->pers.lastweapons[4] == &gI_weapon_grenadelauncher))
			&&((it == ent->client->pers.weapon
			&& ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_bucky)])
			|| (it != ent->client->pers.weapon
			&& (!ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_grenadelauncher)]
			|| (ent->client->pers.inventory[ITEM_INDEX(gI_weapon_grenadelauncher.ammo)]
			< gI_weapon_grenadelauncher.quantity
			&& !g_select_empty->value)))))
		{
			it = &gI_weapon_bucky;
		}
		
		else if (((ent->client->pers.lastweapons[4] == &gI_weapon_bucky) ||
			(ent->client->pers.lastweapons[4] == &gI_weapon_bazooka)||
			(ent->client->pers.lastweapons[4] == &gI_weapon_grenadelauncher))
			&&((it == ent->client->pers.weapon
			&& ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_doubleimpact)])
			|| (it != ent->client->pers.weapon
			&& (!ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_grenadelauncher)]
			|| (ent->client->pers.inventory[ITEM_INDEX(gI_weapon_grenadelauncher.ammo)]
			< gI_weapon_grenadelauncher.quantity
			&& !g_select_empty->value)))))
		{
			it = &gI_weapon_doubleimpact;
		}
		
		else if (((ent->client->pers.lastweapons[4] == &gI_weapon_doubleimpact)||
			(ent->client->pers.lastweapons[4] == &gI_weapon_bucky) ||
			(ent->client->pers.lastweapons[4] == &gI_weapon_bazooka)||
			(ent->client->pers.lastweapons[4] == &gI_weapon_grenadelauncher))
			&&((it == ent->client->pers.weapon
			&& ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_floatingminelauncher)])
			|| (it != ent->client->pers.weapon
			&& (!ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_grenadelauncher)]
			|| (ent->client->pers.inventory[ITEM_INDEX(gI_weapon_grenadelauncher.ammo)]
			< gI_weapon_grenadelauncher.quantity
			&& !g_select_empty->value)))))
		{
			it = &gI_weapon_floatingminelauncher;
		}
		
		else if (((ent->client->pers.lastweapons[4] == &gI_weapon_floatingminelauncher)||
			(ent->client->pers.lastweapons[4] == &gI_weapon_doubleimpact)||
			(ent->client->pers.lastweapons[4] == &gI_weapon_bucky) ||
			(ent->client->pers.lastweapons[4] == &gI_weapon_bazooka)||
			(ent->client->pers.lastweapons[4] == &gI_weapon_grenadelauncher))
			&&((it == ent->client->pers.weapon
			&& ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_stickinggrenadelauncher)])
			|| (it != ent->client->pers.weapon
			&& (!ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_grenadelauncher)]
			|| (ent->client->pers.inventory[ITEM_INDEX(gI_weapon_grenadelauncher.ammo)]
			< gI_weapon_grenadelauncher.quantity
			&& !g_select_empty->value)))))
		{
			it = &gI_weapon_stickinggrenadelauncher;
		}
		else 
			it =  &gI_weapon_grenadelauncher;
		ent->client->pers.lastweapons[4] = it;
		
		
	}
	
	else if (it == &gI_weapon_chaingun)
	{
		it = ent->client->pers.lastweapons[5];
		if ((ent->client->pers.lastweapons[5] == &gI_weapon_chaingun)
			&&((it == ent->client->pers.weapon
			&& ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_streetsweeper)])
			|| (it != ent->client->pers.weapon
			&& (!ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_chaingun)]
			|| (ent->client->pers.inventory[ITEM_INDEX(gI_weapon_chaingun.ammo)]
			< gI_weapon_chaingun.quantity
			&& !g_select_empty->value)))))
		{
			it = &gI_weapon_streetsweeper;
		}
		
		else if (((ent->client->pers.lastweapons[5] == &gI_weapon_streetsweeper)||
			(ent->client->pers.lastweapons[5] == &gI_weapon_chaingun))
			&&((it == ent->client->pers.weapon
			&& ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_explosivechaingun)])
			|| (it != ent->client->pers.weapon
			&& (!ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_chaingun)]
			|| (ent->client->pers.inventory[ITEM_INDEX(gI_weapon_chaingun.ammo)]
			< gI_weapon_chaingun.quantity
			&& !g_select_empty->value)))))
		{
			it = &gI_weapon_explosivechaingun;
		}
		
		else if (((ent->client->pers.lastweapons[5] == &gI_weapon_explosivechaingun)||
			(ent->client->pers.lastweapons[5] == &gI_weapon_streetsweeper)||
			(ent->client->pers.lastweapons[5] == &gI_weapon_chaingun))
			&&((it == ent->client->pers.weapon
			&& ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_explosivestreetsweeper)])
			|| (it != ent->client->pers.weapon
			&& (!ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_chaingun)]
			|| (ent->client->pers.inventory[ITEM_INDEX(gI_weapon_chaingun.ammo)]
			< gI_weapon_chaingun.quantity
			&& !g_select_empty->value)))))
		{
			it = &gI_weapon_explosivestreetsweeper;
		}
		
		else if (((ent->client->pers.lastweapons[5] == &gI_weapon_explosivestreetsweeper)||
			(ent->client->pers.lastweapons[5] == &gI_weapon_explosivechaingun)||
			(ent->client->pers.lastweapons[5] == &gI_weapon_streetsweeper)||
			(ent->client->pers.lastweapons[5] == &gI_weapon_chaingun))
			&&((it == ent->client->pers.weapon
			&& ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_superblastersweeper)])
			|| (it != ent->client->pers.weapon
			&& (!ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_chaingun)]
			|| (ent->client->pers.inventory[ITEM_INDEX(gI_weapon_chaingun.ammo)]
			< gI_weapon_chaingun.quantity
			&& !g_select_empty->value)))))
		{
			it = &gI_weapon_superblastersweeper;
		}
		
		else if (((ent->client->pers.lastweapons[5] == &gI_weapon_superblastersweeper)||
			(ent->client->pers.lastweapons[5] == &gI_weapon_explosivestreetsweeper)||
			(ent->client->pers.lastweapons[5] == &gI_weapon_explosivechaingun)||
			(ent->client->pers.lastweapons[5] == &gI_weapon_streetsweeper)||
			(ent->client->pers.lastweapons[5] == &gI_weapon_chaingun))
			&&((it == ent->client->pers.weapon
			&& ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_freezersweeper)])
			|| (it != ent->client->pers.weapon
			&& (!ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_chaingun)]
			|| (ent->client->pers.inventory[ITEM_INDEX(gI_weapon_chaingun.ammo)]
			< gI_weapon_chaingun.quantity
			&& !g_select_empty->value)))))
		{
			it = &gI_weapon_freezersweeper;
		}
		
		else if (((ent->client->pers.lastweapons[5] == &gI_weapon_freezersweeper)||
			(ent->client->pers.lastweapons[5] == &gI_weapon_superblastersweeper)||
			(ent->client->pers.lastweapons[5] == &gI_weapon_explosivestreetsweeper)||
			(ent->client->pers.lastweapons[5] == &gI_weapon_explosivechaingun)||
			(ent->client->pers.lastweapons[5] == &gI_weapon_streetsweeper)||
			(ent->client->pers.lastweapons[5] == &gI_weapon_chaingun))
			&&((it == ent->client->pers.weapon
			&& ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_flamesweeper)])
			|| (it != ent->client->pers.weapon
			&& (!ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_chaingun)]
			|| (ent->client->pers.inventory[ITEM_INDEX(gI_weapon_chaingun.ammo)]
			< gI_weapon_chaingun.quantity
			&& !g_select_empty->value)))))
		{
			it = &gI_weapon_flamesweeper;
		}
		
		else if (((ent->client->pers.lastweapons[5] == &gI_weapon_flamesweeper)||
			(ent->client->pers.lastweapons[5] == &gI_weapon_freezersweeper)||
			(ent->client->pers.lastweapons[5] == &gI_weapon_superblastersweeper)||
			(ent->client->pers.lastweapons[5] == &gI_weapon_explosivestreetsweeper)||
			(ent->client->pers.lastweapons[5] == &gI_weapon_explosivechaingun)||
			(ent->client->pers.lastweapons[5] == &gI_weapon_streetsweeper)||
			(ent->client->pers.lastweapons[5] == &gI_weapon_chaingun))
			&&((it == ent->client->pers.weapon
			&& ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_grenadesweeper)])
			|| (it != ent->client->pers.weapon
			&& (!ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_chaingun)]
			|| (ent->client->pers.inventory[ITEM_INDEX(gI_weapon_chaingun.ammo)]
			< gI_weapon_chaingun.quantity
			&& !g_select_empty->value)))))
		{
			it = &gI_weapon_grenadesweeper;
		}
		
		else if (((ent->client->pers.lastweapons[5] == &gI_weapon_grenadesweeper)||
			(ent->client->pers.lastweapons[5] == &gI_weapon_flamesweeper)||
			(ent->client->pers.lastweapons[5] == &gI_weapon_freezersweeper)||
			(ent->client->pers.lastweapons[5] == &gI_weapon_superblastersweeper)||
			(ent->client->pers.lastweapons[5] == &gI_weapon_explosivestreetsweeper)||
			(ent->client->pers.lastweapons[5] == &gI_weapon_explosivechaingun)||
			(ent->client->pers.lastweapons[5] == &gI_weapon_streetsweeper)||
			(ent->client->pers.lastweapons[5] == &gI_weapon_chaingun))
			&&((it == ent->client->pers.weapon
			&& ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_bazookasweeper)])
			|| (it != ent->client->pers.weapon
			&& (!ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_chaingun)]
			|| (ent->client->pers.inventory[ITEM_INDEX(gI_weapon_chaingun.ammo)]
			< gI_weapon_chaingun.quantity
			&& !g_select_empty->value)))))
		{
			it = &gI_weapon_bazookasweeper;
		}
		
		else if (((ent->client->pers.lastweapons[5] == &gI_weapon_bazookasweeper)||
			(ent->client->pers.lastweapons[5] == &gI_weapon_grenadesweeper)||
			(ent->client->pers.lastweapons[5] == &gI_weapon_flamesweeper)||
			(ent->client->pers.lastweapons[5] == &gI_weapon_freezersweeper)||
			(ent->client->pers.lastweapons[5] == &gI_weapon_superblastersweeper)||
			(ent->client->pers.lastweapons[5] == &gI_weapon_explosivestreetsweeper)||
			(ent->client->pers.lastweapons[5] == &gI_weapon_explosivechaingun)||
			(ent->client->pers.lastweapons[5] == &gI_weapon_streetsweeper)||
			(ent->client->pers.lastweapons[5] == &gI_weapon_chaingun))
			&&((it == ent->client->pers.weapon
			&& ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_snipersweeper)])
			|| (it != ent->client->pers.weapon
			&& (!ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_chaingun)]
			|| (ent->client->pers.inventory[ITEM_INDEX(gI_weapon_chaingun.ammo)]
			< gI_weapon_chaingun.quantity
			&& !g_select_empty->value)))))
		{
			it = &gI_weapon_snipersweeper;
		}
		
		else if (((ent->client->pers.lastweapons[5] == &gI_weapon_snipersweeper)||
			(ent->client->pers.lastweapons[5] == &gI_weapon_bazookasweeper)||
			(ent->client->pers.lastweapons[5] == &gI_weapon_grenadesweeper)||
			(ent->client->pers.lastweapons[5] == &gI_weapon_flamesweeper)||
			(ent->client->pers.lastweapons[5] == &gI_weapon_freezersweeper)||
			(ent->client->pers.lastweapons[5] == &gI_weapon_superblastersweeper)||
			(ent->client->pers.lastweapons[5] == &gI_weapon_explosivestreetsweeper)||
			(ent->client->pers.lastweapons[5] == &gI_weapon_explosivechaingun)||
			(ent->client->pers.lastweapons[5] == &gI_weapon_streetsweeper)||
			(ent->client->pers.lastweapons[5] == &gI_weapon_chaingun))
			&&((it == ent->client->pers.weapon
			&& ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_railgunsweeper)])
			|| (it != ent->client->pers.weapon
			&& (!ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_chaingun)]
			|| (ent->client->pers.inventory[ITEM_INDEX(gI_weapon_chaingun.ammo)]
			< gI_weapon_chaingun.quantity
			&& !g_select_empty->value)))))
		{
			it = &gI_weapon_railgunsweeper;
		}
		
		else if (((ent->client->pers.lastweapons[5] == &gI_weapon_railgunsweeper)||
			(ent->client->pers.lastweapons[5] == &gI_weapon_snipersweeper)||
			(ent->client->pers.lastweapons[5] == &gI_weapon_bazookasweeper)||
			(ent->client->pers.lastweapons[5] == &gI_weapon_grenadesweeper)||
			(ent->client->pers.lastweapons[5] == &gI_weapon_flamesweeper)||
			(ent->client->pers.lastweapons[5] == &gI_weapon_freezersweeper)||
			(ent->client->pers.lastweapons[5] == &gI_weapon_superblastersweeper)||
			(ent->client->pers.lastweapons[5] == &gI_weapon_explosivestreetsweeper)||
			(ent->client->pers.lastweapons[5] == &gI_weapon_explosivechaingun)||
			(ent->client->pers.lastweapons[5] == &gI_weapon_streetsweeper)||
			(ent->client->pers.lastweapons[5] == &gI_weapon_chaingun))
			&&((it == ent->client->pers.weapon
			&& ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_chunkgun)])
			|| (it != ent->client->pers.weapon
			&& (!ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_chaingun)]
			|| (ent->client->pers.inventory[ITEM_INDEX(gI_weapon_chaingun.ammo)]
			< gI_weapon_chaingun.quantity
			&& !g_select_empty->value)))))
		{
			it = &gI_weapon_chunkgun;
		}
		
		else if (((ent->client->pers.lastweapons[5] == &gI_weapon_chunkgun)||
			(ent->client->pers.lastweapons[5] == &gI_weapon_railgunsweeper)||
			(ent->client->pers.lastweapons[5] == &gI_weapon_snipersweeper)||
			(ent->client->pers.lastweapons[5] == &gI_weapon_bazookasweeper)||
			(ent->client->pers.lastweapons[5] == &gI_weapon_grenadesweeper)||
			(ent->client->pers.lastweapons[5] == &gI_weapon_flamesweeper)||
			(ent->client->pers.lastweapons[5] == &gI_weapon_freezersweeper)||
			(ent->client->pers.lastweapons[5] == &gI_weapon_superblastersweeper)||
			(ent->client->pers.lastweapons[5] == &gI_weapon_explosivestreetsweeper)||
			(ent->client->pers.lastweapons[5] == &gI_weapon_explosivechaingun)||
			(ent->client->pers.lastweapons[5] == &gI_weapon_streetsweeper)||
			(ent->client->pers.lastweapons[5] == &gI_weapon_chaingun))
			&&((it == ent->client->pers.weapon
			&& ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_stickygrenadesweeper)])
			|| (it != ent->client->pers.weapon
			&& (!ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_chaingun)]
			|| (ent->client->pers.inventory[ITEM_INDEX(gI_weapon_chaingun.ammo)]
			< gI_weapon_chaingun.quantity
			&& !g_select_empty->value)))))
		{
			it = &gI_weapon_stickygrenadesweeper;
		}
		else 
			it =  &gI_weapon_chaingun;
		ent->client->pers.lastweapons[5] = it;
		
	}
	
	else if (it == &gI_weapon_rocketlauncher)
	{
		it = ent->client->pers.lastweapons[6];
		if ((ent->client->pers.lastweapons[6] == &gI_weapon_rocketlauncher)
			&&((it == ent->client->pers.weapon
			&& ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_homing)])
			|| (it != ent->client->pers.weapon
			&& (!ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_rocketlauncher)]
			|| (ent->client->pers.inventory[ITEM_INDEX(gI_weapon_rocketlauncher.ammo)]
			< gI_weapon_rocketlauncher.quantity
			&& !g_select_empty->value)))))
		{
			it = &gI_weapon_homing;
		}
		
		else if (((ent->client->pers.lastweapons[6] == &gI_weapon_homing)||
			(ent->client->pers.lastweapons[6] == &gI_weapon_rocketlauncher))
			&&((it == ent->client->pers.weapon
			&& ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_skipper)])
			|| (it != ent->client->pers.weapon
			&& (!ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_rocketlauncher)]
			|| (ent->client->pers.inventory[ITEM_INDEX(gI_weapon_rocketlauncher.ammo)]
			< gI_weapon_rocketlauncher.quantity
			&& !g_select_empty->value)))))
		{
			it = &gI_weapon_skipper;
		}
		
		else if (((ent->client->pers.lastweapons[6] == &gI_weapon_skipper)||
			(ent->client->pers.lastweapons[6] == &gI_weapon_homing)||
			(ent->client->pers.lastweapons[6] == &gI_weapon_rocketlauncher))
			&&((it == ent->client->pers.weapon
			&& ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_guided)])
			|| (it != ent->client->pers.weapon
			&& (!ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_rocketlauncher)]
			|| (ent->client->pers.inventory[ITEM_INDEX(gI_weapon_rocketlauncher.ammo)]
			< gI_weapon_rocketlauncher.quantity
			&& !g_select_empty->value)))))
		{
			it = &gI_weapon_guided;
		}
		
		else if (((ent->client->pers.lastweapons[6] == &gI_weapon_guided)||
			(ent->client->pers.lastweapons[6] == &gI_weapon_skipper)||
			(ent->client->pers.lastweapons[6] == &gI_weapon_homing)||
			(ent->client->pers.lastweapons[6] == &gI_weapon_rocketlauncher))
			&&((it == ent->client->pers.weapon
			&& ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_spiralrocketlauncher)])
			|| (it != ent->client->pers.weapon
			&& (!ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_rocketlauncher)]
			|| (ent->client->pers.inventory[ITEM_INDEX(gI_weapon_rocketlauncher.ammo)]
			< gI_weapon_rocketlauncher.quantity
			&& !g_select_empty->value)))))
		{
			it = &gI_weapon_spiralrocketlauncher;
			
		}
		
		else if (((ent->client->pers.lastweapons[6] == &gI_weapon_spiralrocketlauncher)||
			(ent->client->pers.lastweapons[6] == &gI_weapon_guided)||
			(ent->client->pers.lastweapons[6] == &gI_weapon_skipper)||
			(ent->client->pers.lastweapons[6] == &gI_weapon_homing)||
			(ent->client->pers.lastweapons[6] == &gI_weapon_rocketlauncher))
			&&((it == ent->client->pers.weapon
			&& ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_guidednuke)])
			|| (it != ent->client->pers.weapon
			&& (!ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_guidednuke)]
			|| (ent->client->pers.inventory[ITEM_INDEX(gI_weapon_guidednuke.ammo)]
			< gI_weapon_rocketlauncher.quantity
			&& !g_select_empty->value)))))
		{
			it = &gI_weapon_guidednuke;
		}
		
		else 
			it =  &gI_weapon_rocketlauncher;
		ent->client->pers.lastweapons[6] = it;
	}
	
	
	else if (it == &gI_weapon_hyperblaster)
	{
		it = ent->client->pers.lastweapons[7];
		if ((ent->client->pers.lastweapons[7] == &gI_weapon_hyperblaster)
			&&((it == ent->client->pers.weapon
			&& ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_plasma)])
			|| (it != ent->client->pers.weapon
			&& (!ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_hyperblaster)]
			|| (ent->client->pers.inventory[ITEM_INDEX(gI_weapon_hyperblaster.ammo)]
			< gI_weapon_hyperblaster.quantity
			&& !g_select_empty->value)))))
		{
			it = &gI_weapon_plasma;
		}
		
		else if (((ent->client->pers.lastweapons[7] == &gI_weapon_plasma) ||
			(ent->client->pers.lastweapons[7] == &gI_weapon_hyperblaster))
			&&((it == ent->client->pers.weapon
			&& ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_positron)])
			|| (it != ent->client->pers.weapon
			&& (!ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_hyperblaster)]
			|| (ent->client->pers.inventory[ITEM_INDEX(gI_weapon_hyperblaster.ammo)]
			< gI_weapon_hyperblaster.quantity
			&& !g_select_empty->value)))))
		{
			it = &gI_weapon_positron;
		}
		
		else if (((ent->client->pers.lastweapons[7] == &gI_weapon_positron) ||
			(ent->client->pers.lastweapons[7] == &gI_weapon_plasma) ||
			(ent->client->pers.lastweapons[7] == &gI_weapon_hyperblaster))
			&&((it == ent->client->pers.weapon
			&& ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_freezatron)])
			|| (it != ent->client->pers.weapon
			&& (!ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_hyperblaster)]
			|| (ent->client->pers.inventory[ITEM_INDEX(gI_weapon_hyperblaster.ammo)]
			< gI_weapon_hyperblaster.quantity
			&& !g_select_empty->value)))))
		{
			it = &gI_weapon_freezatron;
		}
		
		else if (((ent->client->pers.lastweapons[7] == &gI_weapon_freezatron) ||
			(ent->client->pers.lastweapons[7] == &gI_weapon_positron) ||
			(ent->client->pers.lastweapons[7] == &gI_weapon_plasma) ||
			(ent->client->pers.lastweapons[7] == &gI_weapon_hyperblaster))
			&&((it == ent->client->pers.weapon
			&& ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_lightninggun)])
			|| (it != ent->client->pers.weapon
			&& (!ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_hyperblaster)]
			|| (ent->client->pers.inventory[ITEM_INDEX(gI_weapon_hyperblaster.ammo)]
			< gI_weapon_hyperblaster.quantity
			&& !g_select_empty->value)))))
		{
			it = &gI_weapon_lightninggun;
		}
		
		else if (((ent->client->pers.lastweapons[7] == &gI_weapon_lightninggun) ||
			(ent->client->pers.lastweapons[7] == &gI_weapon_freezatron) ||
			(ent->client->pers.lastweapons[7] == &gI_weapon_positron) ||
			(ent->client->pers.lastweapons[7] == &gI_weapon_plasma) ||
			(ent->client->pers.lastweapons[7] == &gI_weapon_hyperblaster))
			&&((it == ent->client->pers.weapon
			&& ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_icecubegun)])
			|| (it != ent->client->pers.weapon
			&& (!ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_hyperblaster)]
			|| (ent->client->pers.inventory[ITEM_INDEX(gI_weapon_hyperblaster.ammo)]
			< gI_weapon_hyperblaster.quantity
			&& !g_select_empty->value)))))
		{
			it = &gI_weapon_icecubegun;
		}
		
		else if (((ent->client->pers.lastweapons[7] == &gI_weapon_icecubegun) ||
			(ent->client->pers.lastweapons[7] == &gI_weapon_lightninggun) ||
			(ent->client->pers.lastweapons[7] == &gI_weapon_freezatron) ||
			(ent->client->pers.lastweapons[7] == &gI_weapon_positron) ||
			(ent->client->pers.lastweapons[7] == &gI_weapon_plasma) ||
			(ent->client->pers.lastweapons[7] == &gI_weapon_hyperblaster))
			&&((it == ent->client->pers.weapon
			&& ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_disintegrator)])
			|| (it != ent->client->pers.weapon
			&& (!ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_hyperblaster)]
			|| (ent->client->pers.inventory[ITEM_INDEX(gI_weapon_hyperblaster.ammo)]
			< gI_weapon_hyperblaster.quantity
			&& !g_select_empty->value)))))
		{
			it = &gI_weapon_disintegrator;
		}
		
		else 
			it =  &gI_weapon_hyperblaster;
		ent->client->pers.lastweapons[7] = it;
	}
	
	else if (it == &gI_weapon_railgun)
	{
		it = ent->client->pers.lastweapons[8];
		if ((ent->client->pers.lastweapons[8] == &gI_weapon_railgun)
			&&((it == ent->client->pers.weapon
			&& ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_railgun2)])
			|| (it != ent->client->pers.weapon
			&& (!ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_railgun)]
			|| (ent->client->pers.inventory[ITEM_INDEX(gI_weapon_railgun.ammo)]
			< gI_weapon_railgun.quantity
			&& !g_select_empty->value)))))
		{
			it = &gI_weapon_railgun2;
		}
		
		else if (((ent->client->pers.lastweapons[8] == &gI_weapon_railgun2)||
			(ent->client->pers.lastweapons[8] == &gI_weapon_railgun))
			&&((it == ent->client->pers.weapon
			&& ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_wallpiercingrailgun)])
			|| (it != ent->client->pers.weapon
			&& (!ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_railgun)]
			|| (ent->client->pers.inventory[ITEM_INDEX(gI_weapon_railgun.ammo)]
			< gI_weapon_railgun.quantity
			&& !g_select_empty->value)))))
		{
			it = &gI_weapon_wallpiercingrailgun;
		}
		
		else if (((ent->client->pers.lastweapons[8] == &gI_weapon_wallpiercingrailgun)||
			(ent->client->pers.lastweapons[8] == &gI_weapon_railgun2)||
			(ent->client->pers.lastweapons[8] == &gI_weapon_railgun))
			&&((it == ent->client->pers.weapon
			&& ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_sonicrailgun)])
			|| (it != ent->client->pers.weapon
			&& (!ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_railgun)]
			|| (ent->client->pers.inventory[ITEM_INDEX(gI_weapon_railgun.ammo)]
			< gI_weapon_railgun.quantity
			&& !g_select_empty->value)))))
		{
			it = &gI_weapon_sonicrailgun;
		}
		
		else if (((ent->client->pers.lastweapons[8] == &gI_weapon_sonicrailgun)||
			(ent->client->pers.lastweapons[8] == &gI_weapon_wallpiercingrailgun)||
			(ent->client->pers.lastweapons[8] == &gI_weapon_railgun2)||
			(ent->client->pers.lastweapons[8] == &gI_weapon_railgun))
			&&((it == ent->client->pers.weapon
			&& ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_antimattercannon)])
			|| (it != ent->client->pers.weapon
			&& (!ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_railgun)]
			|| (ent->client->pers.inventory[ITEM_INDEX(gI_weapon_railgun.ammo)]
			< gI_weapon_railgun.quantity
			&& !g_select_empty->value)))))
		{
			it = &gI_weapon_antimattercannon;
		}
		
		else if (((ent->client->pers.lastweapons[8] == &gI_weapon_antimattercannon)||
			(ent->client->pers.lastweapons[8] == &gI_weapon_sonicrailgun)||
			(ent->client->pers.lastweapons[8] == &gI_weapon_wallpiercingrailgun)||
			(ent->client->pers.lastweapons[8] == &gI_weapon_railgun2)||
			(ent->client->pers.lastweapons[8] == &gI_weapon_railgun))
			&&((it == ent->client->pers.weapon
			&& ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_nuke)])
			|| (it != ent->client->pers.weapon
			&& (!ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_railgun)]
			|| (ent->client->pers.inventory[ITEM_INDEX(gI_weapon_railgun.ammo)]
			< gI_weapon_railgun.quantity
			&& !g_select_empty->value)))))
		{
			it = &gI_weapon_nuke;
		}
		
		else if (((ent->client->pers.lastweapons[8] == &gI_weapon_nuke)||
			(ent->client->pers.lastweapons[8] == &gI_weapon_antimattercannon)||
			(ent->client->pers.lastweapons[8] == &gI_weapon_sonicrailgun)||
			(ent->client->pers.lastweapons[8] == &gI_weapon_wallpiercingrailgun)||
			(ent->client->pers.lastweapons[8] == &gI_weapon_railgun2)||
			(ent->client->pers.lastweapons[8] == &gI_weapon_railgun))
			&&((it == ent->client->pers.weapon
			&& ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_durg)])
			|| (it != ent->client->pers.weapon
			&& (!ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_railgun)]
			|| (ent->client->pers.inventory[ITEM_INDEX(gI_weapon_railgun.ammo)]
			< gI_weapon_railgun.quantity
			&& !g_select_empty->value)))))
		{
			it = &gI_weapon_durg;
		}
		
		else if (((ent->client->pers.lastweapons[8] == &gI_weapon_durg)||
			(ent->client->pers.lastweapons[8] == &gI_weapon_nuke)||
			(ent->client->pers.lastweapons[8] == &gI_weapon_antimattercannon)||
			(ent->client->pers.lastweapons[8] == &gI_weapon_sonicrailgun)||
			(ent->client->pers.lastweapons[8] == &gI_weapon_wallpiercingrailgun)||
			(ent->client->pers.lastweapons[8] == &gI_weapon_railgun2)||
			(ent->client->pers.lastweapons[8] == &gI_weapon_railgun))
			&&((it == ent->client->pers.weapon
			&& ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_kaminit)])
			|| (it != ent->client->pers.weapon
			&& (!ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_railgun)]
			|| (ent->client->pers.inventory[ITEM_INDEX(gI_weapon_railgun.ammo)]
			< gI_weapon_railgun.quantity
			&& !g_select_empty->value)))))
		{
			it = &gI_weapon_kaminit;
		}
		
		else 
			it =  &gI_weapon_railgun;
		ent->client->pers.lastweapons[8] = it;
	}
	
	else if (it == &gI_weapon_bfg)
	{
		it = ent->client->pers.lastweapons[9];
		if ((ent->client->pers.lastweapons[9] == &gI_weapon_bfg)
			&&((it == ent->client->pers.weapon
			&& ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_fbfg)])
			|| (it != ent->client->pers.weapon
			&& (!ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_bfg)]
			|| (ent->client->pers.inventory[ITEM_INDEX(gI_weapon_bfg.ammo)]
			< gI_weapon_bfg.quantity
			&& !g_select_empty->value)))))
		{
			it = &gI_weapon_fbfg;
		}
		
		else if (((ent->client->pers.lastweapons[9] == &gI_weapon_fbfg)||
			(ent->client->pers.lastweapons[9] == &gI_weapon_bfg))
			&&((it == ent->client->pers.weapon
			&& ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_energyvortex)])
			|| (it != ent->client->pers.weapon
			&& (!ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_bfg)]
			|| (ent->client->pers.inventory[ITEM_INDEX(gI_weapon_bfg.ammo)]
			< gI_weapon_bfg.quantity
			&& !g_select_empty->value)))))
		{
			it = &gI_weapon_energyvortex;
		}
		
		else if (((ent->client->pers.lastweapons[9] == &gI_weapon_energyvortex)||
			(ent->client->pers.lastweapons[9] == &gI_weapon_fbfg)||
			(ent->client->pers.lastweapons[9] == &gI_weapon_bfg))
			&&((it == ent->client->pers.weapon
			&& ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_vacuummaker)])
			|| (it != ent->client->pers.weapon
			&& (!ent->client->pers.inventory[ITEM_INDEX(&gI_weapon_bfg)]
			|| (ent->client->pers.inventory[ITEM_INDEX(gI_weapon_bfg.ammo)]
			< gI_weapon_bfg.quantity
			&& !g_select_empty->value)))))
		{
			it = &gI_weapon_vacuummaker;
		}
		else 
			it =  &gI_weapon_bfg;
		ent->client->pers.lastweapons[9] = it;
	}
	
	else if (it == &gI_ammo_grenades)
	{
		// If grenades are already selected, move to the next type.
		if (it == ent->client->pers.weapon)
		{
			// Move to the next grenade type, skipping the ones that are not in
			// our inventory.
			do
			{
				ent->client->dM_grenade += 1;
				it = itemlist[ITEM_INDEX(&gI_ammo_grenades)
					+ ent->client->dM_grenade];
			} while (it != &gI_weapon_grenadelauncher
				&& !ent->client->pers.inventory[ITEM_INDEX(it)]);
			
			// If we've gone past the last type, wrap around to the beginning.
			if (it == &gI_weapon_grenadelauncher)
			{
				ent->client->dM_grenade = DM_NORMALGRENADE;
				it = &gI_ammo_grenades;
			}
		}
		
		// If grenades were not already selected, restore their old choice.
		else
			it = itemlist[ITEM_INDEX(&gI_ammo_grenades) + ent->client->dM_grenade];
	}
	
	// test if they have item
	index = ITEM_INDEX (it);
	if (!ent->client->pers.inventory[index])
	{
		gi.cprintf (ent, PRINT_HIGH, "Out of item: %s\n", s);
		return;
	}
	
	// Print what they selected.  Some of the normal weapons have been replaced
	// by the alternates, and all the alternates need to be printed.  Also, we
	// already printed the message for the grenades.
	if (it == &gI_weapon_machinegun)
	{
		gi.cprintf (ent, PRINT_MEDIUM, "Machine Rocket Gun\n");
	}
	else if (it == &gI_weapon_grenadelauncher)
	{
		if (ent->client->dM_grenade == DM_NORMALGRENADE)
			gi.cprintf (ent, PRINT_MEDIUM, "Fire Grenade Launcher\n");
		else
			gi.cprintf (ent, PRINT_MEDIUM, "%s Launcher\n",
			itemlist[ITEM_INDEX(&gI_ammo_grenades) + ent->client->dM_grenade]
			->pickup_name);
	}
	else if (it == &gI_weapon_bazooka)
	{
		if (ent->client->dM_grenade == DM_NORMALGRENADE)
			gi.cprintf (ent, PRINT_MEDIUM, "Bazooka\n");
		else
			gi.cprintf (ent, PRINT_MEDIUM, "%s Bazooka\n",
			itemlist[ITEM_INDEX(&gI_ammo_grenades) + ent->client->dM_grenade]
			->pickup_name);
	}
	else if (it == &gI_weapon_rocketlauncher)
	{
		gi.cprintf (ent, PRINT_MEDIUM, "Flame Rocket Launcher\n");
	}
	
	else if (it == &gI_weapon_railgun)
	{
		gi.cprintf (ent, PRINT_MEDIUM, "Flamethrower\n");
	}
	else if (it == &gI_weapon_railgun2)
	{
		gi.cprintf (ent, PRINT_MEDIUM, "Railgun\n");
	}
	else if (it == &gI_weapon_sonicrailgun)
	{
		gi.cprintf (ent, PRINT_MEDIUM, "Sonic Railgun\n");
	}
	
	else if (it == &gI_weapon_durg)
	{
		gi.cprintf (ent, PRINT_MEDIUM, "Durg\n");
	}
	
	else if (it == &gI_ammo_grenades)
	{
		if (ent->client->dM_grenade == DM_NORMALGRENADE)
			gi.cprintf (ent, PRINT_MEDIUM, "Standard Grenade\n");
		else
			gi.cprintf (ent, PRINT_MEDIUM, "%s\n",
			itemlist[ITEM_INDEX(&gI_ammo_grenades) + ent->client->dM_grenade]
			->pickup_name);
	}
	else if (it->pickup_name)
	{
		// Show them the pickup name.
		gi.cprintf (ent, PRINT_MEDIUM, "%s\n", it->pickup_name);
	}
	
	// Try to use it.
	it->use (ent, it);
//	STOP_PERFORMANCE_TIMER(__func__);
}

//==================
// Cmd_Drop_f
//
// Drop an inventory item
//==================
void Cmd_Drop_f (edict_t *ent)
{
	int			index;
	gitem_t		*it;
	char		*s;
	
	s = gi.args();
	it = FindItem (s);
	if (!it)
	{
		gi.cprintf (ent, PRINT_HIGH, "unknown item: %s\n", s);
		return;
	}

	if (!it->drop)
	{
		gi.cprintf (ent, PRINT_HIGH, "Item cannot be dropped.\n");
		return;
	}

	index = ITEM_INDEX(it);
	
	if (!ent->client->pers.inventory[index])
	{
		gi.cprintf (ent, PRINT_HIGH, "Out of item: %s\n", s);
		return;
	}
	
	it->drop (ent, it);
}


//  =================
//  Cmd_Inven_f
//  =================
void Cmd_Inven_f (edict_t *ent)
{
	int			i;
	gclient_t	*cl;
	
	cl = ent->client;
	
	cl->showscores = QFALSE;
	cl->showhelp = QFALSE;
	
	//ZOID
	if (ent->client->menu)
	{
		PMenu_Close(ent);
		ent->client->update_chase = QTRUE;
		return;
	}
	//ZOID
	
	if (cl->showinventory)
	{
		cl->showinventory = QFALSE;
		return;
	}
	
	//ZOID
	if (ctf->value && cl->resp.ctf_team == CTF_NOTEAM)
	{
		CTFOpenJoinMenu(ent);
		return;
	}
	//ZOID
	
	cl->showinventory = QTRUE;
	
	gi.WriteByte (svc_inventory);
	for (i=0 ; i<MAX_ITEMS ; i++)
	{
		gi.WriteShort (cl->pers.inventory[i]);
	}
	gi.unicast (ent, QTRUE);
	
	if (cl->pers.scanner_active & 1) // if it's on
		cl->pers.scanner_active = 2; // set the changed bit
	
	if (cl->heat_active & 1)
		cl->heat_active = 2;
}

//  =================
//  Cmd_InvUse_f
//  =================
void Cmd_InvUse_f (edict_t *ent)
{
	gitem_t		*it;
	
	//ZOID
	if (ent->client->menu) 
	{
		PMenu_Select(ent);
		return;
	}
	//ZOID
	
	ValidateSelectedItem (ent);
	
	if (ent->client->pers.selected_item == -1)
	{
		gi.cprintf (ent, PRINT_HIGH, "No item to use.\n");
		return;
	}
	
	it = itemlist[ent->client->pers.selected_item];
	if (!it->use)
	{
		gi.cprintf (ent, PRINT_HIGH, "Item is not usable.\n");
		return;
	}
	it->use (ent, it);
}

//  =================
//  Cmd_WeapPrev_f
//  =================
void Cmd_WeapPrev_f (edict_t *ent)
{
	gclient_t	*cl;
	unsigned int i, index;
	gitem_t		*it;
	unsigned int selected_weapon;
	
	cl = ent->client;	
	
	if (!cl->pers.weapon)
		return;
	
	selected_weapon = ((unsigned)(ITEM_INDEX(cl->pers.weapon)));
	
	// scan  for the next valid one
	for (i=1 ; i <= MAX_ITEMS ; i++)
	{
		// index = (selected_weapon + i)%MAX_ITEMS;
		index = (selected_weapon + i) & (MAX_ITEMS-1);
		if (!cl->pers.inventory[index])
			continue;
		it = itemlist[index];
		if (!it->use)
			continue;
		if (! (it->flags & IT_WEAPON) )
			continue;
		it->use (ent, it);
		if (cl->newweapon == it)
			return;	// successful
	}
}

//  =================
//  Cmd_WeapNext_f
//  =================
void Cmd_WeapNext_f (edict_t *ent)
{
	gclient_t	*cl;
	unsigned int i, index;
	gitem_t		*it;
	unsigned int selected_weapon;
	
	cl = ent->client;
	if (!cl->pers.weapon)
		return;
	
	selected_weapon = ((unsigned)(ITEM_INDEX (cl->pers.weapon)));
	
	// scan  for the next valid one
	for (i=1 ; i<=MAX_ITEMS ; i++)
	{
		// index = (selected_weapon + MAX_ITEMS - i)%MAX_ITEMS;
		index = (selected_weapon + MAX_ITEMS - i) & (MAX_ITEMS-1);
		if (!cl->pers.inventory[index])
			continue;
		it = itemlist[index];
		if (!it->use)
			continue;
		if (!(it->flags & IT_WEAPON))
			continue;
		it->use (ent, it);
		if (cl->newweapon == it) 
			return;	// successful
	}
}

//  =================
//  Cmd_WeapLast_f
//  =================
void Cmd_WeapLast_f (edict_t *ent)
{
	gclient_t	*cl;
	int			index;
	gitem_t		*it;
	
	cl = ent->client;
	if (!cl->pers.weapon || !cl->pers.lastweapon)
		return;
	
	index = ITEM_INDEX(cl->pers.lastweapon);
	if (!cl->pers.inventory[index])
		return;
	it = itemlist[index];
	if (!it->use)
		return;
	if (! (it->flags & IT_WEAPON) )
		return;
	it->use (ent, it);
}

//  =================
//  Cmd_InvDrop_f
//  =================
void Cmd_InvDrop_f (edict_t *ent)
{
	gitem_t		*it;
	
	ValidateSelectedItem (ent);
	
	if (ent->client->pers.selected_item == -1)
	{
		gi.cprintf (ent, PRINT_HIGH, "No item to drop.\n");
		return;
	}
	
	it = itemlist[ent->client->pers.selected_item];
	if (!it->drop)
	{
		gi.cprintf (ent, PRINT_HIGH, "Item cannot be dropped.\n");
		return;
	}
	it->drop (ent, it);
}

//  =================
//  Cmd_Kill_f
//  =================
void Cmd_Kill_f (edict_t *ent)
{
	//ZOID
	if (ent->solid == SOLID_NOT)
		return;
	//ZOID
	
	// Don't allow frozen people to kill themselves -- people use this to cheat
	// inflictors out of a frag.
	if (ent->frozen)
		return;
	
	// Don't let them kill themselves more often than every 5 seconds.
	if ((level.time - ent->client->respawn_time) < 5)
		return;
	
	ent->flags &= ~FL_GODMODE;
	ent->health = 0;
	ent->s.effects = EF_NONE;
	ent->s.renderfx = 0;
	meansOfDeath = MOD_SUICIDE;
	player_die (ent, ent, ent, 100000, vec3_origin);
}

//  =================
//  Cmd_PutAway_f
//  =================
void Cmd_PutAway_f (edict_t *ent)
{
	ent->client->showscores = QFALSE;
	ent->client->showhelp = QFALSE;
	ent->client->showinventory = QFALSE;
	//ZOID
	if (ent->client->menu)
		PMenu_Close(ent);
	ent->client->update_chase = QTRUE;
	//ZOID
}


int PlayerSort (void const *a, void const *b)
{
	int		anum, bnum;
	
	anum = *(int *)a;
	bnum = *(int *)b;
	
	anum = game.clients[anum].ps.stats[STAT_FRAGS];
	bnum = game.clients[bnum].ps.stats[STAT_FRAGS];
	
	if (anum < bnum)
		return -1;
	if (anum > bnum)
		return +1;
	return 0;
}

//  =================
//  Cmd_Players_f
//  =================
void Cmd_Players_f (edict_t *ent)
{
	int		i;
	int		count;
	char	small[64];
	char	large[1280];
	int		index[256];

	count = 0;
	for (i = 0 ; i < maxclients->value ; i++)
	{
		if (game.clients[i].pers.connected && (g_edicts + i + 1)->inuse)
		{
			index[count] = i;
			count++;
		}
	}

	// sort by frags
	qsort (index, count, sizeof(index[0]), PlayerSort);

	// print information
	large[0] = 0;

	for (i = 0 ; i < count ; i++)
	{
		Com_sprintf (small, sizeof(small), "%3i %s\n",
			game.clients[index[i]].ps.stats[STAT_FRAGS],
			game.clients[index[i]].pers.netname);
		if (strlen (small) + strlen(large) > sizeof(large) - 100 )
		{	// can't print all of them in one packet
			strcat (large, "...\n");
			break;
		}
		strcat (large, small);
	}

	gi.cprintf (ent, PRINT_HIGH, "%s\n%i players\n", large, count);
}

//  =================
//  Cmd_Push_f
//  =================
void Cmd_Push_f (edict_t *ent)
{
	vec3_t	start;
	vec3_t	forward;
	vec3_t	end;
	trace_t	tr;

	// No pushing when you're dead.
	if (ent->deadflag)
		return;

	// Or if you're frozen.
	if (ent->frozen)
		return;

	// Or if push/pull has been banned.
	if (i_featureban & FB_PUSHPULL)
		return;

	// Or when you're a ghost.
	if (ctf->value && ent->movetype == MOVETYPE_NOCLIP && ent->solid == SOLID_NOT)
		return;

	//User can't use push tool if you are flying the angeloe
	if (ent->client->pers.special == AOENERGY)
		return;

	VectorCopy (ent->s.origin, start);
	start[2] += ent->viewheight - 8;
	AngleVectors (ent->client->v_angle, forward, NULL, NULL);
	VectorMA (start, 8192, forward, end);
	tr = gi.trace (start, NULL, NULL, end, ent, MASK_SHOT);

	if (tr.ent->client != NULL && tr.ent->client->pers.special == AOENERGY) 
		return; //QW// target flying angeloe blocks the pusher

	if (tr.ent && ((tr.ent->svflags & SVF_MONSTER)
		/* || (tr.ent->spawnflags & (DROPPED_ITEM | DROPPED_PLAYER_ITEM)) */
		|| (tr.ent->client)))
	{
		// Show a trail from the pusher to the pushee.
		gi.WriteByte (svc_temp_entity);
		gi.WriteByte (TE_BUBBLETRAIL);
		gi.WritePosition (start);
		gi.WritePosition (tr.endpos);
		gi.multicast (ent->s.origin, MULTICAST_PHS);
		gi.WriteByte (svc_temp_entity);
		gi.WriteByte (TE_BFG_LASER);
		gi.WritePosition (start);
		gi.WritePosition (tr.endpos);
		gi.multicast (ent->s.origin, MULTICAST_PHS);

		// Have the pusher emit a sound.
		gi.sound (ent, CHAN_WEAPON, gi.soundindex ("items/damage3.wav"), 1,
			ATTN_NORM, 0);

		// Now push them.
		VectorScale (forward, 2500, forward);
		VectorAdd (forward, tr.ent->velocity, tr.ent->velocity);
		if (tr.ent->client !=NULL)	// not decoys
		{	
			gi.bprintf (PRINT_HIGH, "%s pushed %s\n", ent->client->pers.netname, tr.ent->client->pers.netname);
			//gi.cprintf (tr.ent, PRINT_HIGH, "You were pushed by %s\n", ent->client->pers.netname);
		}
	}
}

//  =================
//  Cmd_Pull_f
//  =================
void Cmd_Pull_f (edict_t *ent)
{
	vec3_t	start;
	vec3_t	forward;
	vec3_t	end;
	trace_t	tr;
	
	// No pulling when you're dead.
	if (ent->deadflag)
		return;
	
	// Or if you're frozen.
	if (ent->frozen)
		return;
	
	// Or if push/pull has been banned.
	if (i_featureban & FB_PUSHPULL)
		return;
	
	// Or when you're a ghost.
	if (ctf->value && ent->movetype == MOVETYPE_NOCLIP && ent->solid == SOLID_NOT)
		return;
	
	// user can't use pull tool if you are flying the angeloe
	if (ent->client->pers.special == AOENERGY) 
		return; 
	
	VectorCopy(ent->s.origin, start);
	start[2] += ent->viewheight - 8;
	AngleVectors(ent->client->v_angle, forward, NULL, NULL);
	VectorMA(start, 8192, forward, end);
	tr = gi.trace(start, NULL, NULL, end, ent, MASK_SHOT);
	
	if (tr.ent->client != NULL && tr.ent->client->pers.special == AOENERGY) 
		return; //QW// target flying angeloe blocks the puller
	
	if (tr.ent && ((tr.ent->svflags & SVF_MONSTER)
		// (tr.ent->spawnflags & (DROPPED_ITEM | DROPPED_PLAYER_ITEM))
		|| (tr.ent->client)))
	{
		// Show a trail from the puller to the pullee.
		gi.WriteByte (svc_temp_entity);
		gi.WriteByte (TE_BUBBLETRAIL);
		gi.WritePosition (start);
		gi.WritePosition (tr.endpos);
		gi.multicast (ent->s.origin, MULTICAST_PHS);
		gi.WriteByte (svc_temp_entity);
		gi.WriteByte (TE_BFG_LASER);
		gi.WritePosition (start);
		gi.WritePosition (tr.endpos);
		gi.multicast (ent->s.origin, MULTICAST_PHS);
		
		// Have the puller emit a sound.
		gi.sound (ent, CHAN_WEAPON, gi.soundindex ("items/damage3.wav"), 1,
			ATTN_NORM, 0);
		
		// Now pull them.
		VectorScale (forward, -2500, forward);
		VectorAdd (forward, tr.ent->velocity, tr.ent->velocity);
		if (tr.ent->client !=NULL)		// not decoys
		{
			gi.bprintf (PRINT_HIGH, "%s pulled %s\n", ent->client->pers.netname, tr.ent->client->pers.netname);
			//gi.cprintf (tr.ent, PRINT_HIGH, "You were pulled by %s\n", ent->client->pers.netname);
		}
	}
}

void Cmd_Wave_f (edict_t *ent)
{
	int		i;
	
	i = atoi (gi.argv(1));
	
	// can't wave when ducked
	if (ent->client->ps.pmove.pm_flags & PMF_DUCKED)
		return;
	
	// Or when you're a ghost.
	if (ctf->value && ent->movetype == MOVETYPE_NOCLIP && ent->solid == SOLID_NOT)
		return;
	
	if (ent->client->anim_priority > ANIM_WAVE)
		return;
	
	ent->client->anim_priority = ANIM_WAVE;
	
	switch (i)
	{
	case 0:
		gi.cprintf (ent, PRINT_HIGH, "flipoff\n");
		ent->s.frame = FRAME_flip01 - 1;
		ent->client->anim_end = FRAME_flip12;
		break;
	case 1:
		gi.cprintf (ent, PRINT_HIGH, "salute\n");
		ent->s.frame = FRAME_salute01 - 1;
		ent->client->anim_end = FRAME_salute11;
		break;
	case 2:
		gi.cprintf (ent, PRINT_HIGH, "taunt\n");
		ent->s.frame = FRAME_taunt01 - 1;
		ent->client->anim_end = FRAME_taunt17;
		break;
	case 3:
		gi.cprintf (ent, PRINT_HIGH, "wave\n");
		ent->s.frame = FRAME_wave01 - 1;
		ent->client->anim_end = FRAME_wave11;
		break;
	case 4:
	default:
		gi.cprintf (ent, PRINT_HIGH, "point\n");
		ent->s.frame = FRAME_point01 - 1;
		ent->client->anim_end = FRAME_point12;
		break;
	}
}

/*
=================
Cmd_FireMode_f
MUCE: new function for adjusting firing mode
=================
*/
void Cmd_FireMode_f (edict_t *ent)
{
	// Don't allow if the burst machinegun has been banned.
	if (i_weaponban & WB_BURSTMACHINEGUN)
		return;
	
	switch (ent->client->pers.fire_mode)
	{
	case 0:
		ent->client->pers.fire_mode = 1;
		gi.cprintf (ent, PRINT_HIGH,
			"Standard Machinegun Burst Fire Mode\n");
		break;
		
	case 1:
	default:
		ent->client->burstfire_count = 0;
		ent->client->pers.fire_mode = 0;
		gi.cprintf (ent, PRINT_HIGH,
			"Standard Machinegun Fully Automatic Mode\n");
		break;
	}
}

/*
==================
Cmd_Say_f
==================
*/
void Cmd_Say_f (edict_t *ent, qboolean team, qboolean arg0)
{
	int		i,j, cip[4];
	edict_t	*other;
	char	*p, ip[20];
	char	text[2048];
	gclient_t *cl;
	
	if (gi.argc () < 2 && !arg0)
		return;
	
	// If there's no teamplay, don't do team chat.
	if (!ctf->value && !((int)(dmflags->value) & (DF_MODELTEAMS | DF_SKINTEAMS)))
		team = QFALSE;
	
	// If this player is dead or a ghost, don't allow team chat.
	if (ctf->value 
		&& (ent->deadflag || (ent->solid == SOLID_NOT && ent->movetype == MOVETYPE_NOCLIP))
		&& !TeamplayCheckCountdown())
		team = QFALSE;
	
	// set up the player name to start the text string
	if (team)
		Com_sprintf (text, sizeof(text), "(%s): ", ent->client->pers.netname);
	else
		Com_sprintf (text, sizeof(text), "%s: ", ent->client->pers.netname);
	
	if (arg0)
	{
		strcat (text, gi.argv(0));
		strcat (text, " ");
		strcat (text, gi.args());
	}
	else
	{
		p = gi.args();
		
		if (*p == '"')
		{
			p++;
			p[strlen(p)-1] = 0;
		}
		strcat(text, p);
	}
	
	// don't allow percents in chat text
	for (;;)
	{
		p = strstr(text, "%");
		if (p)
			*p = 0x20; // replacement
		else
			break;
	}
	
	// don't let text be too long for malicious reasons
	if (strlen(text) > 150)
		text[150] = 0;
	
	strcat(text, "\n");
	
	if (flood_msgs->value) 
	{
		cl = ent->client;
		
		if (level.time < cl->flood_locktill) 
		{
			gi.cprintf(ent, PRINT_HIGH, "You can't talk for %d more seconds\n",
				(int)(cl->flood_locktill - level.time));
			return;
		}
		
		i = (int) (cl->flood_whenhead - flood_msgs->value + 1);
		
		if (i < 0)
			i = (sizeof(cl->flood_when)/sizeof(cl->flood_when[0])) + i;
		
		if (cl->flood_when[i] 
			&& level.time - cl->flood_when[i] < flood_persecond->value) 
		{
			cl->flood_locktill = level.time + flood_waitdelay->value;
			gi.cprintf(ent, PRINT_CHAT, "Flood protection:  You can't talk for %d seconds.\n",
				(int)flood_waitdelay->value);
			return;
		}

		cl->flood_whenhead = (cl->flood_whenhead + 1) %
			(sizeof(cl->flood_when)/sizeof(cl->flood_when[0]));
		cl->flood_when[cl->flood_whenhead] = level.time;
	}
	
	if (dedicated->value)
	{ 
		cip[0] =  ent->client->pers.ipAddr & 0xff;
		cip[1] = (ent->client->pers.ipAddr & 0xff00) / 0x100;
		cip[2] = (ent->client->pers.ipAddr & 0xff0000) / 0x10000;
		cip[3] = (ent->client->pers.ipAddr & 0xff000000) / 0x1000000;
		sprintf(ip, "%d.%d.%d.%d", cip[0], cip[1], cip[2], cip[3]);
		gi.dprintf("[%s]:: %s", ip, text);	// for the log
	}
	
	for (j = 1; j <= game.maxclients; j++)
	{
		other = &g_edicts[j];
		if (!other->inuse)
			continue;
		if (!other->client)
			continue;
		if (team)
		{
			if (!OnSameTeam (ent, other))
				continue;
		}
		gi.cprintf (other, PRINT_CHAT, "%s", text);		// for the clients
	}
	ARLog_Stats("CHAT: %s\\%d\n", ent->client->pers.netname, strlen(gi.argv(1)));
	//OutputDebugString(text);
}

/* 
================= 
Cmd_Lowlight_f 
JDB: new command for lowlight vision (GL mode ONLY) 4/4/98 
================= 
*/ 
void Cmd_Lowlight_f (edict_t *ent) 
{ 
	// No nightvision if it's been banned.
	if (i_featureban & FB_NIGHTVISION)
		return;
	
	if (ent->client->lowlight ^= 1) 
	{ 
		gi.cvar_forceset("gl_saturatelighting","1"); 
		gi.cvar_forceset("r_fullbright","1"); 
		ent->client->ps.fov = 75; 
	} 
	else 
	{ 
		gi.cvar_forceset("gl_saturatelighting","0"); 
		gi.cvar_forceset("r_fullbright","0"); 
		ent->client->ps.fov = 90; 
	} 
} 

/* 
================= 
Cmd_Zoom_f 
================= 
*/ 
void Cmd_Zoom_f (edict_t *ent) 
{
	int zoomtype;
	
	// No zooming if it's been banned.
	if (i_featureban & FB_ZOOM)
		return;
	
	zoomtype = atoi (gi.argv (1));
	if (zoomtype == 0)	
	{	
		ent->client->ps.fov = 90;		
	}	
	else if (zoomtype == 1)
	{	
		if (ent->client->ps.fov == 90)
			ent->client->ps.fov = 40;
		else if (ent->client->ps.fov == 40) 
			ent->client->ps.fov = 20;
		else if (ent->client->ps.fov == 20)
			ent->client->ps.fov = 10;
		else
			ent->client->ps.fov = 90;	
	}	
}

void Cmd_PlayerList_f(edict_t *ent)
{
	int i;
	char st[80];
	char text[1400];
	edict_t *e2;
	
	// connect-time, ping, score, name
	*text = 0;
	// make it all look nice
	gi.cprintf (ent, PRINT_HIGH, "\ntime  ping score name");
	gi.cprintf (ent, PRINT_HIGH, "\n----- ---- ----- ----------------\n");
	
	for (i = 0, e2 = g_edicts + 1; i < maxclients->value; i++, e2++)
	{
		if (!e2->inuse)
			continue;
		
		Com_sprintf(st, sizeof(st), "%02d:%02d %4d %5d %s%s\n",
			(level.framenum - e2->client->resp.enterframe) / 600,
			((level.framenum - e2->client->resp.enterframe) % 600)/10,
			e2->client->ping,
			e2->client->resp.score,
			e2->client->pers.netname,
			e2->client->resp.spectator ? " (spectator)" : "");
		if (strlen(text) + strlen(st) > sizeof(text) - 50)
		{
			sprintf(text+strlen(text), "And more...\n");
			gi.cprintf(ent, PRINT_HIGH, "%s", text);
			return;
		}
		strcat(text, st);
	}
	gi.cprintf(ent, PRINT_HIGH, "%s", text);
}

void Cmd_NotRecognized(edict_t *ent)
{
	gi.cprintf (ent, PRINT_HIGH, "Unrecognized command: %s %s\n",
		gi.argv ( 0 ), gi.args());
}

/*
=================
ClientCommand
=================
*/
void ClientCommand (edict_t *ent)
{
	char *cmd;
	
	if (!ent->client)
		return;		// not fully in game yet
	
	cmd = gi.argv( 0 );
	
	// Only a few commands are available during intermissions.
	if (level.intermissiontime)
	{
		if (Q_stricmp (cmd, "players") == 0)
			Cmd_Players_f (ent);
		else if (Q_stricmp (cmd, "say") == 0)
			Cmd_Say_f (ent, QFALSE, QFALSE);
		else if (Q_stricmp (cmd, "say_team") == 0
			|| Q_stricmp (cmd, "steam") == 0)
		{
			Cmd_Say_f (ent, QTRUE, QFALSE);
		}
		else if (Q_stricmp (cmd, "score") == 0)
			Cmd_Score_f (ent);
		else if (Q_stricmp (cmd, "help") == 0)
			Cmd_Help_f (ent);
		else if (Q_stricmp (cmd, "checkstats") == 0)
			Cmd_CheckStats_f (ent);
		
		// Those are the only commands we allow now.
		return;
	}
	
	
	// Parse the command.  To speed things up cheaply, do a switch on the first
	// few characters in the command.
	switch (cmd[0])
	{
	case 'a':
		// Angels provided by the Superheros II MOD
		if ( Q_stricmp (cmd, "angelod") == 0) Cmd_AngelOD(ent);		//death
		else if ( Q_stricmp (cmd, "angelol") == 0) Cmd_AngelOL(ent);	//life
		else if ( Q_stricmp (cmd, "angelom") == 0) Cmd_AngelOM(ent);	//mercy
		else if ( Q_stricmp (cmd, "angeloe") == 0) Cmd_AngelOE(ent);	//energy
		else if ( Q_stricmp (cmd, "angelof") == 0) Cmd_AngelOF(ent);	//frost
		else if ( Q_stricmp (cmd, "angelop") == 0) Cmd_AngelOP(ent);	//plague
		else if ( Q_stricmp (cmd, "angeloflame") == 0) Cmd_AngelOB(ent);	//flame
		else if ( Q_stricmp (cmd, "angeloh") == 0) Cmd_AngelOH(ent);	//horror
		else if ( Q_stricmp (cmd, "angelohorror") == 0) Cmd_AngelOH(ent);	//horror
		else if ( Q_stricmp (cmd, "angelob") == 0) Cmd_AngelOBlind(ent);	//blindness
		else if ( Q_stricmp (cmd, "angeloblind") == 0) Cmd_AngelOBlind(ent);	//blindness
		else if ( Q_stricmp (cmd, "angelor") == 0) Cmd_AngelOR(ent);		//revenge
		else if ( Q_stricmp (cmd, "angelou") == 0) Cmd_AngelOU(ent);	//unfreeze
		
		// CCH: new command for calling airstrikes
		else if (Q_stricmp (cmd, "airstrike") == 0)
			Cmd_Airstrike_f (ent);
		else
			Cmd_NotRecognized(ent);
		break;
		
	case 'b':
		if (Q_stricmp(cmd, "baton") == 0) 
			Cmd_Baton_f(ent);
		else
			Cmd_NotRecognized(ent);
		break;
		
	case 'c':
		if (Q_stricmp (cmd, "cloak") == 0)
			Cmd_cloak_f  (ent);
		// CCH: new command 'checkstats'
		else if (Q_stricmp (cmd, "checkstats") == 0)
			Cmd_CheckStats_f (ent);
		else if (Q_stricmp (cmd, "chasecam") == 0)
			Cmd_Third_f (ent);
		else
			Cmd_NotRecognized(ent);
		break;
		
	case 'd':
		if (Q_stricmp (cmd, "drop") == 0)
			Cmd_Drop_f (ent);
		else if (Q_stricmp (cmd, "droparmor") == 0)
			Cmd_DropArmor_f(ent);
		else if (Q_stricmp (cmd, "decoy") == 0)
			Toggle_Decoy_f (ent); 
		else if (Q_stricmp (cmd, "detonate") == 0)
			Cmd_Detonate_f(ent);
		else if (Q_stricmp (cmd, "devmenu") == 0)
			Cmd_DevMenu_f(cmd, ent);
		else
			Cmd_NotRecognized(ent);
		break;
		
	case 'e':
		if (Q_stricmp (cmd, "explosive") == 0)
			Cmd_Explosive (ent);
		else
			Cmd_NotRecognized(ent);
		break;
		
	case 'f':
		if (Q_stricmp (cmd, "flashlight") == 0)
			Cmd_Flashlight (ent);
		else if (Q_stricmp(cmd, "feign") == 0)
			Cmd_FeignDeath (ent);
		else if (Q_stricmp(cmd, "fph") == 0)
			Cmd_FPH(ent);
		else if (Q_stricmp (cmd, "firemode") == 0)
			Cmd_FireMode_f (ent);
		else
			Cmd_NotRecognized(ent);
		break;
		
	case 'g':
		if (Q_stricmp (cmd, "give") == 0)
			Cmd_Give_f (ent);
		if (Q_stricmp (cmd, "givehook") == 0)
			HookGiveBinds_f (ent);
		else if (Q_stricmp (cmd, "god") == 0)
			Cmd_God_f (ent);
		else
			Cmd_NotRecognized(ent);
		break;
		
	case 'h':
		if (Q_stricmp (cmd, "help") == 0)
			Cmd_Help_f (ent);
		else if (Q_stricmp (cmd, "height") == 0)
			Cmd_Height(ent);
		else if (Q_stricmp (cmd, "hook") == 0)
			Cmd_Hook_f (ent);
		else if (Q_stricmp (cmd, "holoquad") == 0)
			Cmd_Holoquad_f (ent);
		else if (Q_stricmp (cmd, "holoinvul") == 0)
			Cmd_Holoinvul_f(ent);
		else if (Q_stricmp (cmd, "holobfg") == 0)
			Cmd_Holobfg_f(ent);
		else if (Q_stricmp (cmd, "holoweapon") == 0)
			Cmd_Holoweapon_f(ent);
		else
			Cmd_NotRecognized(ent);
		break;
		
	case 'i':
		if (Q_stricmp (cmd, "id") == 0)
			Cmd_PlayerID(ent);
		else if (Q_stricmp(cmd,"invis") == 0)
			Cmd_Invisible(ent);
		else if (Q_stricmp (cmd, "inven") == 0)
			Cmd_Inven_f (ent);
		else if (Q_stricmp (cmd, "invnext") == 0)
			SelectNextItem (ent, -1);
		else if (Q_stricmp (cmd, "invprev") == 0)
			SelectPrevItem (ent, -1);
		else if (Q_stricmp (cmd, "invnextw") == 0)
			SelectNextItem (ent, IT_WEAPON);
		else if (Q_stricmp (cmd, "invprevw") == 0)
			SelectPrevItem (ent, IT_WEAPON);
		else if (Q_stricmp (cmd, "invnextp") == 0)
			SelectNextItem (ent, IT_POWERUP);
		else if (Q_stricmp (cmd, "invprevp") == 0)
			SelectPrevItem (ent, IT_POWERUP);
		else if (Q_stricmp (cmd, "invuse") == 0)
			Cmd_InvUse_f (ent);
		else if (Q_stricmp (cmd, "invdrop") == 0)
			Cmd_InvDrop_f (ent);
		else
			Cmd_NotRecognized(ent);
		break;
		
	case 'k':
		if (Q_stricmp (cmd, "kill") == 0)
			Cmd_Kill_f (ent);
		else if (Q_stricmp (cmd, "kamikaze") == 0) 
			Cmd_Kamikaze_f (ent);
		else
			Cmd_NotRecognized(ent);
		break;
		
	case 'l':
		if (Q_stricmp (cmd, "lsight") == 0) 
			SP_LaserSight (ent);
		else if (Q_stricmp (cmd, "laser") == 0) 
			PlaceLaserTripwire (ent);
		else if (Q_stricmp (cmd, "logplayers") == 0)
			Cmd_LogPlayers_f(ent);
		else if (Q_stricmp (cmd,"lbind") == 0)
			Cmd_LBind_f(ent);
		else if (Q_stricmp (cmd,"location") == 0)
			Cmd_Location_f(ent);
		else
			Cmd_NotRecognized(ent);
		break;
		
	case 'm':
		if (Q_stricmp (cmd, "man") == 0)
			Cmd_Help(ent);
		else
			Cmd_NotRecognized(ent);
		break;
		
	case 'n':
		if (Q_stricmp (cmd, "notarget") == 0)
			Cmd_Notarget_f (ent);
		else if (Q_stricmp (cmd, "noclip") == 0)
			Cmd_Noclip_f (ent);
		else if (Q_stricmp (cmd, "nightvision") == 0)
			Cmd_Lowlight_f (ent);
		else if (Q_stricmp(cmd, "no") == 0) 
			Voting_CmdVote_f(ent, NO);
		else
			Cmd_NotRecognized(ent);
		break;

// For oakbot, bot code not debugged.
	case 'o':
		if (Q_stricmp (cmd, "oak") == 0) //spawn oak bot
			SP_Oak_f(ent);
		else
			Cmd_NotRecognized(ent);
		break;
		
	case 'p':
		if (Q_stricmp (cmd, "push") == 0)
			Cmd_Push_f (ent);
		else if (Q_stricmp (cmd, "pull") == 0)
			Cmd_Pull_f (ent);
		else if (Q_stricmp (cmd, "putaway") == 0)
			Cmd_PutAway_f (ent);
		else if (Q_stricmp (cmd, "players") == 0)
			Cmd_Players_f (ent);
		else if (Q_stricmp(cmd, "playerlist") == 0)
			Cmd_PlayerList_f (ent);
		else if (Q_stricmp(cmd, "playerid") == 0)
			Cmd_PlayerID(ent);
		else
			Cmd_NotRecognized(ent);
		break;
		
	case 'r':
		if (Q_stricmp (cmd, "range") == 0)
			Cmd_Rangefinder(ent);
		else
			Cmd_NotRecognized(ent);
		break;
		
	case 's':
		if (Q_stricmp (cmd, "say") == 0)
			Cmd_Say_f (ent, QFALSE, QFALSE);
		else if (Q_stricmp (cmd, "say_team") == 0
			|| Q_stricmp (cmd, "steam") == 0)
			Cmd_Say_f (ent, QTRUE, QFALSE);
		else if (Q_stricmp (cmd, "scanner") == 0)
			Toggle_Scanner (ent);
		else if (Q_stricmp (cmd, "score") == 0)
			Cmd_Score_f (ent);
		else if (Q_stricmp (cmd, "skinlist") == 0)
		{
			Cmd_SkinList_f (ent);
			ent->spamtimer = level.time + 4.0f;	// anti spammage
		}
		else if (Q_stricmp (cmd, "sweeperfiremode") == 0)
			Cmd_SweeperFireMode(ent);
		else
			Cmd_NotRecognized(ent);
		break;
		
	case 't':
		if (Q_stricmp (cmd, "thirdx") == 0)
			Cmd_ThirdX_f (ent);
		else if (Q_stricmp (cmd, "thirdz") == 0)
			Cmd_ThirdZ_f (ent);
		else if (Q_stricmp (cmd, "team") == 0)
			CTFTeam_f (ent);
		else if (Q_stricmp (cmd, "tracker") == 0)
			Cmd_Tracker_f (ent);
		else if (Q_stricmp (cmd, "trackercount") == 0)
			Cmd_TrackerCounter (ent);
		else if (Q_stricmp (cmd, "turretcount") == 0)
			Cmd_TurretCounter (ent);
		else if (Q_stricmp (cmd,"time") == 0)
		{
			Cmd_Time_f(ent);
			ent->spamtimer = level.time + 6.0f;	// anti spammage
		}
		else if ((Q_stricmp (cmd,"timeremaining") == 0) || (Q_stricmp (cmd,"timeleft") == 0))
		{
			Cmd_TimeRemaining_f(ent);
			ent->spamtimer = level.time + 6.0f;	// anti spammage
		}
		else if (Q_stricmp (cmd,"timeall") == 0)
		{
			Cmd_TimeAll_f(ent);
			ent->spamtimer = level.time + 6.0f;	// anti spammage
		}
		else if (Q_stricmp (cmd, "turret") == 0)
			Cmd_Turret_f (ent);
		else
			Cmd_NotRecognized(ent);
		break;
		
	case 'u':
		if (Q_stricmp (cmd, "use") == 0)
			Cmd_Use_f (ent);
		else
			Cmd_NotRecognized(ent);
		break;
		
	case 'v':
		if (Q_stricmp(cmd, "vote") == 0)		// command is gi.argv(0)
		{
			if (Q_stricmp(gi.argv(1), "map") == 0)	// gi.argv(1) determines election type
				Voting_BeginElection(ent, ELECT_MAP);	// set the vote type flag and who started it.
			else if (Q_stricmp(gi.argv(1), "yes") == 0)	// 'vote yes' command
				Voting_CmdVote_f(ent, YES);
			else if (Q_stricmp(gi.argv(1), "no") == 0)	// 'vote no' command
				Voting_CmdVote_f(ent, NO);
			else
				Cmd_NotRecognized(ent);	//bad subcommand
		}
		else if (Q_stricmp (cmd, "ver") == 0)
			Cmd_Ver_f (ent);
		else
			Cmd_NotRecognized(ent);
		break;
		
	case 'w':
		if (Q_stricmp (cmd, "weapprev") == 0)
			Cmd_WeapPrev_f (ent);
		else if (Q_stricmp (cmd, "weapnext") == 0)
			Cmd_WeapNext_f (ent);
		else if (Q_stricmp (cmd, "weaplast") == 0)
			Cmd_WeapLast_f (ent);
		else if (Q_stricmp (cmd, "wave") == 0)
			Cmd_Wave_f (ent);
		else if (Q_stricmp (cmd, "weallknow") == 0)
			Cmd_WWW_f ();
		else
			Cmd_NotRecognized(ent);
		break;
		
	case 'y':
		if (Q_stricmp(cmd, "yes") == 0) 
			Voting_CmdVote_f(ent, YES);
		else
			Cmd_NotRecognized(ent);
		break;
		
	case 'z':
		if (Q_stricmp (cmd, "zoom") == 0)
			Cmd_Zoom_f (ent);
		else
			Cmd_NotRecognized(ent);
		break;
		
	default:
			Cmd_NotRecognized(ent);
		break;
	}
}
