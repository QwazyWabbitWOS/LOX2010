
#include "g_local.h"
#include "g_team.h"
#include "m_player.h"
#include "scanner.h"

// JDB: Define variables for lowlight vision effect 4/4/98
#define LLV_R 0.0f
#define LLV_G 0.7f
#define LLV_B 0.0f
#define LLV_A 0.7f

static	edict_t* current_player;
static	gclient_t* current_client;

static	vec3_t	forward, right, up;
float	xyspeed;

float	bobmove;
int		bobcycle;		// odd cycles are right foot going forward
float	bobfracsin;		// sin(bobfrac*M_PI)

/*
===============
SV_CalcRoll

===============
*/
float SV_CalcRoll(vec3_t angles, vec3_t velocity)
{
	float	sign;
	float	side;
	float	value;

	side = DotProduct(velocity, right);
	sign = side < 0 ? -1 : 1;
	side = fabs(side);

	value = sv_rollangle->value;

	if (side < sv_rollspeed->value)
		side = side * value / sv_rollspeed->value;
	else
		side = value;

	return side * sign;

}


/*
===============
P_DamageFeedback

Handles color blends and view kicks
===============
*/
void P_DamageFeedback(edict_t* player)
{
	gclient_t* client;
	float	side;
	float	realcount, count, kick;
	vec3_t	v = { 0 };
	int		r, l;
	static	vec3_t	power_color = { 0.0, 1.0, 0.0 };
	static	vec3_t	acolor = { 1.0, 1.0, 1.0 };
	static	vec3_t	bcolor = { 1.0, 0.0, 0.0 };

	client = player->client;

	// flash the backgrounds behind the status numbers
	client->ps.stats[STAT_FLASHES] = 0;
	if (client->damage_blood)
		client->ps.stats[STAT_FLASHES] |= 1;
	if (client->damage_armor && !(player->flags & FL_GODMODE) && (client->invincible_framenum <= level.framenum))
		client->ps.stats[STAT_FLASHES] |= 2;

	// total points of damage shot at the player this frame
	count = (client->damage_blood + client->damage_armor + client->damage_parmor);
	if (count == 0)
		return;		// didn't take any damage

	// start a pain animation if still in the player model
	if (client->anim_priority < ANIM_PAIN && player->s.modelindex == 255)
	{
		static int		i;

		client->anim_priority = ANIM_PAIN;
		if (client->ps.pmove.pm_flags & PMF_DUCKED)
		{
			player->s.frame = FRAME_crpain1 - 1;
			client->anim_end = FRAME_crpain4;
		}
		else
		{
			i = (i + 1) % 3;
			switch (i)
			{
			case 0:
				player->s.frame = FRAME_pain101 - 1;
				client->anim_end = FRAME_pain104;
				break;
			case 1:
				player->s.frame = FRAME_pain201 - 1;
				client->anim_end = FRAME_pain204;
				break;
			case 2:
				player->s.frame = FRAME_pain301 - 1;
				client->anim_end = FRAME_pain304;
				break;
			}
		}
	}

	realcount = count;
	if (count < 10)
		count = 10;	// always make a visible effect

	// play an appropriate pain sound
	if ((level.time > player->pain_debounce_time) && !(player->flags & FL_GODMODE) && (client->invincible_framenum <= level.framenum))
	{
		r = 1 + (rand() & 1);
		player->pain_debounce_time = level.time + 0.7;
		if (player->health < 25)
			l = 25;
		else if (player->health < 50)
			l = 50;
		else if (player->health < 75)
			l = 75;
		else
			l = 100;
		gi.sound(player, CHAN_VOICE, gi.soundindex(va("*pain%i_%i.wav", l, r)), 1, ATTN_NORM, 0);
	}

	// the total alpha of the blend is always proportional to count
	if (client->damage_alpha < 0)
		client->damage_alpha = 0;
	client->damage_alpha += count * 0.01;
	if (client->damage_alpha < 0.2)
		client->damage_alpha = 0.2f;
	if (client->damage_alpha > 0.6)
		client->damage_alpha = 0.6f;		// don't go too saturated

	// the color of the blend will vary based on how much was absorbed
	// by different armors
	VectorClear(v);
	if (client->damage_parmor)
		VectorMA(v, (float)client->damage_parmor / realcount, power_color, v);
	if (client->damage_armor)
		VectorMA(v, (float)client->damage_armor / realcount, acolor, v);
	if (client->damage_blood)
		VectorMA(v, (float)client->damage_blood / realcount, bcolor, v);
	VectorCopy(v, client->damage_blend);


	//
	// calculate view angle kicks
	//
	kick = abs(client->damage_knockback);
	if (kick && player->health > 0)	// kick of 0 means no view adjust at all
	{
		kick = kick * 100 / player->health;

		if (kick < count * 0.5)
			kick = count * 0.5;
		if (kick > 50)
			kick = 50;

		VectorSubtract(client->damage_from, player->s.origin, v);
		VectorNormalize(v);

		side = DotProduct(v, right);
		client->v_dmg_roll = kick * side * 0.3;

		side = -DotProduct(v, forward);
		client->v_dmg_pitch = kick * side * 0.3;

		client->v_dmg_time = level.time + DAMAGE_TIME;
	}

	//
	// clear totals
	//
	client->damage_blood = 0;
	client->damage_armor = 0;
	client->damage_parmor = 0;
	client->damage_knockback = 0;
}





