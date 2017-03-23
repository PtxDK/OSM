/*
 * Halt the system
 */

#ifndef KUDOS_KERNEL_WRITE_H
#define KUDOS_KERNEL_WRITE_H

int kernel_write(int filehandle, const void *buffer, int length);

#endif /* KUDOS_KERNEL_WRITE_H */
