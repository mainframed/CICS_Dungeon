/////////////////////////////////////////////////////////////////
// Header file for CICS Dungeon/Rogue
//
// AUTHOR:
//   Philip Young aka Soldier of FORTRAN
//
// 10/08/2022
// License GPL v3
/////////////////////////////////////////////////////////////////


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Required for CICS
#include "dfhaid.h"

// Define if we're in KICKS
#define KICKS 1

/* Colors */
#define BLUE            0xf1
#define RED             0xf2
#define PINK            0xf3
#define GREEN           0xf4
#define TURQUOISE       0xf5
#define YELLOW          0xf6
#define NEUTRAL_WHITE   0xf7
#define BLACK           0xf8
// The following are not supported by all emulators
#define DEEP_BLUE	      0xf9
#define ORANGE          0xfa
#define PURPLE          0xfb
#define PALE_GREEN      0xfc
#define PALE_TURQUOISE  0xfd
#define GREY            0xfe
#define WHITE           0xff
/* Constants */
#define MAXROWS         23 // 24-2 for header and footer
#define MAXCOLS         78 // 80-2 for header and footer
#define MAXSCREEN       10240 //we need a big buffer
#define MAXX 45
#define WTO_MESSAGE     "RGUE - %s"
#define NFO_MESSAGE     "RGUE - TRN: %.4s UserID: %.8s TermID: %.4s"
#define SBA_MESSAGE     "RGUE - SBA ROW %d, COL %d"
#define HEADER_STATS    "%s: %d"
#define SEEKING_BEHAVIOR 2
#define RANDOM_BEHAVIOR  1      
// typedef struct PLAYER {
//   char name[18];
// } player;

// struct PLAYER commarea;

typedef struct Position {
  int y; // row
  int x; // col
} Position;

typedef struct Level {
  int level;
  char ** tiles;
  char ** colors;
  int number_of_rooms;
  struct Room ** rooms;
  struct Monster ** monsters;
  int number_of_monsters;
  struct Player * player;
  Position stairs_up;
  Position stairs_down;
  struct Item * weapon;
  struct Item * potion;
  struct Item * armor;
  int leveled_up;
  char message[80];
} Level;

typedef struct Monster {
  char symbol;
  char string[2];
  char *name;
  char color;
  int health;
  int alive;
  int attack;
  int speed;
  int defense;
  int pathfinding;
  int room_number;
  Position * position;
} Monster;

typedef struct Player {
  char name[19];
  Position * position;
  int hp;
  int gold;
  int atk;
  int def;
  int xp;
  int level;
  int max_health;
 } Player;

typedef struct __mycomm__ {
     char name[19];
 } mycomm;


typedef struct dead {
     char name[19];
     char killed_by[19];
 } dead;

typedef struct Room {
  Position pos; // Top Left
  int height;
  int width;
  struct Door ** doors;
  int number_of_doors;
  // Monster * monsters;
  // Item * items;
} Room;

typedef struct Door {
  Position * position;
  int connected;
} Door;

typedef struct Game {
  Level * levels[6];
  int current_level;
  int number_of_levels;
} Game;

typedef enum {WEAPON_TYPE, POTION_TYPE, ARMOR_TYPE} ItemType;

typedef struct Weapon {
    int attack;
} Weapon;

typedef struct Potion {
    int health;
} Potion;

typedef struct Armor {
    int defense;
} Armor;

typedef struct Item {
  ItemType type;
  Position * position;
  union {
    Weapon * weapon;
    Potion * potion;
    Armor * armor;
  } item;
  char name[80];
  char color;
  char symbol;
  int collected;
} Item;

/*
 * From x3270 project
 * code_table is used to translate buffer 
 * addresses and attributes to the 3270
 * datastream representation
 */
 static unsigned char code_table[64] = {
    0x40, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7,
    0xC8, 0xC9, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F,
    0x50, 0xD1, 0xD2, 0xD3, 0xD4, 0xD5, 0xD6, 0xD7,
    0xD8, 0xD9, 0x5A, 0x5B, 0x5C, 0x5D, 0x5E, 0x5F,
    0x60, 0x61, 0xE2, 0xE3, 0xE4, 0xE5, 0xE6, 0xE7,
    0xE8, 0xE9, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F,
    0xF0, 0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7,
    0xF8, 0xF9, 0x7A, 0x7B, 0x7C, 0x7D, 0x7E, 0x7F,
};

char screen[MAXSCREEN]; // Screen Buffer
int location;               // Index tracker of screen buffer
char wtomsg[126];               // WTO message buffer
// char map[22][78];  // Map buffer
// char colors[22][78]; // Colors buffer


// 3270 Functions 
short encode_sba (int x, int y);
void append_screen(const char *chars, int length);
void sba(int x, int y);
void change_color(char color);
int size_of_screen();
void exit_kicks();

// map.c
Level * create_level(int level);
char ** save_positions();
char ** save_colors();
int draw_level(Level * level);
Room * create_room(int grid, int number_of_doors);
void rooms_setup(Level * level);
int draw_room(Room *room, Level * level);
void connect_doors(Level * level);
void add_stairs(Level * level);

// init.c

void init_screen();
void header(Player * player, int floor);
void footer(Level * level);
void intro(char * name);

// screen.c
void put_tile(int y, int x, char *ch, char color, Level * level);
void draw_tiles(Level * level);
char get_char(int y, int x, char ** tiles) ;
// int walkable(int x,int y, char ** tiles);
void render(Level * level);
void put_items(Level * level);
void put_stairs(Level * level);

// Player
Player * player_setup() ;
int handle_input(int eibaid, Level * level, Game * game);
void put_player(Level * level);
int place_player(Level * level);
void dead_player(Level * level, char * killed_by);
int sync_stats_between_levels(Game * game);
void killed_all_monsters(Level * level);

// rogue.c
void write_op();



// Monsters

int add_monsters(Level * level);
Monster * select_monster(int level);
Monster * create_monster(char symbol, int health, int attack, 
                          int speed, int defense, 
                          int pathfinding, char color, char *name);
int set_starting_position(Monster * monster, Room * room, 
                          Level * level, int room_number);
int move_monsters(Level * level);
int m_seek(Position * start, Position * destination, 
          Level * level, Monster * monster);
void m_random(Position * position, Level * level, Monster * monster);
void put_monster(Monster * monster);
void kill_monster(Monster * monster);

// combat.c
int combat(Level * level, Monster * monster, int order);

// pathfinding.c

int path_find(Position * start, Position * end, Level * level);
int add_position_YX(int ** frontier, int frontier_count, int y, int x);
int add_neighbors_YX(int ** frontier, int *** camefrom,
                     int frontier_count, int y, int x, 
                     Level * level, int monster);
int check_pos(int y, int x, Level * level);

int position_check(int y, int x, Level * level, int monster);

// items.c
