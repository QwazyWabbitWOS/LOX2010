
/***************************************************************************/
/*                    Spiral Rockets MOD                                   */
/***************************************************************************/

#ifndef SPIRAL_ROCKET_H
#define SPIRAL_ROCKET_H

void mod_fire_rocket2(edict_t *self, vec3_t start, vec3_t dir, vec3_t los, int damage, int speed, float damage_radius, int radius_damage);
void mod_SpiralThink(edict_t *ent);
void Weapon_SpiralRocketLauncher_Fire (edict_t *ent);

#endif //SPIRAL_ROCKET_H
