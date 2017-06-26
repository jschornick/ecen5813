# ECEN5813 Project 1

This repository includes the code required for Project 1 of ECEN5813.

---

## Directory organization:

 - [`src`](src) : Makefiles and C/ASM source files
 - [`include`](include) : All header files
   - [`common`](include/common) : Platform-independent header files
   - [`kl25z`](include/kl25z) : KL25z platform-specific header files
   - [`CMSIS`](include/CMSIS) : ARM architecture-specific header files
 - [`platform`](platform) : Non-source files necessary for building (e.g., linker scripts)
 - [`test`](test) : Additional verification tests

---

## Build instructions

The main executable can be built for three target platforms:

  - `HOST` : native, system gcc
  - `BBB` : cross-compile for BeagleBone Black running Linux
  - `KL25Z` : cross-compile for a bare-metal KL25Z development board

### To perform a full build

A full build will create the project executable `project1.elf`, as well as object files `*.o` and dependency lists `*.d` for each source file.

```
$ cd src
$ make clean
$ make PLATFORM={HOST,BBB,KL25Z}
```

If `PLATFORM` is not set, the build system will default to `PLATFORM=HOST`. 

**NOTE**: Builds are performed in-tree, so it is necessary to `make clean` when changing the `PLATFORM` setting.

### Alternate built targets

A variety of additional build targets are available. All targets will honor the `PLATFORM` setting.

 - `build`         : Build target executable
 - `build-lib`     : Build target executable from library archive
 - `build-ld `     : Build target by directly invoking the linker
 - `compile-all`   : Compile all sources into object files
 - `clean      `   : Remove all generated files
 - `<file>.o   `   : Compile a single .c/.S source file
 - `<file>.i   `   : Precompile a single source file
 - `<file>.d   `   : Create the dependency file for a source file
 - `<file>.asm `   : Compile a single C source file into assembly
 - `libproject1.a` : Create library of platform independent functions
 - `runtests`      : Build and run additional tests
