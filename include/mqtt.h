
/**
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
