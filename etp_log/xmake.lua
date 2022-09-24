add_rules("mode.debug", "mode.release")
set_optimize("fastest")

target("etp_static_log")
    set_kind("static")
    add_files("log.cpp")

