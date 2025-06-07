// #include <pongis.h>
// #include <stdio.h>
// #include <syscalls.h>
// #include "character_data.h"

// // ################################# CONSTANTES #################################

// // initial directions
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
// } Ball;
// typedef Ball * BallPtr;

// typedef struct {
//     int x, y; // posicion 
//     int dx, dy; // dirreccion y sentido actual
// } Enemy;
// typedef Enemy * EnemyPtr;

// typedef struct {
//     int name[20]; // nombre del jugador
//     int score; // puntaje del jugador

//     int x, y; // posicion 
//     int dx, dy; // dirreccion y sentido actual
// } Player; 
// typedef Player * PlayerPtr;

// typedef struct {
//     int x, y; // Position of the hole
// } Hole; 
// typedef Hole * HolePtr;

// // ################################# VARIABLES #################################

// // estas variables las voy a tener q inicializar cuando comience el juego
// // y actualizar en cada nivel

// static Ball ball; 
// static Enemy enemy;
// static Player player1;
// static Player player2; // si hay dos jugadores
// static Hole hole;

// static int key; // variable para almacenar la tecla presionada
// static int lastKey; // variable para almacenar la ultima tecla presionada
// static int level = 1;
// static int end_of_game = 0; // flag para terminar la ejecucion del juego

// // ################################# NOTES #################################
// /*
//     La idea principal es que sea un ciclo, y vaya valiando que end_game != 1
//     El juego se puede terminar si:
//         - el jugador aborta el nivel (alguna combinacion de teclas)                 --> salida forzada
//         - el jugador presiona un futuro "boton" que diga, salir del juego           --> salida premeditada
// */

// // ################################# FUNCTIONS #################################

// // ################################## GAME LOGIC #################################

// // Funcion llamada desde el main del userland 
// void startGame(void) {
    
//     initializeAllObjects(); // Inicializa todos los objetos del juego

//     pongis();
    
//     return;
// }

// void welcome(){

//     sys_fill_screen(BACKGROUND_COLOR);

//     printf("Bienvenido a Pongis Golf!\n");
//     printf("Presiona 'ENTER' para comenzar el juego.\n");
//     printf("Selecione un nivel de dificultad:\n");
//     printf("Seleccione la cantidad de jugadores:\n");
    
//     //sys_sleep(2000);
//     return;

//     // --> Implentar esto con syscalls 
// }

// void initializeAllObjects(void) {
//     // Inicializa todos los objetos del juego

//     end_of_game = 0;
//     level = 1;
//     key = 0; 
//     lastKey = 0;

//     ball.x = BALL_INITIAL_X;
//     ball.y = BALL_INITIAL_Y;
//     ball.dx = 0;
//     ball.dy = 0;

//     enemy.x = 0; // Posicion inicial del enemigo
//     enemy.y = 0; // Posicion inicial del enemigo
//     enemy.dx = 0; // Direccion inicial del enemigo
//     enemy.dy = 0; // Sentido inicial del enemigo

//     player1.x = INITIAL_POS_1_X; // Posicion inicial del jugador 1
//     player1.y = INITIAL_POS_1_Y; // Posicion inicial del jugador 1
//     player1.dx = INITIAL_DIR_1_X; // Direccion inicial del jugador 1
//     player1.dy = INITIAL_DIR_1_Y; // Sentido inicial del jugador 1

//     player2.x = INITIAL_POS_2_X; // Posicion inicial del jugador 2
//     player2.y = INITIAL_POS_2_Y; // Posicion inicial del jugador 2
//     player2.dx = INITIAL_DIR_2_X; // Direccion inicial del jugador 2
//     player2.dy = INITIAL_DIR_2_Y; // Sentido inicial del jugador 2

//     hole.x = 700; // Posicion del hoyo
//     hole.y = 500; // Posicion del hoyo

//     return;
// }

// void printScore(PlayerPtr player) {
//     if(player == NULL) {
//         // printf("Jugador no encontrado.\n");
//         return;
//     }

