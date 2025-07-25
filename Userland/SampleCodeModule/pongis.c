#include <myStrings.h>
#include <pongis.h>
#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <syscalls.h>


Point initialPosPlayer1[] = {{INITIAL_POS_1_X_L1, INITIAL_POS_1_Y_L1},
                             {INITIAL_POS_1_X_L2, INITIAL_POS_1_Y_L2},
                             {INITIAL_POS_1_X_L3, INITIAL_POS_1_Y_L3}};

Point initialPosPlayer2[] = {{INITIAL_POS_2_X_L1, INITIAL_POS_2_Y_L1},
                             {INITIAL_POS_2_X_L2, INITIAL_POS_2_Y_L2},
                             {INITIAL_POS_2_X_L3, INITIAL_POS_2_Y_L3}};

Point initialPosBall[] = {{BALL_INITIAL_X_L1, BALL_INITIAL_Y_L1},
                          {BALL_INITIAL_X_L2, BALL_INITIAL_Y_L2},
                          {BALL_INITIAL_X_L3, BALL_INITIAL_Y_L3}};

Point initialPosHole[] = {{HOLE_INITIAL_X_L1, HOLE_INITIAL_Y_L1},
                          {HOLE_INITIAL_X_L2, HOLE_INITIAL_Y_L2},
                          {HOLE_INITIAL_X_L3, HOLE_INITIAL_Y_L3}};

const int hole_size[][2] = {
            {HOLE_RADIUS_L1, HOLE_SHADOW_RADIUS_L1},
            {HOLE_RADIUS_L2, HOLE_SHADOW_RADIUS_L2},
            {HOLE_RADIUS_L3, HOLE_SHADOW_RADIUS_L3}
};

// ################################# VARIABLES #################################
static Ball ball;
static Player player1;
static Player player2;// si hay dos jugadores
static Hole hole;

static int key;// variable para almacenar la tecla presionada
static int playerCount;
static int level;
static int scored;// flag para saber si la pelota fue embocada
static int scoredByP1;
static int scoredByP2;
static int end_of_game;// flag para terminar la ejecucion del juego

#define MAX_SCAN_CODE 0x53
static uint8_t keysState[MAX_SCAN_CODE + 1];

int height, width;

// ################################# FUNCTIONS #################################

void handleInput(PlayerPtr player1, PlayerPtr player2, int key);
void movePlayer(PlayerPtr player);
void moveBall(void);
void initializeAllObjects(void);
void initializePlayer(int l, PlayerPtr player, Point initialPos[], int playerColor);
void printOptions(void);
void welcome(void);
void drawBall(int l);
void drawHole(int l);
int collided(int x1, int y1, int x2, int y2, int radiusSum);
void pongis(void);
void checkCollisions(void);
void checkCollisionsPlayer(PlayerPtr player);
void checkCollisionsBall(PlayerPtr player);


Note welcome_theme[] = {{NOTE_C4, 2}, {NOTE_F4, 2}, {NOTE_C5, 2}};
Note hit_ball_theme[] = {{NOTE_C4, 1}, {NOTE_F4, 1}};
Note new_level[] = {{NOTE_C4, 2}, {NOTE_E4, 2}, {NOTE_G4, 2}, {NOTE_C5, 2}};

// ################################## GAME LOGIC #################################

// Funcion llamada desde al ingresar el comando pongis desde la terminal
void startGame(void) {
    initializeAllObjects();

    welcome();
    pongis();

    sys_clear_screen();
    sys_fill_screen(BACKGROUND_COLOR);
    char *endMessage = "Juego finalizado";
    putsCenter(endMessage);
    printPadding('\n', 3);
    char buffer[30];
    sprintf(buffer, "Puntaje Jugador 1: %d\n", player1.score);
    putsWidthCenter(buffer);
    if (playerCount > 1) {
        sprintf(buffer, "Puntaje Jugador 2: %d\n", player2.score);
        putsWidthCenter(buffer);
    }
    putsWidthCenter("Presione ENTER para terminar.");

    char c = 0;
    while (c != '\n') { sys_read(1, &c, 1); }
    return;
}

void initializeAllObjects() {
    end_of_game = 0;
    level = 1;
    playerCount = 0;
    player1.score = 0;
    player2.score = 0;
    scored = 0;

    height = sys_get_screen_height();
    width = sys_get_screen_width();
}

