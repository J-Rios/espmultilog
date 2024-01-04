/**
 * @file    misc.cpp
 * @author  Jose Miguel Rios Rubio <jrios.github@gmail.com>
 * @date    2024-01-03
 * @version 1.0.0
 *
 * @section DESCRIPTION
 *
 * ESPMULTILOG Miscellaneous Functionalities source file.
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

/* Libraries */

// Header Interface
#include "misc.h"

// Hardware Abstraction Layer Framework
#include "Arduino.h"

// ESP32 WiFi Library
#include "WiFi.h"
#include "esp_wifi.h"

// Constant Data
#include "constants.h"

// Global Data
#include "../global/global.h"

/*****************************************************************************/

/* Function Implementations */

/**
 * @details This function populate the device ID string global element with
 * the "PROJECT_NAME+UUID" (UUID == MAC Address bytes).
 */
char* get_device_id()
{
    char uuid[ns_const::MAX_UUID_LENGTH] = { 0 };
    get_device_uuid(uuid, ns_const::MAX_UUID_LENGTH);

    static_assert(
        ( sizeof(ns_const::PROJECT_NAME) + ns_const::MAX_UUID_LENGTH )
        <= ns_const::MAX_DEVICE_ID_LENGTH,
        "PROJECT_NAME too long for Device ID"
    );

    snprintf(ns_device::id, ns_const::MAX_DEVICE_ID_LENGTH, "%s_%s",
        ns_const::PROJECT_NAME, uuid);

    return ns_device::id;
}

/**
 * @details This function creates a string based on device MAC Address bytes
 * to be used as a device UUID.
 */
void get_device_uuid(char* uuid, const uint32_t uuid_size)
{
    uint8_t mac[ns_const::MAC_ADDRESS_NUM_BYTES];

    if (WiFiGenericClass::getMode() == WIFI_MODE_NULL)
    {   esp_read_mac(mac, ESP_MAC_WIFI_STA);   }
    else
    {   esp_wifi_get_mac((wifi_interface_t)ESP_IF_WIFI_STA, mac);   }

    snprintf(uuid, uuid_size, "%02X%02X%02X%02X%02X%02X",
        mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
}

/**
 * @details Safe conversion a string number into uint8_t element.
 */
t_return_code safe_atoi_u8(const char* in_str, const size_t in_str_len,
    uint8_t* out_int, bool check_null_terminated)
{
    t_return_code rc = t_return_code::RC_BAD;
    uint32_t u32_val = 0;

    // Check if input str length is large than expected for an uint8_t
    if (in_str_len > 4)
    {   return t_return_code::RC_INVALID_INPUT;   }

    rc = safe_atoi_u32(in_str, in_str_len, &u32_val, check_null_terminated);
    if (rc == t_return_code::RC_OK)
    {   *out_int = (uint8_t)(u32_val & 0xFF);   }

    return rc;
}

/**
 * @details Safe conversion a string number into uint32_t element.
 */
t_return_code safe_atoi_u32(const char* in_str, const size_t in_str_len,
    uint32_t* out_int, bool check_null_terminated)
{
    size_t converted_num;
    size_t multiplicator;

    // Check if input str has less or more chars than expected int32_t
    // range (1 to 3 chars)
    if ((in_str_len < 1) || (in_str_len > 10))
    {   return t_return_code::RC_INVALID_INPUT;   }

    // Check if input str is not terminated
    if (check_null_terminated)
    {
        if (in_str[in_str_len] != '\0')
        {   return t_return_code::RC_INVALID_INPUT;   }
    }

    // Check if any of the character of the str is not a number
    for (uint8_t i = 0; i < in_str_len; i++)
    {
        if (in_str[i] < '0' || in_str[i] > '9')
        {   return t_return_code::RC_BAD;   }
    }

    // Create the int
    converted_num = 0;
    for (uint8_t i = 0; i < in_str_len; i++)
    {
        multiplicator = 1;
        for (uint8_t ii = in_str_len-1-i; ii > 0; ii--)
        {   multiplicator = multiplicator * 10;   }

        converted_num = converted_num + (multiplicator * (in_str[i] - '0'));
    }

    // Check if number is higher than max uint32_t val
    if (converted_num > UINT32_MAX)
    {   return t_return_code::RC_BAD;   }

    // Get the converted number and return operation success
    *out_int = (uint32_t)converted_num;
    return t_return_code::RC_OK;
}

/*****************************************************************************/
