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

#include "g_local.h"
#include "g_team.h"
#include "m_player.h"

#include "l_angels.h"
#include "scanner.h"
#include "kamikaze.h"
#include "l_ar_admin.h"
#include "l_rockets.h"
#include "l_backpack.h"


void ClientUserinfoChanged (edict_t *ent, char *userinfo);
void ClientPrintMOTD (edict_t *ent);

void SP_misc_teleporter_dest (edict_t *ent);
void Create_Ghost(void);

//
// Gross, ugly, disgustuing hack section
//

// this function is an ugly as hell hack to fix some map flaws
//
// the coop spawn spots on some maps are SNAFU.  There are coop spots
// with the wrong targetname as well as spots with no name at all
//
// we use carnal knowledge of the maps to fix the coop spot targetnames to match
// that of the nearest named single player spot

static void SP_FixCoopSpots (edict_t *self)
{
	edict_t	*spot;
	vec3_t	d;

	spot = NULL;

	for (;;)
	{
		spot = G_Find(spot, FOFS(classname), "info_player_start");
		if (!spot)
			return;
		if (!spot->targetname)
			continue;
		VectorSubtract(self->s.origin, spot->s.origin, d);
		if (VectorLength(d) < 384)
		{
			if ((!self->targetname) || Q_strcasecmp(self->targetname, spot->targetname) != 0)
			{
				//gi.dprintf("FixCoopSpots changed %s at %s targetname from %s to %s\n", self->classname, vtos(self->s.origin), self->targetname, spot->targetname);
				self->targetname = spot->targetname;
			}
			return;
		}
	}
}

// now if that one wasn't ugly enough for you then try this one on for size
// some maps don't have any coop spots at all, so we need to create them
// where they should have been

static void SP_CreateCoopSpots (edict_t *self)
{
	edict_t	*spot;

	if (Q_strcasecmp(level.mapname, "security") == 0)
	{
		spot = G_Spawn();
		spot->classname = "info_player_coop";
		spot->s.origin[0] = 188 - 64;
		spot->s.origin[1] = -164;
		spot->s.origin[2] = 80;
		spot->targetname = "jail3";
		spot->s.angles[1] = 90;

		spot = G_Spawn();
		spot->classname = "info_player_coop";
		spot->s.origin[0] = 188 + 64;
		spot->s.origin[1] = -164;
		spot->s.origin[2] = 80;
		spot->targetname = "jail3";
		spot->s.angles[1] = 90;

		spot = G_Spawn();
		spot->classname = "info_player_coop";
		spot->s.origin[0] = 188 + 128;
		spot->s.origin[1] = -164;
		spot->s.origin[2] = 80;
		spot->targetname = "jail3";
		spot->s.angles[1] = 90;

		return;
	}
}


/*QUAKED info_player_start (1 0 0) (-16 -16 -24) (16 16 32)
The normal starting point for a level.
*/
void SP_info_player_start(edict_t *self)
{
	if (coop->value && Q_strcasecmp(level.mapname, "security") == 0)
	{
		// invoke one of our gross, ugly, disgusting hacks
		self->think = SP_CreateCoopSpots;
		self->nextthink = level.time + FRAMETIME;
	}
}

/*QUAKED info_player_deathmatch (1 0 1) (-16 -16 -24) (16 16 32)
potential spawning position for deathmatch games
*/
void SP_info_player_deathmatch(edict_t *self)
{
	if (!deathmatch->value)
	{
		G_FreeEdict (self);
		return;
	}

	SP_misc_teleporter_dest (self);

	// If this is a teamplay game, do some more setup.
	if (ctf->value)
		TeamplaySetupSpawnpoint (self);
}

/*QUAKED info_player_coop (1 0 1) (-16 -16 -24) (16 16 32)
potential spawning position for coop games
*/

void SP_info_player_coop(edict_t *self)
{
	if (!coop->value)
	{
		G_FreeEdict (self);
		return;
	}

	if ((Q_strcasecmp(level.mapname, "jail2") == 0)   ||
		(Q_strcasecmp(level.mapname, "jail4") == 0)   ||
		(Q_strcasecmp(level.mapname, "mine1") == 0)   ||
		(Q_strcasecmp(level.mapname, "mine2") == 0)   ||
		(Q_strcasecmp(level.mapname, "mine3") == 0)   ||
		(Q_strcasecmp(level.mapname, "mine4") == 0)   ||
		(Q_strcasecmp(level.mapname, "lab") == 0)     ||
		(Q_strcasecmp(level.mapname, "boss1") == 0)   ||
		(Q_strcasecmp(level.mapname, "fact3") == 0)   ||
		(Q_strcasecmp(level.mapname, "biggun") == 0)  ||
		(Q_strcasecmp(level.mapname, "space") == 0)   ||
		(Q_strcasecmp(level.mapname, "command") == 0) ||
		(Q_strcasecmp(level.mapname, "power2") == 0)  ||
		(Q_strcasecmp(level.mapname, "strike") == 0))
	{
		// invoke one of our gross, ugly, disgusting hacks
		self->think = SP_FixCoopSpots;
		self->nextthink = level.time + FRAMETIME;
	}
}


/*QUAKED info_player_intermission (1 0 1) (-16 -16 -24) (16 16 32)
The deathmatch intermission point will be at one of these
Use 'angles' instead of 'angle', so you can set pitch or roll as well as yaw.  'pitch yaw roll'
*/
void SP_info_player_intermission(void)
{
}


//=======================================================================


void player_pain (edict_t *self, edict_t *other, float kick, int damage)
{
	// player pain is handled at the end of the frame in P_DamageFeedback
}


qboolean IsFemale (edict_t *ent)
{
	char		*info;

	if (!ent->client)
		return QFALSE;

	info = Info_ValueForKey (ent->client->pers.userinfo, "skin");
	if (info[0] == 'f' || info[0] == 'F')
		return QTRUE;
	return QFALSE;
}


