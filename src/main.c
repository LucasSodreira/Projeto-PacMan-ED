#include "main.h"
#include "game.h"
#include "ghost.h"
#include "queue.h"
#include "logger.h"
#include "stats.h"
#include "maze.h"
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
    Queue* ghost_queue = create_queue();
    Ghost ghosts[MAX_GHOSTS];
    Position pacman = {5, 5};  // Starting position
    char* maze = load_maze(current_level);  // Load maze for current level
    bool game_over = false;
    int i;

    // Initialize RNG and ghosts
    srand((unsigned)time(NULL));
    init_ghosts(ghosts, MAX_GHOSTS);

    // Set initial difficulty
    for (i = 0; i < MAX_GHOSTS; i++) {
        set_ghost_difficulty(&ghosts[i], DIFFICULTY_MEDIUM);
        enqueue(ghost_queue, ghosts[i]);
        update_queue_stats_enqueue(queue_stats, queue_size(ghost_queue));
    }    // Main game loop
    while (game_status != GAME_OVER) {
        // Handle pause state
        if (game_status == PAUSED) {
            printf("\nGAME PAUSED\nPress 'p' to continue or 'q' to quit\n");
            sleep_ms(100);  // Prevent CPU hammering
            
            // Handle input during pause
            char input = get_user_input();
            switch (tolower(input)) {
                case 'p': game_status = PLAYING; break;
                case 'q': game_status = GAME_OVER; break;
                // Debug commands during pause
                case 'd': handle_debug_command('d'); break;
                case 's': handle_debug_command('s'); break;
                case 'l': handle_debug_command('l'); break;
            }
            continue;
        }

        // Handle player input
        char input = get_user_input();
        if (process_player_input(input, &pacman, maze, &game_status)) {
            update_game_stats_move(game_stats);
        }

        // Move ghosts using FIFO queue
        for (i = 0; i < MAX_GHOSTS; i++) {
            Ghost ghost = dequeue(ghost_queue);
            update_queue_stats_dequeue(queue_stats, queue_size(ghost_queue));
            
            Position old_pos = ghost.pos;
            
            // Use improved AI for ghost movement
            Direction new_dir = calculate_next_direction(&ghost, pacman, maze);
            ghost.direction = new_dir;
            ghost.pos = get_next_position(ghost.pos, new_dir);
            
            // Log movement and update stats
            logger_log_ghost_movement(ghost.ghost_id, old_pos.x, old_pos.y, ghost.pos.x, ghost.pos.y);
            update_game_stats_move(game_stats);
            
            // Re-queue ghost
            enqueue(ghost_queue, ghost);
            update_queue_stats_enqueue(queue_stats, queue_size(ghost_queue));
        }

        // Check collisions
        Ghost* ghosts_arr;
        int arr_size;
        ghosts_arr = queue_to_array(ghost_queue, &arr_size);
        for (i = 0; i < arr_size; i++) {
            if (positions_equal(ghosts_arr[i].pos, pacman)) {
                // Check if ghost is frightened
                if (ghosts_arr[i].state == GHOST_FRIGHTENED) {
                    score += 200;  // Points for eating ghost
                    update_game_stats_score(game_stats, score);
                } else {
                    logger_collision_detected(pacman.x, pacman.y, ghosts_arr[i].ghost_id);
                    update_game_stats_collision(game_stats);
                    game_status = GAME_OVER;
                }
            }
        }
        free(ghosts_arr);

        // Draw game state
        clear_screen();
        printf("PAC-MAN - Level %d - Score: %d\n", current_level, score);
        printf("Difficulty: %s\n", difficulty_names[DIFFICULTY_MEDIUM]);
        draw_game(ghosts, MAX_GHOSTS, pacman, maze);
        printf("\nPress 'p' to pause, 'q' to quit\n");

        // Small delay to control game speed
        sleep_ms(33);  // ~30 FPS
    }

    // Game over - cleanup and statistics
    end_profiling(ai_profile);
    logger_game_ended(score, current_level);
    print_profile_result(ai_profile);
    print_queue_stats(queue_stats);
    print_detailed_game_stats(game_stats);
    destroy_queue(ghost_queue);
    destroy_queue_stats(queue_stats);
    destroy_game_stats(game_stats);
    destroy_profile_data(ai_profile);
    free(maze);
    logger_shutdown();

    // Show game over screen
    clear_screen();
    printf("\n\n");
    printf("    GAME OVER!\n");
    printf("    Final Score: %d\n", score);
    printf("    Level Reached: %d\n", current_level);
    printf("\n    Press any key to exit...\n");
    getchar();
}

int main() {
    // Show title screen
    clear_screen();
    printf("\n\n");
    printf("    PAC-MAN\n");
    printf("    -------\n\n");
    printf("    Press Enter to start...\n");
    printf("    'p' - Pause game\n");
    printf("    'q' - Quit game\n\n");
    getchar();
 
    game_loop();
    return 0;
}