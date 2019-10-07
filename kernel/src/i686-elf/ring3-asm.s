.section .text
.global stallion_tss_flush
.type stallion_tss_flush, @function

# void tss_flush(void* entry_point)
stallion_tss_flush:
  # First, flush the TSS.
  mov $0x2b, %ax
  ltr %ax

  # Next, jump to usermode via IRET.
  pop %ebx # Store entry point addr in ebx
  # IRET:
    # SS
    # ESP
    # EFLAGS
    # CS
    # EIP
  # cli
  mov $0x23, %ax # Also set RPL to 3 (User data=0x20)
  mov %ax, %ds
  mov %ax, %es
  mov %ax, %fs
  mov %ax, %gs # User data seg

  mov %esp, %eax
  pushl $0x23 # SS (select the same segment as above)
  pushl %eax # ESP
  pushf # EFLAGS

  pushl $0x1b # CS User code seg
  push %ebx # ebx has entry_point, the jump address
  iret
.size stallion_tss_flush, . - stallion_tss_flush