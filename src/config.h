#ifndef CONFIG_H
#define CONFIG_H

// ===== INFORMAÇÕES DO PROJETO =====
#define GAME_TITLE "PAC-MAN TERMINAL"
#define GAME_VERSION "1.0"
#define GAME_AUTHOR "Equipe Estruturas de Dados"

// ===== CONFIGURAÇÕES DO MAPA =====
#define MAZE_WIDTH 40
#define MAZE_HEIGHT 20
#define MAX_MAP_WIDTH MAZE_WIDTH
#define MAX_MAP_HEIGHT MAZE_HEIGHT
#define MAX_MAP_SIZE 50
#define MAX_NAME_SIZE 50

// Configurações de pontuação
#define POINTS_PER_DOT 10
#define POINTS_PER_POWER_PELLET 50
#define BONUS_LEVEL_COMPLETE 100
#define POINTS_FOR_EXTRA_LIFE 1000

// Configurações de vidas
#define DEFAULT_LIVES 3
#define STARTING_LIVES 3
#define MAX_LIVES 5

// Configurações de velocidade
#define GAME_SPEED_MS 200        // Velocidade base do jogo em ms
#define GHOST_SPEED_DIVIDER 2    // Fantasmas se movem a cada N frames
#define PLAYER_SPEED_DIVIDER 1   // Jogador se move a cada N frames

// ===== CONFIGURAÇÕES DE GAMEPLAY =====
#define DEFAULT_LIVES 3
#define MAX_LIVES 5
#define MAX_LEVELS 10
#define MAX_GHOSTS 4
#define GHOST_RESPAWN_TIME 5     // Segundos para reaparecer

// ===== SÍMBOLOS DO JOGO =====
#define SYMBOL_PLAYER 'P'
#define SYMBOL_GHOST_RED 'F'
#define SYMBOL_GHOST_GREEN 'G' 
#define SYMBOL_GHOST_BLUE 'B'
#define SYMBOL_GHOST_PINK 'R'
#define SYMBOL_WALL '#'
#define SYMBOL_DOT '.'
#define SYMBOL_POWER_PELLET 'O'
#define SYMBOL_EMPTY_SPACE ' '
#define SYMBOL_GHOST_FRIGHTENED 'A' // 'A' para Assustado
#define SYMBOL_GHOST_EATEN 'e'      // 'e' para olhos/comido (eaten)
#define SYMBOL_WALL_CHAR SYMBOL_WALL // Para consistência em draw_game se SYMBOL_WALL_CHAR for usado

// ===== CORES ANSI =====
#define COLOR_PLAYER 33      // Amarelo
#define COLOR_GHOST_RED 31   // Vermelho
#define COLOR_GHOST_GREEN 32 // Verde
#define COLOR_GHOST_BLUE 34  // Azul
#define COLOR_GHOST_PINK 35  // Magenta
#define COLOR_GHOST_FRIGHTENED 36 // Ciano (mesmo que DOT para contraste, ou pode ser 34 - Azul)
#define COLOR_GHOST_EATEN 37      // Branco (mesmo que WALL)
#define COLOR_WALL 37        // Branco
#define COLOR_DOT 36         // Ciano
#define COLOR_POWER_PELLET 33 // Amarelo (mesmo que PLAYER) para destaque
#define COLOR_RESET 0        // Reset

// ===== CONTROLES =====
#define KEY_UP 'W'
#define KEY_DOWN 'S'
#define KEY_LEFT 'A'
#define KEY_RIGHT 'D'
#define KEY_QUIT 'Q'
#define KEY_PAUSE 'P'

// Controles de movimento
#define KEY_MOVE_UP 'W'
#define KEY_MOVE_DOWN 'S'
#define KEY_MOVE_LEFT 'A'
#define KEY_MOVE_RIGHT 'D'

// ===== CONFIGURAÇÕES DE DEBUG =====
#ifdef DEBUG
    #define DEBUG_MODE 1
    #define SHOW_GHOST_PATHS 1
    #define SHOW_COORDINATES 1
#else
    #define DEBUG_MODE 0
    #define SHOW_GHOST_PATHS 0
    #define SHOW_COORDINATES 0
#endif

// ===== CONFIGURAÇÕES DE PLATAFORMA =====
#ifdef _WIN32
    #define CLEAR_COMMAND "cls"
    #define PATH_SEPARATOR '\\'
#else
    #define CLEAR_COMMAND "clear"
    #define PATH_SEPARATOR '/'
#endif

// ===== CONFIGURAÇÕES DE ARQUIVOS =====
#define MAX_FILENAME_SIZE 100
#define LOG_BUFFER_SIZE 1024
#define DEFAULT_LOG_FILE "game.log"
#define DEFAULT_STATS_FILE "game_stats.dat"

// ===== CONFIGURAÇÕES DE MEMÓRIA =====
#define INITIAL_QUEUE_SIZE 10
#define MAX_QUEUE_SIZE 100

// Configurações de fantasmas
#define SCATTER_CHASE_INTERVAL 100
#define FRIGHTENED_MODE_DURATION 150
#define POINTS_PER_GHOST_EATEN 200

#endif // CONFIG_H

