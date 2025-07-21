#include "ghost.h"
#include "utils.h"
#include "config.h"
#include "player.h"
#include "logger.h"
#include <stdlib.h>

static const char GHOST_SYMBOLS[MAX_GHOSTS] = {SYMBOL_GHOST_RED, SYMBOL_GHOST_GREEN, SYMBOL_GHOST_BLUE, SYMBOL_GHOST_PINK};
static const Position SCATTER_TARGETS[MAX_GHOSTS] = {
    {1, 1},
    {MAX_MAP_WIDTH - 2, 1},
    {1, MAX_MAP_HEIGHT - 2},
    {MAX_MAP_WIDTH - 2, MAX_MAP_HEIGHT - 2}
};

void init_ghosts(Ghost ghosts[], int count) {
    for (int i = 0; i < count; i++) {
        ghosts[i].ghost_id = i;
        ghosts[i].symbol = GHOST_SYMBOLS[i % MAX_GHOSTS];
        ghosts[i].state = GHOST_NORMAL;
        ghosts[i].is_active = false;
        ghosts[i].difficulty = DIFFICULTY_MEDIUM;
        ghosts[i].scatter_mode = false;
        ghosts[i].timer = 0;
        ghosts[i].pos = (Position){1,1};
        ghosts[i].initial_pos = ghosts[i].pos;
        ghosts[i].direction = NORTH;
        ghosts[i].target = SCATTER_TARGETS[i % MAX_GHOSTS];
    }
}

Direction calculate_next_direction(const Ghost* ghost, Position pacman_pos, Direction pacman_dir, const Ghost* ghosts, const Maze* maze_data) {
    if (!maze_data || !ghost) return DIR_INVALID;
    Direction best_dir = ghost->direction;
    long min_distance = -1;
    Position target = calculate_target_position(ghost, pacman_pos, pacman_dir, ghosts);
    for (Direction dir = NORTH; dir <= WEST; dir++) {
        if (!is_valid_direction(dir)) continue;
        Position next_pos = get_next_position(ghost->pos, dir);
        bool allow_reverse = (ghost->state == GHOST_EATEN || ghost->state == GHOST_FRIGHTENED);
        if (!is_valid_move_ghost(next_pos, maze_data) || (!allow_reverse && dir == get_opposite_direction(ghost->direction))) {
            continue;
        }
        long distance;
        if (ghost->state == GHOST_FRIGHTENED) {
            distance = -manhattan_distance(next_pos, pacman_pos);
            distance += (rand() % 50) - 25;
        } else {
            distance = manhattan_distance(next_pos, target);
        }
        if (min_distance == -1 || distance < min_distance) {
            min_distance = distance;
            best_dir = dir;
        }
    }
    if (min_distance == -1) {
        Position next_pos_current_dir = get_next_position(ghost->pos, ghost->direction);
        if (is_valid_move_ghost(next_pos_current_dir, maze_data)) {
            return ghost->direction;
        }
        return DIR_INVALID;
    }
    return best_dir;
}

Position calculate_target_position(const Ghost* ghost, const Position pacman_pos, Direction pacman_dir, const Ghost* ghosts) {
    // EATEN: volta para base
    if (ghost->state == GHOST_EATEN) {
        return ghost->initial_pos;
    }
    // FRIGHTENED: tenta se afastar do Pac-Man (com aleatoriedade)
    if (ghost->state == GHOST_FRIGHTENED) {
        Position target = pacman_pos;
        target.x += (rand() % 7) - 3; // Aleatoriedade
        target.y += (rand() % 7) - 3;
        return target;
    }
    // SCATTER: cada um vai para seu canto
    if (ghost->scatter_mode) {
        switch (ghost->ghost_id % MAX_GHOSTS) {
            case 0: return (Position){MAX_MAP_WIDTH - 2, 1}; // Blinky: canto superior direito
            case 1: return (Position){1, 1};                // Pinky: canto superior esquerdo
            case 2: return (Position){MAX_MAP_WIDTH - 2, MAX_MAP_HEIGHT - 2}; // Inky: canto inferior direito
            case 3: return (Position){1, MAX_MAP_HEIGHT - 2}; // Clyde: canto inferior esquerdo
        }
    }
    // CHASE: comportamento individual
    switch (ghost->ghost_id % MAX_GHOSTS) {
        case 0: // Blinky (vermelho): persegue Pac-Man
            return pacman_pos;
        case 1: { // Pinky (rosa): mira 4 casas à frente da direção do Pac-Man
            Position target = pacman_pos;
            for (int i = 0; i < 4; i++) {
                target = get_next_position(target, pacman_dir);
            }
            return target;
        }
        case 2: { // Inky (azul): vetor entre Blinky e 2 casas à frente do Pac-Man, duplicado
            // Blinky é ghost 0
            Position blinky_pos = ghosts[0].pos;
            Position ahead = pacman_pos;
            for (int i = 0; i < 2; i++) {
                ahead = get_next_position(ahead, pacman_dir);
            }
            int vx = ahead.x - blinky_pos.x;
            int vy = ahead.y - blinky_pos.y;
            Position target = {blinky_pos.x + 2 * vx, blinky_pos.y + 2 * vy};
            return target;
        }
        case 3: { // Clyde (verde): persegue se longe (>8), foge se perto
            int dist = manhattan_distance(ghost->pos, pacman_pos);
            if (dist > 8) {
                return pacman_pos;
            } else {
                return (Position){1, MAX_MAP_HEIGHT - 2}; // canto inferior esquerdo
            }
        }
    }
    // Fallback
    return pacman_pos;
}

