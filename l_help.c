#include "g_local.h"
#include "l_help.h"

char* Info_Man_Text1 = {
	"\nMan (Online Manual)---------------------------------------\n"
	"Command: man \"command, weapon or feature\"\n"
	"Help is available for: airstrike,\n"
	"angels, anticamper, anti-flaregun, baton, bazooka, bazooka sweeper,\n"
	"blindness grenade, buckyball launcher,  cataclysm device, chasecam,\n"
	"checkstats, chunkgun, cloak, cluster grenade,drop, decoy, detonate,\n"
	"day, night, disruptor, doubleimpact, durggun, explosive,\n"
};

char* Info_Man_Text2 = {
	"energy vortex, flashlight, feign, fph, firemode, flaregun, freezer,\n"
	"freezegun, freeze grenade, flamethrower, fbfg, grapple,\n"
	"grenade, guided missile, height, hook, homer grenade,\n"
	"homing rocket launcher, holoquad, holoinvul, holobfg, holoweapon,\n"
	"invis, icecube gun, kamikaze, lbind, lox, lsight, laser, logplayers,\n"
	"lightning gun, mace, machine rocket gun, machinegun, midnight grenade,\n"
	"nightvision, "
};

char* Info_Man_Text3 = {
	"nailgun, organic, push, pull, playerid, plasma grenade,\n"
	"plasma cluster grenade, plasma rifle, positron,\n"
	"positron grenade, rangefinder, revenge, railgun sweeper,\n"
	"rocket grenade, railbomb, scanner, score, sweeperfiremode,\n"
	"sweepers, superblaster, snipergun, shrapnel grenade,\n"
	"sticking grenade, skipper homing rocket, slugarmor,\n"
	"spinning rail grenade, tracker, trackercount, turret, turretcount,\n"
	"teleport grenade, time, timeleft, timeall, ver\n"
	"Put multi-word commands in quotes.\n"
	"------------------------------------------------\n"
};

char* Info_Airstrike_Text = {
	"\nAirstrike---------------------------------------\n"
	"Command: Airstrike\n"
	"Usage: Bind airstrike to a key.\n"
	"Point to the sky and call the airstrike. Then point\n"
	"to the target that you want to destroy. When it\n"
	"arrives the target will be pounded with several\n"
	"rockets. It takes 5 seconds for the airstrike to\n"
	"arrive, if it doesn't get intercepted.\n"
	"------------------------------------------------\n"
};

char* Info_Angel_Text = {
	"\nAngels--------------------------------------\n"
	"There are several angels available in WoD:LOX.\n"
	"Each gives the player a different edge over\n"
	"the other players and players should plan their\n"
	"strategies accordingly. There are ten angels\n"
	"currently available:\n"
 	"	angelol - Angel of Life\n"
 	"	angelod - Angel of Death\n"
 	"	angelom - Angel of Mercy\n"
 	"	angelof - Angel of Frost\n"
	"	angeloflame - Angel of Flame\n"
 	"	angelop - Angel of Plague\n"
 	"	angeloblind - Angel of Blindness\n"
 	"	angelohorror - Angel of Horror\n"
 	"	angelor - Angel of Revenge\n"
 	"	angeloe - Angel of Energy\n"
 	"	angelou - Angel of Unfreeze\n"
	"--------------------------------------------\n"
};

char* Info_AngelOB_Text = {
	"\nAngel Of Flame----------------------------------\n"
	"Command: angeloflame\n"
	"Usage: bind angeloflame to a key or type it into the\n"
	"console. When you do want to use it press the bound\n"
	"key. The Angel Of Flame will set a person on fire\n"
	"when they are close enough to you.\n"
	"Angel of Life protects against Angel of Flame.\n"
	"------------------------------------------------\n"
};

char* Info_AngelOBlind_Text = {
	"\nAngel Of Blindness------------------------------\n"
	"Command: angeloblind\n"
	"Usage: bind angeloblind or angelob to a key or type\n"
	"it into the console. When you do want to use it press\n"
	"the key. The Angel Of Blindness will cause the affected\n"
	"person to go blind for a short while when they are\n"
	"close enough to you.\n"
	"------------------------------------------------\n"
};

char* Info_AngelOD_Text = {
	"\nAngel Of Death----------------------------------\n"
	"Command: angelod\n"
	"Usage: Bind angelod to a key or type it into the\n"
	"console. When you do want to use it press the bound\n"
	"key. The Angel Of Death will shoot in parallel with\n"
	"your shot in the direction you are aiming.\n"
	"------------------------------------------------\n"
};

char* Info_AngelOE_Text = {
	"\nAngel Of Energy----------------------------------\n"
	"Command: angeloe\n"
	"Usage: bind angeloe to a key or type it into the\n"
	"console. When you do want to use it press the bound\n"
	"key. The Angel Of Energy will catch energy bolts and\n"
	"plasma rays when they are close enough to you.\n"
	"------------------------------------------------\n"
};

char* Info_AngelOF_Text = {
	"\nAngel Of Frost----------------------------------\n"
	"Command: angelof\n"
	"Usage: bind angelof to a key or type it into the\n"
	"console. When you do want to use it press the bound\n"
	"key. The Angel Of Frost will freeze a person when\n"
	"they are close enough to you.\n"
	"Angel of Mercy or Angel of UnFreeze protects against Angel of Frost.\n"
	"------------------------------------------------\n"
};

char* Info_AngelOH_Text = {
	"\nAngel Of Horror---------------------------------\n"
	"Command: angelohorror\n"
	"Usage: bind angelohorror or angeloh to a key or type\n"
	"it into the console. When you do want to use it press\n"
	"the bound key. The Angel Of Horror will cause the\n"
	"affected person to see colors at random when they are\n"
	"in range of the orbiting angel.\n"
	"------------------------------------------------\n"
};

