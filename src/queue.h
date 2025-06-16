#ifndef QUEUE_H
#define QUEUE_H

#include <stdbool.h>
#include "utils.h"
#include "ghost.h"

typedef struct QueueNode {
    Ghost ghost;
    struct QueueNode* next;
} QueueNode;

typedef struct {
    QueueNode* front;
    QueueNode* rear;
    int size;
} Queue;

Queue* create_queue();
bool enqueue(Queue* queue, Ghost ghost);
Ghost dequeue(Queue* queue);
int is_empty(Queue* queue);
void destroy_queue(Queue* queue);
void print_queue(Queue* queue);

int queue_size(Queue* queue);
Ghost queue_peek(Queue* queue);
int is_full(Queue* queue, int max_size);
void clear_queue(Queue* queue);
int find_ghost_in_queue(Queue* queue, int ghost_id);
int count_active_ghosts(Queue* queue);
Queue* clone_queue(Queue* source);
Ghost* queue_to_array(Queue* queue, int* array_size);

#endif