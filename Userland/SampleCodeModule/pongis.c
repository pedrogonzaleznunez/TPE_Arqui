#include <pongis.h>
#include <stdio.h>
#include <stdlib.h>
#include <syscalls.h>
#include "character_data.h"

// ################################# CONSTANTES #################################

// initial directions
#define CHARACTER_WIDTH 248
#define CHARACTER_HEIGHT 248
#define FIXED_POINT 1024 // para representar 1.0 como 1024

#define TOTAL_DIRECTIONS 36
#define DEGREE_STEP 10
#define TWO_PI_FIXED1 (FIXED_POINT * 628 / 100)  // ≈ 2π
#define ANGLE_STEP_FIXED (TWO_PI_FIXED1 / TOTAL_DIRECTIONS)  // Paso de 10° en radianes

// ################################# DATA STRUCTURES #################################

// Esta estructura representa la bola que hay que embocar
typedef struct {
    int x, y; // posicion 
    int dx, dy; // dirreccion y sentido actual
} Ball;
typedef Ball * BallPtr;

typedef struct {
    int x, y; // posicion 
    int dx, dy; // dirreccion y sentido actual
} Enemy;
typedef Enemy * EnemyPtr;

typedef struct {
    int score; // puntaje del jugador
    int x, y; // posicion 
    int dx, dy; // dirreccion y sentido actual
} Player; 
typedef Player * PlayerPtr;

typedef struct {
    int x, y; // Position of the hole
} Hole; 
typedef Hole * HolePtr;

// ################################# VARIABLES #################################

const unsigned char numero1[30][3] = {
    {0x30, 0x00, 0x00}, // filas 0-3
    {0x30, 0x00, 0x00},
    {0x30, 0x00, 0x00},
    {0x30, 0x00, 0x00},
    
    {0xF0, 0x00, 0x00}, // filas 4-7
    {0xF0, 0x00, 0x00},
    {0xF0, 0x00, 0x00},
    {0xF0, 0x00, 0x00},

    {0x30, 0x00, 0x00}, // filas 8-27 (repetimos el tronco de 1)
    {0x30, 0x00, 0x00},
    {0x30, 0x00, 0x00},
    {0x30, 0x00, 0x00},

    {0x30, 0x00, 0x00},
    {0x30, 0x00, 0x00},
    {0x30, 0x00, 0x00},
    {0x30, 0x00, 0x00},

    {0x30, 0x00, 0x00},
    {0x30, 0x00, 0x00},
    {0x30, 0x00, 0x00},
    {0x30, 0x00, 0x00},

    {0x30, 0x00, 0x00},
    {0x30, 0x00, 0x00},
    {0x30, 0x00, 0x00},
    {0x30, 0x00, 0x00},

    {0xF0, 0x00, 0x00}, // base
    {0xF0, 0x00, 0x00},
    {0xF0, 0x00, 0x00},
    {0xF0, 0x00, 0x00},

    {0x00, 0x00, 0x00}, // relleno negro
    {0x00, 0x00, 0x00}
};

const unsigned char numero2[30][3] = {
    {0xF8, 0x1F, 0x00}, {0xF8, 0x1F, 0x00}, {0xF8, 0x1F, 0x00}, {0xF8, 0x1F, 0x00}, // 0x0E -> 1110 => 11110000 00011111
    {0x08, 0x20, 0x00}, {0x08, 0x20, 0x00}, {0x08, 0x20, 0x00}, {0x08, 0x20, 0x00}, // 0x11 -> 10001
    {0x08, 0x00, 0x00}, {0x08, 0x00, 0x00}, {0x08, 0x00, 0x00}, {0x08, 0x00, 0x00}, // 0x01
    {0x10, 0x00, 0x00}, {0x10, 0x00, 0x00}, {0x10, 0x00, 0x00}, {0x10, 0x00, 0x00}, // 0x02
    {0x20, 0x00, 0x00}, {0x20, 0x00, 0x00}, {0x20, 0x00, 0x00}, {0x20, 0x00, 0x00}, // 0x04
    {0x40, 0x00, 0x00}, {0x40, 0x00, 0x00}, {0x40, 0x00, 0x00}, {0x40, 0x00, 0x00}, // 0x08
    {0xF8, 0x1F, 0x00}, {0xF8, 0x1F, 0x00}, {0xF8, 0x1F, 0x00}, {0xF8, 0x1F, 0x00}, // 0x1F
    {0x00, 0x00, 0x00}, {0x00, 0x00, 0x00}  // relleno negro (opcional)
};

