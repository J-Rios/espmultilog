/**
 * @file    cli.cpp
 * @author  Jose Miguel Rios Rubio <jrios.github@gmail.com>
 * @date    2024-01-04
 * @version 1.0.0
 *
 * @section DESCRIPTION
 *
 * ESPMULTILOG Command Line Interface (CLI) source file.
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
#include "cli.h"

// Standard C++ Libraries
#include <cinttypes>
#include <cstdint>

// Hardware Abstraction Layer Framework
#include "Arduino.h"

// WiFi Library
#include <WiFi.h>

// Command Line Interface Library
#include <minbasecli.h>

// Constant Data
#include "constants.h"

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

/**
 * @brief Command Line Interface Object.
 */
CommandLineInterface CLI;

/*****************************************************************************/

/* In-Scope Global Elements */

MINBASECLI Cli;

/*****************************************************************************/

/* In-Scope Function Prototypes */

// CLI command callback functions
static void cmd_wifi_status(MINBASECLI* Cli, int argc, char* argv[]);
static void cmd_reboot(MINBASECLI* Cli, int argc, char* argv[]);
static void cmd_version(MINBASECLI* Cli, int argc, char* argv[]);
static void cmd_uart(MINBASECLI* Cli, int argc, char* argv[]);

// Common Functions
static void show_invalid_cmd(MINBASECLI* Cli);

/*****************************************************************************/

/* Command Line Interface: Public Methods */

void CommandLineInterface::init()
{
    // CLI init to use Serial as interface
    Cli.setup(&Serial, ns_const::DEFAULT_UART_BAUD_RATE);
    Serial.setDebugOutput(true);

    Serial.printf("%s (v%d.%d.%d)\n",
        ns_const::PROJECT_NAME,
        (int)(ns_const::FW_APP_VERSION_X),
        (int)(ns_const::FW_APP_VERSION_Y),
        (int)(ns_const::FW_APP_VERSION_Z));

    // Add commands and bind callbacks to them
    Cli.add_cmd("wifi_status", &cmd_wifi_status, "Show current wifi info.");
    Cli.add_cmd("reboot", &cmd_reboot, "Reboot the system.");
    Cli.add_cmd("version", &cmd_version, "Shows current frimware version.");
    Cli.add_cmd("uart", &cmd_uart, "Setup and Control an UART Port.");

    Cli.printf("\nCommand Line Interface is ready\n\n");
}

void CommandLineInterface::process()
{
    Cli.run();
}

/*****************************************************************************/

/* Command Functions */

static void cmd_wifi_status(MINBASECLI* Cli, int argc, char* argv[])
{
    Cli->printf("\nWiFi Information:\n");
    Cli->printf("-----------------\n");

    // MAC Address
    Cli->printf("MAC Address: %s\n", WiFi.macAddress().c_str());

    // Hostname
    Cli->printf("HostName: %s\n", WiFi.getHostname());

    // WiFi Mode
    Cli->printf("Mode: ");
    wifi_mode_t wifi_mode = WiFi.getMode();
    if (wifi_mode == WIFI_MODE_AP)
    {   Cli->printf("Access Point\n");   }
    else if (wifi_mode == WIFI_MODE_STA)
    {   Cli->printf("Station\n");   }
    else if (wifi_mode == WIFI_MODE_APSTA)
    {   Cli->printf("Access Point + Station\n");   }
    else
    {   Cli->printf("Unkown\n");   }

    // WiFi Status
    int status = WiFi.getStatusBits();
    Cli->printf("Status: ");
    if ( (status & (STA_STARTED_BIT | AP_STARTED_BIT)) == 0)
    {   Cli->printf("WiFi RF Off\n");   }
    else if ( (status & STA_STARTED_BIT) && (status & STA_CONNECTED_BIT) )
    {   Cli->printf("Connected\n");   }
    else
    {   Cli->printf("Disconnected\n");   }

    // WiFi Network Info
    Cli->printf("SSID: %s\n", WiFi.SSID().c_str());
    Cli->printf("Channel: %d\n", (int)(WiFi.channel()));
    Cli->printf("Auto-Reconnect: ");
    if (WiFi.getAutoReconnect())
    {   Cli->printf("Enabled\n");   }
    else
    {   Cli->printf("Disabled\n");   }
    Cli->printf("GW: %s\n", WiFi.gatewayIP().toString().c_str());
    Cli->printf("Subnet: %s\n", WiFi.subnetMask().toString().c_str());
    Cli->printf("DNS: %s\n", WiFi.dnsIP().toString().c_str());

    // WIFi IP
    if ( (status & STA_HAS_IP_BIT) || (status & STA_HAS_IP6_BIT) )
    {   Cli->printf("IP: %s\n", WiFi.localIP().toString().c_str());   }
    else
    {   Cli->printf("IP: None\n");   }

    Cli->printf("\n");
}

