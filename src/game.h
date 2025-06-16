#ifndef GAME_H
#define GAME_H

#include "utils.h" // Para Position, GameStatus, Player (se definido aqui)
#include "maze.h"  // Para Maze
#include "ghost.h" // Para Ghost
#include "player.h"// Para Player (se não estiver em utils.h)
#include <stdbool.h>

// Game status management
// Assinatura atualizada para refletir as mudanças em game.c e adicionar player_start_pos
void update_game(Player* player, Maze* maze_data, Ghost ghosts[], int ghost_count, GameStatus* game_status, bool* game_over_flag, Position player_start_pos);
// Assinatura atualizada para refletir as mudanças em game.c
void draw_game(Player* player, Maze* maze_data, Ghost ghosts[], int ghost_count, GameStatus game_status);

// Player control functions
// get_player_input não está definida no código fornecido, mas é chamada em main.c.
// Se for uma função global, sua declaração deve estar aqui ou em um header apropriado como input.h.
// Por enquanto, vou mantê-la comentada se não for parte do escopo direto.
// Direction get_player_input(void);

// Assinatura atualizada para refletir as mudanças em game.c
bool process_player_input(Player* player, Maze* maze_data, Ghost ghosts[], int ghost_count, GameStatus* game_status, char input_char);

// Game state functions
// handle_debug_command não está totalmente implementada ou usada consistentemente, pode ser removida/revisada.
// void handle_debug_command(char input);
void toggle_pause(GameStatus* status);
void update_score(int* score, char maze_item);

#endif