char* Info_AngelOL_Text = {
	"\nAngel Of Life-----------------------------------\n"
	"Command: angelol\n"
	"Usage: bind angelol to a key or type it into the\n"
	"console. When you want to use it press the bound\n"
	"key. The Angel Of Life will pick up items for you\n"
	"whenever you are near enough to them.\n"
	"------------------------------------------------\n"
};

char* Info_AngelOM_Text = {
	"\nAngel Of Mercy----------------------------------\n"
	"Command: angelom\n"
	"Usage: bind angelom to a key or type it into the\n"
	"console. When you want to use it press the bound\n"
	"key. The Angel Of Mercy will catch projectiles for\n"
	"you, sometimes.\n"
	"Angel of Mercy also protects against Angel of Frost.\n"
	"------------------------------------------------\n"
};

char* Info_AngelOP_Text = {
	"\nAngel Of Plague---------------------------------\n"
	"Command: angelop\n"
	"Usage: bind angelop to a key or type it into the\n"
	"console. When you want to use it press the bound\n"
	"key. When an opponent comes within range the\n"
	"Angel Of Plague rapidly depletes his health until he\n"
	"has only 10 health points left, making him an easy\n"
	"frag.\n"
	"Flying the Angel of Life makes you immune to plague.\n"
	"------------------------------------------------\n"
};

char* Info_AngelOR_Text = {
	"\nAngel Of Revenge--------------------------------\n"
	"Command: angelor\n"
	"Usage: bind angelor to a key or type it into the\n"
	"console. When you do want to use it press the bound\n"
	"key. The Angel Of Revenge rapidly depletes a player's\n"
	"health when you are close enough to him.\n"
	"The angel will take off health until they have 50 health\n"
	"points left. Very effective against regen if you can get close.\n"
	"Flying the Angel of Revenge makes you immune to revenge.\n"
	"------------------------------------------------\n"
};

char* Info_AngelOU_Text = {
	"\nAngel Of Unfreeze----------------------------------\n"
	"Command: angelou\n"
	"Usage: bind angelou to a key or type it into the\n"
	"console. When you do want to use it press the bound\n"
	"key. The Angel Of Unfreeze will unfreeze you immediately\n"
	"and give you some protection against freezing weapons.\n"
	"Angel of Unfreeze protects against Angel of Frost.\n"
	"------------------------------------------------\n"
};

char* Info_Baton_Text = {
	"\nBaton-------------------------------------------\n"
	"Command: baton\n"
	"Usage: bind baton to a key or type it into the\n"
	"console. When you do want to use it press the bound\n"
	"key you will throw another type of grenade that is\n"
	"sort of like a pipe bomb but with a twist. It costs\n"
	"one frag to throw it and if you shoot one down you\n"
	"get a frag for it.\n"
	"------------------------------------------------\n"
};

char* Info_Cloak_Text = {
	"\nCloak-------------------------------------------\n"
	"Command: cloak\n"
	"Usage: bind cloak to a key or type it into the\n"
	"console. When you do want to use it press the bound\n"
	"key. This feature is for all of you that hate those\n"
	"pesky homing weapons. Your blip will disappear from\n"
	"the scanner, homing weapons won't home on you and\n"
	"your homing weapons won't home on targets. Downside - you\n"
	"will be surrounded by a green shell.\n"
	"------------------------------------------------\n"
};

char* Info_Checkstats_Text = {
	"\nCheckstats--------------------------------------\n"
	"Command: checkstats\n"
	"Usage: bind checkstats to a key or type it into the\n"
	"console. Have you ever wanted to know how your targets\n"
	"are fairing? Well if you press the bound key you can\n"
	"see how much health and how far away they are.\n"
	"------------------------------------------------\n"
};

char* Info_Chasecam_Text = {
	"\nChasecam----------------------------------------\n"
	"Command: chasecam\n"
	"Usage: bind chasecam to a key or type it into the\n"
	"console. Allows you to play quake2 as if it was a 3rd\n"
	"person game. It's pretty cool, but personally quake2\n"
	"is a 1st person shooter and that's how we like to\n"
	"play it.\n"
	"------------------------------------------------\n"
};

char* Info_Drop_Text = {
	"\nDrop--------------------------------------------\n"
	"Command: drop <item or weapon>\n"
	"Usage: use this command to drop a weapon or item that\n"
	"you no longer want to you use or that your teammate\n"
	"needs. This function is disabled in unlimited ammo mode.\n"
	"------------------------------------------------\n"
};

char* Info_Decoy_Text = {
	"\nDecoy-------------------------------------------\n"
	"Command: decoy\n"
	"Usage: bind decoy to a key or type it into the\n"
	"console. When you do want to use it press the bound\n"
	"key. The decoy is most effective when someone is\n"
	"chasing you and you are very low in health. Just\n"
	"activate it and confuse your enemy. If your enemy\n"
	"is also low in life there is the chance that they\n"
	"could be killed by your decoy.\n"
	"------------------------------------------------\n"
};

char* Info_Detonate_Text = {
	"\nDetonate----------------------------------------\n"
	"Command: detonate\n"
	"Usage: bind detonate to a key or type it into the\n"
	"console. When you want to use it press the bound\n"
	"key, but be sure that you have an explosive out.\n"
	"What are explosives? Well then you should check out\n"
	"the explosives command as well.\n"
	"------------------------------------------------\n"
};

char* Info_Explosive_Text = {
	"\nExplosive---------------------------------------\n"
	"Command: explosive\n"
	"Usage: bind explosive to a key or type it into the\n"
	"console. The explosive command is a toggle. Press the\n"
	"bound key to activate them, press it again to deactivate.\n"
	"When explosive is active the grenade launcher fires\n"
	"explosive charges. Detonate the explosives by using the\n"
	"detonate command. Launching a second explosive\n"
	"before detonating the first will detonate the first.\n"
	"------------------------------------------------\n"
};

