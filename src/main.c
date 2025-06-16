#include "main.h"
#include "game.h"
#include "ghost.h"   
#include "maze.h"
#include "player.h"
#include "logger.h"
#include "stats.h"
#include "utils.h"
#include "queue.h"
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <stdbool.h>

static GameStatus game_status = PLAYING;
static int current_level = 1;
static const char* difficulty_names[] = {"Easy", "Medium", "Hard"};

static void initialize_level(int level_number, Maze* maze, Player* player,
                             Ghost ghosts[MAX_GHOSTS], Position* out_player_start_pos,
                             Position out_ghost_initial_positions[MAX_GHOSTS],
                             int* out_active_ghosts_from_map,
                             Queue* ghost_move_queue) {

    LOG_I("Inicializando nível %d...", level_number);

    maze_init(maze, level_number, out_player_start_pos, out_ghost_initial_positions, out_active_ghosts_from_map);

    int old_score = (player->lives > 0 || player->score > 0) ? player->score : 0;
    int old_lives = (player->lives > 0) ? player->lives : DEFAULT_LIVES;

    player_init(player, *out_player_start_pos);

    if (level_number > 1 || old_score > 0) {
        player->score = old_score;
    }
    if (level_number > 1 || old_lives != DEFAULT_LIVES) {
         player->lives = old_lives;
    }

    init_ghosts(ghosts, MAX_GHOSTS);

    for (int i = 0; i < MAX_GHOSTS; i++) {
        if (i < *out_active_ghosts_from_map) {
            ghosts[i].pos = out_ghost_initial_positions[i];
            ghosts[i].initial_pos = out_ghost_initial_positions[i];
            ghosts[i].is_active = true;
            ghosts[i].state = GHOST_NORMAL;
            LOG_I("Fantasma %d (%c) configurado para o nível %d em (%d,%d)",
                  ghosts[i].ghost_id, ghosts[i].symbol, level_number, ghosts[i].pos.x, ghosts[i].pos.y);
        } else {
            ghosts[i].is_active = false;
        }
    }

    clear_queue(ghost_move_queue);
    for (int i = 0; i < MAX_GHOSTS; i++) {
        if (ghosts[i].is_active) {
            if (!enqueue(ghost_move_queue, ghosts[i])) {
                LOG_E("Falha ao enfileirar fantasma %d para o nível %d. A fila pode estar cheia ou ocorreu um erro de alocação.", ghosts[i].ghost_id, level_number);
            }
        }
    }
    LOG_I("Nível %d inicializado. Jogador em (%d,%d). Fantasmas ativos: %d. Score: %d. Vidas: %d. Fila de fantasmas com %d elementos.",
          level_number, player->pos.x, player->pos.y, *out_active_ghosts_from_map, player->score, player->lives, queue_size(ghost_move_queue));
}

