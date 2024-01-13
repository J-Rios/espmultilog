/**
 * @file    iface_uart.cpp
 * @author  Jose Miguel Rios Rubio <jrios.github@gmail.com>
 * @date    2024-01-04
 * @version 1.0.0
 *
 * @section DESCRIPTION
 *
 * ESPMULTILOG Universal Asynchronous Receiver-Transmitter (UART) Interface
 * source file.
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
#include "iface_uart.h"

// C++ Standard Libraries
#include <cstring>

// Global Data
#include "../../global/global.h"

// Miscellaneous Functions
#include "../../misc/misc.h"

// MQTT Communication
#include "../../mqtt/mqtt.h"

/*****************************************************************************/

/* Object Instantiation */

/**
 * @brief Interface Object.
 */
InterfaceUART IfaceUART;

/*****************************************************************************/

/* Public Methods */

/**
 * @details The constructor of the class initializes all it internal
 * attributes to default values.
 */
InterfaceUART::InterfaceUART()
{
    initialized = false;
    for (uint8_t i = 0U; i < ns_const::MAX_NUM_UART; i++)
    {
        SerialPort[i] == nullptr;
        memset((void*)(topic_cfg[i]), 0, ns_const::MQTT_TOPIC_MAX_LEN);
        memset((void*)(topic_rx[i]), 0, ns_const::MQTT_TOPIC_MAX_LEN);
        memset((void*)(topic_tx[i]), 0, ns_const::MQTT_TOPIC_MAX_LEN);
        for (uint8_t ii = 0U; ii < DATA_RX_BUFFER_SIZE; ii++)
        {   rx_data[i][ii] = 0U;   }
        num_data_rx[i] = 0U;
    }
    msg_status_port_n = 1U;
    t_last_status_sent = 0U;
}

/**
 * @details The init method of the Interface initializes all the required
 * elements of the Interface for the logging.
 */
void InterfaceUART::init(const char* device_uuid)
{
    // Assign Serial Port Objects
    SerialPort[0] = &Serial;
    #if SOC_UART_NUM > 1
        SerialPort[1] = &Serial1;
    #endif
    #if SOC_UART_NUM > 2 // ESP32-C3 only has 2 UARTs
        SerialPort[2] = &Serial2;
    #endif

    // Prepare MQTT Topics
    snprintf(topic_status, sizeof(topic_status), MQTT_TOPIC_STATUS,
        device_uuid);
    for (uint8_t i = 0U; i < ns_const::MAX_NUM_UART; i++)
    {
        snprintf(topic_cfg[i], sizeof(topic_cfg[i]),
            MQTT_TOPIC_CFG, device_uuid);
        snprintf(topic_rx[i], sizeof(topic_rx[i]), MQTT_TOPIC_RX, device_uuid);
        snprintf(topic_tx[i], sizeof(topic_tx[i]), MQTT_TOPIC_TX, device_uuid);
    }

    // Init counter for UART Status info MQTT messages send
    t_last_status_sent = millis();

    initialized = true;
}

/**
 * @details The process method of the Interface manage how the interface must
 * be handled for logging.
 */
void InterfaceUART::process()
{
    // Do nothing if component was not initialized
    if (initialized == false)
    {   return;   }

    // Handle Serial Ports Message Receptions
    for (uint8_t i = 0U; i < ns_const::MAX_NUM_UART; i++)
    {   handle_uart_rx(i);   }

    // Send current UART status information each second to MQTT
    if (millis() - t_last_status_sent >= T_SEND_STATUS_INFO_MS)
    {
        mqtt_send_uart_status_info();
        t_last_status_sent = millis();
    }
}

/**
 * @details Getter method to return any of the internal MQTT UART Configuration
 * topic attribute.
 */
const char* InterfaceUART::get_topic_cfg(const uint8_t uart_n)
{
    // Do nothing if specified UART Port number is invalid
    if (uart_n >= ns_const::MAX_NUM_UART)
    {   return nullptr;   }

    return topic_cfg[uart_n];
}

/**
 * @details Getter method to return any of the internal MQTT UART Transmission
 * topic attribute.
 */
const char* InterfaceUART::get_topic_tx(const uint8_t uart_n)
{
    // Do nothing if specified UART Port number is invalid
    if (uart_n >= ns_const::MAX_NUM_UART)
    {   return nullptr;   }

    return topic_tx[uart_n];
}

/**
 * @details Check type of configuration command string was requested by the
 * "data" argument, then call to the corresponding configuration method.
 */
