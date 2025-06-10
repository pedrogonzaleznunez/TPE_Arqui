#ifndef PONGIS_H_
#define PONGIS_H_

// ###### COLORS ######
#define BACKGROUND_COLOR 0x006400 // 0x329bab
#define OBSTACLES_COLOR  //0x3bf4c4

// #define CHARACTER_COLOR 0x3bf4c4
#define PLAYER_COLOR_1 0x8B0000 //0x0033FF
#define PLAYER_COLOR_2 0x301934 //0x024A86
#define ENEMY_COLOR    0xfdfdfd
#define BALL_COLOR     0xFFFFFF //0xffd714
#define HOLE_COLOR     0x000000 //0x0f2120

#define MAX_SPEED_X 3.0f
#define MAX_SPEED_Y 4.0f

#define PLAYER_ACCELERATION 1
#define PLAYER_MAX_VELOCITY 5
#define BALL_BASE_SPEED 3

#define PLAYER_WIDTH  40   // o el valor real del sprite
#define PLAYER_HEIGHT 40   // idem
#define BALL_RADIUS   10   // o el tamaño correspondiente

#define RADIUS_FIX 15// Ajuste para la colisión con el hoyo

// ###### SHADOWS ######
#define SHADOW_COLOR 0x527d7a

// ###### CHARACTER POSITIONS per level ######
// Player 1 and Player 2 initial positions
// LEVEL 1
#define INITIAL_POS_1_X_L1 400
#define INITIAL_POS_1_Y_L1 100
#define INITIAL_POS_2_X_L1 200
#define INITIAL_POS_2_Y_L1 200

// LEVEL 2
#define INITIAL_POS_1_X_L2 200
#define INITIAL_POS_1_Y_L2 100
#define INITIAL_POS_2_X_L2 400
#define INITIAL_POS_2_Y_L2 600

// LEVEL 3
#define INITIAL_POS_1_X_L3 200
#define INITIAL_POS_1_Y_L3 300
#define INITIAL_POS_2_X_L3 600
#define INITIAL_POS_2_Y_L3 400

// ###### BALL INITIAL  ######

// #define BALL_RADIUS 15// Radio de la bola

// Initial position of the ball per level
// LEVEL 1
#define BALL_INITIAL_X_L1 300
#define BALL_INITIAL_Y_L1 500

// LEVEL 2
#define BALL_INITIAL_X_L2 600
#define BALL_INITIAL_Y_L2 200

// LEVEL 3
#define BALL_INITIAL_X_L3 100
#define BALL_INITIAL_Y_L3 300
// ###### HOLE INITIAL  ######
#define HOLE_RADIUS        20// Radio del hoyo
#define HOLE_SHADOW_RADIUS 22// Radio de la sombra del hoyo

// Initial position of the hole per level
#define HOLE_INITIAL_X_L1 700
#define HOLE_INITIAL_Y_L1 500

#define HOLE_INITIAL_X_L2 600
#define HOLE_INITIAL_Y_L2 400

#define HOLE_INITIAL_X_L3 500
#define HOLE_INITIAL_Y_L3 300


#define INITIAL_DIR_1_X -1
#define INITIAL_DIR_1_Y 0
#define INITIAL_DIR_2_X 1
#define INITIAL_DIR_2_Y 0
#define INITIAL_DIR_3_X 0
#define INITIAL_DIR_3_Y -1

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

// ###### SCANCODES ######
#define W_SCANCODE           0x11
#define A_SCANCODE           0x1E
#define S_SCANCODE           0x1F
#define D_SCANCODE           0x20
#define ARROW_UP_SCANCODE    0x48
#define ARROW_DOWN_SCANCODE  0x50
#define ARROW_LEFT_SCANCODE  0x4B
#define ARROW_RIGHT_SCANCODE 0x4D

// ###### FUNCTIONS ######
void startGame(void);
void welcome(void);
void initializeAllObjects(void);

void drawEnemy(int l);
void drawBall(int l);
void drawHole(int l);
int collided(int x1, int y1, int x2, int y2, int radiusSum);
void pongis(void);


#endif