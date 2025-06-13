#include "player.h"
#include "logger.h"
#include "stats.h"
#include "utils.h"
#include <stdio.h>

void init_player(Player *player, Position start_pos) {
    player->pos = start_pos;
    player->dir = NONE;
    player->score = 0;
    player->lives = INITIAL_LIVES;
    player->moves = 0;
}

void update_player(Player *player, char **maze) {
    char input = get_user_input();

    Direction new_dir = get_direction_from_input(input);
    if (!is_valid_direction(new_dir)) {
        LOG_W("Direção inválida: %c", input);
        return;
    }

    Position next_pos = get_next_position(player->pos, new_dir);
    if (!is_valid_position(maze, next_pos)) {
        LOG_D("Movimento bloqueado por parede em (%d, %d)", next_pos.x, next_pos.y);
        return;
    }

    // Movimento válido
    player->dir = new_dir;
    player->pos = next_pos;
    player->moves++;
    LOG_D("Jogador moveu para (%d, %d)", next_pos.x, next_pos.y);

    // Coleta de ponto
    if (maze[next_pos.y][next_pos.x] == '.') {
        player->score += 10;
        maze[next_pos.y][next_pos.x] = ' ';
        LOG_I("Ponto coletado! Score: %d", player->score);
        register_point_collected();  // stats.h
    }

    // TODO: Verificação de colisão com fantasmas, perder vida, etc.
}

bool check_victory(char **maze) {
    for (int y = 0; maze[y] != NULL; y++) {
        for (int x = 0; maze[y][x] != '\0'; x++) {
            if (maze[y][x] == '.') {
                return false;
            }
        }
    }
    LOG_I("Vitória! Todos os pontos foram coletados.");
    return true;
}

void display_player_status(const Player *player) {
    printf("Score: %d | Vidas: %d | Movimentos: %d\n",
           player->score, player->lives, player->moves);
}
