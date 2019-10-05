//
// AR-Admin logging functions.
// Altered Realms http://www.alteredrealms.com/
// Program: ARSG - Altered Realms Stats Generator
// generates web page statistics based on output
// in this format.
//

#include "g_local.h"
#include "l_ar_admin.h"

#include <time.h>
#include <stdio.h>
#include <stdlib.h>

cvar_t	*logarstats;		// Enable AlteredRealm player stats log (default = 0)
cvar_t	*statsfile;			// the name of the AlteredRealm stats log file ("stats.log")
cvar_t	*statsfile_rename;	// how often we rename the file
cvar_t	*logfile_name;		// the server or qconsole.log file
cvar_t	*logfile;			// the logfile mode control
cvar_t	*logfile_rename;	// how often we rename the server log file

//#define AR_DEBUG

/* Calling syntax: */

/*
if (attacker->client) 
ARLog_Stats("OBIT: %s\\%s\\%d\\%d\\%d\\%d\\%s\n", // a player killed another player
	self->client->pers.netname, 
	attacker->client->pers.netname, 
	self->client->ps.ping, 
	attacker->client->ps.ping, 
	meansOfDeath, hitLocation,level.mapname); 
else 
ARLog_Stats("KILL: %s\\%d\\%d\\%d\\%s\n",	// a player killed himself
	self->client->pers.netname, 
	self->client->ps.ping, 
	meansOfDeath, hitLocation, level.mapname); 
*/

// Output the statistics strings to a file in the gamedir
// Inputs: 
//			pointer to the string to be logged.
//			checks logarstats 0 = off, 1 or 2 to enable.
//
void ARLog_Stats(const char *fmt, ... ) 
{ 
	
	va_list	argptr; 
	char	ar_string[1024]; 
	char	ar_tmp[1024]; 
    time_t	long_time;
	struct	tm	*ar_st; 
	char	logpath [MAX_QPATH];
	FILE	*f; 
	
	if (logarstats->value == 0)
		return;
	
	va_start(argptr, fmt); 
	vsprintf(ar_tmp, fmt, argptr); 
	va_end(argptr); 
	
	time(&long_time);
	ar_st = localtime(&long_time); 
	
	Com_sprintf( ar_string, sizeof(ar_string), "%02i/%02i/%02i %02i:%02i:%02i : %s", 
		ar_st->tm_mon + 1, 
		ar_st->tm_mday, 
		ar_st->tm_year + 1900, 
		ar_st->tm_hour, 
		ar_st->tm_min, 
		ar_st->tm_sec, 
		ar_tmp); 
	
	sprintf(logpath, "%s/%s", gamedir->string, statsfile->string);
	
	if (logarstats->value == 1)	// log to separate file
	{
		if ((f = fopen(logpath, "a")) != NULL) 
		{
			fprintf(f, "%s", ar_string); 
			fclose(f); 
		}
		else
			gi.dprintf("Error writing to %s\n", statsfile->string);
	}	
	else if (logarstats->value == 2) //log to qconsole.log
		gi.dprintf("%s", ar_string);
	
#ifdef AR_DEBUG
	OutputDebugString(ar_string);
#endif
	
} 

// Check local time and return true if it's midnight
int	CheckLocalMidnight(void)
{
    time_t	long_time;
	struct	tm	*ltime; 
	
	time(&long_time);
	ltime = localtime(&long_time); 
	
	if (ltime->tm_hour == 0 && ltime->tm_min == 0 && ltime->tm_sec == 0)
		return true;
	else 
		return false;
}

// this renames the stats file from lox/stats.log (or whatever)
// to lox/stats/YYYYMMDD-stats.log. The lox/stats/ folder MUST exist.
void RenameStatsLog(void)
{
	char	logpath[MAX_QPATH];
	char	newname[MAX_QPATH];
    time_t	long_time;
	struct	tm	*ltime; 
	
	time(&long_time);
	ltime = localtime(&long_time); 
	
	if ((statsfile_rename->value == 1)	// every day
		|| (statsfile_rename->value == 2 && ltime->tm_wday == 0)	// sundays
		|| (statsfile_rename->value == 3 && ltime->tm_mday == 1))	// 1st of month
	{
		sprintf(logpath, "%s/%s", gamedir->string, statsfile->string);
		sprintf(newname, "%s/stats/%02i%02i%02i-%s", 
			gamedir->string,
			ltime->tm_year + 1900, 
			ltime->tm_mon + 1, 
			ltime->tm_mday, 
			statsfile->string); 
		
		if (rename(logpath, newname))
		{
			if(errno == EACCES)
			{
				gi.dprintf("Error renaming %s to %s, make sure target folder exists.\n", logpath, newname);
			}
			else if (errno == ENOENT)
			{
				gi.dprintf("Error renaming %s, it doesn't exist.\n", logpath);
			}
		}
	}
}

//QW// rename qconsole.log once per day | week | month
// depending on value of logfile_rename, default weekly. (g_save.c)
void RenameConsoleLog(void)
{
	char	logpath [MAX_QPATH], newname[MAX_QPATH];
    time_t	long_time;
	struct	tm	*ltime; 
	char	*logval = "  ";
	
	time(&long_time);
	ltime = localtime(&long_time);
	
	if ((logfile_rename->value == 1)	// every day
		|| (logfile_rename->value == 2 && ltime->tm_wday == 0)	// sundays
		|| (logfile_rename->value == 3 && ltime->tm_mday == 1))	// 1st of month
	{
		logfile = gi.cvar("logfile", "", CVAR_NOSET);	//expose the variables
		logfile_name = gi.cvar("logfile_name", "", CVAR_NOSET);
		logval = logfile->string;			// save current mode
		gi.cvar_forceset("logfile", "0");	// turn off logging
		gi.dprintf("Backing up logfile\n");	// post a message to force log closure
		sprintf(logpath, "%s/%s", gamedir->string, logfile_name->string);
		sprintf(newname, "%s/logs/%02i%02i%02i-%s", 
			gamedir->string,
			ltime->tm_year + 1900, 
			ltime->tm_mon + 1, 
			ltime->tm_mday, 
			logfile_name->string); 
		
		if (rename(logpath, newname))
		{
			if(errno == EACCES)
			{
				gi.dprintf("Error renaming %s to %s, make sure target folder exists.\n", logpath, newname);
			}
			else if (errno == ENOENT)
			{
				gi.dprintf("Error renaming %s, it doesn't exist.\n", logpath);
			}
		}
		
		gi.cvar_forceset("logfile", logval);	// restore previous mode
		gi.dprintf("Logfile backup complete.\n"); //announce and reopen
	}
}
