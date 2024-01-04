/**
 * @file    network_interface.h
 * @author  Jose Miguel Rios Rubio <jrios.github@gmail.com>
 * @date    2024-01-03
 * @version 1.0.0
 *
 * @section DESCRIPTION
 *
 * ESP32 network events handler component that tracks current device network
 * status like WiFi/Ethernet connection, IP assigment and general network
 * availability.
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
