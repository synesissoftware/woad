# woad - TODO <!-- omit in toc -->


## Table of Contents <!-- omit in toc -->

- [Build systems](#build-systems)
  - [Synesis CMake helper scripts](#synesis-cmake-helper-scripts)
  - [Continuous integration](#continuous-integration)
- [Functional improvements](#functional-improvements)
- [Performance improvements](#performance-improvements)
- [Packaging improvements](#packaging-improvements)


## Build systems


### Synesis CMake helper scripts

* [x] `prepare_cmake.sh`;
* [x] `build_cmake.sh`;
* [x] `clean_cmake.sh`;
* [x] `remove_cmake_artefacts.sh`;
* [x] `ctest_cmake.sh`;
* [x] `run_all_examples.sh`;


### Continuous integration

* [x] OS × GCC / Clang / MSVC / MinGW matrix;


## Functional improvements

* [x] ~~~SGR colour and reset codes~~~ - ✅;
* [x] ~~~runtime version API (`woad_version()`, `woad_version_string()`)~~~ - ✅;
* [x] ~~~TTY-conditional colour codes (process and per-stream)~~~ - ✅;
* [x] ~~~Windows virtual-terminal gating (OS build + `GetConsoleMode`)~~~ - ✅;
* [x] ~~~environment override of colour policy (force / inhibit)~~~ - ✅;
* [ ] `COLORTERM` / `TERM` capability inspection beyond the `dumb` special case;
* [ ] Cygwin, MSYS2, and Git-Bash (mintty) terminals: work out how to be compatible with them. Built against the MSVC runtime, these present as pipes rather than as consoles, so `_isatty()` answers 0 and `GetConsoleMode()` fails, and **woad** therefore suppresses colour even though mintty interprets ANSI perfectly well. Probing the pipe's name for the `msys-`/`cygwin-`…`-pty`…`-to-master` form via `GetFileInformationByHandleEx(FileNameInfo)` is the usual remedy; native Cygwin builds (`__CYGWIN__`) need none of it, since their `isatty()` is already correct. Decide also whether such a terminal should be reported by `woad_stream_is_tty()`, by `woad_console_supports_ansi()`, or by both, and how that interacts with `WOAD_SET_CONSOLE_MODE`, there being no console mode to set;
* [ ] 256-colour and 24-bit (truecolour) sequences;


## Performance improvements

* \<none


## Packaging improvements

* \<none


<!-- ########################### end of file ########################### -->
