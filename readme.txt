=================================================
           Lethal Orifice Examination 
          for Quake2 - *Version 1.10*
       With additions by QwazyWabbit<WOS>
	     for Versions 1.12.1 and higher.
=================================================

Date: 07/04/2005

Author: Dave Martin 
    a.k.a The Lopez Email: opus@aracnet.com
Original Author: Steven Boswell 
    a.k.a. Hash Assassin
Original Original author: Matt Wright 
    a.k.a. PunK
Wyrm ][ Code: Josep del Rio Email : wyrm@telefragged.com

Page: 
	http://www.clanwos.org/docs/
	   - Lethal Orifice eXamination page. 

   It has the latest version of the mod and recent news 
   about the mod.  Also check out this page and ensure you have
   the latest version.

        http://www.clanwos.org



I. Forward and History of the Mod
II. Installation
III. LOX
IV. Weapons
V. Features
VI. Commands
VII. Mod Info
VIII. Credits
IX. Mod Version History

============================================
Forword and History of the Mod, By The Lopez
============================================
Thank you for choosing my mod. This is truely one of the most 
weapon packed and feature packed mods that has ever been written. 
It has no theme other than grab a weapon and completely destroy 
your enemy. Please after you play it don't email me to say well 
the mod sucks, it is just a combination of a bunch of different
mods. I already know that. We, the developement team, have gone 
into a bunch of different mods sourcecodes and found which weapons 
and features were the best.  Knowing this information we code that 
code and added it into ours. But originally this mod was not really 
a mod, it was a joke. It was a joke on the part of Clan Lox. 

During a smoke break, after a night of drinking and playing quake, 
a bunch of us started to say "wow wouldn't it be cool to have a 
chain railgun in WOD?" In my drunken stupor I said yeah that 
would be cool. So the next morning I down- loaded the WoD code 
and started to play around with it. We, Gametoken and myself
created the original sweepers that day, superblaster, freezer, 
grenade, bazooka and railgun. We put this new DLL onto our clan 
server, Monty's Meatgrinder, and started to kick peoples butts. 
Then people started to get mad cause our DLL was messing up the 
stats. We changed the name of our mod from just WoD to WoD:LOX,
Lethal Orifice eXamination.

At the time we only had one server up and the end of the school 
year was drawing near. Then one day when I was going from WoD 
server to WoD serve trying to get people to switch to WoD:LOX 
I met clan RAD. After a butt-kicking on my part I told the people 
who were there about my mod and they seemed interested.  If I 
remember right it was Deathdawg and Papatick. I think that was who was 
there from clan RAD. There might have been others but I can't remember. 
So I showed them the mod and they liked them. They set up the second 
of the WoD:LOX servers. It was because of their interest in the mod 
that kept me wanting to develop the mod even further.

After tearful goodbyes clan Lox went their seperate ways for the summer.
At home I kept developing the mod, adding new weapons and features. 
But that was not the only thing that I did. I put up the eight lox 
servers that have served use faithfully since then. More and more 
people started to join our ranks as WoD:LOX players.

============================================
LOX for the 21st Century by QwazyWabbit<WOS>
============================================

In the four years between the last release of Wod:LOX 1.11 
in September of 1999 and December 2003 the LOX mod waned along with
the popularity of Quake 2 and eventually there were only 1 or 2
servers running LOX in any form on the Internet. Part of the 
problem, it seemed to me, was the stability and general crankyness
of LOX on both the Linux and Windows platforms. If only we could
fix some of the unstable interactions of some of the features, like
the guided missles making player POV go nuts when they were intercepted
by the angel of mercy, or the fact that decoys never worked right, or
the way the server would crash every couple of hours if you didn't 
have all the right features configured or certain weapons banned.
If only...

Finally, I decided after years of enjoying the mod and enduring
all the vagaries and bugs and server crashes to actually get
off my butt and do something about it. I had the LOX source code,
a version that was purported to be LOX 1.11, in a ZIP archive. 
But what's this? I compile the code - and lo and behold
it _says_ it's LOX 1.11 and runs just fine but where are the guided
nukes and the guided darKMajick grenades? It turns out that the
only remaining source archive of the LOX code was a version that
was numbered 1.11 but was something between 1.10 and 1.11 and it was
missing a lot of features that existed in the LOX binaries that
were being distributed world wide. I had intended to make some minor
bug fixes and some tweaks to the code here and there and just improve
it just a little bit and then sit back and enjoy the new game 
and watch everyone have a good time. Little did I know.

As Dave described, LOX is mixture of WOD and lots of other cool
features and weapons from other mods that everyone thought would
be cool to have in a really insanely funny mod. The result was
some pretty weird interaction of those features and sometimes some
server-crashing incompatibilities. I come from a background in
real-time systems and robotics where if the system crashes, really
bad things happen in the real world. I couldn't just let a few bugs
remain un-smashed. My goal now is to make LOX as full-featured as possible
and to minimize the pain an admin has to endure to keep the server up
and to make LOX as smooth and stable as I possibly could. So here is 
the result of one year of effort, off and on, as I had time on weekends
and in the evenings in my motel room during the week, (I travel a 
lot in my job), finding bugs and cleaning up code and in general, 
just learning all the tricks I needed to learn about programming FPS 
games in C, without dispatchers and tasks and all the other neat things 
that make programming real-time events a little easier. I won't bore 
you with the details of my learning experience, but what started out 
being a few minor tweaks here and there became a full-on rewrite of 
what has now become over 220 source files worth of code from a variety 
of authors coding to different standards and different objectives. 
I broke a few things along the way, I fixed a few things, I added a 
few things and I think and hope that LOX is better than it was when 
I started working on it. I haven't run out of bugs or ideas yet, there 
are still more versions yet to come with added features and weapons that 
will hopefully make it even more fun.

The first few LOX 1.12.x packages installed on top of existing WoD:LOX 
installations, so LOX 1.10 or 1.11 should be your starting point 
since LOX versions 1.12.1 thru 1.12.4 are all merely DLL replacements. 
Beginning with LOX version 1.12.6 I am creating a full installation 
ZIP file that will drop everything you need to play LOX into your 
Quake 2 installation. This ZIP file contains the LOX libraries for 
Windows and for Linux and should be compatible with Windows 98, 
Windows NT 4.0, Windows XP, Fedora Core, Red Hat 9.0 and 9.1 with 
other flavors of Linux yet to be tested. Some versions of Linux 
require the Quake 2 v3.21 code be built on or for the target 
system, so Linux owners, you know what to do. Loxophilia, the 
WOS server that runs LOX for our clan is running Red Hat Enterprise 
Linux using Quake 2, v3.21 built on RH9 and LOX code built on 
Fedora Core 1. 

=======================================
TheLopez emerges from his long torpor
=======================================
Well after several years of being away from you all, I have made the
decision to lend a hand to the new crew of LOX mod developers. I am
sorry if I have ignored your emails in the past but due to the new
job that I had I felt that it would be necessary to make a clean 
break from all things quake for a while as so to get settled in my 
job and be as successful as I could. Now that I have achieved that 
goal and many others, I am ready to rejoin the ranks of the q2 world
once again. I have created a blog where the LOX community is welcome
to comment and if you want post your mindsplats regarding LOX weapon
and feature development. For all you WOS clan members, this blog is 
not intended to replace any of your forums. They are a wonderful 
place for discussion and I tip my hat to all of you for keeping my
dream alive. As for QwazyWabbit<WOS>, you have taken the LOX code to
the next level and I tip my hat to you as well. Without you LOX of
have most certainly died off a long time ago. 

As for LOX in the future and what we wanted to do originally.
When we first developed LOX we wanted to create two versions, a light
weight LOX and a complete LOX mod. The light weight LOX we envisioned
almost everyone could play without having to download a huge amount
of extra skins, sounds, etc. The heavy weight LOX could potentially
replace any and all media files in the game, making for a complete LOX
experience. Obviously the neither versions of LOX happened, instead
we got a mix of the two, which resulting in something that seemed to
make a lot of people happy ... to my surprise. It is still my dream 
to have these two versions of LOX, if it will happen I don't know.
But anyways, enough with the sappy stuff and on to LOX, enjoy it,
slaughter, be slaughtered, but most importantly, have fun.

Link to TheLopez's Skunkworks blog:
http://www.coreyanddavid.com/DavidsBlog/Skunkworks/TheLopezsSkunkworks.html

========
CTF:LOX
========

The current versions, beginning with 1.12.6 now supports CTF Extinction,
a two team variation of CTF without the flags. Red team and blue team
compete head-to-head for control of all the spawn pads in the map.
At the beginning of a map, all the pads are green, as each player 
runs over a pad the pad changes to his team's color. Once all the 
players have spawned for the first time they can only respawn on
pads belonging to their team. There can only be as many players 
on that team as there are pads of that team's color. Once all the 
pads are claimed by a team the other team must convert them to
their own color by running over them. The game is won on two levels:
total frags and extinction of the other team. To win the game you 
must convert all the pads to your color and frag all the other team's
players into extinction. Each map is played in two rounds with each
team initially spawning at opposite ends of the map and trading ends
for round two. The mode is set in a dedicated server by setting ctf
to 1 and setting some other variables. The Extinction maps need special
entities added in order to play well, these entities can be added
to any map by creating special "ent" files containing the necessary
entities and they will be added to the map when the server loads it.
Each ent file bears the name of the map it's for. For example, 
carena.bsp is the map, carena.ent is the ent file for it. The ent
files are stored in the quake2/lox/ent folder and retrieved as each
map is loaded. Some sample ent files are included with the new package.


I hope you enjoy playing this mod as much as I have and I hope this
mod will last many more years on this classic game called Quake 2.

=================================
 Manual Installation - Important
=================================

Make a directory in your Quake2 Directory, 

Say c:\quake2\LOX

("LOX" is what all the servers should be using.  It's best to keep
things consistent, so that QuakeII knows how to match up servers with
what you have installed on your machine.)

Unzip the mod using winzip -or- pkunzip -d LOX.zip
Unzip the files straight into the LOX directory.  (If you unzip
to another directory and then move the files over, be sure you move
any hidden files too.)

Now, to start, launch Quake and type "set game lox" in the console.
You can now start a single-player game, or a deathmatch server, or 
whatever you want to do.

If you have trouble installing LOX, feel free to e-mail me.

You must have Quake2 version 3.20 to run this mod.

==========================================
Description of Weapons of Destruction:LOX
==========================================

There are four ways to enjoy Weapons of Destruction:LOX

o Single-player
  The Stroggs won't know what hit them.  To make things a little more
  fair, you might want to select Nightmare skill level -- type "skill 3"
  at your Q2 console.
o Deathmatch
  The classic way.  Get a bunch of your friends together and blow each
  other to smithereens.  Or join a public server and wreak chaos on
  total strangers.  Either way is fun.
o CTF Extinction Deathmatch
  Capture the pads for your team, while fragging all members of the 
  other team until they can't respawn, winning the match. All the weapons
  and features of Deathmatch LOX with CTF style teams.
o COOP Mode
  Your team of humans fights the Stroggs in the original game maps.
  Best played at skill level 3, the LOX weapons make quick work of the
  Stroggs. The standard Q2 weapons like rockets and grenades are enhanced
  in LOX so the Stroggs have more powerful weaponry.


Weapons of Destruction:LOX adds several new weapons and features to the
game.  All the weapons are available as "alternates" to the normal
weapons.  Once you grab a normal weapon and select it, select it again
to get the alternate weapons in rotation.  (Select it again to return to the normal
weapon.  You get the idea.)  There are 16 different types of grenades;
keep selecting grenades to run through the list.  All these weapons can
also be selected from the inventory, or with "use" commands.

As I write this (Feb. 19, 2005) the CTF and COOP modes are the least
debugged and utilized modes in LOX 1.12.7. As later versions are released
you can expect some of the bugs in these modes to disappear. For the most
action and the most stable server I recommend Deathmatch mode. If you feel
adventurous, try the CTF and COOP modes but read the Clan WOS forums first
for updated bug information or pointers to newer releases.

=======
Weapons
=======

Super Blaster: The alternate blaster.  Fires a red bolt with a sparkling
red trail and does quite a lot of damage.  Uses 5 cells per shot.

Flare Gun: Another alternate to the blaster. Fires a flare into the air 
that will slowly go to the ground.

Mace: Use this weapon to blind your opponents.

Sniper Gun: The alternate shotgun.  Fires a very powerful bolt that
usually gibs your opponent, but it's very slow to reload and takes
5 shells per shot.

Explosive Shotgun: The alternate shotgun. Fires an explosive shell at
whatever you aim at.

Explosive SuperShotgun: The alternate supershotgun.  Like the supershotgun
but fires explosive shells instead.

Freeze Gun: The alternate supershotgun.  Freezes your opponent in place
so that you can dispatch them with conventional weaponry.  Or just
freeze them to death, if you like.  The more you freeze them, the longer
they stay frozen.  Uses 10 cells per shot.

Machine Rocket Gun: The alternate machinegun, and everyone's favorite it
seems.  Instead of bullets, this fires a steady stream of rockets.
They're not as powerful as normal rockets, but then you can't bombard
a whole area with normal rockets.  Uses 2 bullets per shot.

Burst-fire machinegun: If you have the regular machinegun selected,
"cmd firemode" will switch between fully-automatic mode (the usual) and
a new burst-fire mode.  It'll shoot 6 bullets in a quick burst.
Surprisingly effective!

Nailgun: Just as the name says. You get to fire nails at people. Hehehe.
have fun and nail your enemies to death.

Explosive Chaingun: Like the chaingun but fires explosive bullets rather
than the regular bullets.

Streetsweeper: The alternate chaingun.  Fires a steady stream of shotgun
shells.  Tends to clear the way -- just charge through a group of
enemies and watch them disappear.

Explosive Streetsweeper: The alternate chaingun.  Fires a steady stream of shotgun
explosive shells.  Tends to clear the way -- just charge through a group of
enemies and watch them disappear.


SuperBlastersweeper: Another alternate chaingun. Fires a steady stream of
superblaster blots. 

Freezersweeper: Another alternate chaingun. Fires a steady stream of
freezer blots. 

Grenadesweeper: Another alternate chaingun. Fires a steady stream of
regular grenades. 

Bazookasweeper: Another alternate chaingun. Fires a steady stream of
regular grenades. 

Railgunsweeper: Another alternate chaingun. Fires a steady stream of
rails. One of the lopez's and Gametokens favorite weapons. It's a 
shower of death on your enemies.

Chunk Gun: Another alternate chaingun. Do you guys remember the nice gib
gun back in quake. Well because of Z'ha'dums insistance we had to bring 
it back but with a twist. Shoot your enemies or shoot the gibs on the ground.
When someone gets hit by them or steps on them they will get infected and hurt.
This is a great weapon to isolate a room with.

Flamesweeper: Another alternate chaingun. This is an improved version of
the flamethrower. It has a shorter range but causes more damage.

Durgsweeper: Like the Durg gun but in sweeper form instead.

Snipersweeper: Well since we had the railgunsweeper this we couldn't leave this 
one out now could we?

Sticking Grenadesweeper: Like the grenade sweeper but fires sticking grenades 
instead.

darKMajick grenades: The alternate grenades.

o Cluster grenade: Explodes into several other grenades.
o Rail bomb: Explodes into several railgun shots.
o Plasma grenade: Explodes into a BFG shot.
o Napalm grenade: Explodes into a bunch of fire and burning explosions.
o Shrapnel grenade: Explodes into a bunch of flaming shrapnel.
o Cataclysm device: Has to be seen to be believed.  Explodes like an
  atomic bomb, complete with a shockwave that tends to kill everything.
o Plasma Cluster grenade: A virtual tree of death.
o Freeze grenade: Like the Rail bomb but shoots freeze bolts instead.
o Teleport grenade: Teleports you where ever you aim it. Though be warned 
  using it as a normal hand grenade will cause you pain. Use it to telefrag
  your favorite LOX player.
o Homer grenade: Will launch a grenade that when it explodes will launch
  4 weak homing rockets.
o Rocket grenade: Will launch a grenade that when it explodes will launch
  4 weak rockets.
o Blindness Grenade: Will launch a grenade that when it explodes will 
blind anyone in a certain radius.
o Positron Grenade: Will launch a grenade that when it explodes will
create the positron ball of fire

The grenade launcher will lob whatever grenade type you selected last.

Bazooka: The alternate grenade launcher.  Instead of lobbing grenades,
it fires them in a straight line.  Like the grenade launcher, it fires
whatever type of grenade you selected last.

Sticking Grenade: Use this alternate for the grenade launcher to launch
any of the DM grenades that will stick to whatever surface they hit, and
I mean whatever surface, yes this does mean players too.

Buckyball Launcher: This alternate grenade launcher will fire buckyballs
similar to the buckyballs in the chaos mod.

Double Impact: Like the grenade launcher but fires two grenades instead of
one.

Homing Rocket Launcher: The alternate rocketlauncher.  Aim in someone's
general direction and fire; it'll do the rest, we really mean it.

Skipper Homing Rocket: Another alternate rocketlauncher. Does anyone remember
the original Wing Commander? We did, and we recreated the skipper torpedos from
the game. Basically these bad boys will cloak and uncloak until they hit an enemy.
But watch out they have a mind of their own and will chase you if provoked.

Guided Missile:  Another alternate to the rocket launcher. When you fire this
rocket you will chase the rocket and guide it with your mouse. Another camper
killer weapon.

Spiral Rocket Launcher:  This is another alternate to the rocket launcher. It
shoots three rockets. There is the main rocket in the center while the other
two spiral around the first. It makes for an interesting effect.

Plasma Rifle: The alternate hyperblaster.  Fires a beam that you can
wave around; it tends to fry anything it touches.

Lightning gun: When you use this alternate to the hyperblaster you can shock your
enemies to death. But bewarned, do not, and I say, do not jump into watery areas
with this weapon.

Icecube gun: Another alternate to the hyperblaster. The more you hold it down
the more damage it will do. It is really a cool gun.

Durg Gun: Another alternate to the railgun, Drop Your Gun. Shot this at your 
enemy and they will drop their current weapon. Ever get sick of those people
who use the railgunsweeper all the time, well here's your chance.

FBFG: An alternate to the BFG10K. When it shoots it's lasers it will freeze the
people that it hits instead of killing them

Vacuum Maker: Another alternate to the BFG. When you use this you can bring in
anything that is not attached to the ground. When I say everything I mean 
everything.

Energy Vortex: Launch this and run, very, very fast. Anything that gets within
its radius will get sucked in and crushed. It's pretty uncomfortable.

Disruptor: All I have to say is when you see the black spots run away.

Sonic Railgun: Another alternate to the railgun. The longer you hold down the
attack button the more damage it will do. There is a damage cap though of 250 
points. 

Kaminit Gun: When someone gets hit by this weapon their Kam. sequence will get
initiated. LOL. 

Wall Piercing Railgun: Ever get mad cause someone decides to hide behind a wall
when you want to rail them? Well now you can get them with this railgun.

Flame Thrower: The alternate railgun.  It has a longer range than the flamesweeper
but it causes less damage. Here are the specifics (partly
from the Napalm mod text file by Patrick Martin):

Monsters and players that catch on fire will burn for at least 5 seconds.
The following will extinguish the fire:
  * The player immerses in liquid at waist deep.
    Crouching can help if the liquid is too shallow.
  * The player picks up adrenaline or megahealth.
  * The player activates an invulnerability powerup.
  * Health box has a percent chance equal to its power of
    extinguishing the fire.  If the fire is not extinguished,
    then it will speed up the time when the fire burns out.
  * A bio-suit greatly reduces the chances of catching on fire
    and offers partial immunity to indirect fire damage.
  * Active power armor reduces the chance of igniting.
  * If a fire "ignites" a target already on fire, then the
    time before the fire burns out is reset.
  * All frags go to the entity who ignited the target most recently.
  * Some monsters, such as tanks or bosses (except the makron), cannot burn.

The flame thrower uses 1 slug per fire and fires quite rapidly.
The standard railgun now uses 4 slugs per shot.  All slug quantities
and capacities have been boosted by 4 times, to account for this.

There are a few more weapon-like features not done with "alternate"
weapons.

Laser Tripwires: Much like the Duke Nukem 3D laser-tripped grenades.
Set up a darKMajick grenade to fire when they walk through a laserbeam.
The wall-mounted (floor-mounted, ceiling-mounted, door-mounted,
whatever) grenade can also be shot at until it blows up.  Takes 5 cells,
plus the grenades.

Tractor Beam: When all else fails, use The Force.  Pull player or
monster toward you or push them away.

Kamikaze: When all else *really* fails.  This isn't entirely a weapon,
since it kills you, so it is intended for deathmatch only.  It sends a
big explosion from where you die...kinda like strapping a huge timebomb
on your chest and setting it off.

Finally, some of the standard weapons have been modified:

The blaster fires red or blue lasers.

The super shotgun now has a big kickback.  Shoot someone and they go
flying; if the shot doesn't kill them, maybe they'll die when they slam
into the wall.

The rockets from the standard rocket launcher, and hand-thrown grenades,
will spray fire when they explode.

When you grab a machinegun, you get the machine-rocket gun first.
Similarly, when you grab a railgun, the flamethrower comes up first.

========
Features
========

Swinging Grappling hook: This is not a glorified flying machine, like
the grapples in other mods.  Not only do you swing around as you use
it, but you can seriously hurt yourself if you use it wrong.  You can
can also make the line grow or shrink at will.

Decoy: Sort of like the Hologram in Duke3D, except it shoots at players.

Scanner: Track players down in DM. (LOX MODIFICATION) Well the original
scanner was pretty good, but now we made it better. It is relocatible now.
If you just use the scanner as is it will stay in the center but, if you
do scanner <xpos> <ypos> you will be able to relocate the scanner. When
it is at the original position it's coordinates are 0,0.

Jetpack: This replaces the quad.  (Who needs quad damage with all these
heavy weapons?  Right.)  Will let you fly, but it's got all the problems
of a real jetpack: you have to work at it to avoid slamming into walls
and so on.

Lasersight: A little red ball will dance in front of you, showing you
where your gun is pointed.  It'll glow brighter once it finds a target.
But be careful, everyone else can see it too.

Sniper Zooming: Press F11 to cycle through views and f10 to switch back
to your normal view as a sort of a "panic button".

Night Vision: this only works right in GL (3dfx) mode.

Cloak: This essensially nullifies the homing weapons and the scanner. If you
use this command it will hide you from the scanner and make it so the homing
weapons don't home on you. But your homing weapons will also not home on the
other players as well.

Detonate: Bind this command to a key. Use the command whenever you want to 
detonate an explosive.

Explosive: Bind this command to a key. Whenever you want to use it press the
bound key and then fire from the grenade launcher. You can only have one 
grenade out at a time and when you fire a second grenade the old grenade will 
detonate.

Invisibility: This is our second in the cloaking family. When you use it you
become invisible to your opponents but your blip is still on the scanner and 
the homing weapons can still home on you. This is the ultimate snipper tool.

Flashlight: Ever have problems with a camper hiding out in the dark, camping
out and sniping everyone that he sees? Well with this you can find them and 
put a stop to them.

Check Stats: Have you ever wanted to know how healthy your enemies are? Well
know you can know by using the checkstats command. By using this command you
will be able to know how much health and armor you enemies have, and how far
away they are from your current location. 

Rangefinder: This is a permanent addition to the quake2 hud. By using this,
nice little tool, you'll be able to tell how far away your target is and how
much of a lob you need to use to hit them with your Plasma Cluster grenade.

Player Id: Just point your crosshairs at an enemy and you'll get their name,
model and skin that they are using.

Revenge Factor: Imagine, you are the person with the highest frag and every-
body is looking for you, not because they miss you. But because you were the
last person to kill them and because you are the person with the highest frag
count. Sounds like fun huh. If you frag the person who fragged you last its
worth two frag points to you. If you frag the person with the most frags then
they are worth three frag points. So listen up kiddies this is where it gets
fun. If the person with the most frags was also the person who fragged you 
last and then you frag them you get a whooping five frags. Sounds like fun huh?
Well that should put an end to those pesky campers.

Angel of Death:  A small glowing spirit orbits you constantly. Whenever you fire, 
it fires too, in whichever direction you're facing effectively doubling your 
firepower (at the cost of double ammo use, of course).   

Angel of Life:  A different small glowing spirit orbits you, picking up any items 
you pass by.   

Angel of Mercy:  Yet another spirit. This one intercepts as many incoming projectiles 
as it can, rendering them harmless.  

Angel of Frost : A cool new angel that will make your opponents freeze in their 
tracks. Let this little help loose and it will freeze anyone who comes close to you
but watch out, it may mistake you for an enemy sometimes.

Angel of Plague: Use this angel to make yourself the head of the game. Watch as this
angel touches to infect other players. While infected the players will be inflicted
with damage.

Angel of Flame: With this angel you can be hot, really hot. Just light this angel up
and you can burn all of your opponents up.

Angel of Horror: Ever want to scare the begezuz out of your opponents, well with this
angel you can.

Angel of Blindness: Let this angel rip and your enemies will never see that single 
railshot coming at them.

Angel of Revenge: If you die you might want to activate this little toy. Next time
you meet that person the angel will take 50% of their life away.

Angel of Unfreeze: Don't you hate it when you get frozen? Well now with this little baby
you can get unfrozen.

Fake death: Have you ever wanted to play dead? Well now you can.

Day-Night System: You can now simulate the passing of time in WOD:LOX by enabling this
feature in the DMFLAGS. A full rotation is 13 min long.

Height: Ever want to know if that jump from one height to a lower height will hurt you? 
well now you can see how high up in the air you are.

Sweeper Fire Mode: Ever wondered what would happen if you could increase the firerate of 
the railgunsweeper well, now you can. But if you increase the speed in which the sweepers
fire you will use more ammo and also will increase the temperature of the weapon which
might cause it to explode.

Ver: Shows the version of the game

LogPlayers: This command will log the current players in the game, their ips, pings, 
current map, etc. This is useful for logging those pesky players who want to
spoof peoples names.

Airstrike: Just point and paint a target and let the flyboys take care of the rest.
They will pound the painted location with 10 rockets. But watch out they can be
intercepted.

Tracker: Well we did it again, we added a WoD:LOX exclusive feature. Now you might
be asking yourselves, what is so original about the trackers, aren't they just like
normal turrents? Well no they're not. When you drop one it will look like a skull gib,
depending on what weapon you were using at the time is what it will shoot.

Anti-Camper: Ever get really tired of all those campers? Well I have a solution for
you. When you set the dmflag to enable this feature campers will have one warning
before they are executed by the mod. Campers beware.

Holoquad: Ever want to fool your opponents? Ever want a way to lure them out into the 
open with something as tempting as quad damage? Well guess what, now you can. Just use
the 'holoquad' command to drop a fake quad damage and let the fun begin.

Holoinvul: Use the 'holoinvul' command to drop a fake invulnerability.

Holoweapon: Use the 'holoweapon' command to drop a fake copy of your current weapon.

Slug Armor: Ever get tired of those people who always use the railgun sweeper? Well now
with this new armor you can enjoy yourself at their expense. You have a 300 to 1 chance
of getting this armor. When you do get the armor those people who do abuse the railgun-
sweeper will get a nice surprise.

Cell Armor: Ever get tired of those people who always using the positron or freezer? Well now
with this new armor you can enjoy yourself at their expense. You have a 300 to 1 chance
of getting this armor. When you do get the armor those people who do abuse the positron
and freezer will get a nice surprise.

Shell Armor: Same as Slug and Cell armor but protects against shell weapons.

Bullet Armor: Same as Slug and Cell armor but protects against bullet weapons.

Grenade Armor:Same as Slug and Cell armor but protects against Grenade weapons

Rocket Armor:Same as Slug and Cell armor but protects against Rocket weapons

Angel of Energy defends against: 
   "plasma" 
   "bfg blast" 
   "bolt" 
   "noclass" 
   "flare" 
   "antiflare" 
   "positron" 
   "freezatron" 
   "floating mine" 
   "positron explosion" 
   "freezatron explosion" 
   "cataclysm explosion" 
   "plasma explosion" 
   "pulse" 
   "homing rocket" 
   "nuke" 

Angel of Mercy defends against: 
   "plasma" 
   "grenade" 
   "rocket" 
   "bfg blast" 
   "bolt" 
   "super shot" 
   "buckyball" 
   "flare" 
   "spike" 
   "positron" 
   "freezatron" 
   "pulse" 
   "homing rocket" 
   "guided rocket" 
   "guided nuke" 
   "nuke"

++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Fixes and Enhancements by QwazyWabbit: LOX Version 1.12.4 and higher.
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

Time: Shows the current time of day. (works on all platforms)

Timeleft: Shows the time remaining in the current match to the 
client that issued the command.

Timeall: Sends a message to all clients with the time remaining in the
match plus the current date and time.

Vote map: Initiate a vote for any map on the system. Now players can select
a map that they would like to play and they don't need an admin to rcon it.

Angeloe: Angel of Energy, intercepts items like angelom plus some others.
This angel intercepts items the angelom doesn't and vice versa, neither angel
does it all.

Enhanced the "man" command. Added several new weapon explanations and bind
instructions to the help text. Now newbies can get online descriptions of 
how to use the LOX weapon set. For example: type 'man angels' or 'man grenades'
to see how these features work.

CTF:LOX Cloaked players glow in the color of their team so players can tell
each other apart.

-note:
 The rangefinder, player id, revenge factor do not have any key bindings so
please don't ask what the key bindings for them are.

========
Commands
========

--------------------------------------------------------
Function		| binding	| Command
--------------------------------------------------------
Night Vision		| N		| cmd night vision
--------------------------------------------------------
Jetpack			| J		| use jetpack
--------------------------------------------------------
Laser Sight		| S		| cmd lsight
--------------------------------------------------------
Laser Tripwire		| L		| cmd laser
--------------------------------------------------------
TractorBeam Pull	| o		| cmd pull
--------------------------------------------------------
TractorBeam Push	| p		| cmd push
--------------------------------------------------------
Cycles Zoom Views	| f11		| cmd zoom 1
--------------------------------------------------------
Original View		| f10		| cmd zoom 0
--------------------------------------------------------
Kamikaze		| end		| cmd kamikaze
--------------------------------------------------------
darKMajick grenades     | N\A           | see below
--------------------------------------------------------
Grappling Hook:		| G		| +hook
Slide up grapple        | R		| +shrink
Slide down grapple      | F		| +grow
--------------------------------------------------------
Scanner			| S		| cmd scanner <xpos> <ypos>
--------------------------------------------------------
Decoy			| D		| cmd decoy
--------------------------------------------------------
Toggle Burst fire for   | M		| cmd firemode
Standard machine gun    | M		| cmd firemode
--------------------------------------------------------
Angel of Life		| l		| cmd angelol
Angel of Death          | l		| cmd angelod
Angel of Mercy	        | l		| cmd angelom
Angel of Frost		| l		| cmd angelof
Angel of Plague		| l		| cmd angelop
Angel of Flame		| l		| cmd angeloflame
Angel of Horror		| l		| cmd angelohorror
Angel of Blindness	| l		| cmd angeloblind
Angel of Revenge	| l		| cmd angelor
Angel of Unfreeze		| l	| cmd angelou
--------------------------------------------------------
Flashlight		| k		| cmd flashlight
--------------------------------------------------------
use commands "thirdx" and "thirdz" to adjust the chasecam
use command "invis" to become invisible
use command "cloak" to cloak from scanner
use command "flashlight" to use flashlight
use command "sweeperfiremode" to toggle the sweeper fire mode
use command "explosive" to toggle the grenades explosive mode
use command "detonate" to detonate the explosive
use command "skinlist" to display the names and skins of players
--------------------------------------------------------
			
* Tip * Use console command "give all" to get all the items.

========
Mod info
========

New DLL(s): Yep

New Sound(s): Yep

New Model(s): Yep

Known Bugs: I've fixed all the bugs I've received. If you find any more,
mail me at jblabs@aracnet.com

Source: none, yet. But if you really want to try to decipher it then you
can contact me. Or you can use Visual Studio to disassemble the dll.

=======
Credits
=======
- Dave Martin opus@aracnet.com,javabean@aracnet.com,jblabs@aracnet.com
  - Code Warrior using Visual Studio 5.0
- Dusty Lloyd
  - Pseudo Code Warrior using Visual Studio 5.0
  - Newest version of Modula-3 for the enhanced homer code
  - Ada for the angel code
- Justin Sucharski
  - Blind as A Bat Boy using Photoshop for skinning (we all know)
- Brendan McWalters winkel@zork.net
  - Provided moral support and food
  - and someone who we could smack around a bit, with a wet trout of course.
Testers
 - Rommel[RnR]
 - Tiger's Cum Rag<LOX>
 - Hippi<LOX>
 - Winkel<LOX>
 - Betrayal <LOX>
 - PurityControl<LOX>
 - All of Clan RAD
 - Ed Gein and Clan OBD
 
People to thank
 - Clan RAD for being so supportive of my mod, without them the mod wouldn't be
   where it is today
 - My girlfriend for being patient on those nights where I would get up from sleeping
   to fix a bug or two.
 - Boarder for the Angel Of Revenge idea
 - To everyone else who submitted a weapon or feature idea that I forgot to put in.
 - Chaos Mod developers for being so understanding
 - All the people who play the mod and keep it alive
__________________________________________________________________________________

The NEW LOX, WOD:LOX 1.12.x is maintained by QwazyWabbit<WOS>.

Check the Web Board at http://www.clanwos.org/forums/ for more information
and general knowledge about LOX. This should be your first stop since
there are many LOX questions that have been asked and answered there.
I frequent this board and will be there to answer questions about LOX.

I would like to thank all the members of Clan WOS who added their
suggestions, comments and support to this effort and who fragged their
brothers and all other unsuspecting victims on the Clan WOS server, Loxophilia,
in their efforts to test the new code. The existence of these revisions is due
in large part to the existence of the WOS clan, without them, LOX would have
died out long ago.

QW
__________________________________________________________________________________

===================
Mod Version History
===================

1.12.7 
- RELEASED: January 16, 2005
- Fixed conflict between rangefinder and CTF HUD variables and tokens. Now
  the game correctly displays CTF icons and scores.
- Added rangefinder to HUD in CTF Extinction.
- Set CTF cloaking colors to red and blue so players can tell each other
  apart when cloaked. Need orange glow for invulnerability?
- Fixed lightning gun effect on grappling hook chain in CTF mode.
- Fixed OnSameTeam function to properly handle skin teams and CTF teams.
- Added skinlist command. This lists the player's skins by client number
  for all players in the game. I added this to get around the fact that
  R1CH's R1Q2 client hack kills the skins command in that version of the
  client.
- Fixed up HUD, now using manifest constants in strings so game can keep
  the messages straight without a lot of programmer work. Created a
  unified initialization function for the HUD in all modes. 
  FPM, Rangefinder, Height and playerid toggles all survive across respawns.
- Eliminated redundant HUD functions in CTF code.
- Changed FPM to FPH.
- Made ext_devt cvar to enable users to use the lasertrips to get point locationals
  for extinction map ents and weapon placements. The old way used a compile-time
  define to create a special build for it. This provides a runtime switch anyone
  can use.
- Fixed cheats testing in coop mode. Disallow cheat commands if we're in coop mode
  and the cheats flag is not set.
- Fixed several different server-crashing bugs in coop mode.

1.12.6
- RELEASED: December 19, 2004
- Extensive modification of weaponban and featureban constants
  and cleanup of float/int conversion overflow problems. System
  now uses unsigned ints instead of floats for weapon bans and
  feature bans. The Lopez' Server Configuration Tool is now obsolete
  as many weaponbans and feature bans changed values. The new 
  constants are listed in the server_readme.txt. The advantage of
  all this is now all the individual weapon bans actually work. An
  added benefit of the way the system gets the values it needs is
  that the values can be decimal, octal or hexadecimal format. 
  (e.g.,12345, 0123, or 0x124af)
- Added anti-spam logic to map voting module. Now players can only
  initiate a map vote an admin-configured number of times in any one
  match. The default is 2. Cvar electstarts controls this.
- Added 6 second timer to decoy command to prevent spamming.
- Improved maplist processing in the mod to check that the map file
  can be found before passing the map change to the intermission
  functions in the Q2 core. This prevents server crashes due to
  typos in the maplist file or Upper/lower case sensitivity. 
- LOX now logs the date and time of map changes and player joins
  in the qconsole.log if logging is enabled.
- Fixed weapon heat in sweeper code. Sweepers heat in all fire modes.
  Admin can turn on dmflag DF_WEAPONHEAT to enable this feature.
  Overheating your weapon causes it to blow up - and you with it.
- Angel of flame deflects angel of frost and angel of frost deflects
  angel of flame. Frost will not un-flame you and flame will not
  un-freeze you.
- Guided missile is now a skull gib. (Phlem<WOS>)
- Spiral rockets are now head gibs. (Phlem<WOS>)
- Many cool and funny obituary texts added by Phlem. Sometimes it's
  hard not to just sit there and watch the texts roll by as the game
  is played.
- Added green and gold versions of the pak1.pak file to the installation
  package in order allow users to improve the readability of the game text. 
  As the game gets older, so do the players and their eyes.
- Added angel of energy.
- Modified many angel features. Separated the angel source files and
  made many features and weapons more modular and atomic. Now weapons
  can be added, deleted, modified without slicing up monsterous source
  files.
- Improved hook help text. LOX hook is unique in that it has several binds
  needed for extend, retract, etc. FAQ in the game is "how to hook?" Now
  users can be told 'man hook' and server help text is displayed in console.
- Modifed angel of revenge and angel of plague to do more damage to opponents.
- Modified interaction of angels:
  Angel of mercy and revenge deflect angel of revenge.
  Angel of unfreeze or angel of mercy deflect angel of frost.
- Cloak does not hide you from angels.
- Removed durgsweeper. Causes server lag and crash when too many entities
  are created too quickly. Durggun remains in inventory.
- Added kaminit, disruptor and railgun trackers. Launching trackers with
  super rail shotgun in hand now launches rail trackers.
- Added map voting. Now users can select maps and majority wins the map
  immediately changes. Now players don't have to beg admins for map rcons.
- Source code now compiles cleanly on Windows VC++ 6.0 and Linux GCC with
  minimal pragmas. VC set to warning level 4 (max.) and nice clean compiles.
  Linux Makefile is maintained in sync with Windows project.
- Added cvar campertime, anticamper feature can now be enabled with admin
  programmable time limits from 5 to 600 seconds.
- Fixed angel of mercy vs. guided missiles. This was a really old bug and took
  a long time to debug and fix. Angel of mercy now successfully defends
  against guided rockets without sending the POV of the guided rocket owner
  into the floor or some other weird space.
- Kamikaze now uses kamikaze.wav, players now yell Banzai! when going kami.
- Fixed freeing of angle edicts on player death.
- CTF:LOX Extinction now works properly. Grappling hook is fixed, the
  cloak is colored by team color, HUD is fixed, scoring display is fixed.
- The tracker lock bug is fixed at last.

1.12.5
- RELEASED: Never. (completed 10/29/2004)
- This version had lag problems in test and was never released.
  
1.12.4
- RELEASED: June 26, 2004
- Admin configurable limit on floating mines, default is 7, max allowed 10.
- Fixed bug that caused laser tripwires to cause server crashes in LOX.
- Allow floating mine launcher to launch darKMajick grenade types.

1.12.3 
- RELEASED: May 31, 2004 for Windows and Linux.
- Allow angel of mercy to catch nukes.
- Fixed up floating mine launcher selection & inventory.

1.12.1 & 1.12.2
- RELEASED March 20, 2004 and March 23, 2004.
- Decoys no longer crash the server when used with homers and skipper rockets.
- Decoys now spawn with skin of their owner instead of skin of client 0.
- Grapple and and chain no longer remain as lingering entities if a player is
  disconnected from the server.
- Added admin-configurable limits on laser tripwires and trackers.
- Fixed floating mine launcher code and added admin configurable limits.
- Added runtime cvar, debugmodels so server won't flood clients with model entity
  debugging information unless the variable is set.
- Allow BFG to be enabled in infinite ammo mode. Originally hard-code out.
- Shortened gamename to prevent InfoStringlength Exceeded messages.
- Added kamifrozen cvar, admin can now allow players to be kaminitted or
  go kamikaze while frozen.
- Reinvented guided grenades and guided nukes. This code was lost between the
  1.10 and 1.11 versions. The 1.11 release binaries had it but the 1.11 sources
  that 1.12 was based on didn't have them.
- Added Timeleft function. Fixed Time command. Time functions now
  work on Windows and Linux platforms.
- Cleaned up player obituary texts.
- Increased kamikaze damage. Now you have to run like hell to get away from a
  kamikaze player if you want to survive.
- Increased range of vacuum maker by 20% and increased damage.

1.12 
- December 12, 2003, QwazyWabbit begins renewed development of WOD:LOX. This was
  initially begun as an attempt to fix many of the worst bugs of LOX that made full
  use of many of the features of WOD:LOX impossible. The new LOX has become an
  attractive new mod on the good old Quake2 engine with new and better weapons and
  features. The current development effort is directed at making LOX a stable and
  full-featured mod that allows admins more control of the style of play they want
  to see on their server. The Admin chooses the weapons and features he wants. This
  allows him to make LOX a strategic and skillful game full of stealth, stalk and
  accurate marksmanship, or a full-on fragfest of cataclysmic explosions, rapid-fire
  streetsweepers and weapons of mass destruction.

1.11 Summary by QwazyWabbit:
- This release included darKMajick grenades and guided nukes and guided grenades.
  Original source code that included these features was "lost". The binaries for
  WOD:LOX 1.11 became the standard release most commonly found on game servers
  but the mod languished undeveloped from September 1999 until December 2003.

1.10
- RELEASED: WoD:LOX Server Configuration Tool. There is a link to the zip file at the
  wodlox download page. This should help server admins configure their wod:lox servers.
- Added the Explosive Chaingun. Hehehehe this should make the game a little bit more
  interesting. Fires explosive bullets rather than than the regular shells.
- Added the Explosive Shotgun weapons. Each one of the shell weapons now have an 
  explosive alternate. People should get a kick out of the new Explosive Streetsweeper.
- Got the Linux version to compile, but I am still getting Segmentation Faults right
  after someone connects. I am trying to get it to work. Another problem that we had
  was that the Torture Chamber was making it so the Linux binary would not load up.
  I had to take it out of both versions. Hopefully I can get it to work sometime in
  the future.
- It works, it works. After getting reports that the explosive and detonate command didn't
  work I was bummed. I thought that I had fixed the bloody thing. Then I noticed that
  when the explosive killed you, when you died, if you had an explosive out it had 
  to be detonated, thus causing damage to you, killing you, and so on and so on. You
  can see how this could cause a server to crash, well this is fixed now.
- I added rocket armor and grenade armor. Though this does not protect you from the
  grenade or rocket splash damage.
- Fixed up the angel of death to fire the new weapons
- Fixed up the trackers to fire the new weapons
- Changed the Spiral Rocket Launcher, so when the rockets actually hit they will act
  like the flame rockets.
- added variables for revenge factor, revengeaward and highfraggeraward. Hopefully 
  this should motivate people to use this feature of the mod.
- added the daynight server variable. It is set by default to 13 minutes to change it
  just add the line 'set nightdaytime <time in min>' where <time in min> is the number
  of minutes you want the cycle to take. A cycle if you don't remember is the time it
  takes for it to go from day -> night -> day.
- changed the MOD messages for the explosive weapons.
- Added 3 new commands that you guys will love. Well actually it is four commands. The first
  of the commands is lbind. This command is needed for the following three commands. 
  onspawn, ondeath and onenemydeath. So if you can imagine these new commands will be 
  very useful. For instance you no longer will have to set up your HUD each time after you
  die. Instead you can do 'lbind onspawn "<commands>" ' the commands have to be in quotes else
  it won't work properly. Enjoy.
- Added the Sticky Grenadesweeper
- Added Explosive Machinegun
- Added and fixed the High Fragger Ghost feature. Bascially what this does is create a ghost like
  effect above the person with the most frags.
- Added the positron grenade
- Added the Kaminit gun. Basically when you hit someone with this weapon it will initiate their kamakaze
  sequence.
- Added the Wall Piercing Railgun, so all of you who like hiding behind walls, beware.
- Added Angel Of Unfreeze per request of many people. I know that the Angel of Frost can become 
  overly annoying when used. Well here is my answer to it.
- WoD:LOX Linux finally compiled and worked fine. Thank you gametoken, sigh.

1.09
- I have fixed the explosives bug as well as I can. There is a server-client timing 
  issue that the ID Software code has. I really don't know what to do.
- After play testing the trackers with some people we noticed that the rate of fire
  has been too fast. I slowed the rate of fire down. Plus the trackers now track a 
  lot better.
- All the sweepers actually work correctly now. After tweaking their code a bit I 
  have been able to make them work like the streetsweeper works. As long as you hold
  down the attack button down you will fire. 
- If you have the sweeperfiremode enabled and have the attack button pressed while 
  firing the attack button down you run the risk of having your sweeper explode. You
  have been warned.
- I have gotten sick of people asking me "How do I do ...." you can finish the rest.
  Well as a solution to this I have implemented a in WoD:LOX help system. Ask it a 
  question about a feature, command or person and you will get a short description 
  and the command to it. I hope this helps people.
  08-14-99, wow I finished the commands part of the in game online manual. I am going
  to start on the weapons parts of it. It is looking really good.
  08-15-99, The weapons part of the in game online manual is done now.
- Modified the scanner so it can be relocatible. Basically if you type in the console
  Scanner <xpos> <ypos>, you will be able to determine the location of the scanner.
  Try it, it is cool.
- Added Holo commands, now you can drop a decoy BFG, invulnerability and quad damage.
  You can use these to lure people out in the open. WoD:LOX sould be a little bit more
  interesting now. Hehehehe.
- I fixed the Trackers so they actually shoot the correct ammo now. So if you drop a
  tracker while using the blaster, it will shot blaster shots. Trackers can now be
  destroyed by shooting them.
  I also toned down the trackers. You can only shoot 3 of them out and when they shoot
  their ammo they will shot a lot slower.
- Added the time command. Now you can see what time it is, useful for those long nights
  of fragging.
- Fixed the invisibility command. After playtesting it was noticed that you could be
  invisible and cloaked at the same time.
- Toned down the grenade and bazooka sweepers. After play testing it was noticed that
  excessive use of these sweepers made the servers crash.
- Added the Slug armor. This armor makes the player absorb the damage that any of the
  Slug weapons do.
- Added the Cell Armor. Same as Slug armor but absorbs the damage from any weapons that
  use cells.
- Added the Shell Armor. Same as Shell armor but absorbs the damage from any weapons that
  use shells.
- Added the Bullet Armor. Same as Bullet armor but absorbs the damage from any weapons that
  use bullets.
- Added the sonic railgun. Shoots when you release the attack button. The longer you hold
  the attack button down the more damage it does.
- Added the Spiral Rocket Launcher. Shoots 3 rockets. The one main rocket in the center
  with two outer rockets spiralling around the main one.

1.08
- After getting reports that the explosives were not working correctly in the mods
  I desided to investigate the problem. And low and behold there was a problem in 
  the code. I fixed the bug, in the process I created a second command called detonate.
  This is the command that you use to detonate a explosive from now on.
- Added the Durg gun, with this weapon you can make your opponents lose their current 
  weapon.
- Added the Durgsweeper.
- Added the Angel of Revenge. 
- Added the Double Impact weapon, shoots two grenades verses the one for the grenade
  launcher.
- Added the Tracker, WoD:LOX style turrents.
- Added the FBFG.
- Added Disruptor from the Wyrm ][ code. With that code in place I will be able to use
  its special effects to create some new weapons.
- Added the Vacuum Maker. This should make the game more interesting because now you are
  able to pull people and objects to you.
- Added the infamous energy vortex
- Added a time variable for the night-day system

1.06-1.07
- Well I have been avoiding this for way to long so here is the lowdown on
  the new version of the mod. We have got 13 servers up and over 1000 playing
  the mod now.
- Added three new angels, Angel Of Horror, Angel of Blindness and Angel of Flame
- Added a new grenade, Blindness Grenade, based on the Angel of Blindness
- Added the mace weapon, again based on the Angel of Blindness
- Fixed the day-night system. It seems like the day-night system was still 
  not working together. Basically the system should have reset itself on 
  each level load, but it did not. Well Now it does.
- Added the ver command
- Added the logplayers command. This command will display the players names, ip 
  addresses, current weapon and current map. This is to help weed out the name
  spoofers.
- Added the Airstrike command. 
- Added the Flamesweeper, new type of flamethrower.
- Added the chunkgun, new type of sweeper based on the Angel of plague. You
  can all thank Z'ha'dum for this one. 
- Fixed the new angel code to make it more efficient and concice.
- Created a workaround for the Index Overflow error
- Started to work on two new angels, Angel of Evil and Angel of Revenge.
1.05
- Fixed the annoying Angel Of Frost bug where if two people were playing
  with the angel of frost active and one person froze the other person, 
  the frozen person's angel could still freeze people.
- Fixed the Day-Night bug where after a level ended with a change in the 
  lighting, the new level would start normal, then all of a sudden change
  to the next light level.
- Added the new Angel Of Plague
- Added the new Icecube gun
- Fixed the Sticking Grenades so they will work with the explosive command

1.04a
- Added the Anti-Camper Deterant System
- Added a new Angel, Angel of Frost
- Increased the Damage of the Guided Missiles
- Started the creation of Server Admin functions,
  remote map changing, password protection, etc.
- Removed the egg weapons were causing the servers to crash
- added the Height function

1.04
- Added the Day-Night system, look at the dmflags for that
- Added three angels, life, mercy and death.
- Added the flaregun
- Added the organic armor
- Per request, I have toned down the buckyballs and made it so the 
  flares cause no damage

1.03
- Added the rangefinder.
- Added Player Id when player holds crosshairs over enemy
- Added checkstats command
- Added Revenge Factor
- Fixed up the feature, weapon and grenade banning, just set them up in
  the server cfg file

1.02a
- fixed the superblaster, it actually shoots normally now
- added two new functions invisibility and flashlight
- added a nailgun
- populated the buckyball death messages

1.02
-Broadsword added
-Cloak command modified, added timer and coloring to person when using it
-added homer and rocket grenades
-explosive command added

WOD:LOX BORN
1.01
-New sweepers added
-Homing rocket code enhanced
-New grenades added
-Cloak command added
-Removed weallknow command
-Renamed the MOD from wod to LOX

6.02
-Grenade launcher now fires all darKMajick grenades
-New alternate grenade-launcher weapon, the bazooka.  Fires all
 darKMajick grenades, but in a straight line.
-Lasers replaced with laser tripwires that fire all the
 darKMajick grenades.
-Homing rockets can now be shot down, and only take 1 rocket per shot
 (was 2 per shot).
-Doubled the amount of ammo backpacks & bandoliers give.
-New plasma rifle.
-Fire now makes a sizzling sound.
-Added ZbotCheck 1.01 by the Lithium crew.  (Windows version only: the
 Linux version won't link & they seem to be uninterested in providing
 support.)
6.01
-Added the allow_cataclysm server variable
-Sniper gun now takes 5 shells, not 2.  Weapon balancing thing.
-Cataclysm device kills no longer credited to inflictor.
-Homing rocket radius explosion kills now credited to inflictor.
-Added "You've been frozen" message when player gets frozen.  It's
 a lame hack until I rewrite the frozen code to make being frozen
 more intuitive.
-Fixed even more bugs -- spawnpoints no longer get thrown in
 explosions, decoys work again, skin no longer gets hosed when
 you're frozen, the frozen obituary message is accurate now,
 the lasersight gets cleaned up when the player dies, and
 NoAmmoWeaponChange() is a little more intelligent.
-Officially released a Linux version, finally.
6.0
 Done by Hash Assassin, whatis@yyz.com, on or around 9/28/1998.  My
 first official version.
-Fixed an ungodly number of bugs, compiler warnings, and inefficiencies, some
 in WoD code, some in the original Id code.
-Added streetsweeper
-Rewrote plasma rifle to be more like Doom II version
-Rewrote NoAmmoWeaponChange() to select some of the new weapons.
-Increased ammo capacities.  Now more fragfest than ever!
-"maplist" feature.  Just set the Q2 variable "maplist" to something non-zero,
 and the next map will be chosen out of the maplist.txt file in your WOD dir.
 No more running maps in a boring old loop.
-Deathmatch players now invulnerable for 3 seconds when they spawn.  Gives them
 a fighting chance & protects against people putting laser tripwires on spawn
 spots.
-Balanced the weapons better.  Now ammo usage is more in line with power.
-Added/modified client obituary messages.
-Laser sight brightens when on a player or monster.  Hee hee.
-Removed triple hyperblaster.  It's a nice weapon, just very laggy.  And the
 machine rockets are already pushing the limits.

5.4a-

-Fixed Crashing when dying with freeze gun.

5.4-

-Added Freeze Gun
-Night Vision
-Re-added standard railgun
-Added new Grenade and Super Blaster effects
-No bugs needed to be fixed! Woohoo!

5.3:

-Perfect Chasecam, I'm pretty sure it's bugless.
-Re-added standard Machine-Gun
-Added Burst Fire mode for Standard machine-gun
-Fixed Cluster Bomb bug
-Fixed kamikaze when dead bug
-Removed Viewable models, do to so many bugs [people wanted to use their
custom models, but got a white diamond on its ass, etc.]


5.2:

-replaced detpipes with shrapnel grenade
-added sniper gun
-added decoy
-added plasma gun
-better rocket explosions
-replaced grappling hook with swinging grappling hook
-fixed tons of bugs
-removed ricocheting lasers and range finder- it lags net play horribly

5.0:

-Added chasecam
-Added Visible Weapons
-Added sound for ricocheting lasers
-Added scanner
-MOTD support
-Fixed yet more bugs - Thanks Mike Fox

4.9b:

-Fixed messages
-Fixed crashing when using super blaster
-Lowered number grenades picked up

4.9:

-Fixed numerous deathmatch bugs.

4.8:

-Re coded the WHOLE mod with the 3.14 source as a base. This should
fix all the deathmatch and coop bugs.

-Re added shotgun

-Enhanced flamethrower burn. It now uses a model for the burn animation.

4.7:

-Now works with 3.12/3.13/3.14

-added Grappling Hook

-fixed 8 twice

-put fire model for fire on the ground

4.6 - minor update:

-Updated flame thrower. It has a great new model.

4.5:

-Removed super blaster model, so now there's no null skin error, and it
can run on servers without clients having the patch.

-Replaced Burst Railgun with Flamethrower

-Added Flame Grenade Launcher

-Replaced chain shotgun with Fire Rocket Launcher

-Changed Chain Shotgun to Flame Rocket Launcher

4.1 - small update:

-(Re)Added random blue/red lasers to blaster

-(Re)Added long kickback for Super Shotgun.

4.0:

-Removed Homing Rockets due to code conflicts

-Added modified model for Superblaster

-Removed Grenade Launcher proximity (see below)

-Added "darKMajick" grenades- Cluster, Floating Proximity 
(improved from previous grenade launcher proximity), 
Railbomb, Plasma Bomb, Napalm Grenade, and Cataclysm device

-Added kamikaze device

-Added ricocheting lasers sound

-Added Machine Gun Rockets

-Fixed entities covering up blaster laser effects

Other: This was a challenging task. I had to rewrite the whole code from
the base up to add in the darKMajick grenades.

Version 3.6:

-Made the jetpack much better.

-Fixed crashing

Version 3:

-Fixed the laser sight skin error problem

-Added Rangefinder

-Added "Star Wars" Lasers

-Added Zoom Commands

-Added Laser Tripwire

-Added Burst Fire shotgun

Version 2 :

-Changed the super blaster trail effects so it looks like a blaster and not 
a grenade.

-Changed the Cluster Bombs to a Triple Grenade launcher

-Balanced Railgun more.

-Detonatable Throwing Grenades!

Version 1: Initial release.

======
misc.
======

Read that freakin' id license agreement.

If you need further assistance with installing/playing this mod,
contact me.
