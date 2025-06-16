#ifndef PLAYER_H
#define PLAYER_H

#include "config.h"
#include "utils.h"
#include "maze.h"

// Forward declaration para evitar dependência circular
typedef struct Ghost Ghost;

void player_init(Player* player, Position start_pos);
void player_move(Player* player, Maze* maze, char input, Ghost* ghosts, int ghost_count);
int player_has_won(Player* player, GameState* game);
void player_lose_life(Player* player, Position start_pos);

#endif
