#include "utils.h"
#include "queue.h"

int main() {
    printf("=== TESTE DAS ESTRUTURAS BASE ===\n\n");
    
    // Teste 1: Inicializar estado do jogo
    printf("1. Testando inicialização do GameState...\n");
    GameState game;
    initialize_game_state(&game);
    print_game_stats(&game);
    
    // Teste 2: Testar criação de fantasmas
    printf("2. Testando criação de fantasmas...\n");
    Ghost ghost1 = {{5, 5}, NORTH, 1, 'F', 1};
    Ghost ghost2 = {{10, 10}, EAST, 2, 'G', 1};
    
    printf("Fantasma 1: ID=%d, Pos=(%d,%d), Dir=%d, Symbol='%c'\n", 
           ghost1.ghost_id, ghost1.pos.x, ghost1.pos.y, ghost1.direction, ghost1.symbol);
    printf("Fantasma 2: ID=%d, Pos=(%d,%d), Dir=%d, Symbol='%c'\n", 
           ghost2.ghost_id, ghost2.pos.x, ghost2.pos.y, ghost2.direction, ghost2.symbol);
    
    // Teste 3: Testar fila de fantasmas
    printf("\n3. Testando fila de fantasmas...\n");
    Queue* ghost_queue = create_queue();
    
    if (ghost_queue) {
        printf("✓ Fila criada com sucesso!\n");
        
        // Adicionar fantasmas
        enqueue(ghost_queue, ghost1);
        enqueue(ghost_queue, ghost2);
        printf("✓ Fantasmas adicionados à fila!\n");
        
        // Mostrar conteúdo da fila
        print_queue(ghost_queue);
        
        // Remover um fantasma
        Ghost removed = dequeue(ghost_queue);
        printf("Fantasma removido: ID=%d, Pos=(%d,%d)\n", 
               removed.ghost_id, removed.pos.x, removed.pos.y);
        
        // Mostrar fila após remoção
        printf("Fila após remoção:\n");
        print_queue(ghost_queue);
        
        // Limpar memória
        destroy_queue(ghost_queue);
        printf("✓ Fila destruída com sucesso!\n");
    } else {
        printf("✗ Erro ao criar fila!\n");
    }
    
    // Teste 4: Testar funções utilitárias
    printf("\n4. Testando funções utilitárias...\n");
    printf("Posição (5,5) válida para mapa 20x20? %s\n", 
           is_valid_position(5, 5, 20, 20) ? "Sim" : "Não");
    printf("Posição (25,25) válida para mapa 20x20? %s\n", 
           is_valid_position(25, 25, 20, 20) ? "Sim" : "Não");
    
    printf("\n=== TODOS OS TESTES CONCLUÍDOS ===\n");
    printf("Se não houve erros acima, suas estruturas estão funcionando!\n");
    
    return 0;
}