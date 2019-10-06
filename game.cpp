#include "game.h"
#include <iostream>

void create_map (int **map) {

    for (int i = 0; i < 10; i++) {
        map[i] = new int[margin_heigth];
    }

    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < margin_heigth; j++) {
            map[i][j] = 0;
        }
    }

}

void add_ship (ship *&head , ship *ship1) {
    ship *p = new ship;

    if (head == NULL) {
        head = ship1;
        return;
    }

    p = head;

    while (p->next) {
        p = p->next;
    }

    p->next = ship1;

    ship1->next = NULL;

}

bool check_inverse(ship *head , ship *ship1) {
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

bool check_neighbours_down(ship *head , ship ship1) {
    ship *s = new ship;
    int count1 = 0;
    int count2 = 0;
    s = head;
    if (ship1.inverse == false) {
        while (s) {
            while (count1 < ship1.size) {
                while (count2 < s->size) {
                    if ( ( (ship1.coord_x + 1 == s->coord_x ) || (ship1.coord_x - 1) == s->coord_x ) && (ship1.coord_y + count1 == s->coord_y + count2)) {
                        return false;
                    }
                    if ( (ship1.coord_y  + count1 + 1 == s->coord_y + count2) && (ship1.coord_x + 1 == s->coord_x || ship1.coord_x - 1 == s->coord_x )) {
                        return false;
                    }
//                    if (ship1.coord_y + count1 + 1== s->coord_y + count2 && ship1.coord_x - 1) {
//                        return false;
//                    }
                    count2++;
                }
                count2 = 0;
                count1++;
            }
            count1 = 0;
            s = s->next;
        }
    } else if (ship1.inverse == true) {
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
            if (ship1.coord_x + (count ) >= margin_width ) {
                return false;
            }

            count++;
        }
    } else {
        while (count < ship1.size) {
            if (ship1.coord_y + count >= margin_heigth ) {
                return false;
            }

            count++;
        }
    }

    return true;
}

void make_inverse (ship *head , ship *current_ship) {

    current_ship->inverse = !current_ship->inverse;
    if (check_inversion(*current_ship) && check_inverse(head, current_ship) && check_neighbours_down(head, *current_ship)) {//                 checking inversed ship
        return;
    } else {
        current_ship->inverse = !current_ship->inverse;
        return;
    }


}

bool check_collision (const ship &ship1 , const ship &ship2) {
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

bool can_put_ship (ship *head , ship ship1) {

    if (ship1.inverse) {
        if (ship1.coord_x + ship1.size > margin_width) {
            return false;
        }
    } else {
        if (ship1.coord_y + ship1.size > margin_heigth) {
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

bool can_move (const ship *moved_ship ,const  int cursor_x , const int cursor_y) {

    if (moved_ship->inverse) {
        if ((cursor_x < 0 || cursor_x + moved_ship->size > margin_heigth) || (cursor_y < 0 || cursor_y > margin_width) ) {
            return false;
        }
    } else {
        if ((cursor_x < 0 || cursor_x > margin_heigth) || (cursor_y + moved_ship->size > margin_width || cursor_y < 0) ) {
            return false;
        }
    }

    return true;
}

ship *create_ship (const ship *head) {

    if (head == NULL) {
        ship *ship1 = new ship;

        ship1->coord_x = 0;
        ship1->coord_y = 0;
        ship1->size = 4;
        ship1->init = false;
        ship1->inverse = true;
        ship1->damage = 0;
        ship1->next = NULL;

        return ship1;
    }

    int three = 0;
    int two = 0;
    int one = 0;

    const ship *s = new ship;

    s = head;

    while (s) {
        if (s->size == 3) {
            three++;
        } else if (s->size == 2) {
            two++;
        } else if (s->size == 1) {
            one++;
        }

        s = s->next;
    }

    if (three < 2) {
        ship *ship1 = new ship;

        ship1->coord_x = 0;
        ship1->coord_y = 0;
        ship1->size = 3;
        ship1->init = false;
        ship1->inverse = true;
        ship1->damage = 0;
        ship1->next = NULL;

        return ship1;
    } else if (two < 3) {
        ship *ship1 = new ship;

        ship1->coord_x = 0;
        ship1->coord_y = 0;
        ship1->size = 2;
        ship1->init = false;
        ship1->inverse = true;
        ship1->damage = 0;
        ship1->next = NULL;

        return ship1;
    } else if (one < 4) {
        ship *ship1 = new ship;

        if (one == 3) {
            one = 3;
        }

        ship1->coord_x = 0;
        ship1->coord_y = 0;
        ship1->size = 1;
        ship1->init = false;
        ship1->inverse = true;
        ship1->damage = 0;
        ship1->next = NULL;

        return ship1;
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

bool if_ship (world *game , ship *head) {
    ship *s;

    s = head;

    while (s) {
        if (if_damage(*s , game->cursor_x , game->cursor_y)) {
            s->damage++;
            return true;
        }
        s = s->next;
    }

    return false;
}

void put_mishits_on_map (int **map , int i_coord , int j_coord , int size , int inverse) {

    if (inverse) {
        for (int j = j_coord - 1 ; j < j_coord + size + 1 ; j++) {
            if (i_coord - 1 != -1) {
                *(*(map + i_coord - 1) + j) = 1;
            }

            if (i_coord + 1 != 10) {
                *(*(map + i_coord + 1) + j) = 1;
            }
        }

        *(*(map + i_coord ) + j_coord - 1) = 1;
        *(*(map + i_coord ) + j_coord + size ) = 1;

    } else {
        for (int i = i_coord - 1 ; i < i_coord + size + 1 ; i++) {
            if (j_coord - 1 != -1 && i != -1 && i != 10) {
                *(*(map + i ) + j_coord - 1) = 1;
            }

            if (j_coord + 1 != 10 && i != -1 && i != 10) {
                *(*(map + i ) + j_coord + 1) = 1;
            }
        }

        if (i_coord - 1 != -1) {
            *(*(map + i_coord - 1) + j_coord) = 1;
        }
        if (i_coord + size != 10) {
            *(*(map + i_coord + size) + j_coord) = 1;
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

bool was_hit (int cursor_row , int cursor_column , int **map ) {

    if ( map[cursor_row][cursor_column] == 1 || map[cursor_row][cursor_column] == 2) {
        return false;
    }

    return true;
}
