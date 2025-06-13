#include "maze.h"

static const char* simple_map[MAZE_HEIGHT] = {
    "##########",
    "#..#.....#",
    "#..#..#..#",
    "#.......##",
    "##########"
};

void maze_init(Maze* maze) {
    for (int y = 0; y < MAZE_HEIGHT; y++) {
        for (int x = 0; x < MAZE_WIDTH; x++) {
            char c = simple_map[y][x];
            if (c == '#') maze->grid[y][x] = CELL_WALL;
            else if (c == '.') maze->grid[y][x] = CELL_POINT;
            else maze->grid[y][x] = CELL_EMPTY;
        }
    }
}

int maze_is_wall(const Maze* maze, Position pos) {
    if (pos.x < 0 || pos.x >= MAZE_WIDTH || pos.y < 0 || pos.y >= MAZE_HEIGHT)
        return 1;
    return maze->grid[pos.y][pos.x] == CELL_WALL;
}

int maze_has_point(const Maze* maze, Position pos) {
    if (pos.x < 0 || pos.x >= MAZE_WIDTH || pos.y < 0 || pos.y >= MAZE_HEIGHT)
        return 0;
    return maze->grid[pos.y][pos.x] == CELL_POINT;
}

void maze_remove_point(Maze* maze, Position pos) {
    if (pos.x < 0 || pos.x >= MAZE_WIDTH || pos.y < 0 || pos.y >= MAZE_HEIGHT)
        return;
    if (maze->grid[pos.y][pos.x] == CELL_POINT)
        maze->grid[pos.y][pos.x] = CELL_EMPTY;
}
