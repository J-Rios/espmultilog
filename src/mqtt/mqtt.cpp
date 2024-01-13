/**
 * @file    mqtt.cpp
 * @author  Jose Miguel Rios Rubio <jrios.github@gmail.com>
 * @date    2024-01-03
 * @version 1.0.0
 *
 * @section DESCRIPTION
 *
 * ESPMULTILOG MQTT Communication source file.
 *
 * Check Device General Messages:
 * mosquitto_sub -F '%I\n%t\n%p\n' -h "test.mosquitto.org" -p 1883
 *               -t "/XXXXXXXXXXXX/control/+"
 *
 * Request FW Version:
 * mosquitto_pub -h "test.mosquitto.org" -p 1883
 *               -t "/XXXXXXXXXXXX/control/in" -m "version"
 *
 * Request Reboot:
 * mosquitto_pub -h "test.mosquitto.org" -p 1883
 *               -t "/XXXXXXXXXXXX/control/in" -m "reboot"
 *
 * Configure UART Port N speed to 9600 bauds:
 * mosquitto_pub -h "test.mosquitto.org" -p 1883
 *               -t "/XXXXXXXXXXXX/uart/N/cfg" -m "bauds 9600"
 *
 * Enable Logging of UART Port N:
 * mosquitto_pub -h "test.mosquitto.org" -p 1883
 *               -t "/XXXXXXXXXXXX/uart/N/cfg" -m "enable"
 *
 * Disable Logging of UART Port N:
 * mosquitto_pub -h "test.mosquitto.org" -p 1883
 *               -t "/XXXXXXXXXXXX/uart/N/cfg" -m "disable"
 *
 * Transmit a message through UART Port N:
 * mosquitto_pub -h "test.mosquitto.org" -p 1883
 *               -t "/XXXXXXXXXXXX/uart/N/tx" -m "the message to send"
 *
 * Check for current UARTs configurations (periodically sent by device):
 * mosquitto_sub -F '%I\n%t\n%p\n' -h "test.mosquitto.org" -p 1883
 *               -t "/XXXXXXXXXXXX/status/uart"
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
#include "mqtt.h"

// C++ new operator Library
#include <new>

// Hardware Abstraction Layer Framework
#include "Arduino.h"

// GLobal Data
#include "../global/global.h"

// Device Interfaces
#include "../interfaces/adc/iface_adc.h"
#include "../interfaces/can/iface_can.h"
#include "../interfaces/dio/iface_dio.h"
#include "../interfaces/i2c/iface_i2c.h"
#include "../interfaces/spi/iface_spi.h"
#include "../interfaces/uart/iface_uart.h"

// Miscellaneous Library
#include "../misc/misc.h"

/*****************************************************************************/

/* Object Instantiation */

MQTTCommunication MQTT;

// Reserved static memory space in BSS section to instantiate the MQTT client
// object
static uint8_t bss_memory_mqtt_client[sizeof(PubSubClient)];

/*****************************************************************************/

/* In-Scope Function Callbacks */

static void cb_msg_rx(char* topic, uint8_t* payload, unsigned int length)
{
    static const uint16_t MAX_MQTT_PAYLOAD_LENGTH = 257U;
    static char payload_str[MAX_MQTT_PAYLOAD_LENGTH];

    Serial.println("MQTT MSG RX");
    Serial.printf("  Topic: %s\n", topic);

    if (length >= MAX_MQTT_PAYLOAD_LENGTH)
    {   length = MAX_MQTT_PAYLOAD_LENGTH - 1U;   }

    unsigned int i = 0;
    while (i < length)
    {
        payload_str[i] = (char)(payload[i]);
        i = i + 1;
    }
    payload_str[i] = '\0';

    Serial.printf("  Payload: %s", payload_str);

    // MQTT FUOTA Mechanism
    MQTT.MqttFuota.mqtt_msg_rx(topic, payload, (uint32_t)(length));

    // Handle Message by Topic
    MQTT.handle_msg_rx(topic, payload_str);
}

/*****************************************************************************/

/* Constructor & Destructor */

MQTTCommunication::MQTTCommunication()
{
    is_initialized = false;
    WIFIClient = nullptr;
    MQTTClient = nullptr;
}

MQTTCommunication::~MQTTCommunication()
{   /* Nothing to do */   }

/*****************************************************************************/

/* Public Methods */

bool MQTTCommunication::init(WiFiClient* wifi_client)
{
    using namespace ns_const;

    // Check if provided WiFi Client is valid
    if (wifi_client == nullptr)
    {   return false;   }

    // Do nothing if component is already initialized
    if (is_initialized)
    {   return true;   }

    // Set MQTT Topics
    snprintf(topic_input, sizeof(topic_input),
        MQTT_TOPIC_IN, get_device_uuid());
    snprintf(topic_output, sizeof(topic_output),
        MQTT_TOPIC_OUT, get_device_uuid());
    Serial.printf("MQTT Topics to use:\n");
    Serial.printf("%s\n", topic_input);
    Serial.printf("%s\n", topic_output);

    WIFIClient = wifi_client;
    MQTTClient = new(bss_memory_mqtt_client) PubSubClient(*wifi_client);
    MQTTClient->setBufferSize(2048);
    MQTTClient->setServer(MQTT_SERVER, MQTT_PORT);
    MQTTClient->setCallback(cb_msg_rx);

    t_fw_info app_info;
    app_info.version[0] = FW_APP_VERSION_X;
    app_info.version[1] = FW_APP_VERSION_Y;
    app_info.version[2] = FW_APP_VERSION_Z;
    if (MqttFuota.init(MQTTClient, app_info) == false)
    {   return false;   }

    is_initialized = true;
    return true;
}

