/*
 * System calls.
 */
#include <cswitch.h>
#include "proc/syscall.h"
#include "kernel/halt.h"
#include "kernel/panic.h"
#include "lib/libc.h"
#include "kernel/assert.h"
#include "vm/memory.h"
#include "drivers/polltty.h"
#include "proc/process.h"
#include "fs/vfs.c"




int syscall_write(const char *buffer, int length) {
  /* Not a G1 solution! */
  for (int i = 0; i < length; i++, *buffer++) polltty_putchar(*buffer);
  return length;
}

int syscall_read(char *buffer) {
  /* Not a G1 solution! */
  *buffer = polltty_getchar();
  return 1;
}


int syscall_open(const char *pathname){
  openfile_t file;
  file = vfs_open(pathname);
  vfs_verify_open(file);
  //something something
}

int syscall_close(int filehandle){
  openfile_t file;
  process_id_t pid = process_get_current_process();
  vfs_verify_open(process_table[pid].file_table[filehandle]);
  file = process_table[pid].file_table[filehandle];
  process_table[pid].file_table[filehandle] = -1;
  int ret = vfs_close(file);
  return ret;
}

int syscall_create(const char *pathname, int size){
  int ret = vfs_create(pathname, size);
  return ret;
}

int syscall_delete(const char *pathname){
  int ret = vfs_remove(pathname);
  return ret;
}

int syscall_seek(int filehandle, int offset){
  openfile_t file = process_table.file_table[filehandle];
  vfs_verify_open(file);
  int ret =vfs_seek(file,offset);
  return ret;
}

int syscall_filecount(const char *pathname){
  int ret = (int)vfs_filecount(pathname);
  return ret;
}

int syscall_file(const char *pathname, int nth, char *buffer){
  int ret = vfs_file(pathname,nth,buffer);
  return ret;
}

/**
 * Handle system calls. Interrupts are enabled when this function is
 * called.
 */
uintptr_t syscall_entry(uintptr_t syscall,
                        uintptr_t arg0, uintptr_t arg1, uintptr_t arg2)
{
  arg0 = arg0;
  arg1 = arg1;
  arg2 = arg2;
  /* When a syscall is executed in userland, register a0 contains
   * the number of the syscall. Registers a1, a2 and a3 contain the
   * arguments of the syscall. The userland code expects that after
   * returning from the syscall instruction the return value of the
   * syscall is found in register v0. Before entering this function
   * the userland context has been saved to user_context and after
   * returning from this function the userland context will be
   * restored from user_context.
   */
  switch(syscall) {
  case SYSCALL_HALT:
    halt_kernel();
    break;
  case SYSCALL_READ:
    return syscall_read((void*)arg1);
    break;
  case SYSCALL_WRITE:
    return syscall_write((const void*)arg1, (int)arg2);
    break;
  case SYSCALL_SPAWN:
    return process_spawn((char*) arg0, (char const**) arg1);
    break;
  case SYSCALL_EXIT:
    process_exit((process_id_t) arg0);
    break;
  case SYSCALL_JOIN:
    return process_join((process_id_t) arg0);
    break;
  case SYSCALL_OPEN:
    return syscall_open(/*noget her*/);
    break;
  case SYSCALL_CLOSE:
    return syscall_close(/*noget her*/);
    break;
  case SYSCALL_CREATE:
    return syscall_create(/*noget her*/);
    break;
  case SYSCALL_DELETE:
    return syscall_delete(/*noget her*/);
    break;
  case SYSCALL_SEEK:
    return syscall_seek(/*noget her*/);
    break;
  case SYSCALL_FILECOUNT:
    return syscall_filecount(/*noget her*/);
    break;
  case SYSCALL_FILE:
    return syscall_file(/*noget her*/);
    break;
  default:
    KERNEL_PANIC("Unhandled system call\n");
  }

  return 0;
}
