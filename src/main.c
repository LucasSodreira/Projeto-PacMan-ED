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
#include "config.h"

static GameStatus game_status = PLAYING;
static int current_level = 1;
static int ghost_move_counter = 0;  // Contador simples para movimento dos fantasmas
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
    }    // Não precisamos mais da fila para os fantasmas, mantemos só para compatibilidade
    // clear_queue(ghost_move_queue);
    
    LOG_I("Nível %d inicializado. Jogador em (%d,%d). Fantasmas ativos: %d. Score: %d. Vidas: %d.",
          level_number, player->pos.x, player->pos.y, *out_active_ghosts_from_map, player->score, player->lives);
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
    int exit_code = 0;

    do {
        initialize_level(current_level, &maze, &player, ghosts,
                         &player_start_pos_for_level, ghost_initial_positions_for_level,
                         &active_ghosts_for_level,
                         ghost_move_queue);
        game_status = PLAYING;
        while (game_status == PLAYING || game_status == PAUSED) {

            if (game_status == PAUSED) {
                clear_screen();
                printf("\n");
                printf("    ===============================================\n");
                printf("    |                                             |\n");
                printf("    |           ### JOGO PAUSADO ###             |\n");
                printf("    |                                             |\n");
                printf("    |  Pressione '%c' para continuar              |\n", KEY_PAUSE);
                printf("    |  Pressione '%c' para sair                   |\n", KEY_QUIT);
                printf("    |                                             |\n");
                printf("    ===============================================\n");
                printf("\n    Opcao: ");

                char pause_input = get_user_input();
                if (toupper(pause_input) == KEY_PAUSE) {
                    game_status = PLAYING;
                    LOG_I("Jogo retomado.");
                } else if (toupper(pause_input) == KEY_QUIT) {
                    game_status = GAME_OVER;
                    LOG_I("Jogo encerrado pelo jogador durante a pausa.");
                }                continue;
            }            // Processar input de forma não-bloqueante no Windows
            #ifdef _WIN32
                if (_kbhit()) {  // Verifica se há tecla pressionada
                    char input_char = _getch();
                    process_player_input(&player, &maze, ghosts, active_ghosts_for_level, &game_status, input_char);
                    
                    if (game_status != PLAYING) {
                        continue;
                    }

                    bool level_is_over_flag = false;
                    update_game(&player, &maze, ghosts, active_ghosts_for_level, &game_status, &level_is_over_flag, player_start_pos_for_level);

                    draw_game(&player, &maze, ghosts, active_ghosts_for_level, game_status, current_level);
                    printf("Nivel: %d | Comandos: WASD/Setas, P (Pausar), Q (Sair)\n", current_level);
                }
            #else
                char input_char = get_user_input();
                if (input_char != '\0') {
                    process_player_input(&player, &maze, ghosts, active_ghosts_for_level, &game_status, input_char);
                    
                    if (game_status != PLAYING) {
                        continue;
                    }

                    bool level_is_over_flag = false;
                    update_game(&player, &maze, ghosts, active_ghosts_for_level, &game_status, &level_is_over_flag, player_start_pos_for_level);

                    draw_game(&player, &maze, ghosts, active_ghosts_for_level, game_status, current_level);
                    printf("Nivel: %d | Comandos: WASD/Setas, P (Pausar), Q (Sair)\n", current_level);
                }
            #endif

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

    // Bloco de limpeza de recursos
cleanup:
    end_profiling(ai_profile);
    logger_game_ended(player.score, (all_levels_completed ? current_level -1 : current_level) );
    destroy_game_stats(game_stats);
    destroy_profile_data(ai_profile);
    destroy_queue(ghost_move_queue);
    logger_shutdown();
    clear_screen();
    printf("\n\n");
    
    if (game_status == VICTORY && all_levels_completed) {
        printf("\033[1;32m    ================================================================\033[0m\n");
        printf("\033[1;32m    |                                                              |\033[0m\n");
        printf("\033[1;32m    |       ");
        printf("\033[33mP A R A B E N S ,   V O C E   V E N C E U !\033[0m");
        printf("\033[1;32m        |\033[0m\n");
        printf("\033[1;32m    |                                                              |\033[0m\n");
        printf("\033[1;32m    |           ");
        printf("\x1b[35mVoce completou todos os %d niveis!\x1b[0m", MAX_LEVELS);
        printf("\033[1;32m            |\033[0m\n");
        printf("\033[1;32m    |                                                              |\033[0m\n");
        printf("\033[1;32m    ================================================================\033[0m\n");
    } else if (game_status == VICTORY) {
        printf("\033[1;32m    ================================================\033[0m\n");
        printf("\033[1;32m    |                                              |\033[0m\n");
        printf("\033[1;32m    |            ");
        printf("\033[33mV I T O R I A !\033[0m");
        printf("\033[1;32m            |\033[0m\n");
        printf("\033[1;32m    |                                              |\033[0m\n");
        printf("\033[1;32m    |      ");
        printf("\x1b[35mNivel %d completado!\x1b[0m", all_levels_completed ? MAX_LEVELS : current_level -1);
        printf("\033[1;32m       |\033[0m\n");
        printf("\033[1;32m    |                                              |\033[0m\n");
        printf("\033[1;32m    ================================================\033[0m\n");
    } else {
        printf("\033[1;31m    ================================================\033[0m\n");
        printf("\033[1;31m    |                                              |\033[0m\n");
        printf("\033[1;31m    |             ");
        printf("\033[33mG A M E   O V E R\033[0m");
        printf("\033[1;31m            |\033[0m\n");
        printf("\033[1;31m    |                                              |\033[0m\n");
        printf("\033[1;31m    |          ");
        printf("\x1b[36mTente novamente na proxima vez!\x1b[0m");
        printf("\033[1;31m       |\033[0m\n");
        printf("\033[1;31m    |                                              |\033[0m\n");
        printf("\033[1;31m    ================================================\033[0m\n");    }
    
    printf("\n");
    printf("    +----------------------------------------------------+\n");
    printf("    |                RESULTADO FINAL                     |\n");
    printf("    +----------------------------------------------------+\n");
    printf("    | Nivel Final Alcancado : %-22d |\n", 
           (all_levels_completed ? MAX_LEVELS : (game_status == VICTORY ? current_level -1 : current_level)));
    printf("    | Pontuacao Final       : %-22d |\n", player.score);
    printf("    +----------------------------------------------------+\n");
    
    printf("\n");
    printf("    Pressione ENTER para sair...");
    printf("\n");

    int c;
    while ((c = getchar()) != '\n' && c != EOF);
    getchar();
}

int main(void) {
    setup_console();  // Configura o console para melhor compatibilidade
    
    // Loop do menu principal
    char menu_choice;
    bool should_exit = false;
    
    while (!should_exit) {
        show_title_screen();
        menu_choice = get_user_input();
        
        switch (menu_choice) {
            case '\r':  // Enter key
            case '\n':  // Enter key (algumas variações)
            case ' ':   // Espaço como alternativa
                game_loop();
                should_exit = true;
                break;
                
            case 'I':
            case 'i':
                print_instructions();
                printf("\n    Pressione qualquer tecla para voltar ao menu...");
                get_user_input();
                break;
                
            case 'Q':
            case 'q':
                should_exit = true;
                break;
                
            default:
                // Opção inválida - continua no loop mostrando o menu novamente
                break;
        }
    }
    
    printf("\n");
    printf("*** Obrigado por jogar %s! ***\n", GAME_TITLE);
    printf("\n");
    return 0;
}