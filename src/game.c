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
    printf("\n\n");
    
    // Título principal - simples e compatível com CMD
    printf("    ================================================================\n");
    printf("    |                                                              |\n");
    printf("    |   ####    ##   ####        ###   ###   ##   ##  ##          |\n");
    printf("    |   ##  ##  ##  ##           ## ## ## ##  ### ###  ###         |\n");
    printf("    |   ####    ##  ##           ##  ###  ##  #######  ####        |\n");
    printf("    |   ##      ##  ##           ##   #   ##  ## # ##  ## ##       |\n");
    printf("    |   ##      ##   ####        ##       ##  ##   ##  ##  ##      |\n");
    printf("    |                                                              |\n");
    printf("    |                 TERMINAL EDITION - v1.0                     |\n");
    printf("    |                                                              |\n");
    printf("    ================================================================\n\n");
    
    // Arte ASCII simples do Pac-Man
    printf("                            .-----.\n");
    printf("                           /       \\\n");
    printf("                          |  O     O |\n");
    printf("                          |     >    |\n");
    printf("                          \\   ---   /\n");
    printf("                           '-------'\n\n");
    
    // Menu de opções sem caracteres especiais
    printf("    +---------------------------------------------------------+\n");
    printf("    |                    MENU PRINCIPAL                      |\n");
    printf("    +---------------------------------------------------------+\n");
    printf("    |                                                         |\n");
    printf("    |  [ENTER] - Iniciar Jogo                                |\n");
    printf("    |  [I]     - Instrucoes                                  |\n");
    printf("    |  [Q]     - Sair do Jogo                               |\n");
    printf("    |                                                         |\n");
    printf("    +---------------------------------------------------------+\n\n");
      printf("    Escolha uma opcao: ");
}

void show_game_over_screen(Player* player, GameStatus game_status, int current_level) {
    clear_screen();
    printf("\n\n");
    
    if (game_status == VICTORY) {
        // Tela de vitória sem caracteres especiais
        printf("    ================================================================\n");
        printf("    |                                                              |\n");
        printf("    |    V   V  I  T T T  O O O  R R R  I   A A A    !!!          |\n");
        printf("    |    V   V  I    T    O   O  R   R  I   A   A    !!!          |\n");
        printf("    |    V   V  I    T    O   O  R R R  I   A A A    !!!          |\n");
        printf("    |     V V   I    T    O   O  R   R  I   A   A                 |\n");
        printf("    |      V    I    T    O O O  R   R  I   A   A    !!!          |\n");
        printf("    |                                                              |\n");
        printf("    |                      PARABENS!                              |\n");
        printf("    |                  Voce venceu o jogo!                        |\n");
        printf("    |                                                              |\n");
        printf("    ================================================================\n");
    } else {
        // Tela de game over sem caracteres especiais
        printf("    ================================================================\n");
        printf("    |                                                              |\n");
        printf("    |     G G G  A A A  M   M  E E E    O O O  V   V  E E E  R R R |\n");
        printf("    |     G      A   A  M M M  E        O   O  V   V  E      R   R |\n");
        printf("    |     G  GG  A A A  M   M  E E E    O   O  V   V  E E E  R R R |\n");
        printf("    |     G   G  A   A  M   M  E        O   O   V V   E      R   R |\n");
        printf("    |     G G G  A   A  M   M  E E E    O O O    V    E E E  R   R |\n");
        printf("    |                                                              |\n");
        printf("    |                      FIM DE JOGO                            |\n");
        printf("    |                                                              |\n");
        printf("    ================================================================\n");
    }
    
    printf("\n");
    printf("    +-------------------------------------------------------+\n");
    printf("    |                   ESTATISTICAS FINAIS                |\n");
    printf("    +-------------------------------------------------------+\n");
    printf("    | Nivel Final Alcancado: %-26d |\n", current_level);
    printf("    | Pontuacao Final      : %-26d |\n", player->score);
    printf("    | Vidas Restantes      : %-26d |\n", player->lives);
    printf("    +-------------------------------------------------------+\n");
    printf("\n\n");
    printf("    Pressione ENTER para sair...\n");
}

