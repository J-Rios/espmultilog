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

/*****************************************************************************/

/* Function Implementations */

/**
 * @details This function populate the device ID string global element with
 * the "PROJECT_NAME+UUID" (UUID == MAC Address bytes).
 */
char* get_device_id()
{
    static_assert(
        ( sizeof(ns_const::PROJECT_NAME) + ns_const::MAX_UUID_LENGTH )
        <= ns_const::MAX_DEVICE_ID_LENGTH,
        "PROJECT_NAME too long for Device ID"
    );

    get_device_uuid();

    snprintf(ns_device::id, ns_const::MAX_DEVICE_ID_LENGTH, "%s_%s",
        ns_const::PROJECT_NAME, ns_device::uuid);

    return ns_device::id;
}

/**
 * @details This function creates a string based on device MAC Address bytes
 * to be used as a device UUID.
 */
char* get_device_uuid()
{
    uint8_t mac[ns_const::MAC_ADDRESS_NUM_BYTES];

    if (WiFiGenericClass::getMode() == WIFI_MODE_NULL)
    {   esp_read_mac(mac, ESP_MAC_WIFI_STA);   }
    else
    {   esp_wifi_get_mac((wifi_interface_t)ESP_IF_WIFI_STA, mac);   }

    snprintf(ns_device::uuid, ns_const::MAX_UUID_LENGTH,
        "%02X%02X%02X%02X%02X%02X",
        mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

    return ns_device::uuid;
}

/**
 * @details This function iterate over each string of the array of strings and
 * concatenate that string into the user provided "str_out" string, then an
 * end of string null character is added at the end of the out string to safe
 * ensure the out string will be closed.
 * In case the
 * If any string has been copied, the function will return true.
 */
bool single_str_from_array_of_str(int argc, char* array_str[],
        char* str_out, const size_t str_out_len)
{
    bool something_appended = false;

    for (int i = 0; i < argc; i++)
    {
        const char* str_n = array_str[i];
        strncat(str_out, str_n, str_out_len);
        str_out[str_out_len-1U] = '\0';
        something_appended = true;
    }

    return something_appended;
}

/**
 * @details This function count the number of words in the provided "str_in"
 * string, store it in the provided "s_str_cmd_args->argc", then use it to
 * loop searching for a space character using a pointer in order to find the
 * start address of each word and copy each word to the provided
 * "s_str_cmd_args->argv" array of strings to return it back.
 */
void str_parse_cmd_args(char* str_in, ns_misc::s_str_cmd_args* cmd_args)
{
    // Get Arguments
    char* ptr_data = str_in;
    char* ptr_argv = NULL;

    // Clear any previous parse result
    cmd_args->argc = 0U;
    cmd_args->cmd[0] = '\0';
    for (uint8_t i = 0; i < ns_const::MAX_STR_ARGV; i++)
    {   cmd_args->argv[i][0] = '\0';   }

    // Count number of words in the string
    cmd_args->argc = str_count_words(str_in);

    for (uint8_t i = 0; i < cmd_args->argc; i++)
    {
        // Break if number of arguments is large than buffer
        if (i >= ns_const::MAX_STR_ARGV)
        {   break;   }

        // Point to next argument
        ptr_argv = strstr(ptr_data, " ");
        if (ptr_argv == NULL)
        {
            // No ' ' character found, so it is last command, lets get it
            strncpy(cmd_args->argv[i], ptr_data, strlen(ptr_data));
            cmd_args->argv[i][ns_const::MAX_STR_CMD_ARG_LEN-1] = '\0';
            break;
        }
        ptr_data = ptr_data + (ptr_argv - ptr_data) + 1;
        ptr_argv = ptr_argv + 1;

        // Get the argument
        str_read_until_char(ptr_argv, strlen(ptr_argv), ' ',
            cmd_args->argv[i], ns_const::MAX_STR_CMD_ARG_LEN);
    }
}

/**
 * @details
 * This function loop through provided str_in string characters searching for
 * "X Y" pattern to increase the counter of words until the end of the string.
 */
uint32_t str_count_words(const char* str_in)
{
    uint32_t n = 1;

    // Check if string is empty
    if (str_in == nullptr)
    {   return 0;   }
    if (str_in[0] == '\0')
    {   return 0;   }

    // Check if string just has 1 character
    const size_t str_in_len = sizeof(str_in);
    if (str_in_len == 1)
    {   return 1;   }

    // Check for character occurrences
    for (size_t i = 1; i < str_in_len; i++)
    {
        // Check if end of string detected
        if (str_in[i] == '\0')
        {   break;   }

        // Check if pattern "X Y", "X\rY" or "X\nY" does not meet
        if ((str_in[i] != ' ') && (str_in[i] != '\r') && (str_in[i] != '\n'))
        {   continue;   }
        if ((str_in[i-1] == ' ') || (str_in[i-1] == '\r') ||
                (str_in[i-1] == '\n'))
        {   continue;   }
        if ((str_in[i+1] == ' ') || (str_in[i+1] == '\r') ||
                (str_in[i+1] == '\n'))
        {   continue;   }
        if (str_in[i+1] == '\0')
        {   continue;   }

        // Pattern detected, increase word count
        n = n + 1;
    }

    return n;
}

/**
 * @details
 * This function loop for each character of the provided string checking for
 * the requested "until" character while copying each character into the
 * str_read array.
 */
bool str_read_until_char(char* str, const size_t str_len, const char until_c,
        char* str_read, const size_t str_read_size)
{
    size_t i = 0;
    bool found = false;

    str_read[0] = '\0';
    while (i < str_len)
    {
        if (str[i] == until_c)
        {
            found = true;
            break;
        }
        if (i < str_read_size)
        {   str_read[i] = str[i];   }
        i = i + 1;
    }
    str_read[str_read_size-1] = '\0';
    if (i < str_read_size)
    {   str_read[i] = '\0';   }

    return found;
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
