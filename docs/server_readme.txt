               *************************************
               Instructions for running a WOD:LOX server
               *************************************

I suggest naming your Weapons of Destruction:Lethal Orifice eXamination
directory "lox".  That seems to be the standard over the Internet.  
And it makes it much easier, when it comes to automatic downloads and 
all that, to know all Weapons of Destruction:Lethal Orifice eXamination
files are going into the same directory.

Two default config files have been provided -- server.cfg for deathmatch. 
You will probably want to edit them before running your server for the 
first time (though their defaults work well).

To run a dedicated deathmatch server, the command is:
quake2 +set dedicated 1 +set deathmatch 1 +set game lox +exec server.cfg

To run a dedicated Extinction (CTF) server, the command is:
quake2 +set dedicated 1 +set ctf 1 +set game lox +exec server.cfg

To run a dedicated Cooperative server, the command is:
quake2 +set dedicated 1 +set coop 1 +set game lox +exec server.cfg

-----

Try "set maplist 1" on your deathmatch server...the next map will be
chosen from maplist.txt, instead of running in the normal loop.
(maplist will be subsequently set to 2, 3, etc. by the server,
indicating where it is in the maplist.  It will loop back to 1 at the
end of the file.)

I recommend starting with either "biggun" or "fact3" -- in my
experience, those are the two biggest fragfest maps in the standard
collection, and they're nice to use whenever you want to wake everyone
up and get their blood pumping.

The maplist.txt file, as provided, contains my opinion of which game
maps make good DM maps.  You are free to have your own opinion.
maplist.txt contains the name of each of the good maps, 10 times,
shuffled together thoroughly.  Should be fine for most uses!

-----
Deathmatch has different flags for setting different features on
a dedicated server. These flags are common to all DM servers and are
contained in the dmflags cvar. The definitions are as follows:

// dmflags
 DF_NO_HEALTH			1
 DF_NO_ITEMS			2
 DF_WEAPONS_STAY		4
 DF_NO_FALLING			8
 DF_INSTANT_ITEMS		16
 DF_SAME_LEVEL			32
 DF_SKINTEAMS			64
 DF_MODELTEAMS			128
 DF_NO_FRIENDLY_FIRE	256
 DF_SPAWN_FARTHEST		512
 DF_FORCE_RESPAWN		1024
 DF_NO_ARMOR			2048
 DF_ALLOW_EXIT			4096
 DF_INFINITE_AMMO		8192
 DF_QUAD_DROP			16384
 DF_FIXED_FOV			32768
 DF_REVENGE_FACTOR		65536
 DF_DAYNIGHT			131072
 DF_ANTICAMPER			262144
 DF_HIGHFRAGGER			524288
 DF_WEAPONHEAT			1048576

You can ban any of the weapons and features with the "weaponban" and
"featureban" variables.  Both variables are flags, in the same way
that the "dmflags" variable is: just add together the numbers of the
weapons/features you want to ban, and that's your value for
weaponban/featureban, respectively.

Values for LOX weapons weaponban and featurebans:

----------------- 
LOX 1.12.4 Flags 
----------------- 
 
// weaponban 
SUPERBLASTER		1 
SHOTGUN				2 
SNIPERGUN			4 
SUPERSHOTGUN		8 

FREEZEGUN			16 
MACHINEGUN			32 
MACHINEROCKETGUN    64 
BURSTMACHINEGUN		128 

CHAINGUN			256 
STREETSWEEPER		512 
GRENADE				1024 
CLUSTERGRENADE		2048 

RAILBOMB			4096 
PLASMAGRENADE		8192 
NAPALMGRENADE		16384 
SHRAPNELGRENADE		32768 

CATACLYSMDEVICE		65536 
GRENADELAUNCHER		131072 
BAZOOKA				262144 
ROCKETLAUNCHER		524288 

HOMINGROCKETLAUNCHER   1048576 
HYPERBLASTER		2097152 
PLASMARIFLE			4194304 
RAILGUN				8388608 

FLAMETHROWER		16777216 
BFG10K				33554432 

For example, to ban the cataclysm device, the homing rocketlauncher,
and the BFG10K (the 3 most hated weapons it seems), weaponban is
65536 + 1048576 + 33554432 == 34668544.

