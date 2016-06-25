Files in this release:

lox_1_12_readme.txt - This file
gamei386.so - Library file game extension for Linux.
gamex86.dll - Dynamic Library for Windows.


Prerequisites:

I strongly recommend you install and test the LOX 1.10 and 1.11 packages before updating
to this package.  This package includes only the LOX 1.12 library extension and does
NOT include the other files you need to properly run the mod.

You MUST download and install or have previously installed the LOX 1.11 UPDATE
that included the 1.11 or 1.10 DLL or Library file and all the PAK files associated
with the LOX modification.  

When running this mod on Linux, you must run the Quake2 version 3.21 code.  
Users have reported Segmentation Faults when running this mod on Linux 
with the v3.20 core.

This shared library was compiled on Fedora Core 2-i386 using GCC 3.3.2.

If you already have these and any previous 1.12.x updates
then you don't need to do anything except install this patch.

Instructions for Lox 1.12 DLL Download and Installation:

Before you replace the DLL, make sure Quake2 is not running.

Command line instructions:

1 - Change directory to your Quake2\lox directory

2 - Rename your old lox game dll to gamex86.1_11.dll
    For example: prompt> ren gamex86.dll gamex86.1_11.dll

3 - Copy the new LOX dll to the proper name.
    prompt> copy gamex86_1_12.dll gamex86.dll
    From Winzip, simply drag the DLL from the Winzip window
    to your quake2\lox folder and rename it.
	In Linux the file is gamei386.so and is already properly 
	named in this archive.
	
4 - If you need to return to lox 1.11 later, simply reverse 
    the process by copying your 1.11 file back.
    prompt> copy gamex86.1_11.dll gamex86.dll

Using Winzip

1 - Browse with Explorer to the Quake2\lox folder.
2 - Rename game86.dll you find there to something like gamex86.lox.1.11.dll
    Yes, multiple dots are ok in Windows file names.
3 - Drag the gamex86_1_12_3.dll in this ZIP file to the quake2\lox folder
    and rename it to gamex86.dll
    
    
Fixes in the LOX 1.12.4 release dated June 13, 2004:

12/12/2003 - P_client.c(731) changed "reaped" to "ripped" and took out extra spaces in the text.

12/21/2003 - Fixed decoy bug causing homer rockets, trackers and skippers to crash server.

12/30/2003 - Fixed decoy bug.  Skin of decoy was always skin of client 0, the first client to
             connect to the server.  Modified g_monster.c, monster_start().  The the decoy spawn
			 code was setting the proper self->skinnum, but monster_start() was setting it to 0.
			 Decoys now have skin of their owners.
			              
12/31/2003 - Fixed hook bug.  Grapple and chain lingered if player disconnected with hook engaged.

01/10/2004 - Limit number of deployable lasertrips to 3. (Now a variable, see 1/17/04 below.)
             Added lasertrip member to gclient_s in g_local.h.  

01/10/2004 - Floating mines (g_weapon.c), limit is 7.  TODO: Test mines, check for bugs.
             Essentially no code change so far.  Oops, no floating mine launcher object?
			 FML works in cheat mode (give all) but how to pick up launcher in normal game?

01/15/2004 - Debugged lasertrip limiting.  Tested OK with downlevel client (1.11) and mod-less
             MAC client, no bugs.

01/15/2004 - New cvar, debugmodels. Set it to 1 in server.cfg if you want console output of
             model information.  Now this can be turned off or on at runtime.

01/17/2004 - New cvars, max_trackers and max_lasertrips.  This allows server.cfg to set the
             maximum allowable trackers or laser tripwires a user can deploy.  New commands for
			 config: set maxtrackers n and set maxlasertrips n.

01/18/2004 - Revisited debug model checking.  Do the check always in deathmatch dedicated maplist
             mode as before but only print the messages if debugmodels is set.

01/20/2004 - Enabled BFG weapon in unlimited ammo mode.  See dmflags parsing in g_items.c.  Was
             a hack to disable them long time ago.  Need to make sure weapon banning will work.
			 This might take some work since banning BFG and FBFG will also affect the vortex
			 and other weps that depend on the BFG objects. This complication is probably
			 the reason for the original hack in the dmflags.  
			 Note: Need to check wep select code and do selection based on weapban and keep the 
			 bfg object, just don't allow its selection.

01/21/2004 - Deadman reports shortening gamename fixes nagging Info string length exceeded message.
             This is coming from Info_SetValueForKey() in q_shared.c when the server has to send
			 the server information to a client like Gamespy. This message is caused by the server 
			 detecting that it needs to send a big frame to a client.  Threshold is 512 bytes. 
			 Max key/value size is 64 but LOX sends a lot of extra server keys that other mods don't.
			 Lay one or two proxy DLL's on top of it and you get lots of status info.

