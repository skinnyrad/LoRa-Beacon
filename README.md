# LoRa-Beacon
A project utilizing a compact LoRa transmitter to broadcast a simple beacon message across LoRa frequencies, enabling long-range, low-power communication.  The code supports various Arduino boards, including **Feather 32u4** and **Feather M0**, and is configurable for different transmission parameters such as frequency, power, and bandwidth. Additionally, the project includes an example of basic encryption to introduce concepts of secure communication.

## Prerequisites

- Arduino IDE installed on your computer.
- An Arduino board (Feather 32u4, Feather M0, or any other device compatible with the [RadioHead Packet Radio library](https://www.airspayce.com/mikem/arduino/RadioHead/)).

## Hardware Setup

* Plug the device into your workstation via USB, and attach to your board in Arduino (Tools -> Port)

## Software Setup

1. Install the necessary libraries for the Adafruit Feather M0 with RFM95 LoRa Radio:

* https://learn.adafruit.com/adafruit-feather-m0-radio-with-lora-radio-module/setup
* https://learn.adafruit.com/adafruit-feather-m0-radio-with-lora-radio-module/using-with-arduino-ide

2. For instructions on setting up the 32u4:
* https://learn.adafruit.com/adafruit-feather-32u4-radio-with-lora-radio-module/setup

3. Flash the Adafruit Feather M0:

* Open the LoRa-Beacon.ino file in Arduino IDE
* Select Tools -> Board -> Adafruit SAMD Boards -> Adafruit Feather M0 (SAMD21)
* Select Tools -> Port -> [Your Adafruit Feather M0]
* Select Sketch -> Include Library -> Add .Zip Library -> Select the RadioHead.zip file from this repository
* Click the Upload Button


## Sending Messages

- By default, the sketch is set to transmit a message every second. The message and transmission parameters can be modified in the configuration section of the code.

```cpp
uint8_t message[] = "Device ID: 0001; Status: Active;"; // Message you want to beacon
```

- To switch to encrypted transmission, uncomment the `encryptedTransmit(message, sizeof(message));` line in the `loop()` function and comment out the regular `transmit` call.

```cpp
// Example of how to send an encrypted message instead
encryptedTransmit(message, sizeof(message)); 
```

## Receiving Messages
- To receive messages transmitted by the beacon, you can utilize the [Skinny-LoRa](https://github.com/skinnyrad/Skinny-LoRa) tool.  Skinny-LoRa is a terminal application for transmitting and receiving LoRa traffic using a single Adafruit Feather M0 with RFM95 LoRa Radio.
- Skinny-LoRa will print out any messages received, along with the RSSI value of the packet.  This is quite useful when tracking down the beacon.

## Function Documentation and Code Explanation

### `Configuration Section`

Defines crucial parameters for LoRa communication such as frequency, power level, bandwidth, coding rate, and spreading factor. These settings can be tailored to your specific needs and regulations of your region.

```cpp
#define FREQ 915.0
#define PWR 20
#define BANDWIDTH 125000
#define CR 5
#define SF 7
```

### `transmit Function`

Sends a given message over LoRa.

**Parameters:**
- `msg`: Pointer to the message array to be sent.
- `msg_length`: Length of the message in bytes.

**Usage:**
```cpp
uint8_t message[] = "Your message here";
transmit(message, sizeof(message));
```

### `encryptedTransmit Function`

Encrypts a given message using a simple XOR operation and transmits it over LoRa.

**Parameters:**
- `msg`: Pointer to the message array to be encrypted and sent.
- `msg_length`: Length of the message in bytes.

**Usage:**
- This function is an example of how to implement basic encryption. For serious applications, consider more robust encryption methods.

### `setup Function`

Initializes the LoRa module with the configured parameters. This function is called once at the start of the program.

### `loop Function`

Contains the main logic to transmit messages. By default, it sends a predefined message every second. Users can switch to encrypted messages by modifying the code as described in the "How to Use" section.
  
