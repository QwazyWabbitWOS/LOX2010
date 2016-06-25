
#ifndef L_VACUUMMAKER_H
#define L_VACUUMMAKER_H

#define VAC_MAX_MASS 800
#define VAC_STRENGTH 100
#define VAC_INRANGE	 700
#define VAC_DAMAGE   250
#define VAC_SPEED    500

void fire_vacuummaker (edict_t *self, vec3_t start, vec3_t dir, int damage, int speed, float damage_radius);
void weapon_vacuummaker_fire (edict_t *ent);
void Weapon_VacuumMaker (edict_t *ent);

#endif