02/14/2004 - Added server configuration varable kami_frozen to allow clients
             to go kamikaze even if frozen.  Use "set kamifrozen 1" to allow.

03/07/2004 - Re-invented guided grenades.  Added g_weapon.h for interface into calling
			 the g_weapon.c functions from within l_rockets.c. Added server configuration
			 variable disruptor_trackers to allow trackers to be launched if you are carrying
			 disruptor.  Use set disruptortrackers 1 to allow them.

03/09/2004	 Added time and timeleft/timeremaining commands.  Command "time" tells you what time
			 it is using _strtime().  Command "timeremaining" tells you how many minutes are left
			 in the match.

03/10/2004 - Changed gamename to shorter text. Removed revengereward, highfraggerreward and 
			 sv_bestplayer from the CVAR_SERVERINFO class of variables.  This keeps the status
			 frame under 512 bytes and gets rid of annoying "Info string length exceeded" messages.
			 I don't think these needed to be serverinfo vars anyway.

			 Fixed Cmd_Time_f function.  Originally commented out, I blindly recovered it for use.
			 I must confess I didn't do the homework right away.  _strtime is non-portable. Fixed
			 the glaring error with ANSI C time() and ctime() calls instead. Now it even prints the
			 day, date and time real purty-like.

03/11/2004 - Fixed bugs in time functions. They used gi.bprintf() and this broadcasts too all
             users... oops.  Use gi.cprintf() instead.  It's not a bug, it's a feature.  I might consider
             creating a broadcast version of the time commands later.  Maybe something like "It's <timedate>
             at the <servername> server, but it's <clienttime> at <playername>'s and there's only <minutes> left
             in this match".  With appropriate spam limiting, of course.

03/13/2004 - Replaced some player obituary texts.  Fixed some spelling and tried to add a little humor.

03/17/2004 - Set a hard coded floor of 1 and ceiling of 10 trackers in the tracker limiter. It was possible
             to set huge tracker limits on rcon and lag the server.
			 
			 Reset tracker initial damage to zero.  Setting it to 10 caused lag when a bunch of them
			 died all at once.
			 
			 Fixed up some more text formatting in obituary texts.

03/20/2004 - Increased damage radius of vacuum maker by 20% (1000 -> 1200) and increased damage to 250.
             Doubled Kamikaze damage from 300 to 600.

			 Released as Version 1.12.1.

03/20/2004 - Version 1.12.2 commences.
			 Decreased damage from 1000 to 500 on homer rockets.
             More messing with obituary texts.

03/23/2004 - Released 1.12.2

03/27/2004 - Spell-checked help (man) text.
             Reorganized Cmd_Help function.

04/03/2004 - Split guided, homing and guided nuke rockets into separate files.

04/22/2004 - Added code for addition of floating mine launcher to inventory when player
             picks up a grenade launcher.

05/29/2004 - Fixup nuke, guided nuke, guided rocket classnames.  Enabled angel of mercy
             to catch nukes.  Still working on guided grabs. When angelom catches an eligible
			 projectile it calls G_FreeEdict on that entity and this blows the logic of the
			 guided rockets and guided nukes leaving the player who fired the guided rocket
			 in the floor or somewhere in limbo. The only way to unlock it is to command kill.
			 Keeping the class names distict for each weapon takes care of a lot of interaction
			 issues with angels and trackers and whatnot.

			 Fixed Floating Mine Launcher select. Users need to change bind in bindings.cfg 
			 for grenade launcher to read:
			 alias grenadelauncher5 "use double impact;wait;bind 6 grenadelauncher6"
			 alias grenadelauncher6 "use floating mine launcher;wait;bind 6 grenadelauncher1"

05/31/2004 - Released 1.12.3 for Windows and Linux.

06/13/2004 - Re-release of rebuilt LOX 1.12.3 for Windows and Linux.

06/19/2004 - Coded configurable limit for floating mines.  Default limit is seven
             mines.  Set maxfloatingmines in server config to change.  Maximum allowed
			 per player is hard coded at 10, minumum is 1.

06/26/2004 - Denial of Service bug found by Tommy<WOS>. Can crash server every time 
			 under certain conditions. This is fixed with in versions 1.12.4 and up.

			 Fixed angelom code that had test code in it that printed diagnostic messages
			 in 1.12.3 release of the mod.

			 Defined manifest constants for darKMajick grenade types.

			 Allow floating mine launcher to fire most kinds of darKMajick grenades.
			 FML now works with all grenades except the teleport grenades, if you're
			 holding teleports, it launches normal grenades instead.

			 Released 1.12.4 for Windows and Linux. 