void ClientObituary (edict_t *self, edict_t *inflictor, edict_t *attacker)
{
	int			i, mod;
	char		*message;
	char		*message2;
	qboolean	ff, nofrag;
	edict_t		*temp = NULL;
	edict_t		*hifragger;

	//	if (attacker->client->onenemydeath != NULL)
	//		stuffcmd(attacker,attacker->client->onenemydeath);

	if (coop->value && attacker->client)
		meansOfDeath |= MOD_FRIENDLY_FIRE;

	if (deathmatch->value || coop->value)
	{
		ff = meansOfDeath & MOD_FRIENDLY_FIRE;
		nofrag = meansOfDeath & MOD_NOFRAG;
		mod = meansOfDeath & ~(MOD_FRIENDLY_FIRE|MOD_NOFRAG);
		message = NULL;
		message2 = "";

		// ARSG compatible output for statistics collection
		// see l_ar_admin.c for the ARLog_Stats function.
		if (logarstats->value)
		{
			if (attacker->client != self->client && attacker->client != NULL) 
				ARLog_Stats("OBIT: %s\\%s\\%d\\%d\\%d\\%d\\%s\n", //a player fragged another player
				self->client->pers.netname, //victim
				attacker->client->pers.netname, //attacker
				self->client->ping, 
				attacker->client->ping, 
				mod, //means of death
				/*hitLocation*/NULL,
				level.mapname); 
			else 
				ARLog_Stats("KILL: %s\\%d\\%d\\%d\\%s\n",	// a player killed himself
				self->client->pers.netname, 
				self->client->ping, 
				mod, 
				/*hitLocation*/NULL,
				level.mapname); 
		}

		switch (mod)
		{
		case MOD_SUICIDE:
			message = "decides to end it all";
			break;
		case MOD_FALLING:
			message = "fought the gravity and the gravity won";
			break;
		case MOD_CRUSH:
			message = "was highly compressed";
			break;
		case MOD_WATER:
			message = "ran out of air";
			break;
		case MOD_SLIME:
			stuffcmd(self,"say My eyes! The goggles do nothing!\n");
			message = "unsuccessfully attempted to emulate Radioactiveman";
			break;
		case MOD_LAVA:
			message = "melted like a slab of Velveeta";
			break;
		case MOD_EXPLOSIVE:
			message = "couldn't contain it all";
			break;
		case MOD_BARREL:
			message = "went Niagra";
			break;
		case MOD_EXIT:
			message = "followed Elvis out of the building";
			break;
		case MOD_TARGET_LASER:
			message = "tripped over the little wire with the big bomb";
			break;
		case MOD_TARGET_BLASTER:
			message = "died foolishly";
			break;
		case MOD_CHAMBER: 
			message="was forced to watch too many Jerry Springer reruns"; 
			break; 
		case MOD_SHRAPNEL:
			if (IsFemale(self))
				message = "exposed her organs to the air";
			else
				message = "exposed his organs to the air";
			break;
			break;
		case MOD_BATON: 
			message="was killed by a dumbstick"; 
			break; 

		case MOD_BOMB:
		case MOD_SPLASH:
		case MOD_TRIGGER_HURT:
			message = "left the boundaries of normal space";
			break;
		}
		if (attacker == self)
		{
			switch (mod)
			{
			case MOD_HELD_GRENADE:
				message = "tried to put the pin back in";
				break;
			case MOD_HG_SPLASH:
			case MOD_G_SPLASH:
				if (IsFemale(self))
					message = "spread herself too thin";
				else
					message = "spread himself thin";
				break;
			case MOD_CHAMBER: 
				message="saw the sharp end of the evil poking stick"; 
				break; 
			case MOD_R_SPLASH:
			case MOD_FIRE_SPLASH:
				if (IsFemale(self))
					message = "fried her own little self";
				else
					message = "fried his own little self";
				break;
			case MOD_H_SPLASH:
				if (IsFemale(self))
					message = "blew her mind out";
				else
					message = "blew his mind out";
				break;
			case MOD_BATON: 
				if (IsFemale(self)) 
					message="got killed by her dumbstick"; 
				else 
					message="got killed by his dumbstick"; 
				break; 
			case MOD_BFG_BLAST:
				if (IsFemale(self))
					message = "used more power than she could handle";
				else
					message = "used more power than he could handle";
				break;
			case MOD_MACHINEGUN:
				if (IsFemale(self))
					message = "rocketed her own pocket";
				else
					message = "rocketed his own pocket";
				break;
			case MOD_PROXIMITY:
				if (IsFemale(self))
					message = "bombed her own body";
				else
					message = "bombed his own body";
				break;
			case MOD_PLASMAG:
				if (IsFemale(self))
					message = "clustered her own face";
				else
					message = "clustered his own face";
				break;
			case MOD_CLUSTER:
				if (IsFemale(self))
					message = "sent her own bits flying";
				else
					message = "sent his own bits flying";
				break;
			case MOD_NAPALM:
				if (IsFemale(self))
					message = "Cajun-fried her own hide";
				else
					message = "Cajun-fried his own hide";
				break;
			case MOD_KAMIKAZE:
				message = "went pop in the night";
				break;
			case MOD_SHRAPG:
				if (IsFemale(self))
					message = "shredded herself with a shrapnel grenade";
				else
					message = "shredded himself with a shrapnel grenade";
				break;
			case MOD_ROCKET:
				if (IsFemale(self))
					message = "put a big Bic to her own behind";
				else
					message = "put a big Bic to his own behind";
				break;			
			case MOD_HOMING:
				if (IsFemale(self))
					message = "said WELCOME to her own homer";
				else
					message = "said WELCOME to his own homer";
				break;
			case MOD_FGRENADE:
				if (IsFemale(self))
					message = "serenaded herself with nades";
				else
					message = "serenaded himself with nades";
				break;
			case MOD_RAILGUN:
				message = "stood too close to the flames";
				break;
			case MOD_CATA:
				if (IsFemale(self))
					message = "converted herself to gaseous form";
				else
					message = "became a big cloud of dust";
				break;
			default:
				if (IsFemale(self))
					message = "killed herself";
				else
					message = "killed himself";
				break;
			}
		}
		if (message)
		{
			gi.bprintf (PRINT_MEDIUM, "%s %s.\n", 
				self->client->pers.netname,
				message);
			if (deathmatch->value)
			{
				self->client->resp.score--;
				self->client->resp.deaths++;
			}
			self->enemy = NULL;
			return;
		}

		// message gets a space appended automatically
		// message2 doesn't get spaced so 's display properly
		// if message2 text follows without 's then prepend a space in the text

		self->enemy = attacker;
		if (attacker && attacker->client)
		{
			switch (mod)
			{
			case MOD_CATA:
				message = "was shaken to death by";
				message2 = "'s cataclysm device";
				break;
			case MOD_TELEFRAG:
				message = "failed to respect";
				message2 = "'s bubble";
				break;
			case MOD_BATON: 
				message="was beaten by"; 
				message2="'s dumbstick"; 
				break; 
			case MOD_H_SPLASH:
				message = "couldn't outrun";
				message2 = "'s homer";
				break;
			case MOD_FREEZE:
				message = "was frozen by";
				message2 = "'s deep, chilling breath";
				break;
			case MOD_PLASMAGUN:
				message = "was atomized by";
				message2 = "'s plasma rifle";
				break;
			case MOD_BOLTTHROWER:
				message = "was electrocuted by";
				message2 = "'s bolt thrower";
				break;
			case MOD_SNIPER:
				message = "was cut down by";
				message2 = "'s flying bits of snipe";
				break;
			case MOD_BLASTER:
				message = "died a meaningless death at the end of";
				message2 = "'s laser";
				break;
			case MOD_FGRENADE:
				message = "was illuminated by";
				message2 = "'s fire";
				break;
			case MOD_SHRAPG:
				message = "was shredded by";
				message2 = "'s Quisinart grenade";
				break;
			case MOD_HOMING:
				message = "was sought out by";
				message2 = "'s flying telegram of death";
				break;
			case MOD_TRIPWIRE:
				message = "tripped on";
				message2 = "'s light fantastic";
				break;
			case MOD_SHOTGUN:
				message = "refused to cooperate at";
				message2 = "'s shotgun wedding";
				break;
			case MOD_SSHOTGUN:
				message = "was popped like a wet grocery bag by";
				message2 = "'s super shotgun";
				break;
			case MOD_MACHINEGUN:
				message = "succumbed to the seductive rhythm of";
				message2 = "'s machine rockets";
				break;
			case MOD_MACHINE:
				message = "was gunned down by";
				break;
			case MOD_KAMIKAZE:
				message = "wouldn't let";
				message2 = " leave the world alone";
				break;
			case MOD_CHAINGUN:
				message = "was cut in half by";
				message2 = "'s chaingun";
				break;
			case MOD_GRENADE:
			case MOD_HANDGRENADE:
				message = "was popped like a big pimple by";
				message2 = "'s grenade";
				break;
			case MOD_HG_SPLASH:
			case MOD_G_SPLASH:
				message = "was sliced, diced, and shredded by";
				message2 = "'s grenade";
				break;
			case MOD_ROCKET:
				message = "was fried black by";
				message2 = "'s flying BK broiler";
				break;
			case MOD_SB:
				message = "saw light shining through the holes made by";
				message2 = "'s super blaster";
				break;
			case MOD_GRAPPLE:
				message = "got the Freddy Krueger treatment at the working end of";
				message2 = "'s hook";
				break;
			case MOD_R_SPLASH:
				message = "was not happy to see the rocket in";
				message2 = "'s pocket";
				break;
			case MOD_HYPERBLASTER:
				message = "made like a patty and melted at the end of";
				message2 = "'s hyperblaster";
				break;
			case MOD_RAILGUN:
				message = "was browned on both sides by";
				break;
			case MOD_RAILGUN2:
				message = "was ventilated by slugs from";
				message2 = "'s rail";
				break;
			case MOD_SUPERRAILSHOTGUN:
				message = "was splattered profusely by the spray from";
				message2 = "'s super rail shotgun";
				break;
			case MOD_STREETSWEEP:
				message = "was ground while standing up by";
				message2 = "'s streetsweeper";
				break;
			case MOD_BFG_LASER:
				message = "saw";
				message2 = "'s BFG just a little too late";
				break;
			case MOD_FBFG_BLAST:
				message = "joined";
				message2 = "'s FBFG Blue Man Group";
				break;
			case MOD_BFG_BLAST:
				message = "went all to pieces from";
				message2 = "'s BFG blast";
				break;
			case MOD_BFG_EFFECT:
				message = "went all sputtery from";
				message2 = "'s BFG";
				break;
			case MOD_NAPALM:
				message = "could not get away from";
				message2 = "'s napalm spam assault";
				break;
			case MOD_PROXIMITY:
				message = "was serenaded by";
				message2 = "'s organ-bursting 'nade melody";
				break;
			case MOD_CLUSTER:
				message = "couldn't run fast enough to get away from";
				message2 = "'s cluster grenades";
				break;
			case MOD_RAILBOMB:
				message = "stood too close to";
				message2 = "'s rail bomb";
				break;
			case MOD_PLASMAG:
				message = "formed a human comet when exposed to";
				message2 = "'s plasma grenade";
				break;
				/* case MOD_HANDGRENADE:
				message = "caught";
				message2 = "'s handgrenade";
				break; */
				/* case MOD_HG_SPLASH:
				message = "didn't see";
				message2 = "'s handgrenade";
				break; */
			case MOD_DISRUPTOR:
				message = "was deeply upset by";
				message2 = "'s disruptor";
				break;
			case MOD_VACUUM:
				message = "got the Kirby Treatment from";
				message2 = "'s Vacuum Maker";
				break;
			case MOD_VORTEX:
				message = "walked through";
				message2 = "'s dimensional passageway";
				break;
			case MOD_HELD_GRENADE:
				message = "signs up for";
				message2 = "'s outpatient grenade surgery";
				break;
			case MOD_SUPERBLASTERSWEEPER:
				message = "thinks";
				message2 = "'s big gun is tons of fun";
				break;
			case MOD_FREEZERSWEEPER:
				message = "went all blue and stiff at the sight of";
				message2 = "'s freeze wand";
				break;
			case MOD_FREEZATRON:
				message = "stood in the midst of";
				message2 = "'s icy bubble";
				break;
			case MOD_GRENADESWEEPER:
			case MOD_BAZOOKASWEEPER:
				message = "felt the earth move as";
				message2 = "'s grenade shower fell gently from the sky";
				break;
			case MOD_RAILGUNSWEEPER:
				message = "took one for the home team as";
				message2 = "'s big railsweeper made an abrupt entry";
				break;

			case MOD_PLASMACLUSTERGRENADE:
				{
					char *messageArr1[5] = {"was flustered by","had an At Death Experience on the branches of", 
						"went all atomic from","was geometrically redistributed by", "was rendered assunder by" };
					char *messageArr2[5] = {"'s cluster","'s tree of death","'s glowing plasma hairball", 
						"'s flying fickle plasma finger of fate", "'s plasma weapon"};
					message = messageArr1[((int)(random() * 100.0))%5]; 
					message2 = messageArr2[((int)(random() * 100.0))%5];
				}
				break;
			case MOD_TELEPORTGRENADE:
				message = "tried to share molecules with";
				message2 = "'s body";
				break;
			case MOD_AIRFIST:
				message	= "was stifled by";
				message2 = "'s hot bad breath";
				break;
			case MOD_POSITRON:
				message = "was permanently altered by";
				message2 = "'s positronic charm";
				break;
			case MOD_SWORD:
				message = "was hacked to itty bitty bits by";
				message2 = "'s longsword";
				break;
			case MOD_BUCKY:
				{
					char *messageArr1[5] = {"was laughed at by","was spit on by","was pounced by","was stomped on by", 
						"'s slapped around by"};
					char *messageArr2[5] = {"'s q-ball","'s wacky grenade","'s rabid tribble",
						"'s flying rice krispy treat", "'s wet trout"};
					message = messageArr1[((int)(random() * 100.0))%5]; 
					message2 = messageArr2[((int)(random() * 100.0))%5];
				}
				break;

			case MOD_DISCHARGE:
			case MOD_LIGHTNING:
				message = "shuddered at the touch of";
				message2 = "'s special lightning";
				break;
			case MOD_NAIL:
				message = "'s coffin was nailed shut by";
				message2 = "";
				break;
			case MOD_SUPERNAIL:
				message = "was punctuated with holes from";
				message2 = "'s supernail shooter";
				break;
			case MOD_ROCKETGRENADE:
				message = "flew to pieces from";
				message2 = "'s rocket grenade";
				break;
			case MOD_HOMERGRENADE:
				message = "found there was no place like";
				message2 = "'s homer grenade";
				break;
			case MOD_EGG:
				message = "got";
				message2 = "'s egg in the face";
				break;
			case MOD_PLAGUE:
				message = "caught";
				message2 = "'s raging disease";
				break;
			case MOD_CHAMBER: 
				message="died from"; 
				message2="'s tender, torturous touch"; 
				break; 
			case MOD_EXPLOSIVECHAINGUN:
				message="felt the rage of";
				message2="'s explosive temper";
				break;
			case MOD_EXPLOSIVESTREETSWEEPER:
				message="heard the snap and crackle of";
				message2="'s popping sweeper";
				break;
			case MOD_EXPLOSIVESHOTGUN:
				message="was splattered all over by";
				message2 = "'s explosive shotgun";
				break;
			case MOD_EXPLOSIVESSHOTGUN:
				message="burst into gibs while eating";
				message2="'s firecrackers";
				break;
			case MOD_EXPLOSIVEMACHINEGUN:
				message="burst into gibs while eating";
				message2="'s machinegun firecrackers";
				break;
			case MOD_GUIDEDNUKE:
				message = "was nuked by";
				message2 = "'s smart bomb";
				break;
			case MOD_NUKE:
				message = "fell prey to";
				message2 = "'s WoMD";
				break;
			case MOD_ANTIMATTERCANNON:
				message = "was shredded at a nuclear level by";
				message2 = "'s antimatter cannon";
				break;
			}

			attacker->client->killedby = self;
			self->client->killedby = attacker;
			if (message)
			{
				gi.bprintf (PRINT_MEDIUM,"%s %s %s%s.\n",
					self->client->pers.netname, 
					message,
					attacker->client->pers.netname,
					message2);

				if (deathmatch->value)
				{
					if (ff) // mod masked by MOD_FRIENDLY_FIRE
						attacker->client->resp.score--;
					else if (!nofrag)
					{
						// do we award extra points for revenge?
						if ((int)revenge_factor->value)
						{
							// determine who is highfragger now
							hifragger = g_edicts +1;
							for (i=0 ; i < game.maxclients ; i++)
							{
								temp = g_edicts + 1 + i;
								if (!temp->inuse)
									continue;
								if (temp->client->resp.score >= hifragger->client->resp.score)
									hifragger = temp;		
							}

							//gi.bprintf(PRINT_MEDIUM, "Highfragger is %s\n", temp->client->pers.netname);
							if (self->client->killedby != NULL)
							{
								if (Q_strcasecmp (self->client->killedby->client->pers.netname, attacker->client->pers.netname)==0)
								{
									attacker->client->resp.score = 1 + game.revengereward + attacker->client->resp.score;
									gi.bprintf (PRINT_MEDIUM, "%s awarded extra point for getting revenge on %s\n",
										attacker->client->pers.netname, self->client->pers.netname);
								}

							}

							if (Q_strcasecmp (hifragger->client->pers.netname,self->client->pers.netname)==0)
							{
								attacker->client->resp.score = 1 + game.highfraggeraward + attacker->client->resp.score;
								gi.bprintf (PRINT_MEDIUM, "%s awarded extra points for killing %s\n",
									attacker->client->pers.netname, self->client->pers.netname);
							}
							else
							{
								if (self->client->killedby == NULL) 
								{
									attacker->client->resp.score++;
									self->client->resp.deaths++;
								}
							}
							self->client->killedby = NULL;
						}
						else // no REVENGE_FACTOR, normal scoring.
						{
							attacker->client->resp.score++;
							self->client->resp.deaths++;
						}
					}
				}
				return;
			}
		}
	}

	gi.bprintf (PRINT_MEDIUM,"%s died.\n", self->client->pers.netname);
	if (deathmatch->value)
	{
		self->client->resp.score--;
		self->client->resp.deaths++;
	}
}


