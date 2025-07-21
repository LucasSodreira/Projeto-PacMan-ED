#ifndef CONFIG_H
#define CONFIG_H

// ===== INFORMAÇÕES DO PROJETO =====
#define GAME_TITLE "PAC-MAN TERMINAL"
#define GAME_VERSION "1.0"
#define GAME_AUTHOR "Equipe Estruturas de Dados"

// ===== CONFIGURAÇÕES DE INTERFACE =====
#define USE_COLORS 1             // Habilita cores ANSI (se disponível)
#define CMD_COMPATIBLE 1         // Modo compatível com CMD do Windows
#define USE_SIMPLE_BORDERS 1     // Usa bordas ASCII simples

// ===== CARACTERES DE BORDA ASCII SIMPLES =====
#define BORDER_HORIZONTAL "-"
#define BORDER_VERTICAL   "|"
#define BORDER_TOP_LEFT   "+"
#define BORDER_TOP_RIGHT  "+"
#define BORDER_BOTTOM_LEFT  "+"
#define BORDER_BOTTOM_RIGHT "+"
#define BORDER_CROSS      "+"

// ===== MACROS DE CORES ANSI =====
#if USE_COLORS
    #define COLOR_RESET      "\033[0m"
    #define COLOR_BOLD       "\033[1m"
    #define COLOR_RED        "\033[31m"
    #define COLOR_GREEN      "\033[32m"
    #define COLOR_YELLOW     "\033[33m"
    #define COLOR_BLUE       "\033[34m"
    #define COLOR_MAGENTA    "\033[35m"
    #define COLOR_CYAN       "\033[36m"
    #define COLOR_WHITE      "\033[37m"
    #define COLOR_BRIGHT_RED    "\033[91m"
    #define COLOR_BRIGHT_GREEN  "\033[92m"
    #define COLOR_BRIGHT_YELLOW "\033[93m"
    #define COLOR_BRIGHT_BLUE   "\033[94m"
    #define COLOR_BRIGHT_MAGENTA "\033[95m"
    #define COLOR_BRIGHT_CYAN   "\033[96m"
#else
    #define COLOR_RESET      ""
    #define COLOR_BOLD       ""
    #define COLOR_RED        ""
    #define COLOR_GREEN      ""
    #define COLOR_YELLOW     ""
    #define COLOR_BLUE       ""
    #define COLOR_MAGENTA    ""
    #define COLOR_CYAN       ""
    #define COLOR_WHITE      ""
    #define COLOR_BRIGHT_RED    ""
    #define COLOR_BRIGHT_GREEN  ""
    #define COLOR_BRIGHT_YELLOW ""
    #define COLOR_BRIGHT_BLUE   ""
    #define COLOR_BRIGHT_MAGENTA ""
    #define COLOR_BRIGHT_CYAN   ""
#endif

// Macros simplificadas para impressão
#define PRINT_TITLE(text)    printf("%s%s%s", COLOR_BOLD, text, COLOR_RESET)
#define PRINT_INFO(text)     printf("%s%s%s", COLOR_CYAN, text, COLOR_RESET)
#define PRINT_SUCCESS(text)  printf("%s%s%s", COLOR_GREEN, text, COLOR_RESET)
#define PRINT_ERROR(text)    printf("%s%s%s", COLOR_RED, text, COLOR_RESET)
#define PRINT_WARNING(text)  printf("%s%s%s", COLOR_YELLOW, text, COLOR_RESET)

// ===== CORES PARA ELEMENTOS DO JOGO =====
#define PLAYER_COLOR        COLOR_BRIGHT_YELLOW
#define WALL_COLOR          COLOR_BLUE
#define DOT_COLOR           COLOR_WHITE
#define POWER_PELLET_COLOR  COLOR_BRIGHT_CYAN
#define GHOST_RED_COLOR     COLOR_BRIGHT_RED
#define GHOST_GREEN_COLOR   COLOR_BRIGHT_GREEN
#define GHOST_BLUE_COLOR    COLOR_BRIGHT_BLUE
#define GHOST_PINK_COLOR    COLOR_BRIGHT_MAGENTA
#define GHOST_FRIGHTENED_COLOR COLOR_CYAN
#define GHOST_EATEN_COLOR   COLOR_WHITE

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
#define POINTS_PER_GHOST_EATEN 200

// Configurações de vidas
#define DEFAULT_LIVES 3
#define STARTING_LIVES 3
#define MAX_LIVES 5

// Configurações de velocidade
#define GAME_SPEED_MS 200        // Velocidade base do jogo em ms
#define GHOST_SPEED_DIVIDER 2    // Fantasmas se movem a cada N frames
#define PLAYER_SPEED_DIVIDER 1   // Jogador se move a cada N frames

// ===== CONFIGURAÇÕES DE GAMEPLAY =====
#define MAX_LEVELS 10
#define MAX_GHOSTS 4
#define GHOST_RESPAWN_TIME 5     // Segundos para reaparecer
#define SCATTER_CHASE_INTERVAL 100   // Ticks entre mudanças de modo scatter/chase
#define FRIGHTENED_MODE_DURATION 150 // Duração do modo frightened em ticks

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

// ===== CONTROLES =====
#define KEY_UP 'W'
#define KEY_DOWN 'S'
#define KEY_LEFT 'A'
#define KEY_RIGHT 'D'
#define KEY_QUIT 'Q'
#define KEY_PAUSE 'P'

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

#endif // CONFIG_H