const unsigned char numero3[30][3] = {
    {0xF8, 0x1F, 0x00}, {0xF8, 0x1F, 0x00}, {0xF8, 0x1F, 0x00}, {0xF8, 0x1F, 0x00}, // 0x0E
    {0x08, 0x20, 0x00}, {0x08, 0x20, 0x00}, {0x08, 0x20, 0x00}, {0x08, 0x20, 0x00}, // 0x11
    {0x08, 0x00, 0x00}, {0x08, 0x00, 0x00}, {0x08, 0x00, 0x00}, {0x08, 0x00, 0x00}, // 0x01
    {0xF0, 0x01, 0x00}, {0xF0, 0x01, 0x00}, {0xF0, 0x01, 0x00}, {0xF0, 0x01, 0x00}, // 0x06
    {0x08, 0x00, 0x00}, {0x08, 0x00, 0x00}, {0x08, 0x00, 0x00}, {0x08, 0x00, 0x00}, // 0x01
    {0x08, 0x20, 0x00}, {0x08, 0x20, 0x00}, {0x08, 0x20, 0x00}, {0x08, 0x20, 0x00}, // 0x11
    {0xF8, 0x1F, 0x00}, {0xF8, 0x1F, 0x00}, {0xF8, 0x1F, 0x00}, {0xF8, 0x1F, 0x00}, // 0x0E
    {0x00, 0x00, 0x00}, {0x00, 0x00, 0x00}  // relleno negro
};
// estas variables las voy a tener q inicializar cuando comience el juego
// y actualizar en cada nivel

static Ball ball; 
static Enemy enemy;
static Player player1;
static Player player2; // si hay dos jugadores
static Hole hole;

static int key; // variable para almacenar la tecla presionada
static int lastKey; // variable para almacenar la ultima tecla presionada
static int playerCount; 
static int level = 1;
static int embocada = 0; // flag para saber si la pelota fue embocada
static int end_of_game = 0; // flag para terminar la ejecucion del juego

// ################################# NOTES #################################
/*
    La idea principal es que sea un ciclo, y vaya valiando que end_game != 1
    El juego se puede terminar si:
        - el jugador aborta el nivel (alguna combinacion de teclas)                 --> salida forzada
        - el jugador presiona un futuro "boton" que diga, salir del juego           --> salida premeditada
*/

// ################################# FUNCTIONS #################################

// ################################## GAME LOGIC #################################

// Funcion llamada desde el main del userland 
void startGame(void) {


    welcome();
    pongis();
    
    return;
}

void welcome(){

    sys_beep(200, 20);
    sys_fill_screen(BACKGROUND_COLOR);

    printf("Bienvenido a Pongis Golf!\n");
    printf("Seleccione la cantidad de jugadores:\n");
    printf("Presione 1 para un jugador.\n");
    printf("Presione 2 para dos jugadores.\n");

    do{
        key = getchar(); // Espera a que el usuario presione una tecla

        if (key == '1') {
            printf("Has seleccionado un jugador.\n");
            playerCount = 1;   
            break;

        } else if (key == '2') {
            printf("Has seleccionado dos jugadores.\n");
            playerCount = 2;
            break;

        } 
        else {
            printf("Opcion no valida. Presiona 1 o 2.\n");
        }
    }   while (1); 


    player1.score = 0; 
    player2.score = 0; 
    return;
}


void printScore(PlayerPtr player1, PlayerPtr player2) {
    if(player1 == NULL || (playerCount > 1 && player2 == NULL)) {
        return;
    }

    // imprimir el puntaje del jugador al tope de la pantalla
    if(playerCount == 1) {
        printf("Puntaje del jugador: %d\n", player1->score);
    } else if(playerCount == 2) {
        printf("Puntaje del jugador 1: %d\n", player1->score);
        printf("Puntaje del jugador 2: %d\n", player2->score);
    }
    return;
}

void drawPlayers(int l) {

    switch (l)
    {
    case 1:
        player1.x = INITIAL_POS_1_X_L1;
        player1.y = INITIAL_POS_1_Y_L1;
        player2.x = INITIAL_POS_2_X_L1;
        player2.y = INITIAL_POS_2_Y_L1;
        break;
    case 2:
        player1.x = INITIAL_POS_1_X_L2;
        player1.y = INITIAL_POS_1_Y_L2;
        player2.x = INITIAL_POS_2_X_L2;
        player2.y = INITIAL_POS_2_Y_L2;
        break;
    case 3:
        player1.x = INITIAL_POS_1_X_L3;
        player1.y = INITIAL_POS_1_Y_L3;
        player2.x = INITIAL_POS_2_X_L3;
        player2.y = INITIAL_POS_2_Y_L3;
        break;

    default:
        break;
    }

    sys_draw_circle(player1.x, player1.y, PLAYER_RADIUS, PLAYER_COLOR_1);
    
    if(playerCount > 1){
        sys_draw_circle(player2.x, player2.y, PLAYER_RADIUS, PLAYER_COLOR_2);
    }
}

