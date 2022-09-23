add_rules("mode.debug", "mode.release")
set_optimize("fastest")

target("etp_sdk")
    set_kind("static")
    add_files("modulesdk.c")
    on_install(function (target)end)
    on_uninstall(function (target)end)

