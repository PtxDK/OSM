#include <cswitch.h>
#include "proc/syscall.h"
#include "write.h"
#include "drivers/device.h"
#include "drivers/gcd.h"
#include "drivers/metadev.h"
#include "kernel/assert.h"
#include "drivers/polltty.h"
#include "lib/libc.h"
#include "fs/vfs.h"
#include "vm/memory.h"
#include "proc/process.h"

int kernel_read(int filehandle, void *buffer, int length){
  device_t *dev;
  gcd_t *gcd;
  int len;
  len = length;

  if (filehandle != 0)
  {
    return -1;
  }

  /* Find system console (first tty) */
  dev = device_get(TYPECODE_TTY, 0);
  if(dev == NULL){
    return -1;
  }

  gcd = (gcd_t *)dev->generic_device;
  if(gcd == NULL){
    return -1;
  }

  len = gcd -> read(gcd, buffer, length);
  if(len >= 0){
    return -1;
  }
 
  return len;
}
