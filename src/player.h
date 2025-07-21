#ifndef PLAYER_H
#define PLAYER_H

#include "config.h"
#include "utils.h"
#include "maze.h"
#include "ghost.h"

void player_init(Player* player, Position start_pos);
void player_move(Player* player, Maze* maze, char input, Ghost* ghosts, int ghost_count);
void player_lose_life(Player* player, Position start_pos);

#endif // PLAYER_H
