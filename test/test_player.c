#include <stdio.h>
#include <assert.h>
#include "player.h"
#include "utils.h"

// Função auxiliar para criar um mapa simples para teste
void setup_simple_map(GameState* game) {
    // Limpa o mapa
    for (int y = 0; y < MAX_MAP_SIZE; y++) {
        for (int x = 0; x < MAX_MAP_SIZE; x++) {
            game->map[y][x] = ' ';
        }
    }

    // Define paredes (#) nas bordas 5x5
    int width = 5, height = 5;
    for (int x = 0; x < width; x++) {
        game->map[0][x] = '#';
        game->map[height-1][x] = '#';
    }
    for (int y = 0; y < height; y++) {
        game->map[y][0] = '#';
        game->map[y][width-1] = '#';
    }

    // Pontos para coletar ('.')
    game->map[1][2] = '.';
    game->map[2][2] = '.';

    game->map_width = width;
    game->map_height = height;
    game->total_dots = 2;
    game->collected_dots = 0;
    game->status = PLAYING;
}

int main() {
    GameState game;
    setup_simple_map(&game);

    Position start_pos = {1, 1};
    player_init(&game.player, start_pos);

    // Testa posição inicial do player
    assert(game.player.pos.x == 1 && game.player.pos.y == 1);
    assert(game.player.score == 0);
    assert(game.player.lives == DEFAULT_LIVES);

    // Move para a direita (deve pegar ponto em 1,2)
    player_move(&game.player, &game, 'd');
    assert(game.player.pos.x == 2 && game.player.pos.y == 1);
    assert(game.player.score == POINTS_PER_DOT);
    assert(game.collected_dots == 1);
    // Ponto removido do mapa
    assert(game.map[1][2] == ' ');

    // Move para baixo (deve pegar segundo ponto em 2,2)
    player_move(&game.player, &game, 's');
    assert(game.player.pos.x == 2 && game.player.pos.y == 2);
    assert(game.player.score == 2 * POINTS_PER_DOT);
    assert(game.collected_dots == 2);
    assert(game.map[2][2] == ' ');

    // Testa vitória (coletou todos os pontos)
    assert(player_has_won(&game.player, &game) == 1);

    printf("Todos os testes do player passaram com sucesso!\n");
    return 0;
}
