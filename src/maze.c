#include "maze.h"
#include "config.h"
#include "utils.h"
#include "logger.h"


static const char simple_map[MAZE_HEIGHT][MAZE_WIDTH] = {
    {'#','#','#','#','#','#','#','#','#','#'},
    {'#','.','.','#','.','.','.','.','.','#'},
    {'#','.','.','#','.','.','#','.','.','#'},
    {'#','.','.','.','.','.','.','.','.','#'},
    {'#','#','#','#','#','#','#','#','#','#'}
};

void maze_init(Maze* maze) {
    maze->width = MAZE_WIDTH;
    maze->height = MAZE_HEIGHT;
    maze->total_points = 0;
    
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
    clear_screen();  // ✅ Usar função da Pessoa 1
    
    // Copiar maze para array temporário
    char display[MAZE_HEIGHT][MAZE_WIDTH];
    for (int y = 0; y < MAZE_HEIGHT; y++) {
        for (int x = 0; x < MAZE_WIDTH; x++) {
            display[y][x] = maze->grid[y][x];
        }
    }
    
    // Colocar player no display
    display[player->pos.y][player->pos.x] = player->symbol;
    
    // Renderizar
    printf("=== PAC-MAN ===\n");
    for (int y = 0; y < MAZE_HEIGHT; y++) {
        for (int x = 0; x < MAZE_WIDTH; x++) {
            printf("%c", display[y][x]);
        }
        printf("\n");
    }
    
    printf("\n📊 Score: %d | ❤️  Lives: %d | 📍 Pos: (%d,%d)\n", 
           player->score, player->lives, player->pos.x, player->pos.y);
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