char* Info_Flashlight_Text = {
	"\nFlashlight--------------------------------------\n"
	"Command: flashlight\n"
	"Usage: Bind flashlight to a key or type it into the\n"
	"console. Press the bound key to toggle the flashlight\n"
	"on and off. Use it to light up those dark corners that\n"
	"your enemies camp in.\n"
	"------------------------------------------------\n"
};

char* Info_FeignDeath_Text = {
	"\nFeign Death-------------------------------------\n"
	"Command: feign\n"
	"Usage: bind feign to a key or type it into the\n"
	"console. Use the bound key to toggle the feign\n"
	"mode. Now you too can play opossum with your\n"
	"enemy.\n"
	"------------------------------------------------\n"
};

char* Info_FPH_Text = {
	"\nFPH-------------------------------------------\n"
	"Command: fph\n"
	"Usage: bind fph to a key or type it into the\n"
	"console. This toggles the frags per hour HUD item\n"
	"on or off.\n"
	"------------------------------------------------\n"
};

char* Info_FireMode_Text = {
	"\nFiremode----------------------------------------\n"
	"Command: firemode\n"
	"Usage: bind firemode to a key or type it into the\n"
	"console. If you have the regular machine gun selected,\n"
	"'cmd firemode' will switch between fully-automatic\n"
	"mode (the usual) and a new burst-fire mode.  It'll\n"
	"shoot 6 bullets in a quick burst. Surprisingly effective!\n"
	"------------------------------------------------\n"
};

char* Info_Height_Text = {
	"\nHeight------------------------------------------\n"
	"Command: height\n"
	"Usage: bind height to a key or type it into the\n"
	"console. Press the bound key to toggle the feature.\n"
	"Ever want to know if that jump from one height to a\n"
	"lower height will hurt you? Well now you can see how\n"
	"high up in the air you are.\n"
	"------------------------------------------------\n"
};

char* Info_Hook_f_Text = {
	"\nHook--------------------------------------------\n"
	"Command: +hook, -hook\n"
	"Usage: bind +hook and to a key or type it into the\n"
	"console. Press the bound key to activate it. This is\n"
	"not a glorified flying machine, like the grapples in\n" 
	"other mods. Not only do you swing around as you use it,\n" 
	"but you can seriously hurt yourself if you use it wrong.\n"
	"You can also make the line grow or shrink at will.\n"
	"You will need these aliases to use the hook:\n"
	"alias +hook \"hook action; hook shrink\"\n"
	"alias -hook \"hook stop\"\n"
	"alias +shrink \"hook shrink\"\n"
	"alias -shrink \"hook stop\"\n"
	"alias +grow \"hook grow\"\n"
	"alias -grow \"hook stop\"\n"
	"then bind your favorite hook key to \"+hook\"\n"
	"bind other keys to the +shrink and +grow aliases to use them.\n"
	"------------------------------------------------\n"
};


char* Info_NewCloak_Text = {
	"\nInvisibility------------------------------------\n"
	"Command: invis\n"
	"Usage: bind invis to a key or type it into the\n"
	"console. Press the bound key to toggle it. This is\n"
	"our second in the cloaking family. When you use it\n"
	"you become invisible to your opponents but your blip\n" 
	"is still on the scanner and the homing weapons can\n"
	"still home on you. This is the ultimate sniper tool.\n"
	"------------------------------------------------\n"
};


char* Info_Kamikaze_Text = {
	"\nKamikaze---------------------------------------\n"
	"Command: kamikaze\n"
	"Usage: bind kamikaze to a key or type it into the\n"
	"console. Press the bound key to activate it. When\n"
	"all  *really* fails.  This isn't entirely a\n"
	"weapon, since it kills you, so it is intended for\n"
	"deathmatch only. It sends a big explosion from\n"
	"where you die...kind of like strapping a huge timebomb\n" 
	"on your chest and setting it off.\n"
	"------------------------------------------------\n"
};


char* Info_LBind_Text = {
	"\nLox Bind--------------------------------------\n"
	"Command: lbind\n"
	"Usage: lbind (ondeath|onrespawn|onenemydeath) <command(s)>\n"
	"If more than one command is used they must be enclosed\n"
	"in quotes. By using LBind effectively you can re-setup\n"
	"your playing environment if you happen to die,\n"
	"i.e. bring up the scanner, cloak yourself, choose\n"
	"your favorite angel, etc.\n"
	"-----------------------------------------------\n"
};

char* Info_LOX_Text = {
	"\nLOX-----------------------------------------\n"
	"LOX - Something that you put on a bagel with\n"
	"cream cheese and onions. An obsessive quake2\n"
	"player. A clan made up of programmers who\n"
	"were really blood thirsty. Well all of them\n"
	"fit the word. LOX is this mod, Lethal Orifice\n"
	"eXamination. This mod is a derivative of several\n"
	"mods and is most like WOD.\n"		
	"--------------------------------------------\n"
};

char* Info_LaserSight_Text = {
	"\nLaser Sight-------------------------------------\n"
	"Command: lsight\n"
	"Usage: bind lsight to a key or type it into the\n"
	"console. Press the bound key to toggle it. A little\n" 
	"red ball will dance in front of you, showing you\n"
	"where your gun is pointed.  It'll glow brighter\n" 
	"once it finds a target. But be careful, everyone\n" 
	"else can see it too.\n"
	"------------------------------------------------\n"
};

char* Info_PlaceLaserTripwire_Text = {
	"\nLaser Tripwires---------------------------------\n"
	"Command: laser\n"
	"Usage: bind laser to a key or type it into the\n"
	"console. Press the bound key to place one. Much\n" 
	"like the Duke Nukem 3D laser-tripped grenades.\n"  
	"Set up a darKMajick grenade to fire when they\n"  
	"walk through a laser beam. The wall-mounted\n"
	"(floor-mounted, ceiling-mounted, door-mounted,\n" 
	"whatever) grenade can also be shot at until it\n" 
	"blows up. Takes 5 cells, plus the grenades.\n" 
	"------------------------------------------------\n"
};

