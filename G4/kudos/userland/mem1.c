
#include "lib.h"

#define STRING_LENGTH 12
#define STRING_SOURCE "Nice string\n"
#define STRING_LENGTH2 14
#define STRING_SOURCE2 "Shitty string\n"

int main() {
  char* a_string;

  /* Find the current (initial) heap end. */
  a_string = syscall_memlimit(NULL);

  /* Extend the heap, and check that it worked. */
  if (syscall_memlimit(a_string + STRING_LENGTH) == NULL) {
    printf("virker ikke \n ");
    /* It didn't work, so exit already. */
    return 1;
  }

  /* Copy the source string to the dynamically allocated memory. */
  for (size_t i = 0; i < STRING_LENGTH; i++) {
    a_string[i] = STRING_SOURCE[i];
  }

  /* Write from the dynamically allocated memory. */
  syscall_write(1, a_string, STRING_LENGTH);


  
  // we do it again to test multiple memlimits
  
    /* Find the current (initial) heap end. */
  a_string = syscall_memlimit(NULL);

  /* Extend the heap, and check that it worked. */
  if (syscall_memlimit(a_string + STRING_LENGTH2) == NULL) {
    printf("virker ikke \n ");
    /* It didn't work, so exit already. */
    return 1;
  }

  /* Copy the source string to the dynamically allocated memory. */
  for (size_t i = 0; i < STRING_LENGTH2; i++) {
    a_string[i] = STRING_SOURCE2[i];
  }

  /* Write from the dynamically allocated memory. */
  syscall_write(1, a_string, STRING_LENGTH2);

  return 0;
}
