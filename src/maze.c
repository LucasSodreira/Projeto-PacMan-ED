#include "maze.h"
#include "config.h"
#include "utils.h"
#include "logger.h"
#include <stdio.h>

static void fill_default_map(Maze* maze, Position* out_player_start_pos, Position out_ghost_start_positions[MAX_GHOSTS], int* out_ghost_count) {
    LOG_W("Preenchendo com mapa padrão.");
    maze->total_points = 0;
    *out_ghost_count = 0;

    maze->width = MAZE_WIDTH;
    maze->height = MAZE_HEIGHT;

    for (int y = 0; y < maze->height; y++) {
        for (int x = 0; x < maze->width; x++) {
            if (y == 0 || y == maze->height - 1 || x == 0 || x == maze->width - 1) {
                maze->grid[y][x] = SYMBOL_WALL;
            } else {
                maze->grid[y][x] = SYMBOL_EMPTY_SPACE;
            }
        }
    }

    out_player_start_pos->x = 1;
    out_player_start_pos->y = 1;
    if (maze->grid[out_player_start_pos->y][out_player_start_pos->x] == SYMBOL_WALL) {
         maze->grid[out_player_start_pos->y][out_player_start_pos->x] = SYMBOL_EMPTY_SPACE;
    }

    maze->grid[3][3] = SYMBOL_DOT; maze->total_points++;
    maze->grid[3][5] = SYMBOL_DOT; maze->total_points++;
    maze->grid[5][3] = SYMBOL_POWER_PELLET; maze->total_points++;

    if (MAX_GHOSTS > 0) {
        out_ghost_start_positions[0].x = maze->width / 2;
        out_ghost_start_positions[0].y = maze->height / 2;
        if(maze->grid[out_ghost_start_positions[0].y][out_ghost_start_positions[0].x] == SYMBOL_WALL) {
            maze->grid[out_ghost_start_positions[0].y][out_ghost_start_positions[0].x] = SYMBOL_EMPTY_SPACE;
        }
        *out_ghost_count = 1;
    }
}

bool load_maze(Maze* maze, int level, Position* out_player_start_pos, Position out_ghost_start_positions[MAX_GHOSTS], int* out_ghost_count) {
    char filename[MAX_FILENAME_SIZE];
    int bytes_written = snprintf(filename, sizeof(filename), "maps/level%d.txt", level);

    if (bytes_written < 0 || (size_t)bytes_written >= sizeof(filename)) {
        LOG_E("Erro ao criar nome do arquivo de mapa para o nível %d ou nome muito longo.", level);
        printf("[ERRO] Nome de arquivo de mapa inválido para o nível %d.\n", level);
        return false;
    }

    FILE* file = fopen(filename, "r");

    if (!file) {
        LOG_E("Falha ao abrir o arquivo de mapa: %s.", filename);
        printf("[ERRO] Não foi possível abrir o arquivo de mapa: %s\n", filename);
        return false;
    }

    LOG_I("Carregando mapa: %s...", filename);
    maze->total_points = 0;
    *out_ghost_count = 0;
    out_player_start_pos->x = -1;
    out_player_start_pos->y = -1;

    maze->width = MAZE_WIDTH;
    maze->height = MAZE_HEIGHT;

    for (int y = 0; y < maze->height; y++) {
        for (int x = 0; x < maze->width; x++) {
            maze->grid[y][x] = SYMBOL_EMPTY_SPACE;
        }
    }

    char line_buffer[MAZE_WIDTH + 3];
    int current_y = 0;
    while (fgets(line_buffer, sizeof(line_buffer), file) && current_y < maze->height) {
        for (int current_x = 0; current_x < maze->width; current_x++) {
            char ch = line_buffer[current_x];
            if (ch == '\0' || ch == '\n' || ch == '\r') {
                break;
            }

            switch (ch) {
                case SYMBOL_WALL:
                    maze->grid[current_y][current_x] = SYMBOL_WALL;
                    break;
                case SYMBOL_DOT:
                    maze->grid[current_y][current_x] = SYMBOL_DOT;
                    maze->total_points++;
                    break;
                case SYMBOL_POWER_PELLET:
                    maze->grid[current_y][current_x] = SYMBOL_POWER_PELLET;
                    maze->total_points++;
                    break;
                case SYMBOL_PLAYER:
                    if (out_player_start_pos->x == -1) {
                        out_player_start_pos->x = current_x;
                        out_player_start_pos->y = current_y;
                    }
                    maze->grid[current_y][current_x] = SYMBOL_EMPTY_SPACE;
                    break;
                case SYMBOL_GHOST_RED:
                case SYMBOL_GHOST_BLUE:
                case SYMBOL_GHOST_GREEN:
                case SYMBOL_GHOST_PINK:
                    if (*out_ghost_count < MAX_GHOSTS) {
                        out_ghost_start_positions[*out_ghost_count].x = current_x;
                        out_ghost_start_positions[*out_ghost_count].y = current_y;
                        (*out_ghost_count)++;
                        maze->grid[current_y][current_x] = SYMBOL_EMPTY_SPACE;
                    }
                    break;
                case SYMBOL_EMPTY_SPACE:
                    maze->grid[current_y][current_x] = SYMBOL_EMPTY_SPACE;
                    break;
                default:
                    maze->grid[current_y][current_x] = SYMBOL_EMPTY_SPACE;
                    LOG_W("Caractere não reconhecido '%c' no mapa em (%d,%d). Tratado como espaço.", ch, current_x, current_y);
                    break;
            }
        }
        current_y++;
    }
    fclose(file);

    if (out_player_start_pos->x == -1) {
        LOG_W("Posição do jogador 'P' não encontrada no mapa '%s'. Usando padrão {1,1}.", filename);
        printf("[ERRO] Posição do jogador 'P' não encontrada no mapa '%s'. Usando padrão {1,1}.\n", filename);
        out_player_start_pos->x = 1;
        out_player_start_pos->y = 1;
        if (maze->grid[out_player_start_pos->y][out_player_start_pos->x] == SYMBOL_WALL) {
             maze->grid[out_player_start_pos->y][out_player_start_pos->x] = SYMBOL_EMPTY_SPACE;
        }
    }

    // Validação extra: se não houver pontos, o mapa é inválido
    if (maze->total_points == 0) {
        LOG_E("Mapa '%s' inválido: nenhum ponto coletável encontrado.", filename);
        printf("[ERRO] Mapa '%s' inválido: nenhum ponto coletável encontrado.\n", filename);
        return false;
    }

    LOG_I("Mapa '%s' carregado. Jogador em: (%d,%d), Fantasmas: %d, Pontos: %d.",
          filename, out_player_start_pos->x, out_player_start_pos->y, *out_ghost_count, maze->total_points);
    return true;
}

