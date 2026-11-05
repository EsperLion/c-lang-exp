# learn

Small C playground for data structures: vector, list, stack, and queue etc.

## Requirements

- [xmake](https://xmake.io)
- A C compiler (clang or gcc)

## Run

```sh
xmake              # build the app
xmake run app      # run it

xmake f -m debug   # switch to a debug build
xmake              # rebuild

xmake clean        # remove build output
```

## Tests

```sh
xmake test         # build and run all tests
xmake test stack.test/stack   # run one test
```

## Layout

```
include/   headers
src/       implementations and main.c
tests/     test programs (each has its own main)
xmake.lua  build configuration
```

## Add a source file

Create the file in `src/`, then add it to the `add_files(...)` list of the
target that needs it. For example, to add `src/deque.c` to the app:

```lua
target("app")
    set_kind("binary")
    add_files(
        "src/main.c",
        "src/vector.c",
        "src/list.c",
        "src/stack.c",
        "src/deque.c"
    )
```

Headers in `include/` are already on the include path, so use:

```c
#include "deque.h"
```

## Add a test

Create `tests/deque.test.c` with a `main()` that returns `0` on success.
Then add a target at the end of `xmake.lua`:

```lua
target("deque.test")
    set_kind("binary")
    set_default(false)
    add_files(
        "tests/deque.test.c",
        "src/deque.c"
    )
    add_tests("deque")
```

`xmake test` will build and run it automatically.
