# espmultilog

ESP32 based device to remote log different kind of data interfaces like DIO, ADC, USART, I2C, SPI, CAN, etc.

An Espmultilog device will monitorize the different interfaces availables on the ESP32 controller and forward the information of these interfaces through a MQTT connection in order to share and allow access to it remotely.

This opens the door to develop and use of high level software tools or REST APIs to monitorize or control the device status.

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
