add_rules("mode.debug", "mode.release")
set_optimize("fastest")

target("log")
    set_kind("static")
    add_files("log.c")

