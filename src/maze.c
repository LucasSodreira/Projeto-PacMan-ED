#include "maze.h"
#include "config.h"
#include "utils.h"
#include "logger.h"


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

    fill_simple_map();

    // Copiar mapa e contar pontos
    for (int y = 0; y < MAZE_HEIGHT; y++) {
        for (int x = 0; x < MAZE_WIDTH; x++) {
            maze->grid[y][x] = simple_map[y][x];
            if (simple_map[y][x] == '.') {
                maze->total_points++;
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
    
    // Renderizar com bordas
    printf("╔");
    for (int x = 0; x < MAZE_WIDTH; x++) printf("═");
    printf("╗\n");
    
    for (int y = 0; y < MAZE_HEIGHT; y++) {
        printf("║");
        for (int x = 0; x < MAZE_WIDTH; x++) {
            char cell = display[y][x];
            
            // Aplicar cores baseado no símbolo
            switch (cell) {
                case 'P': 
                    printf("\x1b[33m%c\x1b[0m", cell);  // Amarelo para player
                    break;
                case '#': 
                    printf("\x1b[37m%c\x1b[0m", cell);  // Branco para paredes
                    break;
                case '.': 
                    printf("\x1b[36m%c\x1b[0m", cell);  // Ciano para pontos
                    break;
                case 'F': 
                    printf("\x1b[31m%c\x1b[0m", cell);  // Vermelho para fantasma F
                    break;
                case 'G': 
                    printf("\x1b[32m%c\x1b[0m", cell);  // Verde para fantasma G
                    break;
                case 'B': 
                    printf("\x1b[34m%c\x1b[0m", cell);  // Azul para fantasma B
                    break;
                case 'R': 
                    printf("\x1b[35m%c\x1b[0m", cell);  // Magenta para fantasma R
                    break;
                default:
                    printf("%c", cell);
                    break;
            }
        }
        printf("║\n");
    }
    
    printf("╚");
    for (int x = 0; x < MAZE_WIDTH; x++) printf("═");
    printf("╝\n");
    
    printf("📊 Score: %d | ❤️  Lives: %d | 📍 Pos: (%d,%d)\n", 
           player->score, player->lives, player->pos.x, player->pos.y);
    printf("Controles: WASD para mover, Q para sair\n");
}

// Nova função para renderizar maze com fantasmas
void maze_render_with_ghosts(const Maze* maze, Player* player, Ghost* ghosts, int ghost_count) {
    clear_screen();
    
    // Copiar maze para array temporário
    char display[MAZE_HEIGHT][MAZE_WIDTH];
    for (int y = 0; y < MAZE_HEIGHT; y++) {
        for (int x = 0; x < MAZE_WIDTH; x++) {
            display[y][x] = maze->grid[y][x];
        }
    }
    
    // Colocar fantasmas no display primeiro (para que player sobreponha se necessário)
    for (int i = 0; i < ghost_count; i++) {
        if (ghosts[i].is_active && 
            ghosts[i].pos.x >= 0 && ghosts[i].pos.x < MAZE_WIDTH && 
            ghosts[i].pos.y >= 0 && ghosts[i].pos.y < MAZE_HEIGHT) {
            display[ghosts[i].pos.y][ghosts[i].pos.x] = ghosts[i].symbol;
        }
    }
    
    // Colocar player no display (sobrescreve fantasmas na mesma posição)
    if (player->pos.x >= 0 && player->pos.x < MAZE_WIDTH && 
        player->pos.y >= 0 && player->pos.y < MAZE_HEIGHT) {
        display[player->pos.y][player->pos.x] = player->symbol;
    }
    
    // Renderizar com bordas
    printf("╔");
    for (int x = 0; x < MAZE_WIDTH; x++) printf("═");
    printf("╗\n");
    
    for (int y = 0; y < MAZE_HEIGHT; y++) {
        printf("║");
        for (int x = 0; x < MAZE_WIDTH; x++) {
            char cell = display[y][x];
            
            // Aplicar cores baseado no símbolo
            switch (cell) {
                case 'P': 
                    printf("\x1b[33m%c\x1b[0m", cell);  // Amarelo para player
                    break;
                case '#': 
                    printf("\x1b[37m%c\x1b[0m", cell);  // Branco para paredes
                    break;
                case '.': 
                    printf("\x1b[36m%c\x1b[0m", cell);  // Ciano para pontos
                    break;
                case 'F': 
                    printf("\x1b[31m%c\x1b[0m", cell);  // Vermelho para fantasma F
                    break;
                case 'G': 
                    printf("\x1b[32m%c\x1b[0m", cell);  // Verde para fantasma G
                    break;
                case 'B': 
                    printf("\x1b[34m%c\x1b[0m", cell);  // Azul para fantasma B
                    break;
                case 'R': 
                    printf("\x1b[35m%c\x1b[0m", cell);  // Magenta para fantasma R
                    break;
                default:
                    printf("%c", cell);
                    break;
            }
        }
        printf("║\n");
    }
    
    printf("╚");
    for (int x = 0; x < MAZE_WIDTH; x++) printf("═");
    printf("╝\n");
    
    printf("📊 Score: %d | ❤️  Lives: %d | 📍 Pos: (%d,%d)\n", 
           player->score, player->lives, player->pos.x, player->pos.y);
    
    // Mostrar info dos fantasmas
    printf("👻 Fantasmas: ");
    for (int i = 0; i < ghost_count; i++) {
        if (ghosts[i].is_active) {
            printf("%c(%d,%d) ", ghosts[i].symbol, ghosts[i].pos.x, ghosts[i].pos.y);
        }
    }
    printf("\n");
    printf("Controles: WASD para mover, Q para sair\n");
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
// Função stub para load_maze (retorna um mapa vazio)
char* load_maze(int level) {
    int size = MAX_MAP_WIDTH * MAX_MAP_HEIGHT;
    char* maze = (char*)malloc(size);
    if (!maze) return NULL;
    for (int i = 0; i < size; i++) maze[i] = (i % MAX_MAP_WIDTH == 0 || i % MAX_MAP_WIDTH == MAX_MAP_WIDTH-1 || i < MAX_MAP_WIDTH || i >= size-MAX_MAP_WIDTH) ? '#' : ' ';
    return maze;
}