//     // imprimir el puntaje del jugador al tope de la pantalla
//     printf("Puntaje del jugador %s: %d\n", player->name, player->score);
//     return;
// }

// void drawEnemy(void) {
//     // Dibuja el enemigo en la pantalla
//     sys_draw_circle(enemy.x, enemy.y, ENEMY_RADIUS, ENEMY_COLOR);
// }

// void drawBall() {
//     // Dibuja la bola en la pantalla
//     sys_draw_circle(ball.x, ball.y, BALL_RADIUS, BALL_COLOR);
// }

// void drawHole(){
//     sys_draw_circle(hole.x, hole.y, 22, SHADOW_COLOR); // Dibuja la sombra del hoyo
//     sys_draw_circle(hole.x, hole.y, 20, HOLE_COLOR); // Dibuja el hoyo
// }

// void kickBallIfNear(PlayerPtr player) {
//     if (collided(player->x, player->y, ball.x, ball.y, 30)) {
//         ball.dx = player->dx * 5;
//         ball.dy = player->dy * 5;
//     }
// }

// int distanceSquared(int x1, int y1, int x2, int y2) {
//     int dx = x2 - x1;
//     int dy = y2 - y1;
//     return dx * dx + dy * dy;
// }

// void checkCollisions(PlayerPtr player) {
//     if (player == NULL) return;

//     // 1. Bordes del campo
//     if (player->x - PLAYER_RADIUS < 0 || player->x + PLAYER_RADIUS > FIELD_WIDTH) {
//         player->x -= player->dx * 10; // volver a la posición anterior
//     }
//     if (player->y - PLAYER_RADIUS < 0 || player->y + PLAYER_RADIUS > FIELD_HEIGHT) {
//         player->y -= player->dy * 10;
//     }

//     // 2. Colisión con hoyo
//     int dist_sq = distanceSquared(ball.x, ball.y, hole.x, hole.y);
//     int r_sum = BALL_RADIUS + HOLE_RADIUS;

//     if (dist_sq <= r_sum * r_sum) {
//         end_of_game = 1;
//         printf("¡Jugador embocó la pelota en el hoyo!\n");
//     }

//     // 3. Colisión con la pelota (modificado)
//     dist_sq = distanceSquared(player->x, player->y, ball.x, ball.y);
//     r_sum = PLAYER_RADIUS + BALL_RADIUS;
//     if (dist_sq <= r_sum * r_sum) {
//         // En lugar de hacer rebotar al jugador, golpeamos la pelota
//         kickBallIfNear(player);
//     }

//     // // 4. Colisión con enemigo
//     // dist_sq = distanceSquared(player->x, player->y, enemy.x, enemy.y);
//     // r_sum = PLAYER_RADIUS + ENEMY_RADIUS;
//     // if (dist_sq <= r_sum * r_sum) {
//     //     // Acción posible: perder vida, rebote, terminar juego...
//     //     printf("¡Cuidado! Colisión con el enemigo.\n");
//     // }
// }


// void pongis(void) {
//     welcome();

//     do {
//         sys_clear_screen();
//         sys_fill_screen(BACKGROUND_COLOR); 
//         initializeAllObjects();
//         drawHole();
//         //drawEnemy();
//         drawBall(); // Dibujamos la pelota en su posición inicial

//         while(!end_of_game) {
//             key = getchar();
//             handleInput(&player1, key, lastKey);
//             movePlayer(&player1);
//             checkCollisions(&player1);
//             moveBall();  // Mover la pelota después de procesar colisiones
//             lastKey = getchar(); // Actualizar la última tecla presionada

//         }

//     } while(!end_of_game);
// }

// /**
//  * @brief Verifica si dos círculos colisionan.
//  * @param x1 Coordenada x del primer círculo.
//  * @param y1 Coordenada y del primer círculo.
//  * @param x2 Coordenada x del segundo círculo.
//  * @param y2 Coordenada y del segundo círculo.
//  * @param radiusSum Suma de los radios de los dos círculos.
//  * @return 1 si colisionan, 0 si no.
//  */
// int collided(int x1, int y1, int x2, int y2, int radiusSum) {
//     int dx = x1 - x2;
//     int dy = y1 - y2;
//     return dx * dx + dy * dy <= radiusSum * radiusSum;
// }

