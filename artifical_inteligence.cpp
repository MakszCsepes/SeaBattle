#include <ctime>
#include "ai.h"
#include <iostream>
#include <fstream>

#define NO_SIDE 0
#define LEFT 1
#define RIGHT 2
#define UP 3
#define DOWN 4

using namespace std;

void create_coords_array(int **ship_coords, const ship ship) {

    for(int i = 0 ; i < ship.size; i++) {
        ship_coords[i] = new int[2];
    }

    if(ship.inverse == horizontal) {
        for(int i = 0 ; i < ship.size ; i++) {
            ship_coords[i][0] = ship.coord_y;
            ship_coords[i][1] = ship.coord_x + i;
        }
    } else if(ship.inverse == vertical) {
        for(int i = 0 ; i < ship.size ; i++) {
            ship_coords[i][0] = ship.coord_y + i;
            ship_coords[i][1] = ship.coord_x;
        }
    }
}
void copy_from_file1_to_file2(char file1[], char file2[]) {
    ifstream from("/home/max/Programming/Games/SeaBattle/cmake-build-debug/ai_coords.txt");
    ofstream to("/home/max/Programming/Games/SeaBattle/cmake-build-debug/copy.txt");

    if (from.is_open()) { // вызов метода is_open()

    } else {
        cout << "Файл не открыт!\n\n" << endl;
        return;
    }
    string str;

    while(getline(from, str)) {
        to << str << endl;
    }
}
int get_number_of_file_lines() {
    int lines = 0;
    ifstream fin("ai_coords.txt");

    char *str = new char[1024];

    while(!fin.eof()) {
        fin.getline(str, 1024, '\n');
        lines++;
    }
    fin.close();

    return lines;
}
void get_rand_coords(int &cursor_row, int &cursor_col, int &rand_line) {
    ifstream fin("ai_coords.txt");
    int lines = get_number_of_file_lines();

    srand(time(0));
    rand_line = rand() % lines;

    int line_number = 1;
    char c;

    while(fin.get(c)){
        if(c == '\n'){
            line_number++;
        }

        if(line_number == rand_line) {
            fin >> cursor_row;
            fin >> cursor_col;
            return;
        }
    }
}

void recreate_files() {
    ofstream fout("ai_coords.txt");

    for(int i = 0 ; i < 10 ; i++) {
        for(int j = 0 ; j < 10 ; j++) {
            fout << i << " " << j << endl;
        }
    }

    copy_from_file1_to_file2("ai_coords.txt", "copy.txt");
}
int get_line_of_coords(int i, int j, char *file_name) {
    ifstream fin(file_name);
    int line = 0;

    int char1 = 0;
    int char2 = 0;

    while (fin >> char1 && fin >> char2) {
        // if a newline character
        line++;

        // file content not to be deleted
        if (char1 == i && char2 == j) {
            return line;
        }
    }

}
void remove_line_from_file(char *file_name, int n) {
    ifstream from(file_name);
    ofstream ofs;
    ofs.open("temp.txt", ofstream::out);

    // loop getting single characters
    char c;
    int line_no = 1;

    while (from.get(c)) {
        // if a newline character
        if (c == '\n') {
            line_no++;
        }

        // file content not to be deleted
        if (line_no != n) {
            ofs << c;
        }
    }

    ofs.close();
    from.close();
    remove(file_name);
    rename("temp.txt", file_name);

}
void remove_ship_coords_from_file(const ship ship, char *file_name) {
    int **ship_coords = new int*[ship.size];
    create_coords_array(ship_coords, ship);

    for(int i = 0 ; i < ship.size ; i++) {
        remove_line_from_file(file_name, get_line_of_coords(ship_coords[i][0], ship_coords[i][1], file_name));
    }
}

void init_computer (world *game) {
    int cursor_row;
    int cursor_column;
    int inverse;
    ship *new_ship;
    ship *current_ship;
    int file_line;

    while (game->computer.init == false) {

        new_ship = create_ship(game->computer.head);

        if (new_ship != NULL) {
            current_ship = new_ship;

            srand(time(0));

            get_rand_coords(cursor_row, cursor_column, file_line);

            inverse = 0 + rand() % (1 - 0 + 1); // 1(true) - horizontal, 0(false) - vertical

            inverse == horizontal ? current_ship->inverse = horizontal : current_ship->inverse = vertical;

            if (can_move(current_ship, cursor_column, cursor_row)) {
                current_ship->coord_x = cursor_column;
                current_ship->coord_y = cursor_row;

                if (can_put_ship(game->computer.head, *current_ship)) {
                    add_ship(game->computer.head, current_ship);

                    remove_ship_coords_from_file(*current_ship, "ai_coords.txt");
                }

            }

        } else {
            game->computer.init = true;
        }
    }
}

void intel_hit (world &game , int j , int i) {

    do {
        // 5 - NO SIDES
        if (game.hit_side == 5) {
            game.hit_side = NO_SIDE;
            do_hit(game);
            return;
        }

        game.cursor_x = j;
        game.cursor_y = i;

        switch (game.hit_side) {
            case LEFT: // 1 - LEFT
                if (j - 1 >= 0) {
                    game.cursor_x--;
                } else {
                    game.hit_side++;
                }
                break;
            case RIGHT: // 2 - RIGHT
                if (j + 1 <= 9) {
                    game.cursor_x++;
                } else {
                    game.hit_side++;
                }
                break;
            case UP: // 3 - UP
                if (i - 1 >= 0) {
                    game.cursor_y--;
                } else {
                    game.hit_side++;
                }
                break;
            case DOWN: // 4 - DOWN
                if (i + 1 <= 9) {
                    game.cursor_y++;
                } else {
                    game.hit_side++;
                }
                break;
            default:
                game.hit_side = NO_SIDE;
                break;
        }

        game.hit_side++;
    } while (!was_hit(game.cursor_y, game.cursor_x, game.map_A));

    if (if_ship(&game , game.player1.head)) {
        game.hit_side--;

        game.computer.points++;

        game.map_A[game.cursor_y][game.cursor_x] = HIT;

        intel_hit(game , game.cursor_x , game.cursor_y);

    } else {
        game.hit_side--;
        if (game.hit_side == DOWN) {
             game.hit_side = NO_SIDE;
        } else {
            game.hit_side++;
        }

        game.map_A[game.cursor_y][game.cursor_x] = MISHIT;
    }

}

bool random_hit (world &game) {
    int line_in_file;

    get_rand_coords(game.cursor_y, game.cursor_x, line_in_file);

    remove_line_from_file("enemy_coords.txt", line_in_file);
    if (if_ship(&game, game.player1.head)) {
        game.computer.points++;

        game.map_A[game.cursor_y][game.cursor_x] = HIT;

        game.intel_hit_i = game.cursor_y;
        game.intel_hit_j = game.cursor_x;

        return true;
    } else {
        game.map_A[game.cursor_y][game.cursor_x] = MISHIT;

        return false;
    }

}

void do_hit (world &game) {

    if (game.hit_side == NO_SIDE) {
        if (random_hit(game)) {
            game.hit_side = LEFT;
            intel_hit(game, game.intel_hit_j, game.intel_hit_i);
        }
    } else {
        game.cursor_x = game.intel_hit_j;
        game.cursor_y = game.intel_hit_i;

        intel_hit(game, game.intel_hit_j, game.intel_hit_i);
    }
}
