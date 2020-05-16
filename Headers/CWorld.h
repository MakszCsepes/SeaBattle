#ifndef SEABATTLE_CWORLD_H
#define SEABATTLE_CWORLD_H

#include "CPlayer.h"
#include <cstring>
#include <SDL2/SDL_ttf.h>


#define PUT_SHIPS 0
#define PLAY_GAME 1
#define ENDGAME 2

const int SCRIPT_VERSION_POSITION_X = 0;
const int SCRIPT_VERSION_POSITION_Y = WINDOW_HEIGHT - 50;

using namespace std;

struct coordinate{
    int i;
    int j;
};

class CWorld : public IDrawable {
    list<coordinate> l;
public:
    CPlayer user;
    CPlayer ai;

    lua_State* lua_state; // state for Lua script
    int game_state; // PUT_SHIPS, PLAY_GAME, ENDGAME
    bool turn;
    bool run_game;

    // text
    TTF_Font* font;
    SDL_Surface* text_surface;
    SDL_Texture* text_texture;

    // SDL gameT
    SDL_Event event;
    int loop_done;
    SDL_Rect mouseRect;
    point2DT mousePoint;
    point2DT mapScroll2Dpos;
    int mapScrollSpeed;
    isoEngineT isoEngine;
    int lastTiledClick;


    int frame_count;
    int old_frame_count;
    double FPS;

    int time_stamp;

    CWorld() = delete;
    CWorld(const CPlayer& User, const CPlayer& AI, int game_state, lua_State* lua, const list<coordinate>& list) {
        this->user = User;
        this->ai = AI;

        this->frame_count = 0;
        this->old_frame_count = 0;
        this->time_stamp = 0;
        this->FPS = 0;
        this->game_state = game_state;

        this->lua_state = lua;

        this->l = list;

        this->turn = USER_TURN;

        this->run_game = true;
        // text
        SDL_Color text_color = {0, 0, 0};
        this->font = TTF_OpenFont("/usr/share/fonts/opentype/noto/NotoSansCJK.ttc", 25);
        this->text_surface = TTF_RenderText_Solid(font, NULL, text_color);
        this->text_texture = SDL_CreateTextureFromSurface(get_renderer(), text_surface);
    }
    CWorld(const CWorld& world) {
        this->l = world.l;
        this->user = world.user;
        this->ai = world.ai;
        this->lua_state = world.lua_state;
        this->turn = world.turn;
        this->game_state = world.game_state;
        this->time_stamp = 0;
        this->run_game = true;
        this->FPS = world.FPS;

        this->font = world.font;
        this->text_surface = world.text_surface;
        this->text_texture = world.text_texture;
    }
    CWorld(CWorld* world) {
        this->l = world->l;

        this->lua_state = world->lua_state;

        this->user = world->user;
        this->ai = world->ai;

        this->turn = world->turn;
        this->game_state = world->game_state;
        this->time_stamp = world->time_stamp;

        this->font = world->font;
        this->text_surface = world->text_surface;
        this->text_texture = world->text_texture;
    }
    ~CWorld() {
        TTF_CloseFont(font);
        SDL_FreeSurface(text_surface);
        SDL_DestroyTexture(text_texture);

        TTF_Quit();
    }

    void draw(isoEngineT*);
    void draw_text(char*, int, int,  SDL_Color );
    void draw_FPS();
    void draw_script_version(lua_State*);
    void change_turn();
    void change_run_game();
    void init_ai();


    Uint32 get_timestamp_now();

    CWorld& operator =(const CWorld* obj) {
        user = obj->user;
        ai = obj->ai;

        game_state = obj->game_state;

        lua_state = obj->lua_state;

        l = obj->l;

        turn = obj->turn;
    }
};

#endif //SEABATTLE_CWORLD_H
