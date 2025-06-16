#include "player.h"
#include "maze.h"
#include "logger.h"
#include "utils.h"  // Para Position, Direction, Player, GameState (se definidos aqui), get_next_position, is_valid_position, is_valid_direction
#include "game.h"   // Para update_score (e GameState se definido aqui)
#include "ghost.h"  // Para Ghost struct e estados GHOST_FRIGHTENED, GHOST_EATEN
#include "config.h" // Para SYMBOL_*, POINTS_FOR_EXTRA_LIFE, KEY_MOVE_*, DEFAULT_LIVES, MAX_LIVES

// Função auxiliar para converter input em direção
// Se não for chamada de fora deste arquivo .c, idealmente seria static.
Direction get_direction_from_input(char input) {
    // Usa toupper para aceitar tanto maiúsculas quanto minúsculas
    char upper_input = toupper(input);
    switch (upper_input) {
        case KEY_MOVE_UP:    return NORTH;
        case KEY_MOVE_LEFT:  return WEST;
        case KEY_MOVE_DOWN:  return SOUTH;
        case KEY_MOVE_RIGHT: return EAST;
        default: return DIR_INVALID; // DIR_INVALID definido em utils.h ou similar
    }
}

void player_init(Player* player, Position start_pos) {
    player->pos = start_pos;
    player->score = 0;
    player->lives = DEFAULT_LIVES;
    player->symbol = SYMBOL_PLAYER;

    LOG_I("Jogador inicializado em (%d,%d). Vidas: %d, Score: %d.",
          start_pos.x, start_pos.y, player->lives, player->score);
}

void player_move(Player* player, Maze* maze, char input, Ghost ghosts[], int ghost_count) {
    Direction dir = get_direction_from_input(input);
    if (dir == DIR_INVALID || !is_valid_direction(dir)) {
        return; // Input inválido ou direção não reconhecida
    }

    Position next_pos = get_next_position(player->pos, dir);

    if (!is_valid_position(next_pos.x, next_pos.y, maze->width, maze->height)) {
        return; // Movimento fora dos limites
    }

    if (maze_is_wall(maze, next_pos)) {
        return; // Movimento bloqueado por parede
    }

    char item_coletado = maze->grid[next_pos.y][next_pos.x];

    if (item_coletado == SYMBOL_DOT || item_coletado == SYMBOL_POWER_PELLET) {
        update_score(&player->score, item_coletado);
        maze_remove_point(maze, next_pos);
        LOG_I("Item '%c' coletado em (%d,%d)! Score: %d", item_coletado, next_pos.x, next_pos.y, player->score);

        if (item_coletado == SYMBOL_POWER_PELLET) {
            LOG_I("POWER PELLET! Fantasmas assustados!");
            for (int i = 0; i < ghost_count; i++) {
                if (ghosts[i].is_active && ghosts[i].state != GHOST_EATEN) {
                    ghosts[i].state = GHOST_FRIGHTENED;
                    ghosts[i].timer = 0;
                    LOG_D("Fantasma %d (%c) agora FRIGHTENED.", ghosts[i].ghost_id, ghosts[i].symbol);
                }
            }
        }
    }

    player->pos = next_pos;

    // Lógica de vida extra.
    // ATENÇÃO: `static` em uma função tem escopo de arquivo e mantém seu valor entre chamadas da função,
    // mas não será resetado entre diferentes `game_loop`s se o programa não reiniciar.
    // Para um jogo com múltiplos inícios/fins de `game_loop` sem fechar o app, isso precisaria de um reset global.
    static int last_score_at_extra_life_check = 0;
    if (POINTS_FOR_EXTRA_LIFE > 0 && player->score / POINTS_FOR_EXTRA_LIFE > last_score_at_extra_life_check / POINTS_FOR_EXTRA_LIFE) {
        if (player->lives < MAX_LIVES) {
             player->lives++;
             LOG_I("Vida extra ganha! Vidas: %d. Score: %d", player->lives, player->score);
        } else {
             LOG_I("Vida extra por score, mas já com vidas máximas (%d).", MAX_LIVES);
        }
    }
    last_score_at_extra_life_check = player->score; // Atualiza para a verificação da próxima vez
}

// A função player_has_won foi considerada obsoleta pois a lógica de vitória
// (maze_count_points == 0) é verificada em update_game (game.c).
// Se for necessária uma lógica de vitória específica do jogador, ela pode ser reativada.
/*
int player_has_won(Player* player, GameState* game) {
    (void)player;
    if (game) {
        return (game->collected_dots == game->total_dots && game->total_dots > 0);
    }
    return 0;
}
*/

void player_lose_life(Player* player, Position start_pos) {
    if (player->lives > 0) {
        player->lives--;
        LOG_I("Jogador perdeu uma vida. Vidas restantes: %d", player->lives);
    } else {
        LOG_W("Jogador já sem vidas, mas player_lose_life foi chamada.");
    }
    player->pos = start_pos;
    // Resetar estado do jogador adicional (ex: power-up temporário) pode ser feito aqui.
}
