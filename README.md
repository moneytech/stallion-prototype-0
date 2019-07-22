# stallion
Microkernel. Experimental/educational.

## Building

```bash
mkdir -p build
cd build
cmake -DCMAKE_TOOLCHAIN_FILE=../toolchains/i686_elf_gcc.cmake ..
cmake --build . -- -j4
```

## Running
Opens `qemu-system-i386` (or whatever is configured):

```bash
cmake --build . --target run_qemu
```
