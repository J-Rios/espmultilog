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
}

/**
 * @details The init method of the Interface initializes all the required
 * elements of the Interface for the logging.
 */
void InterfaceUART::init(const char* device_uuid)
{
    SerialPort[0] = &Serial;
    #if SOC_UART_NUM > 1
        SerialPort[1] = &Serial1;
    #endif
    #if SOC_UART_NUM > 2 // ESP32-C3 only has 2 UARTs
        SerialPort[2] = &Serial2;
    #endif

    for (uint8_t i = 0U; i < ns_const::MAX_NUM_UART; i++)
    {
        snprintf(topic_cfg[i], sizeof(topic_cfg[i]),
            MQTT_TOPIC_CFG, device_uuid);
        snprintf(topic_rx[i], sizeof(topic_rx[i]), MQTT_TOPIC_RX, device_uuid);
        snprintf(topic_tx[i], sizeof(topic_tx[i]), MQTT_TOPIC_TX, device_uuid);
    }

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
 * @details This function is a setter to enable or disable handling of
 * UART Port data as raw bytes by modifying the value of the Global
 * uart_cfg enable field.
 */
bool InterfaceUART::uart_config_mode_bytes(const uint8_t uart_n,
        const bool enable)
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

    ns_device::ns_uart::uart_cfg[uart_n].mode_raw_bytes = enable;

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

    // Echo the message to Tx trhrough MQTT to acknowledge transmission
    publish_tx(uart_n, msg);

    // Transmit the message through the UART Port
    if (ns_device::ns_uart::uart_cfg[uart_n].mode_raw_bytes)
    {   SerialPort[uart_n]->print(msg);   }
    else
    {   SerialPort[uart_n]->println(msg);   }

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

    // Handle UART data reception
    uint8_t* ptr_rx_data = rx_data[uart_n];
    uint32_t* ptr_num_data_rx = &(num_data_rx[uart_n]);
    if (SerialPort[uart_n]->available())
    {
        // Read received byte
        ptr_rx_data[*ptr_num_data_rx] = SerialPort[uart_n]->read();
        *ptr_num_data_rx = *ptr_num_data_rx + 1U;

        // UART data received as raw bytes to send them as chunks
        if (ns_device::ns_uart::uart_cfg[uart_n].mode_raw_bytes)
        {
            // Send MQTT message if buffer is completed
            if (*ptr_num_data_rx == DATA_RX_BUFFER_SIZE-2U)
            {
                ptr_rx_data[DATA_RX_BUFFER_SIZE-1U] = (uint8_t)('\0');
                msg_published = publish_tx(uart_n, (const char*)(ptr_rx_data));
                *ptr_num_data_rx = 0U;
            }
        }

        // UART data received as strings to send at end of line
        else
        {
            // Send MQTT message if last received byte is an End Of Line
            if (ptr_rx_data[*ptr_num_data_rx - 1U] == '\n')
            {
                ptr_rx_data[*ptr_num_data_rx - 1U] == (uint8_t)('\0');
                msg_published = publish_tx(uart_n, (const char*)(ptr_rx_data));
                *ptr_num_data_rx = 0U;
            }

            // Send MQTT message if buffer is completed
            else if (*ptr_num_data_rx == DATA_RX_BUFFER_SIZE-2U)
            {
                ptr_rx_data[DATA_RX_BUFFER_SIZE-1U] = (uint8_t)('\0');
                msg_published = publish_tx(uart_n, (const char*)(ptr_rx_data));
                *ptr_num_data_rx = 0U;
            }
        }
    }

    return msg_published;
}

/**
 * @details Uses the MQTT component to send a received UART message through
 * the UART Rx topic.
 */
bool InterfaceUART::publish_rx(const uint8_t uart_n, const char* msg)
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
bool InterfaceUART::publish_tx(const uint8_t uart_n, const char* msg)
{
    // Do nothing if specified UART Port number is invalid
    if (uart_n >= ns_const::MAX_NUM_UART)
    {   return false;   }

    return MQTT.publish(topic_tx[uart_n], msg);
}

/*****************************************************************************/
