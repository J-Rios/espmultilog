/**
 * @file    main.cpp
 * @author  Jose Miguel Rios Rubio <jrios.github@gmail.com>
 * @date    03-01-2024
 * @version 1.0.0
 *
 * @section DESCRIPTION
 *
 * Main source file of the ESPMULTILOG project.
 *
 * @section LICENSE
 *
 * MIT License
 *
 * Copyright (c) 2023 Jose Miguel Rios Rubio
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

/* Libraries */

// Arduino Framework
#include <Arduino.h>

// Constant Data
#include "constants.h"

// Global Data
#include "global.h"

// MQTT Communication
#include "mqtt.h"

// Network State Library
#include "network_interface.h"

// WiFi Commissioning Portal
#include "wifi_commissioning.h"

/*****************************************************************************/

/* In-Scope Function Prototypes */

/**
 * @brief Configure and initialize the Debug Serial.
 */
static void serial_debug_setup();

/**
 * @brief Configure and initialize the WiFi and MQTT.
 */
static void network_setup();

/*****************************************************************************/

/* Setup & Loop Functions */

void setup()
{
    serial_debug_setup();
    network_setup();
}

void loop()
{
    // Run Standard Managers
    WifiCommissioning.process();

    // Run WiFi Connection Required Managers
    if (Network.available())
    {
        MQTT.process();
    }
}

/*****************************************************************************/

/* In-Scope Functions */

/**
 * @details This function initialize the Debug UART Port and write an App
 * start header information with the project name and version.
 */
static void serial_debug_setup()
{
    Serial.begin(ns_const::DEFAULT_UART_BAUD_RATE);

    Serial.printf("%s (v%d.%d.%d)\n",
        ns_const::PROJECT_NAME,
        (int)(ns_const::FW_APP_VERSION_X),
        (int)(ns_const::FW_APP_VERSION_Y),
        (int)(ns_const::FW_APP_VERSION_Z));
}

/**
 * @details This function initializes the Network component to start tracking
 * network events changes. Then it initializes the WiFi Manager component and
 * request to try WiFi connection to the system configured WiFi Station. At
 * last, it initializes the MQTT communication interface component.
 */
static void network_setup()
{
    Network.init();
    WifiCommissioning.init();
    WifiCommissioning.connect();
    MQTT.init(&(ns_wifi::WifiClient));
}

/*****************************************************************************/
