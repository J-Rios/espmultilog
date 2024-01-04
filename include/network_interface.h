/**
 * @file    network_interface.h
 * @author  Jose Miguel Rios Rubio
 * @date    2023.10.13
 * @version 1.0.0
 * @brief   ESP32 network events handler component that tracks current device
 * network status like WiFi/Ethernet connection, IP assigment and general
 * network availability.
 */

/*****************************************************************************/

/* Include Guard */

#ifndef WIFI_EVENTS_H
#define WIFI_EVENTS_H

/*****************************************************************************/

/* Libraries */

// WiFi Library
#include <WiFi.h>

/*****************************************************************************/

/* Class Interface */

class NetworkInterface
{
    /******************************************************************/

    /* Public Attributes */

    public:

        /**
         * @brief Device is connected to the network.
         */
        bool connected;

        /**
         * @brief Device has received an IP from DHCP.
         */
        bool has_ip;

        /**
         * @brief Device is available for network communication.
         */
        bool net_available;

        /**
         * @brief Time instant when the device reach the network availability
         * state (received last IP assigment).
         */
        unsigned long t0_connection;

    /******************************************************************/

    /* Public Methods */

    public:

        /**
         * @brief Construct a new Network Interface object.
         */
        NetworkInterface();

        /**
         * @brief Initialize the Network Interface (start handling network
         * events).
         * @return true Initialization result success.
         * @return false Initialization result fail.
         */
        bool init();

        /**
         * @brief Check if network is completely available for operation (the
         * device is connected to the network and has IP to communicate).
         * @return true Network communication available.
         * @return false Network communication unavailable.
         */
        bool available();

        /**
         * @brief Clear network information.
         */
        void clear_state();

    /******************************************************************/
};

/*****************************************************************************/

/* Object Declaration */

/**
 * @brief Network Interface.
 */
extern NetworkInterface Network;

/*****************************************************************************/

/* Include Guard Close */

#endif /* WIFI_EVENTS_H */
