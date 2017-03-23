#include <stdlib.h>
#include <stdio.h>
#include "stack.h"

int main(int argc, char* argv[]) {
  stack_t stack = stack_new();
  int c;
  char c0;

  while ((c = getc(stdin)) != EOF) {
    c0 = (char) c;
    if (c0 == '+') {
      /* Pop to tal fra stakken, læg dem sammen, og læg resultatet tilbage i
       * stakken.
       */
      int x;
      int y;

      x = stack_get(stack);
      stack_remove(&stack);
      y = stack_get(stack);
      stack_remove(&stack);

      stack_add(&stack, x + y);
    }

    /* TODO: Lav flere operatorer. */
    else if (c0 == '=') {
      int top = stack_get(stack);
      printf("%d\n", top);
    }
    else {
      int number;
      ungetc(c, stdin);
      /* TODO: Tjek for scanf-fejl.  Kig i man-siden. */
      scanf("%d", &number);
      stack_add(&stack, number);
    }

    /* Bliv ved med at læse til og med linjens newline. */
    while (getc(stdin) != '\n');
  }
}
