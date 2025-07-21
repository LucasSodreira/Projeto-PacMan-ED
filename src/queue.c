#include <stdio.h>
#include <stdlib.h>
#include "ghost.h"
#include "queue.h"

Queue* create_queue() {
    Queue* queue = (Queue*)malloc(sizeof(Queue));
    if (!queue) {
        printf("Erro: Não foi possível alocar memória para a fila\n");
        return NULL;
    }
    
    queue->front = NULL;
    queue->rear = NULL;
    queue->size = 0;
    return queue;
}

bool enqueue(Queue* queue, Ghost ghost) {
    if (!queue) {
        return false;
    }
    
    QueueNode* new_node = (QueueNode*)malloc(sizeof(QueueNode));
    if (!new_node) {
        printf("Erro Crítico: Falha ao alocar memória para novo nó da fila.\n");
        return false;
    }
    
    new_node->ghost = ghost;
    new_node->next = NULL;
    
    if (is_empty(queue)) {
        queue->front = new_node;
        queue->rear = new_node;
    } else {
        queue->rear->next = new_node;
        queue->rear = new_node;
    }
    
    queue->size++;
    return true;
}

Ghost dequeue(Queue* queue) {
    Ghost empty_ghost = {
        .pos = {-1, -1},
        .direction = NORTH,
        .ghost_id = -1,
        .symbol = ' ',
        .state = GHOST_NORMAL,
        .is_active = 0,
        .target = {-1, -1},
        .difficulty = DIFFICULTY_EASY,
        .scatter_mode = 0,
        .timer = 0
    };
    
    if (!queue || is_empty(queue)) {
        printf("Erro: Tentativa de dequeue em fila vazia\n");
        return empty_ghost;
    }
    
    QueueNode* temp = queue->front;
    Ghost ghost = temp->ghost;
    
    queue->front = queue->front->next;
    if (queue->front == NULL) {
        queue->rear = NULL;
    }
    
    free(temp);
    queue->size--;
    
    return ghost;
}

int is_empty(Queue* queue) {
    return (queue == NULL || queue->front == NULL);
}

void destroy_queue(Queue* queue) {
    if (!queue) return;
    // Libera todos os nós da fila
    QueueNode* current = queue->front;
    while (current) {
        QueueNode* next = current->next;
        free(current);
        current = next;
    }
    queue->front = NULL;
    queue->rear = NULL;
    queue->size = 0;
    free(queue);
}

void print_queue(Queue* queue) {
    if (!queue || is_empty(queue)) {
        printf("Fila vazia\n");
        return;
    }
    
    printf("Fila de fantasmas (tamanho: %d):\n", queue->size);
    QueueNode* current = queue->front;
    int position = 1;
    
    while (current) {
        Ghost g = current->ghost;
        printf("  %d. ID:%d Pos:(%d,%d) Dir:%d Symbol:'%c'\n", 
               position, g.ghost_id, g.pos.x, g.pos.y, g.direction, g.symbol);
        current = current->next;
        position++;
    }
    printf("\n");
}

int queue_size(Queue* queue) {
    if (!queue) return 0;
    return queue->size;
}

Ghost queue_peek(Queue* queue) {
    Ghost empty_ghost = {
        .pos = {-1, -1},
        .direction = NORTH,
        .ghost_id = -1,
        .symbol = ' ',
        .state = GHOST_NORMAL,
        .is_active = 0,
        .target = {-1, -1},
        .difficulty = DIFFICULTY_EASY,
        .scatter_mode = 0,
        .timer = 0
    };
    
    if (!queue || is_empty(queue)) {
        printf("Aviso: Tentativa de peek em fila vazia\n");
        return empty_ghost;
    }
    
    return queue->front->ghost;
}

int is_full(Queue* queue, int max_size) {
    if (!queue) return 1;
    return queue->size >= max_size;
}

void clear_queue(Queue* queue) {
    if (!queue) return;
    
    while (!is_empty(queue)) {
        dequeue(queue);
    }
    
    queue->front = NULL;
    queue->rear = NULL;
    queue->size = 0;
}

int find_ghost_in_queue(Queue* queue, int ghost_id) {
    if (!queue || is_empty(queue)) return 0;
    
    QueueNode* current = queue->front;
    int position = 1;
    
    while (current) {
        if (current->ghost.ghost_id == ghost_id) {
            return position;
        }
        current = current->next;
        position++;
    }
    
    return 0;
}

int count_active_ghosts(Queue* queue) {
    if (!queue || is_empty(queue)) return 0;
    
    int count = 0;
    QueueNode* current = queue->front;
    
    while (current) {
        if (current->ghost.is_active) {
            count++;
        }
        current = current->next;
    }
    
    return count;
}

Queue* clone_queue(Queue* source) {
    if (!source) return NULL;
    
    Queue* new_queue = create_queue();
    if (!new_queue) {
        return NULL;
    }
    
    QueueNode* current = source->front;
    while (current) {
        if (!enqueue(new_queue, current->ghost)) {
        }
        current = current->next;
    }
    
    return new_queue;
}

Ghost* queue_to_array(Queue* queue, int* array_size) {
    if (!queue || is_empty(queue) || !array_size) {
        if (array_size) *array_size = 0;
        return NULL;
    }
    
    *array_size = queue->size;
    Ghost* array = (Ghost*)malloc(sizeof(Ghost) * queue->size);
    if (!array) {
        printf("Erro: Não foi possível alocar memória para array\n");
        *array_size = 0;
        return NULL;
    }
    
    QueueNode* current = queue->front;
    int index = 0;
    
    while (current && index < queue->size) {
        array[index] = current->ghost;
        current = current->next;
        index++;
    }
    
    return array;
}