/*
* Copyright (C) 1997-2001 Id Software, Inc.
*
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation; either version 2
* of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  
* 
* See the GNU General Public License for more details.
* 
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*
*/

#include "g_local.h"
#include "g_team.h"
#include "scanner.h"
#include "performance.h"

//
//======================================================================
//
//  INTERMISSION
//  
//======================================================================
//

void MoveClientToIntermission (edict_t *ent)
{
	if (deathmatch->value || coop->value)
		ent->client->showscores = true;
	VectorCopy (level.intermission_origin, ent->s.origin);
	ent->client->ps.pmove.origin[0] = level.intermission_origin[0]*8;
	ent->client->ps.pmove.origin[1] = level.intermission_origin[1]*8;
	ent->client->ps.pmove.origin[2] = level.intermission_origin[2]*8;
	VectorCopy (level.intermission_angle, ent->client->ps.viewangles);
	ent->client->ps.pmove.pm_type = PM_FREEZE;
	ent->client->ps.gunindex = 0;
	ent->client->ps.blend[3] = 0;
	ent->client->ps.rdflags &= ~RDF_UNDERWATER;
	
	// clean up powerup info
	/*ATTILA begin*/
	ent->client->Jet_framenum = 0;
	ent->client->Jet_remaining = 0;
	/*ATTILA end*/
	
	/* Wonderslug Cancel Kamikaze if going to new level */
	ent->client->kamikaze_framenum = 0;
	/* WonderSlug End */
	ent->client->quad_framenum = 0;
	ent->client->invincible_framenum = 0;
	ent->client->breather_framenum = 0;
	ent->client->enviro_framenum = 0;
	ent->client->grenade_blew_up = false;
	ent->client->grenade_time = 0;
	
	// don't drown during intermission
	ent->waterlevel = 0;
	
	ent->viewheight = 0;
	ent->s.modelindex = 0;
	ent->s.modelindex2 = 0;
	ent->s.modelindex3 = 0;
	ent->s.modelindex = 0;
	ent->s.effects = EF_NONE;
	ent->s.sound = 0;
	ent->solid = SOLID_NOT;
	
	// add the layout
	
	if (deathmatch->value || coop->value)
	{
		DeathmatchScoreboardMessage (ent, NULL);
		gi.unicast (ent, true);
	}
}

void BeginIntermission (edict_t *targ)
{
	int		i, n;
	edict_t	*ent, *client;
	
	if (level.intermissiontime)
		return;		// already activated
	
	//ZOID
	if (deathmatch->value && ctf->value)
		CTFCalcScores();
	//ZOID
	
	game.autosaved = false;
	
	// respawn any dead clients
	for (i=0 ; i<maxclients->value ; i++)
	{
		client = g_edicts + 1 + i;
		if (!client->inuse)
			continue;
		if (client->health <= 0)
			respawn(client);
		
	}
	
	level.intermissiontime = level.time;
	level.changemap = targ->map;
	
	if (strstr(level.changemap, "*"))
	{
		if (coop->value)
		{
			for (i=0 ; i<maxclients->value ; i++)
			{
				client = g_edicts + 1 + i;
				if (!client->inuse)
					continue;
				// strip players of all keys between units
				for (n = 0; n < game.num_items; n++)
				{
					if (itemlist[n]->flags & IT_KEY)
					{
						client->client->pers.inventory[n] = 0;
						break;
					}
				}
			}
		}
	}
	else
	{
		if (!deathmatch->value)
		{
			level.exitintermission = 1;		// go immediately to the next level
			return;
		}
	}
	
	level.exitintermission = 0;
	
	// find an intermission spot
	ent = G_Find (NULL, FOFS(classname), "info_player_intermission");
	if (!ent)
	{	// the map creator forgot to put in an intermission point.
		ent = G_Find (NULL, FOFS(classname), "info_player_start");
		if (!ent)
			ent = G_Find (NULL, FOFS(classname), "info_player_deathmatch");
	}
	else
	{	// chose one of four spots
		i = rand() & 3;
		while (i--)
		{
			ent = G_Find (ent, FOFS(classname), "info_player_intermission");
			if (!ent)	// wrap around the list
				ent = G_Find (ent, FOFS(classname), "info_player_intermission");
		}
	}
	
	VectorCopy (ent->s.origin, level.intermission_origin);
	VectorCopy (ent->s.angles, level.intermission_angle);
	
	// move all clients to the intermission point
	for (i=0 ; i<maxclients->value ; i++)
	{
		client = g_edicts + 1 + i;
		if (!client->inuse)
			continue;
		gi.sound(client, CHAN_SONG, gi.soundindex("world/darkchoir.wav"), 0.4f, ATTN_NORM, 0);
		MoveClientToIntermission (client);
	}
}

