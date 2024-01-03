
/* Include Guard */

#ifndef CONSTANTS_H
#define CONSTANTS_H

/*****************************************************************************/

/* Libraries */

// Standard C++ Libraries
#include <cstdint>

// Configuration Data
#include "config.h"

/*****************************************************************************/

/* System Configuration Constants */

namespace ns_const
{
    // Maximum number of characters for WiFi SSID
    static const uint8_t MAX_WIFI_SSID_LENGTH = 32U;

    // Maximum number of characters for WiFi WPA Key
    static const uint8_t MAX_WIFI_PASSWD_LENGTH = 63U;

    // MAC Address string length (XX:XX:XX:XX:XX:XX)
    static const uint8_t MAC_ADDRESS_LENGTH = 18U;
}

/*****************************************************************************/

/* Include Guard Close */

#endif /* CONSTANTS_H */
