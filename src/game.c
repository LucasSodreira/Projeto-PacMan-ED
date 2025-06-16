#include "game.h"
#include "logger.h"
#include "config.h" // Para KEY_MOVE_UP, etc. e SYMBOL_*
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

// Adicionar includes necessários para as novas structs na assinatura de process_player_input
#include "player.h" // Para Player struct e player_move
#include "maze.h"   // Para Maze struct
#include "ghost.h"  // Para Ghost struct

// Assinatura atualizada para usar Player* e Maze* e incluir player_start_pos
void update_game(Player* player, Maze* maze_data, Ghost ghosts[], int ghost_count, GameStatus* game_status, bool* game_over_flag, Position player_start_pos) {
    // A movimentação dos fantasmas (move_ghosts) é chamada no loop principal em main.c

    // Checa colisão entre Pacman e fantasmas
    if (check_collision_with_pacman(player, ghosts, ghost_count, player->pos)) {
        // Se check_collision_with_pacman retornar true, significa colisão fatal.
        player_lose_life(player, player_start_pos);

        if (player->lives <= 0) {
            *game_over_flag = true;
            *game_status = GAME_OVER;
            LOG_I("GAME OVER! Pacman ficou sem vidas.");
        } else {
            LOG_I("Pacman perdeu uma vida! Vidas restantes: %d. Resetando posições.", player->lives);
            for (int i = 0; i < ghost_count; i++) {
                reset_ghost(&ghosts[i]);
            }
            // O jogador já foi resetado para player_start_pos dentro de player_lose_life.
            // O jogo continua, então não é necessário definir game_over_flag = true aqui.
        }
    }

    // Checa condição de vitória (SOMENTE se o jogo não acabou por colisão fatal)
    if (!(*game_over_flag)) {
        if (maze_count_points(maze_data) == 0) {
            *game_over_flag = true;
            *game_status = VICTORY;
            LOG_I("VITÓRIA! Todos os pontos foram coletados!");
        }
    }
}

// Assinatura atualizada para usar Player* e Maze*
void draw_game(Player* player, Maze* maze_data, Ghost ghosts[], int ghost_count, GameStatus game_status) {
    clear_screen();
    
    // Exibe Score, Vidas e Estado do Jogo. O Nível é exibido em main.c.
    printf("Score: %d | Vidas: %d | Estado: %s\n", player->score, player->lives, game_status_to_string(game_status));

    printf("╔");
    for (int i = 0; i < maze_data->width; i++) printf("═");
    printf("╗\n");

    for (int y = 0; y < maze_data->height; y++) {
        printf("║");
        for (int x = 0; x < maze_data->width; x++) {
            Position current_pos = {x, y};
            char symbol_on_maze = maze_data->grid[y][x];

            bool is_ghost = false;
            for (int i = 0; i < ghost_count; i++) {
                if (ghosts[i].is_active && positions_equal(ghosts[i].pos, current_pos)) {
                    int color = COLOR_RESET;
                    char ghost_symbol_to_draw = ghosts[i].symbol; // Símbolo padrão do fantasma

                    if (ghosts[i].state == GHOST_FRIGHTENED) {
                        ghost_symbol_to_draw = SYMBOL_GHOST_FRIGHTENED; // Usa SYMBOL_GHOST_FRIGHTENED de config.h
                        color = COLOR_GHOST_FRIGHTENED;                 // Usa COLOR_GHOST_FRIGHTENED de config.h
                    } else if (ghosts[i].state == GHOST_EATEN) {
                        ghost_symbol_to_draw = SYMBOL_GHOST_EATEN;      // Usa SYMBOL_GHOST_EATEN de config.h
                        color = COLOR_GHOST_EATEN;                      // Usa COLOR_GHOST_EATEN de config.h
                    } else { // Estado GHOST_NORMAL
                         switch (ghosts[i].symbol) {
                            case SYMBOL_GHOST_RED:   color = COLOR_GHOST_RED; break;
                            case SYMBOL_GHOST_GREEN: color = COLOR_GHOST_GREEN; break;
                            case SYMBOL_GHOST_BLUE:  color = COLOR_GHOST_BLUE; break;
                            case SYMBOL_GHOST_PINK:  color = COLOR_GHOST_PINK; break;
                            default: color = COLOR_RESET;
                        }
                    }
                    printf("\x1b[%dm%c\x1b[0m", color, ghost_symbol_to_draw);
                    is_ghost = true;
                    break;
                }
            }

            if (!is_ghost) {
                if (positions_equal(player->pos, current_pos)) {
                    printf("\x1b[%dm%c\x1b[0m", COLOR_PLAYER, player->symbol); // SYMBOL_PLAYER de config.h
                } else {
                    switch (symbol_on_maze) {
                        case SYMBOL_WALL: printf("\x1b[%dm%c\x1b[0m", COLOR_WALL, SYMBOL_WALL); break; // Usa SYMBOL_WALL
                        case SYMBOL_DOT: printf("\x1b[%dm%c\x1b[0m", COLOR_DOT, SYMBOL_DOT); break;
                        case SYMBOL_POWER_PELLET: printf("\x1b[%dm%c\x1b[0m", COLOR_POWER_PELLET, SYMBOL_POWER_PELLET); break;
                        default: printf("%c", SYMBOL_EMPTY_SPACE); break;
                    }
                }
            }
        }
        printf("║\n");
    }

    printf("╚");
    for (int i = 0; i < maze_data->width; i++) printf("═"); // Usa maze_data->width
    printf("╝\n");
}

