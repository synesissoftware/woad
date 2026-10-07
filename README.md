# woad <!-- omit in toc -->

Minimal ANSI terminal colour codes, for C


![C](https://img.shields.io/badge/C-00599C?style=flat&logo=c&logoColor=white)
[![License](https://img.shields.io/badge/License-BSD_3--Clause-blue.svg)](https://opensource.org/licenses/BSD-3-Clause)
[![GitHub release](https://img.shields.io/github/v/release/synesissoftware/woad.svg)](https://github.com/synesissoftware/woad/releases/latest)
[![Last Commit](https://img.shields.io/github/last-commit/synesissoftware/woad)](https://github.com/synesissoftware/woad/commits/master)
[![CI](https://github.com/synesissoftware/woad/actions/workflows/ci.yml/badge.svg)](https://github.com/synesissoftware/woad/actions/workflows/ci.yml)


## Table of Contents <!-- omit in toc -->

- [Introduction](#introduction)
- [Installation](#installation)
- [Components](#components)
- [Examples](#examples)
- [Project Information](#project-information)
  - [Where to get help](#where-to-get-help)
  - [Contribution guidelines](#contribution-guidelines)
  - [Dependencies](#dependencies)
  - [Related projects](#related-projects)
  - [License](#license)


## Introduction

**woad** provides the smallest useful set of fixed ANSI SGR colour sequences for library authors. It is not a console or TUI framework.

**woad** is the **C** implementation. The public surface is string macros: there are no functions. The sequences are always the colour codes; they do not inspect TTY state or Windows console mode.


## Installation

**woad** is a header-only C library. The install flow is in [INSTALL.md](./INSTALL.md). From a clone:

```bash
./prepare_cmake.sh
./build_cmake.sh
./ctest_cmake.sh --verbose
```

Use via include. Nothing is linked:

```c
#include <woad/woad.h>

#include <stdio.h>

int main(void)
{
    puts(WOAD_FG_GREEN "ok" WOAD_RESET);

    return 0;
}
```


## Components

**woad** ships SGR string macros and version macros. TTY/stream gating and Windows virtual-terminal opt-in are not implemented yet.


### Constants

* `WOAD_RESET` — reset all attributes;
* `WOAD_FG_*` / `WOAD_BG_*` — foreground and background, including `WOAD_FG_BRIGHT_*` and `WOAD_BG_BRIGHT_*`;
* `WOAD_VER_MAJOR`, `WOAD_VER_MINOR`, `WOAD_VER_PATCH`, `WOAD_VER_REVISION`, `WOAD_VER_ALPHABETA`, `WOAD_VER`, `WOAD_VER_STRING`.


## Examples

Examples live under **examples/c/**. The directory is the subject; the built program is `example.c.<subject>`. Build them with `BUILD_EXAMPLES` (on by default); run via **run_all_examples.sh**.

| Example | Language | Notes |
| ------- | -------- | ----- |
| [**example.c.colour**](./examples/c/colour/) | C | Emits a coloured status line for each foreground |
| [**example.c.version**](./examples/c/version/) | C | Prints `WOAD_VER_STRING` in green |


## Project Information


### Where to get help

* [GitHub Page](https://github.com/synesissoftware/woad)
* [GitHub Issues](https://github.com/synesissoftware/woad/issues)
* [FAQ.md](./FAQ.md)
* [HOW_YOU_CAN_HELP.md](./HOW_YOU_CAN_HELP.md)


### Contribution guidelines

Defect reports, feature requests, and pull requests are welcome on https://github.com/synesissoftware/woad.

See also [HOW_YOU_CAN_HELP.md](./HOW_YOU_CAN_HELP.md).


### Dependencies

The **C** API has no non-standard dependencies. Building examples and tests needs **CMake** 3.16 or later and a C11 toolchain (C90-capable MSVC is accepted on older Visual C++).


### Related projects

The other **woad** implementations:

| Project | Language |
| ------- | -------- |
| [**woad.Go**](https://github.com/synesissoftware/woad.Go/) | Go |
| [**woad.NET**](https://github.com/synesissoftware/woad.NET/) | .NET |
| [**woad.Python**](https://github.com/synesissoftware/woad.Python/) | Python |
| [**woad.Ruby**](https://github.com/synesissoftware/woad.Ruby/) | Ruby |
| [**woad.Rust**](https://github.com/synesissoftware/woad.Rust/) | Rust |
| [**woad.Zig**](https://github.com/synesissoftware/woad.Zig/) | Zig |


### License

**woad** is released under the 3-clause BSD license. See [LICENSE](./LICENSE) for details.


<!-- ########################### end of file ########################### -->
