/*
 * TLB handling
 */

/*
#include "kernel/panic.h"
#include "kernel/assert.h"
#include "kernel/thread.h"
#include <tlb.h>
#include <pagetable.h>
*/

#include "kernel/panic.h"
#include "kernel/assert.h"
#include "tlb.h"
#include "pagetable.h"
#include "kernel/thread.h"
#include "lib/libc.h"
#include "proc/process.h"
//#include "kernel/thread.h"

//#define CONFIG_MAX_THREADS 32

//extern thread_table_t thread_table[CONFIG_MAX_THREADS];

uint32_t tlb_entry_is_valid(tlb_entry_t* entry, uint32_t vaddr)
{
  if (ADDR_IS_ON_ODD_PAGE(vaddr)) {
    return entry->V1;
  } else {
    return entry->V0;
  }
}

void tlb_error(uint32_t is_userland, char* msg){
  //checks if we are in a userland then is_userland should be 0
  if (is_userland == 0) {
    kprintf(msg);
    kprintf("\n");
    process_exit(255);
  } else {
    KERNEL_PANIC(msg);
  }
}

void tlb_access_exception(uint32_t is_userland){
  // gets data about the exception that was caught
  tlb_exception_state_t exec;
  _tlb_get_exception_state(&exec);

  //gets the current pagetable from the current thread
  pagetable_t *ptable = thread_get_current_thread_entry()->pagetable;


  // checks if there is a ptabe associated with the thread.
  if(ptable == NULL){
    tlb_error(is_userland,"No pagetable associated with this thread found!");
  }
  
  //a loop that runs through the whole ptable
  for (uint32_t i = 0; i < ptable->valid_count; i++) {
    tlb_entry_t *entry = &ptable->entries[i];
    kprintf("entry CALLED?\n");
    //check if adress exists in a page
    if (entry->VPN2 == exec.badvpn2) {
      kprintf("entry CALLED?\n");
      /* The virtual address matches a page's virtual address. */

      //check if it does not exist in a page
      if (tlb_entry_is_valid(entry, exec.badvaddr)) {
        /* Place matching TLB entry somewhere in TLB. */
        kprintf("tlb_entry_is_valid CALLED?\n");

        _tlb_write_random(&ptable->entries[i]);
        return;
      }
      else {
        //evt tlb_error
        //tlb entry is invalid
        tlb_error(is_userland, "Invalid tlb entry found!");
      }
    }
  }
  // we have not found a pagetable.
  // check if it is userland or not, then handle it.
  //tlb_error(is_userland,"Can't find the page in this pagetable");
}

void tlb_modified_exception(uint32_t is_userland)
{
  //kprintf("in tlb_modified_eception\n");
  tlb_error(is_userland, "TLB modified exception.");
}

void tlb_load_exception(uint32_t is_userland)
{
  //kprintf("in tlb_load_exception\n");
  tlb_access_exception(is_userland);
}

void tlb_store_exception(uint32_t is_userland)
{
  //kprintf("in tlb_store_exception\n");
   tlb_access_exception(is_userland);
}

/**
 * Fill TLB with given pagetable. This function is used to set memory
 * mappings in CP0's TLB before we have a proper TLB handling system.
 * This approach limits the maximum mapping size to 128kB.
 *
 * @param pagetable Mappings to write to TLB.
 *
 */

/*
void tlb_fill(pagetable_t *pagetable)
{
  if(pagetable == NULL)
    return;

  // Check that the pagetable can fit into TLB. This is needed until
  //   we have proper VM system, because the whole pagetable must fit
  //  into TLB.
  KERNEL_ASSERT(pagetable->valid_count <= (_tlb_get_maxindex()+1));

  _tlb_write(pagetable->entries, 0, pagetable->valid_count);

  // Set ASID field in Co-Processor 0 to match thread ID so that
  //   only entries with the ASID of the current thread will match in
  //   the TLB hardware.
  _tlb_set_asid(pagetable->ASID);
}
*/
