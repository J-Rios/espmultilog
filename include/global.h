
/* Include Guard */

#ifndef GLOBAL_H
#define GLOBAL_H

/*****************************************************************************/

/* Libraries */

// Standard C++ Libraries
#include <cstdint>
#include <cstddef>

// Constant Data
#include "constants.h"

// WiFi Library
#include <WiFi.h>

/*****************************************************************************/

/* Global Elements */

namespace ns_device
{
    /**
     * @brief Device ID String.
     */
    extern char id[ns_const::MAX_DEVICE_ID_LENGTH];
}

namespace ns_wifi
{
    /**
     * @brief WiFi Client Object.
     */
    extern WiFiClient WifiClient;
}

/*****************************************************************************/

/* Include Guard Close */

#endif /* GLOBAL_H */
