#include <Ticker.h>

Ticker timer;

int D1 = 14;
int D2 = 15;
int D3 = 33;
int D4 = 13;
int latchPin = 12;
int clockPin = 27;
int dataPin = 14;

int count = 0;
int numbers[4];
int cathodePins[] = {14, 15, 33, 13};

byte table[10] = {
  B11111100, B01100000, B11011010, B11110010, B01100110,
  B10110110, B10111110, B11100000, B11111110, B11110110
};

void setup() {
  Serial.begin(115200);
  pinMode(D4, OUTPUT);
  pinMode(D3, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(D1, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  digitalWrite(D4, HIGH);
  digitalWrite(D3, HIGH);
  digitalWrite(D2, HIGH);
  digitalWrite(D1, HIGH);
  Serial.println("Enter a number between 0 and 9999");
}

void loop() {
  if (Serial.available()) {
    String s = Serial.readString();
    long number = (long)s.toInt();
    
    if (number > 9999) {
      Serial.println("Enter a number between 0 and 9999");
    } else {
      displayTemperature(number);
    }
  }
}

void displayTemperature(int temperature) {
  timer.detach();
  screenOff();
  separate(temperature);
  timer.attach_ms(1, Display);
}

void separate(long num) {
  numbers[0] = num / 1000;
  num = num % 1000;
  numbers[1] = num / 100;
  num = num % 100;
  numbers[2] = num / 10;
  numbers[3] = num % 10;
}

void Display() {
  screenOff();
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, LSBFIRST, table[numbers[count]]);
  digitalWrite(cathodePins[count], LOW);
  digitalWrite(latchPin, HIGH);
  count++;
  if (count == 4) {
    count = 0;
  }
}

void screenOff() {
  digitalWrite(D4, HIGH);
  digitalWrite(D3, HIGH);
  digitalWrite(D2, HIGH);
  digitalWrite(D1, HIGH);
}
