#ifndef XDP_PROFILE_CORE_SYSTEM_MONITOR_H_
#define XDP_PROFILE_CORE_SYSTEM_MONITOR_H_

#include <fstream>
#include <mutex>
#include <iostream>
#include <thread>
#include <chrono>

#include "xdp/profile/plugin/ocl/xocl_profile.h"
#include "xdp/profile/plugin/ocl/xocl_plugin.h"

namespace xdp {

enum class PowerProfileStatus {
    IDLE,
    POLLING,
    STOPPING,
    STOPPED
};

class OclPowerProfile {
public:
    OclPowerProfile(xrt::device* xrt_device, std::shared_ptr<XoclPlugin> xocl_plugin);
    ~OclPowerProfile();
    void poll_power();
    bool should_continue();
    void start_polling();
    void stop_polling();
    void write_header();
private:
    std::ofstream power_profiling_output;
    std::mutex status_lock;
    PowerProfileStatus status;
    std::thread polling_thread;
    std::string power_profile_config;
    xrt::device* target_device;
};

}

#endif