#ifndef TYPES
#define TYPES

#define DEFAULT_TILEBAG_FILE    "default files/default.tilebag"
#define DEFAULT_MOSAIC_FILE     "default files/default.mozaic"
#define INITIAL_TILEBAG_FILE    "initial.tilebag"

#define MOSAIC_DIM          5
#define NO_OF_PLAYERS       2
#define NO_OF_ROUNDS        5
#define NO_OF_FACTORIES     6
#define CENTER_FACTORY      0
#define TILEBAG_MIN_SIZE    100
#define FACTORY_MAX_SIZE    4
#define FLOOR_LINE_MAX_SIZE 7
#define VALID_TURN_TILES    "BURLY"
#define EMPTY_SLOT          '.'
#define FIRST_PLAYER_TILE   'F'

typedef char Tile;
typedef char** Mosaic;

#endif // TYPES