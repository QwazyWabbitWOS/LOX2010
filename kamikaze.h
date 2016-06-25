/* 

kamikaze.h

by Wonderslug

*/
#ifndef KAMIKAZE_H
#define KAMIKAZE_H

/*  Amount of Damage caused */
#define KAMIKAZE_DAMAGE                 600

/* Radius of blast */
#define KAMIKAZE_DAMAGE_RADIUS  800 // Quake Units

/* Count down time */
#define KAMIKAZE_BLOW_TIME               50 // 1/10 seconds

void Start_Kamikaze_Mode(edict_t *the_doomed_one); // setup and start self destruct mode
qboolean Kamikaze_Active(edict_t *the_doomed_one);
void Kamikaze_Explode(edict_t *the_doomed_one);
void Kamikaze_Cancel(edict_t *the_spared_one);

#endif
