#include "g_local.h"
#include "g_team.h"


typedef struct match_s
{

	// The total frag scores.  (Not set until the end of the game.)
	int total1, total2;

	// Where to spawn the ghost player.
	edict_t *spawnHere;

	// When to start the match.  (0 == start immediately.)
	int matchStartFrame;

	// What round we're playing.  (Either 1 or 2.)
	int round;
} match_t;

match_t matchgame;

/**
Initialize matches only if matchplay is set.
This sets a 40 second countdown timer.
*/
void MatchInit(void)
{
	// Reset the teamplay game status.
	memset (&matchgame, 0, sizeof (matchgame));

	// See if we're doing teamplay.
	matchplay = gi.cvar ("matchplay", "0", 0);
	//	ctf_forcejoin = gi.cvar ("ctf_forcejoin", "", 0);

	// If we are, set up a few things.
	if (matchplay->value)
	{
		// Change the gamename.
		//		gi.cvar_forceset ("gamename", GAMEVERSION_E);

		// Start the match in 40 seconds.
		matchgame.matchStartFrame = 400;

		// Start with round 1.
		matchgame.round = 1;
	}
}

// Play the countdown sound, and spawn players into the game (when the time
// comes).
void MatchplayDoCountdown (void)
{
	// Doing a countdown?
	if (matchgame.matchStartFrame)
	{
		// 10 seconds remaining?
		if (level.framenum == matchgame.matchStartFrame - 11*10)
		{
			// Play 10 to 1 countdown file.
			gi.sound (g_edicts,
				CHAN_RELIABLE+CHAN_AUTO+CHAN_NO_PHS_ADD,
				gi.soundindex ("world/10_0.wav"), 1, ATTN_NONE, 0);
		}
		else if (level.framenum == matchgame.matchStartFrame)
		{
			int i;

			// Time to fight!

			// Reset the score for round 2.  (They were able to see the score
			// during the intermission.)
			if (MatchplayCheckRound2())
			{
				//matchgame.spawn1 = matchgame.spawn2 = 0;

				// Put all team members into the game.
				for (i = 1; i <= maxclients->value; i++)
				{
					edict_t *ent;

					// Fetch the team player.
					ent = g_edicts + i;
					if (!ent->inuse)
						continue;
					if (!ent->client)
						continue;
					if (ent->client->resp.ctf_team != CTF_TEAM1
						&& ent->client->resp.ctf_team != CTF_TEAM2)
						continue;

					// Spawn them into the game.
					respawn (ent);
				}
			}
			// No more countdown.
			matchgame.matchStartFrame = 0;
		}
	}
}


void MatchplaySpawnEntities (char *mapname, char *entities, char *spawnpoint)
{
	FILE *f;
	char szFile[MAX_QPATH];
	int nEntSize, nRead;
	char *pszCustomEnt;

	// If custom_ents is off in deathmatch and if not ctf, do the normal thing.
	if (!custom_ents->value && !ctf->value)
	{
		SpawnEntities (mapname, entities, spawnpoint);
		return;
	}

	// Create the pathname to the entity file.
	Com_sprintf (szFile, sizeof (szFile), "%s/ent/%s.ent",
		gamedir->string, mapname);

	// Try to open it.
	f = fopen (szFile, "rb");
	if (!f)
	{
		// No custom entity file, so just use the default.
		SpawnEntities (mapname, entities, spawnpoint);
		return;
	}

	// Get the size of the file.
	if (fseek (f, 0, SEEK_END) != 0)
	{
		gi.dprintf ("%s: fseek %s\n", __func__, szFile);
		fclose(f);
		return;
	}

	nEntSize = ftell (f);
	if (nEntSize < 0)
	{
		gi.dprintf ("%s: ftell %s (%d)\n", __func__, szFile, nEntSize);
		fclose(f);
		return;
	}

	if (fseek (f, 0, SEEK_SET) != 0)
	{
		gi.dprintf ("%s: fseek %s\n", __func__, szFile);
		fclose(f);
		return;
	}

	// Create a buffer and read the custom entity file.
	pszCustomEnt = gi.TagMalloc (nEntSize + 1, TAG_LEVEL);
	if (!pszCustomEnt)
	{
		gi.dprintf ("%s: TagMalloc\n", __func__);
		fclose(f);
		return;
	}
	nRead = fread (pszCustomEnt, 1, nEntSize, f);
	if (nRead != nEntSize)
	{
		gi.dprintf ("%s: fread %s (%d/%d)\n",
			__func__, szFile, nRead, nEntSize);
		gi.TagFree (pszCustomEnt);
		fclose (f);
		return;
	}

	// Null-terminate the string.
	pszCustomEnt[nEntSize] = '\0';

	// Now spawn *these* entities!
	SpawnEntities (mapname, pszCustomEnt, spawnpoint);

	// Clean up.
	gi.TagFree (pszCustomEnt);
	fclose (f);
	return;
}


