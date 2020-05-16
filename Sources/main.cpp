#include <iostream>
#include "CWorld.h"
#include <time.h>

using namespace luabridge;

void printMessage(const std::string& s) {
    std::cout << s << " ";
}
void init_list(list<coordinate>& l) {
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

    luaL_dofile(L, "Resources/Lua Scripts/script 101 .lua");
    lua_pcall(L, 0, 0, 0);
    return L;
}
CWorld* get_game(){
    // init user
    CCursor u_cursor(AI_MAP_OFFSET_X, AI_MAP_OFFSET_Y);
    CMap user_map(USER_MAP_OFFSET_X, USER_MAP_OFFSET_Y, u_cursor);
    char u_name[15];
    strcpy(u_name, "max");
    CPlayer user(u_name, user_map);

    // init ai
    CCursor ai_cursor(USER_MAP_OFFSET_X, USER_MAP_OFFSET_Y);
    CMap ai_map(AI_MAP_OFFSET_X, AI_MAP_OFFSET_Y, ai_cursor);
    char ai_name[15];
    strcpy(ai_name, "AI");
    CPlayer computer(ai_name, ai_map);

    int game_state(PUT_SHIPS);

    lua_State* L(get_new_script());

    list<coordinate> l;
    init_list(l);

    return new CWorld(user, computer, game_state, L, l);
}

gameT gameT1;

SDL_Rect tiles_rect[NUM_ISO_TILES];
textureT tilesTex;

SDL_Rect submarine_rect[TEXTURE_NUMBER_OF_SHIP];
textureT submarineTex;

SDL_Rect destroyer_rect[TEXTURE_NUMBER_OF_SHIP];
textureT destroyerTex;

SDL_Rect cruiser_rect[TEXTURE_NUMBER_OF_SHIP];
textureT cruiserTex;

SDL_Rect battleship_rect[TEXTURE_NUMBER_OF_SHIP];
textureT battleshipTex;

void setupRect(SDL_Rect* rect, int x, int y, int w, int h) {
    rect->x = x;
    rect->y = y;
    rect->w = w;
    rect->h = h;
}
void init_tile_clip(SDL_Rect* tiles_rect, textureT* tilesTex, int w, int h, int iso_size) {
    int x = 0;
    int y = 0;

    texture_init(tilesTex, 0, 0, 0, NULL, NULL, SDL_FLIP_NONE);

    for (int i = 0; i < iso_size; i++) {
        setupRect(&tiles_rect[i], x, y, w, h);
        x += w;
    }
}
void init() {
    gameT1.loop_done = 0;

    init_tile_clip(tiles_rect, &tilesTex, 64, 80, NUM_ISO_TILES);

    init_tile_clip(submarine_rect, &submarineTex, 60, 50, TEXTURE_NUMBER_OF_SHIP);
    init_tile_clip(destroyer_rect, &destroyerTex, 107, 74, TEXTURE_NUMBER_OF_SHIP);
    init_tile_clip(cruiser_rect, &cruiserTex, 126, 92, TEXTURE_NUMBER_OF_SHIP);
    init_tile_clip(battleship_rect, &battleshipTex, 145, 95, TEXTURE_NUMBER_OF_SHIP);

    init_IsoEngine(&gameT1.isoEngine);

    gameT1.isoEngine.scrollX = 200;
    gameT1.isoEngine.scrollY = -200;
    gameT1.mapScroll2Dpos.x = -200;
    gameT1.mapScroll2Dpos.y = 0;

    gameT1.mapScrollSpeed = 3;

    gameT1.lastTiledClick = -1;

    if(load_texture(&tilesTex, "Resources/Images/isotiles_sb_2.png") == 0) {
        fprintf(stderr, "Error, could not load texture : Images/isotiles_sb.png");
        exit(0);
    }

    if(load_texture(&submarineTex, "Resources/Images/submarine_inv.png") == 0) {
        fprintf(stderr, "Error, could not load texture : data/submarine.png");
        exit(0);
    }

    if(load_texture(&destroyerTex, "Resources/Images/destroyer_inv2.png") == 0) {
        fprintf(stderr, "Error, could not load texture : data/destroyer.png");
        exit(0);
    }

    if(load_texture(&cruiserTex, "Resources/Images/cruiser_inv.png") == 0) {
        fprintf(stderr, "Error, could not load texture : data/cruiser.png");
        exit(0);
    }

    if(load_texture(&battleshipTex, "Resources/Images/battleship_inv.png") == 0) {
        fprintf(stderr, "Error, could not load texture : data/battleship.png");
        exit(0);
    }
}

