/**
 * @file    network_interface.cpp
 * @author  Jose Miguel Rios Rubio
 * @date    2024.01.03
 * @version 1.0.0
 * @brief   ESP32 network events handler component that tracks current device
 * network status like WiFi/Etehrnet connection, IP assigment and general
 * network availability.
 */

/*****************************************************************************/

/* Libraries */

// Header Interface
#include "network_interface.h"

// Hardware Abstraction Layer Framework
#include "Arduino.h"

/*****************************************************************************/

/* Object Instantiation */

NetworkInterface Network;

/*****************************************************************************/

/* Network Events Handler Callback */

static void cb_net_events(WiFiEvent_t event)
{
    system_event_id_t wifi_event = (system_event_id_t)(event);
    Serial.printf("[WiFi-event] event: %d\n", (int)(wifi_event));

    switch (wifi_event)
    {
        case SYSTEM_EVENT_WIFI_READY:
            Serial.println("WiFi interface ready");
            break;

        case SYSTEM_EVENT_STA_START:
            Serial.println("WiFi client started");
            break;

        case SYSTEM_EVENT_STA_STOP:
            Serial.println("WiFi clients stopped");
            Network.clear_state();
            break;

        case SYSTEM_EVENT_STA_DISCONNECTED:
            Serial.println("Disconnected from WiFi access point");
            Network.clear_state();
            break;

        case SYSTEM_EVENT_STA_CONNECTED:
            Serial.println("Connected to access point");
            break;

        case SYSTEM_EVENT_STA_GOT_IP:
        case SYSTEM_EVENT_GOT_IP6:
            Serial.print("Obtained IP address: ");
            Serial.println(WiFi.localIP());
            Network.has_ip = true;
            Network.net_available = true;
            Network.t0_connection = millis();
            break;

        case SYSTEM_EVENT_STA_LOST_IP:
            Serial.println("IP address lost");
            Network.net_available = false;
            Network.has_ip = false;
            Network.t0_connection = 0U;
            break;

        case SYSTEM_EVENT_AP_START:
            Serial.println("WiFi access point started");
            break;

        case SYSTEM_EVENT_AP_STOP:
            Serial.println("WiFi access point stopped");
            break;

        case SYSTEM_EVENT_AP_STACONNECTED:
            Serial.println("Client connected");
            break;

        case SYSTEM_EVENT_AP_STADISCONNECTED:
            Serial.println("Client disconnected");
            break;

        case SYSTEM_EVENT_AP_STAIPASSIGNED:
            Serial.println("Assigned IP address to client");
            break;

        default:
            break;
    }
}

/*****************************************************************************/

/* Class Constructor */

/**
 * @details
 * The Constructor of the class initializes all the internal attributes to
 * initial default values.
 */
NetworkInterface::NetworkInterface()
:
    connected(false),
    has_ip(false),
    net_available(false),
    t0_connection(0U)
{
    clear_state();
}

/*****************************************************************************/

/* Public Methods */

bool NetworkInterface::init()
{
    Serial.println("Network Interface Initialized");
    WiFi.onEvent(cb_net_events);
    return true;
}

bool NetworkInterface::available()
{
    return net_available;
}

/*****************************************************************************/

/* Private Methods */

void NetworkInterface::clear_state()
{
    connected = false;
    has_ip = false;
    net_available = false;
    t0_connection = 0U;
}

/*****************************************************************************/
