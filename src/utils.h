#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>  // Para va_list
#include <math.h>    // Para abs()

#ifdef _WIN32
    #include <windows.h>  // Para Sleep()
#else
    #include <unistd.h>   // Para usleep()
#endif

// Definições de constantes
#define MAX_MAP_SIZE 50
#define MAX_GHOSTS 10
#define MAX_NAME_SIZE 50

// ===== CONSTANTES =====
#define DEFAULT_LIVES 3
#define POINTS_PER_DOT 10
#define BONUS_LEVEL_POINTS 100
#define POINTS_FOR_EXTRA_LIFE 1000
#define GAME_FPS 30
#define GHOST_MOVE_DELAY 2
#define MAX_LEVELS 10

// Cores para fantasmas (se suportado)
#define COLOR_RED 31
#define COLOR_GREEN 32
#define COLOR_BLUE 34
#define COLOR_MAGENTA 35
#define COLOR_YELLOW 33
#define COLOR_RESET 0

// Direções possíveis
typedef enum {
    NORTH = 0,
    EAST = 1, 
    SOUTH = 2,
    WEST = 3
} Direction;

// Status do jogo
typedef enum {
    PLAYING,
    GAME_OVER,
    VICTORY,
    PAUSED
} GameStatus;

// Estrutura para representar uma posição
typedef struct {
    int x, y;
} Position;

// Estrutura do Pac-Man
typedef struct {
    Position pos;           // Posição atual
    int score;             // Pontuação
    int lives;             // Vidas restantes
    char symbol;           // Símbolo no mapa ('P')
} Player;

// Estrutura de um Fantasma
typedef struct {
    Position pos;          // Posição atual
    Direction direction;   // Direção atual
    int ghost_id;         // ID único
    char symbol;          // Símbolo no mapa ('F', 'G', 'B', 'R')
    int is_active;        // Se está ativo no jogo
} Ghost;

// Estrutura do Estado do Jogo
typedef struct {
    char map[MAX_MAP_SIZE][MAX_MAP_SIZE];  // Matriz do labirinto
    int map_width;                         // Largura do mapa
    int map_height;                        // Altura do mapa
    Player player;                         // Dados do jogador
    int total_dots;                        // Total de pontos no mapa
    int collected_dots;                    // Pontos coletados
    int num_ghosts;                        // Número de fantasmas
    GameStatus status;                     // Status atual do jogo
    int level;                            // Nível atual
} GameState;

// Funções utilitárias
void clear_screen();
void print_instructions();
char get_user_input();
int is_valid_position(int x, int y, int width, int height);
void initialize_game_state(GameState* game);
void print_game_stats(GameState* game);

// ===== FUNÇÕES AUXILIARES =====

// Funções de direção
int is_valid_direction(Direction dir);
const char* direction_to_string(Direction dir);
Direction string_to_direction(const char* str);
Direction get_opposite_direction(Direction dir);
Position get_next_position(Position pos, Direction dir);
Direction random_direction();

// Funções de posição
int manhattan_distance(Position a, Position b);
int positions_equal(Position a, Position b);

// Funções de validação
int is_valid_ghost_id(int ghost_id);
int is_valid_ghost_symbol(char symbol);

// Funções de conversão
const char* game_status_to_string(GameStatus status);
void format_time(char* buffer, int seconds);

// Funções de utilidade geral
void sleep_ms(int milliseconds);
int random_range(int min, int max);
void debug_log(const char* format, ...);

#endif