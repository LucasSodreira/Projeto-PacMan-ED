#include "config.h"
#include "utils.h"
#include "maze.h"
#include "ghost.h"
#include "player.h"
#include "logger.h"
#include "game.h"
#include <ctype.h>

Direction get_direction_from_input(char input) {
    char upper_input = toupper(input);
    switch (upper_input) {
        case KEY_MOVE_UP:    return NORTH;
        case KEY_MOVE_LEFT:  return WEST;
        case KEY_MOVE_DOWN:  return SOUTH;
        case KEY_MOVE_RIGHT: return EAST;
        default: return DIR_INVALID;
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

void player_move(Player* player, Maze* maze, char input, Ghost* ghosts, int ghost_count) {
    Direction dir = get_direction_from_input(input);
    if (dir == DIR_INVALID || !is_valid_direction(dir)) {
        return;
    }

    Position next_pos = get_next_position(player->pos, dir);

    if (!is_valid_position(next_pos.x, next_pos.y, maze->width, maze->height)) {
        return;
    }

    if (maze_is_wall(maze, next_pos)) {
        return;
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

    static int last_score_at_extra_life_check = 0;
    if (POINTS_FOR_EXTRA_LIFE > 0 && player->score / POINTS_FOR_EXTRA_LIFE > last_score_at_extra_life_check / POINTS_FOR_EXTRA_LIFE) {
        if (player->lives < MAX_LIVES) {
             player->lives++;
             LOG_I("Vida extra ganha! Vidas: %d. Score: %d", player->lives, player->score);
        } else {
             LOG_I("Vida extra por score, mas já com vidas máximas (%d).", MAX_LIVES);
        }
    }
    last_score_at_extra_life_check = player->score;
}

void player_lose_life(Player* player, Position start_pos) {
    if (player->lives > 0) {
        player->lives--;
        LOG_I("Jogador perdeu uma vida. Vidas restantes: %d", player->lives);
    } else {
        LOG_W("Jogador já sem vidas, mas player_lose_life foi chamada.");
    }
    player->pos = start_pos;
}
