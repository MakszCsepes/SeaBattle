#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "ai.h"

using namespace std;

int main() {

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        cout << "SDL_Init Error: " << SDL_GetError() << endl;
        return 0;
    }

    player person1 = {"Max", false, true, 0, 0, NULL};
    int **map1 = new int *[SHIP_QUANTITY];
    create_map(map1);

    player computer = {"PC", false, false, 0, 425, NULL};
    int **map2 = new int *[SHIP_QUANTITY];
    create_map(map2);

    int cursor_row = 0;
    int cursor_column = 0;

    ship *current_ship;
    ship *new_ship;
    world game = {person1, computer, map1, map2, cursor_column, cursor_row, INIT_PLAYERS_SHIP, current_ship};


    TTF_Init();

    SDL_Window *screen = SDL_CreateWindow("NavalBattle", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, window_width, window_height,SDL_WINDOW_ALLOW_HIGHDPI);

    SDL_Renderer *renderer = SDL_CreateRenderer(screen, -1, SDL_RENDERER_SOFTWARE);

    SDL_Event event;

    bool run_game = true;

    if (!game.player1.init) {

        current_ship = create_ship(person1.head);

        game.current_ship = current_ship;
    }

    while (run_game) {

        while (SDL_WaitEvent(&event)) {

            draw_world(renderer, game);

            switch (event.type) {
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym) {
                        case SDLK_RIGHT :
                            cursor_column++;

                            if (game.state == INIT_PLAYERS_SHIP) {
                                if (current_ship->inverse) {
                                    if (cursor_column + current_ship->size > margin_width) {
                                        cursor_column = 0;
                                    }
                                } else {
                                    if (cursor_column > margin_width - 1) {
                                        cursor_column = 0;
                                    }
                                }
                            } else if (game.state == PLAY_GAME){
                                if (cursor_column > margin_heigth - 1) {
                                    cursor_column = 0;
                                }
                            }
                            break;
                        case SDLK_LEFT :
                            cursor_column--;

                            if (game.state == INIT_PLAYERS_SHIP) {

                                if (current_ship->inverse) {
                                    if (cursor_column < 0) {
                                        cursor_column = margin_width - current_ship->size;
                                    }
                                } else {
                                    if (cursor_column < 0) {
                                        cursor_column = margin_width - 1;
                                    }
                                }
                            } else if (game.state == PLAY_GAME){
                                if (cursor_column < 0) {
                                    cursor_column = margin_heigth - 1;
                                }
                            }
                            break;
                        case SDLK_DOWN :
                            cursor_row++;

                            if (game.state == INIT_PLAYERS_SHIP) {

                                if (current_ship->inverse) {
                                    if (cursor_row > margin_width - 1) {
                                        cursor_row = 0;
                                    }
                                } else {
                                    if (cursor_row + current_ship->size > margin_width ) {
                                        cursor_row = 0;
                                    }
                                }
                            } else if (game.state == PLAY_GAME){
                                if (cursor_row > margin_width - 1) {
                                    cursor_row = 0;
                                }
                            }
                            break;
                        case SDLK_UP :
                            cursor_row--;

                            if (game.state == INIT_PLAYERS_SHIP) {
                                if (current_ship->inverse) {
                                    if (cursor_row < 0) {
                                        cursor_row = margin_width - 1;
                                    }
                                } else {
                                    if (cursor_row < 0) {
                                        cursor_row = margin_width - current_ship->size;
                                    }
                                }
                            } else if (game.state == PLAY_GAME){
                                if (cursor_row < 0) {
                                    cursor_row = margin_width - 1;
                                }
                            }
                            break;
                        case SDLK_i:
                            if (game.state == INIT_PLAYERS_SHIP) {
                                make_inverse(person1.head, current_ship);
                            }
                            break;
                        case SDLK_ESCAPE:
                            recreate_files();

                            delete_map(game.map_A);
                            delete_map(game.map_B);
                            return 0;
                        case SDLK_SPACE:
                            if (game.state == INIT_PLAYERS_SHIP) {

                                if (can_put_ship(game.player1.head, *current_ship)) {

                                    add_ship(game.player1.head, current_ship);

                                    new_ship = create_ship(game.player1.head);

                                    if (new_ship) {
                                        current_ship = new_ship;

                                        game.current_ship = current_ship;
                                        cursor_column = 0;
                                        cursor_row = 0;
                                    } else {
                                        game.player1.init = true;
                                        init_computer(&game);// TODO init computer
                                        game.state = PLAY_GAME;
                                        cursor_column = 0;
                                        cursor_row = 0;
                                    }
                                }
                            } else if (game.state == PLAY_GAME) {
                                if (was_hit(game.cursor_y , game.cursor_x , game.map_B)) {
                                    if (if_ship(&game, game.computer.head)) {
                                        game.player1.points++;

                                        game.map_B[game.cursor_y][game.cursor_x] = HIT;
                                    } else {
                                        game.map_B[game.cursor_y][game.cursor_x] = MISHIT;
                                        do_hit(game);
                                    }
                                }
                            }
                            break;
                        default :
                            break;
                    }
                    break;
                default:
                    break;
            }


            if(game.state == INIT_PLAYERS_SHIP) {
                if (game.player1.init && game.computer.init) {
                    game.state = PLAY_GAME;
                } else if(can_move(current_ship, cursor_column, cursor_row)) {
                    current_ship->coord_x = cursor_column;
                    current_ship->coord_y = cursor_row;
                }
            } else if(game.state == PLAY_GAME) {
                if(game.player1.points == 20 || game.computer.points == 20) {
                    game.state = ENDGAME;
                    break;
                }
                game.cursor_y = cursor_row;
                game.cursor_x = cursor_column;
            } else if(game.state != ENDGAME){
                draw_world(renderer, game);
            } else {
                run_game = false;
            }

        }
    }

    return 0;
}