bool is_valid_move_ghost(Position pos, const Maze* maze_data) {
    if (!maze_data) return false;
    if (pos.x < 0 || pos.x >= maze_data->width || pos.y < 0 || pos.y >= maze_data->height) {
        return false;
    }
    return maze_data->grid[pos.y][pos.x] != SYMBOL_WALL;
}

void update_ghost_state(Ghost* ghost, int timer_value) {
    (void)timer_value;
    if (ghost->state == GHOST_NORMAL) {
        if (ghost->timer % (SCATTER_CHASE_INTERVAL * 3) == 0 && ghost->timer > 0) {
            ghost->scatter_mode = !ghost->scatter_mode;
            LOG_D("Fantasma %d (%c) modo: %s.", ghost->ghost_id, ghost->symbol, ghost->scatter_mode ? "SCATTER" : "CHASE");
        }
    }
    if (ghost->state == GHOST_FRIGHTENED) {
        if (ghost->timer > FRIGHTENED_MODE_DURATION) {
            ghost->state = GHOST_NORMAL;
            ghost->scatter_mode = false;
            ghost->timer = 0;
            LOG_D("Fantasma %d (%c) não está mais FRIGHTENED.", ghost->ghost_id, ghost->symbol);
        }
    }
}

void set_ghost_difficulty(Ghost* ghost, DifficultyLevel difficulty) {
    if (!ghost) return;
    ghost->difficulty = difficulty;
    LOG_D("Fantasma %d: dificuldade alterada para %d", ghost->ghost_id, difficulty);
}

void reset_ghost(Ghost* ghost) {
    if (!ghost) return;
    ghost->state = GHOST_NORMAL;
    ghost->scatter_mode = true;
    ghost->timer = 0;
    ghost->is_active = true;
    ghost->pos = ghost->initial_pos;
    ghost->direction = NORTH;
    LOG_D("Fantasma %d (%c) resetado para pos (%d,%d), estado NORMAL.", ghost->ghost_id, ghost->symbol, ghost->pos.x, ghost->pos.y);
}

bool check_collision_with_pacman(Player* player, Ghost ghosts[], int count, Position pacman_pos) {
    for (int i = 0; i < count; i++) {
        if (!ghosts[i].is_active || ghosts[i].state == GHOST_EATEN) continue;
        if (positions_equal(ghosts[i].pos, pacman_pos)) {
            if (ghosts[i].state == GHOST_FRIGHTENED) {
                ghosts[i].state = GHOST_EATEN;
                ghosts[i].is_active = false;
                ghosts[i].timer = 0;
                if (player) {
                    player->score += POINTS_PER_GHOST_EATEN;
                    LOG_I("Fantasma %d (%c) comido! +%d pontos. Score: %d",
                          ghosts[i].ghost_id, ghosts[i].symbol, POINTS_PER_GHOST_EATEN, player->score);
                } else {
                    LOG_W("Player é NULL em check_collision_with_pacman ao dar score.");
                }
                return false;
            }
            LOG_I("Colisão fatal! Pacman (%d,%d) vs Fantasma %d (%c) (%d,%d)",
                  pacman_pos.x, pacman_pos.y, ghosts[i].ghost_id, ghosts[i].symbol, ghosts[i].pos.x, ghosts[i].pos.y);
            return true;
        }
    }
    return false;
}