bool InterfaceUART::configure(const uint8_t uart_n, int argc, char* argv[])
{
    bool cfg_success = false;

    // Do nothing if not enough number of arguments provided
    if (argc == 0U)
    {   return false;   }

    // Point to configuration command and argument
    char* cmd = argv[0];
    char* arg = argv[1];

    // UART Port Logging Enable
    if (strcmp(cmd, "enable") == 0)
    {   cfg_success = uart_enable(uart_n, true);   }

    // UART Port Logging Disable
    else if (strcmp(cmd, "disable") == 0)
    {   cfg_success = uart_enable(uart_n, false);   }

    // UART Port Configure Baudrate
    else if (strcmp(cmd, "bauds ") == 0)
    {
        // Try to convert baudrate argument string to u32
        uint32_t bauds = ns_const::DEFAULT_UART_BAUD_RATE;
        t_return_code convert_rc = safe_atoi_u32(arg, sizeof(arg), &bauds);
        if (convert_rc != t_return_code::RC_OK)
        {   return false;   }

        cfg_success = uart_config_speed(uart_n, bauds);
    }

    // Unknown/Unexpected config
    else
    {   return false;   }

    return cfg_success;
}

/**
 * @details This function is a setter to configure an UART Port speed by
 * modifying the value of the Global uart_cfg bauds field.
 */
bool InterfaceUART::uart_config_speed(const uint8_t uart_n,
        const uint32_t bauds)
{
    // Do nothing if component was not initialized
    if (initialized == false)
    {   return false;   }

    // Do nothing for UART0 that is used as device CLI
    if (uart_n == 0U)
    {   return false;   }

    // Do nothing if specified UART Port number is invalid
    if (uart_n >= ns_const::MAX_NUM_UART)
    {   return false;   }

    ns_device::ns_uart::uart_cfg[uart_n].bauds = bauds;

    return true;
}

/**
 * @details This function is a setter to enable or disable an UART Port by
 * modifying the value of the Global uart_cfg enable field.
 */
bool InterfaceUART::uart_enable(const uint8_t uart_n, const bool enable)
{
    // Do nothing if component was not initialized
    if (initialized == false)
    {   return false;   }

    // Do nothing for UART0 that is used as device CLI
    if (uart_n == 0U)
    {   return false;   }

    // Do nothing if specified UART Port number is invalid
    if (uart_n >= ns_const::MAX_NUM_UART)
    {   return false;   }

    ns_device::ns_uart::uart_cfg[uart_n].enable = enable;

    return true;
}

/**
 * @details This function checks that the provided UART Port number is valid
 * and has been enabled, then it echoe the message to Transmitt by the UART
 * trhrough the MQTT Tx topic to acknowledge it transmission, at the end, the
 * message is transmitted through the corresponding UART port.
 */
bool InterfaceUART::uart_tx_msg(const uint8_t uart_n, const char* msg)
{
    // Do nothing if component was not initialized
    if (initialized == false)
    {   return false;   }

    // Do nothing for UART0 that is used as device CLI
    if (uart_n == 0U)
    {   return false;   }

    // Do nothing if specified UART Port number is invalid
    if (uart_n >= ns_const::MAX_NUM_UART)
    {   return false;   }

    // Do nothing if the UART Port was not enabled
    if (ns_device::ns_uart::uart_cfg[uart_n].enable == false)
    {   return false;   }

    // Transmit the message through the UART Port
    SerialPort[uart_n]->write(msg);

    // Publish to MQTT to notify transmission
    mqtt_publish_tx(uart_n, msg);

    return true;
}

/**
 * @details This function checks that the provided UART Port number is valid
 * and has been enabled, then it send each of the messages from the provided
 * argument list through the UART port.
 */
bool InterfaceUART::uart_tx_msg(const uint8_t uart_n, int argc, char* argv[])
{
    // Do nothing if component was not initialized
    if (initialized == false)
    {   return false;   }

    // Do nothing for UART0 that is used as device CLI
    if (uart_n == 0U)
    {   return false;   }

    // Do nothing if specified UART Port number is invalid
    if (uart_n >= ns_const::MAX_NUM_UART)
    {   return false;   }

    // Do nothing if the UART Port was not enabled
    if (ns_device::ns_uart::uart_cfg[uart_n].enable == false)
    {   return false;   }

    // Transmit the message through the UART Port
    for (int i = 0; i < argc; i++)
    {   SerialPort[uart_n]->write(argv[i]);   }

    // Publish to MQTT to notify transmission
    char msg_tx[DATA_RX_BUFFER_SIZE];
    msg_tx[0] = '\0';
    if (single_str_from_array_of_str(argc, argv, msg_tx, DATA_RX_BUFFER_SIZE))
    {   mqtt_publish_tx(uart_n, msg_tx);   }

    return true;
}

