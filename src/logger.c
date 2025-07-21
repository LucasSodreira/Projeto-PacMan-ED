#include "logger.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>

// Instância global do logger
static Logger g_logger = {0};

// Cores ANSI para output colorido
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

int logger_init(const char* filename, LogLevel min_level) {
    if (!filename) {
        printf("Logger: nome de arquivo nulo.\n");
        return 0;
    }
    if (g_logger.is_initialized) {
        printf("Logger já inicializado\n");
        return 1;
    }
    
    // Configurar logger
    g_logger.config.min_level = min_level;
    g_logger.config.include_timestamp = 1;
    g_logger.config.include_level = 1;
    g_logger.config.include_function = 1;
    g_logger.config.colored_output = 1;
    g_logger.total_messages = 0;
    
    // Zerar contadores
    for (int i = 0; i < 5; i++) {
        g_logger.messages_by_level[i] = 0;
    }
    
    // Configurar arquivo de log
    if (filename) {
        strncpy(g_logger.config.log_filename, filename, sizeof(g_logger.config.log_filename) - 1);
        g_logger.config.log_file = fopen(filename, "w");
        if (!g_logger.config.log_file) {
            printf("Erro: Não foi possível abrir arquivo de log %s\n", filename);
            return 0;
        }
    } else {
        g_logger.config.log_file = NULL; // Usar stdout
        strcpy(g_logger.config.log_filename, "stdout");
    }
    
    g_logger.is_initialized = 1;
    
    LOG_I("Logger inicializado - Arquivo: %s, Nível mínimo: %s", 
          g_logger.config.log_filename, logger_level_to_string(min_level));
    
    return 1;
}

void logger_shutdown() {
    if (!g_logger.is_initialized) return;
    
    LOG_I("Encerrando logger - Total de mensagens: %d", g_logger.total_messages);
    
    if (g_logger.config.log_file && g_logger.config.log_file != stdout) {
        fclose(g_logger.config.log_file);
        g_logger.config.log_file = NULL;
    }
    
    g_logger.is_initialized = 0;
}

void logger_set_level(LogLevel level) {
    if (!g_logger.is_initialized) return;
    
    g_logger.config.min_level = level;
    LOG_I("Nível de log alterado para: %s", logger_level_to_string(level));
}

void logger_set_colored_output(int enabled) {
    if (!g_logger.is_initialized) return;
    
    g_logger.config.colored_output = enabled;
}

void logger_log(LogLevel level, const char* function, const char* format, ...) {
    if (!g_logger.is_initialized) {
        printf("[LOGGER] Logger não inicializado!\n");
        return;
    }
    if (level < g_logger.config.min_level) {
        return;
    }
    
    FILE* output = g_logger.config.log_file ? g_logger.config.log_file : stdout;
    
    // Timestamp
    if (g_logger.config.include_timestamp) {
        time_t now = time(NULL);
        char time_str[64];
        strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", localtime(&now));
        fprintf(output, "[%s] ", time_str);
    }
    
    // Cor e nível
    if (g_logger.config.include_level) {
        if (g_logger.config.colored_output && output == stdout) {
            switch (level) {
                case LOG_DEBUG:    fprintf(output, ANSI_COLOR_CYAN "[DEBUG]" ANSI_COLOR_RESET " "); break;
                case LOG_INFO:     fprintf(output, ANSI_COLOR_GREEN "[INFO]" ANSI_COLOR_RESET " "); break;
                case LOG_WARNING:  fprintf(output, ANSI_COLOR_YELLOW "[WARNING]" ANSI_COLOR_RESET " "); break;
                case LOG_ERROR:    fprintf(output, ANSI_COLOR_RED "[ERROR]" ANSI_COLOR_RESET " "); break;
                case LOG_CRITICAL: fprintf(output, ANSI_COLOR_MAGENTA "[CRITICAL]" ANSI_COLOR_RESET " "); break;
            }
        } else {
            fprintf(output, "[%s] ", logger_level_to_string(level));
        }
    }
    
    // Função
    if (g_logger.config.include_function && function) {
        fprintf(output, "%s(): ", function);
    }
    
    // Mensagem
    va_list args;
    va_start(args, format);
    vfprintf(output, format, args);
    va_end(args);
    
    fprintf(output, "\n");
    
    // Flush se for arquivo
    if (g_logger.config.log_file) {
        fflush(g_logger.config.log_file);
    }
    
    // Atualizar contadores
    g_logger.total_messages++;
    g_logger.messages_by_level[level]++;
}

void logger_log_queue_operation(const char* operation, int queue_size) {
    LOG_D("Operação na fila: %s - Tamanho atual: %d", operation, queue_size);
}

void logger_log_ghost_movement(int ghost_id, int old_x, int old_y, int new_x, int new_y) {
    LOG_D("Fantasma %d moveu de (%d,%d) para (%d,%d)", ghost_id, old_x, old_y, new_x, new_y);
}

void logger_log_player_action(const char* action, int x, int y) {
    LOG_D("Jogador %s na posição (%d,%d)", action, x, y);
}

void logger_print_stats() {
    if (!g_logger.is_initialized) {
        printf("Logger não inicializado\n");
        return;
    }
    
    printf("\n=== ESTATÍSTICAS DO LOGGER ===\n");
    printf("Total de mensagens: %d\n", g_logger.total_messages);
    printf("DEBUG: %d\n", g_logger.messages_by_level[LOG_DEBUG]);
    printf("INFO: %d\n", g_logger.messages_by_level[LOG_INFO]);
    printf("WARNING: %d\n", g_logger.messages_by_level[LOG_WARNING]);
    printf("ERROR: %d\n", g_logger.messages_by_level[LOG_ERROR]);
    printf("CRITICAL: %d\n", g_logger.messages_by_level[LOG_CRITICAL]);
    printf("Arquivo: %s\n", g_logger.config.log_filename);
    printf("=============================\n\n");
}

const char* logger_level_to_string(LogLevel level) {
    switch (level) {
        case LOG_DEBUG:    return "DEBUG";
        case LOG_INFO:     return "INFO";
        case LOG_WARNING:  return "WARNING";
        case LOG_ERROR:    return "ERROR";
        case LOG_CRITICAL: return "CRITICAL";
        default:           return "UNKNOWN";
    }
}

// ===== FUNÇÕES ESPECÍFICAS PARA O JOGO =====

void logger_game_started() {
    LOG_I("═══════════════════════════");
    LOG_I("    JOGO INICIADO");
    LOG_I("═══════════════════════════");
}

void logger_game_ended(int score, int time_played) {
    LOG_I("═══════════════════════════");
    LOG_I("    JOGO FINALIZADO");
    LOG_I("Pontuação final: %d", score);
    LOG_I("Tempo jogado: %d segundos", time_played);
    LOG_I("═══════════════════════════");
}

void logger_level_completed(int level, int score) {
    LOG_I("🎉 NÍVEL %d COMPLETADO! Pontuação: %d", level, score);
}

void logger_collision_detected(int player_x, int player_y, int ghost_id) {
    LOG_W("💥 COLISÃO detectada! Jogador (%d,%d) vs Fantasma %d", player_x, player_y, ghost_id);
}

void logger_dot_collected(int x, int y, int points) {
    LOG_D("🔴 Ponto coletado em (%d,%d) - Pontos: %d", x, y, points);
}