// /***
//  * @brief Maneja la entrada del jugador y actualiza su dirección.
//  * @param player Puntero al jugador que se va a mover.
//  * @param key Tecla actual presionada.
//  * @param lastKey Última tecla presionada.
// ***/
// void handleInput(PlayerPtr player, int key, int lastKey) {
//     if (player == NULL) return;

//     // Reset default
//     player->dx = 0;
//     player->dy = 0;

//     // Combinaciones para diagonales
//     if ((key == 'w' && lastKey == 'd') || (key == 'd' && lastKey == 'w')) {
//         player->dy = -1;
//         player->dx = 1;
//     } else if ((key == 'w' && lastKey == 'a') || (key == 'a' && lastKey == 'w')) {
//         player->dy = -1;
//         player->dx = -1;
//     } else if ((key == 's' && lastKey == 'd') || (key == 'd' && lastKey == 's')) {
//         player->dy = 1;
//         player->dx = 1;
//     } else if ((key == 's' && lastKey == 'a') || (key == 'a' && lastKey == 's')) {
//         player->dy = 1;
//         player->dx = -1;
//     } else {
//         // Movimiento simple
//         switch (key) {
//             case 'w':
//                 player->dy = -1;
//                 break;
//             case 's':
//                 player->dy = 1;
//                 break;
//             case 'a':
//                 player->dx = -1;
//                 break;
//             case 'd':
//                 player->dx = 1;
//                 break;
//         }
//     }
// }

// void movePlayer(PlayerPtr player) {

//     if (player == NULL) return;

//     int old_x = player->x;
//     int old_y = player->y;

//     player->x += player->dx * SPEED;
//     player->y += player->dy * SPEED;

//     sys_draw_circle(old_x, old_y, PLAYER_RADIUS, BACKGROUND_COLOR);
//     sys_draw_circle(player->x, player->y, PLAYER_RADIUS, PLAYER_COLOR);

// }

// /**
//  * @brief Mueve la pelota y maneja las colisiones con los bordes y el hoyo.
//  */
// void moveBall(void) {
//     // Borrar la pelota de su posición anterior
//     sys_draw_circle(ball.x, ball.y, BALL_RADIUS, BACKGROUND_COLOR);
    

//     ball.x += ball.dx * SPEED; 
//     ball.y += ball.dy * SPEED;
    
//     // colisiones con los bordes de la pantalla
//     if (ball.x - BALL_RADIUS < 0 || ball.x + BALL_RADIUS > FIELD_WIDTH) {
//         ball.dx = -ball.dx; 
//         ball.x += ball.dx;  
//     }
//     if (ball.y - BALL_RADIUS < 0 || ball.y + BALL_RADIUS > FIELD_HEIGHT) {
//         ball.dy = -ball.dy; 
//         ball.y += ball.dy;  
//     }
    
//     // la friccion reduce gradualmente la velocidad
//     if (ball.dx != 0 || ball.dy != 0) {
//         if (ball.dx > 0) 
//             ball.dx--;
//         else if (ball.dx < 0) 
//             ball.dx++;
        
//         if (ball.dy > 0) 
//             ball.dy--;
//         else if (ball.dy < 0)
//             ball.dy++;
//     }
    
//     // Verificar si la pelota entra en el hoyo
//     if (collided(ball.x, ball.y, hole.x, hole.y, BALL_RADIUS + HOLE_RADIUS - RADIUS_FIX)) {
//         end_of_game = 1;
//         printf("¡Has embocado la pelota! ¡Nivel completado!\n");
//     }
    
//     // Dibujar la pelota en su nueva posición
//     sys_draw_circle(ball.x, ball.y, BALL_RADIUS, BALL_COLOR);
// }

