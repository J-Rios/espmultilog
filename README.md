# espmultilog

ESP32 based device to remote log different kind of data interfaces like DIO, ADC, USART, I2C, SPI, CAN, etc.

An Espmultilog device will monitorize the different interfaces availables on the ESP32 controller and forward the information of these interfaces through a MQTT connection in order to share and allow access to it remotely.

This opens the door to develop and use of high level software tools or REST APIs to monitorize or control these device interfaces.

## Device Setup

The device requires a network connection to a MQTT server, so the first time the device is powered, a **Commisionning** procedure through WiFi must be followed by the user in order to setup the WiFi network credentials into the device.

The device uses **WiFi Manager** to handle this procedure, at startup it tries to connect to the last configured WiFi network but in case there is no connection configured or the connection fails, the device will launch a WiFi Access Point (AP) to allow the user connect into it and access to the Commisioning Web Server for device configuration.

The device WiFi AP that will be triggered by default will be something like the follow:

- **AP SSID:** epmultilog_XXXXXXXXXXXX
- **AP Password:** espmultilog1234

Note: The "XXXXXXXXXXXX" is the device MAC Address.

To setup the device, the user must connect to this WiFi AP with any device (i.e. PC, Smartphone, etc.), then use a web browser and access to the device configuration web page by accessing the address "192.168.4.1". Once the page is available, provide your network WiFi connection credentials to the device in order to make the device store this information and connect to it for the standard operation.

A device that has been setup, will use the configured credentials to connect and use that WiFi network in the future (after any device reboot), so there is no need to do this setup again (only in case you want to modify the WiFi network were the device should connect, for example if you plan to move the device to a different location and use a different network).

## Device Command Line Interface

The device provides a Command Line Interface (CLI) through the default Serial interface that can be used to check information, and control/configure the device.

The default CLI configuration is:

- Serial
- 8 data bits
- 1 stop bit
- none parity
- 115200 bauds
- All end of lines supported (CR, LF and CRLF).

### Device setup through CLI

These are the available commands that you can use in the CLI:

```bash
# Reboot the device
reboot

# Show device current firmware version
version

# Show Current WiFi Connection Information
wifi_status

# Setup and Control logging of an UART Port
uart N command [arg1] [arg2]
```

## Interface Support Status

Here is the list of current supported interfaces:

- [ ] ADC.
- [ ] DIO.
- [ ] I2C.
- [x] Serial UART/USART.
- [ ] SPI.
- [ ] System.
- [ ] TWAI (CAN).

## Device Support Status

The project has been tested and validated on the next ESP32 devices (could work on other ESP32 devices but has not been tested yet):

- [x] ESP32.
- [ ] ESP32-C2.
- [ ] ESP32-C3.
- [ ] ESP32-C6.
- [ ] ESP32-H2.
- [ ] ESP32-S2.
- [ ] ESP32-S3.

## ADC Interface

The project could allow logging the **Analog to Digital Converters (ADCs) input values** measurements.

## DIO Interface

The project could allow logging the state of the **Digital Inputs and Outputs (DIOs)** of the ESP32 in real time, showing if each DIO is configured as input or output and it current **logical level state (LOW or HIGH)**.

## I2C

The project could allow logging any **I2C transactions** that flows through an I2C interface.

## UART/USART Interface

The project could allow logging any **Serial communication message** that flows through an **UART/USART port**.

The ESP32 devices have 2 or 3 configurable UARTs that can be used, the first UART is setup in this project to be used as a CLI for device debug and configuration so that Port is not allowed to be used for logging.

By default, the device doesn't log any of the UARTs, the user is required to remotely configure and enable any of the UARTs through MQTT to make it start logging.

There is 3 types of MQTT Topics related to UARTs Interface Logging:

- **/XXXXXXXXXXXX/uart/N/cfg** - Topic for UART Ports Configuration.
- **/XXXXXXXXXXXX/uart/N/rx** - Topic to log received data from the UART Port.
- **/XXXXXXXXXXXX/uart/N/tx** - Topic to send and log transmitted data through the UART Port.

Notes:

- The **"XXXXXXXXXXXX"** is the target device UUID (MAC Address, i.e. "1234567890AB").
- The **"N"** is a number ("1" or "2") to target the device UART Port to configure/log/control.
- If UART speed is not configured, by default the device will use 115200 bauds on all the Ports.

### UART/USART Interface Setup example

For example, the procedure to configure and enable the logging of the UART Port 1 on a device with a MAC address 12:34:56:78:90:AB, is as follows:

1. Power-up the device and wait until it is connected to the MQTT Server.

2. Subscribe to device UART configuration MQTT topic to check acknowledge from the device to the future requests:

    ```bash
    mosquitto_sub -F '%I\n%t\n%p\n' -h "test.mosquitto.org" -p 1883 -t "/1234567890AB/uart/#"
    ```

3. Send a MQTT message to setup UART port 1 communication speed to 9600 bauds:

    ```bash
    mosquitto_pub -h "test.mosquitto.org" -p 1883 -t "/1234567890AB/uart/1/cfg" -m "bauds 9600"
    ```

4. Check for device acknowledge response in the same topic with *"bauds ok"* to be sure it was set properly.

5. Send a MQTT message to enable logging on UART Port 1:

    ```bash
    mosquitto_pub -h "test.mosquitto.org" -p 1883 -t "/1234567890AB/uart/1/cfg" -m "enable"
    ```

6. Check for device acknowledge response in the same topic with *"enabled"* to be sure it was set properly.

7. Subscribe to receive UART logging:

    ```bash
    mosquitto_sub -h "test.mosquitto.org" -p 1883 -t "/1234567890AB/uart/1/rx"
    ```

Note that you can modify the speed at any time without the need of disabling the port. Also, you can disable an UART port by:

```bash
mosquitto_pub -h "test.mosquitto.org" -p 1883 -t "/1234567890AB/uart/1/cfg" -m "disable"
```

## SPI Interface

The project could allow logging any **SPI transactions** that flows through an SPI interface.

## System Interface

The project could allow to send system information like device variant, running SDK and framework version, running time from last reboot, etc.

## TWAI (CAN)

The project could allow logging **Two-Wire Automotive Interface (TWAI) messages** that flows through a **CAN Bus** interface.
