#ifndef GAME_H
#define GAME_H

#include "utils.h"    // Para Position, Direction, GameStatus
#include "ghost.h"    // Para Ghost
#include <stdbool.h>

// Interface functions
void show_title_screen(void);
void show_game_over_screen(Player* player, GameStatus game_status, int current_level);

// Game status management
void update_game(Ghost ghosts[], int ghost_count, Position *pacman_pos, bool *game_over, const char* maze);
// Note: draw_game foi movida para maze.c como maze_render_with_ghosts

// Player control functions
Direction get_player_input(void);
bool process_player_input(char input, Position* pacman_pos, const char* maze, GameStatus* status);

// Game state functions
void handle_debug_command(char input);
void toggle_pause(GameStatus* status);
void update_score(int* score, char maze_item);

#endif // GAME_H