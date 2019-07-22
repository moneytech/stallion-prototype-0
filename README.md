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
### Building `grub` for `clang`
It's actually pretty much not possible, so unless you already have GRUB installed,
you can't build it from source on Mac. :/

This was my attempt at getting a compile:

```bash
./configure --disable-werror TARGET_CC="clang" TARGET_CFLAGS="-target i686-elf" \
TARGET_OBJCOPY=i686-elf-objcopy TARGET_STRIP=i686-elf-strip \
TARGET_NM=i686-elf-nm TARGET_RANLIB=i686-elf-ranlib --target=i686-elf
```
