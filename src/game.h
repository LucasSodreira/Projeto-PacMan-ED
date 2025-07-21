#ifndef GAME_H
#define GAME_H

#include "config.h"
#include "utils.h"
#include "player.h"
#include "maze.h"
#include "ghost.h"
#include <stdbool.h>

void show_title_screen(void);
void show_game_over_screen(Player* player, GameStatus game_status, int current_level);
void update_game(Player* player, Maze* maze_data, Ghost* ghosts, int ghost_count, GameStatus* game_status, bool* game_over_flag, Position player_start_pos);
void draw_game(Player* player, Maze* maze_data, Ghost* ghosts, int ghost_count, GameStatus game_status, int current_level);
void process_player_input(Player* player, Maze* maze_data, Ghost* ghosts, int ghost_count, GameStatus* game_status, char input_char);
void handle_debug_command(char input);
void toggle_pause(GameStatus* status);
void update_score(int* score, char maze_item);

#endif // GAME_H