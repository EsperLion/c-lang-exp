# Sanitizer examples

These small programs contain bugs on purpose. Compile them **without**
sanitizers and they may appear to work. Compile them with sanitizers and the
bugs are reported with file/line information.

Build one example into the ignored `build/` directory:

```sh
mkdir -p build/sanitizers

# AddressSanitizer + UndefinedBehaviorSanitizer
clang -std=c11 -g -O0 \
  -fsanitize=address,undefined -fno-omit-frame-pointer \
  examples/sanitizers/heap_overflow.c \
  -o build/sanitizers/heap_overflow

./build/sanitizers/heap_overflow
```

Try the same command with the other files:

- `heap_overflow.c` — writes past the end of a heap allocation (ASan)
- `use_after_free.c` — reads memory after `free` (ASan)
- `signed_overflow.c` — overflows a signed `int` (UBSan)

UBSan reports an error but keeps running by default. Add
`-fno-sanitize-recover=all` to the compile command if you want it to stop
immediately (useful when running tests).

To build the whole project with sanitizers:

```sh
xmake f -c -m debug \
  --cxflags="-fsanitize=address,undefined -fno-omit-frame-pointer -fno-sanitize-recover=all" \
  --ldflags="-fsanitize=address,undefined -fno-sanitize-recover=all"
xmake test
```

Then reset the configuration before making a normal build:

```sh
xmake f -c -m release
```
