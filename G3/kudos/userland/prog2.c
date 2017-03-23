#include "lib.h"

int main(void) {

	// Refers to the existing semaphore
  usr_sem_t* sem = syscall_usr_sem_open("lol", -1);

  printf("bar (2) \n%d", 42);

  syscall_usr_sem_vacate(sem);

  return 0;
}
