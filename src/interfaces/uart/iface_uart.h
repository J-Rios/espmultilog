/**
 * @file    iface_uart.h
 * @author  Jose Miguel Rios Rubio <jrios.github@gmail.com>
 * @date    2024-01-04
 * @version 1.0.0
 *
 * @section DESCRIPTION
 *
 * ESPMULTILOG Universal Asynchronous Receiver-Transmitter (UART) Interface
 * header file.
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

#ifndef INTERFACE_UART_H
#define INTERFACE_UART_H

/*****************************************************************************/

/* Libraries */

// C++ Standard Libraries
#include <cstdint>

// Arduino Framework
#include <Arduino.h>

// Constant Data
#include "constants.h"

/*****************************************************************************/

/* Class Interface */

class InterfaceUART
{
    /******************************************************************/

    /* Private Constants */

    private:

        /**
         * @brief Time to send each MQTT message with UART information.
         */
        static constexpr uint32_t T_SEND_MSG = 1000U;

        /**
         * @brief Maximum number of characters for MQTT Topic string.
         */
        static constexpr uint8_t MQTT_TOPIC_MAX_LEN = 32U;

        /**
         * @brief MQTT Topic to configure-enable an UART Port remotely.
         */
        static constexpr char MQTT_TOPIC_CFG[] = "/%s/uart/%d/cfg";

        /**
         * @brief MQTT Topic to publish UART Rx messages on it.
         */
        static constexpr char MQTT_TOPIC_RX[] = "/%s/uart/%d/rx";

        /**
         * @brief MQTT Topic to publish UART Tx messages on it.
         */
        static constexpr char MQTT_TOPIC_TX[] = "/%s/uart/%d/tx";

        /**
         * @brief Maximum number of bytes that can be buffered from each
         * UART Port received data.
         */
        static constexpr uint32_t DATA_RX_BUFFER_SIZE = (256U + 1U);

    /******************************************************************/

    /* Public Constants */

    public:

    /******************************************************************/

    /* Private Data Types */

    private:

    /******************************************************************/

    /* Public Data Types */

    public:

    /******************************************************************/

    /* Public Methods */

    public:

        /**
         * @brief Construct a new Interface object.
         */
        InterfaceUART();

        /**
         * @brief Initializes the Interface.
         * @param device_uuid Pointer to Device UUID string to be used
         * as part of MQTT messages topic.
         */
        void init(const char* device_uuid);

        /**
         * @brief Manage the Interface.
         */
        void process();

        /**
         * @brief Get the MQTT UART Configuration topic for specified
         * Port.
         * @param uart_n UART Port number topic to get.
         * @return char* Address of internal topic attribute.
         */
        const char* get_topic_cfg(const uint8_t uart_n);

        /**
         * @brief Configure an UART Port.
         * @param uart_n UART Port number to configure.
         * @param argc Number of configuration data arguments.
         * @param argv Array of configuration data arguments strings.
         * @return true Configuration success.
         * @return false Configuration fail.
         */
        bool configure(const uint8_t uart_n, int argc, char* argv[]);

        /**
         * @brief Configure the Baud Rate of an UART Port.
         * @param uart_n UART Port number to configure.
         * @param bauds Baud Rate speed to configure.
         * @return true Configuration success.
         * @return false Configuration fail.
         */
        bool uart_config_speed(const uint8_t uart_n, const uint32_t bauds);

        /**
         * @brief Configure the operation mode of an UART Port to handle
         * it as raw bytes or as strings (default).
         * @param uart_n UART Port number to configure.
         * @param enable Enable/Disable the raw byte mode.
         * @return true Configuration success.
         * @return false Configuration fail.
         */
        bool uart_config_mode_bytes(const uint8_t uart_n, const bool enable);

        /**
         * @brief Enable or disable an UART Port to start being
         * monitorized and logged.
         * @param uart_n UART Port number to enable.
         * @param enable Enable/Disable the UART Port.
         * @return true Enable/Disable success.
         * @return false Enable/Disable fail.
         */
        bool uart_enable(const uint8_t uart_n, const bool enable);

        /**
         * @brief Transmit a message through the specified UART Port.
         * The UART Port must be already configured-enabled.
         * The message is also echoed through MQTT Tx topic to
         * acknowledge it transmission.
         * @param uart_n UART Port number to Transmit the message.
         * @param msg Message data to be transmitted.
         * @return true Transmission success.
         * @return false Transmission fail.
         */
        bool uart_tx_msg(const uint8_t uart_n, const char* msg);

    /******************************************************************/

    /* Private Methods */

    private:

        /**
         * @brief Handle reception of UART messages from the specified
         * UART Port (forward received messages from UART to MQTT).
         * @param uart_n UART Port number to handle.
         * @return true Handle successs.
         * @return false Handle fail.
         */
        bool handle_uart_rx(const uint8_t uart_n);

        /**
         * @brief Send an UART Rx message to the component MQTT.
         * @param uart_n UART Port number to publish on it MQTT Topic.
         * @param msg Message payload data to send.
         * @return true Publish success.
         * @return false Publish fail.
         */
        bool publish_rx(const uint8_t uart_n, const char* msg);

        /**
         * @brief Send an UART Tx message to the component MQTT.
         * @param uart_n UART Port number to publish on it MQTT Topic.
         * @param msg Message payload data to send.
         * @return true Publish success.
         * @return false Publish fail.
         */
        bool publish_tx(const uint8_t uart_n, const char* msg);

    /******************************************************************/

    /* Public Attributes */

    public:

    /******************************************************************/

    /* Private Attributes */

    private:

        /**
         * @brief Component initialized status (init() method was call).
         */
        bool initialized;

        /**
         * @brief Pointers to Serial Ports to use.
         */
        HardwareSerial* SerialPort[ns_const::MAX_NUM_UART];

        /**
         * @brief MQTT Topics to configure-enable an UART Port remotely.
         */
        char topic_cfg[ns_const::MAX_NUM_UART][MQTT_TOPIC_MAX_LEN];

        /**
         * @brief MQTT Topics to send UART Rx message.
         */
        char topic_rx[ns_const::MAX_NUM_UART][MQTT_TOPIC_MAX_LEN];

        /**
         * @brief MQTT Topics to send UART Tx message.
         */
        char topic_tx[ns_const::MAX_NUM_UART][MQTT_TOPIC_MAX_LEN];

        /**
         * @brief Received UART data buffers.
         */
        uint8_t rx_data[ns_const::MAX_NUM_UART][DATA_RX_BUFFER_SIZE];

        /**
         * @brief Number of bytes stored in received UART data buffers.
         */
        uint32_t num_data_rx[ns_const::MAX_NUM_UART];

    /******************************************************************/
};

/*****************************************************************************/

/* Object Declaration */

/**
 * @brief Interface Declaration.
 */
extern InterfaceUART IfaceUART;

/*****************************************************************************/

/* Include Guard Close */

#endif /* INTERFACE_UART_H */
