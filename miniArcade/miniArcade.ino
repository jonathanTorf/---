#include <Wire.h> //hello github
#include <Adafruit_GFX.h>
#include <Adafruit_LEDBackpack.h>
#include "snake.h"

Adafruit_8x8matrix matrix = Adafruit_8x8matrix();

int xPin = A0;
int yPin = A1;
int swPin = 2;

int gameSelection = 0;

void setup() {
  Serial.begin(9600);
  pinMode(swPin, INPUT_PULLUP);
  matrix.begin(0x70);   //default I2C address
  matrix.setBrightness(2);
  matrix.setRotation(1);

  digitalWrite(13, HIGH); //power on the joystick
  DrawBitMap();
}

void loop() {
  int xVal = analogRead(xPin);
  int joyStickButton = digitalRead(swPin);
  Serial.print("xVal: ");
  Serial.println(xVal);
  Serial.print("joyStickButton: ");
  Serial.println(joyStickButton);
  Serial.println("");

  if (xVal > 70) {
    gameSelection--;
    selectScreen();
  }
  else if (xVal < 30) {
    gameSelection++;
    selectScreen();
  }
  if (joyStickButton == LOW) {
    Serial.println("button pressed");
    if (gameSelection == 0) startSnake();
  }

  delay(200);
}

void selectScreen() {
  if (gameSelection == 0) DrawBitMap();
  else if (gameSelection == 1) {
    matrix.clear();
    matrix.writeDisplay();
  }
  gameSelection = constrain(gameSelection, 0, 1);
}

const uint8_t gameOverBitmap[] PROGMEM = {
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000
};