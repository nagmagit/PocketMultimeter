float measureVoltage(float reference) {
  analogReference(DEFAULT);
  
  float V = analogRead(DIV_INPUTPIN) * reference / 1023.0;

  delay(10);
  
  return V;
}
