/**
 * @file    constants.h
 * @author  Jose Miguel Rios Rubio <jrios.github@gmail.com>
 * @date    2024-01-03
 * @version 1.0.0
 *
 * @section DESCRIPTION
 *
 * ESPMULTILOG Constant Data header file.
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

    /**
     * @brief Maximum number of Serial Ports in the device.
     */
    static constexpr uint8_t MAX_NUM_UART = SOC_UART_NUM;

    /**
     * @brief Maximum number of words in a string parsed for
     * command + arguments handling.
     */
    static const uint32_t MAX_STR_ARGV = 5U;

    /**
     * @brief Maximum number of characters for each string in a string
     * parsed for command + arguments handling.
     */
    static const uint32_t MAX_STR_CMD_ARG_LEN = 32U;
}

/*****************************************************************************/

/* Include Guard Close */

#endif /* CONSTANTS_H */
