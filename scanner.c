/*
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
#include "scanner.h"

void	ClearScanner(gclient_t *client)
{
	client->pers.scanner_active = 0;
}

void Toggle_Scanner (edict_t *ent)
{
	// No scanner if it's been banned.
	if (i_featureban & FB_SCANNER)
		return;
	
	if (!ent->client || ent->health <= 0)
		return;
	
	if (gi.argc() > 1)
	{
		ent->client->scannerx = (atoi(gi.argv(1))) + 80;
		if (gi.argc() > 2) 
			ent->client->scannery = (atoi(gi.argv(2))) + 40;
	}
	
	// toggle low on/off bit (and clear scores/inventory display if required)
	if ((ent->client->pers.scanner_active ^= 1) & 1)
	{
		ent->client->showinventory	= false;
		ent->client->showscores = false;
	}
	
	// set "just changed" bit
	ent->client->pers.scanner_active |= 2;
}

void ShowScanner(edict_t *ent,char *layout)
{
	int	i;
	edict_t	*player = g_edicts;
	char	stats[65], *tag;
	vec3_t	v;	
	
	// Main scanner graphic draw
	Com_sprintf (stats, sizeof(stats),
		"xv %d yv %d picn %s ", 
		ent->client->scannerx, 
		ent->client->scannery,
		PIC_SCANNER_TAG);
	
	SAFE_STRCAT(layout,stats,LAYOUT_MAX_LENGTH);
	
	// Players dots
	for (i=0 ; i < game.maxclients ; i++)
	{
		float	len;
		int		hd;
		
		// move to player edict
		player++;
		
		if (!player->inuse)		// not in use
			continue;
		if (!player->client)	// not a client or recently disconnected
			continue;
		if (player == ent)		// don't detect self
			continue;
		if (player->deadflag)	// dead
			continue;
		if (player->cloaked)	// cloaked
			continue;
		if (player->oldmodel)	// dead but got past dead test
			continue;

		// Don't show ghosts either.
		if (ctf->value && player->movetype == MOVETYPE_NOCLIP && player->solid == SOLID_NOT)
			continue;
		
		// calc player to enemy vector
		VectorSubtract (ent->s.origin, player->s.origin, v);
		
		// save height differential
		hd = (int) v[2] / SCANNER_UNIT;
		
		// remove height component
		v[2] = 0;
		
		// calc length of distance from top down view (no z)
		len = VectorLength (v) / SCANNER_UNIT;
		
		// in range ?
		if (len <= SCANNER_RANGE)
		{
			int		sx, sy;
			vec3_t	dp;
			vec3_t	normal = {0,0,-1};
			
			
			// normal vector to enemy
			VectorNormalize(v);
			
			// rotate round player view angle (yaw)
			RotatePointAroundVector( dp, normal, v, ent->s.angles[1]);
			
			// scale to fit scanner range (80 = pixel range of scanner)
			VectorScale(dp,len*80/SCANNER_RANGE,dp);
			
			// calc screen (x,y) (2 = half dot width)
			sx = (int) (80 + ent->client->scannerx + dp[1]) - 2;
			sy = (int) (80 + ent->client->scannery + dp[0]) - 2;
			
			// setup dot graphic
			tag = PIC_DOT_TAG;
			
			if (player->client->quad_framenum > level.framenum)
				tag = PIC_QUADDOT_TAG;
			
			if (player->client->invincible_framenum > level.framenum)
				tag = PIC_INVDOT_TAG;
			
			//			tag = PIC_FARTHESTDOT_TAG;
			// Set output
			Com_sprintf (stats, sizeof stats,"xv %i yv %i picn %s ", sx, sy, tag);
			
			SAFE_STRCAT(layout,stats,LAYOUT_MAX_LENGTH);
			
			// clear stats
			*stats = 0;
			
			// set up/down arrow
			if (hd < 0) // target is above us
				Com_sprintf (stats, sizeof(stats),"yv %i picn %s ",
				sy - 5,PIC_UP_TAG);
			else if (hd > 0)	//target is below us
				Com_sprintf (stats, sizeof(stats),"yv %i picn %s ",
				sy + 5,PIC_DOWN_TAG);
			
			// any up/down ?
			if (*stats)
				SAFE_STRCAT(layout, stats, LAYOUT_MAX_LENGTH);
		}
	}
}
