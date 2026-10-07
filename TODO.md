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
* [ ] TTY-conditional colour codes (process and per-stream);
* [ ] Windows virtual-terminal gating (OS build + `GetConsoleMode`);


## Performance improvements

* \<none>


## Packaging improvements

* \<none>


<!-- ########################### end of file ########################### -->
