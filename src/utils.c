#include "config.h"
#include "utils.h"
#include "maze.h"
#include "logger.h"
#include <stdio.h>   // Para printf() e getchar()
#include <time.h>    // Para srand() e time()
#include <string.h>  // Para memset()
#include <stdarg.h>  // Para va_list

#ifdef _WIN32
    #include <windows.h>
    #include <conio.h>
#else
    #include <unistd.h>
    #include <termios.h>
    #include <fcntl.h>
#endif

// ===== FUNÇÕES DE SISTEMA =====

#ifdef _WIN32
void enable_utf8_support(void) {
    // Configura codepage para Windows-1252 (compatível com CMD)
    SetConsoleOutputCP(1252);
    SetConsoleCP(1252);
}

void enable_ansi_colors(void) {
    // Habilita cores ANSI no Windows 10+
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
    DWORD dwMode = 0;
    
    // Habilita processamento de sequências ANSI para cores
    if (GetConsoleMode(hOut, &dwMode)) {
        dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
        SetConsoleMode(hOut, dwMode);
    }
    
    // Configura modo de entrada para melhor compatibilidade
    if (GetConsoleMode(hIn, &dwMode)) {
        dwMode |= ENABLE_VIRTUAL_TERMINAL_INPUT;
        SetConsoleMode(hIn, dwMode);
    }
}

void setup_cmd_console(void) {
    // Configuração específica para CMD do Windows
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    
    // Define tamanho da janela do console
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(hConsole, &csbi)) {
        COORD newSize = {80, 25}; // 80 colunas, 25 linhas
        SetConsoleScreenBufferSize(hConsole, newSize);
    }
    
    // Define título da janela
    SetConsoleTitle("PAC-MAN TERMINAL - v1.0");
    
    // Habilita cores ANSI
    enable_ansi_colors();
    
    // Configura codepage
    enable_utf8_support();
}
#endif

void setup_console(void) {
    #ifdef _WIN32
    setup_cmd_console();
    #endif
}

void clear_screen(void) {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void print_instructions(void) {
    printf("\n");
    printf("    +--------------------------------------------------------------+\n");
    printf("    |                          CONTROLES                          |\n");
    printf("    +--------------------------------------------------------------+\n");
    printf("    |  [W]  - Mover para CIMA                                     |\n");
    printf("    |  [S]  - Mover para BAIXO                                    |\n");
    printf("    |  [A]  - Mover para ESQUERDA                                 |\n");
    printf("    |  [D]  - Mover para DIREITA                                  |\n");
    printf("    |  [P]  - Pausar/Continuar                                    |\n");
    printf("    |  [Q]  - Sair do jogo                                        |\n");
    printf("    +--------------------------------------------------------------+\n");
    printf("\n");
    printf("    +--------------------------------------------------------------+\n");
    printf("    |                        COMO JOGAR                           |\n");
    printf("    +--------------------------------------------------------------+\n");
    printf("    |  * Colete todos os pontos (.) para passar de nivel          |\n");
    printf("    |  * Evite os fantasmas (F, G, B, R)                          |\n");
    printf("    |  * Colete power pellets (O) para temporariamente            |\n");
    printf("    |    deixar os fantasmas vulneraveis                          |\n");
    printf("    |  * Ganhe pontos extras completando niveis rapidamente       |\n");
    printf("    +--------------------------------------------------------------+\n");
    printf("\n");
    printf("    +--------------------------------------------------------------+\n");
    printf("    |                       PONTUACAO                             |\n");
    printf("    +--------------------------------------------------------------+\n");
    printf("    |  Ponto pequeno (.)     : 10 pontos                          |\n");
    printf("    |  Power Pellet (O)      : 50 pontos                          |\n");
    printf("    +--------------------------------------------------------------+\n");
}

void show_game_controls(void) {
    printf("\n");
    printf("    +--------------------------------------------------------------+\n");
    printf("    |                         CONTROLES                           |\n");
    printf("    +--------------------------------------------------------------+\n");
    printf("    |  [W]  - Mover para CIMA                                     |\n");
    printf("    |  [S]  - Mover para BAIXO                                    |\n");
    printf("    |  [A]  - Mover para ESQUERDA                                 |\n");
    printf("    |  [D]  - Mover para DIREITA                                  |\n");
    printf("    |  [P]  - PAUSAR/RETOMAR o jogo                               |\n");
    printf("    |  [Q]  - SAIR DO JOGO                                        |\n");
    printf("    +--------------------------------------------------------------+\n");
    
    printf("\n");
    
    // Seção de símbolos
    printf("    +--------------------------------------------------------------+\n");
    printf("    |                         SIMBOLOS                            |\n");
    printf("    +--------------------------------------------------------------+\n");
    printf("    |  C  - Pac-Man (VOCE)                                        |\n");
    printf("    |  F/G/B/R - Fantasmas                                        |\n");
    printf("    |  #  - Parede                                                |\n");
    printf("    |  .  - Ponto para coletar                                    |\n");
    printf("    |  O  - Power Pellet (deixa fantasmas azuis)                  |\n");
    printf("    |     - Caminho livre                                         |\n");
    printf("    +--------------------------------------------------------------+\n");
    
    printf("\n");
    printf("    Pressione qualquer tecla para começar...\n");
}

void show_game_objective(void) {
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
        case DIR_INVALID:
        default:
            // Não move se direção inválida
            break;
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

