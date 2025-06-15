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
static GameStatus game_status = PLAYING;
static int current_level = 1;
static int score = 0;
static const char* difficulty_names[] = {"Easy", "Medium", "Hard"};

void game_loop() {
    // System initialization
    logger_init("game.log", LOG_DEBUG);
    logger_game_started();
    QueueStats* queue_stats = create_queue_stats();
    GameStats* game_stats = create_game_stats();
    ProfileData* ai_profile = start_profiling("Ghost AI Performance");

    // Initialize game components
    Ghost ghosts[MAX_GHOSTS];  // Array principal de fantasmas
    Player player;
    Maze maze;
    int i;

    // Initialize maze and player
    maze_init(&maze);
    Position start_pos = {2, 2};
    player_init(&player, start_pos);

    // Initialize RNG and ghosts
    srand((unsigned)time(NULL));
    init_ghosts(ghosts, MAX_GHOSTS);

    // Set initial difficulty and positions
    for (i = 0; i < MAX_GHOSTS; i++) {
        set_ghost_difficulty(&ghosts[i], DIFFICULTY_MEDIUM);
        
        // Posições iniciais dos fantasmas em áreas seguras
        ghosts[i].pos.x = 5 + (i * 4);
        ghosts[i].pos.y = 3 + (i % 2);
        
        // Garantir que não estão em paredes
        while (maze_is_wall(&maze, ghosts[i].pos)) {
            ghosts[i].pos.x++;
            if (ghosts[i].pos.x >= maze.width) {
                ghosts[i].pos.x = 1;
                ghosts[i].pos.y++;
            }
        }
        
        LOG_I("Fantasma %d (%c) posicionado em (%d,%d)", 
              i, ghosts[i].symbol, ghosts[i].pos.x, ghosts[i].pos.y);
    }

    LOG_I("Jogo iniciado! Use WASD para mover, P para pausar, Q para sair");

    // Main game loop
    while (game_status != GAME_OVER && game_status != VICTORY) {
        // Handle pause state
        if (game_status == PAUSED) {
            clear_screen();
            printf("\n🎮 JOGO PAUSADO 🎮\n");
            printf("Pressione 'P' para continuar ou 'Q' para sair\n");
            
            char input = get_user_input();
            switch (toupper(input)) {
                case 'P': 
                    game_status = PLAYING; 
                    LOG_I("Jogo retomado");
                    break;
                case 'Q': 
                    game_status = GAME_OVER; 
                    LOG_I("Jogo encerrado durante pausa");
                    break;
            }
            continue;
        }

        // Draw game state with ghosts PRIMEIRO
        maze_render_with_ghosts(&maze, &player, ghosts, MAX_GHOSTS);
        printf("Nível: %d | Score: %d | Vidas: %d\n", 
               current_level, player.score, player.lives);
        printf("Controles: WASD=mover, P=pausar, Q=sair\n");

        // Handle player input
        char input = get_user_input();
        
        // Process input based on type
        switch (toupper(input)) {
            case 'W':
            case 'A':
            case 'S':
            case 'D':
                // Use player_move diretamente com o maze
                player_move(&player, &maze, input);
                update_game_stats_move(game_stats);
                break;
                
            case 'P':
                game_status = PAUSED;
                LOG_I("Jogo pausado");
                break;
                
            case 'Q':
                game_status = GAME_OVER;
                LOG_I("Jogo encerrado pelo jogador");
                break;
                
            default:
                // Input inválido - não fazer nada
                break;
        }

        // Move ghosts using array diretamente (SEM fila para simplificar)
        if (game_status == PLAYING) {
            for (i = 0; i < MAX_GHOSTS; i++) {
                if (!ghosts[i].is_active) continue;
                
                Position old_pos = ghosts[i].pos;
                
                // Update ghost state
                update_ghost_state(&ghosts[i], ghosts[i].timer++);
                
                // Calculate new direction using maze structure
                Direction new_dir = calculate_next_direction_maze(&ghosts[i], player.pos, &maze);
                ghosts[i].direction = new_dir;
                
                // Move ghost
                Position new_pos = get_next_position(ghosts[i].pos, new_dir);
                if (is_valid_move_ghost_maze(new_pos, &maze)) {
                    ghosts[i].pos = new_pos;
                    LOG_D("Fantasma %d moveu para (%d,%d)", i, new_pos.x, new_pos.y);
                }
                
                // Update stats
                update_game_stats_move(game_stats);
            }

            // Check collisions
            if (check_collision_with_pacman(ghosts, MAX_GHOSTS, player.pos)) {
                logger_collision_detected(player.pos.x, player.pos.y, ghosts[0].ghost_id);
                update_game_stats_collision(game_stats);
                player_lose_life(&player, start_pos);
                
                if (player.lives <= 0) {
                    game_status = GAME_OVER;
                    LOG_I("Game Over - sem vidas restantes");
                }
            }

            // Check win condition
            if (maze_count_points(&maze) == 0) {
                game_status = VICTORY;
                LOG_I("Nível completado!");
            }
        }

        // Game speed control
        sleep_ms(500);  // Velocidade mais lenta para debug
    }

    // Game over - cleanup and statistics
    end_profiling(ai_profile);
    logger_game_ended(player.score, current_level);
    print_profile_result(ai_profile);
    print_detailed_game_stats(game_stats);
    
    // Cleanup
    destroy_queue_stats(queue_stats);
    destroy_game_stats(game_stats);
    destroy_profile_data(ai_profile);
    logger_shutdown();

    // Show game over screen
    clear_screen();
    printf("\n\n");
    if (game_status == VICTORY) {
        printf("    🎉 VITÓRIA! 🎉\n");
    } else {
        printf("    💀 GAME OVER! 💀\n");
    }
    printf("    Pontuação Final: %d\n", player.score);
    printf("    Nível Alcançado: %d\n", current_level);
    printf("\n    Pressione ENTER para sair...\n");
    getchar();
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