// ===== FUNÇÕES DE LÓGICA DO JOGO =====

void update_game(Player* player, Maze* maze_data, Ghost* ghosts, int ghost_count, GameStatus* game_status, bool* game_over_flag, Position player_start_pos) {
    (void)ghost_count; // Não usado mais, verificamos todos os fantasmas
    
    logger_log_player_action("moveu para", player->pos.x, player->pos.y);

    // Verificar colisão com todos os fantasmas ativos
    for (int i = 0; i < MAX_GHOSTS; i++) {
        if (ghosts[i].is_active && positions_equal(ghosts[i].pos, player->pos)) {
            logger_collision_detected(player->pos.x, player->pos.y, ghosts[i].ghost_id);
            
            // Se o fantasma está assustado, ele é comido
            if (ghosts[i].state == GHOST_FRIGHTENED) {
                ghosts[i].state = GHOST_EATEN;
                player->score += POINTS_PER_GHOST_EATEN;
                LOG_I("Fantasma %d comido! +%d pontos", ghosts[i].ghost_id, POINTS_PER_GHOST_EATEN);
                return;
            }
            
            // Caso contrário, jogador perde uma vida
            player_lose_life(player, player_start_pos);
            
            if (player->lives <= 0) {
                *game_status = GAME_OVER;
                *game_over_flag = true;
                LOG_I("Game Over - Sem vidas restantes");
            } else {
                LOG_I("Vida perdida! Vidas restantes: %d", player->lives);
                // Resetar posições dos fantasmas após perder vida
                for (int j = 0; j < MAX_GHOSTS; j++) {
                    if (ghosts[j].is_active) {
                        ghosts[j].pos = ghosts[j].initial_pos;
                        ghosts[j].state = GHOST_NORMAL;
                    }
                }
            }
            return;
        }
    }

    if (maze_count_points(maze_data) == 0) {
        *game_status = VICTORY;
        *game_over_flag = true;
    }
}

