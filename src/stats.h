#ifndef STATS_H
#define STATS_H

#include <time.h>

typedef struct Player Player;

typedef struct {
    int total_enqueues;
    int total_dequeues;
    int max_size_reached;
    int current_size;
    double avg_size;
    int operations_count;
    clock_t creation_time;
    clock_t last_operation_time;
} QueueStats;

typedef struct {
    int total_moves;
    int total_dots_collected;
    int total_collisions;
    int total_games_played;
    int highest_score;
    double avg_game_time;
    int total_lives_lost;
} GameStats;

typedef struct {
    clock_t start_time;
    clock_t end_time;
    double elapsed_ms;
    const char* operation_name;
} ProfileData;

QueueStats* create_queue_stats();
void update_queue_stats_enqueue(QueueStats* stats, int current_size);
void update_queue_stats_dequeue(QueueStats* stats, int current_size);
void calculate_avg_size(QueueStats* stats);
void print_queue_stats(QueueStats* stats);
void destroy_queue_stats(QueueStats* stats);

GameStats* create_game_stats();
void update_game_stats_move(GameStats* stats);
void update_game_stats_dot_collected(GameStats* stats);
void update_game_stats_collision(GameStats* stats);
void update_game_stats_game_ended(GameStats* stats, int score, double game_time);
void print_detailed_game_stats(GameStats* stats);
void save_game_stats_to_file(GameStats* stats, const char* filename);
void load_game_stats_from_file(GameStats* stats, const char* filename);
void destroy_game_stats(GameStats* stats);

ProfileData* start_profiling(const char* operation_name);
void end_profiling(ProfileData* profile);
void print_profile_result(ProfileData* profile);
void destroy_profile_data(ProfileData* profile);

void analyze_queue_performance(QueueStats* stats);
void generate_performance_report(QueueStats* queue_stats, GameStats* game_stats);
void update_game_stats_score(GameStats* stats, int score);

#endif
void destroy_profile_data(ProfileData* profile);

// Funções auxiliares de análise
void analyze_queue_performance(QueueStats* stats);
void generate_performance_report(QueueStats* queue_stats, GameStats* game_stats);
// Função utilitária para atualizar score (stub)
void update_game_stats_score(GameStats* stats, int score);


