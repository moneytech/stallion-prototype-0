# stallion
![Horse](http://www.illinoishorsefair.com/wp-content/uploads/2014/11/Wild-Stallion.small_.jpg)

A high-performance, capability-based operating system for
secure enterprise and personal computing.

## Why Stallion?
* No viruses - programs are untrusted and heavily sandboxed
* High performance - no cruft of legacy code to weigh down the system
* Secure - programs can't interact with each other, preventing data
leaks

## Building from Source
You'll need a GCC cross compiler. At this time, only 32-bit x86 is
supported, so ensure you have `i686-elf-gcc`, or a similar toolchain.

You can then build the entire operating system in one pass, by running
the following in your shell:

```bash
# Replace -j4 with the number of cores on your system
# The default COMPILER_PREFIX is i686-elf.
make -j4 COMPILER_PREFIX=i686-elf
```

When the build process is complete, the OS can be found at
`src/stallion.iso`.