//===============
//SV_CalcViewOffset
//
//  Auto pitching on slopes?
//  
//	fall from 128: 400 = 160000
//	fall from 256: 580 = 336400
//	fall from 384: 720 = 518400
//	fall from 512: 800 = 640000
//	fall from 640: 960 = 
//	
//	damage = deltavelocity*deltavelocity  * 0.0001
//	  
//		===============
//
void SV_CalcViewOffset(edict_t* ent)
{
	float* angles;
	float		bob;
	float		ratio;
	float		delta;
	float		zcheck;
	float		xycheck;
	float		xyzratio;
	vec3_t		v = { 0 }, tempv = { 0 };

	//===================================

	if (!ent->thirdperson) //We aren't Thirdperson so caclulate like normal
	{

		// base angles
		angles = ent->client->ps.kick_angles;

		// if dead, fix the angle and don't add any kick
		if (ent->deadflag)
		{
			VectorClear(angles);

			ent->client->ps.viewangles[ROLL] = 40;
			ent->client->ps.viewangles[PITCH] = -15;
			ent->client->ps.viewangles[YAW] = ent->client->killer_yaw;
		}
		else
		{
			// add angles based on weapon kick

			VectorCopy(ent->client->kick_angles, angles);

			// add angles based on damage kick

			ratio = (ent->client->v_dmg_time - level.time) / DAMAGE_TIME;
			if (ratio < 0)
			{
				ratio = 0;
				ent->client->v_dmg_pitch = 0;
				ent->client->v_dmg_roll = 0;
			}
			angles[PITCH] += ratio * ent->client->v_dmg_pitch;
			angles[ROLL] += ratio * ent->client->v_dmg_roll;

			// add pitch based on fall kick

			ratio = (ent->client->fall_time - level.time) / FALL_TIME;
			if (ratio < 0)
				ratio = 0;
			angles[PITCH] += ratio * ent->client->fall_value;

			// add angles based on velocity

			delta = DotProduct(ent->velocity, forward);
			angles[PITCH] += delta * run_pitch->value;

			delta = DotProduct(ent->velocity, right);
			angles[ROLL] += delta * run_roll->value;

			// add angles based on bob

			delta = bobfracsin * bob_pitch->value * xyspeed;
			if (ent->client->ps.pmove.pm_flags & PMF_DUCKED)
				delta *= 6;		// crouching
			angles[PITCH] += delta;
			delta = bobfracsin * bob_roll->value * xyspeed;
			if (ent->client->ps.pmove.pm_flags & PMF_DUCKED)
				delta *= 6;		// crouching
			if (bobcycle & 1)
				delta = -delta;
			angles[ROLL] += delta;
		}

		//===================================

		// base origin

		VectorClear(v);

		// add view height

		v[2] += ent->viewheight;

		// add fall height

		ratio = (ent->client->fall_time - level.time) / FALL_TIME;
		if (ratio < 0)
			ratio = 0;
		v[2] -= ratio * ent->client->fall_value * 0.4f;

		// add bob height

		bob = bobfracsin * xyspeed * bob_up->value;
		if (bob > 6)
			bob = 6;
		//gi.DebugGraph (bob *2, 255);
		v[2] += bob;

		// add kick offset

		VectorAdd(v, ent->client->kick_origin, v);

		// absolutely bound offsets
		if (v[0] < -32)
			v[0] = -32;
		else if (v[0] > 32)
			v[0] = 32;
		if (v[1] < -32)
			v[1] = -32;
		else if (v[1] > 32)
			v[1] = 32;

	}
	else  //If we are in third person perspective calculate the camera view
	{
		xycheck = ent->thirdoffx;
		zcheck = ent->thirdoffz;
		xyzratio = zcheck / xycheck;

		//These calculates the cameras reletive position to the player
		//because the the view offset is not normally reletive to the players angles
		v[0] = cos(DEG2RAD(ent->client->ps.viewangles[YAW])) * xycheck;
		v[1] = sin(DEG2RAD(ent->client->ps.viewangles[YAW])) * xycheck;

		//These Move the camera back and forth depending on the players view pitch
		v[0] = -cos(DEG2RAD(ent->client->ps.viewangles[PITCH])) * v[0];
		v[1] = -cos(DEG2RAD(ent->client->ps.viewangles[PITCH])) * v[1];

		//This calculates the camera height depending on the players view pitch but
		//if the player is ducking keep the height constant

		if (ent->client->ps.pmove.pm_flags & PMF_DUCKED) v[2] = 8;
		else v[2] = 30 + sin(DEG2RAD(ent->client->ps.viewangles[PITCH])) * zcheck;

		//Now lets just check to see if the camera is in a wall. If it is move it forward and repeat if nessecary
		VectorAdd(ent->s.origin, v, tempv);
		while (gi.pointcontents(tempv) & CONTENTS_SOLID)
		{
			xycheck = xycheck - 0.5;
			zcheck = xyzratio * xycheck;;
			v[0] = cos(DEG2RAD(ent->client->ps.viewangles[YAW])) * xycheck;
			v[1] = sin(DEG2RAD(ent->client->ps.viewangles[YAW])) * xycheck;

			v[0] = -cos(DEG2RAD(ent->client->ps.viewangles[PITCH])) * v[0];
			v[1] = -cos(DEG2RAD(ent->client->ps.viewangles[PITCH])) * v[1];

			if (ent->client->ps.pmove.pm_flags & PMF_DUCKED) v[2] = 8;
			else v[2] = 30 + sin(DEG2RAD(ent->client->ps.viewangles[PITCH])) * zcheck;

			VectorAdd(ent->s.origin, v, tempv);
		}

	}

	if (v[2] < -32)
		v[2] = -22;
	else if (v[2] > 30)
		v[2] = 30;

	//Wyrm: chasecam "change view" function
	if (!ent->client->chasetoggle && !ent->client->missile)
	{
		if (v[0] < -14)
			v[0] = -14;
		else if (v[0] > 14)
			v[0] = 14;
		if (v[1] < -14)
			v[1] = -14;
		else if (v[1] > 14)
			v[1] = 14;
		if (v[2] < -22)
			v[2] = -22;
		else if (v[2] > 30)
			v[2] = 30;
	}
	else
	{
		//QW// for chasing the guided nukes and missiles
		VectorSet(v, 0, 0, 0);
		if (ent->client->missile != NULL)
		{
			vec3_t fwd;
			vec3_t neworigin;
			trace_t      tr;

			AngleVectors(ent->client->ps.viewangles, fwd, NULL, NULL);

			VectorMA(ent->client->missile->s.origin, -32, fwd, neworigin);
			tr = gi.trace(ent->client->missile->s.origin, NULL, NULL, neworigin, ent, MASK_SHOT);

			VectorCopy(tr.endpos, neworigin);

			ent->client->ps.pmove.origin[0] = neworigin[0] * 8;
			ent->client->ps.pmove.origin[1] = neworigin[1] * 8;
			ent->client->ps.pmove.origin[2] = neworigin[2] * 8;
			VectorCopy(ent->client->ps.viewangles, ent->client->missile->s.angles);
			VectorScale(fwd, 400, ent->client->missile->velocity);
		}
		else
		{
			if (ent->client->chasecam != NULL)
			{
				//VectorScale(ent->client->chasecam->s.origin, 8, ent->client->ps.pmove.origin);
				ent->client->ps.pmove.origin[0] = ent->client->chasecam->s.origin[0] * 8;
				ent->client->ps.pmove.origin[1] = ent->client->chasecam->s.origin[1] * 8;
				ent->client->ps.pmove.origin[2] = ent->client->chasecam->s.origin[2] * 8;
				VectorCopy(ent->client->chasecam->s.angles, ent->client->ps.viewangles);
			}
		}
	}
	VectorCopy(v, ent->client->ps.viewoffset);
}

