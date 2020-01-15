#include "draw.h"

#define NO_HIT_HERE 0
#define WAS_HIT 1
#define WAS_MISHIT 2
#define WAS_KILL 3

void init_computer (world *game);
void do_ai_hit (world &game);

void add_ship_to_list (ship *&dest_head , ship *ship1);
bool check_inverse(ship *head , ship *ship1);