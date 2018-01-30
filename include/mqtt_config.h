#ifndef _MQTT_CONFIG_H_
#define _MQTT_CONFIG_H_

#include "sdkconfig.h"
#include <stdio.h>

#if CONFIG_MQTT_LOG_ERROR_ON
  #define mqtt_error(...) ESP_LOGE(TAG, __VA_ARGS__)
#else
  #define mqtt_error(...)
#endif

#if CONFIG_MQTT_LOG_WARN_ON
  #define mqtt_warn(...) ESP_LOGW(TAG, __VA_ARGS__)
#else
  #define mqtt_warn(...)
#endif

#if CONFIG_MQTT_LOG_INFO_ON
  #define mqtt_info(...) ESP_LOGI(TAG, __VA_ARGS__)
#else
  #define mqtt_info(...)
#endif

#if !CONFIG_MQTT_QUEUE_BUFFER_SIZE_WORD
  #define CONFIG_MQTT_QUEUE_BUFFER_SIZE_WORD 1024
#endif

#endif