void TossClientWeapon (edict_t *self)
{
	gitem_t		*item;
	edict_t		*drop;
	qboolean	quad;
	float		spread;

	if (!deathmatch->value)
		return;

	// Get the client's weapon.
	item = self->client->pers.weapon;

	// If they had no ammo left, or if it's the blaster
	// or one of the special weapons, don't throw it.
	if (item == &gI_weapon_blaster)
		item = &gI_ammo_cells; //QW// toss cells
	else if (!self->client->pers.inventory[self->client->ammo_index]) //out of ammo, no drop
		item = NULL;
	else if (item == &gI_weapon_superblaster)
		item = &gI_ammo_cells; //QW// toss cells
	else if (item == &gI_weapon_flaregun)
		item = NULL;
	else if (item == &gI_weapon_antiflaregun)
		item = NULL;
	else if (item == &gI_weapon_mace)
		item = NULL;

	if (!((int)(dmflags->value) & DF_QUAD_DROP))
		quad = QFALSE;
	else
		quad = (self->client->quad_framenum > (level.framenum + 10));

	if (item && quad)
		spread = 22.5;
	else
		spread = 0.0;

	if (item)
	{
		self->client->v_angle[YAW] -= spread;
		drop = Drop_Item (self, item);
		self->client->v_angle[YAW] += spread;
		drop->spawnflags = DROPPED_PLAYER_ITEM;
	}

	if (quad)
	{
		self->client->v_angle[YAW] += spread;
		drop = Drop_Item (self, &gI_item_quad);
		self->client->v_angle[YAW] -= spread;
		drop->spawnflags |= DROPPED_PLAYER_ITEM;

		drop->touch = Touch_Item;
		drop->nextthink = level.time + (self->client->quad_framenum - level.framenum) * FRAMETIME;
		drop->think = G_FreeEdict;
	}
}


//==================
//  LookAtKiller
//==================

void LookAtKiller (edict_t *self, edict_t *inflictor, edict_t *attacker)
{
	vec3_t		dir;

	if (attacker && attacker != world && attacker != self)
	{
		VectorSubtract (attacker->s.origin, self->s.origin, dir);
	}
	else if (inflictor && inflictor != world && inflictor != self)
	{
		VectorSubtract (inflictor->s.origin, self->s.origin, dir);
	}
	else
	{
		self->client->killer_yaw = self->s.angles[YAW];
		return;
	}

	if (dir[0])
		self->client->killer_yaw = 180/M_PI*atan2(dir[1], dir[0]);
	else
	{
		self->client->killer_yaw = 0;
		if (dir[1] > 0)
			self->client->killer_yaw = 90;
		else if (dir[1] < 0)
			self->client->killer_yaw = -90;
	}
	if (self->client->killer_yaw < 0)
		self->client->killer_yaw += 360;
}


//==================
//   player_die
//==================

void player_die (edict_t *self, edict_t *inflictor, edict_t *attacker, int damage, vec3_t point)
{

	int		n;

	// execute the ondeath string if there is one
	if (Q_strcasecmp (self->client->pers.ondeath, "") != 0)
		stuffcmd(self, self->client->pers.ondeath);

	// execute the attacker's onenemydeath string if there is one
	if (attacker->client != NULL)
	{
		if (Q_strcasecmp (attacker->client->pers.onenemydeath, "") != 0)
			stuffcmd(attacker, attacker->client->pers.onenemydeath);
	}

	self->client->mine = 0;
	self->client->mine_count = 0;
	self->client->curr_heat = 0;
	self->client->heat_active = QFALSE;
	self->client->chamber = 0; // Turn OFF. 
	self->client->baton = 0; // Turn OFF. 

	if (self->client->holo != NULL)
		G_FreeEdict(self->client->holo);

	if (self->client->feign)
		Cmd_FeignDeath (self);

	VectorClear (self->avelocity);
	ClearScanner (self->client);

	// detonate any undetonated explosive
	if (self->client->explosive != NULL)
		self->client->explosive->nextthink = level.time + 3;

	Kamikaze_Cancel(self); /* No Kamikaze Now!!*/

	if (self->thirdperson)
		ThirdEnd (self);

	// Remove any decoy.
	if (self->decoy)
		free_decoy (self);

	// free any angels he was using
	if (self->client->angel != NULL)
	{
		G_FreeEdict (self->client->angel);
		self->client->angel = NULL;
	}

	// No grappling hook when you're dead.
	self->client->hookstate = 0;

	// No being frozen when you're dead.
	self->frozen = 0;
	self->frozentime = level.time;
	self->unfrozen = 1;

	self->client->cloak_time = level.time;
	self->takedamage = DAMAGE_YES;

	self->movetype = MOVETYPE_TOSS;

	self->s.modelindex2 = 0;	// remove linked weapon model
	self->client->ps.gunindex = 0;

	self->s.angles[0] = 0;
	self->s.angles[2] = 0;

	self->s.sound = 0;
	self->client->weapon_sound = 0;

	self->maxs[2] = -8;

	//	self->solid = SOLID_NOT;
	self->svflags |= SVF_DEADMONSTER;

	if (!self->deadflag)
	{
		//Wyrm: guided missile
		if (self->client->missile)
			GuidedRocket_Think(self->client->missile);

		//Wyrm: chasecam
		ChasecamRemove(self);
		self->client->respawn_time = level.time + 1.0f;
		LookAtKiller (self, inflictor, attacker);
		self->client->ps.pmove.pm_type = PM_DEAD;
		ClientObituary (self, inflictor, attacker);

		// If backpack feature enabled and limited ammo game
		if (backpacktoss->value) // && (!((int)dmflags->value & DF_INFINITE_AMMO)))
			Backpack_ClientToss(self); 	// Toss out a backpack full of player's items
		else
			TossClientWeapon (self);	// Toss out the player's current weapon

		if (deathmatch->value && !self->client->showscores)
			Cmd_Help_f (self);		// show scores
	}

	// remove powerups
	self->client->quad_framenum = 0;
	self->client->invincible_framenum = 0;
	self->client->breather_framenum = 0;
	self->client->enviro_framenum = 0;

	// clear inventory
	memset(self->client->pers.inventory, 0, sizeof(self->client->pers.inventory));

	if (Jet_Active(self))
	{ 
		Jet_BecomeExplosion( self, damage );
		/*stop jetting when dead*/
		self->client->Jet_framenum = 0;
	}
	else 

		if (self->health < -40)
		{	// gib
			gi.sound (self, CHAN_BODY, gi.soundindex ("misc/udeath.wav"), 1, ATTN_NORM, 0);
			for (n= 0; n < 4; n++)
				ThrowGib (self, "models/objects/gibs/sm_meat/tris.md2", damage, GIB_ORGANIC);
			
			ThrowClientHead (self, damage);
			self->client->anim_priority = ANIM_DEATH;
			self->client->anim_end = 0;
			self->takedamage = DAMAGE_NO;
		}
		else
		{	// normal death
			if (!self->deadflag)
			{
				static int i;

				i = (i + 1) % 3;
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
			}
		}
		// CCH: call off strike       
		self->client->airstrike_called = 0;
		if (self->lasersight != NULL)
			SP_LaserSight(self);
		self->deadflag = DEAD_DEAD;
		gi.linkentity (self);
}

//=======================================================================
// InitClientPersistant

// This is only called when the game first initializes in single player,
// but is called after each death and level change in deathmatch
//=======================================================================

