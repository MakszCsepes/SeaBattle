#include "draw.h"
#include <iostream>

void create_map (int** map) {

    for (int i = 0; i < MARGIN_HEIGHT; i++) {
        map[i] = new int[MARGIN_WIDTH];

        for (int j = 0; j < MARGIN_WIDTH; j++) {
            map[i][j] = WATER_AREA;
        }
    }
}

void delete_map (int** map) {
    for(int i = 0 ; i < MARGIN_HEIGHT ; i++) {
        delete[](map[i]);
    }
}

void add_ship_to_list(ship* &dest_head , ship *ship1) {
    ship *p = new ship;

    if (dest_head == NULL) {
        dest_head = ship1;
        return;
    }

    p = dest_head;

    while (p->next) {
        p = p->next;
    }

    p->next = ship1;

    ship1->next = NULL;
}

bool check_inverse(ship* head , ship* ship1) {
    int count = 1;
    int count2 = 0;
    ship *s = new ship;

    s = head;

    if (ship1->inverse) {
        while (s) {
            while (count2 < s->size) {
                while (count <= ship1->size) {
                    if (s->coord_x == ship1->coord_x + count && s->coord_y + count2 == ship1->coord_y) {

                        return false;
                    }
                    count++;
                }
                count = 1;
                count2++;

            }
            count2 = 0;
            s = s->next;
        }
    } else {
        while (s) {
            while (count2 < s->size) {
                while (count <= ship1->size) {
                    if (s->coord_x + count2 == ship1->coord_x && s->coord_y == ship1->coord_y + count) {
                        return false;
                    }
                    count++;
                }
                count2++;
                count = 1;
            }
            count2 = 0;
            s = s->next;
        }
    }

    return true;
}

bool check_neighbours_down(ship* head , ship ship1) {
    ship *s = new ship;
    int count1 = 0;
    int count2 = 0;

    s = head;

    if ( !ship1.inverse ) {
        while (s) {
            while (count1 < ship1.size) {
                while (count2 < s->size) {
                    if ( ( (ship1.coord_x + 1 == s->coord_x ) || (ship1.coord_x - 1) == s->coord_x ) && (ship1.coord_y + count1 == s->coord_y + count2)) {
                        return false;
                    }
                    if ( (ship1.coord_y  + count1 + 1 == s->coord_y + count2) && (ship1.coord_x + 1 == s->coord_x || ship1.coord_x - 1 == s->coord_x )) {
                        return false;
                    }
                    count2++;
                }
                count2 = 0;
                count1++;
            }
            count1 = 0;
            s = s->next;
        }
    } else if ( ship1.inverse ) {
        while (s) {
            while (count1 < ship1.size) {
                while (count2 < s->size) {
                    if ( ( (ship1.coord_x + 1 == s->coord_x ) || (ship1.coord_x - 1) == s->coord_x ) && (ship1.coord_y + count1  == s->coord_y + count2 )) {
                        return false;
                    }
                    if ( (ship1.coord_y  + count1 + 1 == s->coord_y + count2 ) && (ship1.coord_x + 1 == s->coord_x || ship1.coord_x - 1 == s->coord_x )) {
                        return false;
                    }
                    count2++;
                }
                count2 = 0;
                count1++;
            }
            count1 = 0;
            s = s->next;
        }
    }

    return true;
}

bool check_inversion (ship ship1) { // already inversed ship
    int count = 0;

    if (ship1.inverse) {
        while (count < ship1.size) {
            if (ship1.coord_x + (count ) >= MARGIN_WIDTH ) {
                return false;
            }

            count++;
        }
    } else {
        while (count < ship1.size) {
            if (ship1.coord_y + count >= MARGIN_HEIGHT ) {
                return false;
            }

            count++;
        }
    }

    return true;
}