void drawEnemy(int l) {
    // Dibuja el enemigo en la pantalla
    sys_draw_circle(enemy.x, enemy.y, ENEMY_RADIUS, ENEMY_COLOR);
}

void drawBall(int l) {

    switch (l)
    {
    case 1:
        ball.x = BALL_INITIAL_X_L1;
        ball.y = BALL_INITIAL_Y_L1;
        ball.dx = INITIAL_DIR_1_X;
        ball.dy = INITIAL_DIR_1_Y;
        break;
    case 2:
        ball.x = BALL_INITIAL_X_L2;
        ball.y = BALL_INITIAL_Y_L2;
        ball.dx = INITIAL_DIR_2_X;
        ball.dy = INITIAL_DIR_2_Y;
        break;
    case 3:
        ball.x = BALL_INITIAL_X_L3;
        ball.y = BALL_INITIAL_Y_L3;
        ball.dx = INITIAL_DIR_3_X;
        ball.dy = INITIAL_DIR_3_Y;
        break;

    default:
        break;
    }

    sys_draw_circle(ball.x, ball.y, BALL_RADIUS, BALL_COLOR);
}

void drawHole(int l){

    switch (l)
    {
    case 1:
        hole.x = HOLE_INITIAL_X_L1;
        hole.y = HOLE_INITIAL_Y_L1;
        break;
    case 2:
        hole.x = HOLE_INITIAL_X_L2;
        hole.y = HOLE_INITIAL_Y_L2;
        break;
    case 3:
        hole.x = HOLE_INITIAL_X_L3;
        hole.y = HOLE_INITIAL_Y_L3;
        break;
    default:
        break;
        
    }
    sys_draw_circle(hole.x, hole.y, HOLE_SHADOW_RADIUS, SHADOW_COLOR); 
    sys_draw_circle(hole.x, hole.y, HOLE_RADIUS, HOLE_COLOR); 
}

void kickBallIfNear(PlayerPtr player) {
    if (collided(player->x, player->y, ball.x, ball.y, 30)) {
        ball.dx = player->dx * 5;
        ball.dy = player->dy * 5;
    }
}

int distanceSquared(int x1, int y1, int x2, int y2) {
    int dx = x2 - x1;
    int dy = y2 - y1;
    return dx * dx + dy * dy;
}

void checkCollisions(PlayerPtr player1, PlayerPtr player2) {
    if (player1 == NULL || player2 == NULL) return;

    // 1. Bordes del campo
    // Colisiones con los bordes del campo para player1
    if (player1->x - PLAYER_RADIUS < 0 || player1->x + PLAYER_RADIUS > FIELD_WIDTH) {
        player1->x -= player1->dx * 10; // volver a la posición anterior
    }
    if (player1->y - PLAYER_RADIUS < 0 || player1->y + PLAYER_RADIUS > FIELD_HEIGHT) {
        player1->y -= player1->dy * 10;
    }

    // Colisiones con los bordes del campo para player2
    if (player2->x - PLAYER_RADIUS < 0 || player2->x + PLAYER_RADIUS > FIELD_WIDTH) {
        player2->x -= player2->dx * 10; // volver a la posición anterior
    }
    if (player2->y - PLAYER_RADIUS < 0 || player2->y + PLAYER_RADIUS > FIELD_HEIGHT) {
        player2->y -= player2->dy * 10;
    }

    // 2. Colisión con hoyo
    int dist_sq = distanceSquared(ball.x, ball.y, hole.x, hole.y);
    int r_sum = BALL_RADIUS + HOLE_RADIUS;

    if (dist_sq <= r_sum * r_sum) {
        embocada = 1; 
        level++; // Incrementar el nivel
        if(level > 3)
            end_of_game = 1; // Terminar el juego si se completan todos los niveles
        printf("¡Jugador embocó la pelota en el hoyo!\n");
    }

    // 3. Colisión con la pelota (player1)
    dist_sq = distanceSquared(player1->x, player1->y, ball.x, ball.y);
    r_sum = PLAYER_RADIUS + BALL_RADIUS;
    if (dist_sq <= r_sum * r_sum) {
        // En lugar de hacer rebotar al jugador, golpeamos la pelota
        kickBallIfNear(player1);
    }

    // 3b. Colisión con la pelota (player2)
    dist_sq = distanceSquared(player2->x, player2->y, ball.x, ball.y);
    r_sum = PLAYER_RADIUS + BALL_RADIUS;
    if (dist_sq <= r_sum * r_sum) {
        kickBallIfNear(player2);
    }

    // // 4. Colisión con enemigo
    // dist_sq = distanceSquared(player->x, player->y, enemy.x, enemy.y);
    // r_sum = PLAYER_RADIUS + ENEMY_RADIUS;
    // if (dist_sq <= r_sum * r_sum) {
    //     // Acción posible: perder vida, rebote, terminar juego...
    //     printf("¡Cuidado! Colisión con el enemigo.\n");
    // }

    // Verificar si la pelota entra en el hoyo
    // if (collided(ball.x, ball.y, hole.x, hole.y, BALL_RADIUS + HOLE_RADIUS - RADIUS_FIX)) {
    //     printf("¡Has embocado la pelota! ¡Nivel completado!\n");
    //     embocada = 1; // Marcar que la pelota fue embocada
    //     level++; // Incrementar el nivel
    //     if(level > 3)
    //         end_of_game = 1; // Terminar el juego si se completan todos los niveles

    // }
}