void InitClientPersistant (gclient_t *client)
{
	gitem_t		*item;
	int ipAddr,fph_active,heightactive,playeridactive,rangefinderactive;
	int lasertrips;
	char ondeath[MAX_INFO_STRING];
	char onrespawn[MAX_INFO_STRING];
	char onenemydeath[MAX_INFO_STRING];

	ipAddr = client->pers.ipAddr;
	fph_active = client->resp.fph_active;
	heightactive = client->resp.heightactive;
	playeridactive = client->resp.playeridactive;
	rangefinderactive = client->resp.rangefinderactive;
	lasertrips = client->lasertrips;		//QW//

	//	effects = client->pers.effects;
	//	renderfx = client->pers.renderfx;

	// copy persistent strings to temp storage
	memcpy (ondeath, client->pers.ondeath, sizeof(ondeath));
	memcpy (onrespawn, client->pers.onrespawn, sizeof(onrespawn));
	memcpy (onenemydeath, client->pers.onenemydeath, sizeof(onenemydeath));

	memset (&client->pers, 0, sizeof(client->pers));
	client->guidedMissileFired = 0;
	client->mine_count = 0;
	client->mine = 0;
	client->heat_active = QFALSE;
	client->curr_heat = 0;
	client->chamber=0; // default to OFF. 
	client->scannerx = 80;
	client->scannery = 40;
	client->baton = 0; // default to OFF. 
	client->tracker = 0;
	client->turret = 0;
	client->expactive = 0;
	client->explosive = NULL;
	client->organicarmor = 0;
	client->cellarmor = 0;
	client->slugarmor = 0;
	client->shellarmor = 0;
	client->bulletarmor = 0;
	client->rocketarmor = 0;
	client->grenadearmor = 0;
	client->loxarmor = 0;
	client->loxhealth = 0;
	client->regenhealth = 0;
	client->vampihealth = 0;
	client->horrortime = 0;
	client->alpha = 0;

	client->resp.heightactive = heightactive;
	client->resp.playeridactive = playeridactive;
	client->resp.rangefinderactive = rangefinderactive;
	client->resp.fph_active = fph_active;
	client->lasertrips = lasertrips;		//QW//

	client->pers.ipAddr = ipAddr;
	// Give them the super blaster.
	client->pers.inventory[ITEM_INDEX(&gI_weapon_superblaster)] = 1;
	client->pers.inventory[ITEM_INDEX(&gI_weapon_flaregun)] = 1;
	client->pers.inventory[ITEM_INDEX(&gI_weapon_antiflaregun)] = 1;
	client->pers.inventory[ITEM_INDEX(&gI_weapon_mace)] = 1;
	// Give them the blaster.
	item = &gI_weapon_blaster;
	client->pers.selected_item = ITEM_INDEX(item);
	client->pers.inventory[client->pers.selected_item] = 1;

	client->pers.weapon = item;
	client->pers.reallastweapon = item;
	//ZOID
	client->pers.lastweapon = item;
	//ZOID

	client->pers.health			= 100;
	client->pers.max_health		= 100;

	// Original settings
	// client->pers.max_bullets		= 250;
	// client->pers.max_shells		= 100;
	// client->pers.max_rockets		= 50;
	// client->pers.max_grenades	= 50;
	// client->pers.max_cells		= 200;
	// client->pers.max_slugs		= 200;

	// New, gnarlier settings
	client->pers.max_bullets	= 400;
	client->pers.max_shells		= 200;
	client->pers.max_rockets	= 100;
	client->pers.max_grenades	= 100;
	client->pers.max_cells		= 400;
	client->pers.max_slugs		= 300;

	client->pers.fire_mode = 0; // Muce: initialize to FA
	client->pers.lastweapons[0] = &gI_weapon_blaster;
	client->pers.lastweapons[1] = &gI_weapon_shotgun;
	client->pers.lastweapons[2] = &gI_weapon_supershotgun;
	client->pers.lastweapons[3] = &gI_weapon_machine;
	client->pers.lastweapons[4] = &gI_weapon_chaingun;
	client->pers.lastweapons[5] = &gI_weapon_grenadelauncher;
	client->pers.lastweapons[6] = &gI_weapon_rocketlauncher;
	client->pers.lastweapons[7] = &gI_weapon_hyperblaster;
	client->pers.lastweapons[8] = &gI_weapon_railgun;
	client->pers.lastweapons[9] = &gI_weapon_bfg;

	// LBind Init
	// Init the LBind code to an empty string no matter what.
	//	client->pers.effects = effects;
	//	client->pers.renderfx = renderfx;
	// restore persistent strings from the temps
	memcpy (client->pers.ondeath, ondeath, sizeof(ondeath));
	memcpy (client->pers.onrespawn, onrespawn, sizeof(onrespawn));
	memcpy (client->pers.onenemydeath, onenemydeath, sizeof(onenemydeath));

	client->pers.connected = QTRUE;
	ClearScanner(client);
}


void InitClientResp (gclient_t *client)
{
	int ctf_team = client->resp.ctf_team; //ZOID

	memset (&client->resp, 0, sizeof(client->resp));
	client->resp.ctf_team = ctf_team; //ZOID
	client->sweeperfiremode = SFM_LOW;
	client->resp.enterframe = level.framenum;
	client->resp.entertime = client->resp.enterframe / 10.0f;
	client->resp.coop_respawn = client->pers;

	if (ctf->value && client->resp.ctf_team < CTF_TEAM1)	//ZOID
		CTFAssignTeam(client);
}

/*
==================
SaveClientData

Some information that should be persistant, like health, 
is still stored in the edict structure, so it needs to
be mirrored out to the client structure before all the
edicts are wiped.
==================
*/
void SaveClientData (void)
{
	int		i;
	edict_t	*ent;

	for (i = 0; i < game.maxclients; i++)
	{
		ent = &g_edicts[1+i];
		if (!ent->inuse)
			continue;
		game.clients[i].pers.health = ent->health;
		game.clients[i].pers.max_health = ent->max_health;
		game.clients[i].pers.powerArmorActive = (ent->flags & FL_POWER_ARMOR);
		if (coop->value)
			game.clients[i].pers.score = ent->client->resp.score;
	}
}

void FetchClientEntData (edict_t *ent)
{
	ent->health = ent->client->pers.health;
	ent->max_health = ent->client->pers.max_health;
	if (ent->client->pers.powerArmorActive)
		ent->flags |= FL_POWER_ARMOR;
	if (coop->value)
		ent->client->resp.score = ent->client->pers.score;
}




//=======================================================================
//
//  SelectSpawnPoint
//  
//=======================================================================


//================
//PlayersRangeFromSpot
//
//  Returns the distance to the nearest player from the given spot
//================

float	PlayersRangeFromSpot (edict_t *spot)
{
	edict_t	*player;
	float	bestplayerdistance;
	vec3_t	v;
	int		n;
	float	playerdistance;


	bestplayerdistance = 9999999;

	for (n = 1; n <= maxclients->value; n++)
	{
		player = &g_edicts[n];

		if (!player->inuse)
			continue;

		if (player->health <= 0)
			continue;

		VectorSubtract (spot->s.origin, player->s.origin, v);
		playerdistance = VectorLength (v);

		if (playerdistance < bestplayerdistance)
			bestplayerdistance = playerdistance;
	}

	return bestplayerdistance;
}

/*
================
SelectRandomDeathmatchSpawnPoint

go to a random point, but NOT the two points closest
to other players
================
*/
edict_t *SelectRandomDeathmatchSpawnPoint (void)
{
	edict_t	*spot, *spot1, *spot2;
	int		count = 0;
	int		selection;
	float	range, range1, range2;

	spot = NULL;
	range1 = range2 = 99999;
	spot1 = spot2 = NULL;

	while ((spot = G_Find (spot, FOFS(classname), "info_player_deathmatch")) != NULL)
	{
		count++;
		range = PlayersRangeFromSpot(spot);
		if (range < range1)
		{
			range1 = range;
			spot1 = spot;
		}
		else if (range < range2)
		{
			range2 = range;
			spot2 = spot;
		}
	}

	if (!count)
		return NULL;

	if (count <= 2)
	{
		spot1 = spot2 = NULL;
	}
	else
		count -= 2;

	selection = rand() % count;

	spot = NULL;
	do
	{
		spot = G_Find (spot, FOFS(classname), "info_player_deathmatch");
		if (spot == spot1 || spot == spot2)
			selection++;
	} while (selection--);

	return spot;
}


/*
================
SelectFarthestDeathmatchSpawnPoint

================
*/
edict_t *SelectFarthestDeathmatchSpawnPoint (void)
{
	edict_t	*bestspot;
	float	bestdistance, bestplayerdistance;
	edict_t	*spot;

	spot = NULL;
	bestspot = NULL;
	bestdistance = 0;
	while ((spot = G_Find (spot, FOFS(classname), "info_player_deathmatch")) != NULL)
	{
		bestplayerdistance = PlayersRangeFromSpot (spot);

		if (bestplayerdistance > bestdistance)
		{
			bestspot = spot;
			bestdistance = bestplayerdistance;
		}
	}

	if (bestspot)
	{
		return bestspot;
	}

	// if there is a player just spawned on each and every start spot
	// we have no choice to turn one into a telefrag meltdown
	spot = G_Find (NULL, FOFS(classname), "info_player_deathmatch");

	return spot;
}

edict_t *SelectDeathmatchSpawnPoint (void)
{
	if ( (int)(dmflags->value) & DF_SPAWN_FARTHEST)
		return SelectFarthestDeathmatchSpawnPoint ();
	else
		return SelectRandomDeathmatchSpawnPoint ();
}


edict_t *SelectCoopSpawnPoint (edict_t *ent)
{
	int		index;
	edict_t	*spot = NULL;
	char	*target;

	index = ent->client - game.clients;
	if (!index) 	// player 0 starts in normal player spawn point
		return spot;

	// assume there are four coop spots at each spawnpoint
	for (;;)
	{
		spot = G_Find (spot, FOFS(classname), "info_player_coop");
		if (spot == NULL)
			return spot;	// we didn't have enough

		target = spot->targetname;
		if (!target)
			target = "";
		if ( Q_strcasecmp(game.spawnpoint, target) == 0 )
		{	// this is a coop spawn point for one of the clients here
			index--;
			if (!index)
				return spot;		// this is it
		}
	}
}


/*
===========
SelectSpawnPoint

Chooses a player start, deathmatch start, coop start, etc
============
*/
void SelectSpawnPoint (edict_t *ent, vec3_t origin, vec3_t angles)
{
	edict_t	*spot = NULL;

	if (deathmatch->value)
		spot = SelectDeathmatchSpawnPoint ();
	else if (coop->value)
		spot = SelectCoopSpawnPoint (ent);

	// find a single player start spot
	if (!spot)
	{
		while ((spot = G_Find (spot, FOFS(classname), "info_player_start")) != NULL)
		{
			if (!game.spawnpoint[0] && !spot->targetname)
				break;

			if (!game.spawnpoint[0] || !spot->targetname)
				continue;

			if (Q_strcasecmp(game.spawnpoint, spot->targetname) == 0)
				break;
		}

		if (!spot)
		{
			if (!game.spawnpoint[0])
			{	// there wasn't a spawnpoint without a target, so use any
				spot = G_Find (spot, FOFS(classname), "info_player_start");
			}
			if (!spot)
				gi.error ("Couldn't find spawn point %s\n", game.spawnpoint);
		}
	}

	VectorCopy (spot->s.origin, origin);
	origin[2] += 9;
	VectorCopy (spot->s.angles, angles);
}