void drawIsoMouse() {
    int correct_X = (gameT1.mapScroll2Dpos.x%32) * 2;
    int correct_Y = (gameT1.mapScroll2Dpos.y%32);

    gameT1.mousePoint.x = (gameT1.mouseRect.x/TILESIZE)*TILESIZE;
    gameT1.mousePoint.y = (gameT1.mouseRect.y/TILESIZE)*TILESIZE;

    // For very other x`position on the map
    if((gameT1.mousePoint.x/TILESIZE)%2) {
        // Move mouse down by half a tile
        // so we can pick isometric tiles on that row as well
        gameT1.mousePoint.y += TILESIZE*0.5;
    }
    texture_renderer_XY_clip(&tilesTex, gameT1.mousePoint.x - correct_X, gameT1.mousePoint.y + correct_Y, &tiles_rect[0]);
}
void get_mouseTile_click(isoEngineT* isoEngine) {
    point2DT point;
    point2DT tileshift;
    point2DT mouse2IsoPoint;

    int correct_X = (gameT1.mapScroll2Dpos.x % 32) * 2;
    int correct_Y = (gameT1.mapScroll2Dpos.y % 32) ;

    //copy mouse point;
    mouse2IsoPoint = gameT1.mousePoint;
    ConverterIsoTo2D(&mouse2IsoPoint);
    //get tile coord
    GetTileCoordinates(&mouse2IsoPoint, &point);

    tileshift.x = correct_X;
    tileshift.y = correct_Y;
    Converter2DToIso(&tileshift);

    point.x -= ((float)isoEngine->scrollX + (float)tileshift.x) / (float)TILESIZE;
    point.y -= ((float)isoEngine->scrollY - (float)tileshift.y) / (float)TILESIZE;

    /*if(point.x >= 0 && point.y >= 0 && point.x < MAP_CELL_WIDTH && point.y < MAP_CELL_HEIGHT) {
        gameT1.lastTiledClick = worldTest[point.y][point.x];
    }*/
}

// tutorial functions
void update()
{
    SDL_GetMouseState(&gameT1.mouseRect.x, &gameT1.mouseRect.y);
}
void update_input_tut() {
    const Uint8* keystate = SDL_GetKeyboardState(NULL);
    while (SDL_PollEvent(&gameT1.event) != 0) {
        switch(gameT1.event.type) {
            case SDL_QUIT:
                gameT1.loop_done = 1;
                break;
            case SDL_KEYDOWN:
                switch (gameT1.event.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        gameT1.loop_done = 1;
                        break;
                }
                break;
            case SDL_MOUSEBUTTONDOWN:
                if(gameT1.event.button.button == SDL_BUTTON_LEFT) {

                    get_mouseTile_click(&gameT1.isoEngine);
                }
                break;
            default:
                break;
        }
    }

    if(keystate[SDL_SCANCODE_W]) {
        gameT1.isoEngine.scrollX += gameT1.mapScrollSpeed;
        gameT1.isoEngine.scrollY += gameT1.mapScrollSpeed;
        gameT1.mapScroll2Dpos.y += gameT1.mapScrollSpeed;

        if(gameT1.mapScroll2Dpos.y > 0) {
            gameT1.mapScroll2Dpos.y = 0;
            gameT1.isoEngine.scrollX -= gameT1.mapScrollSpeed;
            gameT1.isoEngine.scrollY -= gameT1.mapScrollSpeed;
        }
    }
    if(keystate[SDL_SCANCODE_A]) {
        gameT1.isoEngine.scrollX += gameT1.mapScrollSpeed;
        gameT1.isoEngine.scrollY -= gameT1.mapScrollSpeed;
        gameT1.mapScroll2Dpos.x -= gameT1.mapScrollSpeed;
    }
    if(keystate[SDL_SCANCODE_S]) {
        gameT1.isoEngine.scrollX -= gameT1.mapScrollSpeed;
        gameT1.isoEngine.scrollY -= gameT1.mapScrollSpeed;
        gameT1.mapScroll2Dpos.y -= gameT1.mapScrollSpeed;
    }
    if(keystate[SDL_SCANCODE_D]) {
        gameT1.isoEngine.scrollX -= gameT1.mapScrollSpeed;
        gameT1.isoEngine.scrollY += gameT1.mapScrollSpeed;
        gameT1.mapScroll2Dpos.x += gameT1.mapScrollSpeed;
    }
}

