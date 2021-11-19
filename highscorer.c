#include "g_local.h"

edict_t* LeaderMan;

// this function buggy, not finished
void HighScorer(void)
{
	//int	i;
	//int	count = 0;
	//edict_t	*ent = NULL;
	//edict_t	*newleader = NULL;
	//edict_t	*oldleader = NULL;
	//
	//// establish old leader
	//for (i=0; i < game.maxclients; i++) 
	//{
	//	ent = g_edicts + 1 + i;
	//	if (!ent->inuse)
	//		continue;
	//	
	//	count++;
	//}

	//if (count == 1) // only 1 player so he's our first leader
	//{
	//	ent->client->leader = 1;
	//	LeaderMan = ent;
	//}

	//// find current leader
	//for (i=0; i < game.maxclients; i++) 
	//{
	//	ent = g_edicts + 1 + i;
	//	if (!ent->inuse)
	//		continue;
	//	
	//	if (ent->client->leader)
	//		oldleader = ent;

	//	if (ent->client->resp.score > LeaderMan->client->resp.score && ent->client->leader == 0)
	//	{
	//		newleader = ent;		 // we have a new leader
	//		oldleader->client->leader = 0;
	//		newleader->client->leader = 1;
	//		gi.bprintf (PRINT_MEDIUM,"%s has taken the lead!\n", newleader->client->pers.netname);
	//		gi.sound(newleader, CHAN_AUTO, gi.soundindex("misc/comp_up.wav"), 1, ATTN_NORM, 0);

	//	}
	//}
}

//void HighScorerOld(void) 
//{
//	
//	int	i;
//	int	topscore = -999;
//	int	count1 = 0;
//	edict_t	*ent1 = NULL;
//	edict_t	*ent2 = NULL;
//	
//	for (i=0; i < game.maxclients; i++) 
//	{
//		ent1 = g_edicts + 1 + i;
//		if (!ent1->inuse)
//			continue;
//
//		// clear the current leader as we go
//		ent1->client->leader = 0;
//		count1++;
//		
//		if (ent1->client->resp.score > topscore) 
//		{
//			topscore = ent1->client->resp.score;
//			ent2 = ent1;	
//		}
//	}
//	
//	// only one player
//	if (count1 == 1) 
//		ent2 = NULL;
//	
//	if (ent2 != NULL) // mark the leader
//		ent2->client->leader = 1;
//	
//	if (LeaderMan != ent2) 
//	{
//		LeaderMan = ent2;
//		if (LeaderMan != NULL)
//		{
//			gi.bprintf (PRINT_MEDIUM,"%s has taken the lead!\n", LeaderMan->client->pers.netname);
//			gi.sound(LeaderMan, CHAN_AUTO, gi.soundindex("misc/comp_up.wav"), 1, ATTN_NORM, 0);
//		}
//	}
//}

#if 0

// Paril
// So basically this is my method of fixing it.
// The high scoring player will remain the leader until another player reaches over his score.
// Ties are not announced as it can get confusing with more than one player.
// If you wanted to announce ties, you could just keep setting ->tied to true and printing "Player [and player and player] are tied for first"
// and clear it when LeaderMan changes.

// FIXME: Technically, the ->inLead flag is not needed here, but it will be needed
// if you want to add effects to him or something
//#define USE_INLEAD_FLAG
edict_t* pLeader = NULL, * pOldLeader = NULL;

