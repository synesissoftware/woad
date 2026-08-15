# woad <!-- omit in toc -->

Minimal ANSI terminal colour codes, for C

![Language](https://img.shields.io/badge/C-00599C?style=flat&logo=c&logoColor=white)
[![License](https://img.shields.io/badge/License-BSD_3--Clause-blue.svg)](https://opensource.org/licenses/BSD-3-Clause)
[![GitHub release](https://img.shields.io/github/v/release/synesissoftware/woad.svg)](https://github.com/synesissoftware/woad/releases/latest)
[![Last Commit](https://img.shields.io/github/last-commit/synesissoftware/woad)](https://github.com/synesissoftware/woad/commits/master)
[![CI](https://github.com/synesissoftware/woad/actions/workflows/ci.yml/badge.svg)](https://github.com/synesissoftware/woad/actions/workflows/ci.yml)


## Table of Contents <!-- omit in toc -->

- [Introduction](#introduction)
- [Installation](#installation)
- [Building](#building)
- [Components](#components)
- [Project Information](#project-information)
  - [Where to get help](#where-to-get-help)
  - [Contribution guidelines](#contribution-guidelines)
  - [Dependencies](#dependencies)
    - [Efferent (fan-out)](#efferent-fan-out)
    - [Development Dependencies](#development-dependencies)
    - [Afferent (fan-in)](#afferent-fan-in)
  - [Related projects](#related-projects)
  - [License](#license)


## Introduction

**woad** provides the smallest useful set of fixed ANSI SGR colour sequences for library authors. It is not a console or TUI framework.

**woad** is the **C** implementation.


## Installation

**woad** is a header-only C library. Install via **CMake**:

```
cmake -S . -B _build
cmake --build _build
cmake --install _build
```

Use via **include**:

```C
#include <woad/woad.h>

puts(WOAD_FG_GREEN "ok" WOAD_RESET);
```


## Building

From a clone, the Synesis helper scripts are:

```
./prepare_cmake.sh
./build_cmake.sh
./ctest_cmake.sh --verbose
```

Equivalent raw **CMake**:

```
cmake -S . -B _build -DCMAKE_BUILD_TYPE=Release
cmake --build _build
ctest --test-dir _build --output-on-failure
```


## Components

**woad** ships SGR string macros (`WOAD_RESET`, `WOAD_FG_*`, `WOAD_BG_*`, including bright variants) and version macros (`WOAD_VER_MAJOR`, `WOAD_VER_MINOR`, `WOAD_VER_PATCH`, `WOAD_VER_REVISION`, `WOAD_VER_ALPHABETA`, `WOAD_VER`, `WOAD_VER_STRING`). TTY/stream gating and Windows virtual-terminal opt-in are not implemented yet.


## Project Information


### Where to get help

[GitHub Page](https://github.com/synesissoftware/woad "GitHub Page")


### Contribution guidelines

Defect reports, feature requests, and pull requests are welcome on https://github.com/synesissoftware/woad.


### Dependencies


#### Efferent (fan-out)

None.


#### Development Dependencies

* [**CMake**](https://cmake.org/) 3.16 or later;
* a C11 toolchain (C90-capable MSVC is accepted on older Visual C++);


#### Afferent (fan-in)

None (currently).


### Related projects

* [**woad.Python**](https://github.com/synesissoftware/woad.Python/)
* [**woad.Ruby**](https://github.com/synesissoftware/woad.Ruby/)
* [**woad.Rust**](https://github.com/synesissoftware/woad.Rust/)


### License

**woad** is released under the 3-clause BSD license. See [LICENSE](./LICENSE) for details.


<!-- ########################### end of file ########################### -->
