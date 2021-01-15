#include <LiquidCrystal.h>

const byte  MODES        = 4;   // How many modes are there?
const byte  DIV_INPUTPIN = A5;  // Analog pin that will measure Vo
const byte  DIV_PINCOUNT = 4;   // How many dividers are available
const byte  DIV_PIN[]    = {    5,     6,      7,       8 };  // Voltage divider's' pin
const long  DIV_VALUES[] = { 1000,  3600,  20000,  275000 };  // Voltage divider's R1 value
const long  DIV_MAXRES[] = { 5000, 50000, 500000, 2000000 };  // Voltage divider's max R2 value
const float BAT_FULL  = 4.256;
const float BAT_EMPTY = 3.906;

// LCD config.
const byte rs = 4, en = 13, d4 = 12, d5 = 11, d6 = 10, d7 = 9;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// Mode selection
byte mode = 1;
long lastButtonMillis = 0; // Keep track of the last button press time for debouncing

void selectNextMode() {
  long currentMillis = millis();
  
  if (lastButtonMillis + 10 < currentMillis) {
    if (mode == MODES) mode = 0;
    else mode++;

    lastButtonMillis = currentMillis;
  }

  stopBuzz(); // When changing modes turn off the buzzer
}

void setup() {
  Serial.begin(9600);
  lcd.begin(8, 2);

  // Set each divider pin as high impedance and low state (basically disabled)
  for (int i = 0; i < DIV_PINCOUNT; i++) {
    setPin(DIV_PIN[i], INPUT, LOW);
  }

  // Mode select button
  pinMode(2, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(2), selectNextMode, FALLING);
}

void loop() {
  char* label;
  char value[8];

  switch (mode) {
    case 0:
      label = "BATERIA:";
      batteryLevelAsText(value);
      break;
    case 1:
      label = "RESIST.:";
      resistanceAsText(value);
      break;
    case 2:
      label = "CAPACI.:";
      capacitanceAsText(value);
      break;
    case 3:
      label = "VOLTAJ.:";
      voltageAsText(value);
      break;
    case 4:
      label = "TEMPER.:";
      tempAsText(value);
      break;
  }

  lcd.clear();

  lcd.setCursor(0, 0);  // Set the first half of the screen
  lcd.print(label);

  lcd.setCursor(0, 1);  // Set the second half of the screen.
  lcd.print(value);

	delay(100);
}

char* tempAsText(char* buffer) {
  sprintf(buffer, "Infinita");
  
  return buffer;
}

char* resistanceAsText(char* buffer) {
  float res = measureResistance(5);
  char floatBuffer[6];

  if (res < 50 && res > -1) {
    playBuzz();
  } else {
    stopBuzz();
  }
  
  if (res == -1 || res > DIV_MAXRES[DIV_PINCOUNT-1]) {
    sprintf(buffer, "Infinita");
  } else if (res < 1000) {
    dtostrf(res, 3, 2, floatBuffer);
    sprintf(buffer, "%s%c", floatBuffer, (char)0xF4);
  } else if (res < 1000000) {
    dtostrf(res/1000, 3, 2, floatBuffer);
    sprintf(buffer, "%sk%c", floatBuffer, (char)0xF4);
  } else {
    dtostrf(res/1000000, 3, 2, floatBuffer);
    sprintf(buffer, "%sM%c", floatBuffer, (char)0xF4);
  }
  
  return buffer;
}

char* capacitanceAsText(char* buffer) {
  float cap = measureCapacitance(5000);
  char floatBuffer[6];
  
  if (cap == -1) {
    sprintf(buffer, "En corto");
  } else if (cap < 1000) {
    dtostrf(cap, 3, 2, floatBuffer);
    sprintf(buffer, "%s%cF", floatBuffer, (char)0xE4);
  } else if (cap < 1000000) {
    dtostrf(cap/1000, 3, 2, floatBuffer);
    sprintf(buffer, "%smF", floatBuffer);
  }
  
  return buffer;
}

char* voltageAsText(char* buffer) {
  float v = measureVoltage(getBatteryVoltage());
  char floatBuffer[5];
  
  dtostrf(v, 1, 3, floatBuffer);
  sprintf(buffer, "  %sV", floatBuffer);
  
  return buffer;
}

char* batteryLevelAsText(char* buffer) {
  float bat = (getBatteryVoltage()-BAT_EMPTY)*100 / (BAT_FULL-BAT_EMPTY);
  char floatBuffer[3];
  
  dtostrf(bat, 3, 0, floatBuffer);
  sprintf(buffer, "    %s%%", floatBuffer);
  
  return buffer;
}
