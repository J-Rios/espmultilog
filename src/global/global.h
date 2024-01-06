/**
 * @file    global.h
 * @author  Jose Miguel Rios Rubio <jrios.github@gmail.com>
 * @date    2024-01-04
 * @version 1.0.0
 *
 * @section DESCRIPTION
 *
 * ESPMULTILOG Global Data header file.
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

#ifndef GLOBAL_H
#define GLOBAL_H

/*****************************************************************************/

/* Libraries */

// Standard C++ Libraries
#include <cstdint>
#include <cstddef>

// WiFi Library
#include <WiFi.h>

// Constant Data
#include "constants.h"

/*****************************************************************************/

/* Global Elements */

namespace ns_device
{
    /**
     * @brief Device ID String (espmultilog_123456789012).
     */
    extern char id[ns_const::MAX_DEVICE_ID_LENGTH];

    /**
     * @brief Device UUID String (123456789012).
     */
    extern char uuid[ns_const::MAX_UUID_LENGTH];

    namespace ns_uart
    {
        /**
         * @brief Device UART configuration data.
         */
        struct s_uart_config
        {
            // UART Port configuration and enable/disable state
            bool enable;

            // Operation Mode for UART Rx and Tx as raw bytes instead of
            // strings (CLI)
            bool mode_raw_bytes;

            // UART Baud Rate
            uint32_t bauds;

            #if 0 /* Full parameters configuration is not supported */
                // UART Port configuration
                uart_config_t config;
            #endif

            // Default struct initialization
            s_uart_config()
            {
                enable = false;
                mode_raw_bytes = false;
                bauds = ns_const::DEFAULT_UART_BAUD_RATE;
            #if 0 /* Full parameters configuration is not supported */
                config.data_bits = UART_DATA_8_BITS;
                config.stop_bits = UART_STOP_BITS_1;
                config.parity = UART_PARITY_DISABLE;
                config.flow_ctrl = UART_HW_FLOWCTRL_CTS_RTS;
                config.rx_flow_ctrl_thresh = 122;
                #if SOC_UART_SUPPORT_XTAL_CLK
                    config.source_clk = UART_SCLK_XTAL;
                    config.baud_rate = (int)(ns_const::DEFAULT_UART_BAUD_RATE);
                #else
                    config.source_clk = UART_SCLK_APB;
                    config.baud_rate = _get_effective_baudrate(
                        ns_const::DEFAULT_UART_BAUD_RATE);
                #endif
            #endif
            }
        };

        /**
         * @brief Device UARTs configuration data.
         */
        extern s_uart_config uart_cfg[ns_const::MAX_NUM_UART];
    }
}

namespace ns_wifi
{
    /**
     * @brief WiFi Client Object.
     */
    extern WiFiClient WifiClient;
}

namespace ns_misc
{
    /**
     * @brief Struct of string parsed to handle it as a command +
     * arguments.
     */
    struct s_str_cmd_args
    {
        char cmd[ns_const::MAX_STR_CMD_ARG_LEN];
        char argv[ns_const::MAX_STR_ARGV][ns_const::MAX_STR_CMD_ARG_LEN];
        uint8_t argc;
    };

    /**
     * @brief Struct of string parsed to handle it as a command +
     * arguments.
     */
    extern s_str_cmd_args cmd_args;
}

/*****************************************************************************/

/* Include Guard Close */

#endif /* GLOBAL_H */
