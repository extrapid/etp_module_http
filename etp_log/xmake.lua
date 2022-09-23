add_rules("mode.debug", "mode.release")
set_optimize("fastest")

target("etp_log")
    set_kind("shared")
    add_files("log.c")

