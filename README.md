# espmultilog

ESP32 based device to remote log different kind of data interfaces like DIO, ADC, USART, I2C, SPI, CAN, etc.

An Espmultilog device will monitorize the different interfaces availables on the ESP32 controller and forward the information of these interfaces through a MQTT connection in order to share and allow access to it remotely.

This opens the door to develop and use of high level software tools or REST APIs to monitorize or control the device status.

## Device Setup

Once the device startup for the first time, it requires internet connection so a Commisionning procedure through WiFi must be followed by the user in order to setup the WiFi network credentials into the device.

The device uses WiFi Manager to handle this procedure, at startup it try to connect to the last configured WiFi network but in case there is no connection configured or the connection fails, the device will launch a WiFi Access Point (AP) to allow the user connect into it and access to the Commisioning Web Server for device configuration.

The device WiFi AP that will be triggered by default will be something like the follow:

AP SSID: epmultilog_NNNNNNNNNNNN (the "NNNNNNNNNNNN" is the unique device MAC Address)
AP Password: espmultilog1234

To setup the device, the user must connect to this WiFi AP with any device (i.e. PC, Smartphone, etc.), then use a web browser and access to the device configuration web page by accessing the address "192.168.4.1". Once the page is available, provide your network WiFi connection credentials to the device in order to make the device store this information and connect to it for the standard operation.

A device that has been setup, will use the configured credentials to connect and use that WiFi network in the future (after any device reboot), so there is no need to do this setup again (or do it again in case you want to modify the WiFi network were the device should connect, for example if you plan to move the device to a different location and use a different network).

## Device Command Line Interface

The device provides a Command Line Interface (CLI) through the default Serial interface that can be used to check information, and control/configure the device.

The default CLI configuration is:

- Serial
- 8 data bits
- 1 stop bit
- none parity
- 115200 bauds
- All end of lines supported (CR, LF and CRLF).

## Interface Support Status

Here is the list of current supported interfaces:

- [ ] ADC.
- [ ] DIO.
- [ ] I2C.
- [ ] Serial UART/USART.
- [ ] SPI.
- [ ] System.
- [ ] TWAI (CAN).

## Device Support Status

The project has been tested and validated on the next ESP32 devices (could work on other ESP32 devices but has not been tested yet):

- [ ] ESP32.
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

## SPI Interface

The project could allow logging any **SPI transactions** that flows through an SPI interface.

## System Interface

The project could allow to send system information like device variant, running SDK and framework version, running time from last reboot, etc.

## TWAI (CAN)

The project could allow logging **Two-Wire Automotive Interface (TWAI) messages** that flows through a **CAN Bus** interface.
