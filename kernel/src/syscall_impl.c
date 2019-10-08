#include <stallion.h>
#include <stallion/stallion.h>

int32_t stallion_handle_syscall(stallion_interrupt_t *ctx, stallion_t *os) {
  stallion_process_t *proc = os->scheduler.current_process;
  if (proc != NULL) {
    if (ctx->eax == STALLION_SYSCALL_EXIT) {
      // TODO: Pass exit code
      stallion_kill_current_process(&global_os->scheduler);
      return -1;
    } else if (ctx->eax == STALLION_SYSCALL_DECLARE_ATTRIBUTES) {
      if (proc->is_privileged) {
        proc->attributes |= ctx->ebx;
        return STALLION_RESULT_OK;
      }
      return STALLION_RESULT_FAIL;
    } else if (ctx->eax == STALLION_SYSCALL_GET_PID) {
      return proc->id;
    } else {
      kwrites("Unknown syscall code: 0x");
      kputi_r(ctx->eax, 16);
      return STALLION_RESULT_FAIL;
    }
  }
  return STALLION_RESULT_FAIL;
}