void select_state(CWorld* game) {
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
            game->draw(&gameT1.isoEngine);
            break;
        default:
            game->run_game = false;
    }
}
// my own game function
void update_input(CWorld* game, SDL_Event event) {
    if (game->turn == AI_TURN) {
        game->ai.assign_new_hit_coords_from(game->lua_state, game->user);
        if (game->ai.was_ever_hit_on_the_position(game->user.map,game->ai.map.cursor.position_x,game->ai.map.cursor.position_y) == false) {

            game->ai.do_hit(game->user);

            if(!game->ai.get_aim_status()) {
                game->change_turn();
            }
            SDL_Delay(2000);
        }

        return;
    }
    
    while (SDL_PollEvent(&event)) {

        // KEYBOARD EVENTS
        switch (event.type) {
            case SDL_QUIT:
                game->change_run_game();
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    case SDLK_RIGHT :
                        if (game->game_state == PUT_SHIPS) {
                            game->user.current_ship->move(game->user.current_ship->get_current_x() + 1,
                                                          game->user.current_ship->get_current_y());
                            if (game->user.current_ship->get_inverse() == HORIZONTAL) {
                                if (game->user.current_ship->head_coordinate_x + game->user.current_ship->get_size() >
                                    MAP_CELL_WIDTH) {
                                    game->user.current_ship->head_coordinate_x = 0;
                                }
                            } else {
                                if (game->user.current_ship->head_coordinate_x > MAP_CELL_WIDTH - 1) {
                                    game->user.current_ship->head_coordinate_x = 0;
                                }
                            }
                        } else if (game->game_state == PLAY_GAME) {
                            game->user.map.cursor.position_x++;
                            if (game->user.map.cursor.position_x > MAP_CELL_HEIGHT - 1) {
                                game->user.map.cursor.position_x = 0;
                            }
                        }

                        break;
                    case SDLK_LEFT :
                        if (game->game_state == PUT_SHIPS) {
                            game->user.current_ship->move(game->user.current_ship->get_current_x() - 1,
                                                          game->user.current_ship->get_current_y());

                            if (game->user.current_ship->head_coordinate_x < 0) {
                                if (game->user.current_ship->get_inverse() == HORIZONTAL) {
                                    game->user.current_ship->head_coordinate_x =
                                            MAP_CELL_WIDTH - game->user.current_ship->get_size();
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
                            game->user.current_ship->move(game->user.current_ship->get_current_x(),
                                                          game->user.current_ship->get_current_y() + 1);

                            if (game->user.current_ship->get_inverse() == HORIZONTAL) {
                                if (game->user.current_ship->head_coordinate_y > MAP_CELL_WIDTH - 1) {
                                    game->user.current_ship->head_coordinate_y = 0;
                                }
                            } else {
                                if (game->user.current_ship->head_coordinate_y + game->user.current_ship->get_size() >
                                    MAP_CELL_WIDTH) {
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
                            game->user.current_ship->move(game->user.current_ship->get_current_x(),
                                                          game->user.current_ship->get_current_y() - 1);
                            if (game->user.current_ship->get_inverse() == HORIZONTAL) {
                                if (game->user.current_ship->head_coordinate_y < 0) {
                                    game->user.current_ship->head_coordinate_y = MAP_CELL_WIDTH - 1;
                                }
                            } else {
                                if (game->user.current_ship->head_coordinate_y < 0) {
                                    game->user.current_ship->head_coordinate_y =
                                            MAP_CELL_WIDTH - game->user.current_ship->get_size();
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

                        game->change_run_game();
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
                                if (!game->user.was_ever_hit_on_the_position(game->ai.map,
                                                                            game->user.map.cursor.position_x,
                                                                            game->user.map.cursor.position_y)) {
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
    }

    select_state(game);
}

int main(int argc, char* argv[]) {
    srand(time(0));
    init_SDL("Sea_Battle");
    TTF_Init();
    init();

    SDL_Event event;

    CWorld* game = new CWorld(get_game());
    Uint32 frames;
    Uint32 timeStamps;

    while (game->run_game) {
        game->frame_count++;

        frames = game->frame_count - game->old_frame_count;
        if(frames >= 100) {
            timeStamps = game->get_timestamp_now() - game->time_stamp;
            game->FPS = (frames*1000)/timeStamps;

            game->old_frame_count = game->frame_count;

            game->time_stamp = game->get_timestamp_now();
        }

        update_input(game, event);
        game->draw(&gameT1.isoEngine);
    }

    delete game;
    close_down_SDL();
    return 0;
}