void maze_init(Maze* maze, int current_level, Position* out_player_start_pos, Position out_ghost_start_positions[MAX_GHOSTS], int* out_ghost_count) {
    if (!load_maze(maze, current_level, out_player_start_pos, out_ghost_start_positions, out_ghost_count)) {
        LOG_E("Falha crítica ao carregar mapa do nível %d. Usando mapa padrão de emergência.", current_level);
        fill_default_map(maze, out_player_start_pos, out_ghost_start_positions, out_ghost_count);
    }
    
    // Se não há fantasmas no mapa, adicionar fantasmas padrão
    if (*out_ghost_count == 0) {
        LOG_I("Nenhum fantasma encontrado no mapa. Adicionando fantasmas padrão.");
          // Posições específicas que são espaços livres no mapa level1.txt
        Position default_ghost_positions[4] = {
            {5, 5},   // Espaço livre na parte superior esquerda
            {14, 5},  // Espaço livre na parte superior direita
            {5, 14},  // Espaço livre na parte inferior esquerda
            {14, 14}  // Espaço livre na parte inferior direita
        };
        
        int ghosts_to_add = (MAX_GHOSTS < 4) ? MAX_GHOSTS : 4;
        for (int i = 0; i < ghosts_to_add; i++) {
            // Verificar se a posição é válida (não é parede)
            Position pos = default_ghost_positions[i];
            if (pos.x >= 0 && pos.x < maze->width && pos.y >= 0 && pos.y < maze->height) {
                if (maze->grid[pos.y][pos.x] == SYMBOL_WALL) {
                    // Encontrar uma posição próxima que não seja parede
                    for (int dy = -2; dy <= 2; dy++) {
                        for (int dx = -2; dx <= 2; dx++) {
                            int new_x = pos.x + dx;
                            int new_y = pos.y + dy;
                            if (new_x >= 0 && new_x < maze->width && new_y >= 0 && new_y < maze->height) {
                                if (maze->grid[new_y][new_x] != SYMBOL_WALL) {
                                    pos.x = new_x;
                                    pos.y = new_y;
                                    goto position_found;
                                }
                            }
                        }
                    }
                    position_found:;
                }
                out_ghost_start_positions[*out_ghost_count] = pos;
                (*out_ghost_count)++;
                LOG_I("Fantasma padrão %d adicionado em (%d,%d)", *out_ghost_count, pos.x, pos.y);
            }
        }
    }
    
    maze->player_start_pos_from_map = *out_player_start_pos;
    LOG_I("Labirinto finalizado para nível %d. Jogador em (%d,%d), %d fantasmas, %d pontos.",
        current_level, maze->player_start_pos_from_map.x, maze->player_start_pos_from_map.y, *out_ghost_count, maze->total_points);
}

int maze_is_wall(const Maze* maze, Position pos) {
    if (pos.x < 0 || pos.x >= maze->width || pos.y < 0 || pos.y >= maze->height)
        return 1;
    return maze->grid[pos.y][pos.x] == SYMBOL_WALL;
}

int maze_has_point(const Maze* maze, Position pos) {
    if (pos.x < 0 || pos.x >= maze->width || pos.y < 0 || pos.y >= maze->height)
        return 0;
    char cell = maze->grid[pos.y][pos.x];
    return cell == SYMBOL_DOT || cell == SYMBOL_POWER_PELLET;
}

void maze_remove_point(Maze* maze, Position pos) {
    if (pos.x < 0 || pos.x >= maze->width || pos.y < 0 || pos.y >= maze->height)
        return;
    char cell = maze->grid[pos.y][pos.x];
    if (cell == SYMBOL_DOT || cell == SYMBOL_POWER_PELLET) {
        maze->grid[pos.y][pos.x] = SYMBOL_EMPTY_SPACE;
    }
}

int maze_count_points(const Maze* maze) {
    int count = 0;
    for (int y = 0; y < maze->height; y++) {
        for (int x = 0; x < maze->width; x++) {
            char cell = maze->grid[y][x];
            if (cell == SYMBOL_DOT || cell == SYMBOL_POWER_PELLET) {
                count++;
            }
        }
    }
    return count;
}
