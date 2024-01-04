/**
 * @file    mqtt.h
 * @author  Jose Miguel Rios Rubio <jrios.github@gmail.com>
 * @date    2024-01-03
 * @version 1.0.0
 *
 * @section DESCRIPTION
 *
 * ESPMULTILOG MQTT Communication header file.
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

#ifndef MQTT_H
#define MQTT_H

/*****************************************************************************/

/* Libraries */

// Standard C++ Libraries
#include <cstdint>

// MQTT Library
#include <PubSubClient.h>

// WiFi Library
#include <WiFi.h>

// Firmware Update Over MQTT Library
#include "mqtt_fuota_duino.h"

// Constant Data
#include "constants.h"

/*****************************************************************************/

/* Class Interface */

class MQTTCommunication
{
    /* Public Attributes */

    public:

        MQTTFirmwareUpdate MqttFuota;

        char topic_input[ns_const::MQTT_TOPIC_MAX_LEN];

    /******************************************************************/

    /* Constructor & Destructor */

    public:

        MQTTCommunication();

        ~MQTTCommunication();

    /******************************************************************/

    /* Public Methods */

    public:

        bool init(WiFiClient* wifi_client);

        void process();


        bool is_connected();

        bool publish(const char* topic, const char* payload);

        bool subscribe(const char* topic);

        void handle_msg_rx(const char* topic, char* payload);

        void msg_rx_in(const char* topic, char* payload);

    /******************************************************************/

    /* Private Attributes */

    private:

        bool is_initialized;
        WiFiClient* WIFIClient;
        PubSubClient* MQTTClient;
        char topic_output[ns_const::MQTT_TOPIC_MAX_LEN];

        bool connect();

    /******************************************************************/

    /* Private Methods */

    private:


};

/*****************************************************************************/

/* Object Declaration */

extern MQTTCommunication MQTT;

/*****************************************************************************/

/* Include Guard Close */

#endif /* MQTT_H */
