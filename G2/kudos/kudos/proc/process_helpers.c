#include <arch.h>
#include "proc/process_helpers.h"
#include "proc/elf.h"
#include "kernel/thread.h"
#include "kernel/assert.h"
#include "kernel/interrupt.h"
#include "kernel/config.h"
#include "kernel/spinlock.h"
#include "fs/vfs.h"
#include "kernel/sleepq.h"
#include "vm/memory.h"
#include "proc/process.h"



spinlock_t process_table_slock;
extern void process_set_pagetable(pagetable_t*);
extern process_control_block_t process_table[PROCESS_MAX_PROCESSES];
extern spinlock_t process_table_slock;

/* Sets context for process */
void process_run(process_id_t pid) {
  context_t user_context;

  kprintf("vi er nu i process run\n");
  
  process_set_pagetable(thread_get_thread_entry(thread_get_current_thread())->pagetable);

  /* Initialize the user context. (Status register is handled by
     thread_goto_userland) */
  memoryset(&user_context, 0, sizeof(user_context));
  
  _context_set_ip(&user_context, process_table[pid].entry_point);
  _context_set_sp(&user_context, process_table[pid].stack_top);


  kprintf("lige før thread goto");

  thread_goto_userland(&user_context);
}

//Load and run the executable as a new process in a new thread
//  Argument: executable file name; Returns: process ID of the new process
process_id_t process_spawn(char const* executable, char const **argv){

  // Initialise 'global' variables
  interrupt_status_t intr_status;
  TID_t my_thread;
  process_id_t pid;
  int ret;
  pid = 0;

  kprintf("vi er nu i spawn jaaaaa \n");
  //stop disables
  intr_status = _interrupt_disable();
  //spinlock
  spinlock_acquire(&process_table_slock);

  //Find empty spot
  for ( int i = 0; i < PROCESS_MAX_PROCESSES; i++) {
    if (process_table[i].state == STATE_FREE) {
      pid = i;
      break;
    }
  }

  kprintf("vi er nu længere i spawn wuuhuu\n");
  //Spawn new thread in 'process run'
  my_thread = thread_create((void (*)(uint32_t))(&process_run), pid);
  process_table[pid].Thread_ID = my_thread;

  kprintf("fejler spawn ved checket? %d\n",my_thread);
  // Check if thread has been created
  if (!(my_thread >= 0)) {
    kprintf("Her er fejl  spawn my threads er for lille");
    return -1;
  }


  kprintf("lige før setup new process. pid: %d\n",pid);
  kprintf("lige før setup new process. my_thread: %d\n",my_thread);
  kprintf("lige før setup new process. entry_point: %d\n",process_table[pid].entry_point);
  // Attempt to start new process
  ret = setup_new_process(
                          my_thread,
                          executable,
                          argv,
                          &process_table[pid].entry_point,
                          &process_table[pid].stack_top
                          );
  if (ret < 0){
    kprintf("fejl i setup_new_process ret < 0 ret: %d\n",ret);
    return -1;
  }

  /* Unlock the process table */
  spinlock_release(&process_table_slock);
  //enable interrupts
  _interrupt_set_state(intr_status);


  kprintf("retval ved slut af spawn: %d\n",ret);
   
  thread_run(my_thread);

  return pid;
}


/* Return PID of current process. */
process_id_t process_get_current_process(void){
  TID_t my_thread;
  process_id_t res;
  // gets the current thread it
  my_thread = thread_get_current_thread();

  /* checks for every process if the Thread_ID is the same as the current
     Thread id, if it is we set res to the process id. */
  for (int i=0; i < PROCESS_MAX_PROCESSES; i++) {
    kprintf("i: %d\n",i);
    kprintf("my_thread: %d \n",my_thread);
    kprintf("process_table[i].thread: %d \n",process_table[i].Thread_ID);
    if (process_table[i].Thread_ID == my_thread){
      res = process_table[i].process_id;
      kprintf("process_get_current_process pid = %d",res);
      return res;
    }
  }
  kprintf("fejl i get current process returner -1\n");
  return -1;
}

/* Wait for the given process to terminate, return its return value,
   and mark the process-table entry as free */
int process_join(process_id_t pid){
  kprintf("Starten af process join pid = %d\n",pid);
  //Check if PID is valid
  if ((pid >= 0) && (pid < PROCESS_MAX_PROCESSES)) {
    interrupt_status_t intr_status;
    uint32_t retval;

    intr_status = _interrupt_disable();
    spinlock_acquire(&process_table_slock);

    /* Wait for the child process to exit and become a zombie. */
    while (process_table[pid].state != STATE_ZOMBIE) {
      /* Move to sleep queue and switch to another thread. */
      sleepq_add(&process_table[pid]);
      spinlock_release(&process_table_slock);
      thread_switch();
      spinlock_acquire(&process_table_slock);
    }

    /* Get the return value and prepare its slot for a future process. */
    retval = process_table[pid].retval;
    process_table[pid].state = STATE_FREE;

      //Release the respurce spinlock
    spinlock_release(&process_table_slock);
    //Restores the interrupt mask
    _interrupt_set_state(intr_status);

    return retval;
  }
  kprintf("Fejl i process join pid < 0 %d\n",pid);
  return -1;
}


/* Stop the current process and the kernel thread in which it runs
   Argument: return value */
void process_exit(int retval){
  process_id_t pid;
  thread_table_t * thr;

  kprintf("starten af exit. reval: %d \n",retval);
  if (retval < 0){
    return;
  }

  // gets the process id
  pid = process_get_current_process();

  thr = thread_get_current_thread_entry();

  //spinlock
  spinlock_acquire(&process_table_slock);

  //Disbale interrupt
  _interrupt_set_state(_interrupt_disable());

  // set the process state to ZOMBIE
  process_table[pid].state = STATE_ZOMBIE;

  // Set the process retval to given retval  
  process_table[pid].retval = retval;

  //cleanup
  vm_destroy_pagetable(thr->pagetable);
  thr->pagetable = NULL;

  /* Wake process */
  sleepq_wake_all(&process_table[pid]);

  /* Unlock the process table */
  spinlock_release(&process_table_slock);
  
   //enable interrupts
  _interrupt_set_state(_interrupt_disable());

  kprintf("slutningen af exit :O \n");

  thread_finish();
}
