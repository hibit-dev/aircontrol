// -------------------------------------------------
// Copyright (c) 2022 HiBit <https://www.hibit.dev>
// -------------------------------------------------

analog readAnalog(int xPin, int yPin, int buttonPin, int xCorrection, int yCorrection)
{
  analog analog;

  analog.x = readAnalogAxisLevelMapped(xPin) - xCorrection;
  analog.y = readAnalogAxisLevelMapped(yPin) - yCorrection;

  analog.button.pressed = isAnalogPressed(buttonPin);

  return analog;
}

byte readAnalogAxisLevelMapped(int pin)
{
  return map(analogRead(pin), 0, 1023, 0, 255);
}

bool isAnalogPressed(int pin)
{
  return digitalRead(pin) == 0;
}
