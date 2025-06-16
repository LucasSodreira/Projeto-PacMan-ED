#ifndef PLAYER_H
#define PLAYER_H

#include "utils.h"    // Para Position, Direction, Player, GameState

// Forward declaration para Maze
typedef struct Maze Maze;

// Inicializa o player (posição e valores iniciais)
void player_init(Player* player, Position start_pos);

// Move o player de acordo com o input, atualizando score e vidas
void player_move(Player* player, Maze* maze, char input);

// Checa se o player venceu (coletou todos os pontos)
int player_has_won(Player* player, GameState* game);

// Processa perda de vida (decrementa e reseta posição se houver vidas)
void player_lose_life(Player* player, Position start_pos);

// Função auxiliar para converter input em direção
Direction get_direction_from_input(char input);

#endif // PLAYER_H
