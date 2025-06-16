#ifndef MAZE_H
#define MAZE_H

#include "utils.h"    // Para Position, logging, Player, Ghost (se estiverem aqui)
#include "config.h"   // Para constantes como MAX_GHOSTS, MAZE_WIDTH, MAZE_HEIGHT

// Use apenas MAX_MAP_WIDTH/MAX_MAP_HEIGHT de config.h
// #define MAZE_WIDTH MAX_MAP_WIDTH // Já definido em config.h, não precisa redefinir
// #define MAZE_HEIGHT MAX_MAP_HEIGHT // Já definido em config.h, não precisa redefinir

typedef struct {
    char grid[MAZE_HEIGHT][MAZE_WIDTH];
    int width;
    int height;
    int total_points;
    Position player_start_pos_from_map; // Posição do jogador lida do mapa
} Maze;

// Funções principais
// Assinatura atualizada para carregar nível específico e retornar posições iniciais
void maze_init(Maze* maze, int current_level, Position* out_player_start_pos, Position out_ghost_start_positions[MAX_GHOSTS], int* out_ghost_count);

// Funções de renderização (supondo que Player e Ghost são conhecidos via utils.h ou includes diretos)
// Se Player ou Ghost não estiverem em utils.h, seus respectivos headers seriam necessários.
// No entanto, para manter maze.h mais genérico, pode ser melhor que Player/Ghost sejam forward declared se possível,
// ou que essas funções de renderização estejam em game.h/game.c.
// Por agora, mantenho como estava na leitura anterior do arquivo.
struct Player; // Forward declaration
struct Ghost;  // Forward declaration

void maze_render(const Maze* maze, struct Player* player);
void maze_render_with_ghosts(const Maze* maze, struct Player* player, struct Ghost* ghosts, int ghost_count);

// Funções de estado do labirinto
int maze_is_wall(const Maze* maze, Position pos);
int maze_has_point(const Maze* maze, Position pos); // Verifica se há ponto ou power pellet
void maze_remove_point(Maze* maze, Position pos);   // Remove ponto ou power pellet
int maze_count_points(const Maze* maze);            // Conta pontos restantes (dots + power pellets)

// Função utilitária para carregar o labirinto de um arquivo
// Retorna true se o carregamento for bem-sucedido, false caso contrário.
// Preenche as posições iniciais do jogador e dos fantasmas, e a contagem de fantasmas.
bool load_maze(Maze* maze, int level, Position* out_player_start_pos, Position out_ghost_start_positions[MAX_GHOSTS], int* out_ghost_count);

#endif // MAZE_H
