#include <iostream>
#include "xdp_hal_plugin_interface.h"
#include "xdp_hal_plugin.h"
#include "driver/xclng/xrt/user_gem/plugin/xdp/profile.h"

void open_cb_func() {
    std::cout << "the open callback is called" << std::endl;
    return;
}

void register_cb_funcs() {
    register_cb_open(open_cb_func);
    return;
}

void init_xdp_hal_plugin() {
    std::cout << "init_xdp_hal_plugin called" << std::endl;
    register_cb_funcs();
    return;
}
