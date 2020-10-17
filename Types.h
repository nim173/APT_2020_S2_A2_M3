#ifndef TYPES
#define TYPES

#define DEFAULT_TILEBAG_FILE    "default files/default.tilebag"
#define DEFAULT_MOSAIC_FILE     "default files/default.mozaic"
#define INITIAL_TILEBAG_FILE    "initial.tilebag"

#define MOSAIC_DIM              5
#define ADV_MOSAIC_DIM          6
#define NO_OF_PLAYERS           2
#define NO_OF_ROUNDS            5
#define NO_OF_FACTORIES         6
#define CENTER_FACTORY          0
#define TILEBAG_MIN_SIZE        100
#define FACTORY_MAX_SIZE        4
#define FLOOR_LINE_MAX_SIZE     7
#define ADV_FLOOR_LINE_MAX_SIZE 8
#define VALID_TURN_TILES        "BURLYO"
#define EMPTY_SLOT              '.'
#define FIRST_PLAYER_TILE       'F'
#define NUM_TILES_PER_COLOUR    20
#define ADV_MODE_SIXTH_TILE     'O'   
#define MIN_STORAGE_ROW         1
#define CPU_PLAYER_NO           1
#define FLOOR_LINE_INDEX        5

#define NEW_SAVED_GAME_FORMAT   "#myformat"
#define ADV_SAVED_GAME          "advancedMode=on"
#define AI_SAVED_GAME           "AIMode=on"


typedef char Tile;
typedef char** Mosaic;

#endif // TYPES