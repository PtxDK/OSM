/*
 * Process startup.
 */

#ifndef KUDOS_PROC_PROCESS
#define KUDOS_PROC_PROCESS

#include "lib/types.h"
#include "vm/memory.h"

#define PROCESS_PTABLE_FULL  -1
#define PROCESS_ILLEGAL_JOIN -2

#define PROCESS_MAX_FILELENGTH 256
#define PROCESS_MAX_PROCESSES  128
#define PROCESS_MAX_FILES      10

typedef int process_id_t;

typedef enum state{
  STATE_FREE,
  STATE_RUNNING,
  STATE_ZOMBIE,
}state;

typedef struct process_control_block_t{
  state state;
  process_id_t process_id;
  int Thread_ID;
  int retval;
  virtaddr_t entry_point;
  virtaddr_t stack_top;

} process_control_block_t;

void process_init();

int thread_spawn(char const* filename);

void thread_exit(int retval);

int thread_join(int pid);

#endif