/*
==============
SV_CalcGunOffset
==============
*/
void SV_CalcGunOffset(edict_t* ent)
{
	int		i;
	float	delta;

	// gun angles from bobbing
	ent->client->ps.gunangles[ROLL] = xyspeed * bobfracsin * 0.005;
	ent->client->ps.gunangles[YAW] = xyspeed * bobfracsin * 0.01;
	if (bobcycle & 1)
	{
		ent->client->ps.gunangles[ROLL] = -ent->client->ps.gunangles[ROLL];
		ent->client->ps.gunangles[YAW] = -ent->client->ps.gunangles[YAW];
	}

	ent->client->ps.gunangles[PITCH] = xyspeed * bobfracsin * 0.005;

	// gun angles from delta movement
	for (i = 0; i < 3; i++)
	{
		delta = ent->client->oldviewangles[i] - ent->client->ps.viewangles[i];
		if (delta > 180)
			delta -= 360;
		if (delta < -180)
			delta += 360;
		if (delta > 45)
			delta = 45;
		if (delta < -45)
			delta = -45;
		if (i == YAW)
			ent->client->ps.gunangles[ROLL] += 0.1 * delta;
		ent->client->ps.gunangles[i] += 0.2 * delta;
	}

	// gun height
	VectorClear(ent->client->ps.gunoffset);
	//	ent->ps->gunorigin[2] += bob;

	// gun_x / gun_y / gun_z are development tools
	for (i = 0; i < 3; i++)
	{
		ent->client->ps.gunoffset[i] += forward[i] * (gun_y->value);
		ent->client->ps.gunoffset[i] += right[i] * gun_x->value;
		ent->client->ps.gunoffset[i] += up[i] * (-gun_z->value);
	}
}


/*
=============
SV_AddBlend
=============
*/
void SV_AddBlend(float r, float g, float b, float a, float* v_blend)
{
	float	a2, a3;

	if (a <= 0)
		return;
	a2 = v_blend[3] + (1 - v_blend[3]) * a;	// new total alpha
	a3 = v_blend[3] / a2;		// fraction of color from old

	v_blend[0] = v_blend[0] * a3 + r * (1 - a3);
	v_blend[1] = v_blend[1] * a3 + g * (1 - a3);
	v_blend[2] = v_blend[2] * a3 + b * (1 - a3);
	v_blend[3] = a2;
}

#if _WIN32
#pragma warning(push)
#pragma warning(disable : 4305)	// truncation from 'double' to 'float'
#endif

