#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <math.h>
#include <time.h>
#include "config.h"

#ifdef _WIN32
    #include <windows.h>
    #include <conio.h>
    #define strcasecmp _stricmp
#else
    #include <unistd.h>
    #include <termios.h>
    #include <fcntl.h>
#endif

// ===== FORWARD DECLARATIONS =====
typedef struct Position Position;
typedef struct Player Player;
typedef struct Ghost Ghost;

// ===== ENUMS COMPARTILHADOS =====
typedef enum {
    GHOST_NORMAL,
    GHOST_FRIGHTENED,
    GHOST_EATEN
} GhostState;

typedef enum {
    DIFFICULTY_EASY,
    DIFFICULTY_MEDIUM,
    DIFFICULTY_HARD
} DifficultyLevel;

// Enums são definidos aqui pois são tipos fundamentais
typedef enum {
    NORTH = 0,
    EAST = 1, 
    SOUTH = 2,
    WEST = 3,
    DIR_INVALID = -1
} Direction;

typedef enum {
    PLAYING,
    GAME_OVER,
    VICTORY,
    PAUSED
} GameStatus;

// ===== ESTRUTURAS BÁSICAS =====
struct Position {
    int x, y;
};

struct Player {
    Position pos;
    Direction direction;
    int score;
    int lives;
    char symbol;
};

// Funções principais realmente usadas
void clear_screen(void);
void print_instructions(void);
char get_user_input(void);
void setup_console(void);
int is_valid_position(int x, int y, int width, int height);
int manhattan_distance(Position a, Position b);
int positions_equal(Position a, Position b);

// ===== FUNÇÕES DE DIREÇÃO =====
int is_valid_direction(Direction dir);
const char* direction_to_string(Direction dir);
Direction string_to_direction(const char* str);
Direction get_opposite_direction(Direction dir);
Position get_next_position(Position pos, Direction dir);
Direction random_direction(void);

// ===== FUNÇÕES DE VALIDAÇÃO =====
int is_valid_ghost_id(int ghost_id);
int is_valid_ghost_symbol(char symbol);

// ===== FUNÇÕES DE CONVERSÃO =====
const char* game_status_to_string(GameStatus status);
void format_time(char* buffer, int seconds);

// ===== FUNÇÕES UTILITÁRIAS =====
void sleep_ms(int milliseconds);
int random_range(int min, int max);
void debug_log(const char* format, ...);

// ===== FUNÇÕES DE ESTADO DO JOGO =====
// void initialize_game_state(GameState* game);
// void print_game_stats(GameState* game);

// ===== FUNÇÕES DE INICIALIZAÇÃO DE SISTEMA =====
// Movidas para stats.h para evitar dependências circulares

#endif // UTILS_H