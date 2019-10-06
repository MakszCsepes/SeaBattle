#include <ctime>
#include "ai.h"
#include <iostream>

#define left 1
#define right 2
#define up 3
#define down 4


void init_computer (world *game) {
    int cursor_row;
    int cursor_column;
    int inverse;
    ship *new_ship;
    ship *current_ship;

    while (game->computer.init == false) {

        new_ship = create_ship(game->computer.head);

        if (new_ship != NULL) {
            current_ship = new_ship;

            srand(time(0));
            cursor_column = rand() % 10;
            cursor_row = rand() % 10;
            inverse = 0 + rand() % (1 - 0 + 1);

            if (inverse == 1) {
                current_ship->inverse = true;
            } else {
                current_ship->inverse = false;
            }

            if (can_move(current_ship, cursor_column, cursor_row)) {
                current_ship->coord_x = cursor_column;
                current_ship->coord_y = cursor_row;

                if (can_put_ship(game->computer.head, *current_ship)) {
                    add_ship(game->computer.head, current_ship);
                }

            }


        } else {
            game->computer.init = true;
        }
    }
}

void intel_hit (world &game , int j , int i) {

    do {

        if (game.hit_side == 5) {
            game.hit_side = 0;
            do_hit(game);
            return;
        }

        game.cursor_x = j;
        game.cursor_y = i;

        switch (game.hit_side) {
            case left: // left
                if (j - 1 >= 0) {
                    game.cursor_x--;
                } else {
                    game.hit_side++;
                }
                break;
            case right: // right
                if (j + 1 <= 9) {
                    game.cursor_x++;
                } else {
                    game.hit_side++;
                }
                break;
            case up: // up
                if (i - 1 >= 0) {
                    game.cursor_y--;
                } else {
                    game.hit_side++;
                }
                break;
            case down: // down
                if (i + 1 <= 9) {
                    game.cursor_y++;
                } else {
                    game.hit_side++;
                }
                break;
            default:
                game.hit_side = 0;
                break;
        }

        game.hit_side++;
    } while (!was_hit(game.cursor_y, game.cursor_x, game.map_A));

    if (if_ship(&game , game.player1.head)) {
        game.hit_side--;

        game.computer.points++;

        game.map_A[game.cursor_y][game.cursor_x] = hit;

        intel_hit(game , game.cursor_x , game.cursor_y);

    } else {
        game.hit_side--;
        if (game.hit_side == 4) {
             game.hit_side = 0;
        } else {
            game.hit_side++;
        }

        game.map_A[game.cursor_y][game.cursor_x] = mishit;
    }

}

bool random_hit (world &game) {
    int i;
    int j;

    do {
        srand(time(0));

        i = rand() % 10;
        j = rand() % 10;

        game.cursor_x = j;
        game.cursor_y = i;
    } while (!was_hit(i , j , game.map_A));

    if (if_ship(&game, game.player1.head)) {
        game.computer.points++;

        game.map_A[game.cursor_y][game.cursor_x] = hit;

        game.intel_hit_i = game.cursor_y;
        game.intel_hit_j = game.cursor_x;
        return true;

    } else {
        game.map_A[game.cursor_y][game.cursor_x] = mishit;

        return false;
    }

}

void do_hit (world &game) {

    if (game.hit_side == 0) {
        if (random_hit(game)) {
            game.hit_side = left;
            intel_hit(game, game.intel_hit_j, game.intel_hit_i);
        }
    } else {
        game.cursor_x = game.intel_hit_j;
        game.cursor_y = game.intel_hit_i;

        intel_hit(game, game.intel_hit_j, game.intel_hit_i);

    }
}
