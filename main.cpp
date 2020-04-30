#include <iostream>
#include "World.h"

#include "InitClose.h"
#include "Texture.h"
#include "IsoEngine.h"

using namespace luabridge;

void printMessage(const std::string& s) {
    std::cout << s << " ";
}
void init_list(list<coordinate>& l) {
    list<coordinate> :: iterator iter;
    for(int i = 0 ; i < SHIP_QUANTITY ; i++) {
        for(int j = 0 ; j < SHIP_QUANTITY ; j++) {
            coordinate p = {i, j};
            l.push_back(p);
        }
    }
}
lua_State* get_new_script() {
    lua_State* L = luaL_newstate();
    luaL_openlibs(L);
    getGlobalNamespace(L).addFunction("printMessage", printMessage);

    luaL_dofile(L, "../script 101 .lua");
    lua_pcall(L, 0, 0, 0);
    return L;
}
CWorld* get_game(){
    CCursor u_cursor(AI_MAP_OFFSET_X, USER_MAP_OFFSET_Y);
    CMap user_map(USER_MAP_OFFSET_X, USER_MAP_OFFSET_Y, 0, 0, u_cursor);
    char name[15];
    strcpy(name, "undefined");
    CPlayer user(name, user_map);

    CCursor ai_cursor(USER_MAP_OFFSET_X, USER_MAP_OFFSET_Y);
    CMap ai_map(AI_MAP_OFFSET_X, AI_MAP_OFFSET_Y, 0, 0, ai_cursor);
    strcpy(name, "AI");
    CPlayer computer(name, ai_map);

    int game_state(PUT_SHIPS);

    lua_State* L(get_new_script());

    list<coordinate> l;
    init_list(l);

    return new CWorld(user, computer, game_state, L, l);
}

#define NUM_ISO_TILES 5
typedef struct gameT {
    SDL_Event event;
    int loop_done;
    SDL_Rect mouseRect;
    point2DT mousePoint;
    isoEngineT isoEngine;
}gameT;
gameT game;
textureT tilesTex;
SDL_Rect tiles_rect[NUM_ISO_TILES];

void setupRect(SDL_Rect* rect, int x, int y, int w, int h) {
    rect->x = x;
    rect->y = y;
    rect->w = w;
    rect->h = h;
}
void init_tile_clip() {
    int x = 0;
    int y = 0;

    texture_init(&tilesTex, 0, 0, 0, NULL, NULL, SDL_FLIP_NONE);
    for (int i = 0; i < NUM_ISO_TILES; i++) {
        setupRect(&tiles_rect[i], x, y, 64, 80);
        x += 64;
    }
}

void init() {
    game.loop_done = 0;
    int tileSize = 32;
    init_tile_clip();
    init_IsoEngine(&game.isoEngine, tileSize);
    IsoEngineSetMapSize(&game.isoEngine, 16, 16);

    if(load_texture(&tilesTex, "data/isotiles.png") == 0) {
        fprintf(stderr, "Error, could not load texture : data/isotiles.png");
        exit(0);
    }

}
void drawIsoMouse() {
    game.mousePoint.x = (game.mouseRect.x/TILESIZE)*TILESIZE;
    game.mousePoint.y = (game.mouseRect.y/TILESIZE)*TILESIZE;

    // For very other x`position on the map

    if((game.mousePoint.x/TILESIZE)%2) {
        // Move mouse down by half a tile
        // so we can pick isometric tiles on that row as well
        game.mousePoint.y += TILESIZE*0.5;
    }
    texture_renderer_XY_clip(&tilesTex, game.mousePoint.x, game.mousePoint.y, &tiles_rect[0]);
}
void draw() {
    SDL_SetRenderDrawColor(get_renderer(), 0, 0, 0, 0);
    SDL_RenderClear(get_renderer());

    texture_renderer_XY_clip(&tilesTex, game.mouseRect.x, game.mouseRect.y, &tiles_rect[2]);
    drawIsoMouse();
    SDL_RenderPresent(get_renderer());

    SDL_Delay(10);
}
void update() {
    SDL_GetMouseState(&game.mouseRect.x, &game.mouseRect.y);
}
void update_input() {
    while (SDL_PollEvent(&game.event) != 0) {
        switch(game.event.type) {
            case SDL_QUIT:
                game.loop_done = 1;
                break;
            default:
                break;
        }
    }
}

