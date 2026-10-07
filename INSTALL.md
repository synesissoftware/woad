# woad - Installation and Use <!-- omit in toc -->

**woad** is a header-only C library. Once installed, include
**woad/woad.h**. There is no library to link.


## Table of Contents <!-- omit in toc -->

- [CMake](#cmake)
- [Bundled](#bundled)


## CMake

The primary choice for installation is by use of **CMake**.

1. Obtain the latest distribution of **woad**, from
   https://github.com/synesissoftware/woad/, e.g.

   ```bash
   $ mkdir -p ~/open-source
   $ cd ~/open-source
   $ git clone https://github.com/synesissoftware/woad/
   ```

2. Prepare the CMake configuration, via the **prepare_cmake.sh** script.

   ```bash
   $ cd ~/open-source/woad
   $ ./prepare_cmake.sh -v
   ```

   Useful optional flags:

   * `--disable-examples` / `-E` — omit examples (`BUILD_EXAMPLES=OFF`);
   * `--disable-testing` / `-T` — omit tests (`BUILD_TESTING=OFF`);
   * `--build-shared-libs` — passes `BUILD_SHARED_LIBS=ON`. **woad** is
     header-only, so this does not produce a shared library;

   (**Hint**: execute `$ ./prepare_cmake.sh --help` for more information.)

3. Run a build of the generated **CMake**-derived build files via the
   **build_cmake.sh** script, as in:

   ```bash
   $ ./build_cmake.sh
   ```

   (**NOTE**: if you provide the flag `--run-make` (=== `-m`) in step 2 then
   you do not need this step.)

4. As a check (when testing was not disabled), execute the built unit-test
   programs via **run_all_unit_tests.sh**, as in:

   ```bash
   $ ./run_all_unit_tests.sh
   ```

   Component, scratch, and performance suites have matching runners
   (**run_all_component_tests.sh**, **run_all_scratch_tests.sh**,
   **run_all_performance_tests.sh**); **run_all_automated_tests.sh**
   aggregates the automated categories. Examples (when enabled) may be
   exercised via **run_all_examples.sh**. Windows hosts have native
   **`.cmd`** counterparts. **woad** currently ships unit tests and a
   scratch version reporter; the component and performance runners exit
   successfully when they find no programs.

5. Install the library on the host, via `cmake`, as in:

   ```bash
   $ sudo cmake --install ${SIS_CMAKE_BUILD_DIR:-./_build} --config Release
   ```

6. Then to use the library, it is a simple matter as follows:

   1. Assuming a simplest possible program to verify the installation:

      ```c
      /* main.c */
      #include <woad/woad.h>

      #include <stdio.h>

      int main(void)
      {
          puts(WOAD_FG_GREEN "ok" WOAD_RESET);

          return 0;
      }
      ```

   2. Compile your project against **woad**:

      Due to the installation step (step 5 above) there is no requirement
      for an explicit include directory for **woad**:

      ```bash
      $ cc -c main.c
      ```

   3. There is no library to link. A header-only include is the whole
      dependency:

      ```bash
      $ cc main.c
      ```

   4. Test your project:

      ```bash
      $ ./a.out
      ```

   Consumers that use **CMake** may instead depend on the installed package:

   ```cmake
   find_package(woad REQUIRED)
   target_link_libraries(your_target PRIVATE woad::woad)
   ```


## Bundled

**woad** is small enough that it is commonly bundled into other projects.
In that case:

* add **woad**'s **include** directory to your project's include path; and
* `#include <woad/woad.h>`.


<!-- ########################### end of file ########################### -->
