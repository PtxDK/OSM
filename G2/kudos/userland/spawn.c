/*
 * Test that spawn works to the extent needed (we must be able to spawn new
 * programs, but we don't have to keep track of them..
 */

#include "lib.h"

int main(void) {
  puts("Spawning one program...\n");
  int PID = syscall_spawn("[disk]exit.mips32", NULL);

  int DONE = syscall_join(PID);

  printf("Process returned: %d\n", DONE);

  syscall_halt();


  return 0;
}