void make_inverse (ship* head , ship* current_ship) {
    current_ship->inverse = !current_ship->inverse;

    if (check_inversion(*current_ship) && check_inverse(head, current_ship) && check_neighbours_down(head, *current_ship)) {
        return;
    } else {
        current_ship->inverse = !current_ship->inverse;
        return;
    }
}

bool check_collision (const ship& ship1 , const ship& ship2) {
    const int left_top_x = ship2.coord_x - 1;
    const int left_top_y = ship2.coord_y - 1;

    const int right_bottom_x = ship2.inverse ? ship2.coord_x + ship2.size : ship2.coord_x + 1;
    const int right_bottom_y = ship2.inverse ? ship2.coord_y + 1 : ship2.coord_y + ship2.size;

    if (ship1.inverse) {
        for (int i = ship1.coord_x ; i < ship1.coord_x + ship1.size ; i++) {
            const int x = i;
            const int y = ship1.coord_y;

            if ( (x >= left_top_x && x <= right_bottom_x) && (y >= left_top_y && y <= right_bottom_y) ) {
                return true;
            }
        }
    } else {
        for (int i = ship1.coord_y ; i < ship1.coord_y + ship1.size ; i++) {
            const int x = ship1.coord_x;
            const int y = i;

            if ( (x >= left_top_x && x <= right_bottom_x) && (y >= left_top_y && y <= right_bottom_y) ) {
                return true;
            }
        }
    }

    return false;
}

bool can_put_ship (ship* head , ship ship1) {

    if (ship1.inverse) {
        if (ship1.coord_x + ship1.size > MARGIN_WIDTH) {
            return false;
        }
    } else {
        if (ship1.coord_y + ship1.size > MARGIN_HEIGHT) {
            return false;
        }
    }

    ship *s = new ship;
    s = head;

    while (s) {
        if (check_collision(ship1 , *s)) {
            return false;
        }
        s = s->next;
    }

    return true;
}

bool can_move (const ship* moved_ship ,const  int& cursor_x , const int& cursor_y) {

    if (moved_ship->inverse) {
        if ((cursor_x < 0 || cursor_x + moved_ship->size > MARGIN_HEIGHT) || (cursor_y < 0 || cursor_y > MARGIN_WIDTH) ) {
            return false;
        }
    } else {
        if ((cursor_x < 0 || cursor_x > MARGIN_HEIGHT) || (cursor_y + moved_ship->size > MARGIN_WIDTH || cursor_y < 0) ) {
            return false;
        }
    }

    return true;
}

void count_ships(const ship* head, int& cruiser, int& destroyer, int& submarine) {
    const ship *s = new ship;

    s = head;

    while (s) {
        if (s->size == 3) {
            cruiser++;
        } else if (s->size == 2) {
            destroyer++;
        } else if (s->size == 1) {
            submarine++;
        }

        s = s->next;
    }
}

// get 4-cells ship
ship *get_battleship() {
    ship *battleship = new ship;

    battleship->coord_x = 0;
    battleship->coord_y = 0;
    battleship->size = SHIP_SIZE;
    battleship->inverse = HORIZONTAL;
    battleship->damage = 0;
    battleship->next = NULL;

    return battleship;
}
// get 3-cells ship
ship *get_cruiser() {
    ship *cruiser = new ship;

    cruiser->coord_x = 0;
    cruiser->coord_y = 0;
    cruiser->size = 3;
    cruiser->inverse = HORIZONTAL;
    cruiser->damage = 0;
    cruiser->next = NULL;

    return cruiser;
}
// get 2-cells ship
ship *get_destroyer() {
    ship *destroyer = new ship;

    destroyer->coord_x = 0;
    destroyer->coord_y = 0;
    destroyer->size = 2;
    destroyer->inverse = HORIZONTAL;
    destroyer->damage = 0;
    destroyer->next = NULL;

    return destroyer;
}
// get 1-cell ship
ship *get_submarine() {
    ship *submarine = new ship;

    submarine->coord_x = 0;
    submarine->coord_y = 0;
    submarine->size = 1;
    submarine->inverse = HORIZONTAL;
    submarine->damage = 0;
    submarine->next = NULL;

    return submarine;
}