char* Info_LogPlayers_Text = {
	"\nLogplayers--------------------------------------\n"
	"Command: logplayers\n"
	"Usage: bind logplayers to a key or type it into the\n"
	"console. This command will log the current players\n" 
	"in the game, their IP addresses, pings, current map, etc.\n" 
	"This is useful for loggingthose pesky players who\n" 
	"like to spoof peoples names.\n" 
	"------------------------------------------------\n"
};


char* Info_Lowlight_Text = {
	"\nNight Vision------------------------------------\n"
	"Command: nightvision\n"
	"Usage: bind nightvison to a key or type it into\n"
	"the console. Press the bound key to toggle it.\n" 
	"It only works right in OpenGL (3dfx) mode.\n"  
	"------------------------------------------------\n"
};

char* Info_Tractor_Text = {
	"\nTractor Beam------------------------------------\n"
	"Command: push, pull\n"
	"Usage: bind push or pull to a key or type it into\n"
	"the console. Press the bound keys to activate them.\n" 
	"When all  fails, use The Force. Pull player or\n" 
	"monster toward you or push them away.\n"  
	"------------------------------------------------\n"
};

char* Info_PlayerID_Text = {
	"\nPlayer ID---------------------------------------\n"
	"Command: playerid\n"
	"Usage: bind playerid to a key or type it into the\n"
	"console. Press the bound key to toggle the feature.\n"
	"Just point your cross hairs at an enemy and you'll\n" 
	"get the name, model and skin they are using.\n"
	"------------------------------------------------\n"
};


char* Info_Rangefinder_Text = {
	"\nRangefinder-------------------------------------\n"
	"Command: range\n"
	"Usage: bind range to a key or type it into the\n"
	"console. Press the bound key to toggle the feature.\n"
	"This is a permanent addition to the quake2 HUD. By\n" 
	"using this, nice little tool, you'll be able to tell\n" 
	"how far away your target is and how much of a lob\n" 
	"you need to use to hit them with your Plasma Cluster\n" 
	"grenade.\n" 
	"------------------------------------------------\n"
};

char* Info_Scanner_Text = {
	"\nScanner-----------------------------------------\n"
	"Command: scanner\n"
	"Usage: bind scanner to a key or type it into the\n"
	"console. Press the bound key to toggle the feature.\n"
	"Track players down in DM.\n" 
	"------------------------------------------------\n"
};

char* Info_Score_Text = {
	"\nScore-------------------------------------------\n"
	"Command: score\n"
	"Usage: bind score to a key or type it into the\n"
	"console. Press the bound key to toggle the feature.\n"
	"Use it to check who is in the lead and how many\n" 
	"frags everyone in the game has.\n"
	"------------------------------------------------\n"
};

char* Info_Skinlist_Text = {
	"\nSkinlist-------------------------------------------\n"
	"Command: skinlist\n"
	"Usage: type skinlist in the console.\n"
	"This command causes the server to list the client\n"
	"number, name and skin of every player in the game.\n" 
	"This command replaces the client skins command.\n"
	"------------------------------------------------\n"
};
char* Info_SweeperFireMode_Text = {
	"\nSweeper Fire Mode-------------------------------\n"
	"Command: sweeperfiremode\n"
	"Usage: bind sweeperfiremode to a key or type it\n"
	"in the console. Press the bound key to toggle\n"
	"the feature. Ever wondered what would happen if\n"
	"you could increase the fire rate of the railgunsweeper\n"
	"well, now you can. But if you increase the speed in\n" 
	"which the sweepers fire you will use more ammo and also\n" 
	"will increase the temperature of the weapon which might\n"  
	"cause it to explode.\n" 
	"------------------------------------------------\n"
};

char* Info_Tracker_Text = {
	"\nTracker-----------------------------------------\n"
	"Command: tracker\n"
	"Usage: bind tracker to a key or type it\n"
	"in the console. Press the bound key to throw\n"
	"one. Well we did it again, we added a WoD:LOX\n" 
	"exclusive feature. Now you might be asking\n"
	"yourselves, what is so original about the trackers,\n" 
	"aren't they just like normal turrets? Well no\n"
	"they're not. When you drop one it will look like\n" 
	"a skull gib, depending on what weapon you were\n" 
	"using at the time is what it will shoot.\n"
	"------------------------------------------------\n"
};

char* Info_TrackerCount_Text = {
	"\nTrackercount-----------------------------------\n"
	"Command: trackercount\n"
	"This toggles the trackercount HUD display on or off.\n"
	"Type trackercount into the console and if you have\n"
	"any trackers deployed it will display the number of\n" 
	"trackers in the upper right corner of the display.\n"
	"When there are no trackers out it displays nothing.\n"
	"------------------------------------------------\n"
};

char* Info_Turret_Text = {
	"\nTurret-----------------------------------------\n"
	"Command: turret\n"
	"Usage: bind turret to a key or type it\n"
	"in the console. Press the bound key to throw\n"
	"one. Well we did it again, we added a LOX\n" 
	"exclusive feature. Now you might be asking\n"
	"yourselves, what is so original about the LOX turrets,\n" 
	"aren't they just like normal turrets? Well no\n"
	"they're not. When you drop one it will look like\n" 
	"a skull gib, depending on what weapon you were\n" 
	"using at the time is what it will shoot.\n"
	"------------------------------------------------\n"
};

char* Info_TurretCount_Text = {
	"\nTurretcount-----------------------------------\n"
	"Command: turretcount\n"
	"This toggles the turretcount HUD display on or off.\n"
	"Type turretcount into the console and if you have\n"
	"any turrets deployed it will display the number of\n" 
	"turrets in the upper right corner of the display.\n"
	"When there are no turrets out it displays nothing.\n"
	"------------------------------------------------\n"
};

