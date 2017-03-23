#include <arch.h>
#include <stddef.h>             // NULL, comes from GCC.
#include "proc/usr_sem.h"
#include "proc/elf.h"
#include "kernel/thread.h"
#include "kernel/assert.h"
#include "kernel/interrupt.h"
#include "kernel/config.h"
#include "kernel/assert.h"      // KERNEL_ASSERT
#include "kernel/semaphore.h"
#include "fs/vfs.h"
#include "kernel/sleepq.h"
#include "vm/memory.h"
#include "drivers/device.h"     // device_*
#include "drivers/gcd.h"        // gcd_*
#include "proc/syscall.h"       // FILEHANDLE_*

// Creates the usr_sem_table to hold our usr_sem struct.
// MAX_USR_SEM can be found in usr_sem.h
usr_sem_t usr_sem_table[MAX_USR_SEM];
// Creates the usr_sem__table spinlock
static spinlock_t usr_sem_table_slock;


// Inits the sem_table by setting all entries to 'inuse = 0'
void usr_sem_init(void) {
  
  interrupt_status_t intr_status = _interrupt_disable();
  spinlock_acquire(&usr_sem_table_slock);
  
  for (int i= 0; i < MAX_USR_SEM; i++){
    usr_sem_table[i].inuse = 0;
  }
  
  _interrupt_set_state(intr_status);
  spinlock_release(&usr_sem_table_slock);
}

/* Return a handle to a userland semaphore identified by the string name
** Which can then be used in the future to refer to this semaphore.
*/
usr_sem_t* usr_sem_open(const char* name, int value) {

  interrupt_status_t intr_status = _interrupt_disable();
  spinlock_acquire(&usr_sem_table_slock);

  if (value >= 0) {
    for (int i = 0; i < MAX_USR_SEM; i++) {

      // Checks that the name of the sem is not taken.
      if (stringcmp(usr_sem_table[i].name, name)) {
        spinlock_release(&usr_sem_table_slock);
        _interrupt_set_state(intr_status);
        return NULL;
      }

      // Creates the sem as the first sem that is not in use.
      if (usr_sem_table[i].inuse == 0) {
        stringcopy(usr_sem_table[i].name, (char*)name, 32);
        usr_sem_table[i].value = value;
        usr_sem_table[i].semaphore = semaphore_create(value);
        usr_sem_table[i].inuse = 1;
        spinlock_release(&usr_sem_table_slock);
        _interrupt_set_state(intr_status);
        return &usr_sem_table[i];
      }
    }
  }

  if (value < 0){

    for (int i = 0; i < MAX_USR_SEM; i++) {
      if (stringcmp(usr_sem_table[i].name, name)) {
        spinlock_release(&usr_sem_table_slock);
        _interrupt_set_state(intr_status);
        return &usr_sem_table[i];
      }
    }

    spinlock_release(&usr_sem_table_slock);
    _interrupt_set_state(intr_status);
    return NULL;
  }
  
  spinlock_release(&usr_sem_table_slock);
  _interrupt_set_state(intr_status);
  return NULL;
}

// Try to remove/delete a semaphorez (a wrapper)
int usr_sem_destroy(usr_sem_t* sem) {

  // Check if input is insane
  if (sem->semaphore->value < 0) {
    return -2;
  }

  // Wrapin some destruction
  semaphore_destroy(sem->semaphore);

  return 0;
}

// Decreases the semaphore
// Execute the P operation on the userland semaphore sem (a wrapper)
int usr_sem_procure(usr_sem_t* sem) {

  for (int i = 0; i <= MAX_USR_SEM ;i++) {
    if (stringcmp(usr_sem_table[i].name, sem->name)) {
      semaphore_P(sem->semaphore);
      return 0;
    }
  }

  return -3;
}

// Increases the semaphore
// Execute the P operation on the userland semaphore sem (a wrapper)
int usr_sem_vacate(usr_sem_t* sem) {

  for (int i = 0; i <= MAX_USR_SEM ;i++) {
    if (stringcmp(usr_sem_table[i].name, sem->name)) {
      semaphore_P(sem->semaphore);
      return 0;
    }
  }

  semaphore_V(sem->semaphore);
  return 0;
}

