#include "stats.h"
#include "config.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ===== FUNÇÕES PARA ESTATÍSTICAS DA FILA =====

QueueStats* create_queue_stats() {
    QueueStats* stats = (QueueStats*)malloc(sizeof(QueueStats));
    if (!stats) {
        printf("Erro: Não foi possível alocar memória para estatísticas da fila\n");
        return NULL;
    }
    
    stats->total_enqueues = 0;
    stats->total_dequeues = 0;
    stats->max_size_reached = 0;
    stats->current_size = 0;
    stats->avg_size = 0.0;
    stats->operations_count = 0;
    stats->creation_time = clock();
    stats->last_operation_time = clock();
    
    return stats;
}

void update_queue_stats_enqueue(QueueStats* stats, int current_size) {
    if (!stats) return;
    
    stats->total_enqueues++;
    stats->current_size = current_size;
    stats->operations_count++;
    stats->last_operation_time = clock();
    
    if (current_size > stats->max_size_reached) {
        stats->max_size_reached = current_size;
    }
    
    calculate_avg_size(stats);
}

void update_queue_stats_dequeue(QueueStats* stats, int current_size) {
    if (!stats) return;
    
    stats->total_dequeues++;
    stats->current_size = current_size;
    stats->operations_count++;
    stats->last_operation_time = clock();
    
    calculate_avg_size(stats);
}

void calculate_avg_size(QueueStats* stats) {
    if (!stats || stats->operations_count == 0) return;
    
    // Atualizar média móvel simples
    stats->avg_size = ((stats->avg_size * (stats->operations_count - 1)) + stats->current_size) / stats->operations_count;
}

void print_queue_stats(QueueStats* stats) {
    if (!stats) {
        printf("Estatísticas da fila não disponíveis\n");
        return;
    }
    
    double uptime = (double)(stats->last_operation_time - stats->creation_time) / CLOCKS_PER_SEC;
    
    printf("=== ESTATÍSTICAS DA FILA ===\n");
    printf("Total de inserções: %d\n", stats->total_enqueues);
    printf("Total de remoções: %d\n", stats->total_dequeues);
    printf("Tamanho atual: %d\n", stats->current_size);
    printf("Tamanho máximo alcançado: %d\n", stats->max_size_reached);
    printf("Tamanho médio: %.2f\n", stats->avg_size);
    printf("Total de operações: %d\n", stats->operations_count);
    printf("Tempo de vida: %.2f segundos\n", uptime);
    
    if (stats->operations_count > 0) {
        printf("Operações por segundo: %.2f\n", stats->operations_count / uptime);
    }
    printf("========================\n\n");
}

void destroy_queue_stats(QueueStats* stats) {
    if (stats) {
        free(stats);
    }
}

// ===== FUNÇÕES PARA ESTATÍSTICAS DO JOGO =====

GameStats* create_game_stats() {
    GameStats* stats = (GameStats*)malloc(sizeof(GameStats));
    if (!stats) {
        printf("Erro: Não foi possível alocar memória para estatísticas do jogo\n");
        return NULL;
    }
    
    stats->total_moves = 0;
    stats->total_dots_collected = 0;
    stats->total_collisions = 0;
    stats->total_games_played = 0;
    stats->highest_score = 0;
    stats->avg_game_time = 0.0;
    stats->total_lives_lost = 0;
    
    return stats;
}

void update_game_stats_move(GameStats* stats) {
    if (!stats) return;
    stats->total_moves++;
}

void update_game_stats_dot_collected(GameStats* stats) {
    if (!stats) return;
    stats->total_dots_collected++;
}

void update_game_stats_collision(GameStats* stats) {
    if (!stats) return;
    stats->total_collisions++;
    stats->total_lives_lost++;
}

void update_game_stats_game_ended(GameStats* stats, int score, double game_time) {
    if (!stats) return;
    
    stats->total_games_played++;
    
    if (score > stats->highest_score) {
        stats->highest_score = score;
    }
    
    // Atualizar tempo médio de jogo
    stats->avg_game_time = ((stats->avg_game_time * (stats->total_games_played - 1)) + game_time) / stats->total_games_played;
}

void print_detailed_game_stats(GameStats* stats) {
    if (!stats) {
        printf("Estatísticas do jogo não disponíveis\n");
        return;
    }
    
    printf("=== ESTATÍSTICAS DETALHADAS DO JOGO ===\n");
    printf("Total de jogos: %d\n", stats->total_games_played);
    printf("Total de movimentos: %d\n", stats->total_moves);
    printf("Total de pontos coletados: %d\n", stats->total_dots_collected);
    printf("Total de colisões: %d\n", stats->total_collisions);
    printf("Total de vidas perdidas: %d\n", stats->total_lives_lost);
    printf("Maior pontuação: %d\n", stats->highest_score);
    printf("Tempo médio de jogo: %.2f segundos\n", stats->avg_game_time);
    
    if (stats->total_moves > 0) {
        printf("Eficiência (pontos/movimento): %.2f\n", (double)stats->total_dots_collected / stats->total_moves);
    }
    
    if (stats->total_games_played > 0) {
        printf("Média de pontos por jogo: %.2f\n", (double)stats->total_dots_collected * POINTS_PER_DOT / stats->total_games_played);
        printf("Taxa de colisão: %.2f%%\n", (double)stats->total_collisions / stats->total_moves * 100);
    }
    printf("===========================\n\n");
}