void HighScorer()
{
	int   i;
	edict_t* ent = NULL;
	edict_t* players[MAX_CLIENTS];
	int* scores[MAX_CLIENTS]; // [MAX_CLIENTS]
	int numPlayers = 0;

	pOldLeader = pLeader; // Store this for now.

	// Now, what I have planned here is that the scores would be stored (in the order of clients)
	// and taken care of below.
	for (i = 1; i < game.maxclients; i++)
	{
		ent = g_edicts + i;
		if (!ent->inuse || !ent->client) // !ent->client is just a sanity check
			continue;

		players[numPlayers] = ent;
		scores[numPlayers] = &players[numPlayers]->client->resp.score;
		numPlayers++;
	}

	// Don't bother if we don't have anyone to fight over the high score.
	if (numPlayers <= 1)
		return;

	// If we have no current leader, go through all the players
	// and find someone who has a higher score than the lowest score on the board.
	if (!pLeader)
	{
		edict_t* tempLeader = NULL;
		struct
		{
			int score;
			int index;
		} lowestScore = { 9999,-1 }, highestScore = { -9999, -1 };

		// Find the lowest score
		for (i = 0; i < numPlayers; i++)
		{
			if (*scores[i] < lowestScore.score)
			{
				lowestScore.score = *scores[i];
				lowestScore.index = i;
			}
		}

		// Now see if anyone has a higher score than the lowest score
		// and if they have a higher score than the highest score currently recorded
		highestScore.score = lowestScore.score; // So that we don't get anyone with 0 counted as highest
		for (i = 0; i < numPlayers; i++)
		{
			if (*scores[i] > lowestScore.score && *scores[i] > highestScore.score)
			{
				// Store the current person that we think has the highest score
				tempLeader = &g_edicts[i + 1];
				highestScore.score = *scores[i];
			}
		}

		// If we have a new leader...
		if (tempLeader)
		{
			// check to make sure that no one is tied with us, otherwise our leader status is void.
			for (i = 0; i < numPlayers; i++)
			{
				if (*scores[i] == tempLeader->client->resp.score && players[i] != tempLeader)
				{
					// Someone is tied with us. You are the weakest leader, goodbye.
					tempLeader = NULL;
					break;
				}
			}

			// if we STILL have a leader by here...
			if (tempLeader)
			{
				// This guy is entirely in the lead!
				// Throw him the flag and tell everyone they suck.
				pLeader = tempLeader;
#ifdef USE_INLEAD_FLAG
				pLeader->client->inLead = true;
				pOldLeader->client->inLead = false;
#endif
				gi.bprintf(PRINT_MEDIUM, "%s has taken the lead!\n", pLeader->client->pers.netname);
				gi.sound(pLeader, CHAN_AUTO, gi.soundindex("misc/comp_up.wav"), 1, ATTN_NORM, 0);
			}
		}
	}
	else
	{
		// We already have a leader.
		// Basically we just need to check if anyone matches our score or is ahead of us.
		// If anyone matches our score we take away his flag and tell everyone that player lost lead
		// due to a tie.. however if anyone goes ahead of this guy (double kill in the same frame) then
		// they get to steal the lead right away.
		// This is basically a copy of the entire block above with some minor changes.
		edict_t* tempLeader = NULL;
		int myNumber = (short)(pLeader - g_edicts);
		struct
		{
			int score;
			int index;
		} highestScore = { pLeader->client->resp.score, myNumber };
		// FIXME: Check if the index here is valid..
		// not like it matters in the code below but I like to be right :)

		// So first of all, let's check if anyone has skipped right ahead of us.
		for (i = 0; i < numPlayers; i++)
		{
			if (*scores[i] > highestScore.score)
			{
				// This guy beat him.. but he may not be the only one who did.
				// Store him as the current high scorer
				highestScore.score = *scores[i];
				highestScore.index = i;
				tempLeader = &g_edicts[i];
			}
		}

		if (tempLeader //) FIXME: are the two below necessary?
			|| highestScore.index != myNumber || highestScore.score != pLeader->client->resp.score) // FIXME:
			// remove this .score part when it's validated that the myNumber is valid
		{
			// Someone actually got right ahead of this player.
			// The above check already has the highest scoring player.. but similarily to the 
			// first block we need to see if this guy has a tie-er.
			// check to make sure that no one is tied with us, otherwise our leader status is void.
			for (i = 0; i < numPlayers; i++)
			{
				if (*scores[i] == tempLeader->client->resp.score && players[i] != tempLeader)
				{
					// Someone is tied with us. You are the weakest leader, goodbye.
					tempLeader = NULL;
					break;
				}
			}

			// if we STILL have a leader by here...
			if (tempLeader)
			{
				// This guy is entirely in the lead!
				// Throw him the flag and tell everyone they suck.
				pLeader = tempLeader;
#ifdef USE_INLEAD_FLAG
				pLeader->client->inLead = true;
				pOldLeader->client->inLead = false;
#endif
				gi.bprintf(PRINT_MEDIUM, "%s is no longer in the lead!\n%s has taken the lead!\n",
					pOldLeader->client->pers.netname,
					pLeader->client->pers.netname);
				gi.sound(pLeader, CHAN_AUTO, gi.soundindex("misc/comp_up.wav"), 1, ATTN_NORM, 0);
			}
		}
		else
		{
			// So this means no one has actually beaten the score of the high scorer.
			// As a final means of reconcilliation, let's check if anyone is tied with the
			// current leader.
			for (i = 0; i < numPlayers; i++)
			{
				if (*scores[i] == highestScore.score && (players[i] != pLeader && players[i] != pOldLeader))
				{
					// We got here, so this means that the current leader is no longer a leader at all,
					// just a plain ol' joe.
					highestScore.score = *scores[i];
					highestScore.index = i;

					// Clear his status, let everybody know he isn't in the lead but that no one has taken over.
					pLeader = NULL;
#ifdef USE_INLEAD_FLAG
					pOldLeader->client->inLead = false;
#endif
					gi.bprintf(PRINT_MEDIUM, "%s is no longer in the lead! The spot is up for grabs!\n",
						pOldLeader->client->pers.netname);
				}
			}
		}
	}
}

#endif