Note that if you ban grenades (value 1024), then *all* of the grenade
types, as well as the grenade launcher and bazooka, are banned too.

// loxweaponban 
AIRFIST					1 
SWORD					2 
PULSERIFLE				4 
NAILGUN					8 
BUCKY					16 
SKIPPERHOMINGROCKET		32 
//two spots here 
// 
GUIDEDMISSILE			256 
POSITRON				512 
STICKGRENADELAUNCHER    1024 
ICECUBEGUN				2048 
DURG					4096 
LIGHTNINGGUN			8192 
DOUBLEIMPACT			16384 
FBFG					32768 
DISINTEGRATOR			65536 
VACUUMMAKER				131072 
ENERGYVORTEX			262144 
SPIRALROCKETLAUNCHER	524288 
SONICRAILGUN			1048576 
EXPLOSIVESHOTGUN		2097152 
EXPLOSIVESUPERSHOTGUN	4194304 
EXPLOSIVEMACHINEGUN		8388608 
KAMINIT					16777216 
WALLPIERCINGRAILGUN		33554432 
NUKE					67108864 
FLOATINGMINELAUNCHER	134217728 
ANTIMATTERCANNON		268435456 
SUPERRAILSHOTGUN		536870912 
GUIDEDNUKE				1073741824  
 
// loxsweeperban 
SUPERBLASTERSWEEPER		1 
FREEZERSWEEPER			2 
GRENADESWEEPER			4 
BAZOOKASWEEPER			8 
RAILGUNSWEEPER			16 
FLAMESWEEPER			32 
DURGSWEEPER				64 
CHUNKGUN				128 
SNIPERSWEEPER			256  
EXPLOSIVECHAINGUN		512  
EXPLOSIVESTREETSWEEPER	1024  
STICKYGRENADESWEEPER	2048 
  
// loxgrenadeban 
PLASMACLUSTERGRENADE	1 
TELEGRENADE				2 
FREEZERGRENADE			4 
HOMERGRENADE			8 
ROCKETGRENADE			16 
BLINDINGGRENADE			32 
POSITRONGRENADE			64 
  
// loxarmorban 
ORGANICARMOR	1 
SLUGARMOR		2 
CELLARMOR		4 
SHELLARMOR		8 
BULLETARMOR		16 
ROCKETARMOR		32 
GRENADEARMOR	64 
 
// loxfeatureban 
CLOAK			1 
EXPLOSIVE		2   
INVISIBILITY	4    
FLASHLIGHT		8  
CHECKSTATS		16 
RANGEFINDER		32 
PLAYERID		64 
FEIGNDEATH		128 
ANGELOD			256 
ANGELOL			512 
ANGELOM			1024 
ANGELOF			2048 
ANGELOP			4096 
SWEEPERFIREMODE	8192   
HEIGHT			16384 
ANGELOB			32768 
ANGELOH			65536 
ANGELOBLIND		131072   
AIRSTRIKE		262144 
FPM				524288 
TRACKER			1048576 
ANGELOR			2097152 
BATON			4194304 
HOLO			8388608 
CHAMBER			16777216 
REGENHEALTH		33554432 
ANGELOU			67108864 
VAMPIHEALTH		134217728 
  
// featureban 
BANDOLIER		1 
AMMOPACK		2 
SILENCER		4 
REBREATHER		8 
ENVIROSUIT		16 
ADRENALINE		32 
MEGAHEALTH		64 
POWERARMOR		128 
INVULNERABILITY	256  
DECOY			512 
HOOK			1024  
ID				2048 
KAMIKAZE		4096  
LASERSIGHT		8192 
TRIPLASER		16384 
NIGHTVISION		32768 
PUSHPULL		65536 
SCANNER			131072 
ZOOM			524288 
JETPACK			1048576 
CLOAK			2097152 

For example, to ban the decoy, nightvision, and scanner, featureban is
512 + 32768 + 131072 == 164352.
 
// fragban
 
BLASTER			67108864 
TELEFRAG		134217728 
KAMIKAZE		268435456 
HOOK			536870912 

You can also ban frags for certain weapons, i.e. players can use them
to kill other players, but won't score any points for doing so.  The
"fragban" variable uses the same flags and values as the "weaponban"
variable, with the following additions:

Ban frags for blaster				67108864
Ban frags for telefrags				134217728
Ban frags for kamikazes				268435456
Ban frags for grappling hook		536870912

