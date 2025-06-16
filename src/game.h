#ifndef GAME_H
#define GAME_H

#include "config.h"
#include "utils.h" // Para Position, GameStatus, Player (se definido aqui)
#include "player.h"// Para Player (se não estiver em utils.h)
#include "maze.h"  // Para Maze
#include "ghost.h" // Para Ghost
#include <stdbool.h>

// Interface functions
void show_title_screen(void);
void show_game_over_screen(Player* player, GameStatus game_status, int current_level);

// Game status management
// Assinatura atualizada para refletir as mudanças em game.c e adicionar player_start_pos
void update_game(Player* player, Maze* maze_data, Ghost* ghosts, int ghost_count, GameStatus* game_status, bool* game_over_flag, Position player_start_pos);
// Assinatura atualizada para refletir as mudanças em game.c
void draw_game(Player* player, Maze* maze_data, Ghost* ghosts, int ghost_count, GameStatus game_status);


// Assinatura atualizada para refletir as mudanças em game.c
bool process_player_input(Player* player, Maze* maze_data, Ghost* ghosts, int ghost_count, GameStatus* game_status, char input_char);

void handle_debug_command(char input);
void toggle_pause(GameStatus* status);
void update_score(int* score, char maze_item);

#endif // GAME_H