void welcome() {
    sys_fill_screen(BACKGROUND_COLOR);
    printOptions();

    // play welcome sound
    for (int i = 0; i < sizeof(welcome_theme) / sizeof(Note); i++) {
        sys_beep(welcome_theme[i].freq, welcome_theme[i].dur);
    }

    while (!end_of_game && !playerCount) {
        key = getchar();// Espera a que el usuario presione una tecla
        if (key == '1') {
            putsWidthCenter("Has seleccionado un jugador.\n");
            putsWidthCenter("Usa las teclas WASD para moverte\n");
            playerCount = 1;
        } else if (key == '2') {
            putsWidthCenter("Has seleccionado dos jugadores.\n");
            putsWidthCenter("Usa las teclas WASD para mover al jugador 1\n");
            putsWidthCenter("Usa las flechas para mover al jugador 2\n");
            playerCount = 2;
        } else if (key == 'q') {
            end_of_game = 1;
            return;
        } else {
            sys_fill_screen(BACKGROUND_COLOR);
            printOptions();
            putsWidthCenter("Opcion no valida. Presiona 1 o 2.\n");
        }
        sys_sleep(40);// Para que se llegue a leer la opción
    }

    player1.score = 0;
    player2.score = 0;
    return;
}

void printOptions(void) {
    putsCenterWidthHeightFourthCenter("Bienvenido a Pongis Golf!\n");
    putsWidthCenter("Seleccione la cantidad de jugadores:\n");
    putsWidthCenter("Presione 1 para un jugador.\n");
    putsWidthCenter("Presione 2 para dos jugadores.\n");
    putsWidthCenter("Presiona 'q' para salir del juego.\n");
}

void printScore(PlayerPtr player1, PlayerPtr player2) {
    if (player1 == NULL || (playerCount > 1 && player2 == NULL)) { return; }

    // imprimir el puntaje del jugador al tope de la pantalla
    if(playerCount ==1){
        printf("Puntaje del jugador: %d\n", player1->score);
    } else if (playerCount == 2) {
        printf("Puntaje del jugador 1: %d\n", player1->score);
        printf("Puntaje del jugador 2: %d\n", player2->score);
    }
    return;
}

void initializePlayer(int l, PlayerPtr player, Point initialPos[], int playerColor) {
    if (l < 0 || l > MAX_LEVEL) { return; }
    player->x = initialPos[l - 1].x;
    player->y = initialPos[l - 1].y;
    player->dx = 0;
    player->dy = 0;
    player->color = playerColor;

    // Inicializar velocidad y contador de teclas mantenidas
    player->speed = PLAYER_ACCELERATION;
    player->hold_counter = 0;

    sys_draw_circle(player->x, player->y, PLAYER_RADIUS, player->color);
}

void drawBall(int l) {
    if (l < 0 || l > MAX_LEVEL) { return; }
    ball.x = initialPosBall[l - 1].x;
    ball.y = initialPosBall[l - 1].y;
    ball.dx = ball.dy = 0;

    sys_draw_circle(ball.x, ball.y, BALL_RADIUS, BALL_COLOR);
}

void drawHole(int l) {
    if (l < 0 || l > MAX_LEVEL) { return; }
    hole.x = initialPosHole[l - 1].x;
    hole.y = initialPosHole[l - 1].y;
    sys_draw_circle(hole.x, hole.y, hole.shadow_radius, SHADOW_COLOR);
    sys_draw_circle(hole.x, hole.y, hole.radius, HOLE_COLOR);
}

void kickBallIfNear(PlayerPtr player) {
    if (collided(player->x, player->y, ball.x, ball.y, BALL_RADIUS + PLAYER_WIDTH / 2)) {
        // Si el jugador está cerca de la pelota, la patea
        int kickFactor = 3 + player->speed;

        for (int i = 0; i < sizeof(hit_ball_theme) / sizeof(Note); i++) {
            sys_beep(hit_ball_theme[i].freq, hit_ball_theme[i].dur);
        }

        ball.dx = player->dx * kickFactor;
        ball.dy = player->dy * kickFactor;

        if (player == &player1) {
            scoredByP1 = 1;
            scoredByP2 = 0;
        } else if (player == &player2) {
            scoredByP1 = 0;
            scoredByP2 = 1;
        }
    }
}