For example, to ban frags for cataclysm devices, homing rockets, and
telefrags, fragban is 65536 + 1048576 + 134217728 == 135331840.

Also note that banning frags for the flamethrower (value 16777216) will
ban frags for all fire.

LOX versions after 1.12.6 have moved many flags from where 
they were in the 1.12.4 release.

----------------- 
LOX 1.12.6+ Flags 
----------------- 

// Weapon banning flags.
//weaponban
 WB_SUPERBLASTER			0x00000001	//1
 WB_SHOTGUN					0x00000002	//2
 WB_SNIPERGUN				0x00000004	//4
 WB_SUPERSHOTGUN			0x00000008	//8
 
 WB_FREEZEGUN				0x00000010	//16
 WB_MACHINEGUN				0x00000020	//32
 WB_MACHINEROCKETGUN		0x00000040	//64
 WB_BURSTMACHINEGUN			0x00000080	//128
 
 WB_CHAINGUN				0x00000100	//256
 WB_STREETSWEEPER			0x00000200	//512
 WB_GRENADE					0x00000400	//1024
 WB_CLUSTERGRENADE			0x00000800	//2048
 
 WB_RAILBOMB				0x00001000	//4096
 WB_PLASMAGRENADE			0x00002000	//8192
 WB_NAPALMGRENADE			0x00004000	//16384
 WB_SHRAPNELGRENADE			0x00008000	//32768
 
 WB_GRENADELAUNCHER			0x00010000	//65536
 WB_BAZOOKA					0x00020000	//131072
 WB_ROCKETLAUNCHER			0x00040000	//262144
 WB_HOMINGROCKETLAUNCHER	0x00080000	//524288
 
 WB_HYPERBLASTER			0x00100000	//1048576
 WB_PLASMARIFLE				0x00200000	//2097152
 WB_RAILGUN					0x00400000	//4194304
 WB_FLAMETHROWER			0x00800000	//8388608
 
 WB_BFG10K					0x01000000	//16777216


// Banning flags for the WoD:LOX weapon extensions

//loxweaponban
 LWB_AIRFIST				0x00000001 //1
 LWB_SWORD					0x00000002 //2
 LWB_PULSERIFLE				0x00000004 //4
 LWB_NAILGUN				0x00000008 //8
 
 LWB_BUCKY					0x00000010 //16
 LWB_SKIPPERHOMINGROCKET	0x00000020 //32
 LWB_CATACLYSMDEVICE		0x00000040 //64
 LWB_GUIDEDNUKE				0x00000080 //128
 
 LWB_GUIDEDMISSILE			0x00000100 //256
 LWB_POSITRON				0x00000200 //512
 LWB_STICKGRENADELAUNCHER   0x00000400 //1024
 LWB_ICECUBEGUN				0x00000800 //2048
 
 LWB_DURG					0x00001000 //4096
 LWB_LIGHTNINGGUN			0x00002000 //8192
 LWB_DOUBLEIMPACT			0x00004000 //16384
 LWB_FBFG					0x00008000 //32768
 
 LWB_DISINTEGRATOR			0x00010000 //65536
 LWB_VACUUMMAKER			0x00020000 //131072
 LWB_ENERGYVORTEX			0x00040000 //262144
 LWB_SPIRALROCKETLAUNCHER	0x00080000 //524288
 
 LWB_SONICRAILGUN			0x00100000 //1048576
 LWB_EXPLOSIVESHOTGUN		0x00200000 //2097152
 LWB_EXPLOSIVESUPERSHOTGUN	0x00400000 //4194304
 LWB_EXPLOSIVEMACHINEGUN	0x00800000 //8388608
 
 LWB_KAMINIT				0x01000000 //16777216
 LWB_WALLPIERCINGRAILGUN	0x02000000 //33554432
 LWB_NUKE					0x04000000 //67108864
 LWB_FLOATINGMINELAUNCHER	0x08000000 //134217728
 
 LWB_ANTIMATTERCANNON		0x10000000 //268435456
 LWB_SUPERRAILSHOTGUN		0x20000000 //536870912

