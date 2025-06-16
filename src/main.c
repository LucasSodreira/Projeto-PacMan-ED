#include "main.h"
#include "game.h"
#include "ghost.h"
#include "queue.h"
#include "logger.h"
#include "stats.h"
#include "maze.h"
#include "player.h"
#include "utils.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <ctype.h>

// Game state variables
static GameStatus game_status = PLAYING; // Controla o estado geral do jogo (PLAYING, PAUSED, GAME_OVER, VICTORY)
static int current_level = 1;            // Nível inicial
// static int score = 0; // Removido, player->score é usado
static const char* difficulty_names[] = {"Easy", "Medium", "Hard"}; // Mantido se usado em outro lugar

// Protótipo para get_user_input (deve ser fornecida ou implementada em utils.c ou similar)
// char get_user_input();
// Implementação stub simples para compilar:
char get_user_input() {
    // printf("Input: "); // Descomente para debug de input
    char buf[10];
    if (fgets(buf, sizeof(buf), stdin) != NULL) {
        if (buf[0] != '\n' && buf[0] != '\0') {
            return toupper(buf[0]);
        }
    }
    return '\0';
}


// Nova função para inicializar cada nível
static void initialize_level(int level_number, Maze* maze, Player* player,
                             Ghost ghosts[MAX_GHOSTS], Position* out_player_start_pos,
                             Position out_ghost_initial_positions[MAX_GHOSTS],
                             int* out_active_ghosts_from_map,
                             Queue* ghost_move_queue) { // Parâmetro da fila adicionado

    LOG_I("Inicializando nível %d...", level_number);

    maze_init(maze, level_number, out_player_start_pos, out_ghost_initial_positions, out_active_ghosts_from_map);

    // Salva score e vidas se player já foi inicializado (para níveis > 1)
    int old_score = (player->lives > 0 || player->score > 0) ? player->score : 0;
    int old_lives = (player->lives > 0) ? player->lives : DEFAULT_LIVES;

    player_init(player, *out_player_start_pos);

    if (level_number > 1 || old_score > 0) {
        player->score = old_score;
    }
    if (level_number > 1 || old_lives != DEFAULT_LIVES) {
         player->lives = old_lives;
    }

    init_ghosts(ghosts, MAX_GHOSTS); // Define propriedades padrão, is_active = false

    // Configura fantasmas com base nos dados do mapa e define initial_pos
    for (int i = 0; i < MAX_GHOSTS; i++) {
        if (i < *out_active_ghosts_from_map) {
            ghosts[i].pos = out_ghost_initial_positions[i];
            ghosts[i].initial_pos = out_ghost_initial_positions[i];
            ghosts[i].is_active = true; // Ativa apenas os fantasmas do mapa
            ghosts[i].state = GHOST_NORMAL;
            LOG_I("Fantasma %d (%c) configurado para o nível %d em (%d,%d)",
                  ghosts[i].ghost_id, ghosts[i].symbol, level_number, ghosts[i].pos.x, ghosts[i].pos.y);
        } else {
            ghosts[i].is_active = false; // Garante que fantasmas extras estejam inativos
        }
    }

    clear_queue(ghost_move_queue); // Limpa a fila para o novo nível
    // Enfileira apenas os fantasmas que foram ativados
    for (int i = 0; i < MAX_GHOSTS; i++) {
        if (ghosts[i].is_active) {
            if (!enqueue(ghost_move_queue, ghosts[i])) {
                LOG_E("Falha ao enfileirar fantasma %d para o nível %d. A fila pode estar cheia ou ocorreu um erro de alocação.", ghosts[i].ghost_id, level_number);
                // Dependendo da criticidade, pode-se optar por encerrar o jogo ou continuar com menos fantasmas.
                // Por ora, apenas loga o erro.
            }
        }
    }
    LOG_I("Nível %d inicializado. Jogador em (%d,%d). Fantasmas ativos: %d. Score: %d. Vidas: %d. Fila de fantasmas com %d elementos.",
          level_number, player->pos.x, player->pos.y, *out_active_ghosts_from_map, player->score, player->lives, queue_size(ghost_move_queue));
}


