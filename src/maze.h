#ifndef MAZE_H
#define MAZE_H

#include "utils.h"    // ✅ Para Position, logging
#include "config.h"   // ✅ Para constantes

#ifndef MAZE_WIDTH
#define MAZE_WIDTH 10
#endif

#ifndef MAZE_HEIGHT
#define MAZE_HEIGHT 5
#endif

typedef struct {
    char grid[MAZE_HEIGHT][MAZE_WIDTH];  
    int width;
    int height;
    int total_points;
} Maze;

// Funções principais
void maze_init(Maze* maze);
void maze_render(const Maze* maze, Player* player);  
int maze_is_wall(const Maze* maze, Position pos);
int maze_has_point(const Maze* maze, Position pos);
void maze_remove_point(Maze* maze, Position pos);
int maze_count_points(const Maze* maze);

#endif
