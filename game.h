#define margin_width 10
#define margin_heigth 10

#define WATER_AREA 0
#define MISHIT  1
#define HIT 2

#define INIT_PLAYERS_SHIP 0
#define PLAY_GAME 1
#define ENDGAME 2

#define SHIP_QUANTITY 10

struct ship {
    int coord_x; // logical coordinate 0-9
    int coord_y; // logical coordinate 0-9
    int size;
    int damage;
    bool inverse;
    bool init; // ship was put onto the map
    ship *next;
};

struct player {
    char name[10];
    bool init;
    bool turn;
    int points;
    int offset;

    ship *head;

    int four = 1;
    int three = 2;
    int two = 3;
    int one = 4;
};

struct world {
    player player1;
    player computer;

    int **map_A;
    int **map_B;

    int cursor_x; // 0-9
    int cursor_y; // 0-9

    int state; // 0,1,2

    ship *current_ship;

    int intel_hit_i;
    int intel_hit_j;
    int hit_side;
};

void create_map (int **map);

void delete_map (int **map);

bool can_put_ship (ship *head , ship ship1) ;

bool can_move (const ship *moved_ship ,const  int &cursor_x ,const int &cursor_y) ;

bool check_inverse(ship *head, ship *ship1) ;

bool check_neighbours_down(ship *head, ship ship1) ;

bool check_inversion (ship ship1) ;

void make_inverse (ship *head, ship *current_ship) ;

void add_ship (ship *&head, ship *ship1) ;

ship *create_ship (const ship *head) ;

bool if_ship (world *game, ship *head) ;

void destroyed (ship *head_A, ship *head_B, int **map_A, int **map_B) ;

bool was_hit (int cursor_row, int cursor_column, int **map ) ;
