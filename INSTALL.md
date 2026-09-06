# woad - Installation and Use <!-- omit in toc -->


## Table of Contents <!-- omit in toc -->

- [Two ways to use woad](#two-ways-to-use-woad)
  - [1. Header-only, unbound](#1-header-only-unbound)
  - [2. Bound to the compiled component](#2-bound-to-the-compiled-component)
- [Using CMake](#using-cmake)
  - [Obtain the latest distribution of woad](#obtain-the-latest-distribution-of-woad)
  - [Prepare the CMake configuration](#prepare-the-cmake-configuration)
    - [Visual C++ with static dependency on standard library](#visual-c-with-static-dependency-on-standard-library)
    - [Shared library](#shared-library)
  - [Build](#build)
  - [Test](#test)
  - [Install](#install)
- [Consuming an installed woad](#consuming-an-installed-woad)
  - [CMake](#cmake)
  - [pkg-config](#pkg-config)
  - [By hand](#by-hand)
- [Bundling woad into another project](#bundling-woad-into-another-project)
- [Language and toolchain requirements](#language-and-toolchain-requirements)


## Two ways to use woad

**woad** is deliberately usable at two levels, and you should choose the lower one if it suffices.


### 1. Header-only, unbound

The object-like SGR macros - `WOAD_RESET`, `WOAD_FG_*`, `WOAD_BG_*`, and the version macros - are ordinary string and integer literals. They require nothing but the header, and no linking whatsoever:

```C
#include <woad/woad.h>

puts(WOAD_FG_GREEN "ok" WOAD_RESET);
```

Because they are string literals, they concatenate at compile time, which the conditional forms cannot do. If all you want is "emit this colour", this is the whole library, and copying **include/woad/woad.h** into your own tree is a legitimate way to consume it.


### 2. Bound to the compiled component

The stream-conditional macros - `WOAD_FG_RED_FOR(stm)`, `WOAD_SEQ_FOR(clr, stm)`, and the rest - and the functions they call, require linking against the **woad** library:

```C
#include <woad/woad.h>

printf(
    "%sok%s\n"
,   WOAD_FG_GREEN_FOR(stdout)
,   WOAD_RESET_FOR(stdout)
);
```

For why this part cannot sensibly live in the header, see [Why a compiled component?](./README.md#why-a-compiled-component) in **README.md**.


## Using CMake


### Obtain the latest distribution of woad

You can obtain a **.zip** or **.tar.gz** at the [Releases page](https://github.com/synesissoftware/woad/releases), e.g.:

```bash
$ mkdir -p ~/open-source
$ cd ~/open-source
$ curl -L -O https://github.com/synesissoftware/woad/archive/refs/tags/0.0.1-beta1.tar.gz
$ gunzip 0.0.1-beta1.tar.gz
$ cd woad-0.0.1-beta1
```

Or you may prefer to clone the project via git:

```bash
$ mkdir -p ~/open-source
$ cd ~/open-source
$ git clone https://github.com/synesissoftware/woad/
$ cd woad
```


### Prepare the CMake configuration

Prepare the CMake configuration, via the **prepare_cmake.sh** script, as in:

```bash
./prepare_cmake.sh
```

By default, this will create a **_build** directory under the project directory. If you want to use a specific directory you can do so via the environment variable `SIS_CMAKE_BUILD_DIR`:

```bash
$ export SIS_CMAKE_BUILD_DIR=/tmp/woad
$ ./prepare_cmake.sh
```

> **NOTE**: If you provide the flag `--run-make` (=== `-m`) then the script will also run a build, so the [Build](#build) step can be skipped.

```bash
$ export SIS_CMAKE_BUILD_DIR=/tmp/woad
$ ./prepare_cmake.sh -m
```


#### Visual C++ with static dependency on standard library

If you're using Visual C++ _AND_ you want to use the standard library with static dependency (rather than the default dynamic dependency), then you will need to supply the `--msvc-mt` flag.

```bash
export SIS_CMAKE_BUILD_DIR=/tmp/woad
./prepare_cmake.sh --msvc-mt
```


#### Shared library

**woad** builds as a static library by default. To build it as a shared library, use the helper script's `--build-shared-libs` flag:

```bash
./prepare_cmake.sh --build-shared-libs
```

or, equivalently, set `BUILD_SHARED_LIBS` when invoking CMake directly:

```bash
cmake -S . -B _build -DBUILD_SHARED_LIBS=ON
```

Since `BUILD_SHARED_LIBS` is a configure-time cache variable, it can be selected only when CMake is (re)run; there is nothing correspondingly to pass to **build_cmake.sh**.

On Windows, `WINDOWS_EXPORT_ALL_SYMBOLS` is set on the target, so no export decoration is required in the header and no `WOAD_DLL`-style macro contract is imposed on clients.

> **NOTE**: A shared library is the only form in which the once-per-process determinations are truly once *per process*; with a static library they are once per binary module. See [README.md](./README.md#why-a-compiled-component).


### Build

```bash
./build_cmake.sh
```

> **NOTE**: If you provide the flag `--run-make` (=== `-m`) when preparing then you do not need this step.


### Test

Unit-tests and smoke-tests are provided.

> **NOTE**: If you do not provide the flag `--no-make` (=== `-M`) then the scripts will also run a build.


Unit-test:

```bash
./ctest_cmake.sh --verbose --no-make
```

Smoke-test:

```bash
./run_all_examples.sh --no-make
```


The **colour_for** example is worth running twice, since it demonstrates the whole point of the compiled component:

```bash
./_build/colour_for            # colour, if your terminal warrants it
./_build/colour_for | cat      # no colour, because the destination is a pipe
WOAD_COLOUR=always ./_build/colour_for | cat   # colour anyway
```


### Install

Install the library on the host, via `cmake`, as in:

```bash
sudo cmake --install ${SIS_CMAKE_BUILD_DIR:-./_build} --config Release
```

This installs:

| Artefact                                              | Destination             |
| ----------------------------------------------------- | ----------------------- |
| **include/woad/woad.h**                               | `${includedir}/woad/`   |
| **libwoad.a** / **libwoad.so** / **woad.lib**         | `${libdir}`             |
| **woad.3**                                            | `${mandir}/man3/`       |
| **woad.pc**                                           | `${libdir}/pkgconfig/`  |
| **woad-config.cmake**, **woad-targets.cmake**, et al. | `${libdir}/cmake/woad/` |

The manual page documents the whole interface - macros, functions, environment variables, and the static-versus-shared distinction - and is reachable, once installed, as:

```bash
man 3 woad
```

> **NOTE**: It is generated by CMake from **man/woad.3.in**, so that the version in its `.TH` line cannot drift from the version scraped from **include/woad/woad.h**. Edit the template, never the generated page.


## Consuming an installed woad


### CMake

```CMake
find_package(woad REQUIRED)

target_link_libraries(my_target PRIVATE woad::woad)
```

The imported target carries the include directory, so no separate `target_include_directories()` is needed.


### pkg-config

```bash
cc $(pkg-config --cflags --libs woad) my_program.c -o my_program
```


### By hand

```bash
cc -I/usr/local/include my_program.c -L/usr/local/lib -lwoad -o my_program
```

Or, if you are using only the object-like macros, simply:

```bash
cc -I/usr/local/include my_program.c -o my_program
```


## Bundling woad into another project

Two bundling strategies are supported, and the choice follows the two levels of use above:

* **Header only** - copy **include/woad/woad.h** into your tree and use the object-like macros. Nothing to build, nothing to link, and no operating-system headers are dragged in: **woad.h** includes only `<stdio.h>`, and then only for `FILE`;
* **Header and source** - copy **include/woad/woad.h** and **src/api.c**, and add the latter to your own build. That is one additional implementation file, and it brings the stream-conditional API with it;

Alternatively, add the project as a CMake subdirectory, in which case `BUILD_EXAMPLES` and `BUILD_TESTING` default to `OFF`, since they are enabled only when **woad** is the top-level project:

```CMake
add_subdirectory(third-party/woad)

target_link_libraries(my_target PRIVATE woad::woad)
```


## Language and toolchain requirements

| Requirement                     | Notes                                                                 |
| ------------------------------- | --------------------------------------------------------------------- |
| **CMake** 3.16 or later         | For the supported build                                               |
| C90 or later, or C++98 or later | **woad.h** is valid in all of them; the library sources are C90-clean |
| C11 or later, or any C++        | Required only for the type-dispatched forms - see below               |

The type-dispatched wrappers `woad_seq_for()` and `woad_supports_colour()` accept a `FILE*`, a file descriptor, or (on Windows) a console handle or standard-handle identifier. This is implemented by `_Generic` in C11 and later, and by overloading in C++. In C90 and C99 those wrappers accept a `FILE*` only, and the other currencies remain available under their specific names - `woad_seq_for_fd()`, `woad_seq_for_handle()`, and `woad_seq_for_std_handle()` - which are available in every dialect.


<!-- ########################### end of file ########################### -->
