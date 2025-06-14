#ifndef STATS_H
#define STATS_H

#include "utils.h"
#include <time.h>

// Estrutura para estatísticas da fila
typedef struct {
    int total_enqueues;          // Total de inserções
    int total_dequeues;          // Total de remoções
    int max_size_reached;        // Tamanho máximo alcançado
    int current_size;            // Tamanho atual
    double avg_size;             // Tamanho médio
    int operations_count;        // Contador de operações
    clock_t creation_time;       // Tempo de criação
    clock_t last_operation_time; // Última operação
} QueueStats;

// Estrutura para estatísticas gerais do jogo
typedef struct {
    int total_moves;             // Total de movimentos
    int total_dots_collected;    // Total de pontos coletados
    int total_collisions;        // Total de colisões
    int total_games_played;      // Total de jogos
    int highest_score;           // Maior pontuação
    double avg_game_time;        // Tempo médio de jogo
    int total_lives_lost;        // Total de vidas perdidas
} GameStats;

// Estrutura para profiling de performance
typedef struct {
    clock_t start_time;
    clock_t end_time;
    double elapsed_ms;
    const char* operation_name;
} ProfileData;

// Funções para estatísticas da fila
QueueStats* create_queue_stats();
void update_queue_stats_enqueue(QueueStats* stats, int current_size);
void update_queue_stats_dequeue(QueueStats* stats, int current_size);
void calculate_avg_size(QueueStats* stats);
void print_queue_stats(QueueStats* stats);
void destroy_queue_stats(QueueStats* stats);

// Funções para estatísticas do jogo
GameStats* create_game_stats();
void update_game_stats_move(GameStats* stats);
void update_game_stats_dot_collected(GameStats* stats);
void update_game_stats_collision(GameStats* stats);
void update_game_stats_game_ended(GameStats* stats, int score, double game_time);
void print_detailed_game_stats(GameStats* stats);
void save_game_stats_to_file(GameStats* stats, const char* filename);
void load_game_stats_from_file(GameStats* stats, const char* filename);
void destroy_game_stats(GameStats* stats);

// Funções para profiling
ProfileData* start_profiling(const char* operation_name);
void end_profiling(ProfileData* profile);
void print_profile_result(ProfileData* profile);
void destroy_profile_data(ProfileData* profile);

// Funções auxiliares de análise
void analyze_queue_performance(QueueStats* stats);
void generate_performance_report(QueueStats* queue_stats, GameStats* game_stats);
// Função utilitária para atualizar score (stub)
void update_game_stats_score(GameStats* stats, int score);

#endif // STATS_H
