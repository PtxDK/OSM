#include <stdlib.h>
#include "stack.h"

stack_t stack_new() {
  stack_t stack;
  stack.start = NULL;
  return stack;
}

void stack_add(stack_t* stack, int number) {
  list_t* elem = (list_t*) malloc(sizeof(list_t));
  if (elem == NULL) {
    /* TODO: Gør det her godt i stedet.  Det er *virkelig* dårlig stil
     * at kalde exit fra en biblioteksfunktion.  Find i stedet en måde at
     * signallere fejl på, fx ved at ændre returtypen fra 'void' til 'bool'
     * og returnere 'true' for succes og 'false' for fejl.
     */
    exit(EXIT_FAILURE);
  }
  else {
    elem->number = number;
    elem->next = stack->start;
    stack->start = elem;
  }
}

void stack_remove(stack_t* stack) {
  if (stack->start == NULL) {
    /* TODO: Gør også det her bedre. */
    exit(EXIT_FAILURE);
  }
  else {
    free(stack->start);
    stack->start = stack->start->next;
  }
}

int stack_get(stack_t stack) {
  if (stack.start == NULL) {
    /* TODO: Gør også det her bedre. */
    exit(EXIT_FAILURE);
  }
  else {
    return stack.start->number;
  }
}

void stack_free(stack_t stack) {
  /* TODO: Implementér.  Funktionen skal blive ved med at kalde 'stack_remove'
   * indtil der ikke er flere elementer, dvs. indtil alle elementer er blevet
   * befriet.
   */
}