ship *create_ship (const ship *head) {
    if (head == NULL) {
        return get_battleship();
    }

    int cruisers = 0;
    int destroyers = 0;
    int submarines = 0;

    count_ships(head, cruisers, destroyers, submarines);

    if (cruisers < CRUISER_QUANTITY) {

        return get_cruiser();
    } else if (destroyers < DESTROYER_QUANTITY) {

        return get_destroyer();
    } else if (submarines < SUBMARINE_QUANTITY) {

        return get_submarine();
    } else {
        return NULL;
    }

}

bool if_damage (const ship &checked_ship , int cursor_x , int cursor_y) {

    if (checked_ship.inverse) {
        if (checked_ship.coord_y == cursor_y) {
            for (int j = checked_ship.coord_x; j < checked_ship.size + checked_ship.coord_x; j++) {
                if (j == cursor_x) {
                    return true;
                }
            }
        }

    } else {
        if (checked_ship.coord_x == cursor_x) {
            for (int j = checked_ship.coord_y; j < checked_ship.size + checked_ship.coord_y; j++) {
                if (j == cursor_y) {
                    return true;
                }
            }
        }
    }

    return false;
}

bool if_any_ship_damaged_on_position (int &position_x, int &position_y , ship *head) {
    ship *s;

    s = head;

    while (s) {
        if (if_damage(*s, position_x, position_y)) {
            s->damage++;
            return true;
        }
        s = s->next;
    }

    return false;
}

void put_mishits_on_map (int **map, int i_coord, int j_coord, int size, int inverse) {

    if (inverse) {
        for (int j = j_coord - 1 ; j < j_coord + size + 1 ; j++) {
            if (i_coord - 1 != -1) {
                map[i_coord - 1][j] = MISHIT;
            }

            if (i_coord + 1 != 10) {
                map[i_coord + 1][j] = MISHIT;
            }
        }

//        *(*(map + i_coord ) + j_coord - 1) = 1;
        map[i_coord][j_coord - 1] = MISHIT;
//        *(*(map + i_coord ) + j_coord + size ) = 1;
        map[i_coord][j_coord + size] = MISHIT;

    } else {
        for (int i = i_coord - 1 ; i < i_coord + size + 1 ; i++) {
            if (j_coord - 1 != -1 && i != -1 && i != 10) {
//                *(*(map + i ) + j_coord - 1) = 1;
                map[i][j_coord - 1] = MISHIT;
            }

            if (j_coord + 1 != 10 && i != -1 && i != 10) {
//                *(*(map + i ) + j_coord + 1) = 1;
                map[i][j_coord + 1] = MISHIT;
            }
        }

        if (i_coord - 1 != -1) {
//            *(*(map + i_coord - 1) + j_coord) = 1;
            map[i_coord - 1][j_coord] = MISHIT;
        }
        if (i_coord + size != 10) {
//            *(*(map + i_coord + size) + j_coord) = 1;
            map[i_coord + size][j_coord] = MISHIT;
        }

    }
}

void destroyed (ship *head_A , ship *head_B, int **map_A , int ** map_B) {
    ship *s;

    s = head_B;

    while (s) {
        if (s->damage >= s->size) {
            put_mishits_on_map(map_B , s->coord_y , s->coord_x ,s->size , s->inverse);
        }
        s = s->next;
    }

    s = head_A;

    while (s) {
        if (s->damage >= s->size) {
            put_mishits_on_map(map_A , s->coord_y , s->coord_x ,s->size , s->inverse);
        }
        s = s->next;
    }
}

bool was_hit_on_position (int position_x , int position_y , int **map ) {

    if( map[position_y][position_x] == MISHIT || map[position_y][position_x] == HIT ){
        return true;
    } else {
        return false;
    }

}
