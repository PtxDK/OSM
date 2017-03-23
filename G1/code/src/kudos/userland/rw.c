/*
 * syscall_read/syscall_write test program
 *
 * Uses library implementation of printf instead of using syscall_write
 * directly.
 */

#include "lib.h"
#include "../kudos/proc/syscall.h"

#define LENGTH (64)

int main(void) {
  char to_out[] = "This is to stdout.\n";
  char to_err[] = "This is to stderr.\n";
  //char to_file[] = "This is to file.\n";
  char buffer[LENGTH];
  int retval;

  // Positive tests.

  printf("I am KUDOS userland, who are you?\n");
  retval = syscall_read(FILEHANDLE_STDIN, &buffer, LENGTH);
  if (retval < 0) {
    printf("syscall_read failed with %d :-(\n", retval);
    syscall_halt();
  }
  printf("Hello, %s.\n", buffer);

  retval = syscall_write(FILEHANDLE_STDOUT, to_out, sizeof(to_out) - 1);
  if (retval < 0) {
    printf("syscall_write to stdout failed with %d :-(\n", retval);
    syscall_halt();
  }

  retval = syscall_write(FILEHANDLE_STDERR, to_err, sizeof(to_err) - 1);
  if (retval < 0) {
    printf("syscall_write to stderr failed with %d :-(\n", retval);
    syscall_halt();
  }
  
  syscall_halt();
}