//======================================================================


void InitBodyQue (void)
{
	int		i;
	edict_t	*ent;

	level.body_que = 0;
	for (i=0; i<BODY_QUEUE_SIZE ; i++)
	{
		ent = G_Spawn();
		ent->classname = "bodyque";
	}
}

void body_die (edict_t *self, edict_t *inflictor, edict_t *attacker, int damage, vec3_t point)
{
	int	n;

	if (self->health < -40)
	{
		gi.sound (self, CHAN_BODY, gi.soundindex ("misc/udeath.wav"), 1, ATTN_NORM, 0);
		for (n= 0; n < 4; n++)
			ThrowGib (self, "models/objects/gibs/sm_meat/tris.md2", damage, GIB_ORGANIC);
		self->s.origin[2] -= 48;
		ThrowClientHead (self, damage);
		self->takedamage = DAMAGE_NO;
	}
}

void CopyToBodyQue (edict_t *ent)
{
	edict_t		*body;

	// grab a body que and cycle to the next one
	body = &g_edicts[(int)maxclients->value + level.body_que + 1];
	level.body_que = (level.body_que + 1) % BODY_QUEUE_SIZE;

	// FIXME: send an effect on the removed body

	gi.unlinkentity (ent);

	gi.unlinkentity (body);
	body->s = ent->s;
	body->s.number = body - g_edicts;

	body->svflags = ent->svflags;
	VectorCopy (ent->mins, body->mins);
	VectorCopy (ent->maxs, body->maxs);
	VectorCopy (ent->absmin, body->absmin);
	VectorCopy (ent->absmax, body->absmax);
	VectorCopy (ent->size, body->size);
	body->solid = ent->solid;
	body->clipmask = ent->clipmask;
	body->owner = ent->owner;
	body->movetype = ent->movetype;

	body->die = body_die;
	body->takedamage = DAMAGE_YES;

	M_FlyCheck (body);

	gi.linkentity (body);
}


void respawn (edict_t *self)
{
	if (deathmatch->value || coop->value)
	{

		self->cloaked = QFALSE;
		self->oldmodel = QFALSE;
		self->client->organicarmor = 0;
		self->client->slugarmor = 0;
		self->client->cellarmor = 0;
		self->client->loxarmor = 0;

		// spectator's don't leave bodies
		if (self->movetype != MOVETYPE_NOCLIP)
			CopyToBodyQue (self);

		// Put them back into the game.
		PutClientInServer (self);

		// add a teleportation effect
		self->s.event = EV_PLAYER_TELEPORT;

		// hold in place briefly
		self->client->ps.pmove.pm_flags = PMF_TIME_TELEPORT;
		self->client->ps.pmove.pm_time = 14;

		self->client->respawn_time = level.time;

		if (strlen(self->client->pers.onrespawn) != 0)
			stuffcmd(self,self->client->pers.onrespawn);
	}
	else
	{
		// single-player game. restart the entire server
		gi.AddCommandString ("menu_loadgame\n");
	}
}

//==============================================================


/*
===========
PutClientInServer

Called when a player connects to a server or respawns in
a deathmatch.
============
*/

void PutClientInServer (edict_t *ent)
{
	vec3_t	mins = {-16, -16, -24};
	vec3_t	maxs = {16, 16, 32};
	int		index;
	vec3_t	spawn_origin, spawn_angles;
	gclient_t	*client;
	int		i;
	client_persistent_t	saved;
	client_respawn_t	resp;

	qboolean spawnlive = QTRUE;

	// find a spawn point
	// do it before setting health back up, so farthest
	// ranging doesn't count this client
	if (ctf->value)
	{
		edict_t *spot;

		spot = SelectCTFSpawnPoint (ent, &spawnlive);
		VectorCopy (spot->s.origin, spawn_origin);
		spawn_origin[2] += 9;
		VectorCopy (spot->s.angles, spawn_angles);
	}
	else
		SelectSpawnPoint (ent, spawn_origin, spawn_angles);

	index = ent-g_edicts - 1;
	client = ent->client;

	// deathmatch wipes most client data every spawn
	if (deathmatch->value)
	{
		char		userinfo[MAX_INFO_STRING];

		resp = client->resp;
		memcpy (userinfo, client->pers.userinfo, sizeof(userinfo));
		InitClientPersistant (client);
		ClientUserinfoChanged (ent, userinfo);
	}
	else if (coop->value)
	{
		int			n;
		char		userinfo[MAX_INFO_STRING];

		resp = client->resp;
		memcpy (userinfo, client->pers.userinfo, sizeof(userinfo));
		// this is kind of ugly, but it's how we want to handle keys in coop
		for (n = 0; n <= game.num_items; n++)
		{
			if (itemlist[n]->flags & IT_KEY)
				resp.coop_respawn.inventory[n] = client->pers.inventory[n];
		}
		client->pers = resp.coop_respawn;
		ClientUserinfoChanged (ent, userinfo);
		if (resp.score > client->pers.score)
			client->pers.score = resp.score;
	}
	else
	{
		memset (&resp, 0, sizeof(resp));
	}

	// clear everything but the persistant data
	saved = client->pers;
	memset (saved.zbotBuf, 0, sizeof(saved.zbotBuf));
	memset (client, 0, sizeof(*client));
	client->pers = saved;
	if (client->pers.health <= 0)
		InitClientPersistant(client);
	client->resp = resp;

	// copy some data from the client to the entity
	FetchClientEntData (ent);

	// clear entity values
	ent->groundentity = NULL;
	ent->client = &game.clients[index];
	ent->takedamage = DAMAGE_AIM;
	ent->movetype = MOVETYPE_WALK;
	ent->viewheight = 22;
	ent->inuse = QTRUE;
	ent->classname = "player";
	ent->classnum = CN_PLAYER;
	ent->mass = 200;
	ent->solid = SOLID_BBOX;
	ent->deadflag = DEAD_NO;
	ent->air_finished = level.time + 12;
	ent->clipmask = MASK_PLAYERSOLID;
	ent->model = "players/male/tris.md2";
	ent->pain = player_pain;
	ent->die = player_die;
	ent->waterlevel = 0;
	ent->watertype = 0;
	ent->flags &= ~FL_NO_KNOCKBACK;
	ent->svflags &= ~(SVF_DEADMONSTER|SVF_NOCLIENT);
	// *********************
	// start unfrozen
	ent->frozen = 0;
	ent->unfrozen = 1;
	ent->frozentime = level.time;
	// *********************

	// start invulnerable for variable seconds in DM, but not teamplay.
	if (deathmatch->value && !ctf->value)
	{
		int spawntime;
		spawntime = (int) (respawn_protect->value * 10);
		ent->client->invincible_framenum = level.framenum + spawntime;
	}

	// *********************

	VectorCopy (mins, ent->mins);
	VectorCopy (maxs, ent->maxs);
	VectorClear (ent->velocity);

	// clear playerstate values
	memset (&ent->client->ps, 0, sizeof(client->ps));

	client->ps.pmove.origin[0] = spawn_origin[0]*8;
	client->ps.pmove.origin[1] = spawn_origin[1]*8;
	client->ps.pmove.origin[2] = spawn_origin[2]*8;
	//ZOID
	client->ps.pmove.pm_flags &= ~PMF_NO_PREDICTION;
	//ZOID

	// If this is an "infinite ammo" game, just give it all to them now.
	if (deathmatch->value
		&& ((int)(dmflags->value) & DF_INFINITE_AMMO))
	{
		Add_Ammo (ent, &gI_ammo_shells, 1000);
		Add_Ammo (ent, &gI_ammo_bullets, 1000);
		Add_Ammo (ent, &gI_ammo_grenades, 1000);
		Add_Ammo (ent, &gI_ammo_rockets, 1000);
		Add_Ammo (ent, &gI_ammo_cells, 1000);
		Add_Ammo (ent, &gI_ammo_slugs, 1000);
	}

	if (deathmatch->value && ((int)dmflags->value & DF_FIXED_FOV))
	{
		client->ps.fov = 90;
	}
	else
	{
		client->ps.fov = atoi(Info_ValueForKey(client->pers.userinfo, "fov"));
		if (client->ps.fov < 1)
			client->ps.fov = 90;
		else if (client->ps.fov > 160)
			client->ps.fov = 160;
	}

	client->ps.gunindex = gi.modelindex(client->pers.weapon->view_model);

	// clear entity state values
	ent->s.effects = EF_NONE;
	ent->s.skinnum = ent - g_edicts - 1;
	ent->s.modelindex = 255;		// will use the skin specified model

	ShowGun(ent);					// ### Hentai ### special gun model

	ent->s.frame = 0;
	VectorCopy (spawn_origin, ent->s.origin);
	ent->s.origin[2] += 1;	// make sure off ground
	VectorCopy (ent->s.origin, ent->s.old_origin);

	// set the delta angle
	for (i=0 ; i<3 ; i++)
		client->ps.pmove.delta_angles[i] = ANGLE2SHORT(spawn_angles[i] - client->resp.cmd_angles[i]);

	ent->s.angles[PITCH] = 0;
	ent->s.angles[YAW] = spawn_angles[YAW];
	ent->s.angles[ROLL] = 0;
	VectorCopy (ent->s.angles, client->ps.viewangles);
	VectorCopy (ent->s.angles, client->v_angle);
	// JDB: init variable for lowlight and reset gl_saturatelighting 4/4/98 
	ent->client->lowlight = 0; 
	//gi.cvar_forceset("gl_saturatelighting","0"); 
	//gi.cvar_forceset("r_fullbright","0"); 

	ent->thirdoffx = 32; //Sets up the default reletive camera position
	ent->thirdoffz = 40;
	ent->thirdperson = QFALSE;

	// darKMajick:
	client->dM_grenade = 0;
	client->dM_ammoCost = 1;
	// DM end

	//ZOID
	if (ctf->value && CTFStartClient(ent))
		return;
	//ZOID

	if (!KillBox (ent))
	{	// could't spawn in?
	}

	gi.linkentity (ent);

	// force the current weapon up
	client->newweapon = client->pers.weapon;
	ChangeWeapon (ent);

	// If the entity couldn't be spawned live, make them a ghost.
	if (!spawnlive)
		TeamplayMakeObserver (ent);
	ent->client->scannerx = 80;
	ent->client->scannery = 40;
}