void save_game_stats_to_file(GameStats* stats, const char* filename) {
    if (!stats || !filename) return;
    
    FILE* file = fopen(filename, "w");
    if (!file) {
        printf("Erro: Não foi possível salvar estatísticas em %s\n", filename);
        return;
    }
    
    fprintf(file, "%d,%d,%d,%d,%d,%.2f,%d\n",
            stats->total_moves,
            stats->total_dots_collected,
            stats->total_collisions,
            stats->total_games_played,
            stats->highest_score,
            stats->avg_game_time,
            stats->total_lives_lost);
    
    fclose(file);
    printf("Estatísticas salvas em %s\n", filename);
}

void load_game_stats_from_file(GameStats* stats, const char* filename) {
    if (!stats || !filename) return;
    
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Aviso: Arquivo de estatísticas %s não encontrado\n", filename);
        return;
    }
    
    int result = fscanf(file, "%d,%d,%d,%d,%d,%lf,%d",
                       &stats->total_moves,
                       &stats->total_dots_collected,
                       &stats->total_collisions,
                       &stats->total_games_played,
                       &stats->highest_score,
                       &stats->avg_game_time,
                       &stats->total_lives_lost);
    
    fclose(file);
    
    if (result == 7) {
        printf("Estatísticas carregadas de %s\n", filename);
    } else {
        printf("Erro: Arquivo de estatísticas %s corrompido\n", filename);
    }
}

void destroy_game_stats(GameStats* stats) {
    if (stats) {
        free(stats);
    }
}

// ===== FUNÇÕES PARA PROFILING =====

ProfileData* start_profiling(const char* operation_name) {
    ProfileData* profile = (ProfileData*)malloc(sizeof(ProfileData));
    if (!profile) {
        printf("Erro: Não foi possível alocar memória para profiling\n");
        return NULL;
    }
    
    profile->operation_name = operation_name;
    profile->start_time = clock();
    profile->end_time = 0;
    profile->elapsed_ms = 0.0;
    
    return profile;
}

void end_profiling(ProfileData* profile) {
    if (!profile) return;
    
    profile->end_time = clock();
    profile->elapsed_ms = ((double)(profile->end_time - profile->start_time) / CLOCKS_PER_SEC) * 1000.0;
}

void print_profile_result(ProfileData* profile) {
    if (!profile) {
        printf("Dados de profiling não disponíveis\n");
        return;
    }
    
    printf("[PROFILE] %s: %.3f ms\n", profile->operation_name, profile->elapsed_ms);
}

void destroy_profile_data(ProfileData* profile) {
    if (profile) {
        free(profile);
    }
}

// ===== FUNÇÕES AUXILIARES DE ANÁLISE =====

void analyze_queue_performance(QueueStats* stats) {
    if (!stats) return;
    
    printf("=== ANÁLISE DE PERFORMANCE DA FILA ===\n");
    
    // Análise de utilização
    if (stats->max_size_reached > 0) {
        double utilization = (stats->avg_size / stats->max_size_reached) * 100;
        printf("Utilização média: %.1f%%\n", utilization);
        
        if (utilization > 80) {
            printf("⚠️  Alta utilização - considere aumentar capacidade\n");
        } else if (utilization < 20) {
            printf("ℹ️  Baixa utilização - fila pode ser otimizada\n");
        } else {
            printf("✅ Utilização adequada\n");
        }
    }
    
    // Análise de operações
    if (stats->total_enqueues > 0 && stats->total_dequeues > 0) {
        double ratio = (double)stats->total_enqueues / stats->total_dequeues;
        printf("Taxa enqueue/dequeue: %.2f\n", ratio);
        
        if (ratio > 1.2) {
            printf("⚠️  Mais inserções que remoções - fila crescendo\n");
        } else if (ratio < 0.8) {
            printf("⚠️  Mais remoções que inserções - fila diminuindo\n");
        } else {
            printf("✅ Taxa de operações equilibrada\n");
        }
    }
    
    printf("=====================================\n\n");
}

void generate_performance_report(QueueStats* queue_stats, GameStats* game_stats) {
    printf("╔════════════════════════════════════╗\n");
    printf("║        RELATÓRIO DE PERFORMANCE    ║\n");
    printf("╚════════════════════════════════════╝\n\n");
    
    if (queue_stats) {
        print_queue_stats(queue_stats);
        analyze_queue_performance(queue_stats);
    }
    
    if (game_stats) {
        print_detailed_game_stats(game_stats);
    }
    
    printf("Relatório gerado em: ");
    time_t now = time(NULL);
    printf("%s", ctime(&now));
}


// Função stub para update_game_stats_score
void update_game_stats_score(GameStats* stats, int score) {
    if (stats) stats->highest_score = score;
}
