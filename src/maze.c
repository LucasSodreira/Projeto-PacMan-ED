#include "maze.h"
#include "utils.h"     // DEPOIS de maze.h para ter Position definido
#include "config.h"
#include "logger.h"
#include "ghost.h"     // Para ter Ghost definido completamente
#include "player.h"    // Para ter Player definido completamente
#include <stdio.h>     // Para fopen, fgets, printf
#include <stdlib.h>    // Para malloc
#include <string.h>    // Para strlen, strcpy



// Exemplo de mapa simples para inicialização (ajustado para MAX_MAP_WIDTH/MAX_MAP_HEIGHT)
static char simple_map[MAX_MAP_HEIGHT][MAX_MAP_WIDTH];

static void fill_simple_map() {
    for (int y = 0; y < MAX_MAP_HEIGHT; y++) {
        for (int x = 0; x < MAX_MAP_WIDTH; x++) {
            if (y == 0 || y == MAX_MAP_HEIGHT-1 || x == 0 || x == MAX_MAP_WIDTH-1) {
                simple_map[y][x] = '#';
            } else if ((x+y)%7 == 0) {
                simple_map[y][x] = '.';
            } else {
                simple_map[y][x] = ' ';
            }
        }
    }
}

void maze_init(Maze* maze) {
    maze->width = MAZE_WIDTH;
    maze->height = MAZE_HEIGHT;
    maze->total_points = 0;

    // Carregar mapa do arquivo
    char* loaded_maze = load_maze(1); // Carregar nível 1
    
    if (loaded_maze) {
        // Copiar mapa carregado para a estrutura e contar pontos
        for (int y = 0; y < MAZE_HEIGHT; y++) {
            for (int x = 0; x < MAZE_WIDTH; x++) {
                int index = y * MAX_MAP_WIDTH + x;
                maze->grid[y][x] = loaded_maze[index];
                if (loaded_maze[index] == '.') {
                    maze->total_points++;
                }
            }
        }
        free(loaded_maze); // Liberar memória alocada
    } else {
        // Fallback: usar mapa simples se não conseguir carregar
        fill_simple_map();
        for (int y = 0; y < MAZE_HEIGHT; y++) {
            for (int x = 0; x < MAZE_WIDTH; x++) {
                maze->grid[y][x] = simple_map[y][x];
                if (simple_map[y][x] == '.') {
                    maze->total_points++;
                }
            }
        }
    }

    LOG_I("Labirinto inicializado: %dx%d com %d pontos",
          MAZE_WIDTH, MAZE_HEIGHT, maze->total_points);
}

void maze_render(const Maze* maze, Player* player) {
    clear_screen();
    
    // Copiar maze para array temporário
    char display[MAZE_HEIGHT][MAZE_WIDTH];
    for (int y = 0; y < MAZE_HEIGHT; y++) {
        for (int x = 0; x < MAZE_WIDTH; x++) {
            display[y][x] = maze->grid[y][x];
        }
    }
    
    // Colocar player no display (sobrescreve qualquer coisa na posição)
    if (player->pos.x >= 0 && player->pos.x < MAZE_WIDTH && 
        player->pos.y >= 0 && player->pos.y < MAZE_HEIGHT) {
        display[player->pos.y][player->pos.x] = player->symbol;
    }
      // Renderizar com bordas mais simples
    printf("+");
    for (int x = 0; x < MAZE_WIDTH; x++) printf("-");
    printf("+\n");
    
    for (int y = 0; y < MAZE_HEIGHT; y++) {
        printf("|");
        for (int x = 0; x < MAZE_WIDTH; x++) {
            char cell = display[y][x];
            
            // Aplicar cores baseado no símbolo
            switch (cell) {
                case 'P': 
                    printf("\x1b[33;1mO\x1b[0m");  // Amarelo brilhante para player (O = Pac-Man)
                    break;
                case '#': 
                    printf("\x1b[37;1m#\x1b[0m");  // Branco brilhante para paredes
                    break;
                case '.': 
                    printf("\x1b[36;1m.\x1b[0m");  // Ciano brilhante para pontos
                    break;
                case 'F': 
                    printf("\x1b[31;1mF\x1b[0m");  // Vermelho brilhante para fantasma F
                    break;
                case 'G': 
                    printf("\x1b[32;1mG\x1b[0m");  // Verde brilhante para fantasma G
                    break;                case 'B': 
                    printf("\x1b[34;1mB\x1b[0m");  // Azul brilhante para fantasma B
                    break;
                case 'R': 
                    printf("\x1b[35;1mR\x1b[0m");  // Magenta brilhante para fantasma R
                    break;
                default:
                    printf(" ");  // Espaço em branco para areas vazias
                    break;
            }
        }
        printf("|\n");
    }
    
    printf("+");
    for (int x = 0; x < MAZE_WIDTH; x++) printf("-");
    printf("+\n");
    
    printf("\x1b[36mScore: \x1b[33;1m%d\x1b[36m | Lives: \x1b[31;1m%d\x1b[36m | Pos: \x1b[32m(%d,%d)\x1b[0m\n", 
           player->score, player->lives, player->pos.x, player->pos.y);
    printf("Controles: WASD para mover, Q para sair\n");
}

