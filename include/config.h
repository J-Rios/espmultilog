
/* Include Guard */

#ifndef CONFIG_H
#define CONFIG_H

/*****************************************************************************/

/* Libraries */

// Standard C++ Libraries
#include <cstdint>

/*****************************************************************************/

/* Configurations */

// Default Project Name
#if !defined(SET_PROJECT_NAME)
    #define SET_PROJECT_NAME "ESPMULTILOG"
#endif

// Default Firmware Application Version if not provided
#if !defined(SET_FW_APP_VERSION_X)
    #define SET_FW_APP_VERSION_X 1
#endif
#if !defined(SET_FW_APP_VERSION_Y)
    #define SET_FW_APP_VERSION_Y 0
#endif
#if !defined(SET_FW_APP_VERSION_Z)
    #define SET_FW_APP_VERSION_Z 0
#endif

// Default WiFi Connection
#if !defined(SET_WIFI_SSID)
    #define SET_WIFI_SSID "MyNet"
#endif
#if !defined(SET_WIFI_PWD)
    #define SET_WIFI_PWD "MyNet123456"
#endif

/*****************************************************************************/

/* System Configuration Constants */

namespace ns_const
{
    // Default Project Name
    static const char PROJECT_NAME[] = SET_PROJECT_NAME;

    // Firmware Application Version
    static const uint8_t FW_APP_VERSION_X = (uint8_t)(SET_FW_APP_VERSION_X);
    static const uint8_t FW_APP_VERSION_Y = (uint8_t)(SET_FW_APP_VERSION_Y);
    static const uint8_t FW_APP_VERSION_Z = (uint8_t)(SET_FW_APP_VERSION_Z);

    // WiFi Default Connection
    static const char WIFI_SSID[] = SET_WIFI_SSID;
    static const char WIFI_PWD[] = SET_WIFI_PWD;

    // Default Debug Serial Port Speed
    static const uint32_t DEFAULT_UART_BAUD_RATE = 115200U;
}

/*****************************************************************************/

/* Include Guard Close */

#endif /* CONFIG_H */
