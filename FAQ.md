# woad - FAQ <!-- omit in toc -->

The FAQ list is under (constant) development. If you post a question on the
Issues forum (https://github.com/synesissoftware/woad/issues)
it will be used to create one.


## Table of Contents <!-- omit in toc -->

- [Q1: "How do I build woad?"](#q1-how-do-i-build-woad)
- [Q2: "How do I install woad?"](#q2-how-do-i-install-woad)
- [Q3: "How do I use woad?"](#q3-how-do-i-use-woad)
- [Q4: "Does woad have its own unit-tests?"](#q4-does-woad-have-its-own-unit-tests)
- [Q5: "Where are the examples?"](#q5-where-are-the-examples)


# FAQs: <!-- omit in toc -->


## Q1: "How do I build woad?"

See [INSTALL.md](./INSTALL.md) for the recommended **CMake** flow
(**prepare_cmake.sh**, then **build_cmake.sh**).

```bash
$ ./prepare_cmake.sh -m
```

Execute `$ ./prepare_cmake.sh --help` for the full set of options.


## Q2: "How do I install woad?"

See [INSTALL.md](./INSTALL.md) for details of how to install **woad**.


## Q3: "How do I use woad?"

Include **woad/woad.h**. There is no library to link. The **CMake** target
is `woad::woad`.

A minimal sketch:

```c
#include <woad/woad.h>

#include <stdio.h>

int main(void)
{
    puts(WOAD_FG_GREEN "ok" WOAD_RESET);

    return 0;
}
```

See [INSTALL.md](./INSTALL.md) and the examples under **examples/**.


## Q4: "Does woad have its own unit-tests?"

Yes. Automated tests live under:

* **./test/unit** — unit tests (`test.unit.codes`, `test.unit.version`);
* **./test/scratch** — scratch / exploratory programs (**test.scratch.versions**);

**woad** does not yet ship component or performance programs. Those runners
exit successfully when they find none.

When testing is enabled, build them via **prepare_cmake.sh** / **build_cmake.sh**
and run with **run_all_unit_tests.sh** (and **CTest** where configured). The
tests need only the C toolchain used to build the project.


## Q5: "Where are the examples?"

Examples live under **examples/c/**, each built as `example.c.<subject>`.
They are built when `BUILD_EXAMPLES` is on (the default); omit them with
`--disable-examples` / `-E`. Run built examples via **run_all_examples.sh**.
See the examples table in [README.md](./README.md).


<!-- ########################### end of file ########################### -->
