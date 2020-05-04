
#ifndef SEABATTLE_AI_H
#define SEABATTLE_AI_H

#include "World.h"

#define NO_HIT_HERE 0
#define WAS_MISHIT 1
#define WAS_HIT 2
#define WAS_KILL 3

void init_computer (CWorld *game);
void do_ai_hit (CWorld &game);

void add_ship_to_list (CShip *&dest_head , CShip *ship1);
bool check_inverse(CShip *head , CShip *ship1);

#endif //SEABATTLE_AI_H
