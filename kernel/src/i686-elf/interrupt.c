#include <stallion.h>
#include <stallion/stallion.h>
#include <stallion_i686_elf.h>
// #include <stallion/stallion_api.h>

#define IRQ_INVALID_TSS 0xa
#define ISR_GENERAL_PROTECTION_FAULT 0xd
#define ISR_PAGE_FAULT 0xe

uint32_t stallion_interrupt_handler(stallion_interrupt_t *ctx) {
  // asm volatile("cli");
  // Acknowledge the PIC; send EOI
  outb(0x20, 0x20);
  if (ctx->number < 32) {
    switch (ctx->number) {
    case 0x3: {
      kputs("BREAK!!!");
      hang();
    } break;
    case ISR_GENERAL_PROTECTION_FAULT: {
      // We hit a GPF fault.
      // No big deal, we can handle it.
      return stallion_handle_general_protection_fault(ctx);
    } break;
    case ISR_PAGE_FAULT: {
      stallion_handle_page_fault(ctx);
    } break;
    case 0x6: {
      char *ptr = (char *)ctx->esp;
      kputs("Invalid opcode.");
      kputptr("Errant instruction pointer", ptr);
      // Not really sure how to handle this, but just overwrite it with a
      // NOP.
      *ptr = 0x90;
      // hang();
    } break;
    default: {
      kwrites("Unhandled exception: ");
      kwritei(ctx->number);
      kwrites("; error code=");
      kputi(ctx->error_code);
      kputc('\n');
    } break;
    }
  } else if (ctx->number >= 32 && ctx->number <= 48) {
    uint32_t irq_no = ctx->number - 32;
    switch (irq_no) {
    case IRQ_INVALID_TSS: {
      uint32_t selector_index = ctx->error_code;
      kwrites("Invalid TSS. Selector index: 0x");
      kputi_r(selector_index, 16);
      kputc('\n');
    } break;
    default: {
      kwrites("Unhandled IRQ interrupt: ");
      kputi(irq_no);
      kputc('\n');
    } break;
    }
    // Acknowledge the PIC; send EOI
    if (irq_no >= 8)
      outb(0xa0, 0x20);
  } else if (ctx->number == 128) {
    // kputs("GOT A SYSCALL!!!!");
    if (ctx->eax == STALLION_SYSCALL_EXIT) {
      // Kill the process.
      // kputs("Exiting.");
      stallion_kill_current_process(&global_os->scheduler);
      return 1;
    } else {
      kwrites("Unknown syscall code: 0x");
      kputi_r(ctx->eax, 16);
    }
  } else {
    kwrites("Got unsupported interrupt: 0x");
    kputi_r(ctx->number, 16);
    kputc('\n');
  }
  return 0;
  // asm volatile("sti");
}

uint32_t stallion_handle_general_protection_fault(stallion_interrupt_t *ctx) {
  // If the page requested was not present, then cr2 will
  // specify the faulty address.
  kputs("Encountered general protection fault!");
  kputptr("Errant instruction pointer", (void *)ctx->esp);
  kwrites("Segment selector index: 0x");
  kputi_r(ctx->error_code, 16);

  // We can't make the desired region available to an unprivileged
  // user, for obvious reasons. So, terminate the process.
  // TODO: Pass an error to aborted processes?
  stallion_kill_current_process(&global_os->scheduler);
  return 2;
  // hang();
}

void stallion_handle_page_fault(stallion_interrupt_t *ctx) {
  uint8_t info = ctx->error_code;
  void *ptr = stallion_get_page_fault_pointer();
  uint32_t page_index = ((uint32_t)ptr) / stallion_page_get_page_size();
  // kputptr("Page fault on pointer", ptr);
  // kwrites("Page index: 0x");
  // kputi_r(page_index, 16);
  if (info == 0x0) {
    // TODO: Determine when to map as user page.
    // TODO: When to release such a page?
    // kputs("Attempt to read non-present page.");
    // kputs("Resolving by mapping page.");
    stallion_page_map(ptr, ptr, 0);
    return;
  } else if (info == 0x2) {
    kputs("Attempt to write to non-present page.");
    stallion_page_map(ptr, ptr, 0);
    return;
  } else if (info == 0x4) {
    kputs("Attempt to read non-present page in ring0.");
  } else if (info == 0x5) {
    kputs("Attempt to read ring0 data from ring3.");
  } else if (info == 0x6) {
    kputs("Attempt to write ring0 data from ring3.");
  } else {
    kwrites("Info: 0b");
    kputi_r(info, 2);
    kputc('\n');
  }
  hang();
  // Try to allocate a spare page in a new directory.
  // TODO: Release this page dir later?
  // int index = next_page_directory();
  // if (index == -1) {
  // 	kputs("Ran out of memory while trying to rectify page fault.");
  // 	return;
  // }

  // Try to allocate a page.
  // We can compute the address of the necessary page
  // easily.
  //
  // TODO: Create an allocation record like usual.
  // First, check if the page has been marked present.
  // Then, proceed accordingly, so that the page is not
  // entirely unusable, and so the memory in question
  // does not get overwritten.
  // size_t page_no = ((size_t)ptr) / PAGE_SIZE;
  // mark_page_present(page_no);
}