/*
=============
SV_CalcBlend
=============
*/
void SV_CalcBlend(edict_t* ent)
{
	int		contents;
	vec3_t	vieworg = { 0 };
	int		remaining;

	ent->client->ps.blend[0] = ent->client->ps.blend[1] =
		ent->client->ps.blend[2] = ent->client->ps.blend[3] = 0;

	// add for contents
	VectorAdd(ent->s.origin, ent->client->ps.viewoffset, vieworg);
	contents = gi.pointcontents(vieworg);
	if (contents & (CONTENTS_LAVA | CONTENTS_SLIME | CONTENTS_WATER))
		ent->client->ps.rdflags |= RDF_UNDERWATER;
	else
		ent->client->ps.rdflags &= ~RDF_UNDERWATER;

	if (contents & (CONTENTS_SOLID | CONTENTS_LAVA))
		SV_AddBlend(1.0, 0.3, 0.0, 0.6, ent->client->ps.blend);
	else if (contents & CONTENTS_SLIME)
		SV_AddBlend(0.0, 0.1, 0.05, 0.6, ent->client->ps.blend);
	else if (contents & CONTENTS_WATER)
		SV_AddBlend(0.5, 0.3, 0.2, 0.4, ent->client->ps.blend);

	// add for lowlight
	if (ent->client->lowlight)
		SV_AddBlend(LLV_R, LLV_G, LLV_B, LLV_A, ent->client->ps.blend);

	// add for frozen
	if (ent->frozen)
	{
		// Strong blue color, but it fades in the last second.
		float b = ent->frozentime - level.time;
		if (b > 1.0)
			b = 1.0;

		SV_AddBlend(0.0, 0.0, b, 0.5, ent->client->ps.blend);
	}
	if (ent->client->horrortime > level.time)
		SV_AddBlend(random() * 100.0, random() * 100.0, random() * 100.0, random(), ent->client->ps.blend);
	// add for powerups
	if (ent->client->alphatime > 0)
	{
		ent->client->alphatime--;
		SV_AddBlend(0, 0, 0, ent->client->alpha, ent->client->ps.blend);
	}
	else if (ent->client->alpha > 0)
	{
		ent->client->alpha -= 0.01;
		SV_AddBlend(0, 0, 0, ent->client->alpha, ent->client->ps.blend);
	}
	/*ATTILA begin*/
	if (Jet_Active(ent))
	{
		/*GOD -> dont burn out*/
		if (ent->flags & FL_GODMODE)
			if ((ent->client->Jet_framenum - level.framenum) <= 100)
				ent->client->Jet_framenum = level.framenum + 700;
		/*update the fuel time*/
		ent->client->Jet_remaining = ent->client->Jet_framenum - level.framenum;

		/*if no fuel remaining, remove jetpack from inventory*/
		if (ent->client->Jet_remaining == 0)
			ent->client->pers.inventory[ITEM_INDEX(&gI_item_jetpack)] = 0;

		/*Play jetting sound every 0.6 secs (sound of monster icarus)*/
		if (((int)ent->client->Jet_remaining % 6) == 0)
			gi.sound(ent, CHAN_AUTO, gi.soundindex("hover/hovidle1.wav"), 0.9, ATTN_NORM, 0);

		/*beginning to fade if 4 secs or less*/
		if (ent->client->Jet_remaining <= 40)
			/*play on/off sound every sec*/
			if (((int)ent->client->Jet_remaining % 10) == 0)
				gi.sound(ent, CHAN_ITEM, gi.soundindex("items/protect.wav"), 1, ATTN_NORM, 0);
		if (ent->client->Jet_remaining > 40 || ((int)ent->client->Jet_remaining & 4))
			SV_AddBlend(0, 0, 1, 0.08, ent->client->ps.blend);
	}
	else
	{
		/*ATTILA end*/

		if (ent->client->quad_framenum > level.framenum)
		{
			remaining = ent->client->quad_framenum - level.framenum;
			if (remaining == 30)	// beginning to fade
				gi.sound(ent, CHAN_ITEM, gi.soundindex("items/damage2.wav"), 1, ATTN_NORM, 0);
			if (remaining > 30 || (remaining & 4))
				SV_AddBlend(0, 0, 1, 0.08, ent->client->ps.blend);
		}

		/* Wonderslug make screen go colored */
		else if (ent->client->kamikaze_framenum > level.framenum)
		{
			remaining = ent->client->kamikaze_framenum - level.framenum;
			ent->client->kamikaze_timeleft = remaining;
			if (remaining == 30)
				gi.sound(ent, CHAN_ITEM, gi.soundindex("items/protect2.wav"), 1, ATTN_NORM, 0);
			if (remaining > 30 || (remaining & 4))
				SV_AddBlend(0, 0, 1, 0.08, ent->client->ps.blend);
		}
		/* Wonderslug End */

		else if (ent->client->invincible_framenum > level.framenum)
		{
			remaining = ent->client->invincible_framenum - level.framenum;
			if (remaining == 30)	// beginning to fade
				gi.sound(ent, CHAN_ITEM, gi.soundindex("items/protect2.wav"), 1, ATTN_NORM, 0);
			if (remaining > 30 || (remaining & 4))
				SV_AddBlend(1, 1, 0, 0.08, ent->client->ps.blend);
		}
		else if (ent->client->enviro_framenum > level.framenum)
		{
			remaining = ent->client->enviro_framenum - level.framenum;
			if (remaining == 30)	// beginning to fade
				gi.sound(ent, CHAN_ITEM, gi.soundindex("items/airout.wav"), 1, ATTN_NORM, 0);
			if (remaining > 30 || (remaining & 4))
				SV_AddBlend(0, 1, 0, 0.08, ent->client->ps.blend);
		}
		else if (ent->client->breather_framenum > level.framenum)
		{
			remaining = ent->client->breather_framenum - level.framenum;
			if (remaining == 30)	// beginning to fade
				gi.sound(ent, CHAN_ITEM, gi.soundindex("items/airout.wav"), 1, ATTN_NORM, 0);
			if (remaining > 30 || (remaining & 4))
				SV_AddBlend(0.4, 1, 0.4, 0.04, ent->client->ps.blend);
		}

		else if (ent->client->cloak_time > level.time)
		{
			remaining = ent->client->cloak_time - level.time;
			if (remaining == 3)	// beginning to fade
				gi.sound(ent, CHAN_ITEM, gi.soundindex("items/protect2.wav"), 1, ATTN_NORM, 0);
			if (remaining > 3 || (remaining & 4))
				SV_AddBlend(1, 1, 1, 0.08, ent->client->ps.blend);
		}
		// add for damage
		if (ent->client->damage_alpha > 0)
			SV_AddBlend(ent->client->damage_blend[0], ent->client->damage_blend[1],
				ent->client->damage_blend[2], ent->client->damage_alpha, ent->client->ps.blend);

		if (ent->client->bonus_alpha > 0)
			SV_AddBlend(0.85, 0.7, 0.3, ent->client->bonus_alpha, ent->client->ps.blend);

		// drop the damage value
		ent->client->damage_alpha -= 0.06;
		if (ent->client->damage_alpha < 0)
			ent->client->damage_alpha = 0;

		// drop the bonus value
		ent->client->bonus_alpha -= 0.1;
		if (ent->client->bonus_alpha < 0)
			ent->client->bonus_alpha = 0;
	}
}
#if _WIN32
#pragma warning(pop)
#endif

