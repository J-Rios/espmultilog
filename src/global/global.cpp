/**
 * @file    global.cpp
 * @author  Jose Miguel Rios Rubio <jrios.github@gmail.com>
 * @date    2024-01-04
 * @version 1.0.0
 *
 * @section DESCRIPTION
 *
 * ESPMULTILOG Global Data source file.
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

// Header File
#include "global.h"

/*****************************************************************************/

/* Global Elements - Device */

namespace ns_device
{
    /**
     * @brief Device ID String (espmultilog_123456789012).
     */
    char id[ns_const::MAX_DEVICE_ID_LENGTH] = { '\0' };

    /**
     * @brief Device UUID String (123456789012).
     */
    char uuid[ns_const::MAX_UUID_LENGTH] = { '\0' };

    namespace ns_uart
    {
        /**
         * @brief Device UARTs configuration data.
         */
        s_uart_config uart_cfg[ns_const::MAX_NUM_UART];
    }
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

/* Miscellaneous Elements */

namespace ns_misc
{
    /**
     * @brief Struct of string parsed to handle it as a command +
     * arguments.
     */
    s_str_cmd_args cmd_args;
}

/*****************************************************************************/
