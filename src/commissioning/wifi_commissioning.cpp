/**
 * @file    wifi_commissioning.cpp
 * @author  Jose Miguel Rios Rubio <jrios.github@gmail.com>
 * @date    2024-01-03
 * @version 1.0.0
 *
 * @section DESCRIPTION
 *
 * ESPMULTILOG WiFi Commissioning source file.
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
#include "wifi_commissioning.h"

// Hardware Abstraction Layer Framework
#include "Arduino.h"

// Constant Data
#include "constants.h"

// Miscellaneous Library
#include "../misc/misc.h"

/*****************************************************************************/

/* Object Instantiation */

WiFiCommissioner WifiCommissioning;

/*****************************************************************************/

/* In-Scope Function Callbacks */

static void cb_config_mode(WiFiManager* myWiFiManager)
{
    Serial.println("Entered config mode");
    Serial.println(WiFi.softAPIP());
    Serial.println(myWiFiManager->getConfigPortalSSID());
}

static void cb_param_save()
{
    Serial.println("[CALLBACK] cb_param_save triggered");
    String param = WifiCommissioning.param_get("customfieldid");
    Serial.println("PARAM customfieldid = " + param);
}

/*****************************************************************************/

/* Constructor & Destructor */

WiFiCommissioner::WiFiCommissioner()
{
    is_initialized = false;
}

/*****************************************************************************/

/* Public Methods */

bool WiFiCommissioner::init()
{
    // Explicitly set to Station Mode (by default ESP init at STA+AP)
    WiFi.mode(WIFI_STA);

    _WiFiManager.setDarkMode(true);
    _WiFiManager.setConfigPortalBlocking(false);
    _WiFiManager.setHostname(get_device_id());

    // add a custom input field
    //int max_param_len = 40;

    // Custom input parameter
    // new (&custom_field) WiFiManagerParameter("customfieldid",
    //    "Custom Field Label", "Custom Field Value", max_param_len,
    //    "placeholder=\"Custom Field Placeholder\"");

    // Custom checkbox input parameter
    // new (&custom_field) WiFiManagerParameter("customfieldid",
    //    "Custom Field Label", "Custom Field Value", max_param_len,
    //    "placeholder=\"Custom Field Placeholder\" type=\"checkbox\"");

    // Custom radio button input parameter
    const char* custom_radio_str =
        "<br/>" \
        "<label for='customfieldid'>Custom Field Label</label><br>" \
        "<input type='radio' name='customfieldid' value='1' checked> One<br>" \
        "<input type='radio' name='customfieldid' value='2'> Two<br>" \
        "<input type='radio' name='customfieldid' value='3'> Three";
    new (&custom_field) WiFiManagerParameter(custom_radio_str);

    _WiFiManager.addParameter(&custom_field);

    // Set callbacks
    _WiFiManager.setAPCallback(cb_config_mode);
    _WiFiManager.setSaveParamsCallback(cb_param_save);

    // Set custom menu via array or vector
    // Menu tokens:
    // "wifi","wifinoscan","info","param","close","sep","erase", "restart",
    // "exit" (sep is seperator)
    // Note: if param is in menu, params will not show up in wifi page
    std::vector<const char*> menu =
        { "wifi", "info", "param", "sep", "restart", "exit" };
    _WiFiManager.setMenu(menu);

    // Set Dark theme
    _WiFiManager.setClass("invert");

    // Set static ip
    // _WiFiManager.setSTAStaticIPConfig(
    //    IPAddress(10,0,1,99), IPAddress(10,0,1,1), IPAddress(255,255,255,0));
    // _WiFiManager.setShowStaticFields(true); // force show static ip fields
    // _WiFiManager.setShowDnsFields(true);    // force show dns field always

    // how long to try to connect for before continuing
    _WiFiManager.setConnectTimeout(10);

    // auto close configportal after n seconds
    _WiFiManager.setConfigPortalTimeout(3600U);

    // disable captive portal redirection
    _WiFiManager.setCaptivePortalEnable(false);

    // avoid timeout if client connected to softap
    // _WiFiManager.setAPClientCheck(true);

    // wifi scan settings
    // _WiFiManager.setShowInfoErase(false); // hide erase button on info page
    // _WiFiManager.setScanDispPerc(true);   // show RSSI as percentage

    Serial.println("WiFi Commissioner Initialized");
    is_initialized = true;
    return true;
}

bool WiFiCommissioner::connect()
{
    // Do nothing if component is not initialized
    if (is_initialized == false)
    {   return false;   }

    // Commission AP will be something like:
    // SSID - "espmultilog_012345678900"
    // PSK  - "espmultilog1234"
    if (!_WiFiManager.autoConnect(get_device_id(), ns_const::WIFI_AP_PWD))
    {
        Serial.println("WiFi failed to connect");
        return false;
    }
    else
    {
        Serial.println("WiFi Connected");
        return true;
    }
}

void WiFiCommissioner::process()
{
    // Do nothing if component is not initialized
    if (is_initialized == false)
    {   return;   }

    _WiFiManager.process();
}

String WiFiCommissioner::param_get(String name)
{
    String value = "";

    if (_WiFiManager.server->hasArg(name))
    {   value = _WiFiManager.server->arg(name);   }

    return value;
}

/*****************************************************************************/

/* Private Methods */


/*****************************************************************************/