void game_loop() {
    logger_init("game.log", LOG_DEBUG);
    logger_game_started();

    GameStats* game_stats = create_game_stats();
    ProfileData* ai_profile = start_profiling("Ghost AI Performance");

    Player player = {0};
    Maze maze;
    Ghost ghosts[MAX_GHOSTS];

    Position player_start_pos_for_level;
    Position ghost_initial_positions_for_level[MAX_GHOSTS];
    int active_ghosts_for_level = 0;

    Queue* ghost_move_queue = create_queue();
    if (ghost_move_queue == NULL) {
        LOG_E("Falha crítica: Não foi possível criar a fila de fantasmas. Encerrando.");
        destroy_game_stats(game_stats);
        destroy_profile_data(ai_profile);
        logger_shutdown();
        exit(EXIT_FAILURE);
    }

    srand((unsigned)time(NULL));

    bool all_levels_completed = false;

    do {
        initialize_level(current_level, &maze, &player, ghosts,
                         &player_start_pos_for_level, ghost_initial_positions_for_level,
                         &active_ghosts_for_level,
                         ghost_move_queue);
        
        game_status = PLAYING;

        while (game_status == PLAYING) {

            if (game_status == PAUSED) {
                clear_screen();
                printf("\n\t ### JOGO PAUSADO ###\n");
                printf("\n\t Pressione '%c' para continuar.", KEY_PAUSE);
                printf("\n\t Pressione '%c' para sair.\n", KEY_QUIT);

                char pause_input = get_user_input();
                if (toupper(pause_input) == KEY_PAUSE) {
                    game_status = PLAYING;
                    LOG_I("Jogo retomado.");
                } else if (toupper(pause_input) == KEY_QUIT) {
                    game_status = GAME_OVER;
                    LOG_I("Jogo encerrado pelo jogador durante a pausa.");
                }
                if (game_status != PLAYING) continue;
            }

            draw_game(&player, &maze, ghosts, active_ghosts_for_level, game_status);
            printf("Nível: %d | Comandos: WASD/Setas, P (Pausar), Q (Sair)\n", current_level);

            char input_char = get_user_input();
            if (input_char == '\0' && game_status == PLAYING) {
            } else if (input_char != '\0') {
                 process_player_input(&player, &maze, ghosts, active_ghosts_for_level, &game_status, input_char);
            }
            
            if (game_status != PLAYING) {
                continue;
            }

            if (!is_empty(ghost_move_queue)) {
                Ghost current_ghost_copy = dequeue(ghost_move_queue);

                if (current_ghost_copy.ghost_id != -1) {
                    Ghost* original_ghost = NULL;
                    for (int i = 0; i < MAX_GHOSTS; ++i) {
                        if (ghosts[i].ghost_id == current_ghost_copy.ghost_id) {
                            original_ghost = &ghosts[i];
                            break;
                        }
                    }

                    if (original_ghost) {
                        original_ghost->timer++;
                        update_ghost_state(original_ghost, original_ghost->timer);

                        if (original_ghost->is_active || original_ghost->state == GHOST_EATEN) {
                            if (original_ghost->is_active || (original_ghost->state == GHOST_EATEN && !positions_equal(original_ghost->pos, original_ghost->initial_pos))) {
                                Direction new_dir = calculate_next_direction(original_ghost, player.pos, &maze);
                                if (new_dir != DIR_INVALID) {
                                    Position next_pos = get_next_position(original_ghost->pos, new_dir);
                                    if (is_valid_move_ghost(next_pos, &maze)) {
                                        original_ghost->pos = next_pos;
                                    }
                                    original_ghost->direction = new_dir;
                                }
                                if (original_ghost->state == GHOST_EATEN && positions_equal(original_ghost->pos, original_ghost->initial_pos)) {
                                    LOG_I("Fantasma %d (%c) chegou à base e foi resetado.", original_ghost->ghost_id, original_ghost->symbol);
                                    reset_ghost(original_ghost);
                                }
                            }
                            bool should_reenqueue = false;
                            if (original_ghost->is_active) {
                                should_reenqueue = true;
                            } else if (original_ghost->state == GHOST_EATEN && !original_ghost->is_active) {
                                should_reenqueue = true;
                            }

                            if (should_reenqueue) {
                                if (!enqueue(ghost_move_queue, *original_ghost)) {
                                    LOG_E("Falha ao re-enfileirar fantasma %d. Pode ser perdido no próximo ciclo.", original_ghost->ghost_id);
                                }
                            } else {
                                LOG_D("Fantasma %d (%c) não foi re-enfileirado (estado: %d, ativo: %d)",
                                      original_ghost->ghost_id, original_ghost->symbol, original_ghost->state, original_ghost->is_active);
                            }
                        }
                    }
                }
            }

            bool level_is_over_flag = false;
            update_game(&player, &maze, ghosts, active_ghosts_for_level, &game_status, &level_is_over_flag, player_start_pos_for_level);

            if(game_status == PLAYING) {
                sleep_ms(GAME_SPEED_MS);
            }
        }

        if (game_status == VICTORY) {
            LOG_I("Nível %d completo! Score atual: %d.", current_level, player.score);

            current_level++;
            if (current_level > MAX_LEVELS) {
                LOG_I("Parabéns! Todos os %d níveis foram completados!", MAX_LEVELS);
                all_levels_completed = true;
            } else {
                LOG_I("Preparando próximo nível: %d.", current_level);
            }
        }

    } while (game_status == VICTORY && !all_levels_completed);

    end_profiling(ai_profile);
    logger_game_ended(player.score, (all_levels_completed ? current_level -1 : current_level) );
    
    destroy_game_stats(game_stats);
    destroy_profile_data(ai_profile);
    destroy_queue(ghost_move_queue);
    logger_shutdown();

    clear_screen();
    printf("\n\n");
    if (game_status == VICTORY && all_levels_completed) {
        printf("    ******************************************\n");
        printf("    *  🎉 P A R A B É N S, VOCÊ VENCEU! 🎉  *\n");
        printf("    * Você completou todos os %d níveis! *\n", MAX_LEVELS);
        printf("    ******************************************\n");
    } else if (game_status == VICTORY) {
        printf("    ******************************\n");
        printf("    *       🎉 VITÓRIA! 🎉      *\n");
        printf("    *   Nível %d completado!    *\n", all_levels_completed ? MAX_LEVELS : current_level -1 );
        printf("    ******************************\n");
    } else {
        printf("    ******************************\n");
        printf("    *      💀 GAME OVER 💀      *\n");
        printf("    ******************************\n");
    }
    printf("\n");
    printf("    Nível Final Alcançado: %d\n", (all_levels_completed ? MAX_LEVELS : (game_status == VICTORY ? current_level -1 : current_level) ));
    printf("    Pontuação Final: %d\n", player.score);
    printf("\n\n    Pressione ENTER para sair...\n");

    int c;
    while ((c = getchar()) != '\n' && c != EOF);
    getchar();
}

int main(void) {
    show_title_screen();
    game_loop();
    
    printf("\n\x1b[33;1m*** Obrigado por jogar %s! ***\x1b[0m\n", GAME_TITLE);
    return 0;
}