// Nova função para renderizar maze com fantasmas - com tamanho adaptativo
void maze_render_with_ghosts(const Maze* maze, Player* player, Ghost* ghosts, int ghost_count) {
    clear_screen();
    
    // Calcular o tamanho real do mapa (não usar as constantes fixas)
    int actual_width = 0;
    int actual_height = 0;
    
    // Encontrar a largura real (linha mais longa)
    for (int y = 0; y < MAZE_HEIGHT; y++) {
        int line_width = 0;
        for (int x = 0; x < MAZE_WIDTH; x++) {
            if (maze->grid[y][x] != '\0' && maze->grid[y][x] != '\n') {
                line_width = x + 1;
            }
        }
        if (line_width > actual_width) {
            actual_width = line_width;
        }
    }
    
    // Encontrar a altura real (última linha com conteúdo)
    for (int y = MAZE_HEIGHT - 1; y >= 0; y--) {
        int has_content = 0;
        for (int x = 0; x < actual_width; x++) {
            if (maze->grid[y][x] != '\0' && maze->grid[y][x] != '\n' && maze->grid[y][x] != ' ') {
                has_content = 1;
                break;
            }
        }
        if (has_content) {
            actual_height = y + 1;
            break;
        }
    }
    
    // Garantir tamanho mínimo
    if (actual_width < 10) actual_width = 10;
    if (actual_height < 5) actual_height = 5;
    
    // Copiar maze para array temporário
    char display[MAZE_HEIGHT][MAZE_WIDTH];
    for (int y = 0; y < MAZE_HEIGHT; y++) {
        for (int x = 0; x < MAZE_WIDTH; x++) {
            display[y][x] = maze->grid[y][x];
        }
    }
    
      if (!ghosts) {
        LOG_W("Array de fantasmas é NULL em maze_render_with_ghosts");
    } else {
        // Colocar fantasmas no display primeiro (para que player sobreponha se necessário)
        for (int i = 0; i < ghost_count && i < MAX_GHOSTS; i++) {
            if (ghosts[i].is_active && 
                ghosts[i].pos.x >= 0 && ghosts[i].pos.x < actual_width && 
                ghosts[i].pos.y >= 0 && ghosts[i].pos.y < actual_height) {
                display[ghosts[i].pos.y][ghosts[i].pos.x] = ghosts[i].symbol;
            }
        }
    }
    
    // Colocar player no display (sobrescreve fantasmas na mesma posição)
    if (player && 
        player->pos.x >= 0 && player->pos.x < actual_width && 
        player->pos.y >= 0 && player->pos.y < actual_height) {
        display[player->pos.y][player->pos.x] = player->symbol;
    }

    // Renderizar borda superior do labirinto - tamanho adaptativo
    printf("\x1b[37;1m"); // Branco brilhante para bordas
    printf("    +");
    for (int x = 0; x < actual_width; x++) printf("=");
    printf("+\n");
    printf("\x1b[0m"); // Reset cor
      // Renderizar conteúdo do labirinto - tamanho adaptativo
    for (int y = 0; y < actual_height; y++) {
        printf("\x1b[37;1m    |\x1b[0m"); // Borda esquerda
        for (int x = 0; x < actual_width; x++) {
            char cell = display[y][x];
            
            // Aplicar cores baseado no símbolo - apenas ASCII
            switch (cell) {
                case 'P': 
                    printf("\x1b[33;1mO\x1b[0m");  // Amarelo brilhante para player (O = Pac-Man)
                    break;
                case '#': 
                    printf("\x1b[37;1m#\x1b[0m");  // Branco brilhante para paredes
                    break;
                case '.': 
                    printf("\x1b[36;1m.\x1b[0m");  // Ciano brilhante para pontos
                    break;
                case 'F': 
                    printf("\x1b[31;1mF\x1b[0m");  // Vermelho brilhante para fantasma F
                    break;
                case 'G': 
                    printf("\x1b[32;1mG\x1b[0m");  // Verde brilhante para fantasma G
                    break;
                case 'B': 
                    printf("\x1b[34;1mB\x1b[0m");  // Azul brilhante para fantasma B
                    break;
                case 'R': 
                    printf("\x1b[35;1mR\x1b[0m");  // Magenta brilhante para fantasma R
                    break;
                default:
                    printf(" ");  // Espaço em branco para areas vazias
                    break;
            }
        }        printf("\x1b[37;1m|\x1b[0m\n"); // Borda direita
    }
    
    // Renderizar borda inferior do labirinto - tamanho adaptativo
    printf("\x1b[37;1m"); // Branco brilhante para bordas
    printf("    +");
    for (int x = 0; x < actual_width; x++) printf("=");
    printf("+\n");
    printf("\x1b[0m"); // Reset cor

    printf("\n");
    // HUD do jogador em uma caixa adaptativa
    if (player) {
        int hud_width = actual_width > 45 ? actual_width : 45; // Mínimo de 45 para o HUD
        printf("\x1b[36m    +");
        for (int i = 0; i < hud_width; i++) printf("-");
        printf("+\n");
        printf("    |               STATUS DO JOGADOR");
        for (int i = 32; i < hud_width; i++) printf(" ");
        printf("|\n");
        printf("    +");
        for (int i = 0; i < hud_width; i++) printf("-");
        printf("+\n");
        printf("    | Score: \x1b[33;1m%-10d\x1b[36m | Vidas: \x1b[31;1m%-3d\x1b[36m | Pos: \x1b[32m(%2d,%2d)\x1b[36m",
               player->score, player->lives, player->pos.x, player->pos.y);
        int used_chars = 38; // aprox. número de chars usados
        for (int i = used_chars; i < hud_width; i++) printf(" ");
        printf("|\n");
        printf("    +");
        for (int i = 0; i < hud_width; i++) printf("-");
        printf("+\x1b[0m\n");
    }    
    // Mostrar info dos fantasmas em formato organizado - tamanho adaptativo
    if (ghosts) {
        int hud_width = actual_width > 45 ? actual_width : 45;
        printf("\x1b[35m    +");
        for (int i = 0; i < hud_width; i++) printf("-");
        printf("+\n");
        printf("    |               STATUS FANTASMAS");
        for (int i = 31; i < hud_width; i++) printf(" ");
        printf("|\n");
        printf("    +");
        for (int i = 0; i < hud_width; i++) printf("-");
        printf("+\n");
        printf("    | \x1b[0m");
        for (int i = 0; i < ghost_count && i < MAX_GHOSTS; i++) {
            if (ghosts[i].is_active) {
                // Colorir cada fantasma com sua cor específica
                switch (ghosts[i].symbol) {
                    case 'F': printf("\x1b[31;1mF\x1b[0m"); break;
                    case 'G': printf("\x1b[32;1mG\x1b[0m"); break;
                    case 'B': printf("\x1b[34;1mB\x1b[0m"); break;
                    case 'R': printf("\x1b[35;1mR\x1b[0m"); break;
                    default: printf("%c", ghosts[i].symbol); break;
                }
                printf("(%2d,%2d) ", ghosts[i].pos.x, ghosts[i].pos.y);
            }
        }
        printf("\x1b[35m");
        int used_chars = ghost_count * 8 + 2; // aprox
        for (int i = used_chars; i < hud_width; i++) printf(" ");
        printf("|\n");
        printf("    +");
        for (int i = 0; i < hud_width; i++) printf("-");
        printf("+\x1b[0m\n");
    }
}

