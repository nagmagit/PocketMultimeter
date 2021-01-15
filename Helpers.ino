void setPin(byte pin, byte impedance, byte state) {
  pinMode(pin, impedance);
  digitalWrite(pin, state);
}

// LCD special character codes:
// Ohm: (char)0xF4
// Mu:  (char)0xE4
