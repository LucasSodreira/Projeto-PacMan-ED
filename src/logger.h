#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>
#include <time.h>

// Níveis de log
typedef enum {
    LOG_DEBUG = 0,
    LOG_INFO = 1,
    LOG_WARNING = 2,
    LOG_ERROR = 3,
    LOG_CRITICAL = 4
} LogLevel;

// Configuração do logger
typedef struct {
    LogLevel min_level;          // Nível mínimo para logar
    FILE* log_file;             // Arquivo de log (NULL para stdout)
    int include_timestamp;       // Incluir timestamp
    int include_level;          // Incluir nível
    int include_function;       // Incluir nome da função
    int colored_output;         // Saída colorida (terminal)
    char log_filename[256];     // Nome do arquivo de log
} LoggerConfig;

// Estrutura do logger
typedef struct {
    LoggerConfig config;
    int is_initialized;
    int total_messages;
    int messages_by_level[5];   // Contador por nível
} Logger;

// Macros para facilitar o uso
#define LOG_D(msg, ...) logger_log(LOG_DEBUG, __FUNCTION__, msg, ##__VA_ARGS__)
#define LOG_I(msg, ...) logger_log(LOG_INFO, __FUNCTION__, msg, ##__VA_ARGS__)
#define LOG_W(msg, ...) logger_log(LOG_WARNING, __FUNCTION__, msg, ##__VA_ARGS__)
#define LOG_E(msg, ...) logger_log(LOG_ERROR, __FUNCTION__, msg, ##__VA_ARGS__)
#define LOG_C(msg, ...) logger_log(LOG_CRITICAL, __FUNCTION__, msg, ##__VA_ARGS__)

// Funções do logger
int logger_init(const char* filename, LogLevel min_level);
void logger_shutdown();
void logger_set_level(LogLevel level);
void logger_set_colored_output(int enabled);
void logger_log(LogLevel level, const char* function, const char* format, ...);
void logger_log_queue_operation(const char* operation, int queue_size);
void logger_log_ghost_movement(int ghost_id, int old_x, int old_y, int new_x, int new_y);
void logger_log_player_action(const char* action, int x, int y);
void logger_print_stats();
const char* logger_level_to_string(LogLevel level);

// Funções específicas para o jogo
void logger_game_started();
void logger_game_ended(int score, int time_played);
void logger_level_completed(int level, int score);
void logger_collision_detected(int player_x, int player_y, int ghost_id);
void logger_dot_collected(int x, int y, int points);

#endif // LOGGER_H