06/30/2004 - Version 1.12.5 commences. 
			 Added Angel of Energy.
             
			 Divided angels into their own source files to make maintenance a little
             easier (I hope). Now we can pull an angel if we don't need it anymore or
             modify it without scrolling through a long file to find it.
             Added function prototypes and constants to angels.h
             Modified all other modules that refer to angels to include angels.h.
             Added AngelOE and AngelOU help text. 
             
             Improved HOOK help text, added a description of the binds you need to 
             write to make it work. This helps oldtimers tell newbs to the mod how 
             to do the bind. This is a FAQ, so now users can say "type 'man hook' 
             at the console" and the server will explain it for them.

07/02/2004 - Modified the angels quite a bit. Angel of Revenge now depletes other
             players health by 50% while in contact with the opponent until their
             health is reduced to about 25 points. This even works against regeneration
             holders.  
             
             Angel of Mercy or Angel of Revenge deflect the Angel of Revenge.

             Angel of Unfreeze or Angel of Mercy deflects the Angel of Frost.

             Angel of Plague rapidly depletes your opponent's health until
             he has only 10 health points left, making him an easy kill. The
             range of the angel is 100 quake units. Flying the Angel of Life
             gives immunity to plague. Plague rate of decay is set at 5 health 
			 points per client cycle.

             Being cloaked does not hide you from any angels.

07/04/2004 - Freeze grenade modified.  It now inflicts damage directly on the
             target on detonation.

             Fixed up GuidedRocket_Explode function to simply call Grenade_Explode_dM()
             for explosion effects. Now all the darKMajick grenades are maintained in
             one place.

08/20/2004 - Removed durgsweeper.

08/30/2004 - Added Kaminit, super rail shotgun, and disruptor trackers.
			 Fixed up a lot of code and headers.

09/04/2004 - Adding map voting to allow players to elect a new map instead of depending
             on admins to use the RCON command.

09/12/2004 - Fixed up some manipulations of cvars.
			 Perfected map voting and documented electpercentage, 
			 electduration, electreminders.
			 Bounds checks for these variables so we don't do silly things.
			 Sanity check of linux Makefile to be sure we can still build it right.

09/14/2004 - Compiled under Linux, fixed up EOL's in files as needed.
			 Nice clean compile under Linux now, fixing up warnings in VC6 @ W4.
			 Need to check guided missiles, weaker damage actually increases health
			 of targets at long ranges. Kinda cool, need to find out why though.

09/19/2004 - Added cvar campertime, default value 5 second timeout for anticamper.

10/18/2004 - Found a bug that was causing cloak to be ineffective against homers/skippers.
			 In rockets.c the line if((blip->cloaked == 1) || (blip->oldmodel == 1)) continue;
			 was: if(blip->cloaked == 1 || blip->oldmodel == 1) continue;
			 WAY too subtle!

10/23/2004 - Fixed angelom vs. guided missiles. Angel of mercy now successfully defends
             against guided rockets without sending the POV of the guided rocket owner
			 into the floor or some other weird space.
             Changed sound file for kaminit or kamikaze to play kamikaze.wav.

10/26/2004 - Fixed angelom vs. guided nuke bug.
             Fixed a lingering map voting bug. Server would crash if a non-existant map
			 passed the vote. Map would not change, but intermission would crash it.

10/28/2004 - Fixed freeing of angel edicts on player death.

10/29/2004 - 1.12.5 Released for use on Loxophilia and later for the rest of world.

11/14/2004 - Begin LOX 1.12.6 codebase. Version 1.12.5 will probably never be released
             beyond its test use on Loxophilia.
			 Changed all wepban and featureban logic in order to fix float/int conversion
			 problems long since described in the the notes.txt. Created several i_vars
			 that contain the int-converted strings received by the game and stored in the 
			 cvars as floats and string members. Now the game checks ints against ints
			 without casts and all the attendant overhead. See UpdateBans() for info.
			 
11/20/2004 - Changed default maxentities to 2048 (g_save.c).

11/23/2004 - Changed map vote logic to stop spamming. A player can only initiate two
             map votes in a session by default. Server admins can configure the cvar 
			 electstarts to allow from 1 to 6 election starts per player. 
			 Added veto power for a single no vote.
			 Clipped out YES/NO vote updates, too many updates when everyone is voting.
			 Now it updates on the timer. Player voting doesn't increase the frequency.
			 Voting stops on veto or when a winning vote is mathematically impossible.
			 After observation of 1.12.6 on Loxophilia, changed maxentities back to 1024.



Phlem
9/13/2004 - Changed spiral rocket model to gibs/head2/tris.md2

9/13/2004 - Changed rocket model to gibs/skull/tris.md2 in l_rocket.c

