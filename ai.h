#include "draw.h"

void init_computer (world *game) ;
bool random_hit (world &game) ;
void do_hit (world &game) ;

void add_ship (ship *&head , ship *ship1);
bool check_borders (ship *ship1 , int shift , ship *head) ;
bool check_inverse(ship *head , ship *ship1);
void make_inverse (ship *head , int i , int j) ;