void printLevel(int l) {
    // Seleccionar el dígito correcto según el nivel
    const unsigned char (*selectedDigit)[3];
    
    switch (l) {
        case 1:
            selectedDigit = numero1;
            break;
        case 2:
            selectedDigit = numero2;
            break;
        case 3:
            selectedDigit = numero3;
            break;
        default:

            break;
    }
    
    // Dimensiones de los dígitos
    int digitHeight = 30;   // 30 filas
    int digitWidth = 24;    // 3 bytes * 8 bits = 24 bits de ancho máximo
    
    // Calcular posición central en la pantalla
    int centerX = FIELD_WIDTH / 2;
    int centerY = FIELD_HEIGHT / 2;
    
    // Punto de inicio para dibujar (centrado)
    int startX = centerX - (digitWidth / 2);
    int startY = centerY - (digitHeight / 2);
    
    // Color del dígito
    unsigned int digitColor = 0xFFFFFF;  // Blanco
    
    // Iterar por cada fila
    for (int row = 0; row < digitHeight; row++) {
        // Iterar por cada byte en la fila (3 bytes)
        for (int byte = 0; byte < 3; byte++) {
            // Obtener el valor del byte actual
            unsigned char currentByte = selectedDigit[row][byte];
            
            // Iterar por cada bit en el byte (8 bits)
            for (int bit = 0; bit < 8; bit++) {
                // Comprobar si el bit está encendido (1)
                if (currentByte & (0x80 >> bit)) {
                    // Calcular la posición x del pixel
                    int pixelX = startX + (byte * 8) + bit;
                    // Dibujar el pixel
                    sys_draw_pixel(pixelX, startY + row, digitColor);
                }
            }
        }
    }
}

void newLevel(int l){

    sys_fill_screen(BACKGROUND_COLOR); // Limpiar la pantalla
    // // Limpiar el campo dibujando un rectángulo negro en el centro de la pantalla
    // int rect_width = FIELD_WIDTH / 2;
    // int rect_height = FIELD_HEIGHT / 2;
    // int rect_x = (FIELD_WIDTH - rect_width) / 2;
    // int rect_y = (FIELD_HEIGHT - rect_height) / 2;

    // sys_draw_rec(rect_x, rect_y, rect_width, rect_height, 0x000000);
    sys_draw_circle(FIELD_WIDTH / 2, FIELD_HEIGHT / 2, 40, 0x000000);
    printLevel(level);
    sys_sleep(50);

    sys_clear_screen();
    sys_fill_screen(BACKGROUND_COLOR); 
    drawPlayers(l); // Dibuja los jugadores
    drawHole(l);
    drawBall(l); 
    embocada = 0; // Reiniciar el estado de embocada
}

