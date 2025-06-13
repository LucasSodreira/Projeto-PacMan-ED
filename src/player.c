#include <stdio.h>
#include "player.h"

void init_pacman(Pacman *p, int x, int y) {
    p->x = x;
    p->y = y;
    p->score = 0;
    p->lives = 3;
}

void handle_movement(Pacman *p, int newX, int newY, char maze[MAP_HEIGHT][MAP_WIDTH]) {
    char dest = maze[newY][newX];

    if (dest == '#') return; // Não atravessa parede

    if (dest == '.') {
        p->score++;
        maze[newY][newX] = ' ';
    }

    // Atualiza posição no labirinto
    maze[p->y][p->x] = ' ';
    p->x = newX;
    p->y = newY;
    maze[p->y][p->x] = 'P';
}

void move_pacman(Pacman *p, char direction, char maze[MAP_HEIGHT][MAP_WIDTH]) {
    int dx = 0, dy = 0;

    switch (direction) {
        case 'w': dy = -1; break;
        case 's': dy = 1; break;
        case 'a': dx = -1; break;
        case 'd': dx = 1; break;
        default: return;
    }

    int newX = p->x + dx;
    int newY = p->y + dy;

    if (newX < 0 || newY < 0 || newX >= MAP_WIDTH || newY >= MAP_HEIGHT)
        return;

    handle_movement(p, newX, newY, maze);
}

int check_victory(char maze[MAP_HEIGHT][MAP_WIDTH]) {
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            if (maze[y][x] == '.') return 0;
        }
    }
    return 1;
}