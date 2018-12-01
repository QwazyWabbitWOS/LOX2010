/*************************************************
*
* Module:		oakai.c
*
* Author:		John Crickett
*				dll@crickett.demon.co.uk
*
* Date:			13/12/97
*
* Version:		0.0
*
*
* Module description:
* -------------------
*
* Oak AI file
*
*
* Version History:
* ----------------
*
* 0.0 - intitial release
*
* Added to LOX by QwazyWabbit
* Lots of cleanup from 'demo' version
* Added attack mode functions like wf_decoys
*
************************************************/

/************************************************
*
* Includes
*
************************************************/

#include "g_local.h"	// game interfaces
#include "m_player.h"	// standard player frames
#include "oak.h"


/************************************************
*
* Think Functions 
*
************************************************/
/************************************************
*
* Function:	oak_stand
*
* Description: the function used to setup after respawning
*			   or when no targets.
*
* Arguments:
* self  
*
* Returns:
* void
*
*************************************************/

void oak_stand(edict_t *self)
{
	edict_t	*target;

	self->enemy = NULL;

	// look for a target
	target = findradius(NULL, self->s.origin, OAK_FIND_RANGE);
	while (target)
	{
		if (visible(self, target))
		{
			if (infront(self, target))
			{
				if (strcmp(target->classname, "player") == 0)
				{
					//gi.bprintf(PRINT_HIGH, "Oak: in oak_stand selecting you as target\n");
					self->enemy = target;
				}
			}
		}

		// next taget
		target = findradius(target, self->s.origin, OAK_FIND_RANGE);
	}
	
	if (self->enemy != NULL)
	{
		if (!(self->monsterinfo.aiflags & AI_SOUND_TARGET) && (self->monsterinfo.sight))
			self->monsterinfo.sight (self, self->enemy);

		//gi.bprintf(PRINT_HIGH, "Oak: in oak_stand turning to face you\n");	
		OakAI_FaceEnemy(self);
		OakAI_RunFrames(self, FRAME_run1, FRAME_run6);
		self->think = oak_run;
	}
	else
	{
		// run the anim frames
		OakAI_RunFrames(self, FRAME_stand01, FRAME_stand40);
	}
	self->nextthink = level.time + 0.1;
}

/************************************************
*
* Function:	oak_run
*
* Description: the general runaround think function
*
* Arguments:	
* self 
*
* Returns:
* void
*
*************************************************/

void oak_run(edict_t *self)
{
	OakAI_RunFrames(self, FRAME_run1, FRAME_run6);

	OakAI_FaceEnemy(self);

	OakAI_MoveToEnemy(self, OAK_RUN);

	self->nextthink = level.time + 0.1;
}

/************************************************
*
* Function:	oak_pain
*
* Description: what to do when hurt
*
* Arguments:	
* self
* other - who hurt you
* kick 
* damage 
*
* Returns:
* void
*
*************************************************/

void oak_pain (edict_t *self, edict_t *other, float kick, int damage)
{
	if (level.time < self->pain_debounce_time)
		return;
	
	self->pain_debounce_time = level.time + 3;
	gi.sound (self, CHAN_VOICE, oak_sound_pain, 1, ATTN_NORM, 0);

	//gi.bprintf(PRINT_HIGH, "Oak: in oak_pain ouch!\n");
	OakAI_RunFrames(self, FRAME_pain101, FRAME_pain104);
	self->enemy = other;
	self->think = oak_painthink;
	self->nextthink = level.time + 0.1;
	self->monsterinfo.idle_time = level.time + 0.3;
}

void oak_painthink(edict_t *self)
{
	gi.bprintf(PRINT_HIGH, "Oak: in oak_painthink running anim\n");
	if (self->monsterinfo.idle_time >= level.time)
	{
		OakAI_RunFrames(self, FRAME_pain101, FRAME_pain104);
	}
	else
	{
		self->think = oak_run;
	}
	self->nextthink = level.time + 0.1;
}

