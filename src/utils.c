#include "utils.h"
#include "logger.h"
#include "stats.h"
#include "ghost.h"
#include "maze.h"
#include <time.h>    // Para srand() e time()

#ifdef _WIN32
    #include <windows.h>
    #include <conio.h>
#else
    #include <unistd.h>
    #include <termios.h>
    #include <fcntl.h>
#endif

// ===== FUNÇÕES DE SISTEMA =====

void clear_screen(void) {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void print_instructions(void) {
    printf("\x1b[35;1m"); // Magenta brilhante para título das instruções
    printf("    +===============================================+\n");
    printf("    |                                               |\n");
    printf("    |              I N S T R U C O E S              |\n");
    printf("    |                                               |\n");
    printf("    +===============================================+\n");
    printf("\x1b[0m\n"); // Reset cor
    
    printf("\x1b[32;1m"); // Verde brilhante para controles
    printf("    +---------------------------------------------+\n");
    printf("    |                 CONTROLES                   |\n");
    printf("    +---------------------------------------------+\n");
    printf("\x1b[0m"); // Reset cor
    printf("    | \x1b[37;1m%c\x1b[0m - Mover para CIMA                      |\n", KEY_UP);
    printf("    | \x1b[37;1m%c\x1b[0m - Mover para BAIXO                     |\n", KEY_DOWN);
    printf("    | \x1b[37;1m%c\x1b[0m - Mover para ESQUERDA                  |\n", KEY_LEFT);
    printf("    | \x1b[37;1m%c\x1b[0m - Mover para DIREITA                   |\n", KEY_RIGHT);
    printf("    | \x1b[37;1m%c\x1b[0m - PAUSAR/RETOMAR                       |\n", KEY_PAUSE);
    printf("    | \x1b[37;1m%c\x1b[0m - SAIR DO JOGO                         |\n", KEY_QUIT);
    printf("    +---------------------------------------------+\n\n");
    
    printf("\x1b[34;1m"); // Azul brilhante para símbolos
    printf("    +---------------------------------------------+\n");
    printf("    |                 SIMBOLOS                    |\n");
    printf("    +---------------------------------------------+\n");
    printf("\x1b[0m"); // Reset cor
    printf("    | \x1b[33;1m%c\x1b[0m - Pac-Man (VOCE)                       |\n", SYMBOL_PLAYER);
    printf("    | \x1b[31;1m%c\x1b[0m/\x1b[32;1m%c\x1b[0m/\x1b[34;1m%c\x1b[0m/\x1b[35;1m%c\x1b[0m - Fantasmas                  |\n", 
           SYMBOL_GHOST_RED, SYMBOL_GHOST_GREEN, SYMBOL_GHOST_BLUE, SYMBOL_GHOST_PINK);
    printf("    | \x1b[37;1m%c\x1b[0m - Parede                               |\n", SYMBOL_WALL);
    printf("    | \x1b[36;1m%c\x1b[0m - Ponto para coletar                   |\n", SYMBOL_DOT);
    printf("    | (espaco) - Caminho livre                   |\n");
    printf("    +---------------------------------------------+\n\n");
    
    printf("\x1b[33;1m"); // Amarelo brilhante para objetivo
    printf("    +---------------------------------------------+\n");
    printf("    |                 OBJETIVO                    |\n");
    printf("    +---------------------------------------------+\n");
    printf("    |  Colete todos os pontos sem tocar nos      |\n");
    printf("    |              fantasmas!                     |\n");
    printf("    +---------------------------------------------+\n");
    printf("\x1b[0m\n"); // Reset cor
    printf("\x1b[36m    Pressione ENTER para comecar...\x1b[0m");
    getchar();
}

char get_user_input(void) {
    char input;
    
    #ifdef _WIN32
        input = _getch();
    #else
        // Configurar terminal para leitura não-bloqueante
        struct termios old_termios, new_termios;
        tcgetattr(STDIN_FILENO, &old_termios);
        new_termios = old_termios;
        new_termios.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &new_termios);
        
        input = getchar();
        
        // Restaurar configurações
        tcsetattr(STDIN_FILENO, TCSANOW, &old_termios);
    #endif
    
    // Normalizar para maiúscula
    return (input >= 'a' && input <= 'z') ? (input - 'a' + 'A') : input;
}

// ===== FUNÇÕES DE POSIÇÃO =====

int is_valid_position(int x, int y, int width, int height) {
    return (x >= 0 && x < width && y >= 0 && y < height);
}

int manhattan_distance(Position a, Position b) {
    return abs(a.x - b.x) + abs(a.y - b.y);
}

int positions_equal(Position a, Position b) {
    return (a.x == b.x && a.y == b.y);
}

// ===== FUNÇÕES DE DIREÇÃO =====

int is_valid_direction(Direction dir) {
    return (dir >= NORTH && dir <= WEST);
}

const char* direction_to_string(Direction dir) {
    static const char* directions[] = {"NORTE", "LESTE", "SUL", "OESTE"};
    return is_valid_direction(dir) ? directions[dir] : "DESCONHECIDO";
}

Direction string_to_direction(const char* str) {
    if (!str) return NORTH;
    

    #ifdef _WIN32
        #define strcasecmp _stricmp
    #endif
    
    if (strcasecmp(str, "NORTE") == 0 || strcasecmp(str, "N") == 0) return NORTH;
    if (strcasecmp(str, "LESTE") == 0 || strcasecmp(str, "L") == 0) return EAST;
    if (strcasecmp(str, "SUL") == 0 || strcasecmp(str, "S") == 0) return SOUTH;
    if (strcasecmp(str, "OESTE") == 0 || strcasecmp(str, "O") == 0) return WEST;
    
    return NORTH; // Default
}