//***********************************************************
//
// DM_CreateScoreboard
//
// This forms the scoreboard string based on the current
// scores and updates it as needed when the pings/scores
// change. This is the original DM scoreboard with dogtags.
//
// This function takes a pointer to the string array
// and puts the scoreboard message into it.
// The same 1400 byte array is used to buffer the CTF scoreboard.
//***********************************************************

void DM_CreateScoreboard(edict_t *ent, edict_t *killer, char *string)
{
	char	entry[1024];
	size_t	stringlength;
	int		i, k;
	int		j;
	int		x, y;
	int		sorted[MAX_CLIENTS] = { 0 };
	int		sortedscores[MAX_CLIENTS] = { 0 };
	int		score, total;
	//int		picnum;
	gclient_t	*cl;
	edict_t		*cl_ent;
	char	*tag;
	int		time_in;
	
	// sort the clients by score
	total = 0;
	for (i = 0 ; i < game.maxclients ; i++)
	{
		cl_ent = g_edicts + 1 + i;
		if (!cl_ent->inuse)
			continue;
		score = game.clients[i].resp.score;
		for (j = 0 ; j<total ; j++)
		{
			if (score > sortedscores[j])
				break;
		}
		for (k = total ; k>j ; k--)
		{
			sorted[k] = sorted[k-1];
			sortedscores[k] = sortedscores[k-1];
		}
		sorted[j] = i;
		sortedscores[j] = score;
		total++;
	}
	
	// print level name and exit rules
	string[0] = 0;
	
	stringlength = strlen(string);
	
	// add the clients in sorted order
	if (total > 12)	// max of 12 scores
		total = 12;
	
	for (i = 0 ; i < total ; i++)
	{
		cl = &game.clients[sorted[i]];
		cl_ent = g_edicts + 1 + sorted[i];
		//picnum = gi.imageindex ("i_fixme");
		x = (i >= 6) ? 160 : 0;
		y = 32 + 32 * (i % 6); // dogtag is 32 units high
		
		// add a dogtag
		if (cl_ent == ent)
			tag = "tag1";
		else if (cl_ent == killer)
			tag = "tag2";
		else
			tag = NULL;
		
		if (tag)
		{
			Com_sprintf (entry, sizeof(entry),
				"xv %i yv %i picn %s ", x+32, y, tag);
			j = strlen(entry);
			if (stringlength + j > 1024)
				break;
			strcpy (string + stringlength, entry); //safe
			stringlength += j;
		}
		
		time_in = level.framenum - cl->resp.enterframe;
		
		// send the layout
		Com_sprintf (entry, sizeof(entry),
			"client %i %i %i %i %i %i ",
			x, y, sorted[i], // location & player index
			cl->resp.score,  // his score
			cl->ping, time_in/600);  // his ping and in-game minutes
		
		j = strlen(entry);
		if (stringlength + j > 1024)
			break;
		strcpy (string + stringlength, entry);	//safe
		stringlength += j;
		DbgPrintf("%s size: %lu\n", entry, stringlength);
	}
}

#ifdef SCOREBOARD1

void A_Scoreboard(edict_t *ent)
{
	
	if (ent->client->showscores && ent->client->scoreboardnum == 1)
	{
		// blink header of the winning team during intermission
		if (level.intermissiontime && (level.framenum & 8))	// blink 1/8th second
		{
			if (team1_score > team2_score)
				ent->client->ps.stats[STAT_TEAM1_PIC] = 0;
			else if (team2_score > team1_score)
				ent->client->ps.stats[STAT_TEAM2_PIC] = 0;
			else if (team1_total > team2_total) // frag tie breaker
				ent->client->ps.stats[STAT_TEAM1_PIC] = 0;
			else if (team2_total > team1_total) 
				ent->client->ps.stats[STAT_TEAM2_PIC] = 0;
			else // tie game!
			{
				ent->client->ps.stats[STAT_TEAM1_PIC] = 0;
				ent->client->ps.stats[STAT_TEAM2_PIC] = 0;
			}
		}
		else
		{
			ent->client->ps.stats[STAT_TEAM1_PIC] = gi.imageindex(team1_skin_index);
			ent->client->ps.stats[STAT_TEAM2_PIC] = gi.imageindex(team2_skin_index);
		}
		
		ent->client->ps.stats[STAT_TEAM1_SCORE] = team1_score;
		ent->client->ps.stats[STAT_TEAM2_SCORE] = team2_score;
	}
}
#endif		  