//
// SIGHT frames
//
void oak_sight(edict_t *self, edict_t *other)
{
	if (random() < 0.5)
		gi.sound (self, CHAN_VOICE, oak_sound_sight1, 1, ATTN_NORM, 0);
	else
		gi.sound (self, CHAN_VOICE, oak_sound_sight2, 1, ATTN_NORM, 0);
}

/************************************************
*
* Function:	oak_die
*
* Description: how to die
*
* Arguments:	
* self
* inflictor
* attacker
* damage
* point 
*
* Returns:
* void
*
*************************************************/

void oak_die (edict_t *self, edict_t *inflictor, edict_t *attacker, int damage, vec3_t point)
{
	int i;

	// throw the weapon
	
//	if (self->health < self.gib_health)
//	{	// gib
		gi.sound (self, CHAN_BODY, gi.soundindex ("misc/udeath.wav"), 1, ATTN_NORM, 0);
		for (i= 0; i < 4; i++)
			ThrowGib (self, "models/objects/gibs/sm_meat/tris.md2", damage, GIB_ORGANIC);

		self->takedamage = DAMAGE_NO;

		// respawn the bot
		OAK_Respawn(self);
//	}
//	else
//	{	// normal death


//	}
}

/************************************************
*
* Function:	OakAI_FaceEnemy
*
* Description: turn to face a enemy
*
* Arguments:	
* self 
*
* Returns:
* void
*
*************************************************/

void OakAI_FaceEnemy(edict_t *self)
{
	vec3_t	v;

	VectorSubtract (self->enemy->s.origin, self->s.origin, v);
	self->ideal_yaw = vectoyaw(v);
	M_ChangeYaw (self);
}

/************************************************
*
* Function:	OakAI_MoveToEnemy
*
* Description: moves bot towards an enemy. assumes enemy dead ahead
*
* Arguments:	
* self
* dist
*
* Returns:
* void
*
*************************************************/

void OakAI_MoveToEnemy(edict_t *self, float dist)
{
	OakAI_MoveToGoal(self, dist);
}

void OakAI_MoveToGoal (edict_t *ent, float dist)
{
	edict_t		*goal;
	
	goal = ent->goalentity;

	if (!ent->groundentity && !(ent->flags & (FL_FLY|FL_SWIM)))
		return;

	if (ent->enemy &&  SV_CloseEnough (ent, ent->enemy, dist) )
	{
		ent->think = oak_standclose;
		ent->nextthink = level.time + 0.1;
		return;
	}
	M_walkmove(ent, ent->ideal_yaw, dist);
}

void oak_standclose(edict_t *self)
{
	float i;

	if (self->enemy &&  SV_CloseEnough (self, self->enemy, OAK_RUN) )
	{
		if (((i = random()) < 0.9) && (self->monsterinfo.idle_time < level.time))
		{
			OakAI_RunFrames(self, FRAME_stand01, FRAME_stand40);
		}
		else
		{
			if (self->monsterinfo.idle_time < level.time)
			{
				self->monsterinfo.idle_time = level.time + 1.0;
			}
			OakAI_Wave(self);
		}
	}
	else
	{
		self->think = oak_stand;
	}
	self->nextthink = level.time + 0.1;
}

/************************************************
*
* Animation Functions 
*
************************************************/
/************************************************
*
* Function:	OakAI_RunFrames
*
* Description: runs the animation frames
*
* Arguments:	
* start
* end 
*
* Returns:
* void
*
*************************************************/

void OakAI_RunFrames(edict_t *self, int start, int end)
{
	if ((self->s.frame < end) && (self->s.frame >= start))
	{
		self->s.frame++;
	}
	else 
	{
		self->s.frame = start;
	}

}

/************************************************
*
* Function:	OakAI_Finger
*
* Description: runs the anim frames
*
* Arguments:	
* start
* end 
*
* Returns:
* void
*
*************************************************/

void OakAI_Finger(edict_t *self)
{
	OakAI_RunFrames(self, FRAME_flip01, FRAME_flip12);	
}

