#ifndef XDP_HAL_PLUGIN_H_
#define XDP_HAL_PLUGIN_H_

#include <functional>

using cb_open_type = std::function<void(void)>;

void register_cb_open(cb_open_type && cb);

struct hal_api_call_logger
{
  hal_api_call_logger();
  ~hal_api_call_logger();
};

#define XDP_LOG_API_CALL hal_api_call_logger hal_api_call_logger();

#endif