void A_ScoreboardMessage (edict_t *ent, edict_t *killer, char *string)
{
	//char		damage[50];
	//gclient_t   *cl;
	edict_t     *cl_ent;
	int         maxsize = 1000, i, j, k;
	
	ent->client->scoreboardnum = 2;
	
#ifdef SCOREBOARD1
	
	if (ent->client->scoreboardnum == 1)
	{
		int team, len, deadview;
		int sorted[TEAM_TOP][MAX_CLIENTS];
		int sortedscores[TEAM_TOP][MAX_CLIENTS];
		int score, total[TEAM_TOP], totalscore[TEAM_TOP];
		int totalalive[TEAM_TOP], totalaliveprinted[TEAM_TOP];
		int stoppedat[TEAM_TOP];
		int name_pos[TEAM_TOP];
		
		deadview = (ent->solid == SOLID_NOT ||
			ent->deadflag == DEAD_DEAD ||
			!team_round_going);
		
		ent->client->ps.stats[STAT_TEAM_HEADER] = gi.imageindex ("tag3");
		
		total[TEAM1] = total[TEAM2] = totalalive[TEAM1] = totalalive[TEAM2] = 
			totalscore[TEAM1] = totalscore[TEAM2] = 0;
		
		for (i=0 ; i<game.maxclients ; i++)
		{
			cl_ent = g_edicts + 1 + i;
			if (!cl_ent->inuse)
				continue;
			
			if (game.clients[i].resp.team == NOTEAM)
				continue;
			else
				team = game.clients[i].resp.team;
			
			score = game.clients[i].resp.score;
			if (noscore->value)
			{
				j = total[team];
			}
			else
			{
				for (j = 0; j < total[team]; j++)
				{
					if (score > sortedscores[team][j])
						break;
				}
				for (k=total[team] ; k>j ; k--)
				{
					sorted[team][k] = sorted[team][k-1];
					sortedscores[team][k] = sortedscores[team][k-1];
				}
			}
			sorted[team][j] = i;
			sortedscores[team][j] = score;
			totalscore[team] += score;
			total[team]++;
			if (cl_ent->solid != SOLID_NOT &&
				cl_ent->deadflag != DEAD_DEAD)
				totalalive[team]++;
		}
		
		// I've shifted the scoreboard position 8 pixels to the left in Axshun so it works
		// correctly in 320x240 (Action's does not)--any problems with this?  -FB
		// Also going to center the team names.
		
		name_pos[TEAM1] = ((20 - strlen(team1_name)) / 2) * 8;
		if (name_pos[TEAM1] < 0)
			name_pos[TEAM1] = 0;
		name_pos[TEAM2] = ((20 - strlen(team2_name)) / 2) * 8;
		if (name_pos[TEAM2] < 0)
			name_pos[TEAM2] = 0;
		
		sprintf(string, 
			// TEAM1
			"if 24 xv 0 yv 8 pic 24 endif "  
			"if 22 xv 32 yv 8 pic 22 endif "
			"xv 32 yv 28 string \"%4d/%-3d\" "
			"xv 90 yv 12 num 2 26 "
			"xv %d yv 0 string \"%s\" "
			// TEAM2
			"if 25 xv 160 yv 8 pic 25 endif "  
			"if 22 xv 192 yv 8 pic 22 endif "
			"xv 192 yv 28 string \"%4d/%-3d\" "
			"xv 248 yv 12 num 2 27 "
			"xv %d yv 0 string \"%s\" ",
			totalscore[TEAM1], total[TEAM1], name_pos[TEAM1], team1_name,
			totalscore[TEAM2], total[TEAM2], name_pos[TEAM2] + 160, team2_name);
		
		len = strlen(string);
		
		totalaliveprinted[TEAM1] = totalaliveprinted[TEAM2] = 0;
		stoppedat[TEAM1] = stoppedat[TEAM2] = -1;
		
		for (i=0 ; i < (MAX_SCORES_PER_TEAM + 1) ; i++)
		{
			if (i >= total[TEAM1] && i >= total[TEAM2])
				break; 
			
			// ok, if we're approaching the "maxsize", then let's stop printing members of each
			// teams (if there's more than one member left to print in that team.)
			if (len > (maxsize - 200)) //RiEvEr was 100
			{
				if (i < (total[TEAM1] - 1))
					stoppedat[TEAM1] = i;
				if (i < (total[TEAM2] - 1))
					stoppedat[TEAM2] = i;
			}
			if (i == MAX_SCORES_PER_TEAM-1) 
			{
				if (total[TEAM1] > MAX_SCORES_PER_TEAM)
					stoppedat[TEAM1] = i;
				if (total[TEAM2] > MAX_SCORES_PER_TEAM)
					stoppedat[TEAM2] = i;
			}
			
			if (i < total[TEAM1] && stoppedat[TEAM1] == -1)  // print next team 1 member
			{
				cl = &game.clients[sorted[TEAM1][i]];
				cl_ent = g_edicts + 1 + sorted[TEAM1][i];
				if (cl_ent->solid != SOLID_NOT &&
					cl_ent->deadflag != DEAD_DEAD)
					totalaliveprinted[TEAM1]++;
				
				// AQ truncates names at 12, not sure why, except maybe to conserve scoreboard 
				// string space?  skipping that "feature".  -FB
				
				sprintf(string+strlen(string), 
					"xv 0 yv %d string%s \"%s\" ",  
					42 + i * 8,
					deadview ? (cl_ent->solid == SOLID_NOT ? "" : "2") : "",
					game.clients[sorted[TEAM1][i]].pers.netname);
			}
			
			if (i < total[TEAM2] && stoppedat[TEAM2] == -1)  // print next team 2 member
			{
				cl = &game.clients[sorted[TEAM2][i]];
				cl_ent = g_edicts + 1 + sorted[TEAM2][i];
				if (cl_ent->solid != SOLID_NOT &&
					cl_ent->deadflag != DEAD_DEAD)
					totalaliveprinted[TEAM2]++;
				
				// AQ truncates names at 12, not sure why, except maybe to conserve scoreboard 
				// string space?  skipping that "feature".  -FB
				
				sprintf(string+strlen(string), 
					"xv 160 yv %d string%s \"%s\" ",  
					42 + i * 8,
					deadview ? (cl_ent->solid == SOLID_NOT ? "" : "2") : "",
					game.clients[sorted[TEAM2][i]].pers.netname);
			}
			
			len = strlen(string);
		}
		
		// Print remaining players if we ran out of room
		if (!deadview) // live player viewing scoreboard
		{
			if (stoppedat[TEAM1] > -1)
			{
				sprintf(string + strlen(string), "xv 0 yv %d string \"..and %d more\" ",
					42 + (stoppedat[TEAM1] * 8), total[TEAM1] - stoppedat[TEAM1]);
			}
			if (stoppedat[TEAM2] > -1)
			{
				sprintf(string + strlen(string), "xv 160 yv %d string \"..and %d more\" ",
					42 + (stoppedat[TEAM2] * 8), total[TEAM2] - stoppedat[TEAM2]);
			}
		}
		else // dead player viewing scoreboard
		{
			if (stoppedat[TEAM1] > -1)
			{
				sprintf(string + strlen(string), "xv 0 yv %d string%s \"..and %d/%d more\" ",
					42 + (stoppedat[TEAM1] * 8), 
					(totalalive[TEAM1] - totalaliveprinted[TEAM1]) ? "2" : "",
					totalalive[TEAM1] - totalaliveprinted[TEAM1],
					total[TEAM1] - stoppedat[TEAM1]);
			}
			if (stoppedat[TEAM2] > -1)
			{
				sprintf(string + strlen(string), "xv 160 yv %d string%s \"..and %d/%d more\" ",
					42 + (stoppedat[TEAM2] * 8), 
					(totalalive[TEAM2] - totalaliveprinted[TEAM2]) ? "2" : "",
					totalalive[TEAM2] - totalaliveprinted[TEAM2],
					total[TEAM2] - stoppedat[TEAM2]);
			}
		}
	}
	
#endif
	
	if (ent->client->scoreboardnum == 2)
	{
		int total, score, ping;
		int sortedscores[MAX_CLIENTS] = { 0 };
		int sorted[MAX_CLIENTS] = { 0 };
		
		total = score = 0;
		
		for (i = 0; i < game.maxclients; i++)
		{
			cl_ent = g_edicts + 1 + i;
			if (!cl_ent->inuse)
				continue;
			
			score = game.clients[i].resp.score;
			if (noscore->value)
			{
				j = total;
			}
			else
			{
				for (j = 0; j < total; j++)
				{
					if (score > sortedscores[j])
						break;
				}
				for (k = total ; k > j ; k--)
				{
					sorted[k] = sorted[k-1];
					sortedscores[k] = sortedscores[k-1];
				}
			}
			sorted[j] = i;
			sortedscores[j] = score;
			total++;
		}
		
		if (noscore->value)
		{
			strcpy(string,  "xv 0 yv 32 string2 \"Player          Time Ping\" "
				"xv 0 yv 40 string2 \"--------------- ---- ----\" ");
		}
		else
		{
			// originally included damage inflicted
			//strcpy(string,  "xv 0 yv 32 string2 \"Frags Player          Time Ping Damage\" "
			//	"xv 0 yv 40 string2 \"----- --------------- ---- ---- ------\" ");
			strcpy(string,  "xv 0 yv 32 string2 \"Frags Player          Time Ping\" "
				"xv 0 yv 40 string2 \"----- --------------- ---- ----\" ");
		}
		
		for (i = 0; i < total; i++)
		{
			ping = game.clients[sorted[i]].ping;
			if (ping > 999)
				ping = 999;
			if (noscore->value)
			{
				sprintf(string + strlen(string), 
					"xv 0 yv %d string \"%-15s %4d %4d\" ",
					48 + i * 8, 
					game.clients[sorted[i]].pers.netname,
					(level.framenum - game.clients[sorted[i]].resp.enterframe)/600,
					ping);                  
			}       
			else
			{
				//if (game.clients[sorted[i]].resp.damage_dealt < 1000000)
				//	sprintf(damage, "%d", game.clients[sorted[i]].resp.damage_dealt);
				//else
				//	strcpy(damage, "******");
				sprintf(string + strlen(string), 
					"xv 0 yv %d string \"%5d %-15s %4d %4d\" ",
					48 + i * 8, 
					sortedscores[i],
					game.clients[sorted[i]].pers.netname,
					(level.framenum - game.clients[sorted[i]].resp.enterframe)/600,
					ping);                  
			}

			DbgPrintf("%s size: %lu\n", string, strlen(string));

			if (strlen(string) > (unsigned)(maxsize - 200) && //RiEvEr was -100 
				i < (total - 2))
			{
				sprintf(string + strlen(string),
					"xv 0 yv %d string \"..and %d more\" ",
					48 + (i + 1) * 8,
					(total - i - 1));
				break;
			}
		}
	}
	
	if (strlen(string) > 1300)  // for debugging
		gi.dprintf("Warning: scoreboard string neared or exceeded max length\nDump:\n%s\n---\n", string);
	
	gi.WriteByte (svc_layout);
	gi.WriteString (string);
}