/*=====================
ClientBeginDeathmatch

A client has just connected to the server in deathmatch
mode, so clear everything out before starting them.
=====================*/
void ClientBeginDeathmatch (edict_t *ent)
{ 
	static qboolean Ghost_Spawned = QFALSE;

	// Create Ghost on first Player connect.
	if (highfragger->value)
	{
		if ((sv_bestplayer->value == 1.0) && !Ghost_Spawned)
		{
			Create_Ghost();	
			Ghost_Spawned = QTRUE;
		} 
	}

	G_InitEdict (ent);
	InitClientResp (ent->client);

	// locate ent at a spawn point
	PutClientInServer (ent);

	if (level.intermissiontime)
		MoveClientToIntermission (ent);
	else
	{
		// send effect
		gi.WriteByte (svc_muzzleflash);
		gi.WriteShort (ent-g_edicts);
		gi.WriteByte (MZ_LOGIN);
		gi.multicast (ent->s.origin, MULTICAST_PVS);
		gi.bprintf (PRINT_HIGH, "%s entered the game\n", ent->client->pers.netname);
	}
	
	ClientPrintMOTD(ent);

	// make sure all view stuff is valid
	ClientEndServerFrame (ent);
	return;
}

// Store the message of the day in memory.
char *gMOTD = ((char *)-1); // initialized at startup as bad pointer
cvar_t *motdfile;

void ClientPrintMOTD (edict_t *ent)
{
	FILE *in;
	char motdPath[MAX_QPATH + 1];
	int c;
	int motdBytes;
	char *here;

	// If the MOTD hasn't been loaded, do so.
	if (gMOTD == ((char *)-1))
	{

		// Generate the path to the MOTD file.
		if (gamedir == NULL || motdfile == NULL
			|| !gamedir->string[0] || !motdfile->string[0])
		{
			gMOTD = NULL; // null pointer means we'll never try again
			return;
		}

		sprintf (motdPath, "./%s/%s", gamedir->string, motdfile->string);

		// Open the file.
		in = fopen (motdPath, "rt");
		if (in == NULL)
		{
			gi.dprintf("Unable to open MOTD file.\n");
			gMOTD = NULL;
			return;
		}

		// Count the number of bytes in the file.
		motdBytes = 0;
		while ((c = fgetc (in)), c != EOF)
			motdBytes++;

		// Make space for that many bytes.
		gMOTD = gi.TagMalloc (motdBytes + 1, TAG_GAME);
		gi.dprintf("Allocating %i bytes for MOTD\n", motdBytes +1);

		// Now read the MOTD in for real.  Null-terminate the string.
		fclose (in);
		in = fopen (motdPath, "rt");
		here = gMOTD; //extra pointer for writing into gMOTD
		while ((c = fgetc (in)), c != EOF)
		{
			*here = c;
			here++;
			motdBytes--;
		}
		*here = '\0';

		// If anything went wrong, warn the console.
		if (motdBytes != 0)
			gi.dprintf ("MOTD error: off by %d bytes", motdBytes);

	}
	if (gMOTD != NULL) 	// If a MOTD was successfully loaded, print it.
	{
		gi.centerprintf (ent, "%s", gMOTD);
	}
	return;
}



/*
===========
ClientBegin

called when a client has finished connecting, and is ready
to be placed into the game.  This will happen every level load.
============
*/
void ClientBegin (edict_t *ent)
{
	int		i;

	ent->client = game.clients + (ent - g_edicts - 1);

	if (deathmatch->value)
	{
		ClientBeginDeathmatch (ent);
		return;
	}

	//initialize the lbind strings to zero on new client connect
	memset (ent->client->pers.ondeath, '\0', MAX_INFO_STRING);
	memset (ent->client->pers.onrespawn, '\0', MAX_INFO_STRING);
	memset (ent->client->pers.onenemydeath, '\0', MAX_INFO_STRING);
	//	*ent->client->pers.ondeath = '\0';
	//	*ent->client->pers.onrespawn = '\0';
	//	*ent->client->pers.onenemydeath = '\0';


	// if there is already a body waiting for us (a loadgame), just
	// take it, otherwise spawn one from scratch
	if (ent->inuse)
	{
		// the client has cleared the client side viewangles upon
		// connecting to the server, which is different than the
		// state when the game is saved, so we need to compensate
		// with deltaangles
		for (i = 0; i < 3; i++)
			ent->client->ps.pmove.delta_angles[i] = ANGLE2SHORT(ent->client->ps.viewangles[i]);
	}
	else
	{
		// a spawn point will completely reinitialize the entity
		// except for the persistant data that was initialized at
		// ClientConnect() time
		G_InitEdict (ent);
		ent->classname = "player";
		ent->classnum = CN_PLAYER;
		InitClientResp (ent->client);
		PutClientInServer (ent);
	}

	if (level.intermissiontime)
	{
		MoveClientToIntermission (ent);
	}
	else
	{
		// send effect if in a multiplayer game
		if (game.maxclients > 1)
		{
			gi.WriteByte (svc_muzzleflash);
			gi.WriteShort (ent-g_edicts);
			gi.WriteByte (MZ_LOGIN);
			gi.multicast (ent->s.origin, MULTICAST_PVS);

			gi.bprintf (PRINT_HIGH, "%s entered the game.\n", ent->client->pers.netname);
		}
	}

	// make sure all view stuff is valid
	ClientEndServerFrame (ent);
}

/*
===========
ClientUserInfoChanged

called whenever the player updates a userinfo variable.

The game can override any of the settings in place
(forcing skins or names, etc) before copying it off.
============
*/
void ClientUserinfoChanged (edict_t *ent, char *userinfo)
{
	char	*s;
	int		playernum;

	// check for malformed or illegal info strings
	if (!Info_Validate(userinfo))
	{
		strncpy (userinfo, "\\name\\badinfo\\skin\\male/grunt", sizeof(userinfo)-1);
	}

	// set name
	s = Info_ValueForKey (userinfo, "name");
	strncpy (ent->client->pers.netname, s, sizeof(ent->client->pers.netname)-1);

	// set skin
	s = Info_ValueForKey (userinfo, "skin");

	playernum = ent-g_edicts-1;

	// combine name and skin into a configstring
	if (ctf->value)
		CTFAssignSkin(ent, s);
	else
		gi.configstring (CS_PLAYERSKINS + playernum, va("%s\\%s", ent->client->pers.netname, s) );

	// fov
	if (deathmatch->value && ((int)dmflags->value & DF_FIXED_FOV))
	{
		ent->client->ps.fov = 90;
	}
	else
	{
		ent->client->ps.fov = atoi(Info_ValueForKey(userinfo, "fov"));
		if (ent->client->ps.fov < 1)
			ent->client->ps.fov = 90;
		else if (ent->client->ps.fov > 160)
			ent->client->ps.fov = 160;
	}

	// handedness
	s = Info_ValueForKey (userinfo, "hand");
	if (strlen(s))
	{
		ent->client->pers.hand = atoi(s);
	}
	if (ent->thirdperson)
	{
		ent->clone->s.modelindex = ent->s.modelindex;
		ent->clone->s.skinnum = ent->s.skinnum;
	}


	// save off the userinfo in case we want to check something later
	strncpy (ent->client->pers.userinfo, userinfo, sizeof(ent->client->pers.userinfo)-1);

	// ### Hentai ### BEGIN
	if (!ent->deadflag && ent->s.modelindex == 255)
		ShowGun(ent);
	// ### Hentai ### END

	//	ARLog_Stats("UI: %s\n",ent->client->pers.userinfo); //QW// ARSG stats logging (spams the log)
}


/*
===========
ClientConnect

Called when a player begins connecting to the server.
The game can refuse entrance to a client by returning QFALSE.
If the client is allowed, the connection process will continue
and eventually get to ClientBegin()
Changing levels will NOT cause this to be called again, but
loadgames will.
============
*/
qboolean ClientConnect (edict_t *ent, char *userinfo)
{
	char	*value, *ip;
	qboolean gotAddr;
	ipfilter_t ipAddr;

	// check to see if they are on the banned IP list
	ip = Info_ValueForKey (userinfo, "ip");
	if (SV_FilterPacket(ip))
	{
		Info_SetValueForKey (userinfo, "rejmsg", "Banned.");
		return QFALSE;
	}

	// parse the ip address
	gotAddr = StringToFilter (ip, &ipAddr);

	// check for a password
	value = Info_ValueForKey (userinfo, "password");
	if (strcmp (password->string, value) != 0)
		return QFALSE;

	// they can connect
	ent->client = game.clients + (ent - g_edicts - 1);

	// store their IP address (now, since it seems to change)
	if (gotAddr)
		ent->client->pers.ipAddr = ipAddr.compare;

	// if there is already a body waiting for us (a loadgame), just
	// take it, otherwise spawn one from scratch
	if (!ent->inuse)
	{
		//ZOID -- force team join
		if (ctf->value)
			ent->client->resp.ctf_team = -1;
		//ZOID
		// clear the respawning variables
		InitClientResp (ent->client);
		if (!game.autosaved || !ent->client->pers.weapon)
			InitClientPersistant (ent->client);
	}

	ClientUserinfoChanged (ent, userinfo);

	if (game.maxclients > 1)
	{
		ip = Info_ValueForKey(ent->client->pers.userinfo, "ip");
		gi.dprintf ("%s connected from %s ", ent->client->pers.netname, ip);
		Log_Time(); // time string has \n built in so we don't need one here
	}

	ent->client->pers.connected = QTRUE;
	ARLog_Stats("IN: %s\n",ent->client->pers.netname);
	ARLog_Stats("UI: %s\n",ent->client->pers.userinfo); //QW// ARSG stats logging
	return QTRUE;
}

/*
===========
ClientDisconnect

Called when a player drops from the server.
Will not be called between levels.
============
*/
void ClientDisconnect (edict_t *ent)
{
	int		playernum;

	if (!ent->client || !ent->inuse)
		return;

	if (ent->lasersight != NULL)
		SP_LaserSight (ent);

	gi.bprintf (PRINT_HIGH, "%s disconnected\n", ent->client->pers.netname);
	ARLog_Stats("OUT: %s\n",ent->client->pers.netname);

	// send effect
	gi.WriteByte (svc_muzzleflash);
	gi.WriteShort (ent-g_edicts);
	gi.WriteByte (MZ_LOGOUT);
	gi.multicast (ent->s.origin, MULTICAST_PVS);

	memset (ent->client->pers.ondeath, '\0', MAX_INFO_STRING);
	memset (ent->client->pers.onrespawn, '\0', MAX_INFO_STRING);
	memset (ent->client->pers.onenemydeath, '\0', MAX_INFO_STRING);

	gi.unlinkentity (ent);
	ent->s.modelindex = 0;
	ent->solid = SOLID_NOT;
	ent->inuse = QFALSE;
	ent->classname = "disconnected";
	ent->client->pers.connected = QFALSE;
	ent->client->angel = NULL;
	ent->client->pers.special = 0;
	playernum = ent-g_edicts-1;
	gi.configstring (CS_PLAYERSKINS + playernum, "");
	G_FreeEdict(ent);
}


