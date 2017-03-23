#ifndef KUDOS_PROC_USR_SEM
#define KUDOS_PROC_USR_SEM

#include "lib/types.h"
#include "vm/memory.h"
#include "kernel/spinlock.h"
#include "kernel/thread.h"
#include "kernel/semaphore.h"

#define MAX_USR_SEM 20

typedef struct {
  spinlock_t slock;
  semaphore_t* semaphore;
  int value;
  char name[32];
  int inuse;
} usr_sem_t;

//inits the sem_table setting all names to "not_in_use"
void usr_sem_init(void);

// Return a handle to a userland semaphore identified by the string name,
// which can then be used in the future to refer to this semaphore.
usr_sem_t* usr_sem_open(const char* name, int value);

// Try to remove/delete a semaphore
int usr_sem_destroy(usr_sem_t* sem);

// Execute the P operation on the userland semaphore sem
int usr_sem_procure(usr_sem_t* sem);

// Execute the P operation on the userland semaphore sem
int usr_sem_vacate(usr_sem_t* sem);

#endif /* KUDOS_PROC_USR_SEM_H */