//******************************
//
//  DeathmatchScoreboardMessage
//	
//******************************

void DeathmatchScoreboardMessage (edict_t *ent, edict_t *killer)
{
	char	string[1400] = { 0 };	// limited to 1400 bytes by Q2 engine
	
	if (ent->client->showscores || ent->client->showinventory)
	{
		if (ent->client->pers.scanner_active)
			ent->client->pers.scanner_active = 2; //set changed bit and clear active bit
	}
		  
	if (ctf->value && ent->client->showscores)  
		CTFScoreboardMessage (ent, killer, string);
	if (!ctf->value && ent->client->showscores ) 
	{
		if (newscoreboard->value == 1)
			A_ScoreboardMessage(ent, killer, string);
		else
			DM_CreateScoreboard(ent, killer, string);
	}
	if (ent->client->pers.scanner_active & 1)
		ShowScanner (ent,string);
	
	if (!ent->client->showscores) // heatbar gets sent with scanner update
		ShowHeatbar(ent, string);
	
	gi.WriteByte (svc_layout);
	gi.WriteString (string);
}

//*******************************************
// ShowHeatbar
//
// Form the heatbar and place the pointer
// according to the client's current heat.
// Stuff the heatbar into the scoreboard message
// string.
//*******************************************

void ShowHeatbar(edict_t *ent, char *string) 
{
	char stats[128] = "";
	int i, heat_count = 0;
	char hc_string[4];
	
	if (weaponheat->value == 0) // heat not active no need for update
		return;
	
	//all this just to print some numbers with highlighting
	heat_count = ent->client->curr_heat;
	// protect against overflowing the heatbar array
	if (heat_count > 130) 
		heat_count = 130;
	
	Com_sprintf (hc_string, sizeof(hc_string), "%03i", heat_count); //leading zeros, 3 chars
	for (i = 0; i < 3; i++)
	{
		if (hc_string[i] != 0)
			hc_string[i] = hc_string[i] | 0x80;
	}
	
	if (ent->client->heat_active && ent->client->sweeperactive)
	{
		game.curr_heat_bar_counter = (int)ent->client->curr_heat/10;
		game.curr_heat_bar = game.heat_bar[game.curr_heat_bar_counter];
		
		// send the layout
		Com_sprintf (stats, sizeof(stats),
			"xv 0 yb -65 cstring \"%s %s\n%s\" ", 
			"\xe8\xe5\xe1\xf4 \xed\xe5\xf4\xe5\xf2", //"heat meter" with bit 7 set for highlight
			hc_string, game.curr_heat_bar);   
		SAFE_STRCAT(string, stats, LAYOUT_MAX_LENGTH);
	}
}