// process_player_input lida com input do jogador e chama player_move.
bool process_player_input(Player* player, Maze* maze_data, Ghost ghosts[], int ghost_count, GameStatus* game_status, char input_char) {
    bool movement_attempted = false;
    
    switch (toupper(input_char)) {
        case KEY_MOVE_UP:
        case KEY_MOVE_LEFT:
        case KEY_MOVE_DOWN:
        case KEY_MOVE_RIGHT:
            player_move(player, maze_data, input_char, ghosts, ghost_count);
            movement_attempted = true;
            break;
        case KEY_PAUSE:
            toggle_pause(game_status);
            break;
        case KEY_QUIT:
            *game_status = GAME_OVER;
            LOG_I("Jogo encerrado pelo jogador.");
            break;
        // Mantendo teclas de debug como exemplo.
        case 'D':
        case 'S':
        case 'L':
            handle_debug_command(input_char);
            break;
    }
    return movement_attempted;
}

void handle_debug_command(char input) {
    switch (tolower(input)) {
        case 'd':
            LOG_D("Comando de debug: Dump (não implementado)");
            // logger_print_stats(); // Se existir
            break;
        case 's':
            LOG_D("Comando de debug: Stats (não implementado)");
            // logger_print_stats(); // Se existir
            break;
        case 'l':
            LOG_D("Comando de debug: Log Level (não implementado)");
            // logger_set_level(LOG_DEBUG); // Se existir
            break;
    }
}

void toggle_pause(GameStatus* status) {
    if (*status == PLAYING) { // Apenas pausa se estiver jogando
        *status = PAUSED;
        LOG_I("Jogo PAUSADO.");
    } else if (*status == PAUSED) { // Apenas retoma se estiver pausado
        *status = PLAYING;
        LOG_I("Jogo RETOMADO.");
    }
}

void update_score(int* score, char maze_item) {
    int points = 0;
    switch (maze_item) {
        case SYMBOL_DOT:
            points = POINTS_PER_DOT;
            break;
        case SYMBOL_POWER_PELLET:
            points = POINTS_PER_POWER_PELLET;
            break;
        // A pontuação por comer fantasmas é tratada em check_collision_with_pacman.
    }
    
    if (points > 0) {
        *score += points;
    }
}