//loxtrackerban
 LTB_BFG10K					0x00000001 //1
 LTB_FBFG					0x00000002 //2
 LTB_FREEZEGUN				0x00000004 //4
 LTB_NAILGUN				0x00000008 //8
 LTB_BUCKY					0x00000010 //16
 LTB_RAILGUN				0x00000020 //32
 LTB_ENERGYVORTEX			0x00000040 //64
 LTB_KAMINIT				0x00000080 //128
 LTB_SNIPERGUN				0x00000100 //256
 LTB_POSITRON				0x00000200 //512
 LTB_STICKGRENADELAUNCHER	0x00000400 //1024
 LTB_ICECUBEGUN				0x00000800 //2048
 LTB_DURG					0x00001000 //4096
 LTB_LIGHTNINGGUN			0x00002000 //8192
 LTB_DOUBLEIMPACT			0x00004000 //16384
 LTB_DISINTEGRATOR			0x00008000 //32768
 LTB_PULSERIFLE				0x00010000 //65536
 LTB_EXPLOSIVESHOTGUN		0x00020000 //131072
 LTB_EXPLOSIVESUPERSHOTGUN	0x00040000 //262144
 LTB_EXPLOSIVEMACHINEGUN	0x00080000 //524288

// sweeper bans
// loxsweeperban 
 LSB_SUPERBLASTERSWEEPER	0x00000001
 LSB_FREEZERSWEEPER			0x00000002
 LSB_GRENADESWEEPER			0x00000004
 LSB_BAZOOKASWEEPER			0x00000008
 LSB_RAILGUNSWEEPER			0x00000010
 LSB_FLAMESWEEPER			0x00000020
 LSB_DURGSWEEPER			0x00000040
 LSB_CHUNKGUN				0x00000080
 LSB_SNIPERSWEEPER			0x00000100
 LSB_EXPLOSIVECHAINGUN		0x00000200
 LSB_EXPLOSIVESTREETSWEEPER	0x00000400
 LSB_STICKYGRENADESWEEPER	0x00000800

// loxgrenadeban 
 LGB_PLASMACLUSTERGRENADE	0x00000001
 LGB_TELEGRENADE			0x00000002
 LGB_FREEZERGRENADE			0x00000004
 LGB_HOMERGRENADE			0x00000008
 LGB_ROCKETGRENADE			0x00000010
 LGB_BLINDINGGRENADE		0x00000020
 LGB_POSITRONGRENADE		0x00000040

// Armor banning flags for the WoD:LOX armor extensions
// loxarmorban 
 LAB_ORGANICARMOR			0x00000001
 LAB_SLUGARMOR				0x00000002
 LAB_CELLARMOR				0x00000004
 LAB_SHELLARMOR				0x00000008
 LAB_BULLETARMOR			0x00000010
 LAB_ROCKETARMOR			0x00000020
 LAB_GRENADEARMOR			0x00000040

// loxfeatureban 
 LFB_CLOAK					0x00000001
 LFB_EXPLOSIVE				0x00000002
 LFB_INVISIBILITY			0x00000004
 LFB_FLASHLIGHT				0x00000008
 LFB_CHECKSTATS				0x00000010
 LFB_RANGEFINDER			0x00000020
 LFB_PLAYERID				0x00000040
 LFB_FEIGNDEATH				0x00000080
 LFB_ANGELOD				0x00000100
 LFB_ANGELOL				0x00000200
 LFB_ANGELOM				0x00000400
 LFB_ANGELOF				0x00000800
 LFB_ANGELOP				0x00001000
 LFB_SWEEPERFIREMODE		0x00002000
 LFB_HEIGHT					0x00004000
 LFB_ANGELOB				0x00008000
 LFB_ANGELOH				0x00010000
 LFB_ANGELOBLIND			0x00020000
 LFB_AIRSTRIKE				0x00040000
 LFB_FPM					0x00080000	//524288
 LFB_TRACKER				0x00100000
 LFB_ANGELOR				0x00200000
 LFB_BATON					0x00400000
 LFB_HOLO					0x00800000
 LFB_CHAMBER				0x01000000
 LFB_REGENHEALTH			0x02000000
 LFB_ANGELOU				0x04000000
 LFB_VAMPIHEALTH			0x08000000
 LFB_ANGELOE				0x10000000

// Frag banning flags (used in addition to weapon banning flags).
//fragban
 WFB_BLASTER				0x04000000
 WFB_TELEFRAG				0x08000000
 WFB_KAMIKAZE				0x10000000
 WFB_HOOK					0x20000000

