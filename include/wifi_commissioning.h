
/* Include Guard */

#ifndef WIFI_COMMISSIONING_H
#define WIFI_COMMISSIONING_H

/*****************************************************************************/

/* Libraries */

// WiFi Commissioning Web Server Library
#include <WiFiManager.h>

/*****************************************************************************/

/* Class Interface */

class WiFiCommissioner
{
    /* Constructor & Destructor */

    public:

        /**
         * @brief Construct a new WiFi Commissioner object.
         */
        WiFiCommissioner();

    /******************************************************************/

    /* Public Methods */

    public:

        /**
         * @brief Initialize the WiFi Commissioner.
         * @return true Initialization success.
         * @return false Initialization fail.
         */
        bool init();

        /**
         * @brief Request WiFi Connection to current configured WiFi.
         * @return true Connection success.
         * @return false Connection fail.
         */
        bool connect();

        /**
         * @brief Run an iteration of the WiFi Commissioner main process.
         */
        void process();

        /**
         * @brief Get the value of the specified configurable parameter (if
         * exists).
         * @param name Parameter name to check.
         * @return String Parameter configured value.
         */
        String param_get(String name);

    /******************************************************************/

    /* Private Attributes */

    private:

        /**
         * @brief WiFi Commissioner initialized status (if it was initialized).
         */
        bool is_initialized;

        /**
         * @brief Internal WiFi Manager object to use.
         */
        WiFiManager _WiFiManager;

        /**
         * @brief WiFi Manager Parameter.
         */
        WiFiManagerParameter custom_field;

    /******************************************************************/
};

/*****************************************************************************/

/* Object Declaration */

extern WiFiCommissioner WifiCommissioning;

/*****************************************************************************/

/* Include Guard Close */

#endif /* WIFI_COMMISSIONING_H */