/*****************************************************************************/

/* Private Methods */

/**
 * @details Checks if the UART is enabled and configured, then checks if there
 * is any message received through this port and forward it through MQTT.
 */
bool InterfaceUART::handle_uart_rx(const uint8_t uart_n)
{
    bool msg_published = false;

    // Do nothing for UART0 that is used as device CLI
    if (uart_n == 0U)
    {   return false;   }

    // Do nothing if specified UART Port number is invalid
    if (uart_n >= ns_const::MAX_NUM_UART)
    {   return false;   }

    // Do nothing if the UART Port was not enabled
    if (ns_device::ns_uart::uart_cfg[uart_n].enable == false)
    {   return false;   }

    // Do nothing if there is none UART data received
    if (SerialPort[uart_n]->available() == 0)
    {   return false;   }

    // Handle UART data reception
    uint8_t* ptr_rx_data = rx_data[uart_n];
    uint32_t* ptr_num_data_rx = &(num_data_rx[uart_n]);

    // Read received byte
    ptr_rx_data[*ptr_num_data_rx] = SerialPort[uart_n]->read();
    *ptr_num_data_rx = *ptr_num_data_rx + 1U;

    // Send MQTT message if received byte is an End Of Line
    if (ptr_rx_data[*ptr_num_data_rx - 1U] == '\n')
    {
        ptr_rx_data[*ptr_num_data_rx - 1U] = (uint8_t)('\0');
        msg_published = mqtt_publish_rx(uart_n, (const char*)(ptr_rx_data));
        *ptr_num_data_rx = 0U;
    }

    // Send MQTT message if buffer is completed
    else if (*ptr_num_data_rx == DATA_RX_BUFFER_SIZE - 1U)
    {
        ptr_rx_data[DATA_RX_BUFFER_SIZE - 1U] = (uint8_t)('\0');
        msg_published = mqtt_publish_rx(uart_n, (const char*)(ptr_rx_data));
        *ptr_num_data_rx = 0U;
    }

    return msg_published;
}

/**
 * @details This function prepare a JSON string with the current
 * "msg_status_port_n" UART Port status information and send it.
 * It also update the "msg_status_port_n" variable to send next UART Port
 * information the next time this function is call.
 *
 * UART Status Information JSON Message Format:
 * {
 *     "port":   N, // UART Port Number Status Information
 *     "enable": N, // UART Port logging disabled/enabled (0/1)
 *     "bauds":  N, // Configured Baud Rate
 * }
 */
bool InterfaceUART::mqtt_send_uart_status_info()
{
    char msg[UART_STATUS_INFO_MSG_LEN];

    // Prepare the Message Payload
    snprintf(msg, UART_STATUS_INFO_MSG_LEN,
        "{"
            "\"port\":%" PRIu8 ","
            "\"enable\":%d,"
            "\"bauds\":%" PRIu32
        "}",
        msg_status_port_n,
        (int)(ns_device::ns_uart::uart_cfg[msg_status_port_n].enable),
        ns_device::ns_uart::uart_cfg[msg_status_port_n].bauds
    );

    // Update UART Port Number to send info on the status message
    msg_status_port_n = msg_status_port_n + 1U;
    if (msg_status_port_n >= ns_const::MAX_NUM_UART)
    {   msg_status_port_n = 1U;   }

    // Send the Message
    return MQTT.publish(topic_status, msg);
}

/**
 * @details Uses the MQTT component to send a received UART message through
 * the UART Rx topic.
 */
bool InterfaceUART::mqtt_publish_rx(const uint8_t uart_n, const char* msg)
{
    // Do nothing if specified UART Port number is invalid
    if (uart_n >= ns_const::MAX_NUM_UART)
    {   return false;   }

    return MQTT.publish(topic_rx[uart_n], msg);
}

/**
 * @details Uses the MQTT component to send a transmitted UART message through
 * the UART Tx topic.
 */
bool InterfaceUART::mqtt_publish_tx(const uint8_t uart_n, const char* msg)
{
    // Do nothing if specified UART Port number is invalid
    if (uart_n >= ns_const::MAX_NUM_UART)
    {   return false;   }

    return MQTT.publish(topic_tx[uart_n], msg);
}

/*****************************************************************************/