char* Info_Ver_Text = {
	"\nVersion-----------------------------------------\n"
	"Command: ver\n"
	"Usage: type ver in the console.\n"
	"The server prints the version of the game and some banner\n" 
	"text when this command is sent from a client to a server.\n" 
	"------------------------------------------------\n"
};

char* Info_AntiCamper_Text = {
	"\nAnti-Camper-------------------------------------\n"
	"Ever get really tired of all those campers? Well\n" 
	"I have a solution for you. When you set the dmflags\n" 
	"to enable this feature campers will have one warning\n"  
	"before they are executed by the mod. Campers beware.\n" 
	"------------------------------------------------\n"
};

char* Info_DayNight_Text = {
	"\nDay Night---------------------------------------\n"
	"You can now simulate the passing of time in WOD:LOX.\n"  
	"Enable this feature by setting nightdaytime. A full\n"
	"day-night-day rotation is based on the server nightdaytime\n"
	"value. If nightdaytime is 0, the cycle is disabled.\n" 
	"------------------------------------------------\n"
};

char* Info_Revenge_Text = {
	"\nThe Revenge Factor------------------------------\n"
	"Imagine, you are the person with the highest frag\n" 
	"and everybody is looking for you, not because they\n" 
	"miss you. But because you were the last person to\n" 
	"kill them and because you are the person with the\n" 
	"highest frag count. Sounds like fun huh. If you frag\n" 
	"the person who fragged you last its worth two frag\n" 
	"points to you. If you frag the person with the most\n" 
	"frags then they are worth three frag points. So listen\n"  
	"up kiddies this is where it gets fun. If the person\n" 
	"with the most frags was also the person who fragged\n" 
	"you last and then you frag them you get a whooping\n" 
	"five frags. Sounds like fun huh? Well that should\n" 
	"put an end to those pesky campers.\n"  
	"------------------------------------------------\n"
};

char* Info_Sweepers_Text = {
	"\nSweepers----------------------------------------\n"
	"In WoD:LOX we have added more sweepers. These sweepers\n"
	"are mode powerful and deadly than anything before.\n"  
	"These sweepers in combination with the sweeperfiremode\n"
	"command will make any player almost invincible. Currently\n" 
	"there are 9 sweepers they are the:\n" 
	"	streetsweeper\n"
	"	superblastersweeper\n"
	"	freezersweeper\n"
	"	grenadesweeper\n"
	"	bazookasweeper\n"
	"	railgunsweeper\n"
	"	flamesweeper\n"
	"	chunkgun.\n"
	"------------------------------------------------\n"
};

char* Info_BazookaSweeper_Text = {
	"\nBazookasweeper----------------------------------\n"
	"Another alternate chaingun. Fires a steady stream of\n"  
	"regular grenades.\n"
	"------------------------------------------------\n"
};

char* Info_StreetSweeper_Text = {
	"\nStreetSweeper-----------------------------------\n"
	"The alternate chain gun.  Fires a steady stream of\n" 
	"shotgun shells.  Tends to clear the way -- just\n"
	"charge through a group of enemies and watch them\n"  
	"disappear.\n" 
	"------------------------------------------------\n"
};

char* Info_SuperblasterSweeper_Text = {
	"\nSuperblasterSweeper-----------------------------\n"
	"Another alternate chain gun. Fires a steady stream\n" 
	"of superblaster bolts.\n" 
	"------------------------------------------------\n"
};

char* Info_FreezerSweeper_Text = {
	"\nFreezerSweeper----------------------------------\n"
	"Another alternate chain gun. Fires a steady stream\n"
	"of freezer bolts.\n" 
	"------------------------------------------------\n"
};

char* Info_GrenadeSweeper_Text = {
	"\nGrenadeSweeper----------------------------------\n"
	"Another alternate chain gun. Fires a steady stream\n"
	"of regular grenades.\n" 
	"------------------------------------------------\n"
};

char* Info_RailgunSweeper_Text = {
	"\nRailgunsweeper----------------------------------\n"
	"Another alternate chain gun. Fires a steady stream\n" 
	"of rails. One of the Lopez's and Gametokens favorite\n"
	"weapons. It's a shower of death on your enemies.\n"
	"------------------------------------------------\n"
};

char* Info_ChunkGun_Text = {
	"\nChunkGun----------------------------------------\n"
	"Another alternate chain gun. Do you guys remember\n"
	"the nice gib gun back in quake. Well because of\n"
	"Z'ha'dums insistence we had to bring it back but with\n"
	"a twist. Shoot your enemies or shoot the gibs on the\n"
	"ground. When someone gets hit by them or steps on\n"
	"them they will get infected and hurt. This is a\n"
	"great weapon to isolate a room with.\n"
	"------------------------------------------------\n"
};

char* Info_FlameSweeper_Text = {
	"\nFlamesweeper------------------------------------\n"
	"Another alternate chain gun. This is an improved\n"
	"version of the flamethrower. It has a shorter range\n"
	"but it causes more damage.\n" 
	"------------------------------------------------\n"
};

char* Info_Superblaster_Text = {
	"\nSuperblaster------------------------------------\n"
	"The alternate blaster.  Fires a red bolt with a\n"
	"sparkling red trail and does quite a lot of damage.\n"  
	"Uses 5 cells per shot.\n"
	"------------------------------------------------\n"
};

char* Info_FlareGun_Text = {
	"\nFlare Gun---------------------------------------\n"
	"Another alternate to the blaster. Fires a flare\n"
	"into the air that will slowly drift to the ground.\n"
	"------------------------------------------------\n"
};

