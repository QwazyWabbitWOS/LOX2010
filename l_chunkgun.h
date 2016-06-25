#ifndef CHUNKGUN_H
#define CHUNKGUN_H

void Chunkgun_Fire (edict_t *ent);
void fire_chunk (edict_t *self, vec3_t start, vec3_t dir, int damage, int speed, int mod);
void chunk_touch (edict_t *self, edict_t *other, cplane_t *plane, csurface_t *surf);

#endif