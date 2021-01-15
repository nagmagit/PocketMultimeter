float measureResistance(int iterations) {
  analogReference(DEFAULT);
  
  int pin = getPinForR1();

  if (pin == -1) return -1;
  
  float resSum = 0;

  for (int i = 0; i < iterations; i++) {
    float Vo = analogRead(DIV_INPUTPIN);
    resSum += Vo*DIV_VALUES[pin] / (1023.0-Vo);

    delay(10);
  }
  
  return resSum / iterations;
}

char getPinForR1() {
  char pin = -1;

  for (char i = 0; i < DIV_PINCOUNT; i++) {
    setPin(DIV_PIN[i], OUTPUT, HIGH);
    
    float Vo = analogRead(DIV_INPUTPIN);
    float R2 = Vo*DIV_VALUES[i] / (1023.0-Vo);

    if (R2 < DIV_MAXRES[i]) {
      pin = i;
      break;
    } else {
      setPin(DIV_PIN[i], INPUT, LOW);
    }
  }

  return pin;
}

//
// Now an easter egg:
// Buzzer cucaracha
//

const byte BUZZER_PIN = 3;
const byte BUZZER_FRQ = 262;

const int song[] = {
  262, 262, 262, 349, 440,
  262, 262, 262, 349, 440,
  349, 349, 330, 330, 294, 294, 262,
  0, 0,
  262, 262, 262, 330, 392,
  262, 262, 262, 330, 392,
  523, 587, 523, 466, 440, 392, 349,
  0, 0
};

int notePlaying = 0;

void playBuzz(void) {
  noTone(BUZZER_PIN);
  
  tone(BUZZER_PIN, song[notePlaying++], 100000);

  if (notePlaying >= 38) notePlaying = 0;
}

void stopBuzz(void) {
  noTone(BUZZER_PIN);
}
