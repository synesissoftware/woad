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

* [x] ~~~**prepare_cmake.sh**~~~ - ✅;
* [x] ~~~**build_cmake.sh**~~~ - ✅;
* [x] ~~~**clean_cmake.sh**~~~ - ✅;
* [x] ~~~**remove_cmake_artefacts.sh**~~~ - ✅;
* [x] ~~~**ctest_cmake.sh**~~~ - ✅;
* [x] ~~~**run_all_automated_tests.sh**~~~ - ✅;
* [x] ~~~**run_all_component_tests.sh**~~~ - ✅;
* [x] ~~~**run_all_examples.sh**~~~ - ✅;
* [x] ~~~**run_all_performance_tests.sh**~~~ - ✅;
* [x] ~~~**run_all_scratch_tests.sh**~~~ - ✅;
* [x] ~~~**run_all_unit_tests.sh**~~~ - ✅;
* [x] ~~~native **.cmd** counterparts~~~ - ✅;


### Continuous integration

* [x] ~~~OS × GCC / Clang / MSVC / MinGW matrix~~~ - ✅;
* [x] ~~~modular **ci.yml** + **ci-cell.yml**~~~ - ✅;


## Functional improvements

* [x] ~~~SGR colour and reset codes~~~ - ✅;
* [x] ~~~fixed bold and blinking SGR attributes~~~ - ✅;
* [x] ~~~runtime version API (`woad_version()`, `woad_version_string()`)~~~ - ✅;
* [x] ~~~TTY-conditional colour codes (process and per-stream)~~~ - ✅;
* [x] ~~~Windows virtual-terminal gating (OS build + `GetConsoleMode`)~~~ - ✅;
* [x] ~~~environment override of colour policy (force / inhibit)~~~ - ✅;
* [ ] `COLORTERM` / `TERM` capability inspection beyond the `dumb` special case;
* [ ] Cygwin, MSYS2, and Git-Bash (mintty) terminals: work out how to be compatible with them. Built against the MSVC runtime, these present as pipes rather than as consoles, so `_isatty()` answers 0 and `GetConsoleMode()` fails, and **woad** therefore suppresses colour even though mintty interprets ANSI perfectly well. Probing the pipe's name for the `msys-`/`cygwin-`…`-pty`…`-to-master` form via `GetFileInformationByHandleEx(FileNameInfo)` is the usual remedy; native Cygwin builds (`__CYGWIN__`) need none of it, since their `isatty()` is already correct. Decide also whether such a terminal should be reported by `woad_stream_is_tty()`, by `woad_console_supports_ansi()`, or by both, and how that interacts with `WOAD_SET_CONSOLE_MODE`, there being no console mode to set;
* [ ] 256-colour and 24-bit (truecolour) sequences;


## Performance improvements

* [ ] apply colour policy before the per-call terminal probe in `woad_supports_colour_()` (and thus colour-for): force and inhibit already decide the answer, yet today they still pay `isatty()` / `_isatty()` / `GetConsoleMode()`; **test.performance.colour_for** shows those probes are the hot cost (Unix `isatty` ~150–700 ns; Windows handle/`GetConsoleMode` ~1 µs even when the handle is not a console), while a warm policy read is near the measurement floor;
* [ ] measure a live Windows virtual-terminal path in **test.performance.colour_for**: CI redirects stdout/stderr, so cold `woad_console_supports_ansi` only times `RtlGetVersion` plus failed `GetConsoleMode` on pipes (~2 µs), not a successful `SetConsoleMode` enable; discover a console (`AllocConsole` + `CONOUT$`, rebind `STD_*` only for that group) and report OS build / VT path in the preamble so Win10-server versus Win11+ branches are visible in Actions logs;
* [ ] decide whether Windows handle / std-handle currencies should cache or otherwise amortise `GetConsoleMode` across repeated colour-for calls on the same handle: stream/fd colour-for on redirected Windows is ~4–13 ns, while the handle currencies stay ~1 µs per call; document the trade-off against handle lifetime and `woad_refresh()`;


## Packaging improvements

* \<none>


<!-- ########################### end of file ########################### -->
