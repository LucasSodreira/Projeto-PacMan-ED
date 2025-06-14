#ifndef GAME_H
#define GAME_H

#include "ghost.h"
#include "utils.h"
#include <stdbool.h>

// Game status management
void update_game(Ghost ghosts[], int ghost_count, Position *pacman_pos, bool *game_over, const char* maze);
void draw_game(Ghost ghosts[], int ghost_count, Position pacman_pos, const char* maze);

// Player control functions
Direction get_player_input(void);
bool process_player_input(char input, Position* pacman_pos, const char* maze, GameStatus* status);

// Game state functions
void handle_debug_command(char input);
void toggle_pause(GameStatus* status);
void update_score(int* score, char maze_item);

#endif