Direction get_opposite_direction(Direction dir) {
    static const Direction opposites[] = {SOUTH, WEST, NORTH, EAST};
    return is_valid_direction(dir) ? opposites[dir] : NORTH;
}

Position get_next_position(Position pos, Direction dir) {
    Position next_pos = pos;
    
    switch (dir) {
        case NORTH: next_pos.y--; break;
        case EAST:  next_pos.x++; break;
        case SOUTH: next_pos.y++; break;
        case WEST:  next_pos.x--; break;
    }
    
    return next_pos;
}

Direction random_direction(void) {
    return (Direction)random_range(NORTH, WEST);
}

// ===== FUNÇÕES DE VALIDAÇÃO =====

int is_valid_ghost_id(int ghost_id) {
    return (ghost_id >= 0 && ghost_id < MAX_GHOSTS);
}

int is_valid_ghost_symbol(char symbol) {
    return (symbol == SYMBOL_GHOST_RED || symbol == SYMBOL_GHOST_GREEN || 
            symbol == SYMBOL_GHOST_BLUE || symbol == SYMBOL_GHOST_PINK);
}

// ===== FUNÇÕES DE CONVERSÃO =====

const char* game_status_to_string(GameStatus status) {
    static const char* statuses[] = {"Jogando", "Game Over", "Vitória", "Pausado"};
    return (status >= PLAYING && status <= PAUSED) ? statuses[status] : "Desconhecido";
}

void format_time(char* buffer, int seconds) {
    if (!buffer) return;
    
    int minutes = seconds / 60;
    seconds %= 60;
    snprintf(buffer, 32, "%02d:%02d", minutes, seconds);
}

// ===== FUNÇÕES UTILITÁRIAS =====

void sleep_ms(int milliseconds) {
    #ifdef _WIN32
        Sleep(milliseconds);
    #else
        usleep(milliseconds * 1000);
    #endif
}

int random_range(int min, int max) {
    if (min > max) return min;
    return min + (rand() % (max - min + 1));
}

void debug_log(const char* format, ...) {
    #ifdef DEBUG
        printf("[DEBUG] ");
        va_list args;
        va_start(args, format);
        vprintf(format, args);
        va_end(args);
        printf("\n");
    #else
        (void)format; // Evitar warning
    #endif
}

// ===== FUNÇÕES DE ESTADO DO JOGO =====

void initialize_game_state(GameState* game) {
    if (!game) return;
    
    // Inicializar RNG
    srand((unsigned int)time(NULL));
      // Inicializar player
    game->player.pos.x = 2;
    game->player.pos.y = 2;
    game->player.score = 0;
    game->player.lives = DEFAULT_LIVES;
    game->player.symbol = SYMBOL_PLAYER;
    
    // Inicializar estado
    game->map_width = MAX_MAP_WIDTH;
    game->map_height = MAX_MAP_HEIGHT;
    game->total_dots = 0;
    game->collected_dots = 0;
    game->num_ghosts = 0;
    game->status = PLAYING;
    game->level = 1;
    
    // Limpar mapa
    memset(game->map, SYMBOL_EMPTY_SPACE, sizeof(game->map));
    
    printf("✅ Estado do jogo inicializado!\n");
}

void print_game_stats(GameState* game) {
    if (!game) return;
    
    printf("\n╔════════════════════════════════════╗\n");
    printf("║            ESTATÍSTICAS            ║\n");
    printf("╚════════════════════════════════════╝\n");
    printf("📊 Pontuação: %d\n", game->player.score);
    printf("❤️  Vidas: %d\n", game->player.lives);
    printf("🎯 Nível: %d\n", game->level);
    printf("🔴 Pontos: %d/%d\n", game->collected_dots, game->total_dots);
    printf("🎮 Status: %s\n", game_status_to_string(game->status));
    printf("═══════════════════════════════════════\n");
}

// ===== FUNÇÕES DE INICIALIZAÇÃO DE SISTEMA =====

int initialize_game_systems(QueueStats** queue_stats, 
                            GameStats** game_stats, 
                            ProfileData** ai_profile) {
    // Inicializar sistemas
    if (!logger_init(DEFAULT_LOG_FILE, LOG_DEBUG)) {
        printf("[ERROR] Erro: Falha ao inicializar sistema de logging\n");
        return 0;
    }
      logger_game_started();
    
    *queue_stats = (QueueStats*)create_queue_stats();
    *game_stats = (GameStats*)create_game_stats();
    *ai_profile = (ProfileData*)start_profiling("Ghost AI Performance");
    
    if (!*queue_stats || !*game_stats || !*ai_profile) {
        printf("[ERROR] Erro: Falha ao inicializar sistemas de estatisticas\n");
        return 0;
    }
    
    LOG_I("Todos os sistemas inicializados com sucesso");
    return 1;
}

void cleanup_game_systems(QueueStats* queue_stats, 
                         GameStats* game_stats, 
                         ProfileData* ai_profile,
                         Player* player) {
    // Finalizar profiling
    end_profiling(ai_profile);
    logger_game_ended(player->score, 1);  // current level
      // Imprimir estatísticas finais
    printf("\n[STATS] RELATORIO DE PERFORMANCE:\n");
    print_profile_result(ai_profile);
    print_detailed_game_stats(game_stats);
    
    // Cleanup
    destroy_queue_stats(queue_stats);
    destroy_game_stats(game_stats);
    destroy_profile_data(ai_profile);
    logger_shutdown();
}

// Note: initialize_ghosts_safely movida para ghost.c