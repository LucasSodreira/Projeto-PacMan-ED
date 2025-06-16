#ifndef GHOST_H
#define GHOST_H

#include "utils.h"    // Para Position, Direction, etc.
#include <stdbool.h>
#include "config.h"   // Para MAX_GHOSTS

// Forward declaration da estrutura Maze (definida em maze.h)
typedef struct Maze Maze;

// Constantes dos fantasmas
#define MAX_PATH_LENGTH 100
#define GHOST_MOVE_DELAY 2

// Estrutura dos fantasmas - definição completa (DEPOIS de incluir utils.h)
struct Ghost {
    Position pos;          // Posição atual
    Direction direction;   // Direção atual
    int ghost_id;         // ID único
    char symbol;          // Símbolo no mapa ('F', 'G', 'B', 'R')
    GhostState state;     // Estado atual
    int is_active;        // Se está ativo no jogo
    Position target;      // Posição alvo para IA
    DifficultyLevel difficulty; // Dificuldade individual
    Position path[MAX_PATH_LENGTH]; // Fila FIFO de caminho
    int path_start;       // Início da fila de caminho
    int path_end;        // Fim da fila de caminho
    int scatter_mode;    // Se está em modo de dispersão
    int timer;          // Temporizador de uso geral
};

// Core ghost functions
void init_ghosts(Ghost ghosts[], int count);
void move_ghosts(Ghost ghosts[], int count, const Position pacman_pos, const GameState* game_state);
bool check_collision_with_pacman(Ghost ghosts[], int count, Position pacman_pos);

// Ghost AI functions
Direction calculate_next_direction(const Ghost* ghost, const Position pacman_pos, const char* maze);
Direction calculate_next_direction_maze(const Ghost* ghost, const Position pacman_pos, const Maze* maze);
Direction calculate_best_direction_to_target(const Ghost* ghost, Position target, const Maze* maze);
Position calculate_target_position(const Ghost* ghost, const Position pacman_pos);
bool is_valid_move_ghost(Position pos, const char* maze);
bool is_valid_move_ghost_maze(Position pos, const Maze* maze);

// Ghost state management
void update_ghost_state(Ghost* ghost, int current_time);
void set_ghost_difficulty(Ghost* ghost, DifficultyLevel difficulty);
void reset_ghost(Ghost* ghost);

// System initialization function (moved from utils.h)
void initialize_ghosts_safely(Ghost ghosts[], Maze* maze);

#endif // GHOST_H