float measureCapacitance(int timeout) {
  analogReference(DEFAULT);
  
  byte chargePin = 0;
  byte dischargePin = 0;
  long startTime = micros();
  
  setPin(DIV_PIN[chargePin], OUTPUT, HIGH);
  //long startTimeMillis = startTime/1000;
  while(analogRead(DIV_INPUTPIN) < 645);/* {
    if (startTimeMillis + timeout <= millis())
      return -1;
  }*/
  long endTime = micros();

  float capacitance = (float)(endTime - startTime) / DIV_VALUES[chargePin];

  setPin(DIV_PIN[chargePin], INPUT, LOW);

  // Discharge cap.
  setPin(DIV_PIN[dischargePin], OUTPUT, LOW);
  delay(500);
  setPin(DIV_PIN[dischargePin], INPUT, LOW);

  return capacitance;
}
