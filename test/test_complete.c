#include "../src/utils.h"
#include "../src/queue.h"
#include "../src/stats.h"
#include "../src/logger.h"
#include <assert.h>
#include <time.h>
#include <stdlib.h>

void test_stats_system();
void test_logger_system();
void test_performance_profiling();
void test_integration_all_systems();

int main() {
    printf("=== TESTE COMPLETO DO SISTEMA ===\n\n");
    
    // Inicializar sistemas
    srand((unsigned)time(NULL));
    
    // Executar todos os testes
    test_logger_system();
    test_stats_system();
    test_performance_profiling();
    test_integration_all_systems();
    
    printf("\n=== TESTE COMPLETO FINALIZADO ===\n");
    printf("Sistema completo funcionando perfeitamente! 🎉\n");
    
    return 0;
}

void test_logger_system() {
    printf("1. Testando sistema de logging...\n");
    
    // Inicializar logger
    assert(logger_init("test_log.txt", LOG_DEBUG) == 1);
    
    // Testar diferentes níveis de log
    LOG_D("Esta é uma mensagem de debug");
    LOG_I("Esta é uma mensagem informativa");
    LOG_W("Esta é uma mensagem de aviso");
    LOG_E("Esta é uma mensagem de erro");
    LOG_C("Esta é uma mensagem crítica");
    
    // Testar funções específicas do jogo
    logger_game_started();
    logger_log_player_action("moveu", 5, 5);
    logger_log_ghost_movement(1, 3, 3, 4, 3);
    logger_dot_collected(5, 5, 10);
    logger_collision_detected(5, 5, 1);
    logger_level_completed(1, 250);
    logger_game_ended(250, 120);
    
    // Mostrar estatísticas
    logger_print_stats();
    
    // Testar mudança de nível
    logger_set_level(LOG_WARNING);
    LOG_D("Esta mensagem debug não deve aparecer");
    LOG_W("Esta mensagem warning deve aparecer");
    
    logger_shutdown();
    printf("✅ Sistema de logging funcionando!\n");
}

void test_stats_system() {
    printf("2. Testando sistema de estatísticas...\n");
    
    // Testar estatísticas da fila
    QueueStats* queue_stats = create_queue_stats();
    assert(queue_stats != NULL);
    
    // Simular operações na fila
    for (int i = 0; i < 10; i++) {
        update_queue_stats_enqueue(queue_stats, i + 1);
        if (i > 5) {
            update_queue_stats_dequeue(queue_stats, i);
        }
    }
    
    print_queue_stats(queue_stats);
    analyze_queue_performance(queue_stats);
    
    // Testar estatísticas do jogo
    GameStats* game_stats = create_game_stats();
    assert(game_stats != NULL);
    
    // Simular uma partida
    for (int i = 0; i < 50; i++) {
        update_game_stats_move(game_stats);
        if (i % 5 == 0) {
            update_game_stats_dot_collected(game_stats);
        }
        if (i % 15 == 0) {
            update_game_stats_collision(game_stats);
        }    }
    
    update_game_stats_game_ended(game_stats, 200, 45.5);
    print_detailed_game_stats(game_stats);
    
    // Testar salvar/carregar estatísticas
    save_game_stats_to_file(game_stats, "test_stats.dat");
    
    GameStats* loaded_stats = create_game_stats();
    load_game_stats_from_file(loaded_stats, "test_stats.dat");
    assert(loaded_stats->total_moves == game_stats->total_moves);
    
    // Gerar relatório completo
    generate_performance_report(queue_stats, game_stats);
    
    // Limpeza
    destroy_queue_stats(queue_stats);
    destroy_game_stats(game_stats);
    destroy_game_stats(loaded_stats);
    
    printf("✅ Sistema de estatísticas funcionando!\n");
}

void test_performance_profiling() {
    printf("3. Testando sistema de profiling...\n");
    
    // Testar profiling de criação de fila
    ProfileData* profile1 = start_profiling("Criação da fila");
    Queue* queue = create_queue();
    end_profiling(profile1);
    print_profile_result(profile1);
    
    // Testar profiling de operações em massa
    ProfileData* profile2 = start_profiling("1000 operações enqueue/dequeue");
    
    for (int i = 0; i < 1000; i++) {
        Ghost ghost = {{i, i}, NORTH, i, 'F', 1};
        enqueue(queue, ghost);
        
        if (i > 500) {
            dequeue(queue);
        }
    }
    
    end_profiling(profile2);
    print_profile_result(profile2);
    
    // Testar profiling de busca
    ProfileData* profile3 = start_profiling("Busca por ghost ID");
    int found = find_ghost_in_queue(queue, 750);
    end_profiling(profile3);
    print_profile_result(profile3);
    
    printf("Ghost 750 encontrado na posição: %d\n", found);
    
    // Limpeza
    destroy_queue(queue);
    destroy_profile_data(profile1);
    destroy_profile_data(profile2);
    destroy_profile_data(profile3);
    
    printf("✅ Sistema de profiling funcionando!\n");
}

void test_integration_all_systems() {
    printf("4. Testando integração de todos os sistemas...\n");
    
    // Inicializar todos os sistemas
    logger_init(NULL, LOG_INFO); // Log para stdout
    
    QueueStats* queue_stats = create_queue_stats();
    Queue* queue = create_queue();
    
    LOG_I("Iniciando teste de integração");
    
    // Simular cenário real de jogo
    ProfileData* game_profile = start_profiling("Simulação completa de jogo");
    
    // Adicionar fantasmas com logging e stats
    for (int i = 1; i <= 4; i++) {
        Ghost ghost = {{5 + i, 5 + i}, random_direction(), i, 'F' + i - 1, 1};
        
        ProfileData* enqueue_profile = start_profiling("Enqueue com stats");
        enqueue(queue, ghost);
        update_queue_stats_enqueue(queue_stats, queue_size(queue));
        end_profiling(enqueue_profile);
        
        LOG_I("Fantasma %d adicionado à fila", i);
        logger_log_queue_operation("ENQUEUE", queue_size(queue));
        
        destroy_profile_data(enqueue_profile);
    }
    
    // Simular movimentação dos fantasmas
    for (int turn = 0; turn < 10; turn++) {
        LOG_I("=== Turno %d ===", turn + 1);
        
        if (!is_empty(queue)) {
            Ghost ghost = dequeue(queue);
            update_queue_stats_dequeue(queue_stats, queue_size(queue));
            
            // Simular movimento
            Position old_pos = ghost.pos;
            ghost.pos = get_next_position(ghost.pos, ghost.direction);
            
            logger_log_ghost_movement(ghost.ghost_id, old_pos.x, old_pos.y, 
                                    ghost.pos.x, ghost.pos.y);
            
            // Recolocar na fila
            enqueue(queue, ghost);
            update_queue_stats_enqueue(queue_stats, queue_size(queue));
        }
    }
    
    end_profiling(game_profile);
    
    LOG_I("Teste de integração concluído");
    print_profile_result(game_profile);
    
    // Mostrar estatísticas finais
    print_queue_stats(queue_stats);
    logger_print_stats();
    
    // Limpeza
    destroy_queue(queue);
    destroy_queue_stats(queue_stats);
    destroy_profile_data(game_profile);
    logger_shutdown();
    
    printf("✅ Integração de todos os sistemas funcionando!\n");
}
