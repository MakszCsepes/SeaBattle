#include <iostream>
#include <SDL2/SDL.h>
#include "ai.h"
#include <list>

extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

#include <LuaBridge/LuaBridge.h>

using namespace luabridge;

void printMessage(const std::string& s) {
    std::cout << s << " ";
}
void init_list(list<coordinate> &l) {
    list<coordinate> :: iterator iter;
    for(int i = 0 ; i < SHIP_QUANTITY ; i++) {
        for(int j = 0 ; j < SHIP_QUANTITY ; j++) {
            coordinate p = {i, j};
            l.push_back(p);
        }
    }
}
lua_State* load_script() {
    lua_State* L = luaL_newstate();
    luaL_openlibs(L);
    getGlobalNamespace(L).addFunction("printMessage", printMessage);

    luaL_dofile(L, "script.lua");
    lua_pcall(L, 0, 0, 0);

    return L;
}
world* get_game() {
    list<coordinate> l;
    init_list(l);

    lua_State* L = load_script();

    player user = {false, 0, USER_MAP_OFFSET, nullptr};
    int **user_map = new int *[MARGIN_HEIGHT];
    create_map(user_map);

    player computer = {false, 0, AI_MAP_OFFSET, nullptr};
    int **ai_map = new int *[MARGIN_HEIGHT];
    create_map(ai_map);

    int **state_map = new int *[MARGIN_HEIGHT];
    create_map(state_map);

    int cursor_position_x = 0;
    int cursor_position_y = 0;

    ship *current_ship;

    return new world {user, computer, user_map, ai_map, cursor_position_x, cursor_position_y, PUT_SHIPS, current_ship, state_map, L, l};
    }

int main() {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        cout << "SDL_Init Error: " << SDL_GetError() << endl;
        return 0;
    }
    ship *new_ship;
    world *game = get_game();

    SDL_Window *screen = SDL_CreateWindow("NavalBattle", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_ALLOW_HIGHDPI);
    SDL_Renderer *renderer = SDL_CreateRenderer(screen, -1, SDL_RENDERER_SOFTWARE);
    SDL_Event event;

    game->current_ship = create_ship(game->user.ships_list_head);
    bool run_game = true;

    while (run_game) {

        while (SDL_WaitEvent(&event)) {

            draw_world(renderer, *game);

            switch (event.type) {
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym) {
                        case SDLK_RIGHT :
                            game->map_position_x++;

                            if (game->game_state == PUT_SHIPS) {
                                if (game->current_ship->inverse) {
                                    if (game->map_position_x + game->current_ship->size > MARGIN_WIDTH) {
                                        game->map_position_x = 0;
                                    }
                                } else {
                                    if (game->map_position_x > MARGIN_WIDTH - 1) {
                                        game->map_position_x = 0;
                                    }
                                }
                            } else if (game->game_state == PLAY_GAME) {
                                if ( game->map_position_x > MARGIN_HEIGHT - 1) {
                                    game->map_position_x = 0;
                                }
                            }
                            break;
                        case SDLK_LEFT :
                            game->map_position_x--;

                            if (game->game_state == PUT_SHIPS) {

                                if ( game->current_ship->inverse) {
                                    if ( game->map_position_x < 0) {
                                        game->map_position_x = MARGIN_WIDTH -  game->current_ship->size;
                                    }
                                } else {
                                    if ( game->map_position_x < 0) {
                                        game->map_position_x = MARGIN_WIDTH - 1;
                                    }
                                }
                            } else if (game->game_state == PLAY_GAME){
                                if ( game->map_position_x < 0) {
                                    game->map_position_x = MARGIN_HEIGHT - 1;
                                }
                            }
                            break;
                        case SDLK_DOWN :
                            game->map_position_y++;

                            if (game->game_state == PUT_SHIPS) {

                                if ( game->current_ship->inverse) {
                                    if ( game->map_position_y > MARGIN_WIDTH - 1) {
                                        game->map_position_y = 0;
                                    }
                                } else {
                                    if ( game->map_position_y +  game->current_ship->size > MARGIN_WIDTH ) {
                                        game->map_position_y = 0;
                                    }
                                }
                            } else if (game->game_state == PLAY_GAME){
                                if ( game->map_position_y > MARGIN_WIDTH - 1) {
                                    game->map_position_y = 0;
                                }
                            }
                            break;
                        case SDLK_UP :
                            game->map_position_y--;

                            if (game->game_state == PUT_SHIPS) {
                                if ( game->current_ship->inverse) {
                                    if ( game->map_position_y < 0) {
                                        game->map_position_y = MARGIN_WIDTH - 1;
                                    }
                                } else {
                                    if ( game->map_position_y < 0) {
                                        game->map_position_y = MARGIN_WIDTH -  game->current_ship->size;
                                    }
                                }
                            } else if (game->game_state == PLAY_GAME){
                                if ( game->map_position_y < 0) {
                                    game->map_position_y = MARGIN_WIDTH - 1;
                                }
                            }
                            break;
                        case SDLK_i:
                            if (game->game_state == PUT_SHIPS) {
                                make_inverse(game->user.ships_list_head,  game->current_ship);
                            }
                            break;
                        case SDLK_ESCAPE:

                            delete_map(game->user_map);
                            delete_map(game->ai_map);

                            return 0;
                        case SDLK_SPACE:
                            if (game->game_state == PUT_SHIPS) {

                                if (can_put_ship(game->user.ships_list_head,  *game->current_ship)) {

                                    add_ship_to_list(game->user.ships_list_head,  game->current_ship);
                                    new_ship = create_ship(game->user.ships_list_head);

                                    if (new_ship) {
                                        game->current_ship = new_ship;

                                        game->map_position_x = 0;
                                        game->map_position_y = 0;
                                    } else {
                                        game->user.inited = true;
                                        init_computer(game);

                                        game->game_state = PLAY_GAME;
                                        game->map_position_x = 0;
                                        game->map_position_y = 0;
                                    }
                                }
                            } else if (game->game_state == PLAY_GAME) {
                                // whether user got the position or not
                                if ( !was_hit_on_position( game->map_position_x,  game->map_position_y, game->ai_map) ) {
                                    // if position was damaged - put onto HIT, else put MISGIT
                                    if (if_any_ship_damaged_on_position( game->map_position_x,  game->map_position_y, game->ai.ships_list_head)) {
                                        game->user.points++;
                                        game->ai_map[ game->map_position_y][ game->map_position_x] = HIT;
                                    } else {
                                        game->ai_map[ game->map_position_y][ game->map_position_x] = MISHIT;
                                        do_ai_hit(*game);
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

            switch (game->game_state) {
                case PUT_SHIPS:
                    if (game->user.inited && game->ai.inited) {
                        game->game_state = PLAY_GAME;
                    } else if(can_move( game->current_ship,  game->map_position_x,  game->map_position_y)) {
                        game->current_ship->coord_x =  game->map_position_x;
                        game->current_ship->coord_y =  game->map_position_y;
                    }
                    break;
                case PLAY_GAME:
                    if(game->user.points == 20 || game->ai.points == 20) {
                        game->game_state = ENDGAME;
                        break;
                    }
                    game->map_position_y =  game->map_position_y;
                    game->map_position_x =  game->map_position_x;
                    break;
                case ENDGAME:
                    draw_world(renderer, *game);
                    break;
                default:
                    run_game = false;
            }
        }
    }

    return 0;
}

