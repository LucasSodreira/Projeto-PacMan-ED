#ifndef PLAYER_H
#define PLAYER_H

#include "../maze/maze.h"  // Define o mapa (parede, ponto, etc.)

typedef struct {
    int x, y;          // Posição atual do Pac-Man
    int score;         // Pontuação
    int lives;         // Vidas restantes
} Pacman;

// Inicializa o Pac-Man com posição, pontuação e vidas
void init_pacman(Pacman *p, int x, int y);

// Move o Pac-Man na direção desejada
void move_pacman(Pacman *p, char direction, char maze[MAP_HEIGHT][MAP_WIDTH]);

// Verifica se todos os pontos foram coletados (condição de vitória)
int check_victory(char maze[MAP_HEIGHT][MAP_WIDTH]);

// Atualiza o estado do Pac-Man com base na casa para onde ele se moveu
void handle_movement(Pacman *p, int newX, int newY, char maze[MAP_HEIGHT][MAP_WIDTH]);

#endif