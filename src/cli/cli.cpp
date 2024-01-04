
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

// Miscellaneous Library
#include "../misc/misc.h"

/*****************************************************************************/

/* In-Scope Function Prototypes */

// CLI command callback functions
static void cmd_wifi_status(MINBASECLI* Cli, int argc, char* argv[]);
static void cmd_reboot(MINBASECLI* Cli, int argc, char* argv[]);
static void cmd_version(MINBASECLI* Cli, int argc, char* argv[]);

/*****************************************************************************/

/* In-Scope Global Elements */

MINBASECLI Cli;

/*****************************************************************************/

/* Command Line Interface Functions */

void cli_init()
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

    Cli.printf("\nCommand Line Interface is ready\n\n");
}

void cli_process()
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
