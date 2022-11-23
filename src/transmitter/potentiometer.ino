// -------------------------------------------------
// Copyright (c) 2022 HiBit <https://www.hibit.dev>
// -------------------------------------------------

potentiometer readPotentiometer(int pin)
{
  potentiometer potentiometer;

  potentiometer.level = readPotentiometerLevelMapped(pin);

  return potentiometer;
}

byte readPotentiometerLevelMapped(int pin)
{
  return map(analogRead(pin), 0, 1023, 0, 255);
}
