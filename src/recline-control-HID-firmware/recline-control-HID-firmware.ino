#include "stdio.h"
#include "pico/stdlib.h"
#include "hardware/uart.h"

// Pin Definitions
const int rockerSwitchPins[] = {0, 1, 2, 3, 4, 5}; // GPIO0 to GPIO5
const int buttonPins[] = {6, 8, 9, 10};           // GPIO6, GPIO8, GPIO9, GPIO10
const int powerGoodPin = 13;                      // GPIO13
const int uartTxPin = 16;                         // GPIO16
const int uartRxPin = 17;                         // GPIO17
const int uartTxEnPin = 18;                       // GPIO18
const int uartRxEnPin = 19;                       // GPIO19
const int uartRxLedPin = 20;                      // GPIO20
const int uartTxLedPin = 21;                      // GPIO21
const int sevenSegmentPins[] = {22, 23, 24, 25, 26, 27, 28, 29}; // GPIO22 to GPIO29

void setup() {
    // Configure Rocker Switch Pins as Inputs
    for (int i = 0; i < 6; i++) {
        pinMode(rockerSwitchPins[i], INPUT_PULLUP);
    }

    // Configure Button Pins as Inputs
    for (int i = 0; i < 4; i++) {
        pinMode(buttonPins[i], INPUT_PULLUP);
    }

    // Configure Power Good Pin as Input
    pinMode(powerGoodPin, INPUT);

    // Configure UART TX/RX Enable Pins as Outputs
    pinMode(uartTxEnPin, OUTPUT);
    pinMode(uartRxEnPin, OUTPUT);

    // Configure UART TX/RX LED Pins as Outputs
    pinMode(uartTxLedPin, OUTPUT);
    pinMode(uartRxLedPin, OUTPUT);

    // Configure 7-Segment Display Pins as Outputs
    for (int i = 0; i < 8; i++) {
        pinMode(sevenSegmentPins[i], OUTPUT);
        digitalWrite(sevenSegmentPins[i], LOW); // Turn off all segments initially
    }

    // Configure UART TX/RX pins and initialize UART1
    Serial1.setRX(uartRxPin); // Set the UART RX pin
    Serial1.setTX(uartTxPin); // Set the UART TX pin
    Serial1.begin(9600);      // Start UART1 with a baud rate of 9600

    // Set UART TX/RX Enable to Default States
    digitalWrite(uartTxEnPin, LOW); // Disable TX initially
    digitalWrite(uartRxEnPin, HIGH); // Enable RX initially
}

void loop() {
    // Monitor Rocker Switch States
    for (int i = 0; i < 6; i++) {
        int state = digitalRead(rockerSwitchPins[i]);
        // Process rocker switch state
    }

    // Monitor Button States
    for (int i = 0; i < 4; i++) {
        int state = digitalRead(buttonPins[i]);
        // Process button state
    }

    // Check Power Good Signal
    bool powerGood = digitalRead(powerGoodPin);
    if (!powerGood) {
        // Handle power failure
    }

    // Handle UART Communication
    handleUART();

    // Update 7-Segment Display (example: displaying number 3)
    int numberToDisplay = 3; // Example number
    updateSevenSegmentDisplay(numberToDisplay);
}

void handleUART() {
    // Handle incoming UART data
    if (Serial1.available()) {
        // Enable RX and turn on RX LED
        digitalWrite(uartRxEnPin, HIGH);
        digitalWrite(uartRxLedPin, HIGH);

        // Read incoming data
        char received = Serial1.read();

        // Process received data
        // (Add logic here for interpreting or acting on received data)

        // Turn off RX LED after processing
        digitalWrite(uartRxLedPin, LOW);
    }

    // Transmit data example
    if (true) { // Replace with your actual condition
        // Enable TX and turn on TX LED
        digitalWrite(uartTxEnPin, HIGH);
        digitalWrite(uartTxLedPin, HIGH);

        Serial1.print("Hello, RS485!"); // Send data over RS485

        // Turn off TX LED and disable TX
        digitalWrite(uartTxLedPin, LOW);
        digitalWrite(uartTxEnPin, LOW);
    }
}

void updateSevenSegmentDisplay(int number) {
    // Segment mappings for digits 0-9
    const uint8_t segmentMap[] = {
        0b00111111, // 0
        0b00000110, // 1
        0b01011011, // 2
        0b01001111, // 3
        0b01100110, // 4
        0b01101101, // 5
        0b01111101, // 6
        0b00000111, // 7
        0b01111111, // 8
        0b01101111  // 9
    };

    uint8_t segments = segmentMap[number % 10];

    for (int i = 0; i < 8; i++) {
        digitalWrite(sevenSegmentPins[i], segments & (1 << i));
    }
}
