#ifndef MAZE_H
#define MAZE_H

#include "utils.h"

#define MAZE_WIDTH  10
#define MAZE_HEIGHT 5

typedef enum {
    CELL_EMPTY,
    CELL_WALL,
    CELL_POINT
} CellType;

typedef struct {
    CellType grid[MAZE_HEIGHT][MAZE_WIDTH];
} Maze;

void maze_init(Maze* maze);
int maze_is_wall(const Maze* maze, Position pos);
int maze_has_point(const Maze* maze, Position pos);
void maze_remove_point(Maze* maze, Position pos);

#endif
