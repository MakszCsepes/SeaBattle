#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "ai.h"

using namespace std;

int main() {

    player person1 = {"Max", false, true, 0, 0, NULL};
    int **map1 = new int *[10];
    create_map(map1);

    player computer = {"PC", false, false, 0, 425, NULL};
    int **map2 = new int *[10];
    create_map(map2);

    int **map = new int *[margin_width];
    create_map(map);

    int cursor_row = 0;
    int cursor_column = 0;

    ship *current_ship;
    ship *new_ship;

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        cout << "SDL_Init Error: " << SDL_GetError() << endl;
        return 0;
    }

    world game = {person1, computer, map1, map2, cursor_column, cursor_row, 0, current_ship};

    // running : 0 - initialization
    //           1 - play
    //           2 - results

    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();

    SDL_Window *screen = SDL_CreateWindow("NavalBattle", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1000, 1000,
                                          SDL_WINDOW_ALLOW_HIGHDPI);

    SDL_Renderer *renderer = SDL_CreateRenderer(screen, -1, SDL_RENDERER_SOFTWARE);

    SDL_Event event;

    game.player1 = person1;
    game.computer = computer;

    bool run_game = true;

    if (!game.player1.init) {

        current_ship = create_ship(person1.head);

        game.current_ship = current_ship;
    }

    while (run_game) {

        if (game.running == 0) {
            if (game.player1.init && game.computer.init) {
                game.running = 1;
            }
        } else if (game.running == 1){

        }

        while (SDL_WaitEvent(&event)) {

            draw_world(renderer, game);

            switch (event.type) {
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym) {
                        case SDLK_RIGHT :
                            cursor_column++;

                            if (game.running == 0) {//                                                                  inialization
                                if (current_ship->inverse) {
                                    if (cursor_column + current_ship->size > margin_width) {
                                        cursor_column = 0;
                                    }
                                } else {
                                    if (cursor_column > margin_width - 1) {
                                        cursor_column = 0;
                                    }
                                }
                            } else if (game.running == 1){
                                if (cursor_column > margin_heigth - 1) {
                                    cursor_column = 0;
                                }
                            }
                            break;
                        case SDLK_LEFT :
                            cursor_column--;

                            if (game.running == 0) {//                                                                  inialization

                                if (current_ship->inverse) {
                                    if (cursor_column < 0) {
                                        cursor_column = margin_width - current_ship->size;
                                    }
                                } else {
                                    if (cursor_column < 0) {
                                        cursor_column = margin_width - 1;
                                    }
                                }
                            } else if (game.running == 1){
                                if (cursor_column < 0) {
                                    cursor_column = margin_heigth - 1;
                                }
                                //                                                                                     game
                            }
                            break;
                        case SDLK_DOWN :
                            cursor_row++;

                            if (game.running == 0) {//                                                                  inialization

                                if (current_ship->inverse) {
                                    if (cursor_row > margin_width - 1) {
                                        cursor_row = 0;
                                    }
                                } else {
                                    if (cursor_row + current_ship->size > margin_width ) {
                                        cursor_row = 0;
                                    }
                                }
                            } else if (game.running == 1){
                                if (cursor_row > margin_width - 1) {
                                    cursor_row = 0;
                                }
                                //                                                                                      game
                            }
                            break;
                        case SDLK_UP :
                            cursor_row--;

                            if (game.running == 0) {//                                                                  inialization
                                if (current_ship->inverse) {
                                    if (cursor_row < 0) {
                                        cursor_row = margin_width - 1;
                                    }
                                } else {
                                    if (cursor_row < 0) {
                                        cursor_row = margin_width - current_ship->size;
                                    }
                                }
                            } else if (game.running == 1){
                                if (cursor_row < 0) {
                                    cursor_row = margin_width - 1;
                                }
                                //                                                                                      game
                            }
                            break;
                        case SDLK_i:
                            if (game.running == 0) {//                                                                  inialization
                                make_inverse(person1.head, current_ship);
                            }
                            break;
                        case SDLK_ESCAPE:
                            run_game = false;
                            return 0;
                        case SDLK_SPACE:
                            if (game.running == 0) {//                                                                  inialization

                                if (can_put_ship(game.player1.head, *current_ship)) {

                                    add_ship(game.player1.head, current_ship);

                                    new_ship = create_ship(game.player1.head);

                                    if (new_ship != NULL) {
                                        current_ship = new_ship;

                                        game.current_ship = current_ship;
                                        cursor_column = 0;
                                        cursor_row = 0;
                                    } else {
                                        game.player1.init = true;
                                        //init_computer(&game);// TODO init computer
                                        game.computer.head = game.player1.head;
                                        game.running = 1;
                                        cursor_column = 0;
                                        cursor_row = 0;
                                    }
                                }
                            } else if (game.running == 1){
                                if (was_hit(game.cursor_y , game.cursor_x , game.map_B)) {
                                    if (if_ship(&game, game.computer.head)) {
                                        game.player1.points++;

                                        *(*(game.map_B + game.cursor_y) + game.cursor_x) = 2;
                                    } else {
                                        *(*(game.map_B + game.cursor_y) + game.cursor_x) = 1;
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
            if (game.running == 0) {
                if (can_move(current_ship, cursor_column, cursor_row)) {
                    current_ship->coord_x = cursor_column;
                    current_ship->coord_y = cursor_row;
                }
            } else if (game.running == 1) {
                if (game.player1.points == 20 || game.computer.points == 20) {
                    game.running = 2;

                    break;
                }
                game.cursor_y = cursor_row;
                game.cursor_x = cursor_column;
            }

        }

        if (game.running != 2) {
            draw_world(renderer, game);
        }
    }

    return 0;
}