char* Info_AntiFlareGun_Text = {
	"\nAnti-Flare Gun--------------------------------\n"
	"Ever find the perfect camping spot but it was too\n"
	"well lit for camping? Well, here is a tool in the\n"
	"TheLopez's bag of tricks that will allow you to\n"
	"shade those brightly lit corners of the maps.\n"
	"Anti-Flares though will stick to walls instead of\n"
	"bouncing off them like regular flares.\n"
	"------------------------------------------------\n"
};


char* Info_Mace_Text = {
	"\nMace--------------------------------------------\n"
	"Use this weapon to blind your opponents.\n"
	"------------------------------------------------\n"
};

char* Info_SniperGun_Text = {
	"\nSniper Gun--------------------------------------\n"
	"The alternate shotgun. Fires a very powerful bolt\n"
	"that usually gibs your opponent, but it's very slow\n"
	"to reload and takes 5 shells per shot.\n"
	"------------------------------------------------\n"
};

char* Info_FreezeGun_Text = {
	"\nFreeze Gun--------------------------------------\n"
	"The alternate supershotgun. Freezes your opponent\n" 
	"in place so that you can dispatch them with\n"
	"conventional weaponry. Or just freeze them to death,\n"
	"if you like. The more you freeze them, the longer\n"
	"they stay frozen. Uses 10 cells per shot.\n"
	"------------------------------------------------\n"
};

char* Info_MachineRocketGun_Text = {
	"\nMachine Rocket Gun------------------------------\n"
	"The alternate machine gun, and everyone's favorite\n"
	"it seems.  Instead of bullets, this fires a steady\n"
	"stream of rockets. They're not as powerful as normal\n"
	"rockets, but then you can't bombard a whole area\n"
	"with normal rockets. Uses 2 bullets per shot.\n" 
	"------------------------------------------------\n"
};

char* Info_MachineGun_Text = {
	"\nMachine Gun-------------------------------------\n"
	"The conventional Quake2 machine gun.\n"
	"If you have the regular machine gun selected, 'cmd\n"
	"firemode' will switch between fully-automatic mode\n"
	"(the usual) and a new burst-fire mode. It'll shoot\n"
	"6 bullets in a quick burst. Surprisingly effective!\n"
	"------------------------------------------------\n"
};

char* Info_Nailgun_Text = {
	"\nNail Gun----------------------------------------\n"
	"Just as the name says. You get to fire nails at\n"
	"people. Hehehe. Have fun and nail your enemies\n"
	"to death.\n"
	"------------------------------------------------\n"
};

char* Info_Grenades_Text = {
	"\nGrenades----------------------------------------\n"
	"In WoD, new grenades were introduced to quake2.\n"
	"Then came the new grenades introduced by WoD:LOX.\n"
	"The WoD and WoD:LOX grenades are:\n"
	"   cluster grenade,\n"
	"   rail bomb,\n" 
	"   plasma grenade,\n"
	"   napalm grenade,\n" 
	"   shrapnel grenade,\n" 
	"   cataclysm device,\n" 
	"   plasma cluster grenade,\n" 
	"   freeze grenade,\n" 
	"   teleport grenade,\n" 
	"   homer grenade,\n" 
	"   rocket grenade,\n"
	"   blindness grenade,\n"
	"   midnight grenade,\n"
	"   positron grenade,\n"
	"   freezatron grenade,\n"
	"   banzai grenade,\n"
	"   spinning rail grenade.\n"
	"------------------------------------------------\n"
};

char* Info_ClusterGrenade_Text = {
	"\nCluster Grenade---------------------------------\n"
	"Explodes into several other grenades.\n"
	"------------------------------------------------\n"
};

/* Floating mine launcher:
Bind a key to 'use floating mine launcher'. 
This is a bazooka that launches grenades that float in place 
after flying a certain distance. The grenade type depends on 
the last grenade type selected. This is a great weapon for 
mining the entrances of tunnels or traffic choke points. 
You are immune to your own mines.
*/

char* Info_RailBomb_Text = {
	"\nRail Bomb---------------------------------------\n"
	"Explodes into several railgun shots.\n"
	"------------------------------------------------\n"
};

char* Info_PlasmaGrenade_Text = {
	"\nPlasma Grenade----------------------------------\n"
	"Explodes into a BFG shot.\n"
	"------------------------------------------------\n"
};

char* Info_NapalmGrenade_Text = {
	"\nNapalm Grenade----------------------------------\n"
	"Explodes into a bunch of fire and burning explosions.\n"
	"------------------------------------------------\n"
};

char* Info_ShrapnelGrenade_Text = {
	"\nShrapnel Grenade--------------------------------\n"
	"Explodes into a bunch of flaming shrapnel.\n"
	"------------------------------------------------\n"
};

char* Info_CataclysmDevice_Text = {
	"\nCataclysm Device--------------------------------\n"
	"Has to be seen to be believed.  Explodes like an\n"
	"atomic bomb, complete with a shockwave that tends\n"
	"to kill everything.\n"
	"------------------------------------------------\n"
};

char* Info_PlasmaClusterGrenade_Text = {
	"\nPlasma Cluster Grenade--------------------------\n"
	"A virtual tree of death.\n"
	"------------------------------------------------\n"
};

char* Info_FreezeGrenade_Text = {
	"\nFreeze Grenade----------------------------------\n"
	"Like the Rail bomb but shoots freeze bolts instead.\n"
	"------------------------------------------------\n"
};

char* Info_TeleportGrenade_Text = {
	"\nTeleport Grenade--------------------------------\n"
	"Teleports you to whereever you aim it. Though be\n"
	"warned using it as a normal hand grenade will\n"
	"cause you pain. Use it to telefrag your favorite\n"
	"LOX player.\n"
	"------------------------------------------------\n"
};

char* Info_HomerGrenade_Text = {
	"\nHomer Grenade-----------------------------------\n"
	"Will launch a grenade that when it explodes will\n"
	"launch 4 weak homing rockets.\n"
	"------------------------------------------------\n"
};

