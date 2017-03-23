# Test Eksempel!

Gå ind i userland mappen:
Lav din testfil : proc.c

    #include "lib.h"
    
    Int main(){
    	int pid = sysall_spawn("[disk]hw.mips32", NULL)
    	Int retval = syscall_join(pid)
    	Printf("returværdi: %d\n",retval);
    	syscall_exit(0)
    	Return 0;
    }

Gå ind i makefilen
Tilføj testfilen til sources.

    SOURCES = hello.c proc.c osv osv osv...

I terminal 1
cd dig til kudos (ikke kudos/kudos)
Du skal nu kompilerer de testfiler der står i SOURCES:

    yams-tfs

Der burde nu være printet en masse bs, blandt andet proc.c proc mips'stuff

Åbn nu din terminal til kudos ved at køre:

(Du står stadig i kudos(ikke kudos/kudos)

    yams-term

Terminal 2
Stil dig i kudos (ikke kudos/kudos) og kør:

    yams-sim kudos/kudos-mips32 initprog=[disk]proc.mips32

Test done.