int distanceSquared(int x1, int y1, int x2, int y2) {
    int dx = x2 - x1;
    int dy = y2 - y1;
    return dx * dx + dy * dy;
}

void checkCollisions(void) {
    // 1. Colisión con bordes
    checkCollisionsPlayer(&player1);
    if (playerCount > 1) { checkCollisionsPlayer(&player2); }

    // 2. Colisión con hoyo
    int r_sum = BALL_RADIUS + hole.radius;
    if (distanceSquared(ball.x, ball.y, hole.x, hole.y) <= r_sum * r_sum) {
        scored = 1;
        level++;// Incrementar el nivel

        if (level > MAX_LEVEL)
            end_of_game = 1;// Terminar el juego si se completan todos los niveles
        sys_sleep(5);
    }

    // Colision hoyo con player
    int rad_sum = PLAYER_RADIUS + hole.shadow_radius + RADIUS_FIX;
    int dist_sq_player1 = distanceSquared(player1.x, player1.y, hole.x, hole.y);
    int dist_sq_player2 = playerCount > 1 ? distanceSquared(player2.x, player2.y, hole.x, hole.y) : 0;
    if (dist_sq_player1 <= rad_sum * rad_sum) {
        drawHole(level); 
        movePlayer(&player1);
    } else if (playerCount > 1 && dist_sq_player2 <= rad_sum * rad_sum) {
        drawHole(level);
        movePlayer(&player2);
    }

    // 3. Colisión con pelota
    checkCollisionsBall(&player1);
    if (playerCount > 1) { checkCollisionsBall(&player2); }
}

void checkCollisionsPlayer(PlayerPtr player) {
    if (player == NULL) return;
    if (player->x - PLAYER_RADIUS < 0 || player->x + PLAYER_RADIUS > width) {
        sys_draw_circle(player->x, player->y, PLAYER_RADIUS, BACKGROUND_COLOR);
        player->x -= player->dx * 10;// volver a la posición anterior
    }
    if (player->y - PLAYER_RADIUS < 0 || player->y + PLAYER_RADIUS > height) {
        sys_draw_circle(player->x, player->y, PLAYER_RADIUS, BACKGROUND_COLOR);
        player->y -= player->dy * 10;
    }
}

void checkCollisionsBall(PlayerPtr player) {
    if (player == NULL) { return; }
    int dist_sq = distanceSquared(player->x, player->y, ball.x, ball.y);
    int r_sum = PLAYER_RADIUS + BALL_RADIUS;
    if (dist_sq <= r_sum * r_sum) { kickBallIfNear(player); }
}

void printLevel(int l) {
    // Seleccionar el dígito correcto según el nivel
    char selectedDigit[30];
    sprintf(selectedDigit, "Nivel %d", l);
    int len = strlen(selectedDigit);
    sys_draw_circle(width / 2, height / 2, len * width / sys_chars_width() + 5, 0x000000);
    putsCenter(selectedDigit);
}

void newLevel(int l) {
    sys_fill_screen(BACKGROUND_COLOR);// Limpiar la pantalla
    for (int i = 0; i < sizeof(new_level) / sizeof(Note); i++) {
            sys_beep(new_level[i].freq, new_level[i].dur);
        }
    printLevel(l);
    sys_sleep(30);

    sys_fill_screen(BACKGROUND_COLOR);
    initializePlayer(l, &player1, initialPosPlayer1, PLAYER_COLOR_1);
    if (playerCount > 1) {
        initializePlayer(l, &player2, initialPosPlayer2, PLAYER_COLOR_2);
    }
    printScore(&player1, &player2);
    hole.radius = hole_size[l - 1][0];
    hole.shadow_radius = hole_size[l - 1][1];
    drawHole(l);
    drawBall(l);
    scored = 0;// Reiniciar el estado de embocada
}

