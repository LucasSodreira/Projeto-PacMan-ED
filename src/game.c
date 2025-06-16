#include "game.h"
#include "logger.h"
#include "config.h"
#include "utils.h"
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

// ===== FUNÇÕES DE LÓGICA DO JOGO =====

// Note: draw_game foi movida para maze.c como maze_render_with_ghosts
// para consolidar todas as funções de renderização em um lugar

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