#ifndef PLAYER_H
#define PLAYER_H

#include "utils.h" // Para Player, Position, Direction, GameState (se Player estiver aqui)
#include "maze.h"  // Para Maze
#include "ghost.h" // Para Ghost (necessário para a assinatura de player_move)

// Estrutura Player (se não estiver definida em utils.h ou outro lugar)
// Supondo que Player é definido em utils.h ou similar e GameState também.
// Se GameState (de player_has_won) tiver Player, Maze, Ghosts, etc., certifique-se de que as definições estejam disponíveis.


// Inicializa o player (posição e valores iniciais)
void player_init(Player* player, Position start_pos);

// Move o player de acordo com o input, atualizando score e vidas, e interagindo com fantasmas (power pellet)
void player_move(Player* player, Maze* maze, char input, Ghost ghosts[], int ghost_count);

// Checa se o player venceu (coletou todos os pontos)
// GameState deve ser uma struct que contém o necessário para essa verificação (ex: contagem de pontos totais e coletados)
int player_has_won(Player* player, GameState* game);

// Processa perda de vida (decrementa e reseta posição se houver vidas)
void player_lose_life(Player* player, Position start_pos);

#endif // PLAYER_H
