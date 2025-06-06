#include <pongis.h>
#include <stdio.h>
#include <syscalls.h>
#include <character_data.h>

// ################################# CONSTANTES #################################

#define INITIAL_POS_1_X (SQUARE_DIM * 28)
#define INITIAL_POS_1_Y (SQUARE_DIM * 21)
#define INITIAL_POS_2_X (SQUARE_DIM * 3)
#define INITIAL_POS_2_Y (SQUARE_DIM * 4)

// initial directions
#define INITIAL_DIR_1_X -1
#define INITIAL_DIR_1_Y 0
#define INITIAL_DIR_2_X 1
#define INITIAL_DIR_2_Y 0


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
} BallToScore;

typedef struct pongis {
    int x, y; // posicion 
    int dx, dy; // dirreccion y sentido actual
} Enemy;

typedef struct {
    int x, y; // posicion 
    int dx, dy; // dirreccion y sentido actual
    int speed; // velocidad de movimiento
    int score; // puntaje del jugador
} Player;

typedef struct {
    int x, y; // Position of the hole
} Hole;

// ################################# VARIABLES #################################

// estas variables las voy a tener q inicializar cuando comience el juego
// y actualizar en cada nivel

static BallToScore ball; 
static Enemy enemy;
static Player player;
static Hole hole;

static int level = 1;
static int end_game = 0; // flag para terminar la ejecucion del juego

// ################################# NOTES #################################
/*
    La idea principal es que sea un ciclo, y vaya valiando que end_game != 1
    El juego se puede terminar si:
        - el jugador aborta el nivel (alguna combinacion de teclas)                 --> salida forzada
        - el jugador presiona un futuro "boton" que diga, salir del juego           --> salida premeditada
*/

// ################################# FUNCTIONS #################################

int startGame(void){
    
    
    newLevel(level);
    welcome(); // Mensaje de bienvenida

}

void drawCharacter(uint64_t startX, uint64_t startY) {
    for (uint64_t y = 0; y < CHARACTER_HEIGHT; y++) {
        for (uint64_t x = 0; x < CHARACTER_WIDTH; x++) {
            uint32_t color = characterPixels[y * CHARACTER_WIDTH + x];
            if (color != 0xFFFFFFFF)  // ignoramos fondo transparente
                sys_draw_pixel(startX + x, startY + y,color);
        }
    }
}

void newLevel(int level) {
    
    
    sys_fill_screen(BACKGROUND_COLOR); 
    // Obstaculos
    sys_draw_circle(100,100,200, OBSTABLES_COLOR); 
    sys_draw_circle(800,200,100, OBSTABLES_COLOR); 
    
    // Hole
    hole.x = 900; // Posicion del hoyo
    hole.y = 400; // Posicion del hoyo

    sys_draw_circle(hole.x, hole.y, 24, SHADOW_COLOR); // Dibuja la sombra del hoyo
    sys_draw_circle(hole.x, hole.y, 20, HOLE_COLOR); // Dibuja el hoyo

    int64_t arrows;

    const int speed = 5; // Velocidad de movimiento del jugador
    player.x = 500;
    player.y = 500;
    
    drawCharacter(player.x, player.y); // Dibujar posición inicial

    while (!end_game) {
       

        int64_t arrows = sys_get_arrow();  // Lee estado actual

        // Procesa movimiento...
    
        
        // Guarda posición anterior para limpiar
        int oldX = player.x;
        int oldY = player.y;
        
        // mover jugador
        if (arrows & ARROW_UP)    {
            player.y -= speed;
            sys_draw_rec(100,100,200,200, 0xFFFFFF ); // Limpia el área del obstáculo
        }
        if (arrows & ARROW_DOWN)  {
            player.y += speed;
            printf("Arriba\n");
        }
        if (arrows & ARROW_LEFT)  {
            player.x -= speed;
            printf("Arriba\n");
        }
        if (arrows & ARROW_RIGHT) {
            player.x += speed;
            printf("Arriba\n");
        }
        
        // Solo redibuja si la posición cambió
        if (oldX != player.x || oldY != player.y) {
            clearCharacterArea(oldX, oldY);     // Limpia posición anterior
            drawCharacter(player.x, player.y);  // Dibuja nueva posición
        }
        
        sys_sleep(1);
    }

    // printf("Player position: (%d, %d)\n", player.x, player.y);
        

}



void welcome(void) {
    
    // printf("Welcome to Pongis Golf!\n");
    // printf("In this game, you will control a player who must hit a ball into a hole.\n");
    // printf("You can move the player using the arrow keys \n");

}

extern uint32_t characterPixels[]; 

int sin_taylor(int angle_rad) {
    int x = angle_rad;
    int x3 = (x * x / FIXED_POINT) * x / FIXED_POINT;
    int x5 = (x3 * x * x) / (FIXED_POINT * FIXED_POINT);

    return x - x3 / 6 + x5 / 120;
}

int cos_taylor(int angle_rad) {
    int x = angle_rad;
    int x2 = (x * x) / FIXED_POINT;
    int x4 = (x2 * x2) / FIXED_POINT;

    return FIXED_POINT - x2 / 2 + x4 / 24;
}

int normalize_radians(int angle_rad) {
    // Asegura que el ángulo esté en el rango [-π, π] en punto fijo
    const int PI_FIXED = FIXED_POINT * 314 / 100;   // ≈ π
    const int TWO_PI_FIXED = FIXED_POINT * 628 / 100; // ≈ 2π

    while (angle_rad > PI_FIXED)
        angle_rad -= TWO_PI_FIXED;
    while (angle_rad < -PI_FIXED)
        angle_rad += TWO_PI_FIXED;

    return angle_rad;
}

void drawCharacterRotated(uint64_t centerX, uint64_t centerY, int angle_rad) {
    angle_rad = normalize_radians(angle_rad);  // 💥 ¡Esto evita que explote!

    int cos_theta = cos_taylor(angle_rad);
    int sin_theta = sin_taylor(angle_rad);

    int cx = CHARACTER_WIDTH / 2;
    int cy = CHARACTER_HEIGHT / 2;

    for (int y_out = 0; y_out < CHARACTER_HEIGHT; y_out++) {
        for (int x_out = 0; x_out < CHARACTER_WIDTH; x_out++) {
            int dx = x_out - cx;
            int dy = y_out - cy;

            int x_in = (cos_theta * dx + sin_theta * dy) / FIXED_POINT + cx;
            int y_in = (-sin_theta * dx + cos_theta * dy) / FIXED_POINT + cy;

            if (x_in >= 0 && x_in < CHARACTER_WIDTH && y_in >= 0 && y_in < CHARACTER_HEIGHT) {
                uint32_t color = characterPixels[y_in * CHARACTER_WIDTH + x_in];
                if (color != 0xFFFFFFFF)
                    sys_draw_pixel(centerX - cx + x_out, centerY - cy + y_out, color);
            }
        }
    }
}
void clearCharacterArea(uint64_t centerX, uint64_t centerY) {
    int w = CHARACTER_WIDTH;
    int h = CHARACTER_HEIGHT;
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            sys_draw_pixel(centerX - w/2 + x, centerY - h/2 + y, 0x329bab); // fondo
        }
    }
}