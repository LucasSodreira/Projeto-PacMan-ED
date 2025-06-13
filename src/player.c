#include "player.h"

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
}

void player_move(Player* player, GameState* game, char input) {
    Direction dir = get_direction_from_input(input);
    if (dir < 0 || !is_valid_direction(dir)) {
        // Entrada inválida, não move
        return;
    }

    Position next_pos = get_next_position(player->pos, dir);

    // Verifica se posição é válida no mapa
    if (!is_valid_position(next_pos.x, next_pos.y, game->map_width, game->map_height)) {
        return;
    }

    // Verifica se não é parede ('#' por exemplo)
    if (game->map[next_pos.y][next_pos.x] == '#') {
        return;
    }

    // Move o player
    player->pos = next_pos;

    // Verifica se coletou ponto ('.' no mapa)
    if (game->map[next_pos.y][next_pos.x] == '.') {
        player->score += POINTS_PER_DOT;
        game->map[next_pos.y][next_pos.x] = ' '; // remove ponto do mapa
        game->collected_dots++;

        // Se atingiu pontos para vida extra
        if (player->score % POINTS_FOR_EXTRA_LIFE == 0) {
            player->lives++;
        }
    }
}

int player_has_won(Player* player, GameState* game) {
    return (game->collected_dots == game->total_dots);
}

void player_lose_life(Player* player, Position start_pos) {
    player->lives--;
    player->pos = start_pos;
}
