#include "g_local.h" 

void UnicastSound(edict_t* ent, int flags)
{
	int svc_sound = 9; //hacking past the server
	flags = 10;

	gi.WriteByte(svc_sound);
	gi.WriteByte(flags); // attenuation and channel?
	gi.WriteByte(gi.soundindex("numberone.wav"));
	gi.WriteByte(0);	//attenuation
	gi.WriteShort(0);	//channel
	gi.unicast(ent, true);
}

/*
for (i=0; i<maxclients->value; i++)
{
		ent = g_edicts + 1 + i;
		if (!ent->client || !ent->inuse)
			continue;
		if (!ent->client->resp.disable_sounds)
			UnicastSound(ent, gi.soundindex(gset_vars->numberone_wav));
}
*/
/*
[12:55] <Lmty_> at least it doesn't crash now
[12:56] <Lmty_> the packet if formed well:
[12:56] <Lmty_> void UnicastSound(edict_t *ent, int soundindex_)
[12:56] <Lmty_> {
[12:56] <Lmty_> 	gi.WriteByte (9);	//svc_sound
[12:56] <Lmty_> 	gi.WriteByte (10);	//flags
[12:56] <Lmty_> 	gi.WriteByte (soundindex_); //soundindex
[12:56] <Lmty_> 	gi.WriteByte(0);	//attenuation
[12:56] <Lmty_> 	gi.WriteShort(0);	//channel
[12:56] <Lmty_> 	gi.unicast (ent, true);
[12:56] <Lmty_> }
*/