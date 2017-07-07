# ECEN5813 Project 2

This repository includes the code required for Project 2 of ECEN5813.

---

## Directory organization:

 - `Makefile` : Top-level makefile, see _Build instructions_ below
 - [`src`](src) : C/ASM source files
 - [`include`](include) : Project and platform header files
   - [`common`](include/common) : Platform-independent header files
   - [`kl25z`](include/kl25z) : KL25z platform-specific header files
   - [`CMSIS`](include/CMSIS) : ARM architecture-specific header files
 - [`platform`](platform) : Non-source files necessary for building (e.g., linker scripts, debugger configuration)
 - [`buildsys`](buildsys) : Supporting makefiles for the build system
 - [`test`](test) : Tests

---

## Build instructions

The main executable can be built for three target platforms by setting the `PLATFORM` variable:

  - `HOST` : native, system gcc
  - `BBB` : cross-compile for BeagleBone Black running Linux
  - `KL25Z` : cross-compile for a bare-metal KL25Z development board

If `PLATFORM` is not set, the build system will default to `PLATFORM=HOST`. 

### To perform a full build

Full build example:

```
$ make PLATFORM={HOST,BBB,KL25Z}
```

A successful build will create the project executable `project2_${PLATFORM}.elf` as well as object files `*.o` for each source file.

Temporary build files will be placed in `BUILDOUT/${PLATFORM}`.


### Alternate built targets

A variety of additional build targets are available. All targets will honor the `PLATFORM` setting.

 - `build`         : Build target executable
 - `compile-all`   : Compile all sources into object files
 - `clean      `   : Remove all generated files for platform
 - `clean-all  `   : Remove all generated files
 - `<file>.o   `   : Compile a single .c/.S source file
 - `<file>.i   `   : Precompile a single source file
 - `<file>.asm `   : Compile a single C source file into assembly


## Running tests

```
$ make test
```