void draw_game(Player* player, Maze* maze_data, Ghost* ghosts, int ghost_count, GameStatus game_status) {
    clear_screen();
    
    // Header com informações do jogo usando caracteres ASCII simples
    printf("    +");
    for (int i = 0; i < maze_data->width; i++) printf("-");
    printf("+\n");
      // Linha de status
    char status_buffer[200];
    snprintf(status_buffer, sizeof(status_buffer), " Score: %d | Vidas: %d | Status: %s", 
             player->score, player->lives, game_status_to_string(game_status));
    
    printf("    |%s", status_buffer);
    
    // Preencher o resto da linha até a largura do maze
    int current_len = strlen(status_buffer);
    for (int i = current_len; i < maze_data->width; i++) printf(" ");
    printf("|\n");
    
    printf("    +");
    for (int i = 0; i < maze_data->width; i++) printf("-");
    printf("+\n");

    // Renderizar o labirinto
    for (int y = 0; y < maze_data->height; y++) {
        printf("    |");
        for (int x = 0; x < maze_data->width; x++) {
            Position current = {x, y};
            char symbol = maze_data->grid[y][x];            // Verificar se há um fantasma nesta posição
            bool is_ghost = false;
            for (int i = 0; i < MAX_GHOSTS; i++) {
                if (ghosts[i].is_active && positions_equal(ghosts[i].pos, current)) {
                    // Renderizar fantasma com cor
                    switch (ghosts[i].state) {
                        case GHOST_FRIGHTENED:
                            printf("%s%c%s", GHOST_FRIGHTENED_COLOR, SYMBOL_GHOST_FRIGHTENED, COLOR_RESET);
                            break;
                        case GHOST_EATEN:
                            printf("%s%c%s", GHOST_EATEN_COLOR, SYMBOL_GHOST_EATEN, COLOR_RESET);
                            break;
                        default:
                            // Escolher cor baseada no símbolo/ID do fantasma
                            switch (ghosts[i].ghost_id % 4) {
                                case 0:
                                    printf("%s%c%s", GHOST_RED_COLOR, ghosts[i].symbol, COLOR_RESET);
                                    break;
                                case 1:
                                    printf("%s%c%s", GHOST_GREEN_COLOR, ghosts[i].symbol, COLOR_RESET);
                                    break;
                                case 2:
                                    printf("%s%c%s", GHOST_BLUE_COLOR, ghosts[i].symbol, COLOR_RESET);
                                    break;
                                case 3:
                                    printf("%s%c%s", GHOST_PINK_COLOR, ghosts[i].symbol, COLOR_RESET);
                                    break;
                            }
                            break;
                    }
                    is_ghost = true;
                    break;
                }
            }

            if (!is_ghost) {
                // Verificar se é a posição do jogador
                if (positions_equal(player->pos, current)) {
                    printf("%s%c%s", PLAYER_COLOR, SYMBOL_PLAYER, COLOR_RESET);
                } else {
                    // Renderizar elementos do labirinto com cores
                    switch (symbol) {
                        case SYMBOL_WALL: 
                            printf("%s%c%s", WALL_COLOR, SYMBOL_WALL, COLOR_RESET);
                            break;
                        case SYMBOL_DOT: 
                            printf("%s%c%s", DOT_COLOR, SYMBOL_DOT, COLOR_RESET);
                            break;
                        case SYMBOL_POWER_PELLET: 
                            printf("%s%c%s", POWER_PELLET_COLOR, SYMBOL_POWER_PELLET, COLOR_RESET);
                            break;
                        default: 
                            printf(" ");
                            break;
                    }
                }
            }
        }
        printf("|\n");
    }    // Footer
    printf("    +");
    for (int i = 0; i < maze_data->width; i++) printf("-");
    printf("+\n");
    
    // Legenda colorida
    printf("\n    LEGENDA: ");
    printf("%s%c%s Pacman | ", PLAYER_COLOR, SYMBOL_PLAYER, COLOR_RESET);
    printf("%s%c%s Parede | ", WALL_COLOR, SYMBOL_WALL, COLOR_RESET);
    printf("%s%c%s Pontos | ", DOT_COLOR, SYMBOL_DOT, COLOR_RESET);
    printf("%s%c%s Power-up", POWER_PELLET_COLOR, SYMBOL_POWER_PELLET, COLOR_RESET);
    printf("\n    FANTASMAS: ");
    printf("%s%c%s Vermelho | ", GHOST_RED_COLOR, SYMBOL_GHOST_RED, COLOR_RESET);
    printf("%s%c%s Verde | ", GHOST_GREEN_COLOR, SYMBOL_GHOST_GREEN, COLOR_RESET);
    printf("%s%c%s Azul | ", GHOST_BLUE_COLOR, SYMBOL_GHOST_BLUE, COLOR_RESET);
    printf("%s%c%s Rosa", GHOST_PINK_COLOR, SYMBOL_GHOST_PINK, COLOR_RESET);
    printf("\n    WASD - Mover | P - Pausar | Q - Sair\n");
}

// process_player_input lida com input do jogador e chama player_move.
void process_player_input(Player* player, Maze* maze_data, Ghost* ghosts, int ghost_count, GameStatus* game_status, char input_char) {
    
    switch (toupper(input_char)) {        case KEY_UP:
        case KEY_LEFT:
        case KEY_DOWN:
        case KEY_RIGHT:
            player_move(player, maze_data, input_char, ghosts, ghost_count);
            
            // Mover fantasmas após o movimento do jogador
            for (int i = 0; i < MAX_GHOSTS; i++) {
                if (ghosts[i].is_active) {
                    Direction new_dir = calculate_next_direction(&ghosts[i], player->pos, maze_data);
                    if (new_dir != DIR_INVALID) {
                        Position next_pos = get_next_position(ghosts[i].pos, new_dir);
                        if (is_valid_move_ghost(next_pos, maze_data)) {
                            ghosts[i].pos = next_pos;
                            ghosts[i].direction = new_dir;
                        }
                    }
                    
                    // Atualizar estado do fantasma
                    ghosts[i].timer++;
                    update_ghost_state(&ghosts[i], ghosts[i].timer);
                }            }
            
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