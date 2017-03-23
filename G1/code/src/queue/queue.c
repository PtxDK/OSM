#include <stdlib.h>
#include <stdio.h>
#include "queue.h"

//Initialize the queue before usage
int queue_init(struct queue *queue){
  queue->item_cnt = 0;
  queue->items = NULL;
  queue->capacity = 0;
}

// Insert an element into the priority queue
int queue_push(struct queue *queue, int pri){

  // Increase mamory capacity if needed
  if (queue->item_cnt == queue->capacity){
    queue->capacity = (queue->capacity + 8);

    // Allocate new memory
    queue->items = realloc(queue->items,sizeof(int)* queue->capacity);

    // Checks if items has been made correctly
    if (queue->items == NULL){
      return 1;
    }
  }

  // Run Find the right place in items list and place pri there
  for (int i=0; i <= queue->item_cnt-1; i++) {
    if (pri < queue->items[i]) {
      
      // Loops backwards on the array to move everything one to the right
      for (int j=queue->item_cnt-1; j>=i; j--) {
        queue->items[j+1] = queue->items[j];
      }

      // Sets given element to the right place in items list
      queue->items[i] = pri;
      break;

    } else if (pri >= queue->items[queue->item_cnt-1]) {

    	// Sets given element to be the last element in items list
      queue->items[queue->item_cnt] = pri;
      
      break;
    }
  }


  // If array is empty, set pri as first element
  if (queue->item_cnt == 0) {
  	queue->items[0] = pri;
  }  


  // Add one to item count
  queue->item_cnt++;

  return 0;
}

//Pop the maximum priority out of the queue and make a new max priority item
int queue_pop(struct queue *queue, int *pri_ptr) {
  if (queue->item_cnt == 0) {
    return 2;
  }

  // We are now done and have an array which is 1 smaller
  queue->item_cnt = queue->item_cnt-1;

  // Send the biggest element out to pri_ptr
  *pri_ptr = queue->items[queue->item_cnt];

  // Make capacity smaller if needed
  if (queue->item_cnt < (queue->capacity - 8)) {
    queue->capacity = (queue->capacity - 8);
    // Allocate new memory
    queue->items = realloc(queue->items,sizeof(void *)* queue->capacity);

    // Check if items exist
    if (queue->items == NULL){
      return 1;
    }
  }
  return 0;
}

//Destroy the queue after usage
int queue_destroy(struct queue *queue){
  free(queue->items);
  return 0;
}