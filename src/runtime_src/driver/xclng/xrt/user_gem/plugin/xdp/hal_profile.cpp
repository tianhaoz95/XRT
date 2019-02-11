#include <dlfcn.h>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include "plugin/xdp/hal_profile.h"

namespace bfs = boost::filesystem;

namespace xdphal {

cb_probe_type cb_probe = nullptr;

int test = 10;

bool HalCallLogger::loaded = false;

static boost::filesystem::path&
dllExt()
{
  static boost::filesystem::path sDllExt(".so");
  return sDllExt;
}

inline bool
isDLL(const bfs::path& path)
{
  return (bfs::exists(path)
          && bfs::is_regular_file(path)
          && path.extension()==dllExt());
}

static void
directoryOrError(const bfs::path& path)
{
  if (!bfs::is_directory(path))
    throw std::runtime_error("No such directory '" + path.string() + "'");
}

static const char*
emptyOrValue(const char* cstr)
{
  return cstr ? cstr : "";
}

HalCallLogger::HalCallLogger(int x) {
    std::cout << "hal_api_call_logger is being called" << std::endl;
    std::cout << "checking cb_probe in HalCallLogger" << std::endl;
    std::cout << "value of cb_probe: " << (bool)cb_probe << std::endl;
    if (cb_probe) {
        cb_probe();
    } else {
        std::cout << "cb_probe is not registered" << std::endl;
    }
    std::cout << "hal_api_call_logger the value of test: " << test << std::endl;
    std::cout << "test int address: 0x" << std::hex << &test << std::dec << std::endl;
    return;
}

HalCallLogger::~HalCallLogger() {
    return;
}

void load_xdp_plugin_library() {
    bfs::path xrt(emptyOrValue(getenv("XILINX_XRT")));
    bfs::path libname ("libxdp_hal_plugin.so");
    if (xrt.empty()) {
        throw std::runtime_error("Library " + libname.string() + " not found! XILINX_XRT not set");
    }
    bfs::path p(xrt / "lib");
    directoryOrError(p);
    p /= libname;
    if (!isDLL(p)) {
        throw std::runtime_error("Library " + p.string() + " not found!");
    }
    auto handle = dlopen(p.string().c_str(), RTLD_NOW | RTLD_GLOBAL);
    if (!handle)
        throw std::runtime_error("Failed to open XDP hal plugin library '" + p.string() + "'\n" + dlerror());

    typedef void (* xdpInitType)();

    const std::string s = "init_xdp_hal_plugin";
    auto initFunc = (xdpInitType)dlsym(handle, s.c_str());
    if (!initFunc)
        throw std::runtime_error("Failed to initialize XDP library, '" + s +"' symbol not found.\n" + dlerror());

    const std::string probe_cb_func_name = "probe_cb_func";
    typedef void (* cb_probe_load_type)();
    cb_probe = cb_probe_type((cb_probe_load_type)dlsym(handle, probe_cb_func_name.c_str()));
    
    std::cout << "testing cb_probe the new way" << std::endl;
    cb_probe();

    std::cout << "checking cb_probe after initFunc" << std::endl;
    std::cout << "value of cb_probe: " << (bool)cb_probe << std::endl;

    HalCallLogger::loaded = true;
}

}