/*****************************
DeathmatchScoreboard

  Draw instead of help message.
  Note that it isn't that hard to overflow the 1400 byte message limit!
******************************/
void DeathmatchScoreboard (edict_t *ent)
{
	DeathmatchScoreboardMessage (ent, ent->enemy);
	gi.unicast (ent, true);
}


/*************************
* Cmd_Score_f
*
* Display the scoreboard
**************************/
void Cmd_Score_f (edict_t *ent)
{
	ent->client->showinventory = false;
	ent->client->showhelp = false;
	
	//ZOID
	if (ent->client->menu)
		PMenu_Close(ent);
	//ZOID
	
	if (!deathmatch->value && !coop->value)
		return;
	
	if (ent->client->showscores)
	{
		ent->client->showscores = false;
		ent->client->update_chase = true;
		return;
	}
	
	ent->client->showscores = true;
	DeathmatchScoreboard (ent);
}


//*******************************************
//    HelpComputer
//
//  Draw help computer.
//
//*******************************************
void HelpComputer (edict_t *ent)
{
	char	string[1024];
	char	*sk;
	
	if (skill->value == 0)
		sk = "easy";
	else if (skill->value == 1)
		sk = "medium";
	else if (skill->value == 2)
		sk = "hard";
	else
		sk = "hard+";
	
	// send the layout
	Com_sprintf (string, sizeof(string),
		"xv 32 yv 8 picn help "			// background
		"xv 202 yv 12 string2 \"%s\" "		// skill
		"xv 0 yv 24 cstring2 \"%s\" "		// level name
		"xv 0 yv 54 cstring2 \"%s\" "		// help 1
		"xv 0 yv 110 cstring2 \"%s\" "		// help 2
		"xv 50 yv 164 string2 \" kills     goals    secrets\" "
		"xv 50 yv 172 string2 \"%3i/%3i     %i/%i       %i/%i\" ", 
		sk,
		level.level_name,
		game.helpmessage1,
		game.helpmessage2,
		level.killed_monsters, level.total_monsters, 
		level.found_goals, level.total_goals,
		level.found_secrets, level.total_secrets);
	
	gi.WriteByte (svc_layout);
	gi.WriteString (string);
	gi.unicast (ent, true);
}


