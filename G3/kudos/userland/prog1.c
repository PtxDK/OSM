#include "lib.h"

int main(void) {

	usr_sem_t* sem = syscall_usr_sem_open("lol", 0);
	syscall_spawn("[disk]prog2.mips32", NULL);

	printf ("foo (1)%d\n", 42);
	syscall_usr_sem_procure(sem); // print 'bar'
	printf ("baz (3)%d\n", 42);

	while(1);

  syscall_halt();
  return 0;
}
