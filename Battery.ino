float getBatteryVoltage(void) {
  const long InternalReferenceVoltage = 1100L;  // Adjust this value to your boards specific internal BG voltage x1000
  // REFS1 REFS0          --> 0 1, AVcc internal ref. -Selects AVcc external reference
  // MUX3 MUX2 MUX1 MUX0  --> 1110 1.1V (VBG)         -Selects channel 14, bandgap voltage, to measure
  ADMUX = (0<<REFS1) | (1<<REFS0) | (0<<ADLAR) | (1<<MUX3) | (1<<MUX2) | (1<<MUX1) | (0<<MUX0);

  delay(50);  // Let mux settle a little to get a more stable A/D conversion
  // Start a conversion  
  ADCSRA |= _BV( ADSC );
  // Wait for it to complete
  while( ( (ADCSRA & (1<<ADSC)) != 0 ) );
  // Scale the value
  float results = (((InternalReferenceVoltage * 1024L) / ADC) + 5L) / 1000.0; // calculates for straight line value
  
  return results;
}
