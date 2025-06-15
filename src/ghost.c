#include "ghost.h"
#include "utils.h"
#include "config.h"
#include <stdlib.h>
#include <string.h>

static const char GHOST_SYMBOLS[MAX_GHOSTS] = {'F', 'G', 'B', 'R'}; // Red, Green, Blue, Pink
static const Position SCATTER_TARGETS[MAX_GHOSTS] = {{2,2}, {2,27}, {27,2}, {27,27}}; // Corner positions

void init_ghosts(Ghost ghosts[], int count) {
    for (int i = 0; i < count; i++) {
        ghosts[i].ghost_id = i;
        ghosts[i].symbol = GHOST_SYMBOLS[i % MAX_GHOSTS];
        ghosts[i].state = GHOST_NORMAL;
        ghosts[i].is_active = 1;
        ghosts[i].difficulty = DIFFICULTY_MEDIUM;
        ghosts[i].scatter_mode = 1;
        ghosts[i].timer = 0;
        ghosts[i].path_start = 0;
        ghosts[i].path_end = 0;
        
        // Posições iniciais mais seguras e visíveis
        ghosts[i].pos.x = 5 + (i * 4);
        ghosts[i].pos.y = 5 + (i % 2);
        ghosts[i].direction = NORTH;
        
        ghosts[i].target = SCATTER_TARGETS[i % MAX_GHOSTS];
        
        LOG_I("Fantasma %d (%c) criado na posição (%d,%d)", 
              i, ghosts[i].symbol, ghosts[i].pos.x, ghosts[i].pos.y);
    }
}

Direction calculate_next_direction(const Ghost* ghost, const Position pacman_pos, const char* maze) {
    Direction best_dir = ghost->direction;
    int min_distance = 1000000;
    Position target = calculate_target_position(ghost, pacman_pos);
    
    // Check each possible direction
    for (Direction dir = NORTH; dir <= WEST; dir++) {
        Position next_pos = get_next_position(ghost->pos, dir);
        
        // Skip if move is invalid or opposite to current direction (no 180° turns)
        if (!is_valid_move_ghost(next_pos, maze) || 
            (dir == get_opposite_direction(ghost->direction) && ghost->state != GHOST_FRIGHTENED)) {
            continue;
        }
        
        int distance = manhattan_distance(next_pos, target);
        
        // For frightened mode, use random movement with some intelligence
        if (ghost->state == GHOST_FRIGHTENED) {
            distance = rand() % 100 + manhattan_distance(next_pos, pacman_pos);
        }
        
        // Update best direction if this is better
        if (distance < min_distance) {
            min_distance = distance;
            best_dir = dir;
        }
    }
    
    return best_dir;
}

// Nova função que trabalha com estrutura Maze
Direction calculate_next_direction_maze(const Ghost* ghost, const Position pacman_pos, const Maze* maze) {
    Direction best_dir = ghost->direction;
    int min_distance = 1000000;
    Position target = calculate_target_position(ghost, pacman_pos);
    
    // Check each possible direction
    for (Direction dir = NORTH; dir <= WEST; dir++) {
        Position next_pos = get_next_position(ghost->pos, dir);
        
        // Skip if move is invalid or opposite to current direction (no 180° turns)
        if (!is_valid_move_ghost_maze(next_pos, maze) || 
            (dir == get_opposite_direction(ghost->direction) && ghost->state != GHOST_FRIGHTENED)) {
            continue;
        }
        
        int distance = manhattan_distance(next_pos, target);
        
        // For frightened mode, use random movement with some intelligence
        if (ghost->state == GHOST_FRIGHTENED) {
            distance = rand() % 100 + manhattan_distance(next_pos, pacman_pos);
        }
        
        // Update best direction if this is better
        if (distance < min_distance) {
            min_distance = distance;
            best_dir = dir;
        }
    }
    
    return best_dir;
}

