#ifndef PONGIS_H_
#define PONGIS_H_

// ###### COLORS ######
#define BACKGROUND_COLOR 0x329bab
#define OBSTABLES_COLOR 0x3bf4c4
#define HOLE_COLOR 0x0f2120

// ###### SHADOWS ######
#define SHADOW_COLOR 0x527d7a

typedef enum {
    DIR_EAST = 0,
    DIR_EAST_NORTH = 1,
    DIR_NORTH_EAST = 2,
    DIR_NORTH = 9,
    DIR_WEST = 18,
    DIR_SOUTH = 27,
    DIR_EAST_SOUTH = 35
} Direction;

// ###### FUNCTIONS ######
int startGame(void);
void welcome(void);

#endif