/*
=================
P_FallingDamage
=================
*/
void P_FallingDamage(edict_t* ent)
{
	float	delta;
	int		damage;
	vec3_t	dir = { 0 };

	if (ent->s.modelindex != 255)
		return;		// not in the player model

	if (ent->movetype == MOVETYPE_NOCLIP)
		return;

	if ((ent->client->oldvelocity[2] < 0) && (ent->velocity[2] > ent->client->oldvelocity[2]) && (!ent->groundentity))
	{
		delta = ent->client->oldvelocity[2];
	}
	else
	{
		if (!ent->groundentity)
			return;
		delta = ent->velocity[2] - ent->client->oldvelocity[2];
	}
	delta = delta * delta * 0.0001;

	// never take falling damage if completely underwater
	if (ent->waterlevel == 3)
		return;
	if (ent->waterlevel == 2)
		delta *= 0.25;
	if (ent->waterlevel == 1)
		delta *= 0.5;

	if (delta < 1)
		return;

	if (delta < 15)
	{
		ent->s.event = EV_FOOTSTEP;
		return;
	}

	ent->client->fall_value = delta * 0.5;
	if (ent->client->fall_value > 40)
		ent->client->fall_value = 40;
	ent->client->fall_time = level.time + FALL_TIME;

	if (delta > 30)
	{
		if (ent->health > 0)
		{
			if (delta >= 55)
				ent->s.event = EV_FALLFAR;
			else
				ent->s.event = EV_FALL;
		}
		ent->pain_debounce_time = level.time;	// no normal pain sound
		damage = (delta - 30) / 2;
		if (damage < 1)
			damage = 1;
		VectorSet(dir, 0, 0, 1);

		if (!deathmatch->value || !((int)dmflags->value & DF_NO_FALLING))
			T_Damage(ent, world, world, dir, ent->s.origin, vec3_origin, damage, 0, 0, MOD_FALLING);
	}
	else
	{
		ent->s.event = EV_FALLSHORT;
		return;
	}
}



/*
=============
P_WorldEffects
=============
*/
void P_WorldEffects(void)
{
	qboolean	breather;
	qboolean	envirosuit;
	int			waterlevel, old_waterlevel;

	if (current_player->movetype == MOVETYPE_NOCLIP)
	{
		current_player->air_finished = level.time + 12;	// don't need air
		return;
	}

	waterlevel = current_player->waterlevel;
	old_waterlevel = current_client->old_waterlevel;
	current_client->old_waterlevel = waterlevel;

	breather = current_client->breather_framenum > level.framenum;
	envirosuit = current_client->enviro_framenum > level.framenum;

	//
	// if just entered a water volume, play a sound
	//
	if (!old_waterlevel && waterlevel)
	{
		PlayerNoise(current_player, current_player->s.origin, PNOISE_SELF);
		if (current_player->watertype & CONTENTS_LAVA)
			gi.sound(current_player, CHAN_BODY, gi.soundindex("player/lava_in.wav"), 1, ATTN_NORM, 0);
		else if (current_player->watertype & CONTENTS_SLIME)
			gi.sound(current_player, CHAN_BODY, gi.soundindex("player/watr_in.wav"), 1, ATTN_NORM, 0);
		else if (current_player->watertype & CONTENTS_WATER)
			gi.sound(current_player, CHAN_BODY, gi.soundindex("player/watr_in.wav"), 1, ATTN_NORM, 0);
		current_player->flags |= FL_INWATER;

		// clear damage_debounce, so the pain sound will play immediately
		current_player->damage_debounce_time = level.time - 1;
	}

	//
	// if just completely exited a water volume, play a sound
	//
	if (old_waterlevel && !waterlevel)
	{
		PlayerNoise(current_player, current_player->s.origin, PNOISE_SELF);
		gi.sound(current_player, CHAN_BODY, gi.soundindex("player/watr_out.wav"), 1, ATTN_NORM, 0);
		current_player->flags &= ~FL_INWATER;
	}

	//
	// check for head just going under water
	//
	if (old_waterlevel != 3 && waterlevel == 3)
	{
		gi.sound(current_player, CHAN_BODY, gi.soundindex("player/watr_un.wav"), 1, ATTN_NORM, 0);
	}

	//
	// check for head just coming out of water
	//
	if (old_waterlevel == 3 && waterlevel != 3)
	{
		if (current_player->air_finished < level.time)
		{	// gasp for air
			gi.sound(current_player, CHAN_VOICE, gi.soundindex("player/gasp1.wav"), 1, ATTN_NORM, 0);
			PlayerNoise(current_player, current_player->s.origin, PNOISE_SELF);
		}
		else  if (current_player->air_finished < level.time + 11)
		{	// just break surface
			gi.sound(current_player, CHAN_VOICE, gi.soundindex("player/gasp2.wav"), 1, ATTN_NORM, 0);
		}
	}

	//
	// check for drowning
	//
	if (waterlevel == 3)
	{
		// breather or envirosuit give air
		if (breather || envirosuit)
		{
			current_player->air_finished = level.time + 10;

			if (((int)(current_client->breather_framenum - level.framenum) % 25) == 0)
			{
				if (!current_client->breather_sound)
					gi.sound(current_player, CHAN_AUTO, gi.soundindex("player/u_breath1.wav"), 1, ATTN_NORM, 0);
				else
					gi.sound(current_player, CHAN_AUTO, gi.soundindex("player/u_breath2.wav"), 1, ATTN_NORM, 0);
				current_client->breather_sound ^= 1;
				PlayerNoise(current_player, current_player->s.origin, PNOISE_SELF);
				//FIXME: release a bubble?
			}
		}

		// if out of air, start drowning
		if (current_player->air_finished < level.time)
		{	// drown!
			if (current_player->client->next_drown_time < level.time
				&& current_player->health > 0)
			{
				current_player->client->next_drown_time = level.time + 1;

				// take more damage the longer underwater
				current_player->dmg += 2;
				if (current_player->dmg > 15)
					current_player->dmg = 15;

				// play a gurp sound instead of a normal pain sound
				if (current_player->health <= current_player->dmg)
					gi.sound(current_player, CHAN_VOICE, gi.soundindex("player/drown1.wav"), 1, ATTN_NORM, 0);
				else if (rand() & 1)
					gi.sound(current_player, CHAN_VOICE, gi.soundindex("*gurp1.wav"), 1, ATTN_NORM, 0);
				else
					gi.sound(current_player, CHAN_VOICE, gi.soundindex("*gurp2.wav"), 1, ATTN_NORM, 0);

				current_player->pain_debounce_time = level.time;

				T_Damage(current_player, world, world, vec3_origin, current_player->s.origin, vec3_origin, current_player->dmg, 0, DAMAGE_NO_ARMOR, MOD_WATER);
			}
		}
	}
	else
	{
		current_player->air_finished = level.time + 12;
		current_player->dmg = 2;
	}

	//
	// check for sizzle damage
	//
	if (waterlevel && (current_player->watertype & (CONTENTS_LAVA | CONTENTS_SLIME)))
	{
		if (current_player->watertype & CONTENTS_LAVA)
		{
			if (current_player->health > 0
				&& current_player->pain_debounce_time <= level.time
				&& current_client->invincible_framenum < level.framenum)
			{
				if (rand() & 1)
					gi.sound(current_player, CHAN_VOICE, gi.soundindex("player/burn1.wav"), 1, ATTN_NORM, 0);
				else
					gi.sound(current_player, CHAN_VOICE, gi.soundindex("player/burn2.wav"), 1, ATTN_NORM, 0);
				current_player->pain_debounce_time = level.time + 1;
			}

			if (envirosuit)	// take 1/3 damage with envirosuit
				T_Damage(current_player, world, world, vec3_origin, current_player->s.origin, vec3_origin, 1 * waterlevel, 0, 0, MOD_LAVA);
			else
				T_Damage(current_player, world, world, vec3_origin, current_player->s.origin, vec3_origin, 3 * waterlevel, 0, 0, MOD_LAVA);
		}

		if (current_player->watertype & CONTENTS_SLIME)
		{
			if (!envirosuit)
			{	// no damage from slime with envirosuit
				T_Damage(current_player, world, world, vec3_origin, current_player->s.origin, vec3_origin, 1 * waterlevel, 0, 0, MOD_SLIME);
			}
		}
	}
}


