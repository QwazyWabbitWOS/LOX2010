
//**************************************
// All other angel functions
//**************************************

#include "g_local.h"
#include "x_fire.h"
#include "l_angels.h"

void Angel_Touch(edict_t* ent, edict_t* other, cplane_t* plane, csurface_t* surf)
{
	if (other == world)
	{
		ent->enemy = ent->goalentity;
		ent->goalentity = NULL;
	}
}

void Spawn_Angel(edict_t* ent)
{
	edict_t* angel;

	angel = G_Spawn();
	angel->owner = ent;
	ent->client->angel = angel;

	VectorCopy(ent->s.origin, angel->s.origin);
	angel->movetype = MOVETYPE_FLY;
	angel->solid = SOLID_BBOX;
	//	angel->s.effects |= EF_BLASTER;
	angel->touch = Angel_Touch;
	VectorClear(angel->mins);
	VectorClear(angel->maxs);
	//	angel->s.modelindex = gi.modelindex ("models/objects/gibs/sm_meat/tris.md2");
	angel->nextthink = level.time + FRAMETIME;

	if (ent->client->pers.special == AOLIFE)
	{
		angel->think = AOL_Think;
		angel->classname = "angel of life";
		angel->classnum = CN_AOLIFE;
		angel->s.sound = gi.soundindex("world/comp_hum2.wav");
		angel->s.modelindex = gi.modelindex("models/super2/aolife/tris.md2");
	}

	else if (ent->client->pers.special == AODEATH)
	{
		angel->think = AOD_Think;
		angel->classname = "angel of death";
		angel->classnum = CN_AODEATH;
		angel->s.sound = gi.soundindex("world/amb19.wav");
		angel->s.modelindex = gi.modelindex("models/super2/aodeath/tris.md2");
	}

	else if (ent->client->pers.special == AOMERCY)
	{
		angel->think = AOM_Think;
		angel->classname = "angel of mercy";
		angel->classnum = CN_AOMERCY;
		angel->s.sound = gi.soundindex("world/comp_hum2.wav");
		angel->s.modelindex = gi.modelindex("models/super2/aomercy/tris.md2");
	}

	else if (ent->client->pers.special == AOFROST)
	{
		angel->think = AOF_Think;
		angel->classname = "angel of frost";
		angel->classnum = CN_AOFROST;
		angel->s.sound = gi.soundindex("world/comp_hum2.wav");
		angel->s.modelindex = gi.modelindex("models/super2/aolife/tris.md2");
		angel->s.effects |= EF_COLOR_SHELL;
		angel->s.renderfx |= RF_SHELL_BLUE;
		angel->s.renderfx |= RF_GLOW;

	}

	else if (ent->client->pers.special == AOENERGY)
	{
		angel->think = AOE_Think;
		angel->classname = "angel of energy";
		angel->classnum = CN_AOENERGY;
		angel->s.sound = gi.soundindex("world/comp_hum2.wav");
		angel->s.modelindex = gi.modelindex("models/super2/aomercy/tris.md2");
		angel->s.effects |= EF_COLOR_SHELL;
		angel->s.renderfx |= RF_SHELL_GREEN;
		angel->s.renderfx |= RF_GLOW;

	}

	else if (ent->client->pers.special == AOBLIND)
	{
		angel->think = AOBlind_Think;
		angel->classname = "angel of blindness";
		angel->classnum = CN_AOBLIND;
		angel->s.sound = gi.soundindex("world/comp_hum2.wav");
		angel->s.modelindex = gi.modelindex("models/objects/flash/tris.md2");
		angel->s.effects |= EF_COLOR_SHELL;
		angel->s.renderfx |= RF_SHELL_BLUE;
		angel->s.renderfx |= RF_SHELL_RED;
		angel->s.renderfx |= RF_SHELL_GREEN;
		angel->s.renderfx |= RF_GLOW;

	}

	else if (ent->client->pers.special == AOFLAME)
	{
		angel->think = AOB_Think;
		angel->classname = "angel of flame";
		angel->classnum = CN_AOFLAME;
		angel->s.sound = gi.soundindex("world/comp_hum2.wav");
		angel->s.modelindex = gi.modelindex("models/fire/tris.md2");
		angel->s.renderfx |= RF_GLOW;

	}

	else if (ent->client->pers.special == AOPLAGUE)
	{
		angel->think = AOP_Think;
		angel->classname = "angel of plague";
		angel->classnum = CN_AOPLAGUE;
		angel->s.sound = gi.soundindex("world/comp_hum2.wav");
		angel->s.modelindex = gi.modelindex("models/objects/gibs/head2/tris.md2");
	}

	else if (ent->client->pers.special == AOHORROR)
	{
		angel->think = AOH_Think;
		angel->classname = "angel of horror";
		angel->classnum = CN_AOHORROR;
		angel->s.sound = gi.soundindex("world/comp_hum2.wav");
		angel->s.modelindex = gi.modelindex("models/objects/gibs/arm/tris.md2");
	}


	else if (ent->client->pers.special == AOREVENGE)
	{
		angel->think = AOR_Think;
		angel->classname = "angel of revenge";
		angel->classnum = CN_AOREVENGE;
		angel->s.sound = gi.soundindex("world/amb19.wav");
		angel->s.modelindex = gi.modelindex("models/super2/aodeath/tris.md2");
		angel->s.effects |= EF_COLOR_SHELL;
		angel->s.renderfx |= RF_SHELL_BLUE;
		angel->s.renderfx |= RF_SHELL_GREEN;
		angel->s.renderfx |= RF_GLOW;
		angel->s.effects |= EF_BFG;
	}

	else if (ent->client->pers.special == AOUNFREEZE)
	{
		angel->think = AOU_Think;
		angel->classname = "angel of unfreeze";
		angel->classnum = CN_AOUNFREEZE;
		angel->s.sound = gi.soundindex("world/comp_hum2.wav");
		angel->s.modelindex = gi.modelindex("models/objects/flash/tris.md2");
		angel->s.effects |= EF_COLOR_SHELL;
		angel->s.renderfx |= RF_SHELL_RED;
		angel->s.renderfx |= RF_GLOW;

	}
	else if (ent->client->pers.special == MAGE)
	{
		angel->think = AOM_Think;
		angel->classname = "familiar";
		angel->classnum = CN_MAGE;
		angel->s.sound = gi.soundindex("world/amb19.wav");
		angel->s.modelindex = gi.modelindex("models/super2/aomercy/tris.md2");
	}

	else
	{
		ent->client->angel = NULL;
		G_FreeEdict(angel);
		return;
	}

	gi.linkentity(angel);

}