// Feature banning flags.
//featureban
 FB_BANDOLIER				0x00000001
 FB_AMMOPACK				0x00000002
 FB_SILENCER				0x00000004
 FB_REBREATHER				0x00000008
 FB_ENVIROSUIT				0x00000010
 FB_ADRENALINE				0x00000020
 FB_MEGAHEALTH				0x00000040
 FB_POWERARMOR				0x00000080
 FB_INVULNERABILITY			0x00000100
 FB_DECOY					0x00000200
 FB_HOOK					0x00000400
 FB_ID						0x00000800
 FB_KAMIKAZE				0x00001000
 FB_LASERSIGHT				0x00002000
 FB_TRIPLASER				0x00004000
 FB_NIGHTVISION				0x00008000
 FB_PUSHPULL				0x00010000
 FB_SCANNER					0x00020000
 FB_ZOOM					0x00040000
 FB_JETPACK					0x00080000


-----

WOD:LOX now features IP banning.  You can add or remove addresses from the
filter list with "sv addip <ip>" and "sv removeip <ip>".  The ip address
is specified in dot format, and any unspecified digits will match any
value, so you can specify an entire class C network with "addip 192.246.40".

removeip will only remove an address specified exactly the same way.
You cannot addip a subnet, then removeip a single host.

There is one more way to ban a player.  "sv kickban <player#>" will ban
the player's IP address and then kick them from the server.

"sv listip" prints the current list of filters.  "sv writeip" dumps
"addip <ip>" commands to listip.cfg so it can be execed at a later date.
The filter lists are not saved and restored by default.

In addition, there's a "filterban" variable.  If set to 1 (the default),
then ip addresses matching the current list will be prohibited from
entering the game.  If set to 0, then only addresses matching the list
will be allowed.  This lets you easily set up a private game, or a game
that only allows players from your local network.

There is a nightdaytime variable available now. The default value is 13. 
This means that the time that it takes for the cycle to go from day to
night to day 13 minutes. Just modify the nightdaytime variable to suit 
your needs.

-----

Does your server sometimes crash with "ERROR: *Index: overflow"?

I've finally figured out what the problem is.  Turns out the game is
running out of modelindexes.  Every model in the game (male player,
female player, each weapon, health-pack, and so on) gets loaded in and
is assigned a modelindex.  Quake II has an internal limit of 256 models.

Most people don't get this problem unless they have several extra player
models installed on their server machine, *and* they use the maplist
feature.
         
The problem happens because anything in the map that can move or be
destroyed (exploding walls, bridges, and so on) are also considered a
model.  They might not be used in the deathmatch game, but the
modelindex doesn't get freed.  That's just some sloppiness on Id's part.
I've written them a letter about it.

Until they fix their bug, here's what you do.  First, if you have extra
installed player models, you don't necessarily have to remove them.
First, see if you can get just the standard 8 DM maps to run without
incident.  If not, you have too many player models & you're gonna have
to remove some.  But if the DM maps work fine, then you can run your
server with maplist turned on.  Note what maps cause your server to
crash, and remove them from the maplist.txt file.  Eventually :) you
will have a list of maps that won't crash your server.

07-30-99
Well I have an update to the ERROR: *Index: overflow problem, I have 
created a workaround to the problem. Essentially this is what is going
on. Whenever a new map loads the old models get uncached an the new
models get cached. Knowing this information helped me develop the workaround.
Basically what I did was provide a check that if you are about to get
that error it will end the level and start the next one. Hopefully this
will work until I can get a better solution. 

09-06-99
It turns out that if you have the Grenade and Bazooka sweepers enabled
you will start to get some warnings. Well these are just warnings but
for some reason or another they will cause server lag. To fix the problem
just ban these two weapons. 
-----

As always, if you have questions, feel free to mail me at
jblabs@aracnet.com

All versions of LOX from 1.11 to 1.12.x are maintained by QwazyWabbit.

Modifications and additions for LOX v1.12.x by QwazyWabbit<WOS> 
windowmeister@hotmail.com

See the changelog.txt file for QwazyWabbit's modifications and fixes
for the 1.12+ version of the mod. Portions of the changelog are also 
included in the release archive in the readme.txt file.
