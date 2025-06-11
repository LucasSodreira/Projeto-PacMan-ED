#include "../src/utils.h"
#include "../src/queue.h"
#include <assert.h>
#include <time.h>

void test_advanced_queue_functions();
void test_utility_functions();
void test_direction_functions();
void test_position_functions();
void test_validation_functions();
void test_conversion_functions();

int main() {
    printf("=== TESTE AVANÇADO DAS ESTRUTURAS ===\n\n");
    
    // Inicializar semente aleatória
    srand((unsigned)time(NULL));
    
    // Executar todos os testes
    test_advanced_queue_functions();
    test_utility_functions();
    test_direction_functions();
    test_position_functions();
    test_validation_functions();
    test_conversion_functions();
    
    printf("\n=== TODOS OS TESTES AVANÇADOS CONCLUÍDOS ===\n");
    printf("Se não houve erros, todas as funcionalidades estão funcionando!\n");
    
    return 0;
}

void test_advanced_queue_functions() {
    printf("1. Testando funções avançadas da fila...\n");
    
    Queue* queue = create_queue();
    assert(queue != NULL);
    
    // Teste de fila vazia
    assert(queue_size(queue) == 0);
    assert(is_empty(queue) == 1);
    assert(count_active_ghosts(queue) == 0);
    
    // Criar fantasmas de teste
    Ghost ghost1 = {{5, 5}, NORTH, 1, 'F', 1};
    Ghost ghost2 = {{10, 10}, EAST, 2, 'G', 0};  // Inativo
    Ghost ghost3 = {{15, 15}, SOUTH, 3, 'B', 1};
    
    // Adicionar fantasmas
    enqueue(queue, ghost1);
    enqueue(queue, ghost2);
    enqueue(queue, ghost3);
    
    // Testar funções de consulta
    assert(queue_size(queue) == 3);
    assert(!is_empty(queue));
    assert(count_active_ghosts(queue) == 2); // Apenas ghost1 e ghost3 são ativos
    
    // Testar peek
    Ghost first = queue_peek(queue);
    assert(first.ghost_id == 1);
    assert(queue_size(queue) == 3); // Não deve remover
    
    // Testar busca
    assert(find_ghost_in_queue(queue, 2) == 2); // Ghost2 está na posição 2
    assert(find_ghost_in_queue(queue, 999) == 0); // Não existe
    
    // Testar is_full
    assert(!is_full(queue, 5));
    assert(is_full(queue, 3));
    assert(is_full(queue, 2));
    
    // Testar clonagem
    Queue* cloned = clone_queue(queue);
    assert(cloned != NULL);
    assert(queue_size(cloned) == queue_size(queue));
    
    // Testar conversão para array
    int array_size;
    Ghost* array = queue_to_array(queue, &array_size);
    assert(array != NULL);
    assert(array_size == 3);
    assert(array[0].ghost_id == 1);
    assert(array[2].ghost_id == 3);
    free(array);
    
    // Testar clear
    clear_queue(queue);
    assert(queue_size(queue) == 0);
    assert(is_empty(queue));
    
    // Limpeza
    destroy_queue(queue);
    destroy_queue(cloned);
    
    printf("✅ Funções avançadas da fila funcionando!\n");
}

void test_utility_functions() {
    printf("2. Testando funções utilitárias...\n");
    
    // Testar random_range
    for (int i = 0; i < 10; i++) {
        int rand_num = random_range(1, 5);
        assert(rand_num >= 1 && rand_num <= 5);
    }
    
    // Testar format_time
    char time_buffer[10];
    format_time(time_buffer, 125); // 2:05
    // printf("Tempo formatado: %s\n", time_buffer);
    
    // Testar random_direction
    Direction rand_dir = random_direction();
    assert(rand_dir >= NORTH && rand_dir <= WEST);
    
    printf("✅ Funções utilitárias funcionando!\n");
}

void test_direction_functions() {
    printf("3. Testando funções de direção...\n");
    
    // Testar validação de direção
    assert(is_valid_direction(NORTH));
    assert(is_valid_direction(WEST));
    assert(!is_valid_direction(-1));
    assert(!is_valid_direction(4));
    
    // Testar conversão para string
    assert(strcmp(direction_to_string(NORTH), "NORTE") == 0);
    assert(strcmp(direction_to_string(EAST), "LESTE") == 0);
    
    // Testar conversão de string
    assert(string_to_direction("NORTE") == NORTH);
    assert(string_to_direction("SUL") == SOUTH);
    assert(string_to_direction("INVALIDO") == NORTH); // Default
    
    // Testar direção oposta
    assert(get_opposite_direction(NORTH) == SOUTH);
    assert(get_opposite_direction(EAST) == WEST);
    
    printf("✅ Funções de direção funcionando!\n");
}

void test_position_functions() {
    printf("4. Testando funções de posição...\n");
    
    Position pos1 = {5, 5};
    Position pos2 = {8, 9};
    Position pos3 = {5, 5};
    
    // Testar distância Manhattan
    int distance = manhattan_distance(pos1, pos2);
    assert(distance == 7); // |5-8| + |5-9| = 3 + 4 = 7
    
    // Testar igualdade de posições
    assert(positions_equal(pos1, pos3));
    assert(!positions_equal(pos1, pos2));
    
    // Testar próxima posição
    Position next_north = get_next_position(pos1, NORTH);
    assert(next_north.x == 5 && next_north.y == 4);
    
    Position next_east = get_next_position(pos1, EAST);
    assert(next_east.x == 6 && next_east.y == 5);
    
    printf("✅ Funções de posição funcionando!\n");
}

void test_validation_functions() {
    printf("5. Testando funções de validação...\n");
    
    // Testar validação de ID de fantasma
    assert(is_valid_ghost_id(1));
    assert(is_valid_ghost_id(MAX_GHOSTS));
    assert(!is_valid_ghost_id(0));
    assert(!is_valid_ghost_id(MAX_GHOSTS + 1));
    
    // Testar validação de símbolo de fantasma
    assert(is_valid_ghost_symbol('F'));
    assert(is_valid_ghost_symbol('G'));
    assert(is_valid_ghost_symbol('B'));
    assert(is_valid_ghost_symbol('R'));
    assert(!is_valid_ghost_symbol('P'));
    assert(!is_valid_ghost_symbol('X'));
    
    printf("✅ Funções de validação funcionando!\n");
}

void test_conversion_functions() {
    printf("6. Testando funções de conversão...\n");
    
    // Testar conversão de status do jogo
    assert(strcmp(game_status_to_string(PLAYING), "Jogando") == 0);
    assert(strcmp(game_status_to_string(GAME_OVER), "Game Over") == 0);
    assert(strcmp(game_status_to_string(VICTORY), "Vitória") == 0);
    assert(strcmp(game_status_to_string(PAUSED), "Pausado") == 0);
    
    printf("✅ Funções de conversão funcionando!\n");
}