void MQTTCommunication::process()
{
    // Do nothing if component is not initialized
    if (is_initialized == false)
    {   return;   }

    // Handle MQTT Connection/Reconnection
    if (is_connected() == false)
    {   connect();   }

    // Process MQTT client
    MQTTClient->loop();
    MqttFuota.process();
}

bool MQTTCommunication::is_connected()
{
    // Check if component is not initialized
    if (is_initialized == false)
    {   return false;   }

    return (bool)(MQTTClient->connected());
}

bool MQTTCommunication::publish(const char* topic, const char* payload)
{
    bool publish_ok = false;

    // Do nothing if is not connected
    if (is_connected() == false)
    {   return false;   }

    Serial.println("MQTT MSG TX");
    Serial.printf("  Topic: %s\n", topic);
    Serial.printf("  Payload: %s", payload);
    publish_ok = (bool)(MQTTClient->publish(topic, payload));
    if (publish_ok == false)
    {   Serial.println("[Error] MQTT Publish Fail");   }

    return publish_ok;
}

bool MQTTCommunication::subscribe(const char* topic)
{
    bool subscribe_ok = false;

    // Do nothing if is not connected
    if (is_connected() == false)
    {   return false;   }

    Serial.printf("MQTT Subscribe to: %s\n", topic);
    subscribe_ok = (bool)(MQTTClient->subscribe(topic));
    if (subscribe_ok == false)
    {   Serial.println("[Error] MQTT Subscription Fail");   }

    return subscribe_ok;
}

void MQTTCommunication::handle_msg_rx(const char* topic, char* payload)
{
    using namespace ns_misc;

    // Topic Device Control Input ("/XXXXXXXXXXXX/control/in")
    if (strcmp(topic, topic_input) == 0)
    {
        msg_rx_in(topic, payload);
        return;
    }

    // Topics for UART Management
    for (uint8_t uart_n = 0U; uart_n < ns_const::MAX_NUM_UART; uart_n++)
    {
        // Topic UART Port Configuration ("/XXXXXXXXXXXX/uart/N/cfg")
        if (strcmp(topic, IfaceUART.get_topic_cfg(uart_n)) == 0)
        {
            // Parse string to get handle it as commad+arguments
            str_parse_cmd_args(payload, &cmd_args);
            if (cmd_args.argc == 0U)
            {   return;   }

            // UART Configuration
            IfaceUART.configure(uart_n, cmd_args.argc,
                (char**)(cmd_args.argv));
            return;
        }

        // Topic UART Port Transmission ("/XXXXXXXXXXXX/uart/N/tx")
        if (strcmp(topic, IfaceUART.get_topic_tx(uart_n)) == 0)
        {
            // Transmit UART Message
            if (IfaceUART.uart_tx_msg(uart_n, payload))
            {
                // Echo the MQTT message to acknowledge transmission
                publish(topic, payload);
            }
            return;
        }
    }
}

void MQTTCommunication::msg_rx_in(const char* topic, char* payload)
{
    // Request Device Reboot
    if (strcmp(payload, "reboot") == 0)
    {
        publish(topic_output, "Rebooting");
        MQTTClient->disconnect();
        esp_restart();
    }

    // Request Firmware App Version
    else if (strcmp(payload, "version") == 0)
    {
        char version[16];
        snprintf(version, 16, "v%d.%d.%d",
            (int)(ns_const::FW_APP_VERSION_X),
            (int)(ns_const::FW_APP_VERSION_Y),
            (int)(ns_const::FW_APP_VERSION_Z));
        publish(topic_output, version);
    }
}

/*****************************************************************************/

/* Private Methods */

bool MQTTCommunication::connect()
{
    static const unsigned long T_WAIT_RECONNECT = 5000U;
    static unsigned long t0 = 0U;

    // Do nothing if component is not initialized
    if (is_initialized == false)
    {   return false;   }

    // Do nothing if time for connect/reconnect has not arrive
    if (millis() - t0 < T_WAIT_RECONNECT)
    {   return false;   }
    t0 = millis();

    // MQTT Connection
    Serial.println("MQTT Connection...\n");
    if (MQTTClient->connect(ns_device::id) == false)
    {
        Serial.println("Fail to connect to MQTT Broker...\n");
        return false;
    }

    // Connection Success
    t0 = 0U;
    Serial.println("MQTT Connected\n");
    publish(topic_output, "Device connected");

    // MQTT Subscriptions
    subscribe(topic_input);
    for (uint8_t uart_n = 0U; uart_n < ns_const::MAX_NUM_UART; uart_n++)
    {
        subscribe(IfaceUART.get_topic_cfg(uart_n));
        subscribe(IfaceUART.get_topic_tx(uart_n));
    }


    return is_connected();
}

/*****************************************************************************/
