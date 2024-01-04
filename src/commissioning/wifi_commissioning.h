/**
 * @file    wifi_commissioning.h
 * @author  Jose Miguel Rios Rubio <jrios.github@gmail.com>
 * @date    2024-01-03
 * @version 1.0.0
 *
 * @section DESCRIPTION
 *
 * ESPMULTILOG WiFi Commissioning header file.
 *
 * @section LICENSE
 *
 * MIT License
 *
 * Copyright (c) 2024 Jose Miguel Rios Rubio
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

/*****************************************************************************/

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