/*
===============
G_SetClientEffects
===============
*/
void G_SetClientEffects(edict_t* ent)
{
	int		pa_type;
	int		remaining;

	ent->s.effects = EF_NONE;
	ent->s.renderfx = 0;

	if (ent->health <= 0 || level.intermissiontime)
		return;

	if (ent->powerarmor_time > level.time)
	{
		pa_type = PowerArmorType(ent);
		if (ent->client->slugarmor == 1)
		{
			ent->s.effects |= EF_COLOR_SHELL;
			ent->s.renderfx |= RF_SHELL_BLUE;
			ent->s.renderfx |= RF_SHELL_GREEN;
			ent->s.renderfx |= RF_GLOW;
		}

		else if (ent->client->cellarmor == 1)
		{
			ent->s.effects |= EF_COLOR_SHELL;
			ent->s.renderfx |= RF_SHELL_RED;
			ent->s.renderfx |= RF_SHELL_GREEN;
			ent->s.renderfx |= RF_GLOW;
		}

		else if (ent->client->shellarmor == 1)
		{
			ent->s.effects |= EF_COLOR_SHELL;
			ent->s.renderfx |= RF_SHELL_RED;
			ent->s.renderfx |= RF_SHELL_BLUE;
			ent->s.renderfx |= RF_TRANSLUCENT;
		}

		else if (ent->client->bulletarmor == 1)
		{
			ent->s.effects |= EF_COLOR_SHELL;
			ent->s.renderfx |= RF_SHELL_RED;
			ent->s.renderfx |= RF_SHELL_BLUE;
			ent->s.renderfx |= RF_SHELL_GREEN;
			ent->s.renderfx |= RF_TRANSLUCENT;
		}

		else if (ent->client->rocketarmor == 1)
		{
			ent->s.effects |= EF_COLOR_SHELL;
			ent->s.renderfx |= RF_SHELL_RED;
			ent->s.renderfx |= RF_TRANSLUCENT;
		}

		else if (ent->client->grenadearmor == 1)
		{
			ent->s.effects |= EF_COLOR_SHELL;
			ent->s.renderfx |= RF_SHELL_GREEN;
			ent->s.renderfx |= RF_TRANSLUCENT;
		}

		else if (pa_type == POWER_ARMOR_SCREEN)
		{
			ent->s.effects |= EF_POWERSCREEN;
		}
		else if (pa_type == POWER_ARMOR_SHIELD)
		{
			ent->s.effects |= EF_COLOR_SHELL;
			ent->s.renderfx |= RF_SHELL_GREEN;
		}

	}

	if (ent->client->quad_framenum > level.framenum
		&& (level.framenum & 8))
	{
		remaining = ent->client->quad_framenum - level.framenum;
		if (remaining > 30 || (remaining & 4))
			ent->s.effects |= EF_QUAD;
	}

	if (ent->client->invincible_framenum > level.framenum)
	{
		remaining = ent->client->invincible_framenum - level.framenum;
		if (remaining > 30 || (remaining & 4))
			ent->s.effects |= EF_PENT;
	}

	// show cheaters!!! and Kamikazes!!!
	if ((ent->flags & FL_GODMODE) || (ent->client->kamikaze_mode))
	{
		ent->s.effects |= EF_COLOR_SHELL;
		ent->s.renderfx |= (RF_SHELL_RED | RF_SHELL_GREEN | RF_SHELL_BLUE);
	}

	// show frozen people
	else if (ent->frozen)
	{
		ent->s.effects |= EF_COLOR_SHELL;
		ent->s.renderfx |= RF_SHELL_BLUE;
		ent->s.effects |= EF_FLAG2;
	}

	if (ent->client->cloak_time > level.time)
	{
		ent->s.effects |= EF_SPHERETRANS;
		if (!VectorLength(ent->velocity))
			ent->svflags |= SVF_NOCLIENT;
		else if (ent->movetype != MOVETYPE_NOCLIP)
			ent->svflags &= ~SVF_NOCLIENT;
	}
	else
	{
		ent->svflags &= ~SVF_NOCLIENT;
		ent->client->invisible = false;
	}
	if (ent->cloaked)
	{
		if (ent->client->cloaktime == level.framenum)  // timed out
		{
			ent->cloaked = 0;					//turn off the cloak
			ent->s.effects &= EF_COLOR_SHELL;
			ent->s.renderfx &= RF_SHELL_GREEN;
			ent->s.effects &= EF_FLAG2;
		}
		else
		{
			if (!ctf->value)
			{
				ent->s.effects |= EF_COLOR_SHELL;	// glow green in DM
				ent->s.renderfx |= RF_SHELL_GREEN;
				ent->s.effects |= EF_FLAG2;
			}
			else if (ent->client->resp.ctf_team == CTF_TEAM1)
			{
				ent->s.effects |= EF_COLOR_SHELL;	// glow red in CTF
				ent->s.renderfx |= RF_SHELL_RED;
				ent->s.effects |= EF_FLAG2;
			}
			else if (ent->client->resp.ctf_team == CTF_TEAM2)
			{
				ent->s.effects |= EF_COLOR_SHELL;	// glow blue in CTF
				ent->s.renderfx |= RF_SHELL_BLUE;
				ent->s.effects |= EF_FLAG2;
			}
		}
	}

	//ROGUE disruptor
	if (ent->client->tracker_pain_framenum > level.framenum)
		ent->s.effects |= EF_TRACKERTRAIL;
}




