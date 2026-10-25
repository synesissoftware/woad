# woad {#mainpage}

**woad** is a small C library of minimal ANSI terminal colour codes. Fixed
SGR sequences always emit. Stream-conditional counterparts emit only when
the destination warrants colour, with the decision overridable from the
environment.


## Components

| Module | Header | Summary |
| ------ | ------ | ------- |
| @ref group__woad | `<woad/woad.h>` | Fixed sequences and stream-conditional colour |
| @ref group__woad__version | `<woad/woad.h>` | `WOAD_VER`, `woad_version()`, `woad_version_string()` |
| @ref group__woad__sgr | `<woad/woad.h>` | Compile-time `WOAD_FG_*`, `WOAD_BG_*`, bold, and blinking literals |
| @ref group__woad__policy | `<woad/woad.h>` | `WOAD_COLOUR`, `NO_COLOR`, `CLICOLOR`, and console capability |
| @ref group__woad__inspection | `<woad/woad.h>` | Terminal attachment and per-destination colour support |
| @ref group__woad__sequences | `<woad/woad.h>` | `WOAD_*_FOR()` and `woad_seq_for()` |
| @ref group__woad__iostreams | `<woad/iostreams.hpp>` | [C++ only] Stream queries, manipulators, and inserters for IOStreams |
| @ref group__woad__iostreams__manipulators | `<woad/iostreams.hpp>` | `woad::green`, `woad::reset`, ... applied to subsequent output |
| @ref group__woad__iostreams__inserters | `<woad/iostreams.hpp>` | `woad::green("ok")`, ... applied to one message |


## Quick start

The fixed macros need only the header:

```c
#include <woad/woad.h>

#include <stdio.h>

int main(void)
{
    puts(WOAD_FG_GREEN "ok" WOAD_RESET);

    return 0;
}
```

The stream-conditional API is a compiled library. Link `woad::woad` and
pass the destination:

```c
#include <woad/woad.h>

#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    printf("%sok%s\n", WOAD_FG_GREEN_FOR(stdout), WOAD_RESET_FOR(stdout));

    return EXIT_SUCCESS;
}
```

See
[INSTALL.md](https://github.com/synesissoftware/woad/blob/master/INSTALL.md)
for build and install instructions. Regenerate this HTML with
`./generate_doxygen.sh` (output under `_build/doxygen/html/`).


## Related projects

The other **woad** implementations:

| Project | Language |
| ------- | -------- |
| [woad.Go](https://github.com/synesissoftware/woad.Go/) | Go |
| [woad.NET](https://github.com/synesissoftware/woad.NET/) | .NET |
| [woad.Python](https://github.com/synesissoftware/woad.Python/) | Python |
| [woad.Ruby](https://github.com/synesissoftware/woad.Ruby/) | Ruby |
| [woad.Rust](https://github.com/synesissoftware/woad.Rust/) | Rust |
| [woad.Zig](https://github.com/synesissoftware/woad.Zig/) | Zig |


<!-- ########################### end of file ########################### -->
