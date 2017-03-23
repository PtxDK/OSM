#ifndef STACK_H
#define STACK_H

typedef struct list_t {
  int number;
  struct list_t* next; /* linked list */
} list_t;

typedef struct stack_t {
  list_t* start; /* er NULL hvis stakken er tom */
} stack_t;

/* Lav ny, tom stak. */
stack_t stack_new();

/* Tilføj et element til toppen af stakken. */
void stack_add(stack_t* stack, int number);

/* Fjern det øverste element af stakken. */
void stack_remove(stack_t* stack);

/* Returnér det øverste element af stakken. */
int stack_get(stack_t stack);

/* Befri al dynamisk hukommelse i stakken, dvs. alle 'list_t'-elementer. */
void stack_free(stack_t stack);

#endif
