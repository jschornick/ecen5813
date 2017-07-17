# ECEN5813 Project 2

This repository includes the code required for Project 2 of ECEN5813.

---

## Description

This project demonstrates the use of interrupt-driven serial I/O via UART on
the KL25Z. A data processor is instantiated which takes an ASCII file as input
over serial, processes the input continuously, and sends a tabular summary of
the data when an ASCII escape is received.

Though designed to run on the KL25Z, the application code is platform independent.
The project will build for Linux using C standard I/O in place of serial communication.

The major system components are graphically documented in the
[`architecture diagram`](doc/architecture.svg).

---

## Directory organization:

 - [`makefile`](makefile) : Top-level makefile, see _Build instructions_ below
 - [`src`](src) : C/ASM source files
 - [`include`](include) : Project and platform header files
   - [`common`](include/common) : Platform-independent header files
   - [`kl25z`](include/kl25z) : KL25z platform-specific header files
   - [`CMSIS`](include/CMSIS) : ARM architecture-specific header files
 - [`platform`](platform) : Non-source files necessary for building (e.g., linker scripts, debugger configuration)
 - [`buildsys`](buildsys) : Supporting makefile components for the build system
 - [`tests`](tests) : Unit tests
 - [`doc`](doc) : Addition documentation (architecture diagram)

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
 - `clean`         : Remove all generated files for platform
 - `clean-all`     : Remove all generated files
 - `<file>.o`      : Compile a single .c/.S source file
 - `<file>.i`      : Precompile a single source file
 - `<file>.asm`    : Compile a single C source file into assembly

## Running tests

The unit testing framework is included in this repository as a submodule. A
fresh clone should initialize the submodule before attempting to run tests:

```
$ cd [repo]
$ git submodule init
$ git submodule update
```

Once updated, the build system can automatically build the testing framework and run all unit tests:

```
$ make unittests
```

## Flashing

The KL25Z microcontroller can be flashed with the program image using the following command:

```
$ make flash PLATFORM=KL25Z
```
The image will be rebuilt if necessary.

## Debug

The program for the KL25Z can be easily debugged using GDB/OpenOCD:

```
$ make debug PLATFORM=KL25Z
```
This command will used OpenOCD to connect to the CMSIS-DAP debugging interface
on the KL25Z. GDB will be automatically loaded with the currently built binary,
so debugging can begin immediately.

