#ifndef CONFIG_H
#define CONFIG_H

// ===== INFORMAÇÕES DO PROJETO =====
#define GAME_TITLE "PAC-MAN TERMINAL"
#define GAME_VERSION "1.0"
#define GAME_AUTHOR "Equipe Estruturas de Dados"

// ===== CONFIGURAÇÕES DE INTERFACE =====
#define USE_COLORS 1             // Habilita cores ANSI (se disponível)
#define USE_UNICODE 0            // Desabilita Unicode para compatibilidade
#define USE_SIMPLE_BORDERS 1     // Usa bordas ASCII simples
#define CMD_COMPATIBLE 1         // Modo compatível com CMD do Windows

// ===== CARACTERES DE BORDA ASCII SIMPLES =====
#define BORDER_HORIZONTAL "-"
#define BORDER_VERTICAL "|"
#define BORDER_TOP_LEFT "+"
#define BORDER_TOP_RIGHT "+"
#define BORDER_BOTTOM_LEFT "+"
#define BORDER_BOTTOM_RIGHT "+"
#define BORDER_CROSS "+"

// ===== MACROS DE CORES CONDICIONAIS =====
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
    // Quando cores estão desabilitadas
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

// Configurações de fantasmas
#define SCATTER_CHASE_INTERVAL 100   // Ticks entre mudanças de modo scatter/chase
#define FRIGHTENED_MODE_DURATION 150 // Duração do modo frightened em ticks
#define POINTS_PER_GHOST_EATEN 200   // Pontos por comer um fantasma

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
#define COLOR_GHOST_FRIGHTENED 36 // Ciano
#define COLOR_GHOST_EATEN 37      // Branco
#define COLOR_WALL 37        // Branco
#define COLOR_DOT 36         // Ciano
#define COLOR_POWER_PELLET 33 // Amarelo

// ===== CORES PARA INTERFACE =====
#define COLOR_TITLE 33       // Amarelo brilhante
#define COLOR_SUCCESS 32     // Verde
#define COLOR_ERROR 31       // Vermelho
#define COLOR_INFO 36        // Ciano
#define COLOR_WARNING 33     // Amarelo
#define COLOR_ACCENT 35      // Magenta
#define COLOR_BORDER 37      // Branco

// ===== MACROS DE FORMATAÇÃO =====
#if USE_COLORS
    #define ANSI_COLOR(code) "\x1b[" #code "m"
    #define ANSI_BOLD(code) "\x1b[1;" #code "m"
    #define ANSI_RESET "\x1b[0m"
    #define PRINT_COLOR(color, text) printf(ANSI_COLOR(color) text ANSI_RESET)
    #define PRINT_BOLD(color, text) printf(ANSI_BOLD(color) text ANSI_RESET)
#else
    #define ANSI_COLOR(code) ""
    #define ANSI_BOLD(code) ""
    #define ANSI_RESET ""
    #define PRINT_COLOR(color, text) printf(text)
    #define PRINT_BOLD(color, text) printf(text)
#endif

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

// ===== CONFIGURAÇÕES DE DISPLAY PARA CMD WINDOWS =====
#ifdef _WIN32
    // Caracteres compatíveis com CMD do Windows
    #define BORDER_TOP_LEFT "+"
    #define BORDER_TOP_RIGHT "+"
    #define BORDER_BOTTOM_LEFT "+"
    #define BORDER_BOTTOM_RIGHT "+"
    #define BORDER_HORIZONTAL "-"
    #define BORDER_VERTICAL "|"
    #define WALL_CHAR "#"
    #define DOT_CHAR "."
    #define POWER_PELLET_CHAR "O"
    #define PLAYER_CHAR "P"
    #define GHOST_CHAR_RED "R"
    #define GHOST_CHAR_GREEN "G"
    #define GHOST_CHAR_BLUE "B"
    #define GHOST_CHAR_PINK "P"
    #define EMPTY_CHAR " "
#else
    // Caracteres Unicode para sistemas Unix/Linux
    #define BORDER_TOP_LEFT "╔"
    #define BORDER_TOP_RIGHT "╗"
    #define BORDER_BOTTOM_LEFT "╚"
    #define BORDER_BOTTOM_RIGHT "╝"
    #define BORDER_HORIZONTAL "═"
    #define BORDER_VERTICAL "║"
    #define WALL_CHAR "█"
    #define DOT_CHAR "·"
    #define POWER_PELLET_CHAR "●"
    #define PLAYER_CHAR "Ⓟ"
    #define GHOST_CHAR_RED "👻"
    #define GHOST_CHAR_GREEN "👻"
    #define GHOST_CHAR_BLUE "👻"
    #define GHOST_CHAR_PINK "👻"
    #define EMPTY_CHAR " "
#endif

// ===== CONFIGURAÇÕES DE CORES PARA CMD =====
#ifdef _WIN32
    // Habilitar cores ANSI no Windows 10+ CMD
    #ifndef USE_COLORS
        #define USE_COLORS 1
    #endif
#else
    // Habilitar cores ANSI em sistemas Unix/Linux
    #ifndef USE_COLORS
        #define USE_COLORS 1
    #endif
#endif

#endif // CONFIG_H

