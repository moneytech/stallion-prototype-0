project(stallion_x86 ASM_NASM)

set(CMAKE_ASM_NASM_OBJECT_FORMAT elf32)

enable_language(ASM_NASM)

add_library(stallion_arch a20.asm boot.asm nmi.asm)