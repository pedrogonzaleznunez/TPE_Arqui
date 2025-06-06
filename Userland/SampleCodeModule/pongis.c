
// #include <pongis.h>
// #include <stdio.h>
// #include <syscalls.h>
// #include "character_data.h"

// // ################################# CONSTANTES #################################

// #define INITIAL_POS_1_X (SQUARE_DIM * 28)
// #define INITIAL_POS_1_Y (SQUARE_DIM * 21)
// #define INITIAL_POS_2_X (SQUARE_DIM * 3)
// #define INITIAL_POS_2_Y (SQUARE_DIM * 4)

// // initial directions
// #define INITIAL_DIR_1_X -1
// #define INITIAL_DIR_1_Y 0
// #define INITIAL_DIR_2_X 1
// #define INITIAL_DIR_2_Y 0

// #define CHARACTER_WIDTH 248
// #define CHARACTER_HEIGHT 248
// #define FIXED_POINT 1024 // para representar 1.0 como 1024

// #define TOTAL_DIRECTIONS 36
// #define DEGREE_STEP 10
// #define TWO_PI_FIXED1 (FIXED_POINT * 628 / 100)  // ≈ 2π
// #define ANGLE_STEP_FIXED (TWO_PI_FIXED1 / TOTAL_DIRECTIONS)  // Paso de 10° en radianes


// // ################################# DATA STRUCTURES #################################

// // Esta estructura representa la bola que hay que embocar
// typedef struct {
//     int x, y; // posicion 
//     int dx, dy; // dirreccion y sentido actual
// } BallToScore;

// typedef struct pongis {
//     int x, y; // posicion 
//     int dx, dy; // dirreccion y sentido actual
// } Enemy;

// typedef struct {
//     int x, y; // posicion 
//     int dx, dy; // dirreccion y sentido actual
// } Player;

// typedef struct {
//     int x, y; // Position of the hole
// } Hole;

// // ################################# VARIABLES #################################

// // estas variables las voy a tener q inicializar cuando comience el juego
// // y actualizar en cada nivel

// static BallToScore ball; 
// static Enemy enemy;
// static Player player;
// static Hole hole;

// static int level = 1;
// static int end_game = 0; // flag para terminar la ejecucion del juego

// // ################################# NOTES #################################
// /*
//     La idea principal es que sea un ciclo, y vaya valiando que end_game != 1
//     El juego se puede terminar si:
//         - el jugador aborta el nivel (alguna combinacion de teclas)                 --> salida forzada
//         - el jugador presiona un futuro "boton" que diga, salir del juego           --> salida premeditada
// */

// // ################################# FUNCTIONS #################################
// void startGame(void) {
//     // Inicializar el juego
//     player.x = INITIAL_POS_1_X;
//     player.y = INITIAL_POS_1_Y;
//     player.dx = INITIAL_DIR_1_X;
//     player.dy = INITIAL_DIR_1_Y;

//     enemy.x = INITIAL_POS_2_X;
//     enemy.y = INITIAL_POS_2_Y;
//     enemy.dx = INITIAL_DIR_2_X;
//     enemy.dy = INITIAL_DIR_2_Y;

//     ball.x = 500;
//     ball.y = 500;
//     ball.dx = 0; 
//     ball.dy = 0; 

//     newLevel(level);
// }

// void drawCharacter(uint64_t startX, uint64_t startY) {
//     for (uint64_t y = 0; y < CHARACTER_HEIGHT; y++) {
//         for (uint64_t x = 0; x < CHARACTER_WIDTH; x++) {
//             uint32_t color = characterPixels[y * CHARACTER_WIDTH + x];
//             if (color != 0xFFFFFFFF)  // ignoramos fondo transparente
//                 sys_draw_pixel(startX + x, startY + y,color);
//         }
//     }
// }

// void newLevel(int level) {

//     // // Fondo
//     // // sys_fill_screen(BACKGROUND_COLOR); // Limpia la pantalla
//     // sys_clear_screen();
//     // // Obstaculos
//     // sys_draw_circle(100,100,200, OBSTABLES_COLOR); // Dibuja un circulo rojo en la pantalla
//     // sys_draw_circle(800,200,100, OBSTABLES_COLOR); // Dibuja un circulo rojo en la pantalla

//     // // Hole
//     // hole.x = 700; // Posicion del hoyo
//     // hole.y = 500; // Posicion del hoyo
//     // sys_draw_circle(hole.x, hole.y, 22, SHADOW_COLOR); // Dibuja la sombra del hoyo
//     // sys_draw_circle(hole.x, hole.y, 20, HOLE_COLOR); // Dibuja el hoyo



//     // drawCharacter(500, 500); // Dibuja el personaje 1


//     // int dir_index = 0;

//     // while (1) {
//     //     int angle = dir_index * ANGLE_STEP_FIXED;

//     //     clearCharacterArea(500, 500);
//     //     drawCharacterRotated(500, 500, angle);
        
//     //     dir_index = (dir_index + 1) % TOTAL_DIRECTIONS;

//     //     // sys_sleep();
//     // }

// }

// void welcomePlayers(){

//     sys_clear_screen();
//     sys_fill_screen(BACKGROUND_COLOR);

//     printf("Bienvenido a Pongis Golf!\n");
//     printf("Presiona 'ENTER' para comenzar el juego.\n");
    
