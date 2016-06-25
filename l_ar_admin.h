//
// AR-Admin logging functions.
//
#ifndef L_ARADMIN_H
#define L_ARADMIN_H

extern	cvar_t	*logarstats;		// Enable AlteredRealm player stats log (default = 0)
extern	cvar_t	*statsfile;			// the name of the AlteredRealm stats log file ("stats.log")
extern	cvar_t	*statsfile_rename;	// how often we rename the file
extern	cvar_t	*logfile_name;		// the server or qconsole.log file
extern	cvar_t	*logfile_rename;	// how often we rename the server log file

void ARLog_Stats(const char *fmt, ... );
int	CheckLocalMidnight(void);
void RenameStatsLog(void);
void RenameConsoleLog(void);

#endif
