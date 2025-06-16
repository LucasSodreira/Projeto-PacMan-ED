#ifndef MAZE_H
#define MAZE_H

#include <stdbool.h>
#include "utils.h"
#include "config.h"

typedef struct {
    char grid[MAZE_HEIGHT][MAZE_WIDTH];
    int width;
    int height;
    int total_points;
    Position player_start_pos_from_map;
} Maze;

void maze_init(Maze* maze, int current_level, Position* out_player_start_pos, Position out_ghost_start_positions[MAX_GHOSTS], int* out_ghost_count);

struct Player;
struct Ghost;

void maze_render(const Maze* maze, struct Player* player);
void maze_render_with_ghosts(const Maze* maze, struct Player* player, struct Ghost* ghosts, int ghost_count);

int maze_is_wall(const Maze* maze, Position pos);
int maze_has_point(const Maze* maze, Position pos);
void maze_remove_point(Maze* maze, Position pos);
int maze_count_points(const Maze* maze);

bool load_maze(Maze* maze, int level, Position* out_player_start_pos, Position out_ghost_start_positions[MAX_GHOSTS], int* out_ghost_count);

#endif

