/**
 * @file    config.h
 * @author  Jose Miguel Rios Rubio <jrios.github@gmail.com>
 * @date    2024-01-03
 * @version 1.0.0
 *
 * @section DESCRIPTION
 *
 * ESPMULTILOG Configuration Data header file.
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
    #define SET_PROJECT_NAME "espmultilog"
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
    /**
     * @brief Default Project Name.
     */
    static const char PROJECT_NAME[] = SET_PROJECT_NAME;

    /**
     * @brief Firmware Application Major Version.
     */
    static const uint8_t FW_APP_VERSION_X = (uint8_t)(SET_FW_APP_VERSION_X);

    /**
     * @brief Firmware Application Minor Version.
     */
    static const uint8_t FW_APP_VERSION_Y = (uint8_t)(SET_FW_APP_VERSION_Y);

    /**
     * @brief Firmware Application Patch Version.
     */
    static const uint8_t FW_APP_VERSION_Z = (uint8_t)(SET_FW_APP_VERSION_Z);

    /**
     * @brief Default WiFi SSID to use for connection.
     */
    static const char WIFI_SSID[] = SET_WIFI_SSID;

    /**
     * @brief Default WiFi PSK Password to use for connection.
     */
    static const char WIFI_PWD[] = SET_WIFI_PWD;

    /**
     * @brief Default Commission WiFi AP PSK Password.
     */
    static const char WIFI_AP_PWD[] = "espmultilog1234";

    /**
     * @brief Default Debug Serial Port Speed.
     */
    static const uint32_t DEFAULT_UART_BAUD_RATE = 115200U;

    /**
     * @brief Default MQTT Server/Broker Host to use.
     */
    static const char* MQTT_SERVER = "test.mosquitto.org";

    /**
     * @brief Default MQTT Server/Broker Port to use.
     */
    static const uint16_t MQTT_PORT = 1883U;

    /**
     * @brief MQTT Standard Control Input topic.
     */
    static const char MQTT_TOPIC_IN[] = "/%s/control/in";

    /**
     * @brief MQTT Standard Control Output topic.
     */
    static const char MQTT_TOPIC_OUT[] = "/%s/control/out";
}

/*****************************************************************************/

/* Include Guard Close */

#endif /* CONFIG_H */
