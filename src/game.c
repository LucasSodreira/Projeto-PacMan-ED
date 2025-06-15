#include "game.h"
#include "logger.h"
#include "config.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

void update_game(Ghost ghosts[], int ghost_count, Position *pacman_pos, bool *game_over, const char* maze) {
    // Log player position
    logger_log_player_action("moveu para", pacman_pos->x, pacman_pos->y);

    // Move ghosts - Note: This function signature needs to be updated
    // move_ghosts(ghosts, ghost_count, *pacman_pos, maze);

    // Check collision
    if (check_collision_with_pacman(ghosts, ghost_count, *pacman_pos)) {
        logger_collision_detected(pacman_pos->x, pacman_pos->y, ghosts[0].ghost_id);
        *game_over = true;
    }
}

void draw_game(Ghost ghosts[], int ghost_count, Position pacman_pos, const char* maze) {
    clear_screen();
    
    // Print top border with game title
    printf("╔");
    for (int i = 0; i < MAX_MAP_WIDTH; i++) printf("═");
    printf("╗\n");

    // Print maze with game elements
    for (int y = 0; y < MAX_MAP_HEIGHT; y++) {
        printf("║");
        for (int x = 0; x < MAX_MAP_WIDTH; x++) {
            Position current = {x, y};
            char symbol = maze[y * MAX_MAP_WIDTH + x];

            // Check for game elements at this position
            bool is_ghost = false;
            for (int i = 0; i < ghost_count; i++) {
                if (positions_equal(ghosts[i].pos, current)) {
                    // Color for each ghost type
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
                if (positions_equal(pacman_pos, current)) {
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

    // Print bottom border
    printf("╚");
    for (int i = 0; i < MAX_MAP_WIDTH; i++) printf("═");
    printf("╝\n");
}

bool process_player_input(char input, Position* pacman_pos, const char* maze, GameStatus* status) {
    Direction dir = NORTH;
    bool moved = false;
    
    // Corrigir mapeamento das teclas - usar as teclas definidas em config.h
    switch (toupper(input)) {  // Converter para maiúscula
        case 'W': dir = NORTH; moved = true; break;   // Cima
        case 'S': dir = SOUTH; moved = true; break;   // Baixo
        case 'A': dir = WEST; moved = true; break;    // Esquerda
        case 'D': dir = EAST; moved = true; break;    // Direita
        case 'P': toggle_pause(status); break;        // Pausar
        case 'Q': 
            *status = GAME_OVER;
            LOG_I("Jogo encerrado pelo jogador");
            break;
        case 'd': 
        case 's': 
        case 'l':
            handle_debug_command(input);
            break;
    }
    
    if (moved) {
        Position new_pos = get_next_position(*pacman_pos, dir);
        
        // Verificar limites do maze usando as dimensões corretas
        if (new_pos.x >= 0 && new_pos.x < MAX_MAP_WIDTH && 
            new_pos.y >= 0 && new_pos.y < MAX_MAP_HEIGHT) {
            
            // Acessar o maze usando a estrutura 2D correta
            char cell = '#'; // Default para parede
            
            // Tentar acessar o maze de forma segura
            if (maze) {
                // Se o maze é passado como string linear
                int index = new_pos.y * MAX_MAP_WIDTH + new_pos.x;
                if (index >= 0 && index < MAX_MAP_WIDTH * MAX_MAP_HEIGHT) {
                    cell = maze[index];
                }
            }
            
            if (cell != '#') {  // Não é parede
                logger_log_player_action("moveu para", new_pos.x, new_pos.y);
                *pacman_pos = new_pos;
                
                // Check if dot collected
                if (cell == '.') {
                    logger_dot_collected(new_pos.x, new_pos.y, POINTS_PER_DOT);
                } else if (cell == 'O') {
                    logger_dot_collected(new_pos.x, new_pos.y, POINTS_PER_POWER_PELLET);
                }
                
                return true;
            } else {
                LOG_D("Movimento bloqueado - parede em: (%d,%d)", new_pos.x, new_pos.y);
            }
        } else {
            LOG_D("Movimento bloqueado - fora dos limites: (%d,%d)", new_pos.x, new_pos.y);
        }
    }
    
    return false;
}

void handle_debug_command(char input) {
    switch (tolower(input)) {
        case 'd':
            LOG_D("Comando de debug: Dump do estado do jogo");
            logger_print_stats();
            break;
        case 's':
            LOG_D("Comando de debug: Exibindo estatísticas");
            logger_print_stats();
            break;
        case 'l':
            LOG_D("Comando de debug: Alternando nível de log");
            logger_set_level(LOG_DEBUG);
            break;
    }
}

void toggle_pause(GameStatus* status) {
    *status = (*status == PAUSED) ? PLAYING : PAUSED;
    LOG_I("Estado do jogo alterado para: %s", *status == PAUSED ? "PAUSADO" : "JOGANDO");
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
        case SYMBOL_GHOST_RED:
        case SYMBOL_GHOST_BLUE:
        case SYMBOL_GHOST_GREEN:
        case SYMBOL_GHOST_PINK:
            points = POINTS_FOR_EXTRA_LIFE;
            break;
    }
    
    if (points > 0) {
        *score += points;
        LOG_D("Pontuação atualizada: %d (+%d pontos)", *score, points);
    }
}