int main(int argc, char* argv[]) {
    srand(time(0));

    init_SDL("Naval_Battle");
    init();
    while (!game.loop_done) {
        update();
        update_input();
        draw();
    }

    return 0;
    SDL_Event event;

    CWorld* game = new CWorld(get_game());
    bool run_game = true;

    while (run_game) {

        while (SDL_PollEvent(&event)) {

            game->draw(get_renderer());

            // KEYBOARD EVENTS
            switch (event.type) {
                case SDL_QUIT:
                    run_game = !run_game;
                    break;
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym) {
                        case SDLK_RIGHT :

                            if (game->game_state == PUT_SHIPS) {
                                game->user.current_ship->head_coordinate_x++;
                                if (game->user.current_ship->get_inverse() == HORIZONTAL) {
                                    if (game->user.current_ship->head_coordinate_x + game->user.current_ship->get_size() > MAP_CELL_WIDTH) {
                                        game->user.current_ship->head_coordinate_x = 0;
                                    }
                                } else {
                                    if (game->user.current_ship->head_coordinate_x > MAP_CELL_WIDTH - 1) {
                                        game->user.current_ship->head_coordinate_x = 0;
                                    }
                                }
                            } else if (game->game_state == PLAY_GAME) {
                                game->user.map.cursor.position_x++;
                                if ( game->user.map.cursor.position_x > MAP_CELL_HEIGHT - 1) {
                                    game->user.map.cursor.position_x = 0;
                                }
                            }
                            break;
                        case SDLK_LEFT :

                            if (game->game_state == PUT_SHIPS) {
                                game->user.current_ship->head_coordinate_x--;

                                if (game->user.current_ship->head_coordinate_x < 0) {
                                    if (game->user.current_ship->get_inverse() == HORIZONTAL) {
                                        game->user.current_ship->head_coordinate_x = MAP_CELL_WIDTH - game->user.current_ship->get_size();
                                    } else {
                                        game->user.current_ship->head_coordinate_x = MAP_CELL_WIDTH - 1;
                                    }
                                }
                            } else if (game->game_state == PLAY_GAME) {
                                game->user.map.cursor.position_x--;
                                if (game->user.map.cursor.position_x < 0) {
                                    game->user.map.cursor.position_x = MAP_CELL_HEIGHT - 1;
                                }
                            }
                            break;
                        case SDLK_DOWN :

                            if (game->game_state == PUT_SHIPS) {
                                game->user.current_ship->head_coordinate_y++;


                                if (game->user.current_ship->get_inverse() == HORIZONTAL) {
                                    if (game->user.current_ship->head_coordinate_y > MAP_CELL_WIDTH - 1) {
                                        game->user.current_ship->head_coordinate_y = 0;
                                    }
                                } else {
                                    if (game->user.current_ship->head_coordinate_y + game->user.current_ship->get_size() > MAP_CELL_WIDTH ) {
                                        game->user.current_ship->head_coordinate_y = 0;
                                    }
                                }
                            } else if (game->game_state == PLAY_GAME){
                                game->user.map.cursor.position_y++;
                                if (game->user.map.cursor.position_y > MAP_CELL_WIDTH - 1) {
                                    game->user.map.cursor.position_y = 0;
                                }
                            }
                            break;
                        case SDLK_UP :

                            if (game->game_state == PUT_SHIPS) {
                                game->user.current_ship->head_coordinate_y--;
                                if (game->user.current_ship->get_inverse() == HORIZONTAL) {
                                    if ( game->user.current_ship->head_coordinate_y < 0) {
                                        game->user.current_ship->head_coordinate_y = MAP_CELL_WIDTH - 1;
                                    }
                                } else {
                                    if (game->user.current_ship->head_coordinate_y < 0) {
                                        game->user.current_ship->head_coordinate_y = MAP_CELL_WIDTH - game->user.current_ship->get_size();
                                    }
                                }
                            } else if (game->game_state == PLAY_GAME) {
                                game->user.map.cursor.position_y--;
                                if (game->user.map.cursor.position_y < 0) {
                                    game->user.map.cursor.position_y = MAP_CELL_WIDTH - 1;
                                }
                            }
                            break;
                        case SDLK_r:
                            game->lua_state = get_new_script();
                            break;
                        case SDLK_i:
                            if (game->game_state == PUT_SHIPS) {
                                game->user.current_ship->change_inverse();
                            }
                            break;
                        case SDLK_ESCAPE:
                            run_game = !run_game;
                        case SDLK_SPACE:
                            if (game->game_state == PUT_SHIPS) {
                                if (game->user.can_put_ship()) {
                                    game->user.add_ship_to_player_array();

                                    if((game->user.current_ship = game->user.get_new_ship()) == nullptr) {
                                        game->user.change_inited();
                                        game->user.map.cursor.change_hidden();

                                        game->init_ai();
                                    }
                                }
                            } else if (game->game_state == PLAY_GAME) {
                                if(game->turn == USER_TURN) {
                                    if (game->user.was_ever_hit_on_the_position(game->ai.map,game->user.map.cursor.position_x,game->user.map.cursor.position_y) == false) {
                                        game->user.do_hit(game->ai);
                                        if (!game->user.get_aim_status()) {
                                            game->change_turn();
                                        }
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

            if (game->turn == AI_TURN) {
                game->ai.assign_new_hit_coords_from(game->lua_state, game->user);
                if (game->ai.was_ever_hit_on_the_position(game->user.map,game->ai.map.cursor.position_x,game->ai.map.cursor.position_y) == false) {
                    game->ai.do_hit(game->user);

                    if(!game->ai.get_aim_status()) {
                        SDL_Delay(200);
                        game->change_turn();
                    }
                }
            }
            // SELECT STATE
            switch (game->game_state) {
                case PUT_SHIPS:
                    if (game->user.get_init_status() and game->ai.get_init_status()) {
                        game->game_state = PLAY_GAME;
                    }
                    break;
                case PLAY_GAME:
                    if(game->user.get_points() == 20 or game->ai.get_points() == 20) {
                        game->game_state = ENDGAME;
                        break;
                    }
                    break;
                case ENDGAME:
                    game->draw(get_renderer());
                    break;
                default:
                    run_game = false;
            }
        }

    }

    close_down_SDL();

    return 0;
}