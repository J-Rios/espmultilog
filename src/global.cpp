
/* Libraries */

// Header File
#include "global.h"

/*****************************************************************************/

/* Global Elements - Device */

namespace ns_device
{
    /**
     * @brief Device ID String.
     */
    char id[ns_const::MAX_DEVICE_ID_LENGTH] = { 0 };
}

/*****************************************************************************/

/* Global Elements - WiFi */

namespace ns_wifi
{
    /**
     * @brief WiFi Client Object.
     */
    WiFiClient WifiClient;
}

/*****************************************************************************/
