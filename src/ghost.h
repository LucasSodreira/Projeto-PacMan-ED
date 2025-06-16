#ifndef GHOST_H
#define GHOST_H

#include <stdbool.h>
#include "config.h"   // Para MAX_GHOSTS
#include "utils.h"    // Para Position, Direction, Player (se definido aqui) etc.
#include "maze.h"     // Para estrutura Maze

// Forward declaration para evitar dependência circular
typedef struct Player Player;

// Constantes dos fantasmas
#define MAX_PATH_LENGTH 100
#define GHOST_MOVE_DELAY 2

// Cores dos fantasmas
#define COLOR_GHOST_RED 31
#define COLOR_GHOST_GREEN 32
#define COLOR_GHOST_BLUE 34
#define COLOR_GHOST_PINK 35
// COLOR_GHOST_FRIGHTENED e COLOR_GHOST_EATEN movidas para config.h


// Duração do estado "comido" antes de reativar
#define GHOST_EATEN_DURATION 200 // Ajustar: 200 ticks/frames

// Importar enums de utils.h
// GhostState e DifficultyLevel estão definidos em utils.h

// Estrutura dos fantasmas - definição completa
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
    int timer;          // Temporizador de uso geral (para frightened, eaten, etc.)
    Position initial_pos; // Posição inicial do fantasma (para reset)
};

// Core ghost functions
void init_ghosts(Ghost ghosts[], int count);
// void move_ghosts(Ghost ghosts[], int count, Position pacman_pos, const Maze* maze_data); // Removida/Substituída pela lógica de Fila em main.c
// Updated to include Player* for score update when a ghost is eaten
bool check_collision_with_pacman(Player* player, Ghost ghosts[], int count, Position pacman_pos);

// Ghost AI functions
// Kept the version that uses const Maze* and renamed it (removed the _maze suffix)
Direction calculate_next_direction(const Ghost* ghost, Position pacman_pos, const Maze* maze_data);
Position calculate_target_position(const Ghost* ghost, Position pacman_pos);
// Kept the version that uses const Maze* and renamed it (removed the _maze suffix)
bool is_valid_move_ghost(Position pos, const Maze* maze_data);

// Ghost state management
void update_ghost_state(Ghost* ghost, int current_time);



// ===== FUNÇÕES DA FILA DE CAMINHO INTERNA =====
bool ghost_path_enqueue(Ghost* ghost, Position pos);
Position ghost_path_dequeue(Ghost* ghost);
bool ghost_path_is_empty(const Ghost* ghost);
bool ghost_path_is_full(const Ghost* ghost);
void ghost_path_clear(Ghost* ghost);
int ghost_path_size(const Ghost* ghost);
Position ghost_path_peek(const Ghost* ghost);

// ===== FUNÇÕES AVANÇADAS DE PATHFINDING =====
void ghost_plan_path_to_target(Ghost* ghost, Position target, const Maze* maze_data);
bool ghost_follow_path(Ghost* ghost, const Maze* maze_data);

// System initialization function (moved from utils.h)
void initialize_ghosts_safely(Ghost ghosts[], Maze* maze);

#endif // GHOST_H