int maze_is_wall(const Maze* maze, Position pos) {
    if (pos.x < 0 || pos.x >= MAZE_WIDTH || pos.y < 0 || pos.y >= MAZE_HEIGHT)
        return 1;
    return maze->grid[pos.y][pos.x] == '#';
}

int maze_has_point(const Maze* maze, Position pos) {
    if (pos.x < 0 || pos.x >= MAZE_WIDTH || pos.y < 0 || pos.y >= MAZE_HEIGHT)
        return 0;
    return maze->grid[pos.y][pos.x] == '.';
}

void maze_remove_point(Maze* maze, Position pos) {
    if (pos.x < 0 || pos.x >= MAZE_WIDTH || pos.y < 0 || pos.y >= MAZE_HEIGHT)
        return;
    if (maze->grid[pos.y][pos.x] == '.')
        maze->grid[pos.y][pos.x] = ' ';
}

int maze_count_points(const Maze* maze) {
    int count = 0;
    for (int y = 0; y < MAZE_HEIGHT; y++) {
        for (int x = 0; x < MAZE_WIDTH; x++) {
            if (maze->grid[y][x] == '.') {
                count++;
            }
        }
    }
    return count;
}
// Função para carregar mapa do arquivo
char* load_maze(int level) {
    char filename[256];
    sprintf(filename, "maps/level%d.txt", level);
    
    FILE* file = fopen(filename, "r");
    if (!file) {
        // Se não conseguir abrir o arquivo, criar um mapa básico
        printf("Aviso: Nao foi possivel carregar %s, usando mapa padrao\n", filename);
        int size = MAX_MAP_WIDTH * MAX_MAP_HEIGHT;
        char* maze = (char*)malloc(size);
        if (!maze) return NULL;
        
        // Criar um mapa básico com bordas
        for (int y = 0; y < MAX_MAP_HEIGHT; y++) {
            for (int x = 0; x < MAX_MAP_WIDTH; x++) {
                int index = y * MAX_MAP_WIDTH + x;
                if (x == 0 || x == MAX_MAP_WIDTH-1 || y == 0 || y == MAX_MAP_HEIGHT-1) {
                    maze[index] = '#';  // Bordas
                } else if (x == MAX_MAP_WIDTH/2 && y == MAX_MAP_HEIGHT/2) {
                    maze[index] = 'P';  // Posição inicial do player
                } else if ((x + y) % 4 == 0) {
                    maze[index] = '.';  // Alguns pontos
                } else {
                    maze[index] = ' ';  // Espaço vazio
                }
            }
        }
        return maze;
    }
    
    // Ler o arquivo linha por linha
    char line[MAX_MAP_WIDTH + 2]; // +2 para \n e \0
    int y = 0;
    int max_width = 0;
    
    // Primeiro, determinar dimensões do mapa
    while (fgets(line, sizeof(line), file) && y < MAX_MAP_HEIGHT) {
        int len = strlen(line);
        if (line[len-1] == '\n') line[len-1] = '\0'; // Remove quebra de linha
        if (len > max_width) max_width = len;
        y++;
    }
    
    int map_height = y;
    rewind(file);
    
    // Alocar memória para o mapa
    int size = MAX_MAP_WIDTH * MAX_MAP_HEIGHT;
    char* maze = (char*)malloc(size);
    if (!maze) {
        fclose(file);
        return NULL;
    }
    
    // Inicializar todo o mapa com espaços
    for (int i = 0; i < size; i++) {
        maze[i] = ' ';
    }
    
    // Carregar o mapa do arquivo
    y = 0;
    while (fgets(line, sizeof(line), file) && y < MAX_MAP_HEIGHT) {
        int len = strlen(line);
        if (line[len-1] == '\n') line[len-1] = '\0'; // Remove quebra de linha
        
        for (int x = 0; x < len && x < MAX_MAP_WIDTH; x++) {
            int index = y * MAX_MAP_WIDTH + x;
            maze[index] = line[x];
        }
        y++;
    }
    
    fclose(file);
    printf("Mapa carregado: %s (%dx%d)\n", filename, max_width, map_height);
    return maze;
}
