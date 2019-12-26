#include "draw.h"

void init_computer (world *game) ;
bool random_hit (world &game) ;
void do_hit (world &game) ;

void add_ship (ship *&head , ship *ship1);
bool check_inverse(ship *head , ship *ship1);
void recreate_files();