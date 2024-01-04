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
 * Check Messages:
 * mosquitto_sub -F '%I\n%t\n%p\n' -h "test.mosquitto.org" -p 1883
 *               -t "/24:0A:C4:81:AD:F8/control/+"
 *
 * Request FW Version:
 * mosquitto_pub -h "test.mosquitto.org" -p 1883
 *               -t "/24:0A:C4:81:AD:F8/control/in" -m "version"
 *
 * Request Reboot:
 * mosquitto_pub -h "test.mosquitto.org" -p 1883
 *               -t "/24:0A:C4:81:AD:F8/control/in" -m "reboot"
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
    char device_id[MAC_ADDRESS_LENGTH];
    snprintf(device_id, (size_t)(MAC_ADDRESS_LENGTH), "%s",
        WiFi.macAddress().c_str());
    snprintf(topic_input, sizeof(topic_input), MQTT_TOPIC_IN, device_id);
    snprintf(topic_output, sizeof(topic_output), MQTT_TOPIC_OUT, device_id);
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
    // Topic Device Control Input ("/DEVICEID/control/in")
    if (strcmp(topic, topic_input) == 0)
    {   msg_rx_in(topic, payload);   }

    // Message received from an unknown and unexpected Topic
    else
    {   Serial.println("[Warning] Unknown Topic");   }
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
    subscribe(topic_input);

    return is_connected();
}

/*****************************************************************************/
