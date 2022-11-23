// -------------------------------------------------
// Copyright (c) 2022 HiBit <https://www.hibit.dev>
// -------------------------------------------------

button readButton(int pin)
{
  button button;

  button.pressed = isButtonPressed(pin);

  return button;
}

toggle readToggle(int pin)
{
  toggle toggle;

  toggle.on = isButtonPressed(pin);

  return toggle;
}

bool isButtonPressed(int pin)
{
  return digitalRead(pin) == 0;
}