//==============================================================


edict_t	*pm_passent;

// pmove doesn't need to know about passent and contentmask
trace_t	PM_trace (vec3_t start, vec3_t mins, vec3_t maxs, vec3_t end)
{
	if (pm_passent->health > 0)
		return gi.trace (start, mins, maxs, end, pm_passent, MASK_PLAYERSOLID);
	else
		return gi.trace (start, mins, maxs, end, pm_passent, MASK_DEADSOLID);
}

unsigned CheckBlock (void *b, int c)
{
	int	v,i;
	v = 0;
	for (i=0 ; i<c ; i++)
		v+= ((byte *)b)[i];
	return v;
}
void PrintPmove (pmove_t *pm)
{
	unsigned	c1, c2;

	c1 = CheckBlock (&pm->s, sizeof(pm->s));
	c2 = CheckBlock (&pm->cmd, sizeof(pm->cmd));
	Com_Printf ("sv %3i:%i %i\n", pm->cmd.impulse, c1, c2);
}

// MEFIXED: Muhahaha. Came up with a fix!
qboolean FindSmallPoint (edict_t *ent)
{
	vec3_t loc = {0,0,0};
	vec3_t floor;
	int i;
	int j = 0;
	int k = 0;
	trace_t tr;

	do {
		j++;
		for (i = 0; i < 3; i++)
			loc[i] = rand() % (400 + 1) - 200;

		if (gi.pointcontents(loc) == 0)
		{
			VectorCopy(loc, floor);
			floor[2] = -200;
			tr = gi.trace (loc, vec3_origin, vec3_origin, floor, NULL, MASK_SOLID);
			k++;
			if (tr.contents & MASK_WATER)
				continue; 
			VectorCopy (tr.endpos, loc);
			loc[2] += ent->maxs[2] - ent->mins[2]; // make sure the entity can fit!
		}
	} while (gi.pointcontents(loc) > 0 && j < 1000 && k < 500);

	// STILL not good?
	if (j >= 1000 || k >= 500)
	{
		gi.dprintf ("%s couldn't find a spawnpoint: FindSmallPoint\n", ent->client->pers.netname);
		return QFALSE;
	}

	loc[2] += 82;
	VectorCopy(loc,ent->s.origin);
	VectorCopy(loc,ent->s.old_origin);
	return QTRUE;
}

// Paril, finds a random place on the map to put something.
// FIXME: On small maps, this does not work. There's got to be a better way
qboolean findspawnpoint (edict_t *ent)
{
	vec3_t loc = {0,0,0};
	vec3_t floor;
	int i;
	int j = 0;
	int k = 0;
	trace_t tr;

	do {
		j++;
		for (i = 0; i < 3; i++)
			loc[i] = rand() % (8192 + 1) - 4096;

		if (gi.pointcontents(loc) == 0)
		{
			VectorCopy(loc, floor);
			floor[2] = -4096;
			tr = gi.trace (loc, vec3_origin, vec3_origin, floor, NULL, MASK_SOLID);
			k++;
			if (tr.contents & MASK_WATER)
				continue; 
			VectorCopy (tr.endpos, loc);
			loc[2] += ent->maxs[2] - ent->mins[2]; // make sure the entity can fit!
		}
	} while (gi.pointcontents(loc) > 0 && j < 1000 && k < 500);

	// Paril: Fixed where small maps are not working with this code!
	if (j >= 1000 || k >= 500)
	{
		FindSmallPoint (ent);
		return QFALSE;
	}

	loc[2] += 82;
	VectorCopy(loc,ent->s.origin);
	VectorCopy(loc,ent->s.old_origin);
	return QTRUE;
}

// Paril: Is the player STILL stuck in a solid?
// Note: This code will only see if the player's origin is
// in a solid. If it isn't, we assume he can still hook/crouch/jump out of the spot.
void AreWeStillInSolidcommaspaceIfYesRespawnUs (edict_t *ent)
{
	if (gi.pointcontents(ent->s.origin) & CONTENTS_SOLID)
	{
		vec3_t old_origin;

		VectorCopy (ent->s.origin, old_origin);
		// Crap, we're still in a solid.
		// Let's get him out of there.
		// Note: If the player keeps getting stuck in solid, we
		// should find out how he gets in there.
		gi.cprintf (ent, PRINT_HIGH, "There you go, now stay out of that spot.\n");
		findspawnpoint(ent);
		gi.dprintf ("Player %s was stuck in solid and was teleported out from %s to %s\n", ent->client->pers.netname, vtos(old_origin), vtos(ent->s.origin));
	}
	ent->think4 = NULL;
	ent->nextthink4 = -1;
}