/*
===============
G_SetClientEvent
===============
*/
void G_SetClientEvent(edict_t* ent)
{
	if (ent->s.event)
		return;

	if (ent->groundentity && xyspeed > 225)
	{
		if ((int)(current_client->bobtime + bobmove) != bobcycle)
			ent->s.event = EV_FOOTSTEP;
	}
}

/*
===============
G_SetClientSound
===============
*/
void G_SetClientSound(edict_t* ent)
{
	char* weap;

	if (ent->client->resp.game_helpchanged != game.helpchanged)
	{
		ent->client->resp.game_helpchanged = game.helpchanged;
		ent->client->resp.helpchanged = 1;
	}

	// help beep (no more than three times)
	if (ent->client->resp.helpchanged && ent->client->resp.helpchanged <= 3 && !(level.framenum & 63))
	{
		ent->client->resp.helpchanged++;
		gi.sound(ent, CHAN_VOICE, gi.soundindex("misc/pc_up.wav"), 1, ATTN_STATIC, 0);
	}


	if (ent->client->pers.weapon)
		weap = ent->client->pers.weapon->classname;
	else
		weap = "";

	if (ent->waterlevel && (ent->watertype & (CONTENTS_LAVA | CONTENTS_SLIME)))
		ent->s.sound = snd_fry;
	else if (strcmp(weap, "weapon_railgun") == 0)
		ent->s.sound = gi.soundindex("weapons/rg_hum.wav");
	else if (strcmp(weap, "weapon_bfg") == 0)
		ent->s.sound = gi.soundindex("weapons/bfg_hum.wav");
	else if (ent->client->weapon_sound)
		ent->s.sound = ent->client->weapon_sound;
	else
		ent->s.sound = 0;
}

/*
===============
G_SetClientFrame
===============
*/
void G_SetClientFrame(edict_t* ent)
{
	gclient_t* client;
	qboolean	duck, run;

	if (ent->frozen)
		return;

	if (ent->s.modelindex != 255)
		return;		// not in the player model

	client = ent->client;

	if (client->ps.pmove.pm_flags & PMF_DUCKED)
		duck = true;
	else
		duck = false;
	if (xyspeed)
		run = true;
	else
		run = false;

	// check for stand/duck and stop/go transitions
	if (duck != client->anim_duck && client->anim_priority != ANIM_DEATH)
		goto newanim;
	if (run != client->anim_run && client->anim_priority == ANIM_BASIC)
		goto newanim;
	if (!ent->groundentity && client->anim_priority <= ANIM_WAVE)
		goto newanim;
	// ### Hentai ### BEGIN
	if (client->anim_priority == ANIM_REVERSE)
	{
		if (ent->s.frame > client->anim_end)
		{
			ent->s.frame--;
			return;
		}
	}
	else if (ent->s.frame < client->anim_end) // ### Hentai ### END


	{	// continue an animation
		ent->s.frame++;
		return;
	}


	if (client->anim_priority == ANIM_DEATH)
		return;		// stay there
	if (client->anim_priority == ANIM_JUMP)
	{
		if (!ent->groundentity)
			return;		// stay there
		ent->client->anim_priority = ANIM_WAVE;
		ent->s.frame = FRAME_jump3;
		ent->client->anim_end = FRAME_jump6;
		return;
	}

newanim:
	// return to either a running or standing frame
	client->anim_priority = ANIM_BASIC;
	client->anim_duck = duck;
	client->anim_run = run;

	if (!ent->groundentity)
	{
		client->anim_priority = ANIM_JUMP;
		if (ent->s.frame != FRAME_jump2)
			ent->s.frame = FRAME_jump1;
		client->anim_end = FRAME_jump2;
	}
	else if (run)
	{	// running
		if (duck)
		{
			ent->s.frame = FRAME_crwalk1;
			client->anim_end = FRAME_crwalk6;
		}
		else
		{
			ent->s.frame = FRAME_run1;
			client->anim_end = FRAME_run6;
		}
	}
	else
	{	// standing
		if (duck)
		{
			ent->s.frame = FRAME_crstnd01;
			client->anim_end = FRAME_crstnd19;
		}
		else
		{
			ent->s.frame = FRAME_stand01;
			client->anim_end = FRAME_stand40;
		}
	}
}


