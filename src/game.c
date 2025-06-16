#include "config.h"
#include "utils.h"
#include "maze.h"
#include "ghost.h"
#include "player.h"
#include "logger.h"
#include "game.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

// ===== FUNÇÕES DE INTERFACE =====

void show_title_screen(void) {
    clear_screen();
    printf("\n");
    
    // ASCII Art do título com cores - apenas caracteres ASCII básicos
    printf("\x1b[33;1m"); // Amarelo brilhante
    printf("    +===============================================+\n");
    printf("    |                                               |\n");
    printf("    |   ######   #####   ######      ###    ###    |\n");
    printf("    |   #    ## ##   ## ##          ####  ####     |\n");
    printf("    |   ######  ####### ##          ## #### ##     |\n");
    printf("    |   ##      ##   ## ##          ##  ##  ##     |\n");
    printf("    |   ##      ##   ##  ######     ##      ##     |\n");
    printf("    |                                               |\n");
    printf("    |                 ###    ###  #####  ##    ##  |\n");
    printf("    |                 ####  #### ##   ## ###   ##  |\n");
    printf("    |                 ## #### ## ####### ####  ##  |\n");
    printf("    |                 ##  ##  ## ##   ## ## ## ##  |\n");
    printf("    |                 ##      ## ##   ## ##  ####  |\n");
    printf("    |                                               |\n");
    printf("    |                T E R M I N A L                |\n");
    printf("    |                                               |\n");
    printf("    |                 Versao %s                     |\n", GAME_VERSION);
    printf("    |                                               |\n");
    printf("    +===============================================+\n");
    printf("\x1b[0m"); // Reset cor
    
    printf("\n");
    printf("\x1b[36m"); // Ciano para informações
    printf("    +---------------------------------------------+\n");
    printf("    |               INFORMACOES                   |\n");
    printf("    +---------------------------------------------+\n");
    printf("    | Projeto    : %-27s |\n", GAME_TITLE);
    printf("    | Autor      : %-27s |\n", GAME_AUTHOR);
    printf("    | Disciplina : Estruturas de Dados           |\n");
    printf("    +---------------------------------------------+\n");
    printf("\x1b[0m"); // Reset cor
    printf("\n");
    print_instructions();
}

void show_game_over_screen(Player* player, GameStatus game_status, int current_level) {
    clear_screen();
    printf("\n\n");
    
    if (game_status == VICTORY) {
        printf("\x1b[32;1m"); // Verde brilhante para vitória
        printf("    +===================================================+\n");
        printf("    |                                                   |\n");
        printf("    |  ##    ## ## ########  ######  ######  ## #####  |\n");
        printf("    |  ##    ## ##    ##    ##    ## ##   ## ####   ## |\n");
        printf("    |  ##    ## ##    ##    ##    ## ######  ## #####  |\n");
        printf("    |   ##  ##  ##    ##    ##    ## ##   ## ####   ## |\n");
        printf("    |    ####   ##    ##     ######  ##   ## ## #####  |\n");
        printf("    |                                                   |\n");
        printf("    |                  PARABENS!                        |\n");
        printf("    |             Voce venceu o jogo!                   |\n");
        printf("    |                                                   |\n");
        printf("    +===================================================+\n");
        printf("\x1b[0m"); // Reset cor
    } else {
        printf("\x1b[31;1m"); // Vermelho brilhante para game over
        printf("    +===============================================+\n");
        printf("    |                                               |\n");
        printf("    |        ######   #####  ###    ### #######    |\n");
        printf("    |       ##       ##   ## ####  #### ##         |\n");
        printf("    |       ##   ### ####### ## #### ## #####      |\n");
        printf("    |       ##    ## ##   ## ##  ##  ## ##         |\n");
        printf("    |        ######  ##   ## ##      ## #######    |\n");
        printf("    |                                               |\n");
        printf("    |         ######  ##    ## ####### ######      |\n");
        printf("    |        ##    ## ##    ## ##      ##   ##     |\n");
        printf("    |        ##    ## ##    ## #####   ######      |\n");
        printf("    |        ##    ##  ##  ##  ##      ##   ##     |\n");
        printf("    |         ######    ####   ####### ##   ##     |\n");
        printf("    |                                               |\n");
        printf("    +===============================================+\n");
        printf("\x1b[0m"); // Reset cor
    }
    
    printf("\n");
    printf("\x1b[36m"); // Ciano para estatísticas
    printf("    +---------------------------------------------+\n");
    printf("    |            ESTATISTICAS FINAIS              |\n");
    printf("    +---------------------------------------------+\n");
    printf("    | Pontuacao Final : \x1b[33;1m%-12d\x1b[36m        |\n", player->score);
    printf("    | Nivel Alcancado : \x1b[32;1m%-12d\x1b[36m        |\n", current_level);
    printf("    | Vidas Restantes : \x1b[31;1m%-12d\x1b[36m        |\n", player->lives);
    printf("    +---------------------------------------------+\n");
    printf("\x1b[0m"); // Reset cor
    printf("\n");
    printf("\x1b[33m    Pressione ENTER para sair...\x1b[0m\n");
    getchar();
}