/************************************************
*
* Function:	OakAI_Taunt
*
* Description: runs the anim frames
*
* Arguments:	
* start
* end 
*
* Returns:
* void
*
*************************************************/

void OakAI_Taunt(edict_t *self)
{
	OakAI_RunFrames(self, FRAME_taunt01, FRAME_taunt17);	
}

/************************************************
*
* Function:	OakAI_Wave
*
* Description: runs the anim frames
*
* Arguments:	
* start
* end 
*
* Returns:
* void
*
*************************************************/

void OakAI_Wave(edict_t *self)
{
	OakAI_RunFrames(self, FRAME_wave01, FRAME_wave11);	
}

/************************************************
*
* Function:	OakAI_Salute
*
* Description: runs the anim frames
*
* Arguments:	
* start
* end 
*
* Returns:
* void
*
*************************************************/

void OakAI_Salute(edict_t *self)
{
	OakAI_RunFrames(self, FRAME_salute01, FRAME_salute11);	
}

/************************************************
*
* Function:	OakAI_Point
*
* Description: runs the anim frames
*
* Arguments:	
* start
* end 
*
* Returns:
* void
*
*************************************************/

void OakAI_Point(edict_t *self)
{
	OakAI_RunFrames(self, FRAME_point01, FRAME_point12);	
}

//
// begin the attack methods
//QW//
// copied from wf_decoy.c in LOX
static int shotgun_flash [] = 
{
	MZ2_SOLDIER_SHOTGUN_1, 
	MZ2_SOLDIER_SHOTGUN_2, 
	MZ2_SOLDIER_SHOTGUN_3, 
	MZ2_SOLDIER_SHOTGUN_4, 
	MZ2_SOLDIER_SHOTGUN_5, 
	MZ2_SOLDIER_SHOTGUN_6, 
	MZ2_SOLDIER_SHOTGUN_7, 
	MZ2_SOLDIER_SHOTGUN_8
};

void oak_fire (edict_t *self, int flash_number)
{
	vec3_t	start;
	vec3_t	forward, right, up;
	vec3_t	aim;
	vec3_t	dir;
	vec3_t	end;
	float	r, u;
	int		flash_index;
	
	flash_index = shotgun_flash[flash_number];
	
	AngleVectors (self->s.angles, forward, right, NULL);
	G_ProjectSource (self->s.origin, monster_flash_offset[flash_index], forward, right, start);
	
	if (flash_number == 5 || flash_number == 6)
		VectorCopy (forward, aim);
	else
	{
		VectorCopy (self->enemy->s.origin, end);
		end[2] += self->enemy->viewheight;
		VectorSubtract (end, start, aim);
		vectoangles (aim, dir);
		AngleVectors (dir, forward, right, up);
		
		r = (float)crandom()*1000;
		u = (float)crandom()*500;
		VectorMA (start, 8192, forward, end);
		VectorMA (end, r, right, end);
		VectorMA (end, u, up, end);
		
		VectorSubtract (end, start, aim);
		VectorNormalize (aim);
	}
	
	monster_fire_shotgun (self, start, aim, 2, 1, 
		DEFAULT_SHOTGUN_HSPREAD, 
		DEFAULT_SHOTGUN_VSPREAD, 
		DEFAULT_SHOTGUN_COUNT, 
		flash_index);
}

// Fire weapon
void oak_fire1 (edict_t *self)
{
	oak_fire (self, 0);
}

mframe_t oak_frames_attack1 [] =
{
		ai_charge, 0,  NULL,
		ai_charge, 0,  NULL,
		ai_charge, 0,  oak_fire1,
		ai_charge, 0,  NULL,
		ai_charge, 0,  NULL,
		ai_charge, 0,  NULL,
		ai_charge, 0,  NULL,
		ai_charge, 0,  NULL
};

mmove_t oak_move_attack1 = 
{
	FRAME_attack1, 
	FRAME_attack8, 
	oak_frames_attack1, 
	oak_run
};

void oak_attack(edict_t *self)
{
    self->monsterinfo.currentmove = &oak_move_attack1;
}
