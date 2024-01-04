
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
    /**
     * @brief Maximum number of characters for WiFi SSID.
     */
    static const uint8_t MAX_WIFI_SSID_LENGTH = 32U;

    /**
     * @brief Maximum number of characters for WiFi WPA Key.
     */
    static const uint8_t MAX_WIFI_PASSWD_LENGTH = 63U;

    /**
     * @brief MAC Address number of byte (XX:XX:XX:XX:XX:XX).
     */
    static const uint8_t MAC_ADDRESS_NUM_BYTES = 6U;

    /**
     * @brief MAC Address string length (XX:XX:XX:XX:XX:XX).
     */
    static const uint8_t MAC_ADDRESS_LENGTH = 18U;

    /**
     * @brief Maximum Length of UUID ("xxxxxxxxxxxx").
     */
    static constexpr uint8_t MAX_UUID_LENGTH = 13U;

    /**
     * @brief Maximum number of characters for Device ID.
     */
    static constexpr uint8_t MAX_DEVICE_ID_LENGTH = (MAX_UUID_LENGTH + 32U);

    /**
     * @brief Maximum number of characters expected for a MQTT Topic.
     */
    static constexpr uint8_t MQTT_TOPIC_MAX_LEN = (MAC_ADDRESS_LENGTH + 25U);
}

/*****************************************************************************/

/* Include Guard Close */

#endif /* CONSTANTS_H */
