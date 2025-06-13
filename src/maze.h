#ifndef MAZE_H
#define MAZE_H

typedef struct {
    char** grid;      // matriz do labirinto (ex: ' ' para espaço livre, '#' para parede, '.' para ponto)
    int width;
    int height;
    int total_points; // total de pontos no labirinto
} Maze;

#endif