char* Info_RocketGrenade_Text = {
	"\nRocket Grenade----------------------------------\n"
	"Will launch a grenade that when it explodes will\n"
	"launch 4 weak rockets.\n"
	"------------------------------------------------\n"
};

char* Info_BlindnessGrenade_Text = {
	"\nBlindness Grenade-------------------------------\n"
	"Will launch a grenade that when it explodes will\n"
	"blind anyone in a certain radius.\n"
	"------------------------------------------------\n"
};

char* Info_MidnightGrenade_Text = {
	"\nMidnight Grenade--------------------------------\n"
	"Ever find the perfect camping spot but it was too\n"
	"well lit for camping? The Lopez skunkworks brings\n"
	"campers another useful tool for creating cozy camping\n"
	"spots. Just throw one of these into the area where\n"
    "want to camp and several anti-flare grenades will\n"
	"be dispersed into the area.\n"
	"------------------------------------------------\n"
};

char* Info_PositronGrenade_Text = {
	"\nPositron Grenade--------------------------------\n"
	"If you like the positron you will love the positron\n"
	"grenade even more. In short, it will allow you to\n"
	"create one of those wonderful orange balls of death\n"
	"with even more pin point accuracy\n"
	"------------------------------------------------\n"
};

char* Info_FreezatronGrenade_Text = {
	"\nFreezatron Grenade--------------------------------\n"
	"If you like the positron you will love the freezatron\n"
	"grenade even more. In short, it will allow you to\n"
	"create one of those wonderful blue balls of death\n"
	"with even more pin point accuracy\n"
	"------------------------------------------------\n"
};

char* Info_BanzaiGrenade_Text = {
	"\nBanzai Grenade--------------------------------\n"
	"Ever wondered what would happen if everyone in the\n"
	"room all of a sudden went kamikaze? This grenade\n"
	"from TheLopez Skunkworks will put your wonder to rest.\n"
	"Though when you throw it run for your life not only\n"
	"because you might become the biggest target on the\n"
	"map but because you to can get by the grenade.\n"
	"------------------------------------------------\n"
};

char* Info_SpinningRailGrenade_Text = {
	"\nSpinning Rail Bomb------------------------------\n"
	"When you see this new innovation from TheLopez\n"
	"Skunkworks it is your clue to leave the room because\n"
	"it will leave no one alive.\n"
	"------------------------------------------------\n"
};

char* Info_Bazooka_Text = {
	"\nBazooka-----------------------------------------\n"
	"The alternate grenade launcher.  Instead of lobbing\n"
	"grenades, it fires them in a straight line. Like\n"
	"the grenade launcher, it fires whatever type of\n"
	"grenade you selected last.\n"
	"------------------------------------------------\n"
};

char* Info_StickingGrenade_Text = {
	"\nSticking Grenades-------------------------------\n"
	"This alternate mode for the grenade launcher\n"
	"causes any of the DM grenades to stick to whatever\n"
	"surface they hit, and I mean whatever surface they hit.\n"
	"Yes this does mean players too.\n"
	"Select your grenade type, then select the sticking\n"
	"grenade launcher. For added laughs and effectiveness\n"
	"turn on explosives mode, lay a sticky on your opponent\n"
	"and choose the time of his termination.\n"
	"------------------------------------------------\n"
};

char* Info_BuckyballLauncher_Text = {
	"\nBuckyball Launcher------------------------------\n"
	"This alternate grenade launcher will fire buckyballs\n"
	"similar to the buckyballs in the chaos mod.\n"
	"------------------------------------------------\n"
};

char* Info_DoubleImpact_Text = {
	"\nDouble Impact-----------------------------------\n"
	"Like the grenade launcher but fires two grenades\n"
	"instead of one.\n"
	"------------------------------------------------\n"
};

char* Info_HomingRocketLauncher_Text = {
	"\nHoming Rocket Launcher--------------------------\n"
	"The alternate rocket launcher.  Aim in someone's\n"
	"general direction and fire; it'll do the rest, we\n"
	"really mean it. Beware - if you become a more suitable\n"
	"target, they will home in on you!\n"
	"------------------------------------------------\n"
};

char* Info_SkipperHomingRocket_Text = {
	"\nSkipper Homing Rocket---------------------------\n"
	"Another alternate rocket launcher. Does anyone\n"
	"remember the original Wing Commander? We did, and\n"
	"we recreated the skipper torpedoes from the game.\n"
	"Basically these bad boys will cloak and uncloak\n"
	"until they hit an enemy. But watch out they have\n"
	"a mind of their own and will chase you if provoked.\n"
	"------------------------------------------------\n"
};

char* Info_GuidedMissile_Text = {
	"\nGuided Missile----------------------------------\n"
	"Another alternate to the rocket launcher. When you\n"
	"fire this rocket you will chase the rocket and guide\n"
	"it with your mouse. Another camper killer weapon.\n"
	"------------------------------------------------\n"
};

char* Info_PlasmaRifle_Text = {
	"\nPlasma Rifle------------------------------------\n"
	"The alternate hyperblaster.  Fires a beam that you\n"
	"can wave around; it tends to fry anything it touches.\n"
	"------------------------------------------------\n"
};

char* Info_LightningGun_Text = {
	"\nLightning Gun-----------------------------------\n"
	"When you use this alternate to the hyperblaster\n"
	"you can shock your enemies to death. But be warned,\n"
	"do not, and I say, do not jump into watery areas\n"
	"with this weapon.\n"
	"------------------------------------------------\n"
};

char* Info_IcecubeGun_Text = {
	"\nIcecube Gun-------------------------------------\n"
	"Another alternate to the hyperblaster. The more\n"
	"you hold it down the more damage it will do. It\n"
	"is really a cool gun.\n"
	"------------------------------------------------\n"
};

