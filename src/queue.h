#ifndef QUEUE_H
#define QUEUE_H
 
#include "ghost.h"
#include "utils.h"  // Inclui as definições de Position e Ghost

// Nó da fila
typedef struct QueueNode {
    Ghost ghost;
    struct QueueNode* next;
} QueueNode;

// Estrutura da fila
typedef struct {
    QueueNode* front;
    QueueNode* rear;
    int size;
} Queue;

// Declarações das funções existentes
Queue* create_queue();
bool enqueue(Queue* queue, Ghost ghost); // Retorna bool para indicar sucesso/falha
Ghost dequeue(Queue* queue);
int is_empty(Queue* queue);
void destroy_queue(Queue* queue);
void print_queue(Queue* queue);

// ===== FUNÇÕES ADICIONAIS =====

// Obter o tamanho atual da fila
int queue_size(Queue* queue);

// Espiar o primeiro elemento sem removê-lo
Ghost queue_peek(Queue* queue);

// Verificar se a fila está cheia (com limite máximo)
int is_full(Queue* queue, int max_size);

// Limpar todos os elementos da fila sem destruí-la
void clear_queue(Queue* queue);

// Buscar fantasma por ID na fila
int find_ghost_in_queue(Queue* queue, int ghost_id);

// Contar fantasmas ativos na fila
int count_active_ghosts(Queue* queue);

// Clonar/copiar uma fila
Queue* clone_queue(Queue* source);

// Converter fila para array (para debug/análise)
Ghost* queue_to_array(Queue* queue, int* array_size);

#endif // QUEUE_H