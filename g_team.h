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

//
// CTF 
//

#ifndef CTF_VERSION_H
#define CTF_VERSION_H

#define CTF_VERSION			0.2
#define CTF_VSTRING2(x) #x
#define CTF_VSTRING(x) CTF_VSTRING2(x)
#define CTF_STRING_VERSION  CTF_VSTRING(CTF_VERSION)

#define MAX_TEAMS 2

typedef enum ctfteam_n
{
	CTF_NOTEAM,
	CTF_TEAM1,
	CTF_TEAM2
} ctfteam_t;

typedef enum ctfstate_n
{
	CTF_STATE_START,
	CTF_STATE_PLAYING
} ctfstate_t;

extern cvar_t *ctf;

#define CTF_TEAM1_SKIN "ctf_r"
#define CTF_TEAM2_SKIN "ctf_b"

#define DF_CTF_FORCEJOIN	131072	
#define DF_ARMOR_PROTECT	262144
#define DF_TEAMREBALANCE	524288

void CTFInit (void);
void stuffcmd (edict_t *ent, char *s);

void SP_info_player_team1 (edict_t *self);
void SP_info_player_team2 (edict_t *self);

char *CTFTeamName (int team);
char *CTFOtherTeamName (int team);
void CTFAssignSkin (edict_t *ent, char *s);
void CTFAssignTeam (gclient_t *who);
edict_t *SelectCTFSpawnPoint (edict_t *ent, qboolean *spawnlive);
void CTFCredits(edict_t *ent, pmenu_t *p);
void CTFCalcScores (void);
void SetCTFStats (edict_t *ent);
void CTFScoreboardMessage (edict_t *ent, edict_t *killer, char string[1400]);
void CTFSetTeam (edict_t *ent, int desired_team);
void CTFTeam_f (edict_t *ent);
void CTFID_f (edict_t *ent);

void CTFOpenJoinMenu (edict_t *ent);
qboolean CTFStartClient (edict_t *ent);

char *CTFCheckRules (void);

void SP_misc_ctf_banner (edict_t *ent);
void SP_misc_ctf_small_banner (edict_t *ent);

void UpdateChaseCam (edict_t *ent);
void ChaseNext (edict_t *ent);
void ChasePrev (edict_t *ent);

void SP_trigger_teleport (edict_t *ent);
void SP_info_teleport_destination (edict_t *ent);

void TeamplaySetupSpawnpoint (edict_t *spot);
void TeamplaySpawnpointTouch (edict_t *self, edict_t *other, cplane_t *plane, csurface_t *surf);
void TeamplayMakeObserver (edict_t *ent);
void TeamplayCheckRespawn (void);

qboolean TeamplayCheckCountdown (void);
void TeamplayDoCountdown (void);
void TeamplaySpawnEntities (char *mapname, char *entities, char *spawnpoint);

qboolean TeamplayCheckRound1 (void);
qboolean TeamplayCheckRound2 (void);
void TeamplayStartRound2 (void);

void TeamplayRebalanceTeams (void);

#endif

