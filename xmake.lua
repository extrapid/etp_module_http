add_rules("mode.debug", "mode.release")
set_optimize("fastest")
add_cxxflags("-shared -fPIC")
add_syslinks("dl", "pthread")

includes("etp_sdk/xmake.lua")
includes("etp_log/xmake.lua")

target("deps")
    set_kind("static")
    add_files("addone.cpp")
    add_files("cJSON.cpp")
    add_files("connectClient.cpp")
    add_files("handClient.cpp")
    add_files("str.cpp")
    on_install(function (target)                         end)                                                 on_uninstall(function (target)                       end)

target("etp_module_http")
    set_kind("shared")
    set_filename("etp_module_http.so")
    add_deps("etp_sdk", "etp_log", "deps")
    add_files("main.cpp")
    add_files("init.cpp")

