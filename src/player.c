#include "maze.h"
#include "player.h"
#include "logger.h"
#include "utils.h"

// Função auxiliar para converter input em direção (baseado em WASD)
Direction get_direction_from_input(char input) {
    switch (input) {
        case 'w': case 'W': return NORTH;
        case 'a': case 'A': return WEST;
        case 's': case 'S': return SOUTH;
        case 'd': case 'D': return EAST;
        default: return -1;  // direção inválida
    }
}

void player_init(Player* player, Position start_pos) {
    player->pos = start_pos;
    player->score = 0;
    player->lives = DEFAULT_LIVES;
    player->symbol = 'P';
    
    LOG_I("Jogador inicializado em (%d,%d) com %d vidas", 
          start_pos.x, start_pos.y, DEFAULT_LIVES);
}

void player_move(Player* player, Maze* maze, char input) {
    Direction dir = get_direction_from_input(input);
    if (dir < 0 || !is_valid_direction(dir)) {
        LOG_W("Direção inválida: %c", input);
        return;
    }

    Position next_pos = get_next_position(player->pos, dir);

    if (!is_valid_position(next_pos.x, next_pos.y, maze->width, maze->height)) {
        LOG_D("Movimento bloqueado - fora dos limites: (%d,%d)",
              next_pos.x, next_pos.y);
        return;
    }

    if (maze_is_wall(maze, next_pos)) {
        LOG_D("Movimento bloqueado - parede em: (%d,%d)", next_pos.x, next_pos.y);
        return;
    }

    if (maze_has_point(maze, next_pos)) {
        player->score += POINTS_PER_DOT;
        maze_remove_point(maze, next_pos);
        LOG_I("Ponto coletado! Score: %d", player->score);
    }
    
    // Move o player
    player->pos = next_pos;
    LOG_D("Jogador moveu para (%d,%d)", next_pos.x, next_pos.y);
    logger_log_player_action("moveu", next_pos.x, next_pos.y);

    if (player->score % POINTS_FOR_EXTRA_LIFE == 0) {
        player->lives++;
        LOG_I("Vida extra! Total: %d vidas", player->lives);
    }
}

int player_has_won(Player* player, GameState* game) {
    return (game->collected_dots == game->total_dots);
}

void player_lose_life(Player* player, Position start_pos) {
    player->lives--;
    player->pos = start_pos;
}
