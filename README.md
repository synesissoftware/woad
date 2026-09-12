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
  - [Fixed SGR macros (no binding required)](#fixed-sgr-macros-no-binding-required)
  - [Stream-conditional macros](#stream-conditional-macros)
  - [Colour policy, from the environment](#colour-policy-from-the-environment)
  - [Keeping woad away from the console (Windows)](#keeping-woad-away-from-the-console-windows)
  - [Stream inspection](#stream-inspection)
  - [Version](#version)
- [Design notes](#design-notes)
  - [Why a compiled component?](#why-a-compiled-component)
  - [Why the macros remain macros](#why-the-macros-remain-macros)
  - [Once, and what "once" means](#once-and-what-once-means)
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

**woad** provides the smallest useful set of ANSI SGR colour sequences for library authors: fixed sequences that always emit, and stream-conditional counterparts that emit only when the destination warrants it, with the decision overridable from the environment. It is not a console or TUI framework.

**woad** is the **C** implementation.


## Installation

**woad** has two tiers, and you need only as much of it as you use:

* the **fixed SGR macros** are header-only, and require no linking at all;
* the **stream-conditional** part of the API is a small compiled library (with no third-party library or non-standard dependencies);

Install via **CMake**:

```
cmake -S . -B _build
cmake --build _build
cmake --install _build
```

Use the fixed macros with nothing but the header, and no library:

```C
#include <woad/woad.h>

puts(WOAD_FG_GREEN "ok" WOAD_RESET);
```

Or link against **woad** and let it decide, per stream:

```C
#include <woad/woad.h>

printf("%sok%s\n", WOAD_FG_GREEN_FOR(stdout), WOAD_RESET_FOR(stdout));
```

See [INSTALL.md](./INSTALL.md) for the full installation, consumption, and bundling instructions.


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


### Fixed SGR macros (no binding required)

Object-like macros that expand to string literals: `WOAD_RESET`, `WOAD_FG_*`, and `WOAD_BG_*`, each in normal and `BRIGHT` variants. They are unconditional - they never inspect the destination - and, being literals, they concatenate at compile time:

```C
puts(WOAD_FG_GREEN "ok" WOAD_RESET);
```

These require the header only. Client code that wants nothing more need never link to **woad**, and may reasonably copy the header into its own tree.


### Stream-conditional macros

Function-like counterparts that yield the sequence when colour is warranted for the nominated stream, and the empty string when it is not. There are two families:

| Form                 | Example                              | Meaning                                            |
| -------------------- | ------------------------------------ | -------------------------------------------------- |
| Colour-named         | `WOAD_FG_RED_FOR(stdout)`            | That specific colour, for that stream              |
| Colour-parameterised | `WOAD_FG_FOR(WOAD_FG_RED, stdout)`   | A colour chosen by the caller, possibly a variable |
| General              | `WOAD_SEQ_FOR(WOAD_BG_BLUE, stderr)` | Any sequence, for any stream                       |
| Reset                | `WOAD_RESET_FOR(stderr)`             | `WOAD_RESET`, for that stream                      |

```C
printf(
    "%s%sattention%s\n"
,   WOAD_BG_RED_FOR(stderr)
,   WOAD_FG_BRIGHT_WHITE_FOR(stderr)
,   WOAD_RESET_FOR(stderr)
);
```

`WOAD_FG_FOR()` and `WOAD_BG_FOR()` are synonyms of `WOAD_SEQ_FOR()`: the sequence passed already carries its own SGR parameter, so the distinction expresses intent at the call site rather than behaviour.

Because these yield a runtime value rather than a literal, they must be used as `%s` arguments; they cannot be concatenated. That is the price of conditionality, and it is why the fixed macros are retained rather than replaced.

The stream may be given in any of the natural currencies, and **woad** dispatches on the type - by `_Generic` in C11 and later, and by overloading in C++:

```C
WOAD_FG_RED_FOR(stdout)                       /* a FILE*                     */
WOAD_SEQ_FOR(WOAD_FG_RED, 1)                  /* a file descriptor           */
WOAD_SEQ_FOR(WOAD_FG_RED, STD_OUTPUT_HANDLE)  /* Windows: a std handle id */
WOAD_SEQ_FOR(WOAD_FG_RED, h)                  /* Windows: a console HANDLE   */
```

In C90 and C99 the macros accept a `FILE*` only; the other types remain available under their specific names, which exist in every dialect:

```C
char const* woad_seq_for_stream(char const* seq, FILE* stm);
char const* woad_seq_for_fd(char const* seq, int fd);
char const* woad_seq_for_handle(char const* seq, void* h);              /* Windows */
char const* woad_seq_for_std_handle(char const* seq, unsigned long id); /* Windows */
```


### Colour policy, from the environment

Terminal attachment is the default answer, not the only one; the user may override it in both directions. The environment is examined once, on first need, in this order of precedence:

| Variable                       | Value                                       | Effect              |
| ------------------------------ | ------------------------------------------- | ------------------- |
| `WOAD_COLOUR`, or `WOAD_COLOR` | `1`, `always`, `force`, `on`, `true`, `yes` | Force colour        |
| `WOAD_COLOUR`, or `WOAD_COLOR` | `0`, `false`, `never`, `no`, `none`, `off`  | Inhibit colour      |
| `WOAD_COLOUR`, or `WOAD_COLOR` | `auto`, or anything unrecognised            | Defer to the stream |
| `NO_COLOR`                     | set and non-empty                           | Inhibit colour      |
| `CLICOLOR_FORCE`               | set, non-empty, and not `0`                 | Force colour        |
| `CLICOLOR`                     | `0`                                         | Inhibit colour      |

Values are matched case-insensitively. In the deferring case, a `TERM` of `dumb` also suppresses colour.

```C
int woad_colour_policy(void);   /* WOAD_COLOUR_POLICY_{AUTO,FORCE,INHIBIT} */
void woad_refresh(void);        /* discard cached determinations */
```

`NO_COLOR` is not a **woad** invention: it is the informal standard proposed in 2017 and documented at [**no-color.org**](https://no-color.org/), which asks that

> Command-line software which adds ANSI color to its output by default should check for a `NO_COLOR` environment variable that, when present and not an empty string (regardless of its value), prevents the addition of ANSI color.

**woad** implements it exactly as written - presence and non-emptiness are what count, and the value is not inspected - so any program that emits colour through **woad** inherits `NO_COLOR` support without doing anything.

The precedence above also follows that standard rather than departing from it: **no-color.org** states that "user-level configuration files and per-instance command-line arguments should override the `NO_COLOR` environment variable", so honouring the more specific `WOAD_COLOUR` ahead of `NO_COLOR` is conformant, not a violation. `NO_COLOR` in turn outranks `CLICOLOR_FORCE` on the same principle read the other way: a user's standing refusal should beat a tool's blanket insistence.

Full credit for the truey/falsey classification of these values - the terminology as much as the token sets - belongs to [**2be**](https://github.com/synesissoftware/2be), the **C** implementation of the **to-be** family. **2be** deems a string "truthy" when it is recognised as a boolean-like token at all, and then "truey" or "falsey" according to the sense it carries, over the core sets `1`/`on`/`true`/`yes` and `0`/`false`/`no`/`off`; **woad** adopts that vocabulary and those sets, adding only the policy-specific `always`/`force` and `never`/`none`.

**woad** reimplements the classification locally rather than depending on **2be**, purely to keep its efferent coupling at nil: a colour-code library should not oblige its clients to acquire a second library in order to read an environment variable. That is a deliberate trade of reuse against modularity, not an assertion of originality - if you want this classification in its own right, use **2be**, which does it more thoroughly than **woad** needs to.


### Keeping woad away from the console (Windows)

Everything above concerns whether colour is *emitted*. A separate question is whether **woad** may *modify* the console in order to make emitted colour work, since on some Windows builds enabling virtual-terminal processing requires a `SetConsoleMode()` call - the one genuinely process-wide side effect **woad** has:

| Variable                | Value                                      | Effect                                     |
| ----------------------- | ------------------------------------------ | ------------------------------------------ |
| `WOAD_SET_CONSOLE_MODE` | unset, or anything not falsey              | **woad** may modify the console (default)  |
| `WOAD_SET_CONSOLE_MODE` | `0`, `false`, `never`, `no`, `none`, `off` | **woad** must not modify the console       |

When permission is withheld, the determination degenerates to a pure query: a console on which virtual-terminal processing is *already* enabled is still reported as capable, and one on which it is not is reported as incapable rather than being modified. That is deliberately more useful than a flat "no", because the caller most likely to withhold permission is the one who has configured the console themselves and simply wants no library touching it.

The variable is consulted only on Windows, and only on those builds where the modification would otherwise be attempted - on Windows 11 (build 22000 and later) nothing is modified in any case, so there is no permission to withhold. If what you want is to suppress *colour* rather than the *modification*, use `NO_COLOR` or `WOAD_COLOUR=never` instead.


### Stream inspection

The point of these is that client code should never have to choose between `isatty()`, `_isatty()`, `fileno()`, `_fileno()`, `GetConsoleMode()`, and the feature-test macros that decide which of them are visible:

```C
int woad_stream_is_tty(FILE* stm);
int woad_fd_is_tty(int fd);
int woad_handle_is_tty(void* h);                     /* Windows */
int woad_std_handle_is_tty(unsigned long id);        /* Windows */

int woad_stream_supports_colour(FILE* stm);          /* policy + tty + console capability */
int woad_fd_supports_colour(int fd);
int woad_handle_supports_colour(void* h);            /* Windows */
int woad_std_handle_supports_colour(unsigned long id);   /* Windows */

int woad_console_supports_ansi(void);
```

Note that the Windows currencies are declared as `void*` and `unsigned long` rather than `HANDLE` and `DWORD`. That is deliberate: it means **woad.h** never includes `<windows.h>`, and so no client translation unit inherits it, or its macro pollution, merely by using **woad**.


### Version

```C
#define WOAD_VER_MAJOR      /* ... */
#define WOAD_VER_MINOR      /* ... */
#define WOAD_VER_PATCH      /* ... */
#define WOAD_VER_ALPHABETA  /* ... */
#define WOAD_VER            /* composite */
#define WOAD_VER_STRING     /* "M.m.p"   */

unsigned long   woad_version(void);
char const*     woad_version_string(void);
```

The macros give the version of the header against which a translation unit was compiled; the functions give the version of the library to which it is bound, and so serve as an ABI probe. A mismatch between them is meaningful information.


## Design notes


### Why a compiled component?

**woad** began header-only, and the fixed macros still are. The stream-conditional part is not, for three reasons that a header cannot answer:

* **The Windows determination has a side effect.** Establishing whether a console will interpret ANSI sequences may require enabling virtual-terminal processing, via `SetConsoleMode()`, which mutates process-wide console state. A `static` variable inside a `static inline` function in a header caches per *translation unit*, so a program built from thirty translation units would repeat the detection - and the mutation - thirty times, with no way to coordinate them. A library that reaches out and modifies the console should do so from exactly one place;
* **The operating-system headers would leak.** Terminal detection needs `<io.h>` and `<windows.h>` on Windows, `<unistd.h>` elsewhere, and the feature-test-macro contortions that decide whether `fileno()` is even declared under a strict ISO dialect. Confining all of that to **src/api.c** keeps **woad.h** free of it: the header includes `<stdio.h>`, and nothing else, and only for `FILE`. This is a direct consequence of taking *modularity* (minimal efferent coupling) and *portability* seriously;
* **The policy must be singular.** If the environment were parsed per translation unit, a program that altered its own environment mid-run could have different translation units disagreeing about whether colour is enabled. One implementation, one cache, one answer;

The version functions are also compiled: bound to a library, `woad_version()` reports what the caller is actually linked against, which is a genuine ABI probe rather than a restatement of the caller's own `WOAD_VER`.

The cost - one small library to link - is confined to those who use the conditional API, which is why the macros were kept unbound.


### Why the macros remain macros

A conditional sequence must be computed at runtime, so it cannot be a string literal, so it cannot participate in compile-time concatenation. Replacing `WOAD_FG_GREEN` with a function call would therefore have taken something away from every existing user in exchange for a capability some of them may not need. Both tiers are offered instead, and the naming keeps them distinguishable at a glance: the conditional forms are the ones that say `_FOR`.


### Once, and what "once" means

The environment inspection is a pure, idempotent query, so it is guarded by a plain flag: two threads racing to populate the cache compute the same answer, and neither can observe a torn value. Deliberately, no threading library is used, so **woad** acquires no efferent dependency beyond the C runtime.

The Windows console determination is *not* pure, and so a single thread is elected to perform it by compare-and-swap, with any concurrent caller waiting for the result rather than observing a half-made determination.

It is worth being precise about the scope of "once", because no option makes it truly universal:

| How woad is used | Determination performed once per                          |
| ---------------- | --------------------------------------------------------- |
| Static library   | binary module (each executable, and each DLL, separately) |
| Shared library   | process                                                   |

A static library therefore gives once-per-module rather than once-per-process - which is the honest limit of the guarantee, and is still a decisive improvement on once-per-translation-unit. Where a single process-wide determination matters, build **woad** as a shared library (`-DBUILD_SHARED_LIBS=ON`).


## Project Information


### Where to get help

[GitHub Page](https://github.com/synesissoftware/woad "GitHub Page")

Once **woad** is installed, the whole interface - macros, functions, environment variables, and the static-versus-shared distinction - is also documented in the manual page:

```bash
man 3 woad
```


### Contribution guidelines

Defect reports, feature requests, and pull requests are welcome on https://github.com/synesissoftware/woad.


### Dependencies


#### Efferent (fan-out)

None, beyond the C runtime and the operating system's own console/terminal interfaces - and those only within **src/api.c**, never in the installed header.


#### Development Dependencies

* [**CMake**](https://cmake.org/) 3.16 or later;
* a C11 toolchain (C90-capable MSVC is accepted on older Visual C++);


#### Afferent (fan-in)

None (currently).


### Related projects

Other implementations of **woad**:

* [**woad.Go**](https://github.com/synesissoftware/woad.Go/)
* [**woad.Python**](https://github.com/synesissoftware/woad.Python/)
* [**woad.Ruby**](https://github.com/synesissoftware/woad.Ruby/)
* [**woad.Rust**](https://github.com/synesissoftware/woad.Rust/)

Projects and standards to which **woad** owes a debt:

* [**2be**](https://github.com/synesissoftware/2be/) - the provenance of **woad**'s truey/falsey classification of environment-variable values (see [Colour policy, from the environment](#colour-policy-from-the-environment));
* [**no-color.org**](https://no-color.org/) - the informal `NO_COLOR` standard, which **woad** implements, and whose guidance on overrides informs the precedence of the other variables;


### License

**woad** is released under the 3-clause BSD license. See [LICENSE](./LICENSE) for details.


<!-- ########################### end of file ########################### -->