//     // Espera a que el usuario presione 'ENTER' o que aborte con 'ESC'
//     // while (1) {
//     //     char c = sys_getchar();
//     //     if (c == '\n') { // Enter
//     //         break;
//     //     } else if (c == 27) { // ESC
//     //         printf("Juego abortado.\n");
//     //         end_game = 1; // Termina el juego
//     //         return;
//     //     }
//     // }

//     // --> Implentar esto con syscalls 
// }

// void printScore(Player player){
//     if(player == NULL) {
//         // printf("Jugador no encontrado.\n");
//         return;
//     }

//     // imprimir el puntaje del jugador al tope de la pantalla
//     printf("Puntaje del jugador: %d\n", player.score);
//     return;
// }

// void printScore(void) {
//     printf("Puntaje del jugador: %d\n", player.score);
//     printf("Nivel: %d\n", level);
// }

// void drawEnemy(void) {
//     // Dibuja el enemigo en la pantalla
//     sys_draw_circle(enemy.x, enemy.y, 20, ENEMY_COLOR);
// }

// void drawBall(void) {
//     // Dibuja la bola en la pantalla
//     sys_draw_circle(ball.x, ball.y, 10, BALL_COLOR);
// }





// int main(void) {
    
//     end_of_game = 0;
//     level = 1;
    
//     sys_beep(1000, 500); // Sonido de bienvenida
//     welcomePlayers();

//     do{
//         sys_clear_screen();
//         printf("\n\n");

//         first_round = 1;
//         drawBackground();
//         printScore();

//         while(!end_of_game) {
            
//             drawCharacter();
//             drawEnemy();
//             drawBall();


//             // el nivel de dificultad depende de que tan rapido se muevan los objetos
//             // y de que tan rapido se mueva el jugador
//             sleep(difficulty_level);

//             checkHoleCollision();
//             checkPlayerCollision();

//         }

//         showWinner();

//     } while(!end_of_game);
    
//     return 0;
// }

// void welcome(void) {
    
//     sys_clear_screen();

//     sys_fill_screen(BACKGROUND_COLOR);

//     printf("Bienvenido a Pongis Golf!\n");
//     printf("Este juego consiste en embocar la bola en el hoyo.\n");
//     printf("Para jugar, utiliza las teclas WASD para mover al personaje.\n");

//     printf("Presiona 'ENTER' para entrar al juego.\n");

//     // Espera a que el usuario presione 'ENTER' o que aborte con 'ESC'
  
//     // while (1) {
//     //     char c = sys_getchar();
//     //     if (c == '\n') { // Enter
//     //         break;
//     //     } else if (c == 27) { // ESC
//     //         printf("Juego abortado.\n");
//     //         return;
//     //     }
//     // }                            --> es una idea

//     // Iniciar el juego
//     while(!end_game) {
//         startGame();
//     }


// }

// extern uint32_t characterPixels[]; 

// int sin_taylor(int angle_rad) {
//     int x = angle_rad;
//     int x3 = (x * x / FIXED_POINT) * x / FIXED_POINT;
//     int x5 = (x3 * x * x) / (FIXED_POINT * FIXED_POINT);

//     return x - x3 / 6 + x5 / 120;
// }

// int cos_taylor(int angle_rad) {
//     int x = angle_rad;
//     int x2 = (x * x) / FIXED_POINT;
//     int x4 = (x2 * x2) / FIXED_POINT;

//     return FIXED_POINT - x2 / 2 + x4 / 24;
// }

// int normalize_radians(int angle_rad) {
//     const int PI_FIXED = FIXED_POINT * 314 / 100;   
//     const int TWO_PI_FIXED = FIXED_POINT * 628 / 100; 

//     while (angle_rad > PI_FIXED)
//         angle_rad -= TWO_PI_FIXED;
//     while (angle_rad < -PI_FIXED)
//         angle_rad += TWO_PI_FIXED;

//     return angle_rad;
// }

// void drawCharacterRotated(uint64_t centerX, uint64_t centerY, int angle_rad) {
//     angle_rad = normalize_radians(angle_rad);  

//     int cos_theta = cos_taylor(angle_rad);
//     int sin_theta = sin_taylor(angle_rad);

//     int cx = CHARACTER_WIDTH / 2;
//     int cy = CHARACTER_HEIGHT / 2;

//     for (int y_out = 0; y_out < CHARACTER_HEIGHT; y_out++) {
//         for (int x_out = 0; x_out < CHARACTER_WIDTH; x_out++) {
//             int dx = x_out - cx;
//             int dy = y_out - cy;

//             int x_in = (cos_theta * dx + sin_theta * dy) / FIXED_POINT + cx;
//             int y_in = (-sin_theta * dx + cos_theta * dy) / FIXED_POINT + cy;

//             if (x_in >= 0 && x_in < CHARACTER_WIDTH && y_in >= 0 && y_in < CHARACTER_HEIGHT) {
//                 uint32_t color = characterPixels[y_in * CHARACTER_WIDTH + x_in];
//                 if (color != 0xFFFFFFFF)
//                     sys_draw_pixel(centerX - cx + x_out, centerY - cy + y_out, color);
//             }
//         }
//     }
// }
// void clearCharacterArea(uint64_t centerX, uint64_t centerY) {
//     int w = CHARACTER_WIDTH;
//     int h = CHARACTER_HEIGHT;
//     for (int y = 0; y < h; y++) {
//         for (int x = 0; x < w; x++) {
//             sys_draw_pixel(centerX - w/2 + x, centerY - h/2 + y, 0x329bab); // fondo
//         }
//     }
// }