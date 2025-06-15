#ifndef MAZE_H
#define MAZE_H

#include "utils.h"    // ✅ Para Position, logging
#include "config.h"   // ✅ Para constantes

// Use apenas MAX_MAP_WIDTH/MAX_MAP_HEIGHT de config.h
#define MAZE_WIDTH MAX_MAP_WIDTH
#define MAZE_HEIGHT MAX_MAP_HEIGHT

typedef struct {
    char grid[MAZE_HEIGHT][MAZE_WIDTH];
    int width;
    int height;
    int total_points;
} Maze;

// Funções principais
void maze_init(Maze* maze);
void maze_render(const Maze* maze, Player* player);  
void maze_render_with_ghosts(const Maze* maze, Player* player, Ghost* ghosts, int ghost_count);
int maze_is_wall(const Maze* maze, Position pos);
int maze_has_point(const Maze* maze, Position pos);
void maze_remove_point(Maze* maze, Position pos);
int maze_count_points(const Maze* maze);

// Função utilitária para carregar o labirinto (stub)
char* load_maze(int level);
#endif
