// -------------------------------------------------
// Copyright (c) 2022 HiBit <https://www.hibit.dev>
// -------------------------------------------------

#include "SPI.h"
#include "RF24.h"
#include "nRF24L01.h"
#include "AirControl.h"

// NRF24 pins
#define RF24_CE_PIN 9
#define RF24_CSN_PIN 10 //PIN 10 must be output to work as SPI master

// Joystick pins
// Due to physical position of analog modules X and Y axes are inverted
#define ANALOG_LEFT_X_PIN A3
#define ANALOG_LEFT_Y_PIN A4
#define ANALOG_LEFT_BUTTON_PIN A2

#define ANALOG_RIGHT_X_PIN A6
#define ANALOG_RIGHT_Y_PIN A7
#define ANALOG_RIGHT_BUTTON_PIN A5

#define TOGGLE_UPPER_PIN 3
#define TOGGLE_LOWER_PIN 2

#define BUTTON_LEFT_UPPER_PIN 8
#define BUTTON_LEFT_LOWER_PIN 7
#define BUTTON_RIGHT_UPPER_PIN 6
#define BUTTON_RIGHT_LOWER_PIN 5

#define POTENTIOMETER_LEFT_PIN A0
#define POTENTIOMETER_RIGHT_PIN A1

// Default idle values (calibration)
// Due to physical position of analog modules X and Y axes are inverted
#define ANALOG_LEFT_X_CORRECTION 127
#define ANALOG_LEFT_Y_CORRECTION 128

#define ANALOG_RIGHT_X_CORRECTION 126
#define ANALOG_RIGHT_Y_CORRECTION 122

// Debug & security
#define DEBUG false
#define SECURITY_KEY "hibit" // 10 characters max

// Air Control joystick
air_control airControl;

// NRF24L01
RF24 radio(RF24_CE_PIN, RF24_CSN_PIN);

void setup()
{
  pinMode(ANALOG_LEFT_BUTTON_PIN, INPUT_PULLUP);
  pinMode(ANALOG_RIGHT_BUTTON_PIN, INPUT_PULLUP);

  pinMode(TOGGLE_UPPER_PIN, INPUT_PULLUP);
  pinMode(TOGGLE_LOWER_PIN, INPUT_PULLUP);

  pinMode(BUTTON_LEFT_UPPER_PIN, INPUT_PULLUP);
  pinMode(BUTTON_LEFT_LOWER_PIN, INPUT_PULLUP);
  pinMode(BUTTON_RIGHT_UPPER_PIN, INPUT_PULLUP);
  pinMode(BUTTON_RIGHT_LOWER_PIN, INPUT_PULLUP);

  if (DEBUG) {
    Serial.begin(115200);
  }

  const byte address[6] = "00001";

  radio.begin();

  //Append ACK packet from the receiving radio back to the transmitting radio
  radio.setAutoAck(false); //(true|false)
  //Set the transmission datarate
  radio.setDataRate(RF24_250KBPS); //(RF24_250KBPS|RF24_1MBPS|RF24_2MBPS)
  //Greater level = more consumption = longer distance
  radio.setPALevel(RF24_PA_LOW); //(RF24_PA_MIN|RF24_PA_LOW|RF24_PA_HIGH|RF24_PA_MAX)
  //Default value is the maximum 32 bytes
  radio.setPayloadSize(sizeof(air_control));
  //Act as emitter
  radio.openWritingPipe(address);
  radio.stopListening();
}

void loop()
{
  strcpy(airControl.key, SECURITY_KEY); // Skip if not needed. Default value: hibit

  airControl.analogs.left = readAnalog(ANALOG_LEFT_X_PIN, ANALOG_LEFT_Y_PIN, ANALOG_LEFT_BUTTON_PIN, ANALOG_LEFT_X_CORRECTION, ANALOG_LEFT_Y_CORRECTION);
  airControl.analogs.right = readAnalog(ANALOG_RIGHT_X_PIN, ANALOG_RIGHT_Y_PIN, ANALOG_RIGHT_BUTTON_PIN, ANALOG_RIGHT_X_CORRECTION, ANALOG_RIGHT_Y_CORRECTION);

  airControl.toggles.upper = readToggle(TOGGLE_UPPER_PIN);
  airControl.toggles.lower = readToggle(TOGGLE_LOWER_PIN);

  airControl.buttons.left.upper = readButton(BUTTON_LEFT_UPPER_PIN);
  airControl.buttons.left.lower = readButton(BUTTON_LEFT_LOWER_PIN);
  airControl.buttons.right.upper = readButton(BUTTON_RIGHT_UPPER_PIN);
  airControl.buttons.right.lower = readButton(BUTTON_RIGHT_LOWER_PIN);

  airControl.potentiometers.left = readPotentiometer(POTENTIOMETER_LEFT_PIN);
  airControl.potentiometers.right = readPotentiometer(POTENTIOMETER_RIGHT_PIN);

  radio.write(&airControl, sizeof(air_control));

  if (DEBUG) {
    debug(airControl);
  }
}