/*
==============
ClientThink

This will be called once for each client frame, which will
usually be a couple times for each server frame.
==============
*/
void ClientThink (edict_t *ent, usercmd_t *ucmd)
{
	gclient_t	*client;
	edict_t	*other;
	int		i, j;
	pmove_t	pm;

	//	START_PERFORMANCE_TIMER;

	// If they're frozen and its time to thaw, do it. 
	if (ent->frozen)
	{
		if (level.time >= ent->frozentime || ent->deadflag ) //unfreeze immediately if dead
			ent->frozen = 0;
		else if (ent->unfrozen == 0 && ent->client->pers.special == AOUNFREEZE)
		{
			ent->frozentime = level.time;
			ent->unfrozen = 1;
		}
	}

	level.current_entity = ent;
	client = ent->client;

	if (level.intermissiontime)
	{
		client->ps.pmove.pm_type = PM_FREEZE;
		// can exit intermission after five seconds
		if (level.time > level.intermissiontime + 5.0 
			&& (ucmd->buttons & BUTTON_ANY) )
			level.exitintermission = QTRUE;
		return;
	}

	pm_passent = ent;

	//ZOID
	if (ent->client->chase_target)
	{
		client->resp.cmd_angles[0] = SHORT2ANGLE(ucmd->angles[0]);
		client->resp.cmd_angles[1] = SHORT2ANGLE(ucmd->angles[1]);
		client->resp.cmd_angles[2] = SHORT2ANGLE(ucmd->angles[2]);
		return;
	}
	//ZOID

	// set up for pmove
	memset (&pm, 0, sizeof(pm));
	if (ent->client->organicarmor == 1)
	{
		int index;
		index = ArmorIndex (ent);
		if (index != 0)
		{
			if (ent->client->pers.inventory[index] < 100)
				ent->client->pers.inventory[index]+=1;
			else if (ent->client->pers.inventory[index] < 200)
				ent->client->pers.inventory[index]+=2;
			else if (ent->client->pers.inventory[index] < 300)
				ent->client->pers.inventory[index]+=3;
			else if (ent->client->pers.inventory[index] < 400)
				ent->client->pers.inventory[index]+=4;
			else if (ent->client->pers.inventory[index] < 500)
				ent->client->pers.inventory[index]+=5;
			if (ent->client->pers.inventory[index] > 500)
				ent->client->pers.inventory[index] = 500;
		}
	}

	if (ent->client->regenhealth == 1)
	{
		if (ent->health <= 0) 
			ent->client->regenhealth = 0;
		else if (ent->health < 100) ent->health+=1;
		else if (ent->health < 200) ent->health+=2;
		else if (ent->health < 300) ent->health+=3;
		else if (ent->health < 400) ent->health+=4;
		if (ent->health > 400) ent->health = 400;

	}

	if (ent->movetype == MOVETYPE_NOCLIP)
		client->ps.pmove.pm_type = PM_SPECTATOR;
	else if (ent->s.modelindex != 255)
		client->ps.pmove.pm_type = PM_GIB;
	else if (ent->deadflag || ent->client->feign)
		client->ps.pmove.pm_type = PM_DEAD;
	else if (ent->frozen)
		client->ps.pmove.pm_type = PM_DEAD;
	else
		client->ps.pmove.pm_type = PM_NORMAL;

	client->ps.pmove.gravity = sv_gravity->value;
	if ( Jet_Active(ent) )
		Jet_ApplyJet( ent, ucmd );
	pm.s = client->ps.pmove;

	for (i=0 ; i<3 ; i++)
	{
		pm.s.origin[i] = ent->s.origin[i]*8;
		pm.s.velocity[i] = ent->velocity[i]*8;
	}

	if (memcmp(&client->old_pmove, &pm.s, sizeof(pm.s)))
	{
		pm.snapinitial = QTRUE;
		//		gi.dprintf ("pmove changed!\n");
	}

	// If they're frozen, alter some of the command.
	if (ent->frozen)
	{
		// No buttons pushed.
		ucmd->buttons = 0;

		// No movement.
		ucmd->forwardmove = ucmd->sidemove = ucmd->upmove = 0;

		// No turns.
		ucmd->angles[0] = ANGLE2SHORT(client->resp.cmd_angles[0]);
		ucmd->angles[1] = ANGLE2SHORT(client->resp.cmd_angles[1]);
		ucmd->angles[2] = ANGLE2SHORT(client->resp.cmd_angles[2]);
		client->ps.pmove.delta_angles[0] = 0;
		client->ps.pmove.delta_angles[1] = 0;
		client->ps.pmove.delta_angles[2] = 0;
	}

	pm.cmd = *ucmd;

	pm.trace = PM_trace;	// adds default parms
	pm.pointcontents = gi.pointcontents;

	// perform a pmove
	gi.Pmove (&pm);

	// save results of pmove
	client->ps.pmove = pm.s;
	client->old_pmove = pm.s;

	for (i=0 ; i<3 ; i++)
	{
		ent->s.origin[i] = pm.s.origin[i]*0.125f;
		if ( !Jet_Active(ent)
			|| (Jet_Active(ent)&&(fabs((float)pm.s.velocity[i]*0.125) < fabs(ent->velocity[i]))) )
			ent->velocity[i] = pm.s.velocity[i]*0.125f;
	}

	VectorCopy (pm.mins, ent->mins);
	VectorCopy (pm.maxs, ent->maxs);

	client->resp.cmd_angles[0] = SHORT2ANGLE(ucmd->angles[0]);
	client->resp.cmd_angles[1] = SHORT2ANGLE(ucmd->angles[1]);
	client->resp.cmd_angles[2] = SHORT2ANGLE(ucmd->angles[2]);

	// if jet and on ground, lift us if possible
	if ( Jet_Active(ent) && pm.groundentity && Jet_AvoidGround(ent))
		/* now we are no longer on ground */
		pm.groundentity = NULL;

	if (ent->groundentity && !pm.groundentity && (pm.cmd.upmove >= 10) && (pm.waterlevel == 0))
	{
		gi.sound(ent, CHAN_VOICE, gi.soundindex("*jump1.wav"), 1, ATTN_NORM, 0);
		PlayerNoise(ent, ent->s.origin, PNOISE_SELF);
	}

	ent->viewheight = pm.viewheight;
	ent->waterlevel = pm.waterlevel;
	ent->watertype = pm.watertype;
	ent->groundentity = pm.groundentity;
	if (pm.groundentity)
		ent->groundentity_linkcount = pm.groundentity->linkcount;

	if (ent->deadflag && (!ent->client->feign))
	{
		client->ps.viewangles[ROLL] = 40;
		client->ps.viewangles[PITCH] = -15;
		client->ps.viewangles[YAW] = client->killer_yaw;
	}
	else
	{
		VectorCopy (pm.viewangles, client->v_angle);
		VectorCopy (pm.viewangles, client->ps.viewangles);
	}

	// If they're frozen, alter some of the results.
	if (ent->frozen)
	{
		// Restore their mins/maxs/viewheight, since PM_DEAD did something else
		// to them.
		if (client->ps.pmove.pm_flags & PMF_DUCKED)
		{
			VectorSet (ent->mins, -16, -16, -24);
			VectorSet (ent->maxs, 16, 16, 4);
			ent->viewheight = -2;
		}
		else
		{
			VectorSet (ent->mins, -16, -16, -24);
			VectorSet (ent->maxs, 16, 16, 32);
			ent->viewheight = 22;
		}
	}

	gi.linkentity (ent);

	if (ent->movetype != MOVETYPE_NOCLIP)
		G_TouchTriggers (ent);

	// touch other objects
	for (i = 0; i < pm.numtouch; i++)
	{
		other = pm.touchents[i];
		for (j = 0; j < i; j++)
			if (pm.touchents[j] == other)
				break;
		if (j != i)
			continue;	// duplicated

		// Take care of unfreezing.
		if (other->frozen && other->client)
		{
			if (OnSameTeam (ent, other))	// only touch-unfreeze if we have the same skin
			{
				other->frozen = 0;
			}
		}

		if (!other->touch)
			continue;
		other->touch (other, ent, NULL, NULL);
	}

	client->oldbuttons = client->buttons;
	client->buttons = ucmd->buttons;
	client->latched_buttons |= client->buttons & ~client->oldbuttons;

	// save light level the player is standing on for
	// monster sighting AI
	ent->light_level = ucmd->lightlevel;

	// fire weapon from final position if needed
	if (client->latched_buttons & BUTTON_ATTACK
		//ZOID
		&& ent->movetype != MOVETYPE_NOCLIP
		//ZOID
		&& !ent->frozen)
	{
		if (!client->weapon_thunk)
		{
			client->weapon_thunk = QTRUE;
			Think_Weapon (ent);
		}
	}

	// CCH: Check to see if an airstrike has arrived
	if ( client->airstrike_called && level.time > client->airstrike_time )
	{
		client->airstrike_called = 0; 
		Think_Airstrike (ent);       
	}

	/* WonderSlug ----Code to go boom */ 
	if ((ent->client->kamikaze_framenum <= level.framenum)
		&& (ent->client->kamikaze_mode)) 
		Kamikaze_Explode(ent);
	/* WonderSlug End */

	//ZOID
	for (i = 1; i <= maxclients->value; i++)
	{
		other = g_edicts + i;
		if (other->inuse && other->client->chase_target == ent)
			UpdateChaseCam(other);
	}
	//ZOID

	// Detect ZBot, blab it to everyone, then kick the bastard.  If they keep
	// trying to log in, they'll keep getting outed and kicked.  Ha!
	if (ZbotCheck (ent, ucmd))
	{
		// Tell everyone.
		gi.bprintf (PRINT_CHAT, ">>> LOX detected a Zbot: %s <<<\n",
			ent->client->pers.netname);

		// Now kick the bastard.
		gi.AddCommandString (va ("kick %d\n", ent - g_edicts - 1));
		stuffcmd (ent, "disconnect\n");
	}

	// Onoz we are in the solidity
	//	if (ent->movetype != MOVETYPE_NOCLIP && ent->think4 == NULL && gi.pointcontents(ent->s.origin) & CONTENTS_SOLID)
	//	{
	//		gi.cprintf (ent, PRINT_HIGH, "You look like you're stuck.\n");
	//		ent->think4 = AreWeStillInSolidcommaspaceIfYesRespawnUs;
	//		ent->nextthink4 = level.time + 2;
	//	}

	if (anticamper->value && ent->health > 0)
	{
		if (campertime->value < 5) gi.cvar_set("campertime", "5");
		if (campertime->value > 600) gi.cvar_set("campertime", "600");

		if (VectorCompare(ent->s.origin, ent->client->lastposition) == 0)
		{
			VectorCopy(ent->s.origin, ent->client->lastposition);
			ent->client->lastpositiontime = level.time;
			ent->client->warningtime = 0;
		}

		if (ent->client->lastpositiontime + campertime->value == level.time && ent->client->warningtime == 0)
			gi.cprintf(ent, PRINT_HIGH, "This is your only camping alert. You have 6 seconds to move.\n");

		if (ent->client->lastpositiontime + campertime->value + (float)ent->client->warningtime == level.time && ent->client->warningtime <= 5)
			gi.cprintf(ent, PRINT_HIGH, "%d seconds left ....\n",5-ent->client->warningtime++);

		if (ent->client->warningtime == 6)
		{
			gi.cprintf(ent, PRINT_HIGH, "Don't say I didn't warn you!!!!\n");
			ent->client->warningtime = 0;
			Cmd_Kill_f(ent);
		}
	}

	if ((ent->plaguetime > level.time) && (ent->plaguedamage == 1))
	{
		if (ent->plaguedamagecount == -1)
		{
			ent->plaguedamagecount = (int)(random()*15.0);
			if (ent->plaguedamagecount < 0 )
				ent->plaguedamagecount*=-1;
		}
		else
		{
			vec3_t dir;
			VectorSet (dir, 0, 0, 1);
			if (ent->client != NULL)
				T_Damage (ent, ent, ent->damagecauser, dir, ent->s.origin, vec3_origin, ent->plaguedamagecount, 0, 0, MOD_PLAGUE);
			ent->plaguedamagecount = (int)(.90*(float)ent->plaguedamagecount);
		}
	}

	else if ((ent->plaguetime <= level.time) && (ent->plaguedamage == 1))
	{
		ent->plaguedamage = 0;
		ent->plaguetime = -1;
		ent->plaguedamagecount = -1;
	}


	if (weaponheat->value)
	{
		if (ent->client->curr_heat > 130)
			Kamikaze_Explode(ent);

		if (ent->client->curr_heat > 0)
			ent->client->curr_heat -= 0.5;
	}
	else
		ent->client->curr_heat = 0;

	//_STOP_PERFORMANCE_TIMER(__func__);
}


//
// ==============
// ClientBeginServerFrame
//
//  This will be called once for each server frame, before running
//  any other entities in the world.
//  ==============
//
void ClientBeginServerFrame (edict_t *ent)
{
	gclient_t	*client;
	int		buttonMask;

	if (level.intermissiontime)
		return;

	client = ent->client;

	// run weapon animations if it hasn't been done by a ucmd_t
	if (!client->weapon_thunk
		//ZOID
		&& ent->movetype != MOVETYPE_NOCLIP
		//ZOID
		&& !ent->frozen)
		Think_Weapon (ent);
	else
		client->weapon_thunk = QFALSE;

	if (ent->deadflag)
	{
		// wait for any button just going down
		if ( level.time > client->respawn_time)
		{
			// in deathmatch, only wait for attack button
			if (deathmatch->value)
				buttonMask = BUTTON_ATTACK;
			else
				buttonMask = -1;

			if (( client->latched_buttons & buttonMask ) ||
				(deathmatch->value && ((int)dmflags->value & DF_FORCE_RESPAWN)))
			{
				respawn(ent);
				client->latched_buttons = 0;
			}
		}
		return;
	}

	// add player trail so monsters can follow
	if (!deathmatch->value)
		if (!visible (ent, PlayerTrail_LastSpot()))
			PlayerTrail_Add (ent->s.old_origin);
	client->latched_buttons = 0;
}

//=====================================================
//=========== FLAGGING BEST PLAYER ROUTINES ===========
//===================================================== 

//=========================================================
// Returns Player with Highest Score.
//=========================================================
edict_t *BestScoreEnt(void) 
{
	edict_t *bestplayer=NULL;
	int i, bestscore = -999;
	edict_t *ent; 

	// Search thru all clients
	for (i = 0; i < game.maxclients; i++)
	{
		ent = g_edicts + i + 1;
		if (!G_EntExists(ent)) 
			continue;
		if (ent->client->resp.score > bestscore) 
		{
			bestplayer = ent; // Found one!
			bestscore = ent->client->resp.score; 
		}
	}	
	return bestplayer;
} 

//======================================================
// Ghost think routine. 
//======================================================
void Ghost_Think(edict_t *Ghost) 
{
	vec3_t start = {0,0,0};
	vec3_t up;
	edict_t *fPlayer = NULL; 

	// Unlink Ghost if intermission..
	if (level.intermissiontime >= level.time) 
	{
		gi.unlinkentity(Ghost);
		return; 
	} 

	// Check Scoreboard every 1 sec.
	if (Ghost->delay <= level.time) 
	{
		fPlayer = BestScoreEnt();
		Ghost->delay = level.time + 0.1f; 
	} 

	// Move Ghost above fPlayer.
	if ((fPlayer!=NULL) && G_ClientInGame(fPlayer)) 
	{
		Ghost->svflags &= ~SVF_NOCLIENT; // Turn ON Ghost
		AngleVectors(fPlayer->s.angles, NULL, NULL, up);
		VectorMA(fPlayer->s.origin, 60, up, start);
		VectorCopy(start, Ghost->s.origin); // Move Ghost
		gi.linkentity(Ghost);  // Must update Ghost!
	}
	//else
	// No fPlayer to follow so..
	// turn OFF Ghost temporarily

	Ghost->nextthink = level.time + 0.1f; // Every frame..
} 

//======================================================
// Create Ghost Entity
//======================================================
void Create_Ghost(void) 
{
	edict_t *Ghost; 

	Ghost=G_Spawn();
	Ghost->owner=NULL;
	Ghost->s.effects=EF_TELEPORTER; // Particle effect.
	Ghost->model="";
	Ghost->takedamage=DAMAGE_NO;
	Ghost->movetype=MOVETYPE_NONE;
	Ghost->solid=SOLID_NOT;
	Ghost->s.modelindex = 0;
	Ghost->s.modelindex2 = 0;
	VectorClear(Ghost->mins);
	VectorClear(Ghost->maxs);
	Ghost->delay = 0;
	Ghost->think=Ghost_Think;
	Ghost->nextthink = 1.0; // Start in 1 second.
	gi.linkentity(Ghost);
} 