void pongis(void) {

    
    while (!end_of_game) {
        newLevel(level);
        scored = 0;

        while (!end_of_game && !scored) {
            key = getchar();
            if (key == 'q') { return; }
            handleInput(&player1, &player2, key);
            movePlayer(&player1);
            if (playerCount > 1) { movePlayer(&player2); }
            moveBall();       // Mover la pelota después de procesar colisiones
            checkCollisions();// Verificar colisiones

            if (scored) {
                if (playerCount == 1) {
                    player1.score += 10;// Incrementar el puntaje del jugador 1
                } else if (playerCount == 2) {
                    if (scoredByP1) {
                        player1.score += 10;// Incrementar el puntaje del jugador 1
                    } else if (scoredByP2) {
                        player2.score += 10;// Incrementar el puntaje del jugador 2
                    }
                }
            }
        }
    }
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

    // Guardar las direcciones anteriores para comparar
    int prev_dx1 = player1->dx;
    int prev_dy1 = player1->dy;
    int prev_dx2 = player2->dx;
    int prev_dy2 = player2->dy;

    // Reset default
    player1->dx = 0;
    player1->dy = 0;
    player2->dx = 0;
    player2->dy = 0;

    sys_get_key_state(keysState);

    // Verificar teclas presionadas y establecer direcciones
    if (keysState[W_SCANCODE]) { player1->dy -= 1; }
    if (keysState[S_SCANCODE]) { player1->dy += 1; }
    if (keysState[A_SCANCODE]) { player1->dx -= 1; }
    if (keysState[D_SCANCODE]) { player1->dx += 1; }

    if (playerCount > 1) {
        if (keysState[ARROW_UP_SCANCODE]) { player2->dy -= 1; }
        if (keysState[ARROW_DOWN_SCANCODE]) { player2->dy += 1; }
        if (keysState[ARROW_LEFT_SCANCODE]) { player2->dx -= 1; }
        if (keysState[ARROW_RIGHT_SCANCODE]) { player2->dx += 1; }
    }

    // Manejar aceleración para player1
    if (player1->dx == prev_dx1 && player1->dy == prev_dy1 &&
        (player1->dx != 0 || player1->dy != 0)) {
        // Si la dirección no cambió y hay movimiento, incrementar contador
        player1->hold_counter += 1;
        // Ajustar velocidad según el tiempo que se ha mantenido la tecla presionada
        player1->speed = PLAYER_ACCELERATION + (player1->hold_counter / 5);
        // Limitar la velocidad máxima
        if (player1->speed > PLAYER_MAX_VELOCITY) {
            player1->speed = PLAYER_MAX_VELOCITY;
        }
    } else {
        // Si cambió la dirección o no hay movimiento, reiniciar contador
        player1->hold_counter = 0;
        player1->speed = PLAYER_ACCELERATION;
    }

    // Manejar aceleración para player2 si está en juego
    if (playerCount > 1) {
        if (player2->dx == prev_dx2 && player2->dy == prev_dy2 &&
            (player2->dx != 0 || player2->dy != 0)) {
            player2->hold_counter += 1;
            player2->speed = PLAYER_ACCELERATION + (player2->hold_counter / 5);
            if (player2->speed > PLAYER_MAX_VELOCITY) {
                player2->speed = PLAYER_MAX_VELOCITY;
            }
        } else {
            player2->hold_counter = 0;
            player2->speed = PLAYER_ACCELERATION;
        }
    }
}

void movePlayer(PlayerPtr player) {
    if (player == NULL) return;

    int old_x1 = player->x;
    int old_y1 = player->y;

    // Usar la velocidad específica del jugador en lugar de SPEED constante
    player->x += player->dx * player->speed;
    player->y += player->dy * player->speed;

    sys_draw_circle(old_x1, old_y1, PLAYER_RADIUS, BACKGROUND_COLOR);
    sys_draw_circle(player->x, player->y, PLAYER_RADIUS, player->color);
    return;
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
    if (ball.x - BALL_RADIUS < 0 || ball.x + BALL_RADIUS > width) {
        ball.dx = -ball.dx;
        ball.x += ball.dx;
    }
    if (ball.y - BALL_RADIUS < 0 || ball.y + BALL_RADIUS > height) {
        ball.dy = -ball.dy;
        ball.y += ball.dy;
    }

    // la friccion reduce gradualmente la velocidad
    if (ball.dx != 0 || ball.dy != 0) {
        if (ball.dx > 0) ball.dx--;
        else if (ball.dx < 0)
            ball.dx++;

        if (ball.dy > 0) ball.dy--;
        else if (ball.dy < 0)
            ball.dy++;
    }

    // Dibujar la pelota en su nueva posición
    sys_draw_circle(ball.x, ball.y, BALL_RADIUS, BALL_COLOR);
}