/*
=================
Called for each player at the end of the server frame
and right after spawning
=================
*/
void ClientEndServerFrame(edict_t* ent)
{
	float	bobtime;
	int		i;

	current_player = ent;
	current_client = ent->client;

	//
	// If the origin or velocity have changed since ClientThink(),
	// update the pmove values.  This will happen when the client
	// is pushed by a bmodel or kicked by an explosion.
	// 
	// If it wasn't updated here, the view position would lag a frame
	// behind the body position when pushed -- "sinking into plats"
	//
	for (i = 0; i < 3; i++)
	{
		current_client->ps.pmove.origin[i] = ent->s.origin[i] * 8.0;
		current_client->ps.pmove.velocity[i] = ent->velocity[i] * 8.0;
	}

	//
	// If the end of unit layout is displayed, don't give
	// the player any normal movement attributes
	//
	if (level.intermissiontime)
	{
		// FIXME: add view drifting here?
		current_client->ps.blend[3] = 0;
		current_client->ps.fov = 90;
		G_SetStats(ent);
		return;
	}

	AngleVectors(ent->client->v_angle, forward, right, up);

	// burn from lava, etc
	P_WorldEffects();

	//
	// set model angles from view angles so other things in
	// the world can tell which direction you are looking
	//
	if (ent->client->v_angle[PITCH] > 180)
		ent->s.angles[PITCH] = (-360 + ent->client->v_angle[PITCH]) / 3;
	else
		ent->s.angles[PITCH] = ent->client->v_angle[PITCH] / 3;
	ent->s.angles[YAW] = ent->client->v_angle[YAW];
	ent->s.angles[ROLL] = 0;
	ent->s.angles[ROLL] = SV_CalcRoll(ent->s.angles, ent->velocity) * 4;

	//
	// calculate speed and cycle to be used for
	// all cyclic walking effects
	//
	xyspeed = sqrtf(ent->velocity[0] * ent->velocity[0] + ent->velocity[1] * ent->velocity[1]);

	if (xyspeed < 5)
	{
		bobmove = 0;
		current_client->bobtime = 0;	// start at beginning of cycle again
	}
	else if (ent->groundentity)
	{	// so bobbing only cycles when on ground
		if (xyspeed > 210)
			bobmove = 0.25;
		else if (xyspeed > 100)
			bobmove = 0.125;
		else
			bobmove = 0.0625;
	}

	bobtime = (current_client->bobtime += bobmove);

	if (current_client->ps.pmove.pm_flags & PMF_DUCKED)
		bobtime *= 4;

	bobcycle = (int)bobtime;
	bobfracsin = fabs(sin(bobtime * M_PI));

	// detect hitting the floor
	P_FallingDamage(ent);

	// apply all the damage taken this frame
	P_DamageFeedback(ent);

	// determine the view offsets
	SV_CalcViewOffset(ent);

	// determine the gun offsets
	SV_CalcGunOffset(ent);

	// determine the full screen color blend
	// must be after viewoffset, so eye contents can be
	// accurately determined
	// FIXME: with client prediction, the contents
	// should be determined by the client
	SV_CalcBlend(ent);

	//ZOID
	if (!ent->client->chase_target)
		G_SetStats(ent);

	//ZOID
	//update chasecam follower stats
	for (i = 1; i <= maxclients->value; i++)
	{
		edict_t* e = g_edicts + i;
		if (!ent->inuse || !e->client || e->client->chase_target != ent)
			continue;
		memcpy(e->client->ps.stats,
			ent->client->ps.stats,
			sizeof(ent->client->ps.stats));
		e->client->ps.stats[STAT_LAYOUTS] = 1;
	}
	//ZOID

	G_SetClientEvent(ent);
	G_SetClientEffects(ent);
	G_SetClientSound(ent);
	G_SetClientFrame(ent);

	VectorCopy(ent->velocity, ent->client->oldvelocity);
	VectorCopy(ent->client->ps.viewangles, ent->client->oldviewangles);

	// clear weapon kicks
	VectorClear(ent->client->kick_origin);
	VectorClear(ent->client->kick_angles);

	// if the scoreboard is up, update it
	if (deathmatch->value)
	{
		if (ent->client->showscores && ent->client->menu
			&& !(level.framenum & 31))
		{
			PMenu_Update(ent);
			gi.unicast(ent, false);
		}
		else if ((ent->client->showscores && !(level.framenum & 31))
			|| (ent->client->pers.scanner_active && !(level.framenum & SCANNER_UPDATE_FREQ))
			|| (ent->client->pers.scanner_active & 2))
		{
			DeathmatchScoreboardMessage(ent, ent->enemy);
			ent->client->pers.scanner_active &= ~2;
			gi.unicast(ent, false);
		}
	}

	if (ent->thirdperson)
	{
		gi.unlinkentity(ent->clone);
		ent->clone->s.frame = ent->s.frame;
		VectorCopy(ent->s.origin, ent->clone->s.origin);
		VectorCopy(ent->s.angles, ent->clone->s.angles);
		gi.linkentity(ent->clone);
	}

	if (ent->client->chasetoggle || ent->client->missile)
		CheckChasecam_Viewent(ent);
}

//=================
// Called when starting the ThirdPerson Perspective Mode
//=================
void ThirdBegin(edict_t* ent)
{
	edict_t* clone;

	ent->currentweapon = ent->client->ps.gunindex;
	ent->client->ps.gunindex = 0; //Simple enough Don't show a weapon
	gi.cprintf(ent, PRINT_HIGH, "Chasecam Mode On.\n");

	clone = G_Spawn();
	clone->movetype = MOVETYPE_NONE;
	clone->solid = SOLID_NOT;
	VectorCopy(ent->s.origin, clone->s.origin);
	VectorCopy(ent->s.angles, clone->s.angles);
	clone->s.renderfx = RF_TRANSLUCENT;

	ent->clone = clone;
	clone->s.skinnum = ent->s.skinnum;
	clone->s.modelindex = ent->s.modelindex;
	clone->s.modelindex2 = ent->s.modelindex2;
	clone->s.modelindex3 = ent->s.modelindex3;
	clone->s.modelindex4 = ent->s.modelindex4;

	gi.linkentity(clone);
	ent->thirdperson = true;

	ent->client->ps.pmove.pm_flags |= PMF_NO_PREDICTION;
}

//=================
//  ThirdEnd
//  
//	Called when switching back to FirstPerson Perspective and when the player dies 
//=================
void ThirdEnd(edict_t* ent)
{
	ent->client->ps.gunindex = ent->currentweapon;
	gi.cprintf(ent, PRINT_HIGH, "Chasecam Mode Off.\n");
	ent->thirdperson = false;

	ent->client->ps.pmove.pm_flags &= ~PMF_NO_PREDICTION;

	G_FreeEdict(ent->clone);
}

