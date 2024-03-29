/*============================================================================
ZbotCheck v1.01 for Quake 2 by Matt "WhiteFang" Ayres (matt@lithium.com)

This is provided for mod authors to implement Zbot detection, nothing more.
The code has so far proven to be reliable at detecting Zbot auto-aim clients
(cheaters).  However, no guarantees of any kind are made.  This is provided
as-is.  You must be familiar with Quake 2 mod coding to make use of this.

In g_local.h, add to struct client_respawn_t:

	short angles[2][2];
	int tog;
	int jitter;
	float jitter_time;
	float jitter_last;

Next, in p_client.c, add a simple forward declaration:

	qboolean ZbotCheck(edict_t *ent, usercmd_t *ucmd);

Then in p_client.c, anywhere in the ClientThink function, call the
ZbotCheck function.  Pass it the same parameters you get from ClientThink.
It will return true if the client is using a Zbot.  Simple example:

	if (ZbotCheck(ent, ucmd))
		gi.bprintf(PRINT_HIGH, ">>> Zbot detected: %s\n",
		ent->client->pers.netname);

From here you can do as you please with the cheater.  ZbotCheck will only
return true once, following returns will be false.
============================================================================*/

#include "g_local.h"

#define ZBOT_JITTERMAX	4
#define ZBOT_JITTERTIME	10
#define ZBOT_JITTERMOVE 500

qboolean ZbotCheck(edict_t* ent, usercmd_t* ucmd)
{
	int tog0, tog1;
	client_respawn_t* resp = &ent->client->resp;

	if (!zbot_check->value)
	{
		return false;
	}

	tog0 = resp->tog;
	resp->tog ^= 1;
	tog1 = resp->tog;

	if (ucmd->angles[0] == resp->angles[tog1][0] &&
		ucmd->angles[1] == resp->angles[tog1][1] &&
		ucmd->angles[0] != resp->angles[tog0][0] &&
		ucmd->angles[1] != resp->angles[tog0][1] &&
		abs(ucmd->angles[0] - resp->angles[tog0][0]) +
		abs(ucmd->angles[1] - resp->angles[tog0][1]) >= ZBOT_JITTERMOVE)
	{
		if (level.time <= resp->jitter_last + 0.1)
		{
			if (!resp->jitter)
				resp->jitter_time = level.time;
			if (++resp->jitter == ZBOT_JITTERMAX)
				return true;
		}
		resp->jitter_last = level.time;
	}

	resp->angles[tog1][0] = ucmd->angles[0];
	resp->angles[tog1][1] = ucmd->angles[1];

	if (level.time > resp->jitter_time + ZBOT_JITTERTIME)
		resp->jitter = 0;

	return false;
}
