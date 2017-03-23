#ifndef QUEUE_H
#define QUEUE_H

typedef struct queue {
  int *items;
  int item_cnt;
  int capacity;
}queue;

//Initialize the queue before usage
int queue_init(struct queue *queue);

//Insert an element as a priority
int queue_push(struct queue *queue, int pri);

//Pop the maximum priority out of the queue
int queue_pop(struct queue *queue, int *pri_ptr);

//Destroy the queue after usage
int queue_destroy(struct queue *queue);


#endif //QUEUE_H