static void cmd_reboot(MINBASECLI* Cli, int argc, char* argv[])
{
    Cli->printf("Rebooting...\n");
    Cli->printf("\n--------------------------------\n\n");
    esp_restart();
}

static void cmd_version(MINBASECLI* Cli, int argc, char* argv[])
{
    Cli->printf("ESP-IDF Version: %s\n", esp_get_idf_version());
    Cli->printf("FW App Version: v%d.%d.%d\n",
        (int)(ns_const::FW_APP_VERSION_X),
        (int)(ns_const::FW_APP_VERSION_Y),
        (int)(ns_const::FW_APP_VERSION_Z));
}

/*****************************************************************************/

/* UART Interface */

/**
 * @details UART configuration command.
 *
 * Expected commands format:
 *   uart N command [arg1] [arg2] ...
 *
 * Configure UART Port 1 for 9600 bauds:
 *   uart 1 config bauds 9600
 * Configure UART Port 1 for 9600 bauds:
 *   uart 1 config bauds 9600
 *
 * Enable Logging on UART Port 1:
 *   uart 1 config enable
 *
 * Disable Logging on UART Port 1:
 *   uart 1 config disable
 *
 * Transmit a message through the UART Port 1:
 *   uart 1 tx message to be send
 */
static void cmd_uart(MINBASECLI* Cli, int argc, char* argv[])
{
    // Ignore if command has less than 2 arguments (N config_command)
    if (argc < 3U)
    {   show_invalid_cmd(Cli); return;   }

    // Remove UART Port Number and command from arguments count
    argc = argc - 2;

    // Try to convert first argument from string to u32
    // (expected to be the UART Port Number)
    uint8_t uart_n = ns_const::MAX_NUM_UART;
    t_return_code convert_rc = safe_atoi_u8(argv[0], sizeof(argv[0]), &uart_n);
    if (convert_rc != t_return_code::RC_OK)
    {   show_invalid_cmd(Cli); return;   }
    if (uart_n >= ns_const::MAX_NUM_UART)
    {   show_invalid_cmd(Cli); return;   }

    // UART Port Configuration
    if (strcmp(argv[1], "config") == 0)
    {
        if (IfaceUART.configure(uart_n, argc, &(argv[2])) == false)
        {   show_invalid_cmd(Cli); return;   }
    }

    // Transmit UART message
    if (strcmp(argv[1], "tx") == 0)
    {
        if (IfaceUART.uart_tx_msg(uart_n, argc, &(argv[2])) == false)
        {   Cli->printf("Tx Fail\n");   }
        else
        {   Cli->printf("Tx ok\n");   }
        return;
    }
}

/*****************************************************************************/

/* Common Functions */

/**
 * @details This function just show a message through the CLI to notify
 * that the last command that was provided by the user is unknown or has
 * an invalid sintax.
 */
static void show_invalid_cmd(MINBASECLI* Cli)
{
    Cli->printf("  Invalid Command\n");
}

/*****************************************************************************/
