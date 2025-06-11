#include "utils.h"

#ifdef _WIN32
    #include <windows.h>
    #include <conio.h>
#else
    #include <unistd.h>
    #include <termios.h>
    #include <fcntl.h>
#endif

// Limpar a tela do terminal
void clear_screen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

// Imprimir instruções do jogo
void print_instructions() {
    printf("=== PAC-MAN - INSTRUÇÕES ===\n");
    printf("Controles:\n");
    printf("  W - Mover para cima\n");
    printf("  S - Mover para baixo\n");
    printf("  A - Mover para esquerda\n");
    printf("  D - Mover para direita\n");
    printf("  Q - Sair do jogo\n\n");
    
    printf("Símbolos:\n");
    printf("  P - Pac-Man (você)\n");
    printf("  F/G/B/R - Fantasmas\n");
    printf("  # - Parede\n");
    printf("  . - Ponto para coletar\n");
    printf("  (espaço) - Caminho livre\n\n");
    
    printf("Objetivo: Colete todos os pontos sem tocar nos fantasmas!\n");
    printf("Pressione ENTER para continuar...\n");
    getchar();
}

// Obter entrada do usuário (funciona em Windows e Linux)
char get_user_input() {
    char input;
    
    #ifdef _WIN32
        input = _getch();
    #else
        // Configurar terminal para leitura imediata
        struct termios old_termios, new_termios;
        tcgetattr(STDIN_FILENO, &old_termios);
        new_termios = old_termios;
        new_termios.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &new_termios);
        
        input = getchar();
        
        // Restaurar configurações do terminal
        tcsetattr(STDIN_FILENO, TCSANOW, &old_termios);
    #endif
    
    // Converter para maiúscula
    if (input >= 'a' && input <= 'z') {
        input = input - 'a' + 'A';
    }
    
    return input;
}

// Verificar se uma posição é válida no mapa
int is_valid_position(int x, int y, int width, int height) {
    return (x >= 0 && x < width && y >= 0 && y < height);
}

// Inicializar estado do jogo com valores padrão
void initialize_game_state(GameState* game) {
    if (!game) return;
    
    // Inicializar jogador
    game->player.pos.x = 1;
    game->player.pos.y = 1;
    game->player.score = 0;
    game->player.lives = 3;
    game->player.symbol = 'P';
    
    // Inicializar estado geral
    game->map_width = 0;
    game->map_height = 0;
    game->total_dots = 0;
    game->collected_dots = 0;
    game->num_ghosts = 0;
    game->status = PLAYING;
    game->level = 1;
    
    // Limpar mapa
    for (int i = 0; i < MAX_MAP_SIZE; i++) {
        for (int j = 0; j < MAX_MAP_SIZE; j++) {
            game->map[i][j] = ' ';
        }
    }
    
    printf("Estado do jogo inicializado!\n");
}

// Imprimir estatísticas do jogo
void print_game_stats(GameState* game) {
    if (!game) return;
    
    printf("\n=== ESTATÍSTICAS ===\n");
    printf("Pontuação: %d\n", game->player.score);
    printf("Vidas: %d\n", game->player.lives);
    printf("Nível: %d\n", game->level);
    printf("Pontos coletados: %d/%d\n", game->collected_dots, game->total_dots);
    
    // Status do jogo
    switch (game->status) {
        case PLAYING:
            printf("Status: Jogando\n");
            break;
        case GAME_OVER:
            printf("Status: GAME OVER!\n");
            break;
        case VICTORY:
            printf("Status: VITÓRIA!\n");
            break;
        case PAUSED:
            printf("Status: Pausado\n");
            break;    }
    printf("==================\n\n");
}

// ===== FUNÇÕES AUXILIARES =====

// Validar direção
int is_valid_direction(Direction dir) {
    return (dir >= NORTH && dir <= WEST);
}

// Converter direção para string
const char* direction_to_string(Direction dir) {
    switch (dir) {
        case NORTH: return "NORTE";
        case EAST:  return "LESTE";
        case SOUTH: return "SUL";
        case WEST:  return "OESTE";
        default:    return "DESCONHECIDO";
    }
}

// Converter string para direção
Direction string_to_direction(const char* str) {
    if (!str) return NORTH;
    
    if (strcmp(str, "NORTE") == 0 || strcmp(str, "N") == 0) return NORTH;
    if (strcmp(str, "LESTE") == 0 || strcmp(str, "L") == 0) return EAST;
    if (strcmp(str, "SUL") == 0 || strcmp(str, "S") == 0) return SOUTH;
    if (strcmp(str, "OESTE") == 0 || strcmp(str, "O") == 0) return WEST;
    
    return NORTH; // Default
}

// Obter direção oposta
Direction get_opposite_direction(Direction dir) {
    switch (dir) {
        case NORTH: return SOUTH;
        case EAST:  return WEST;
        case SOUTH: return NORTH;
        case WEST:  return EAST;
        default:    return NORTH;
    }
}

// Calcular próxima posição com base na direção
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

// Calcular distância Manhattan entre duas posições
int manhattan_distance(Position a, Position b) {
    return abs(a.x - b.x) + abs(a.y - b.y);
}

// Verificar se duas posições são iguais
int positions_equal(Position a, Position b) {
    return (a.x == b.x && a.y == b.y);
}

// Validar ID de fantasma
int is_valid_ghost_id(int ghost_id) {
    return (ghost_id >= 1 && ghost_id <= MAX_GHOSTS);
}

// Validar símbolo de fantasma
int is_valid_ghost_symbol(char symbol) {
    return (symbol == 'F' || symbol == 'G' || symbol == 'B' || symbol == 'R');
}

// Converter status do jogo para string
const char* game_status_to_string(GameStatus status) {
    switch (status) {
        case PLAYING:   return "Jogando";
        case GAME_OVER: return "Game Over";
        case VICTORY:   return "Vitória";
        case PAUSED:    return "Pausado";
        default:        return "Desconhecido";
    }
}

// Pausar execução por milissegundos (multiplataforma)
void sleep_ms(int milliseconds) {
    #ifdef _WIN32
        Sleep(milliseconds);
    #else
        usleep(milliseconds * 1000);
    #endif
}

// Gerar número aleatório entre min e max
int random_range(int min, int max) {
    if (min > max) return min;
    return min + (rand() % (max - min + 1));
}

// Gerar direção aleatória
Direction random_direction() {
    return (Direction)random_range(NORTH, WEST);
}

// Formatear tempo em string (mm:ss)
void format_time(char* buffer, int seconds) {
    if (!buffer) return;
    
    int minutes = seconds / 60;
    seconds = seconds % 60;
    sprintf(buffer, "%02d:%02d", minutes, seconds);
}

// Log de debug (apenas se DEBUG estiver definido)
void debug_log(const char* format, ...) {
    #ifdef DEBUG
        printf("[DEBUG] ");
        va_list args;
        va_start(args, format);
        vprintf(format, args);
        va_end(args);
        printf("\n");
    #endif
}