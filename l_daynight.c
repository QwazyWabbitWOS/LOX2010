//
//****************************************************
//		Day-Night Light Level Cycling for Q2:LOX
//****************************************************
//

/*

  This function computes the time interval needed to cycle
  the light level config string thru one day/night/day cycle
  based on the "nightdaytime" cvar. If the nightdaytime 
  is negative, it changes it to zero (disable day/night). 
  The server frames per light level are readjusted to account 
  for minlevel. This maintains the overall day-night time by 
  slightly extending the number of server frames used at each 
  light level.

  QwazyWabbit
*/

#include "g_local.h"

#define UP		0
#define DOWN	1

//cvars for customizing all this
cvar_t	*nightdaytime;
cvar_t	*minlevel;

/*
Suggested initialization in g_save.c:
	nightdaytime  = gi.cvar("nightdaytime","0",0);	//QW// minutes for a day-night cycle
	minlevel = gi.cvar("minlevel", "0", 0);		//QW// lowest light level to apply (0 | 1 | 2)

  For 2 day/nights per map:
  set timelimit 20
  set nightdaytime 10
  or any 2:1 ratio for these times.
*/

/*
Here are the new members of level_locals_t
// for night-day system
char	    lights[2];		// a modifiable string
int			lightcount1;
int			lightcount2;
int			updown;			// 0 up, 1 down
int			cycletime;	// server ticks per light level
*/

// Call this function at the start of G_RunFrame 
// and before the level.framenum++; line.

void DayNightCycle(void)
{
	float result;
	
	// catch frame 0 or frame 1 at start of a new map
	// because we might be called after framenum was incremented
	if (level.framenum <= 1 || nightdaytime->modified || minlevel->modified)
	{
		level.lights[0] = 'm'; //normal daylight
		level.lights[1] = '\0';
		level.lightcount1 = 12;	 //13 light levels a to m
		level.lightcount2 = 0;
		level.updown = DOWN;
		gi.configstring(CS_LIGHTS + 0,"m");
		
		//range checks
		if ((nightdaytime->value > timelimit->value) && timelimit->value != 0.0f)
			gi.cvar_set("nightdaytime", timelimit->string);
		if (nightdaytime->value < 0.5)	// disallow too small or negative values
			gi.cvar_set("nightdaytime", "0");
		if (minlevel->value < 0)
			gi.cvar_set("minlevel", "0");
		if (minlevel->value > 2)				//put a ceiling on how bright "dim" can be
			gi.cvar_set("minlevel", "2");	//otherwise why bother cycling it at all

		nightdaytime->modified = QFALSE;
		minlevel->modified = QFALSE;

		result = (60 / FRAMETIME) * (nightdaytime->value / 26); //convert minutes to frames per step
		
		//adjust number of server frames between light level changes
		level.cycletime =  (int) (result + (minlevel->value * result / (26.0f - minlevel->value)));
	}
	
	if (deathmatch->value && nightdaytime->value != 0.0f)
	{
		// is it time to adjust light levels?
		if (level.lightcount2 == level.cycletime)
		{
			// yes, Are we at the fullest brightness?
			if (level.lightcount1 == 12)
				level.updown = DOWN;	// start reducing
			else if (level.lightcount1 == minlevel->value) // at min light level
				level.updown = UP; // start increasing
			
			if (level.updown == DOWN)
				level.lightcount1--;
			else if (level.updown == UP)
				level.lightcount1++;
			
			// initialized above, a 2 byte string declared in g_local.h
			level.lights[0] = (char)('a' + level.lightcount1); // modify the string (ASCII dependency)
			
			// reset the lightcount2 timer and set the light level in the game
			level.lightcount2 = 0;
			gi.configstring(CS_LIGHTS + 0, level.lights);
			
			// diagnostics
			//gi.dprintf("level.lights %s \n", level.lights);	
			//gi.dprintf("level.lightcount1 %d\n",level.lightcount1);
			//gi.dprintf("level.lightcount2 %d\n",level.lightcount2);
			//gi.dprintf("level.updown %d\n",level.updown);
		}
		else
			level.lightcount2++;
	}
}
