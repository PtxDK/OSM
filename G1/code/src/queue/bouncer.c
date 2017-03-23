#include <stdio.h>  // printf, scanf, stdin
#include <ctype.h>  // isspace
#include "queue.h"

queue Queue;

static inline char
skip_spaces() {
  char c;

  do {
    c = getc(stdin);
  } while (isspace(c));

  return c;
}

static inline void
skip_until_space() {
  char c;

  do {
    c = getc(stdin);
  } while (!(isspace(c) || c == EOF));
}

void
loop() {
  char op = 0;
  int pri = 0;

  while(1) {

    op = skip_spaces();

    switch (op) {
    case EOF:
      return;
      break;



    case 'p':
      //Pop an element off of the queue.
      if (queue_pop(&Queue, &pri) == 2) {
        printf("!! Queue underflow.\n");
      } else {
        printf("=> %d\n", pri);
      }
      break;


    default:
      ungetc(op, stdin);
      if (scanf("%d", &pri) == 1) {
        //Insert the read priority into the queue.
        if (queue_push(&Queue, pri) == 1) {
          printf("!! Queue overflow.\n");
        }
      } else {
        skip_until_space();
        printf("Invalid input!\n");
      }
    }
  }
}

void
shutdown() {
  // Pop everything off of the queue.
  int dummy = 0;
  while (Queue.item_cnt != 0) {
    queue_pop(&Queue, &dummy);
  }
}

int
main() {
  // Initialize the queue.
  queue_init(&Queue);

  loop();

  shutdown();

  //Destroy the queue.
  queue_destroy(&Queue);

  return 0;
}
