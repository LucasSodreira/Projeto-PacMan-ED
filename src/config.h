#ifndef CONFIG_H
#define CONFIG_H

// ===== CONFIGURAÇÕES DO JOGO =====

// Configurações de jogabilidade
#define GAME_TITLE "PAC-MAN TERMINAL"
#define GAME_VERSION "1.0"
#define GAME_AUTHOR "Equipe Estruturas de Dados"

// Configurações de pontuação
#define POINTS_PER_DOT 10
#define POINTS_PER_POWER_PELLET 50
#define POINTS_PER_GHOST_EATEN 200 // Pontuação por comer um fantasma assustado
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

// Configurações do mapa
#define MAX_MAP_WIDTH 50
#define MAX_MAP_HEIGHT 30
#define MAX_LEVELS 10

// Configurações dos fantasmas
#define MAX_GHOSTS 4
#define GHOST_RESPAWN_TIME 5     // Segundos para reaparecer (Não usado atualmente, mas pode ser útil)
#define SCATTER_CHASE_INTERVAL 600 // Ticks para alternar entre modo scatter e chase (ex: 20 seg a 30 FPS)
#define FRIGHTENED_MODE_DURATION 300 // Ticks de duração do modo frightened (ex: 10 seg a 30 FPS)
// GHOST_EATEN_DURATION já está em ghost.h, pode ser movido para cá para consistência.

// Símbolos do jogo
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

// Configurações de debug
#ifdef DEBUG
    #define DEBUG_MODE 1
    #define SHOW_GHOST_PATHS 1
    #define SHOW_COORDINATES 1
#else
    #define DEBUG_MODE 0
    #define SHOW_GHOST_PATHS 0
    #define SHOW_COORDINATES 0
#endif

// Configurações de plataforma
#ifdef _WIN32
    #define CLEAR_COMMAND "cls"
    #define USE_WINDOWS_CONSOLE 1
#else
    #define CLEAR_COMMAND "clear"
    #define USE_WINDOWS_CONSOLE 0
#endif

// Configurações de cores (códigos ANSI)
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

// Configurações de entrada
#define KEY_UP 'W'
#define KEY_DOWN 'S'
#define KEY_LEFT 'A'
#define KEY_RIGHT 'D'
#define KEY_QUIT 'Q'
#define KEY_PAUSE 'P'

// Configurações de arquivo
#define MAX_FILENAME_SIZE 100
#define MAP_FILE_EXTENSION ".txt"
#define SAVE_FILE_EXTENSION ".sav"

// Configurações de memória
#define INITIAL_QUEUE_SIZE 10
#define MAX_QUEUE_SIZE 100

#endif // CONFIG_H
