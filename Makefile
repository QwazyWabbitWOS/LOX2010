#
# Quake2 Makefile for Linux
#
# Jan '98 by Zoid <zoid@idsoftware.com>
#
# Edited July 13, 2025 by QwazyWabbit
#

.DEFAULT_GOAL := game

# this nice line comes from the linux kernel makefile
ARCH := $(shell uname -m | sed -e s/i.86/i386/ \
	-e s/sun4u/sparc64/ -e s/arm.*/arm/ \
	-e s/sa110/arm/ -e s/alpha/axp/)

# On 64-bit OS use the command: setarch i386 make all
# to obtain the 32-bit binary DLL on 64-bit Linux.

CC = gcc -std=c17 -Wall -Wpedantic

# On x64 machines do this preparation:
# sudo apt-get install ia32-libs
# sudo apt-get install libc6-dev-i386
# On Ubuntu 16.x and higher use sudo apt install libc6-dev-i386
# this will let you build 32-bits on ia64 systems
#
# This is for native build
CFLAGS=-O3 -DARCH="$(ARCH)" -DSTDC_HEADERS
# This is for 32-bit build on 64-bit host
ifeq ($(ARCH), i386)
CFLAGS += -m32 -I/usr/include
endif

# use this when debugging
#CFLAGS=-g -Og -DDEBUG -DARCH="$(ARCH)" -Wall -pedantic

# flavors of Linux
ifeq ($(shell uname),Linux)
CFLAGS += -DLINUX
LIBTOOL = ldd
endif

# OS X wants to be Linux and FreeBSD too.
ifeq ($(shell uname),Darwin)
CFLAGS += -DLINUX
LIBTOOL = otool
endif

SHLIBEXT=so
#set position independent code
SHLIBCFLAGS=-fPIC

# Build directory
BUILD_DIR = build$(ARCH)

# Ensure build directory exists
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# List of source files
GAME_SRCS = \
	fileexists.c g_ai.c g_chase.c g_cmds.c g_combat.c g_devmenu.c g_func.c \
	g_items.c g_main.c g_misc.c g_monster.c g_offworld.c \
	g_phys.c g_save.c g_spawn.c g_svcmds.c g_target.c \
	g_team.c g_trigger.c g_turret.c g_utils.c g_weapon.c \
	jetpack.c lsight.c m_actor.c m_berserk.c \
	m_boss2.c m_boss3.c m_boss31.c m_boss32.c m_brain.c g_ent.c \
	m_chick.c m_flash.c m_flipper.c m_float.c m_flyer.c m_gladiator.c \
	m_gunner.c m_hover.c m_infantry.c m_insane.c m_medic.c m_move.c \
	m_mutant.c m_parasite.c m_soldier.c m_supertank.c m_tank.c \
	maplist.c p_client.c p_hook.c p_hud.c p_menu.c p_trail.c p_view.c \
	p_weapon.c q_shared.c s_cam.c scanner.c x_fbomb.c x_fire.c zbotcheck.c \
	kamikaze.c l_airfist.c l_angelob.c l_angeloblind.c l_angelod.c \
	l_angeloe.c l_angelof.c l_angeloh.c l_angelol.c l_angelom.c \
	l_angelop.c l_angelor.c l_angelou.c l_angels.c \
	l_antiflaregun.c l_antimattercannon.c l_ar_admin.c l_backpack.c \
	l_bazookasweeper.c l_bucky.c l_chunkgun.c l_daynight.c \
	l_disintegrator.c l_dm_grenades.c l_doubleimpact.c l_durg.c l_durgsweeper.c\
	l_energyvortex.c l_fbfg.c l_flamesweeper.c l_flamethrower.c \
	l_flaregun.c l_flashlight.c l_floatingminelauncher.c \
	l_freezatron.c l_freezegun.c l_freezersweeper.c l_grenadesweeper.c \
	l_guidednuke.c l_guidedrocket.c l_help.c \
	l_explosivemachinegun.c l_iceballgun.c l_nukegrenade.c oak.c oakai.c \
	l_kaminit.c l_lightninggun.c l_mace.c l_matchplay.c l_nailgun.c l_nuke.c \
	l_plasmarifle.c l_positron.c l_pulserifle.c l_railgunsweeper.c \
	l_rockets.c l_snipersweeper.c l_spiralrocket.c \
	l_stickinggrenades.c l_stickygrenadesweeper.c l_superrailshotgun.c \
	l_superblaster.c l_superblastersweeper.c l_sword.c \
	l_tracker.c l_turret.c l_vacuummaker.c l_voting.c \
	lasertrip.c wf_decoy.c performance.c

GAME_OBJS = $(GAME_SRCS:%.c=$(BUILD_DIR)/%.o)

# Pattern rule to place objects in build directory
$(BUILD_DIR)/%.o: %.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(SHLIBCFLAGS) -MMD -MP -MF $(@:.o=.d) -o $@ -c $<

-include $(GAME_OBJS:.o=.d)

# Build all object files that are out-of-date
game: $(GAME_OBJS) game$(ARCH).real.$(SHLIBEXT)

# Main target: depends on all object files
game$(ARCH).real.$(SHLIBEXT): $(GAME_OBJS)
	$(CC) $(CFLAGS) -shared -o $@ $(GAME_OBJS) -ldl -lm
	$(LIBTOOL) -r $@
	file $@

# Build everything (always rebuild all objects and the shared library)
all:
	$(MAKE) clean
	$(MAKE) $(BUILD_DIR)
	$(MAKE) $(GAME_OBJS)
	$(MAKE) game$(ARCH).real.$(SHLIBEXT)

clean:
	rm -rf $(BUILD_DIR)