void game_loop() {
    // System initialization
    logger_init("game.log", LOG_DEBUG);
    logger_game_started();

    // QueueStats* queue_stats = create_queue_stats(); // Removido
    GameStats* game_stats = create_game_stats();
    ProfileData* ai_profile = start_profiling("Ghost AI Performance");

    Player player = {0};
    Maze maze;
    Ghost ghosts[MAX_GHOSTS]; // Array original dos fantasmas

    Position player_start_pos_for_level;
    Position ghost_initial_positions_for_level[MAX_GHOSTS];
    int active_ghosts_for_level = 0;

    Queue* ghost_move_queue = create_queue();
    if (ghost_move_queue == NULL) {
        LOG_E("Falha crítica: Não foi possível criar a fila de fantasmas. Encerrando.");
        // Limpeza de outros recursos alocados antes de create_queue, se houver.
        destroy_game_stats(game_stats);
        destroy_profile_data(ai_profile);
        logger_shutdown();
        // Idealmente, exibir uma mensagem para o usuário antes de exit()
        // printf("Erro crítico de memória. Não é possível iniciar o jogo.\n");
        exit(EXIT_FAILURE); // Encerra o programa se a fila não puder ser criada.
    }

    srand((unsigned)time(NULL));

    bool all_levels_completed = false;

    do {
        // Inicializa o nível atual, incluindo o enfileiramento dos fantasmas
        initialize_level(current_level, &maze, &player, ghosts,
                         &player_start_pos_for_level, ghost_initial_positions_for_level,
                         &active_ghosts_for_level,
                         ghost_move_queue);
        
        game_status = PLAYING;

        // Loop de jogabilidade para o nível atual
        while (game_status == PLAYING) {
            // Lógica de Pausa
            // Esta lógica de pausa precisa ser acessada mesmo se o estado principal for PLAYING,
            // pois process_player_input pode mudar game_status para PAUSED.
            // Uma forma mais limpa seria ter o loop de pausa dentro de process_player_input ou um estado separado.
            // Por ora, se process_player_input mudar para PAUSED, este loop while termina e o do..while o reinicia.
            // Vamos ajustar para que a pausa seja tratada dentro do loop PLAYING.

            if (game_status == PAUSED) { // Checa se o estado mudou para PAUSED
                clear_screen();
                printf("\n\t ### JOGO PAUSADO ###\n");
                printf("\n\t Pressione '%c' para continuar.", KEY_PAUSE);
                printf("\n\t Pressione '%c' para sair.\n", KEY_QUIT);

                char pause_input = get_user_input();
                if (toupper(pause_input) == KEY_PAUSE) {
                    game_status = PLAYING;
                    LOG_I("Jogo retomado.");
                } else if (toupper(pause_input) == KEY_QUIT) {
                    game_status = GAME_OVER; // Define GAME_OVER para sair de todos os loops
                    LOG_I("Jogo encerrado pelo jogador durante a pausa.");
                }
                if (game_status != PLAYING) continue; // Se saiu ou ainda está em pausa (se get_user_input for complexo)
            }


            draw_game(&player, &maze, ghosts, active_ghosts_for_level, game_status);
            printf("Nível: %d | Comandos: WASD/Setas, P (Pausar), Q (Sair)\n", current_level);

            char input_char = get_user_input();
            if (input_char == '\0' && game_status == PLAYING) {
                 // Se não houver input, apenas continue o loop do jogo (fantasmas se movem, etc.)
                 // sleep_ms(10); // Pequena pausa opcional para reduzir uso de CPU se get_user_input for não bloqueante
            } else if (input_char != '\0') { // Só processa se houver input
                 // process_player_input pode mudar game_status (ex: para GAME_OVER via 'Q', ou PAUSED via 'P')
                 process_player_input(&player, &maze, ghosts, active_ghosts_for_level, &game_status, input_char);
            }
            
            // Se o jogo foi pausado ou encerrado pelo input, não atualiza a lógica de jogo
            if (game_status != PLAYING) {
                continue;
            }

            // Nova lógica de movimento de fantasmas usando a Fila
            if (!is_empty(ghost_move_queue)) {
                Ghost current_ghost_copy = dequeue(ghost_move_queue);

                if (current_ghost_copy.ghost_id != -1) { // Verifica dequeue válido
                    Ghost* original_ghost = NULL;
                    for (int i = 0; i < MAX_GHOSTS; ++i) { // Encontra fantasma original no array
                        if (ghosts[i].ghost_id == current_ghost_copy.ghost_id) {
                            original_ghost = &ghosts[i];
                            break;
                        }
                    }

                    if (original_ghost) {
                        // Incrementa o timer do fantasma original ANTES de chamar update_ghost_state
                        original_ghost->timer++;
                        update_ghost_state(original_ghost, original_ghost->timer);

                        // Processa movimento e lógica de reativação
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
                                // Se um fantasma comido chega à sua posição inicial, ele é resetado por reset_ghost.
                                if (original_ghost->state == GHOST_EATEN && positions_equal(original_ghost->pos, original_ghost->initial_pos)) {
                                    LOG_I("Fantasma %d (%c) chegou à base e foi resetado.", original_ghost->ghost_id, original_ghost->symbol);
                                    reset_ghost(original_ghost); // Torna GHOST_NORMAL e is_active = true
                                }
                            }
                            // Re-enfileirar a versão ATUALIZADA se ainda deve se mover ou ser processado
                            bool should_reenqueue = false;
                            if (original_ghost->is_active) { // Se está ativo (NORMAL, FRIGHTENED, ou reativado de EATEN)
                                should_reenqueue = true;
                            } else if (original_ghost->state == GHOST_EATEN && !original_ghost->is_active) {
                                // Se está comido e ainda não chegou na base (is_active se tornaria true via reset_ghost)
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
        } // Fim do loop while (game_status == PLAYING)

        // Lógica de fim de nível
        if (game_status == VICTORY) {
            LOG_I("Nível %d completo! Score atual: %d.", current_level, player.score);
            // Adicionar bônus por completar o nível, se houver
            // player.score += BONUS_LEVEL_COMPLETE;
            // LOG_I("Bônus de %d aplicado! Novo score: %d", BONUS_LEVEL_COMPLETE, player.score);

            current_level++;
            if (current_level > MAX_LEVELS) {
                LOG_I("Parabéns! Todos os %d níveis foram completados!", MAX_LEVELS);
                all_levels_completed = true;
                // game_status já é VICTORY, o loop do...while terminará.
            } else {
                LOG_I("Preparando próximo nível: %d.", current_level);
                // O loop do...while continuará, e initialize_level será chamado para o novo current_level.
                // Score e vidas são mantidos. O estado do jogo será PLAYING no início do próximo loop.
            }
        }
        // Se game_status for GAME_OVER (por perda de vidas ou 'Q'), o loop do...while terminará.

    } while (game_status == VICTORY && !all_levels_completed); // Continua se o último nível foi uma vitória e nem todos foram completados

    // Fim do jogo (Game Over definitivo ou Vitória em todos os níveis)
    end_profiling(ai_profile);
    logger_game_ended(player.score, (all_levels_completed ? current_level -1 : current_level) ); // Loga o último nível completado ou atual
    
    // Cleanup
    // destroy_queue_stats(queue_stats); // Removido
    destroy_game_stats(game_stats);
    destroy_profile_data(ai_profile);
    destroy_queue(ghost_move_queue); // Destroi a fila
    logger_shutdown();

    // Tela final
    clear_screen();
    printf("\n\n");
    if (game_status == VICTORY && all_levels_completed) {
        printf("    ******************************************\n");
        printf("    *  🎉 P A R A B É N S, VOCÊ VENCEU! 🎉  *\n");
        printf("    * Você completou todos os %d níveis! *\n", MAX_LEVELS);
        printf("    ******************************************\n");
    } else if (game_status == VICTORY) { // Venceu o nível, mas saiu/algo deu errado (ex: quit após vitória antes de todos os níveis)
                                         // Ou se MAX_LEVELS for atingido e o loop principal sair por game_status == VICTORY
        printf("    ******************************\n");
        printf("    *       🎉 VITÓRIA! 🎉      *\n");
        printf("    *   Nível %d completado!    *\n", all_levels_completed ? MAX_LEVELS : current_level -1 );
        printf("    ******************************\n");
    } else { // GAME_OVER
        printf("    ******************************\n");
        printf("    *      💀 GAME OVER 💀      *\n");
        printf("    ******************************\n");
    }
    printf("\n");
    printf("    Nível Final Alcançado: %d\n", (all_levels_completed ? MAX_LEVELS : (game_status == VICTORY ? current_level -1 : current_level) ));
    printf("    Pontuação Final: %d\n", player.score);
    printf("\n\n    Pressione ENTER para sair...\n");

    // Limpar buffer de input antes de esperar pelo Enter final
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
    getchar(); // Espera pelo Enter final
}

int main() {
    // Show title screen
    clear_screen();
    printf("\n\n");
    printf("    🎮 PAC-MAN TERMINAL 🎮\n");
    printf("    ----------------------\n\n");
    printf("    Controles:\n");
    printf("    W/A/S/D - Movimento\n");
    printf("    P - Pausar\n");
    printf("    Q - Sair\n\n");
    printf("    Pressione ENTER para começar...\n");
    getchar();
 
    game_loop();
    return 0;
}