// ===== FUNÇÕES DE LÓGICA DO JOGO =====

void update_game(Player* player, Maze* maze_data, Ghost* ghosts, int ghost_count, GameStatus* game_status, bool* game_over_flag, Position player_start_pos) {
    (void)player_start_pos; // Marca como usado para evitar warning
    
    logger_log_player_action("moveu para", player->pos.x, player->pos.y);

    if (check_collision_with_pacman(player, ghosts, ghost_count, player->pos)) {
        logger_collision_detected(player->pos.x, player->pos.y, ghosts[0].ghost_id);
        *game_status = GAME_OVER;
        *game_over_flag = true;
    }

    if (maze_count_points(maze_data) == 0) {
        *game_status = VICTORY;
        *game_over_flag = true;
    }
}

void draw_game(Player* player, Maze* maze_data, Ghost* ghosts, int ghost_count, GameStatus game_status) {
    clear_screen();
    
    printf("╔");
    for (int i = 0; i < maze_data->width; i++) printf("═");
    printf("╗\n");

    for (int y = 0; y < maze_data->height; y++) {
        printf("║");
        for (int x = 0; x < maze_data->width; x++) {
            Position current = {x, y};
            char symbol = maze_data->grid[y][x];

            bool is_ghost = false;
            for (int i = 0; i < ghost_count; i++) {
                if (positions_equal(ghosts[i].pos, current)) {
                    int color = COLOR_RESET;
                    switch (ghosts[i].symbol) {
                        case SYMBOL_GHOST_RED:   color = COLOR_GHOST_RED; break;
                        case SYMBOL_GHOST_GREEN: color = COLOR_GHOST_GREEN; break;
                        case SYMBOL_GHOST_BLUE:  color = COLOR_GHOST_BLUE; break;
                        case SYMBOL_GHOST_PINK:  color = COLOR_GHOST_PINK; break;
                        default: color = COLOR_RESET;
                    }
                    printf("\x1b[%dm%c\x1b[0m", color, ghosts[i].symbol);
                    is_ghost = true;
                    break;
                }
            }

            if (!is_ghost) {
                if (positions_equal(player->pos, current)) {
                    printf("\x1b[%dm%c\x1b[0m", COLOR_PLAYER, SYMBOL_PLAYER);
                } else {
                    switch (symbol) {
                        case SYMBOL_WALL: printf("\x1b[%dm█\x1b[0m", COLOR_WALL); break;
                        case SYMBOL_DOT: printf("\x1b[%dm·\x1b[0m", COLOR_DOT); break;
                        case SYMBOL_POWER_PELLET: printf("\x1b[%dmO\x1b[0m", COLOR_DOT); break;
                        default: printf("%c", SYMBOL_EMPTY_SPACE); break;
                    }
                }
            }
        }
        printf("║\n");
    }

    printf("╚");
    for (int i = 0; i < maze_data->width; i++) printf("═");
    printf("╝\n");
    
    printf("Score: %d | Vidas: %d | Status: %s\n", 
           player->score, player->lives, game_status_to_string(game_status));
}

// process_player_input lida com input do jogador e chama player_move.
bool process_player_input(Player* player, Maze* maze_data, Ghost* ghosts, int ghost_count, GameStatus* game_status, char input_char) {
    bool movement_attempted = false;
    
    switch (toupper(input_char)) {
        case KEY_UP:
        case KEY_LEFT:
        case KEY_DOWN:
        case KEY_RIGHT:
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
        case 'G': // Changed from 'D' to 'G' for debuG
        case 'L':
            handle_debug_command(input_char);
            break;
    }
    return movement_attempted;
}

void handle_debug_command(char input) {
    switch (tolower(input)) {
        case 'g':
            LOG_D("Comando de debug: Ghost info");
            break;
        case 'l':
            LOG_D("Comando de debug: Log Level");
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