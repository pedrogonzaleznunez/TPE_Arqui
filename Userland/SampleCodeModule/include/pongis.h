#ifndef PONGIS_H_
#define PONGIS_H_

// ###### COLORS ######
#define BACKGROUND_COLOR 0x329bab
#define OBSTACLES_COLOR  0x3bf4c4

// #define CHARACTER_COLOR 0x3bf4c4
#define PLAYER_COLOR 0x12c3fe
#define ENEMY_COLOR  0xfdfdfd
#define BALL_COLOR   0xffd714
#define HOLE_COLOR   0x0f2120

#define RADIUS_FIX 15// Ajuste para la colisión con el hoyo

// ###### SHADOWS ######
#define SHADOW_COLOR 0x527d7a

// ###### CHARACTER POSITIONS ######
#define INITIAL_POS_1_X 300
#define INITIAL_POS_1_Y 500
#define INITIAL_POS_2_X 200
#define INITIAL_POS_2_Y 200

// ###### BALL INITIAL  ######
#define BALL_RADIUS    15// Radio de la bola
#define BALL_INITIAL_X 500
#define BALL_INITIAL_Y 500

#define INITIAL_DIR_1_X -1
#define INITIAL_DIR_1_Y 0
#define INITIAL_DIR_2_X 1
#define INITIAL_DIR_2_Y 0

#define CHARACTER_WIDTH  248
#define CHARACTER_HEIGHT 248
#define FIXED_POINT      1024

#define TOTAL_DIRECTIONS 36
#define DEGREE_STEP      10
#define TWO_PI_FIXED1    (FIXED_POINT * 628 / 100)
#define ANGLE_STEP_FIXED (TWO_PI_FIXED1 / TOTAL_DIRECTIONS)

#define FIELD_WIDTH  1024
#define FIELD_HEIGHT 768

#define PLAYER_RADIUS 20
#define ENEMY_RADIUS  20
#define HOLE_RADIUS   20

#define SPEED 5

// ###### FUNCTIONS ######
void startGame(void);
void welcome(void);
void initializeAllObjects(void);

void drawEnemy(void);
void drawBall(void);
void drawHole(void);
int collided(int x1, int y1, int x2, int y2, int radiusSum);
void pongis(void);


#endif