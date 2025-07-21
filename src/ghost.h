#ifndef GHOST_H
#define GHOST_H

#include <stdbool.h>
#include "config.h"
#include "utils.h"
#include "maze.h"

typedef struct Player Player;

#define GHOST_MOVE_DELAY 2
#define GHOST_EATEN_DURATION 200

struct Ghost {
    Position pos;
    Direction direction;
    int ghost_id;
    char symbol;
    GhostState state;
    int is_active;
    Position target;
    DifficultyLevel difficulty;
    int scatter_mode;
    int timer;
    Position initial_pos;
};

void init_ghosts(Ghost ghosts[], int count);
bool check_collision_with_pacman(Player* player, Ghost ghosts[], int count, Position pacman_pos);
Direction calculate_next_direction(const Ghost* ghost, Position pacman_pos, Direction pacman_dir, const Ghost* ghosts, const Maze* maze_data);
Position calculate_target_position(const Ghost* ghost, const Position pacman_pos, Direction pacman_dir, const Ghost* ghosts);
bool is_valid_move_ghost(Position pos, const Maze* maze_data);
void update_ghost_state(Ghost* ghost, int current_time);
void set_ghost_difficulty(Ghost* ghost, DifficultyLevel difficulty);
void reset_ghost(Ghost* ghost);

#endif // GHOST_H