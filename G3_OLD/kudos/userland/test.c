#include "lib.h"

int main(void) {

  syscall_usr_sem_open("lol", 0);

  syscall_halt();

  return 0;
}