Position calculate_target_position(const Ghost* ghost, const Position pacman_pos) {
    // In scatter mode, target corner
    if (ghost->scatter_mode) {
        return SCATTER_TARGETS[ghost->ghost_id % MAX_GHOSTS];
    }
    
    // In normal mode, behavior depends on ghost type and difficulty
    Position target = pacman_pos;
    
    switch (ghost->ghost_id % MAX_GHOSTS) {
        case 0: // Red ghost - direct chase
            // Target gets more accurate with higher difficulty
            if (ghost->difficulty == DIFFICULTY_EASY) {
                target.x += (rand() % 5) - 2;
                target.y += (rand() % 5) - 2;
            }
            break;
            
        case 1: // Green ghost - predict ahead
            for (int i = 0; i < (4 - ghost->difficulty); i++) {
                target = get_next_position(target, NORTH);
            }
            break;
            
        case 2: // Blue ghost - flank
            target.x = pacman_pos.x + (ghost->pos.x - pacman_pos.x) / 2;
            target.y = pacman_pos.y + (ghost->pos.y - pacman_pos.y) / 2;
            break;
            
        case 3: // Pink ghost - random movement when far, direct chase when close
            if (manhattan_distance(ghost->pos, pacman_pos) > 8) {
                target = SCATTER_TARGETS[ghost->ghost_id % MAX_GHOSTS];
            }
            break;
    }
    
    return target;
}

void move_ghosts(Ghost ghosts[], int count, const Position pacman_pos, const GameState* game_state) {
    for (int i = 0; i < count; i++) {
        if (!ghosts[i].is_active) continue;
        
        // Update ghost state
        update_ghost_state(&ghosts[i], ghosts[i].timer++);
        
        // Calculate new direction based on AI
        Direction new_dir = calculate_next_direction(&ghosts[i], pacman_pos, (char*)game_state->map);
        ghosts[i].direction = new_dir;
        
        // Move ghost
        Position new_pos = get_next_position(ghosts[i].pos, new_dir);
        if (is_valid_move_ghost(new_pos, (char*)game_state->map)) {
            ghosts[i].pos = new_pos;
        }
    }
}

bool is_valid_move_ghost(Position pos, const char* maze) {
    // Check maze boundaries
    if (pos.x < 0 || pos.x >= MAX_MAP_WIDTH || pos.y < 0 || pos.y >= MAX_MAP_HEIGHT) {
        return false;
    }
    
    // Se maze é NULL, assumir que movimento é válido (para debug)
    if (!maze) {
        return true;
    }
    
    // Acessar maze como estrutura 2D
    // O maze é passado como ponteiro linear, então convertemos as coordenadas
    int index = pos.y * MAX_MAP_WIDTH + pos.x;
    if (index >= 0 && index < MAX_MAP_WIDTH * MAX_MAP_HEIGHT) {
        return maze[index] != '#';
    }
    
    return false;
}

// Função alternativa que aceita estrutura Maze diretamente
bool is_valid_move_ghost_maze(Position pos, const Maze* maze) {
    if (!maze) return false;
    
    // Check maze boundaries
    if (pos.x < 0 || pos.x >= maze->width || pos.y < 0 || pos.y >= maze->height) {
        return false;
    }
    
    return maze->grid[pos.y][pos.x] != '#';
}

void update_ghost_state(Ghost* ghost, int current_time) {
    // Switch between scatter and chase modes periodically
    if (current_time % 600 == 0) { // Switch every 20 seconds (assuming 30 FPS)
        ghost->scatter_mode = !ghost->scatter_mode;
    }
    
    // Update frightened state
    if (ghost->state == GHOST_FRIGHTENED) {
        if (ghost->timer > 300) { // 10 seconds of frightened mode
            ghost->state = GHOST_NORMAL;
            ghost->timer = 0;
        }
    }
}

void set_ghost_difficulty(Ghost* ghost, DifficultyLevel difficulty) {
    ghost->difficulty = difficulty;
}

void reset_ghost(Ghost* ghost) {
    ghost->state = GHOST_NORMAL;
    ghost->scatter_mode = 1;
    ghost->timer = 0;
    ghost->is_active = 1;
    ghost->path_start = 0;
    ghost->path_end = 0;
    
    // Reset to initial position
    ghost->pos.x = 14 + (ghost->ghost_id % 2);
    ghost->pos.y = 14 + (ghost->ghost_id / 2);
    ghost->direction = NORTH;
}

bool check_collision_with_pacman(Ghost ghosts[], int count, Position pacman_pos) {
    for (int i = 0; i < count; i++) {
        if (!ghosts[i].is_active) continue;
        
        if (positions_equal(ghosts[i].pos, pacman_pos)) {
            // If ghost is frightened, it gets eaten instead
            if (ghosts[i].state == GHOST_FRIGHTENED) {
                ghosts[i].state = GHOST_EATEN;
                ghosts[i].is_active = 0;
                return false;
            }
            return true;
        }
    }
    return false;
}