void pongis(void) {

    do {
        
        newLevel(level);

        while(!end_of_game) {
            key = getchar();
            handleInput(&player1, &player2, key);
            movePlayer(&player1, &player2); // Mover al jugador
            checkCollisions(&player1, &player2); // Verificar colisiones
            moveBall();  // Mover la pelota después de procesar colisiones
            // lastKey = getchar(); // Actualizar la última tecla presionada
            

            if(embocada){
                newLevel(level);
                if(playerCount == 1) {
                    player1.score += 10; // Incrementar el puntaje del jugador 1
                } else if(playerCount == 2) {
                    player1.score += 5; // Incrementar el puntaje del jugador 1
                    player2.score += 5; // Incrementar el puntaje del jugador 2
                }
                embocada = 0; 
            }

        }

    } while(!end_of_game);
}
/**
 * @brief Verifica si dos círculos colisionan.
 * @param x1 Coordenada x del primer círculo.
 * @param y1 Coordenada y del primer círculo.
 * @param x2 Coordenada x del segundo círculo.
 * @param y2 Coordenada y del segundo círculo.
 * @param radiusSum Suma de los radios de los dos círculos.
 * @return 1 si colisionan, 0 si no.
 */
int collided(int x1, int y1, int x2, int y2, int radiusSum) {
    int dx = x1 - x2;
    int dy = y1 - y2;
    return dx * dx + dy * dy <= radiusSum * radiusSum;
}

/***
 * @brief Maneja la entrada del jugador y actualiza su dirección.
 * @param player Puntero al jugador que se va a mover.
 * @param key Tecla actual presionada.
 * @param lastKey Última tecla presionada.
***/
void handleInput(PlayerPtr player1, PlayerPtr player2, int key) {
    if (player1 == NULL || player2 == NULL) return;

    // Reset default
    player1->dx = 0;
    player1->dy = 0;
    player2->dx = 0;
    player2->dy = 0;
    
    // Movimiento simple
    switch (key) {
        case 'w':
            player1->dy = -1;
            player1->dx = 0;
            break;
        case 's':
            player1->dy = 1;
            player1->dx = 0;
            break;
        case 'a':
            player1->dx = -1;
            player1->dy = 0;
            break;
        case 'd':
            player1->dx = 1;
            player1->dy = 0;
            break;

        case ARROW_UP_CODE:
            player2->dy = -1;
            player2->dx = 0;
            break;
        case ARROW_DOWN_CODE:
            player2->dy = 1;
            player2->dx = 0;
            break;
        case ARROW_LEFT_CODE:
            player2->dx = -1;
            player2->dy = 0;
            break;
        case ARROW_RIGHT_CODE:
            player2->dx = 1;
            player2->dy = 0;
            break;
    }
    
}

void movePlayer(PlayerPtr player1, PlayerPtr player2) {

    if (player1 == NULL || player2 == NULL) return;

    int old_x1 = player1->x;
    int old_y1 = player1->y;
    int old_x2 = player2->x;
    int old_y2 = player2->y;

    player1->x += player1->dx * SPEED;
    player1->y += player1->dy * SPEED;
    player2->x += player2->dx * SPEED;
    player2->y += player2->dy * SPEED;

    
    sys_draw_circle(old_x1, old_y1, PLAYER_RADIUS, BACKGROUND_COLOR);
    sys_draw_circle(player1->x, player1->y, PLAYER_RADIUS, PLAYER_COLOR_1);

    if(playerCount > 1){
        sys_draw_circle(old_x2, old_y2, PLAYER_RADIUS, BACKGROUND_COLOR);
        sys_draw_circle(player2->x, player2->y, PLAYER_RADIUS, PLAYER_COLOR_2);
    }
}

/**
 * @brief Mueve la pelota y maneja las colisiones con los bordes y el hoyo.
 */
void moveBall(void) {
    // Borrar la pelota de su posición anterior
    sys_draw_circle(ball.x, ball.y, BALL_RADIUS, BACKGROUND_COLOR);
    

    ball.x += ball.dx * SPEED; 
    ball.y += ball.dy * SPEED;
    
    // colisiones con los bordes de la pantalla
    if (ball.x - BALL_RADIUS < 0 || ball.x + BALL_RADIUS > FIELD_WIDTH) {
        ball.dx = -ball.dx; 
        ball.x += ball.dx;  
    }
    if (ball.y - BALL_RADIUS < 0 || ball.y + BALL_RADIUS > FIELD_HEIGHT) {
        ball.dy = -ball.dy; 
        ball.y += ball.dy;  
    }
    
    // la friccion reduce gradualmente la velocidad
    if (ball.dx != 0 || ball.dy != 0) {
        if (ball.dx > 0) 
            ball.dx--;
        else if (ball.dx < 0) 
            ball.dx++;
        
        if (ball.dy > 0) 
            ball.dy--;
        else if (ball.dy < 0)
            ball.dy++;
    }
    
    
    
    // Dibujar la pelota en su nueva posición
    sys_draw_circle(ball.x, ball.y, BALL_RADIUS, BALL_COLOR);
}