// Return whether we're in round 1.
qboolean MatchplayCheckRound1 (void)
{
	return (ctf->value && matchgame.round == 1);
}

// Return whether we're in round 2.
qboolean MatchplayCheckRound2 (void)
{
	return (ctf->value && matchgame.round == 2);
}


// Start round 2.
void MatchplayStartRound2 (void)
{
	edict_t *ent, *spot;
	int i;

	// Calculate the score so far.
	CTFCalcScores();

	// Now it's round 2.
	matchgame.round = 2;

	// Wait 15 seconds before starting round 2.
	matchgame.matchStartFrame = level.framenum + 150;

	// Remember the time from this level.
	if (timelimit->value)
		level.roundTimelimit = level.time + 15;

	// Remember the max frags from this level.
	if (fraglimit->value)
	{
		gclient_t *cl;

		for (i = 0; i < maxclients->value; i++)
		{
			cl = game.clients + i;
			if (!g_edicts[i+1].inuse)
				continue;

			if (level.roundFraglimit < cl->resp.score)
				level.roundFraglimit = cl->resp.score;
		}
	}

	// Reset the spawnpoints.  (The score doesn't get reset until the beginning
	// of round 2.)
	ent = NULL;
	while ((ent = G_Find (ent, FOFS (classname), "info_player_deathmatch"))
		!= NULL)
	{
		ent->s.renderfx &= ~(RF_SHELL_RED|RF_SHELL_BLUE);
		ent->s.renderfx |= RF_SHELL_GREEN;
	}

	// Find an intermission spot.
	spot = G_Find (NULL, FOFS(classname), "info_player_intermission");
	if (!spot)
	{	// the map creator forgot to put in an intermission point
		spot = G_Find (NULL, FOFS(classname), "info_player_start");
		if (!spot)
			spot = G_Find (NULL, FOFS(classname), "info_player_deathmatch");
	}
	else
	{	// chose one of four spots
		i = rand() & 3;
		while (i--)
		{
			spot = G_Find (spot, FOFS(classname), "info_player_intermission");
			if (!spot)	// wrap around the list
				spot = G_Find (spot, FOFS(classname), "info_player_intermission");
		}
	}

	// Move all clients to intermission.
	for (i = 0; i < maxclients->value; i++)
	{
		ent = g_edicts + 1 + i;
		if (!ent->inuse)
			continue;

		// Make them a spectator at the intermission point.
		VectorCopy (spot->s.origin, ent->s.origin);
		VectorCopy (spot->s.angles, ent->s.angles);
		MatchplayMakeObserver (ent);

		// Show them the scoreboard.
		ent->client->showscores = true;
		DeathmatchScoreboardMessage (ent, NULL);
		gi.unicast (ent, true);
	}
}


void MatchplayMakeObserver (edict_t *ent)
{
	ent->movetype = MOVETYPE_NOCLIP;
	//ent->deadflag = DEAD_DEAD;
	//ent->health = 0;
	ent->solid = SOLID_NOT;
	ent->svflags |= SVF_NOCLIENT;
	ent->client->ps.pmove.pm_type = PM_NORMAL;
	ent->client->ps.gunindex = 0;
	gi.linkentity (ent);
}

void DoMatchCountdown(edict_t *ent)
{
	int p1, p2;

	p1 = gi.imageindex ("i_ctf1");
	p2 = gi.imageindex ("i_ctf2");

	if (matchgame.matchStartFrame)
	{
		if (level.framenum & 4)
		{
			if (ent->client->resp.ctf_team == CTF_TEAM1)
				ent->client->ps.stats[STAT_TIMER_ICON] = (short) p1;
			else if (ent->client->resp.ctf_team == CTF_TEAM2)
				ent->client->ps.stats[STAT_TIMER_ICON] = (short) p2;
		}
		else
		{
			if (ent->client->resp.ctf_team == CTF_TEAM1)
				ent->client->ps.stats[STAT_TIMER_ICON] = (short) gi.imageindex ("i_ctf1t");
			else if (ent->client->resp.ctf_team == CTF_TEAM2)
				ent->client->ps.stats[STAT_TIMER_ICON] = (short) gi.imageindex ("i_ctf2t");
		}
		ent->client->ps.stats[STAT_TIMER]
		= (short) ((matchgame.matchStartFrame + 3 - level.framenum)/10);
	}
}
