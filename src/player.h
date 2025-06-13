#ifndef PLAYER_H
#define PLAYER_H

#include "utils.h"
#include "maze.h"

// Define quantas vidas o Pac-Man começa
#define INITIAL_LIVES 3

typedef struct {
    Position pos;
    Direction dir;
    int score;
    int lives;
    int moves;
} Player;

// Inicializa o jogador na posição inicial do labirinto
void init_player(Player *player, Position start_pos);

// Atualiza o jogador com base na entrada do usuário e estado do labirinto
void update_player(Player *player, char **maze);

// Verifica se o jogador venceu (todos os pontos foram coletados)
bool check_victory(char **maze);

// Imprime visualmente a HUD de status (vidas, score, etc)
void display_player_status(const Player *player);

#endif