char* Info_DurgGun_Text = {
	"\nDurg Gun----------------------------------------\n"
	"Another alternate to the railgun, Drop Your Gun.\n"
	"Shoot this at your enemy and they will drop their\n"
	"current weapon. Ever get sick of those people who\n"
	"use the railgun sweeper all the time, well here's\n"
	"your chance to deprive them of their favorite wep.\n"
	"------------------------------------------------\n"
};

char* Info_FBFG_Text = {
	"\nFBFG--------------------------------------------\n"
	"An alternate to the BFG10K. When it shoots it's\n"
	"lasers it will freeze the people that it hits instead\n"
	"of killing them.\n"
	"------------------------------------------------\n"
};

char* Info_VacuumMaker_Text = {
	"\nVacuum Maker------------------------------------\n"
	"Another alternate to the BFG. When you use this\n"
	"you can bring in anything that is not attached to\n"
	"the ground. When I say everything I mean everything.\n"
	"------------------------------------------------\n"
};

char* Info_EnergyVortex_Text = {
	"\nEnergy Vortex-----------------------------------\n"
	"Launch this and run away, very, very fast. Anything that\n"
	"gets within its radius will get sucked in and crushed.\n"
	"It's pretty uncomfortable.\n"
	"------------------------------------------------\n"
};

char* Info_Disruptor_Text = {
	"\nDisruptor---------------------------------------\n"
	"All I have to say is when you see the black spots run away.\n"
	"------------------------------------------------\n"
};

char* Info_Positron_Text = {
	"\nPositron-----------------------------------------\n"
	"Ever want to envelope your enemies with a big ball\n"
	"of hell? Well now you can with this alternate of the\n"
	"hyperblaster.\n"
	"------------------------------------------------\n"
};

char* Info_Freezatron_Text = {
	"\nFreezatron---------------------------------------\n"
	"Ever want to envelope your enemies with a big ball\n"
	"of frozen hell? Well now you can with this alternate\n"
	"of the hyperblaster.\n"
	"------------------------------------------------\n"
};


char* Info_FlameThrower_Text = {
	"\nFlameThrower---------------------------------------\n"
	"An alternate to the railgun.  It has a longer range than\n"
	"the flamesweeper but it causes less damage.\n"
	"------------------------------------------------\n"
};

char* Info_Holoquad_Text = {
	"\nHoloquad----------------------------------------\n"
	"Ever want to fool your opponents? Ever want a way\n"
	"to lure them out into the open with something as\n"
	"tempting as quad damage? Well guess what, now you\n"
	"can. Just use the 'holoquad' command to drop a fake\n"
	"quad damage and let the fun begin.\n"
	"------------------------------------------------\n"
};

char* Info_Holoinvul_Text = {
	"\nHoloinvul---------------------------------------\n"
	"Ever want to fool your opponents? Ever want a way\n"
	"to lure them out into the open with something as\n"
	"tempting as invulnerability? Well guess what, now you\n"
	"can. Just use the 'holoinvul' command to drop a fake\n"
	"invulnerability and let the fun begin.\n"
	"------------------------------------------------\n"
};

char* Info_Holobfg_Text = {
	"\nHolobfg-----------------------------------------\n"
	"Ever want to fool your opponents? Ever want a way\n"
	"to lure them out into the open with something as\n"
	"tempting as the BFG? Well guess what, now you\n"
	"can. Just use the 'holobfg' command to drop a fake\n"
	"BFG10K and let the fun begin.\n"
	"------------------------------------------------\n"
};


char* Info_Holoweapon_Text = {
	"\nHoloweapon--------------------------------------\n"
	"Ever want to fool your opponents? Ever want a way\n"
	"to lure them out into the open with something as\n"
	"tempting as your current weapon? Well guess what, now you\n"
	"can. Just use the 'holoweapon' command to drop a fake\n"
	"of your current weapon, let the fun begin.\n"
	"------------------------------------------------\n"
};

char* Info_Time_Text = {
	"\nTime--------------------------------------------\n"
	"Command: time\n"
	"Lets you know what time it is on the server.\n"
	"------------------------------------------------\n"
};

char* Info_Timeleft_Text = {
	"\nTimeleft--------------------------------------------\n"
	"Command: timeleft or timeremaining\n"
	"Displays how much time remains in the game.\n"
	"------------------------------------------------\n"
};

char* Info_Timeall_Text = {
	"\nTimeall--------------------------------------------\n"
	"This is a combination of time remaining in the game\n"
	"plus the current time of day clock on the server.\n"
	"This command sends a broadcast of this information\n"
	"to all the players in the game.\n"
	"------------------------------------------------\n"
};

char* Info_OrganicArmor_Text = {
	"\nOrganic Armor-----------------------------------\n"
	"Ever wish that you had armor like the colossus in\n"
	"x-men? Well if you are lucky enough you might actually\n"
	"pick one of these up. This armor will regenerate\n"
	"itself until it reaches 500. You have a 1 in 1000\n"
	"chance in picking up this armor.\n"
	"------------------------------------------------\n"
};

char* Info_SlugArmor_Text = {
	"\nSlug Armor--------------------------------------\n"
	"Are you tired of being snipped or showered by a\n"
	"storm of slugs? Well maybe you are lucky enough to\n"
	"pick up one of these armors. You have a 1 in 1000\n"
	"chance to pick up one of these.\n"
	"------------------------------------------------\n"
};

char* Info_Voting_Text = {
	"\nVoting--------------------------------------\n"
	"Type 'vote map mapname' or 'vote map next' at the console.\n\n"
	"Players will have 30 seconds to vote yes or no to\n"
	"the selected map. The passing percentage is set in\n"
	"server configuration. If the vote passes the server\n"
	"will begin the selected new map. If the vote fails\n"
	"game play continues.\n"
	"------------------------------------------------\n"
};
