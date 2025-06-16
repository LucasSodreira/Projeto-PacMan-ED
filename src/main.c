#include "main.h"
#include "game.h"
#include "ghost.h"   
#include "maze.h"
#include "player.h"
#include "logger.h"
#include "stats.h"
#include "utils.h"
#include <stdlib.h>
#include <time.h>

// ===== VARIÁVEIS GLOBAIS DO JOGO =====
static GameStatus g_game_status = PLAYING;
static int g_current_level = 1;
static const char* g_difficulty_names[] = {"Fácil", "Médio", "Difícil"};

// ===== FUNÇÕES AUXILIARES =====

// ===== FUNÇÕES AUXILIARES =====

static void handle_player_input_and_movement(Player* player, Maze* maze, GameStats* game_stats) {
    char input = get_user_input();
    
    switch (input) {
        case KEY_UP:
        case KEY_DOWN:
        case KEY_LEFT:
        case KEY_RIGHT:
            player_move(player, maze, input);
            update_game_stats_move(game_stats);
            break;
            
        case KEY_PAUSE:
            g_game_status = (g_game_status == PAUSED) ? PLAYING : PAUSED;
            LOG_I("Jogo %s", (g_game_status == PAUSED) ? "pausado" : "retomado");
            break;
            
        case KEY_QUIT:
            g_game_status = GAME_OVER;
            LOG_I("Jogo encerrado pelo jogador");
            break;
            
        // Comandos de debug
        case 'd':
            handle_debug_command('d');
            break;
        case 's':
            handle_debug_command('s');
            break;
        case 'l':
            handle_debug_command('l');
            break;
            
        default:
            // Input ignorado
            break;
    }
}

static void update_ghosts_ai(Ghost ghosts[], Player* player, Maze* maze, GameStats* game_stats) {
    for (int i = 0; i < MAX_GHOSTS; i++) {
        if (!ghosts[i].is_active) continue;
        
        // Atualizar estado do fantasma
        update_ghost_state(&ghosts[i], ghosts[i].timer++);
        
        // Calcular nova direção usando IA
        Direction new_dir = calculate_next_direction_maze(&ghosts[i], player->pos, maze);
        ghosts[i].direction = new_dir;
        
        // Mover fantasma se movimento for válido
        Position new_pos = get_next_position(ghosts[i].pos, new_dir);
        if (is_valid_move_ghost_maze(new_pos, maze)) {
            ghosts[i].pos = new_pos;
            LOG_D("Fantasma %d moveu para (%d,%d)", i, new_pos.x, new_pos.y);
        }
        
        update_game_stats_move(game_stats);
    }
}

static void check_game_conditions(Ghost ghosts[], Player* player, Maze* maze, 
                                 Position start_pos, GameStats* game_stats) {
    // Verificar colisões
    if (check_collision_with_pacman(ghosts, MAX_GHOSTS, player->pos)) {
        logger_collision_detected(player->pos.x, player->pos.y, ghosts[0].ghost_id);
        update_game_stats_collision(game_stats);
        player_lose_life(player, start_pos);
        
        if (player->lives <= 0) {
            g_game_status = GAME_OVER;
            LOG_I("Game Over - sem vidas restantes");
        } else {
            LOG_W("Vida perdida! Restam %d vidas", player->lives);
        }
    }
    
    // Verificar condição de vitória
    if (maze_count_points(maze) == 0) {
        g_game_status = VICTORY;
        LOG_I("Nível %d completado!", g_current_level);
    }
}

static void render_game_interface(Maze* maze, Player* player, Ghost ghosts[]) {
    maze_render_with_ghosts(maze, player, ghosts, MAX_GHOSTS);
    
    // HUD com cores ANSI e símbolos ASCII simples
    printf("\x1b[36m==================================================\x1b[0m\n");
    printf("\x1b[32mNivel: \x1b[33;1m%d\x1b[0m | \x1b[32mScore: \x1b[33;1m%d\x1b[0m | \x1b[32mVidas: \x1b[31;1m%d\x1b[0m | \x1b[32mDificuldade: \x1b[35m%s\x1b[0m\n", 
           g_current_level, player->score, player->lives, 
           g_difficulty_names[DIFFICULTY_MEDIUM]);
    printf("\x1b[34mControles: \x1b[37m%c%c%c%c\x1b[34m=mover | \x1b[37m%c\x1b[34m=pausar | \x1b[37m%c\x1b[34m=sair\x1b[0m\n",
           KEY_UP, KEY_LEFT, KEY_DOWN, KEY_RIGHT, KEY_PAUSE, KEY_QUIT);
    printf("\x1b[36m==================================================\x1b[0m\n");
}

// ===== FUNÇÃO PRINCIPAL DO GAME LOOP =====


void game_loop(void) {
    // Inicializar sistemas
    struct QueueStats* queue_stats;
    GameStats* game_stats;
    struct ProfileData* ai_profile;
    
    if (!initialize_game_systems(&queue_stats, &game_stats, &ai_profile)) {
        return;
    }
    
    // Inicializar componentes do jogo
    Ghost ghosts[MAX_GHOSTS];
    Player player;
    Maze maze;
    
    // Setup inicial
    maze_init(&maze);
    Position start_pos = {2, 2};
    player_init(&player, start_pos);
    initialize_ghosts_safely(ghosts, &maze);
    
    LOG_I("Jogo iniciado! Use %c%c%c%c para mover", 
          KEY_UP, KEY_LEFT, KEY_DOWN, KEY_RIGHT);
    
    // ===== MAIN GAME LOOP =====
    while (g_game_status != GAME_OVER && g_game_status != VICTORY) {
        
        // Tratar estado de pausa
        if (g_game_status == PAUSED) {
            clear_screen();
            printf("\n    ⏸️  JOGO PAUSADO ⏸️\n");
            printf("    Pressione '%c' para continuar ou '%c' para sair\n", 
                   KEY_PAUSE, KEY_QUIT);
            
            char input = get_user_input();
            if (input == KEY_PAUSE) {
                g_game_status = PLAYING;
                LOG_I("Jogo retomado");
            } else if (input == KEY_QUIT) {
                g_game_status = GAME_OVER;
                LOG_I("Jogo encerrado durante pausa");
            }
            continue;
        }
        
        // Renderizar interface
        render_game_interface(&maze, &player, ghosts);
        
        // Processar input do jogador
        handle_player_input_and_movement(&player, &maze, game_stats);
        
        // Atualizar IA dos fantasmas (apenas se jogando)
        if (g_game_status == PLAYING) {
            update_ghosts_ai(ghosts, &player, &maze, game_stats);
            check_game_conditions(ghosts, &player, &maze, start_pos, game_stats);
        }
        
        // Controle de velocidade do jogo
        sleep_ms(GAME_SPEED_MS);
    }
      // Cleanup e tela final
    cleanup_game_systems(queue_stats, game_stats, ai_profile, &player);
    show_game_over_screen(&player, g_game_status, g_current_level);
}

// ===== FUNÇÃO MAIN =====

int main(void) {
    show_title_screen();
    game_loop();
    
    printf("\n\x1b[33;1m*** Obrigado por jogar %s! ***\x1b[0m\n", GAME_TITLE);
    return 0;
}