//*************************************
//Cmd_Help_f
//
//  Display the current help message
//*************************************
void Cmd_Help_f (edict_t *ent)
{
	// this is for backwards compatibility
	if (deathmatch->value)
	{
		Cmd_Score_f (ent);
		return;
	}

	ent->client->showinventory = false;
	ent->client->showscores = false;

	if (ent->client->showhelp && (ent->client->resp.game_helpchanged == game.helpchanged))
	{
		ent->client->showhelp = false;
		return;
	}

	ent->client->showhelp = true;
	ent->client->resp.helpchanged = 0;
	HelpComputer (ent);
}


//=======================================================================

/*
===============
G_SetStats
===============
*/
void G_SetStats (edict_t *ent)
{
	gitem_t	*item;
	int		index, cells = 0;
	int		power_armor_type;
	short	fph;
	
	//  CCH: local variables for rangefinder
	vec3_t	start = { 0 };
	vec3_t	forward;
	vec3_t	end;
	trace_t	tr; 
	
	//
	// health
	//
	ent->client->ps.stats[STAT_HEALTH_ICON] = level.pic_health;
	ent->client->ps.stats[STAT_HEALTH] = ent->health;
	
	//
	// ammo
	//
	if (!ent->client->ammo_index /* || !ent->client->pers.inventory[ent->client->ammo_index] */)
	{
		ent->client->ps.stats[STAT_AMMO_ICON] = 0;
		ent->client->ps.stats[STAT_AMMO] = 0;
	}
	else
	{
		item = itemlist[ent->client->ammo_index];
		ent->client->ps.stats[STAT_AMMO_ICON] = gi.imageindex (item->icon);
		ent->client->ps.stats[STAT_AMMO] = ent->client->pers.inventory[ent->client->ammo_index];
	}
	
	//
	// armor
	//
	power_armor_type = PowerArmorType (ent);
	if (power_armor_type)
	{
		cells = ent->client->pers.inventory[ITEM_INDEX(&gI_ammo_cells)];
		if (cells == 0)
		{	// ran out of cells for power armor
			ent->flags &= ~FL_POWER_ARMOR;
			gi.sound(ent, CHAN_ITEM, gi.soundindex("misc/power2.wav"), 1, ATTN_NORM, 0);
			power_armor_type = 0;;
		}
	}
	
	index = ArmorIndex (ent);
	if (power_armor_type && (!index || (level.framenum & 8) ) )
	{	// flash between power armor and other armor icon
		ent->client->ps.stats[STAT_ARMOR_ICON] = gi.imageindex ("i_powershield");
		ent->client->ps.stats[STAT_ARMOR] = cells;
	}
	else if (index)
	{
		item = GetItemByIndex (index);
		ent->client->ps.stats[STAT_ARMOR_ICON] = gi.imageindex (item->icon);
		ent->client->ps.stats[STAT_ARMOR] = ent->client->pers.inventory[index];
	}
	else
	{
		ent->client->ps.stats[STAT_ARMOR_ICON] = 0;
		ent->client->ps.stats[STAT_ARMOR] = 0;
	}
	
	//
	// pickup message
	//
	if (level.time > ent->client->pickup_msg_time)
	{
		ent->client->ps.stats[STAT_PICKUP_ICON] = 0;
		ent->client->ps.stats[STAT_PICKUP_STRING] = 0;
	}
	
	//
	// timers
	//
	/*ATTILA begin*/
	if ( Jet_Active(ent) )
	{
		ent->client->ps.stats[STAT_TIMER_ICON] = gi.imageindex ("i_pack");
		ent->client->ps.stats[STAT_TIMER] = ent->client->Jet_remaining/10;
	}
	
	else if (ent->client->cloaktime > level.framenum)
	{
		ent->client->ps.stats[STAT_TIMER_ICON] = gi.imageindex ("p_cloak");
		ent->client->ps.stats[STAT_TIMER] = (ent->client->cloaktime - level.framenum)/10;
	}
	
	else if (ent->client->quad_framenum > level.framenum)
	{
		ent->client->ps.stats[STAT_TIMER_ICON] = gi.imageindex ("p_quad");
		ent->client->ps.stats[STAT_TIMER] = (ent->client->quad_framenum - level.framenum)/10;
	}
	
	/* WonderSlug Kamikaze */ 
	else if (ent->client->kamikaze_framenum > level.framenum) 
	{
		ent->client->ps.stats[STAT_TIMER_ICON] = gi.imageindex ("k_comhead");
		ent->client->ps.stats[STAT_TIMER] = ent->client->kamikaze_timeleft/10;
	}
	/* WonderSlug End */
	
	else if (ent->client->cloak_time> level.time)
	{
		ent->client->ps.stats[STAT_TIMER_ICON] = gi.imageindex ("p_bandolier");
		ent->client->ps.stats[STAT_TIMER] = ent->client->cloak_time - level.time;
	}
	
	else if (ent->client->invincible_framenum > level.framenum)
	{
		ent->client->ps.stats[STAT_TIMER_ICON] = gi.imageindex ("p_invulnerability");
		ent->client->ps.stats[STAT_TIMER] = (ent->client->invincible_framenum - level.framenum)/10;
	}
	else if (ent->client->enviro_framenum > level.framenum)
	{
		ent->client->ps.stats[STAT_TIMER_ICON] = gi.imageindex ("p_envirosuit");
		ent->client->ps.stats[STAT_TIMER] = (ent->client->enviro_framenum - level.framenum)/10;
	}
	else if (ent->client->breather_framenum > level.framenum)
	{
		ent->client->ps.stats[STAT_TIMER_ICON] = gi.imageindex ("p_rebreather");
		ent->client->ps.stats[STAT_TIMER] = (ent->client->breather_framenum - level.framenum)/10;
	}
	
	else
	{
		ent->client->ps.stats[STAT_TIMER_ICON] = 0;
		ent->client->ps.stats[STAT_TIMER] = 0;
	}
	
	//
	// selected item
	//
	if (ent->client->pers.selected_item == -1)
		ent->client->ps.stats[STAT_SELECTED_ICON] = 0;
	else
		ent->client->ps.stats[STAT_SELECTED_ICON]
		= gi.imageindex (itemlist[ent->client->pers.selected_item]->icon);
	
	ent->client->ps.stats[STAT_SELECTED_ITEM] = ent->client->pers.selected_item;
	
	//
	// layouts
	//
	ent->client->ps.stats[STAT_LAYOUTS] = 0;
	
	if (deathmatch->value)
	{
		if (ent->client->pers.health <= 0 || level.intermissiontime
			|| ent->client->showscores || ent->client->pers.scanner_active)
			ent->client->ps.stats[STAT_LAYOUTS] |= 1;
		if (ent->client->showinventory && ent->client->pers.health > 0)
			ent->client->ps.stats[STAT_LAYOUTS] |= 2;
	}
	else
	{
		if (ent->client->showscores || ent->client->showhelp)
			ent->client->ps.stats[STAT_LAYOUTS] |= 1;
		if (ent->client->showinventory && ent->client->pers.health > 0)
			ent->client->ps.stats[STAT_LAYOUTS] |= 2;
	}
	
	//
	// frags
	//
	ent->client->ps.stats[STAT_FRAGS] = ent->client->resp.score;
	
	//
	// help icon / current weapon if not shown
	//
	if (ent->client->resp.helpchanged && (level.framenum&8) )
		ent->client->ps.stats[STAT_HELPICON] = gi.imageindex ("i_help");
	else if ( (ent->client->pers.hand == CENTER_HANDED || ent->client->ps.fov > 91 || ent->thirdperson)
		&& ent->client->pers.weapon)
		ent->client->ps.stats[STAT_HELPICON] = gi.imageindex (ent->client->pers.weapon->icon);
	else
		ent->client->ps.stats[STAT_HELPICON] = 0;
	
	//CCH: rangefinder
	VectorCopy(ent->s.origin,start);
	start[2] += ent->viewheight;
	AngleVectors(ent->client->v_angle,forward,NULL,NULL);
	VectorMA(start,8192,forward,end);
	tr = gi.trace(start,NULL,NULL,end,ent,MASK_SHOT|CONTENTS_SLIME|CONTENTS_LAVA);
	// check for sky and max the range if found
	if (!(i_loxfeatureban & LFB_RANGEFINDER))
	{
		if (tr.surface && (tr.surface->flags & SURF_SKY))
			ent->client->ps.stats[STAT_RANGEFINDER] = 9999;
		else
			ent->client->ps.stats[STAT_RANGEFINDER] = (int)(tr.fraction*8192);
	}
	else
		ent->client->ps.stats[STAT_RANGEFINDER] = 0;
	if (ent->client->resp.rangefinderactive == 0) 
		ent->client->ps.stats[STAT_RANGEFINDER] = 0;
	
	
	//ZOID
	SetCTFStats(ent);
	//ZOID
	
	DoMatchCountdown(ent);
	
	//QW// show playerid in center below crosshair
	if ((tr.ent->client != NULL) && (!(i_loxfeatureban & LFB_PLAYERID)))
	{
		ent->client->ps.stats[STAT_PLAYER] = 
			CS_PLAYERSKINS + (tr.ent - g_edicts - 1);
	}
	else 
		ent->client->ps.stats[STAT_PLAYER] = 0;
	
	if (ent->client->resp.playeridactive == 0) 
		ent->client->ps.stats[STAT_PLAYER] = 0;
	
	//QW// show player height in HUD 
	if ((!(i_loxfeatureban & LFB_HEIGHT)))
	{
		ent->client->ps.stats[STAT_HEIGHT] = ent->s.origin[2];
	}
	else 
		ent->client->ps.stats[STAT_HEIGHT] = 0;
	
	if (ent->client->resp.heightactive == 0)
		ent->client->ps.stats[STAT_HEIGHT] = 0;
	
	//QW// show Frags Per Hour
	if ((!(i_loxfeatureban & LFB_FPH)))
	{
		fph = (short) ent->client->resp.score / ((level.framenum - ent->client->resp.enterframe) / 36000.0f);
		ent->client->resp.fph = fph;
		ent->client->ps.stats[STAT_FPH] = fph;
	}
	else 
		ent->client->ps.stats[STAT_FPH] = 0;

	if (ent->client->resp.fph_active == 0) 
		ent->client->ps.stats[STAT_FPH] = 0;
	
	//QW// show Number of TRACKERS deployed
	if ((!(i_loxfeatureban & LFB_NUMTRKR)))
		ent->client->ps.stats[STAT_NUMTRACKER] = ent->client->tracker;
	else 
		ent->client->ps.stats[STAT_NUMTRACKER] = 0;
	
	if (ent->client->resp.trackercountactive == 0) 
		ent->client->ps.stats[STAT_NUMTRACKER] = 0;
	
	//QW// show Number of TURRETS deployed
	if ((!(i_loxfeatureban & LFB_NUMTRRT)))
		ent->client->ps.stats[STAT_NUMTURRET] = ent->client->turret;
	else 
		ent->client->ps.stats[STAT_NUMTURRET] = 0;
	
	if (ent->client->resp.turretcountactive == 0) 
		ent->client->ps.stats[STAT_NUMTURRET] = 0;
	
	//QW// show player location in HUD 
	if (ent->client->resp.locationactive)
	{
		gi.configstring(CS_GENERAL + (ent - g_edicts - 1), 
			va("%4.0f %4.0f %4.0f Angle %3.0f", 
			ent->s.origin[0], ent->s.origin[1], ent->s.origin[2],
			ent->client->ps.viewangles[1]));
		ent->client->ps.stats[STAT_LOCATION] = CS_GENERAL + (ent - g_edicts - 1);
	}
	else
		ent->client->ps.stats[STAT_LOCATION] = 0;
}
