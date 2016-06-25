
/**************************************************/
/*           Quake 2 Backpack Tossing             */
/**************************************************/

// Original code by Maj.Bitch
// http://webadvisor.aupr.edu/noc/Othertutorials/MajBitch/TUTORIAL%20Backpack%20Drop%20Add-On.htm
// QW // Limits on quantities per game maximums by QwazyWabbit 10/27/2007
// Added a header file to make it more modular.

#include "g_local.h"
#include "g_team.h"
#include "m_player.h"
#include "l_backpack.h"

cvar_t *backpacktoss;    // 1 = Enable Backpack dropping

//=========================================================
// Touch function for when backpack is picked up by other player
//=========================================================
void Backpack_Touch(edict_t *pack, edict_t *other, cplane_t *plane, csurface_t *surf) 
{
	int i, quantity, holding;
	
	if (!G_EntExists(other))
		return;
	
	// Immediately turn OFF Flies effect
	pack->s.effects &= ~EF_FLIES;
	pack->s.sound = 0;
	
	// Play standard item pickup sound...
	gi.sound(other, CHAN_ITEM, gi.soundindex("items/pkup.wav"), 1.0, ATTN_NORM, 0);
	
	//QW// Step thru array of items in backpack
	// add items from pack he doesn't already have and obey limits on ammo
	for (i = 0; i < game.num_items; i++) 
	{
		holding = other->client->pers.inventory[ITEM_INDEX(itemlist[i])];
		quantity = pack->packitems[ITEM_INDEX(itemlist[i])];
		if (!quantity) 
			continue;
		if (itemlist[i]->flags & IT_AMMO)
			Add_Ammo (other, itemlist[i], quantity);
		else if (!holding)
			other->client->pers.inventory[ITEM_INDEX(itemlist[i])] += quantity; 
	}
	
	G_FreeEdict(pack);  // Make pack disappear
}

//======================================================
void Backpack_Think(edict_t *pack) 
{
	pack->s.effects = EF_FLIES;	// Turn ON Flies effect around pack
	pack->s.sound = gi.soundindex("infantry/inflies1.wav");	// Play Flies sound effect
	pack->nextthink = level.time + 20.0f;	// Make pack disappear in 20 seconds
	pack->think = G_FreeEdict;
}

void Backpack_ClientToss(edict_t *player) 
{
	int i;
	int quantity;
	edict_t *self;	// the pack entity
	vec3_t forward, right, up;
	
	if (!deathmatch->value)
		return;
	
	// Create backpack entity
	self = G_Spawn();
	
	// ----------------------------
	// Now fill up the Backpack
	// ----------------------------
	
	// Look thru list of all possible items..
	for (i = 0; i < game.num_items; i++)
	{
		if (!itemlist[i]->classname) 
			continue;
	
		// Does player have any of this item in their inventory?
		quantity = player->client->pers.inventory[ITEM_INDEX(itemlist[i])];
		
		if (!quantity) 
			continue;
		
		// Then.. add this item to the backpack
		self->packitems[ITEM_INDEX(itemlist[i])] = quantity; 
	}
	
	// ------------------------------
	// Finish making the pack entity
	// ------------------------------
	
	self->owner = player;
	self->movetype = MOVETYPE_TOSS;
	self->takedamage = DAMAGE_NO;
	self->spawnflags = DROPPED_ITEM | DROPPED_PLAYER_ITEM;
	self->classname = "decoy";
	self->classnum = CN_DECOY;
	self->solid = SOLID_TRIGGER;
	self->svflags = SVF_MONSTER;
	self->s.modelindex = gi.modelindex("models/items/pack/tris.md2");
	
	VectorCopy(player->s.origin, self->s.origin);
	self->s.origin[2] += 32;
	VectorSet(self->mins,-16,-16,-16); // Size of standard Q2 pack
	VectorSet(self->maxs, 16, 16, 16);
	AngleVectors(player->s.angles, forward, right, up);
	VectorScale(forward, 300, self->velocity);
	VectorMA(self->velocity, 200 + crandom() * 10.0f, up, self->velocity);
	VectorMA(self->velocity, crandom() * 10.0f, right, self->velocity);
	VectorClear(self->avelocity); // No angular rotation
	VectorClear(self->s.angles);  // No tilt..
	
	self->touch = Backpack_Touch;
	
	self->nextthink = level.time + 2.0f; // In 2 secs, make Flies effect..
	self->think = Backpack_Think;
	
	gi.linkentity(self);
}

/*
self = G_Spawn();
AngleVectors(owner->client->v_angle, forward, NULL, up);
VectorMA(owner->s.origin, 100, forward, self->s.origin);
VectorMA(self->s.origin, 25, up, self->s.origin);
self->s.modelindex = gi.modelindex("models/items/pack/tris.md2");
self->classname = "decoy";
VectorSet (self->mins, -16, -16, -24);
VectorSet (self->maxs, 16, 16, 32);
self->movetype = MOVETYPE_STEP;
self->solid = SOLID_BBOX;
self->svflags |= SVF_MONSTER;
gi.linkentity (self);
*/
