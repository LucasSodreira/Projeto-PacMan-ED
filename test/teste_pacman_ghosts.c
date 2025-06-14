#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define MAX_GHOSTS 2
#define MAP_SIZE 10

typedef struct {
    int x, y;
} Position;

typedef struct {
    Position pos;
    Position fila[100];
    int ini, fim;
} Ghost;

void init_ghosts(Ghost ghosts[], int n) {
    for (int i = 0; i < n; i++) {
        ghosts[i].pos.x = rand() % MAP_SIZE;
        ghosts[i].pos.y = rand() % MAP_SIZE;
        ghosts[i].ini = 0;
        ghosts[i].fim = 0;
    }
}

void enqueue(Ghost *g, Position p) {
    g->fila[g->fim++] = p;
}

void move_ghosts(Ghost ghosts[], int n) {
    for (int i = 0; i < n; i++) {
        if (ghosts[i].ini < ghosts[i].fim) {
            ghosts[i].pos = ghosts[i].fila[ghosts[i].ini++];
        } else {
            Position next = {
                ghosts[i].pos.x + (rand() % 3 - 1),
                ghosts[i].pos.y + (rand() % 3 - 1)
            };
            if (next.x < 0) next.x = 0;
            if (next.y < 0) next.y = 0;
            if (next.x >= MAP_SIZE) next.x = MAP_SIZE - 1;
            if (next.y >= MAP_SIZE) next.y = MAP_SIZE - 1;
            enqueue(&ghosts[i], next);
        }
    }
}

bool check_collision(Position pac, Ghost ghosts[], int n) {
    for (int i = 0; i < n; i++) {
        if (ghosts[i].pos.x == pac.x && ghosts[i].pos.y == pac.y)
            return true;
    }
    return false;
}

int main() {
    srand(time(NULL));

    Position pacman = {5, 5};
    Ghost ghosts[MAX_GHOSTS];
    bool game_over = false;

    init_ghosts(ghosts, MAX_GHOSTS);

    while (!game_over) {
        // Movimento simples do Pac-Man
        pacman.x += rand() % 3 - 1;
        pacman.y += rand() % 3 - 1;
        if (pacman.x < 0) pacman.x = 0;
        if (pacman.y < 0) pacman.y = 0;
        if (pacman.x >= MAP_SIZE) pacman.x = MAP_SIZE - 1;
        if (pacman.y >= MAP_SIZE) pacman.y = MAP_SIZE - 1;

        move_ghosts(ghosts, MAX_GHOSTS);

        printf("\nPac-Man em (%d, %d)\n", pacman.x, pacman.y);
        for (int i = 0; i < MAX_GHOSTS; i++) {
            printf("Fantasma %d em (%d, %d)\n", i, ghosts[i].pos.x, ghosts[i].pos.y);
        }

        if (check_collision(pacman, ghosts, MAX_GHOSTS)) {
            printf("\nGAME OVER! O Pac-Man foi pego!\n");
            game_over = true;
        }

        getchar(); // Pressione Enter para continuar
    }

    return 0;
}
