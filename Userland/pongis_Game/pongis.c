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

void welcome(void) {
    
    prinf("Welcome to Pongis Golf!\n");
    printf("In this game, you will control a player who must hit a ball into a hole.\n");
    printf("You can move the player using the arrow keys \n");


}