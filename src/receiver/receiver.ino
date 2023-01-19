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

// Time intervals for connection
#define INTERVAL_MS_SIGNAL_LOST 1000
#define INTERVAL_MS_SIGNAL_RETRY 250

// Debug & security
#define DEBUG false
#define SECURITY_KEY "hibit"

// Air Control joystick
air_control airControl;

// NRF24L01
RF24 radio(RF24_CE_PIN, RF24_CSN_PIN);

// Latest signal time
unsigned long lastSignalMillis = 0;

void setup()
{
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
  //Act as receiver
  radio.openReadingPipe(0, address);
  radio.startListening();
}

void loop()
{
  if (radio.available() > 0) {
    radio.read(&airControl, sizeof(air_control));

    if (strcmp(airControl.key, SECURITY_KEY) != 0) {
      invalidConnection(); // Skip if security code is not used
    } else {
      if (DEBUG) {
        debug(airControl);
      }

      // TODO
      // Available values

      // (byte) airControl.analogs.left.x
      // (byte) airControl.analogs.left.y
      // (byte) airControl.analogs.left.button.pressed

      // (byte) airControl.analogs.right.x
      // (byte) airControl.analogs.right.y
      // (byte) airControl.analogs.right.button.pressed

      // (byte) airControl.toggles.upper.on
      // (byte) airControl.toggles.lower.on

      // (byte) airControl.buttons.left.upper.pressed
      // (byte) airControl.buttons.left.lower.pressed
      // (byte) airControl.buttons.right.upper.pressed
      // (byte) airControl.buttons.right.lower.pressed

      // (byte) airControl.potentiometers.left.level
      // (byte) airControl.potentiometers.right.level

      lastSignalMillis = millis();
    }
  }

  if (millis() - lastSignalMillis > INTERVAL_MS_SIGNAL_LOST) {
    lostConnection();
  }
}

void invalidConnection()
{
  if (DEBUG) {
    Serial.println("Data received but security key is invalid!");

    delay(INTERVAL_MS_SIGNAL_RETRY);
  }
}

void lostConnection()
{
  if (DEBUG) {
    Serial.println("Connection lost, preventing unwanted behavior!");

    delay(INTERVAL_MS_SIGNAL_RETRY);
  }
}
