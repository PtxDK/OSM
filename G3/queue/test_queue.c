#include <stdio.h>
#include <assert.h>
#include <pthread.h>
#include "queue.h"

#define NUM_THREADS 4

static struct queue queue;

void *push_thread(void *arg) {

  printf("%s starting \n", (char *) arg);
  queue_push(&queue, 2);
  queue_push(&queue, 2);
  queue_push(&queue, 2);
  printf("%s ending \n", (char *) arg);
  return NULL;
}

void *pop_thread(void *arg) {
  int pri = 0;

  printf("%s starting \n", (char *) arg);
  queue_pop(&queue, &pri);
  queue_pop(&queue, &pri);
  queue_pop(&queue, &pri);
  printf("%s ending \n", (char *) arg);
  return NULL;
}

int main() {
  queue_init(&queue);
  pthread_t threads[NUM_THREADS];
  int check;
  printf("main: begin\n");
  check = pthread_create(&threads[0], NULL, push_thread, "thread 0"); assert(check == 0);
  check = pthread_create(&threads[1], NULL, pop_thread, "thread 1"); assert(check == 0);
  check = pthread_create(&threads[2], NULL, push_thread, "thread 2"); assert(check == 0);
  check = pthread_create(&threads[3], NULL, pop_thread, "thread 3"); assert(check == 0);
  // join waits for the threads to finish
  printf("join stuff\n");
  for (int i = 0; i < NUM_THREADS; i++) {
    check = pthread_join(threads[i], NULL); assert(check == 0);
  }
  printf("main: end\n");
  return 0;
}
