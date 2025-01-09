/*
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

// Recline HID Control RP2040 Firmware in Arduino
// This firmware is designed to control a variety of peripherals connected to the RP2040 board.
// It supports RS485 communication, motor control, solenoid activation, LED management,
// a 7-segment display, and button/switch monitoring. Detailed explanations of each section
// are provided below for clarity and maintainability.

#include "stdio.h"
#include "pico/stdlib.h"
#include "hardware/uart.h"

// Define payload structures for RS485 commands, tailored for various operations.
// Each structure is specific to a type of command that may be issued or received.

// Structure for motor control commands
struct MotorControlPayload {
    uint8_t motorID;    // ID of the motor to control (0x00 - 0x03)
    uint8_t direction;  // Direction of motor movement (0x00 = Stop, 0x01 = Forward, 0x02 = Reverse)
};

// Structure for solenoid control commands
struct SolenoidControlPayload {
    uint8_t solenoidID; // ID of the solenoid to control (0x00 - 0x04)
    uint8_t state;      // State of the solenoid (0x00 = Off, 0x01 = On)
};

// Structure for LED control commands
struct LEDControlPayload {
    uint8_t ledID;      // ID of the LED to control (0x00 - 0x01)
    uint8_t state;      // State of the LED (0x00 = Off, 0x01 = On)
};

// Structure for querying the status of peripherals
struct QueryStatusPayload {
    uint8_t peripheralID; // ID of the peripheral to query (0x00 = All, 0x01 = Motors, etc.)
};

// Structure for acknowledgment of received commands
struct AcknowledgmentPayload {
    uint8_t originalCommand; // The command being acknowledged
    uint8_t status;          // Status of execution (0x00 = Failure, 0x01 = Success)
};

// Union for payload data to encapsulate various types of command-specific information
union PayloadData {
    MotorControlPayload motorControl;
    SolenoidControlPayload solenoidControl;
    LEDControlPayload ledControl;
    QueryStatusPayload queryStatus;
    AcknowledgmentPayload acknowledgment;
};

// Define the RS485 datagram structure
// This encapsulates all data for transmission or reception over RS485
struct RS485Datagram {
    uint8_t startByte;       // Fixed value to indicate the start of a datagram (e.g., 0x7E)
    uint8_t address;         // Target device address
    uint8_t command;         // Command type
    uint8_t dataLength;      // Length of the payload
    PayloadData payload;     // Nested payload structure containing command-specific data
    uint8_t checksum;        // Checksum for error checking
    uint8_t endByte;         // Fixed value to indicate the end of a datagram (e.g., 0xFF)
};

// Pin definitions for hardware components
const int rockerSwitchPins[] = {0, 1, 2, 3, 4, 5}; // GPIO pins for rocker switches
const int buttonPins[] = {6, 8, 9, 10};           // GPIO pins for buttons
const int powerGoodPin = 13;                      // GPIO pin for power-good signal
const int uartTxPin = 16;                         // GPIO pin for UART TX
const int uartRxPin = 17;                         // GPIO pin for UART RX
const int uartTxEnPin = 18;                       // GPIO pin to enable UART TX
const int uartRxEnPin = 19;                       // GPIO pin to enable UART RX
const int uartTxLedPin = 20;                      // GPIO pin for UART TX LED
const int uartRxLedPin = 21;                      // GPIO pin for UART RX LED
const int sevenSegmentPins[] = {22, 23, 24, 25, 26, 27, 28, 29}; // GPIO pins for 7-segment display

void setup() {
    // Configure rocker switch pins as inputs with pull-up resistors
    for (int i = 0; i < 6; i++) {
        pinMode(rockerSwitchPins[i], INPUT_PULLUP);
    }

    // Configure button pins as inputs with pull-up resistors
    for (int i = 0; i < 4; i++) {
        pinMode(buttonPins[i], INPUT_PULLUP);
    }

    // Configure the power-good signal pin as an input
    pinMode(powerGoodPin, INPUT);

    // Configure UART TX/RX enable pins as outputs
    pinMode(uartTxEnPin, OUTPUT);
    pinMode(uartRxEnPin, OUTPUT);

    // Configure UART TX/RX LED pins as outputs
    pinMode(uartTxLedPin, OUTPUT);
    pinMode(uartRxLedPin, OUTPUT);

    // Configure 7-segment display pins as outputs and set them to LOW initially
    for (int i = 0; i < 8; i++) {
        pinMode(sevenSegmentPins[i], OUTPUT);
        digitalWrite(sevenSegmentPins[i], LOW); // Turn off all segments initially
    }

    // Configure UART pins and initialize UART1
    Serial1.setRX(uartRxPin); // Set UART RX pin
    Serial1.setTX(uartTxPin); // Set UART TX pin
    Serial1.begin(9600);      // Start UART1 with a baud rate of 9600

    // Set default states for UART TX/RX enable pins
    digitalWrite(uartTxEnPin, LOW); // Disable TX initially
    digitalWrite(uartRxEnPin, HIGH); // Enable RX initially
}

void loop() {
    // Monitor rocker switch states and process them as needed
    for (int i = 0; i < 6; i++) {
        int state = digitalRead(rockerSwitchPins[i]);
        // Process rocker switch state (e.g., toggle motors or solenoids)
    }

    // Monitor button states and process them as needed
    for (int i = 0; i < 4; i++) {
        int state = digitalRead(buttonPins[i]);
        // Process button state (e.g., trigger actions based on button presses)
    }

    // Check the power-good signal and handle power failure if detected
    bool powerGood = digitalRead(powerGoodPin);
    if (!powerGood) {
        // Handle power failure (e.g., shut down motors or alert the user)
    }

    // Handle UART communication (transmit or receive data)
    handleUART();

    // Update the 7-segment display to show a specific number (example: 3)
    updateSevenSegmentDisplay(3);
}

void handleUART() {
    // Check for incoming UART data
    if (Serial1.available()) {
        // Enable RX and turn on the RX LED
        digitalWrite(uartRxEnPin, HIGH);
        digitalWrite(uartRxLedPin, HIGH);

        // Read incoming data and process it
        char received = Serial1.read();
        // (Add logic here to interpret and respond to the received data)

        // Turn off RX LED after processing
        digitalWrite(uartRxLedPin, LOW);
    }

    // Example of transmitting a datagram over RS485
    RS485Datagram datagram;
    datagram.startByte = 0x7E;             // Start of datagram
    datagram.address = 0x01;               // Target device address
    datagram.command = 0x01;               // Command type (e.g., motor control)
    datagram.dataLength = sizeof(MotorControlPayload); // Payload length

    // Populate motor control payload
    datagram.payload.motorControl.motorID = 0x02; // Motor ID
    datagram.payload.motorControl.direction = 0x01; // Direction (Forward)

    // Compute checksum for data integrity
    datagram.checksum = datagram.startByte ^ datagram.address ^ datagram.command ^ datagram.dataLength;
    datagram.checksum ^= datagram.payload.motorControl.motorID ^ datagram.payload.motorControl.direction;

    datagram.endByte = 0xFF; // End of datagram

    // Send the datagram
    digitalWrite(uartTxEnPin, HIGH); // Enable TX
    Serial1.write((uint8_t*)&datagram, sizeof(datagram));
    digitalWrite(uartTxEnPin, LOW);  // Disable TX
}

void updateSevenSegmentDisplay(int number) {
    // Ensure the input number is within valid range (0-9)
    if (number < 0 || number > 9) {
        return; // Invalid input, do nothing
    }

    // Segment mappings for digits 0-9 (active-low logic for common-cathode displays)
    const uint8_t segmentMap[] = {
        0b11000000, // 0
        0b11111001, // 1
        0b10100100, // 2
        0b10110000, // 3
        0b10011001, // 4
        0b10010010, // 5
        0b10000010, // 6
        0b11111000, // 7
        0b10000000, // 8
        0b10010000  // 9
    };

    // Retrieve the bit pattern for the given number
    uint8_t segments = segmentMap[number];

    // Update each segment pin based on the bit pattern
    for (int i = 0; i < 8; i++) {
        digitalWrite(sevenSegmentPins[i], !(segments & (1 << i))); // Active-low logic: turn segment on/off
    }
}
