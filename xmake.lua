add_rules("mode.debug", "mode.release")

set_languages("c11")
add_includedirs("include")

-- -------- Main application --------
target("app")
    set_kind("binary")
    add_files(
        "src/main.c",
        "src/vector.c",
        "src/list.c",
        "src/stack.c"
    )

-- -------- Tests --------
target("stack.test")
    set_kind("binary")
    set_default(false)
    add_files(
        "tests/stack.test.c",
        "src/stack.c",
        "src/vector.c"
    )
    add_tests("stack")

target("queue.test")
    set_kind("binary")
    set_default(false)
    add_files(
        "tests/queue.test.c",
        "src/queue.c",
        "src/list.c"
    )
    add_tests("queue")
