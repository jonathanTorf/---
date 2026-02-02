#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_LEDBackpack.h>
#include "snake.h"

Adafruit_8x8matrix matrix = Adafruit_8x8matrix();

const int xPin = A0;
const int yPin = A1;
const int swPin = 2;

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
  int xVal = analogRead(xPin);             //reads the X value of the joystick 
  int joyStickButton = digitalRead(swPin); //reads the button of the joystick
  Serial.print("xVal: ");
  Serial.println(xVal);
  Serial.print("joyStickButton: ");
  Serial.println(joyStickButton);
  Serial.println("");

  if (xVal > 70) {   //if the joystick was moved far enoth to the left
    gameSelection--; //move to teh previous game in the list
    selectScreen();
  }
  else if (xVal < 30) { //if the joystick was moved far enoth to the right
    gameSelection++;    //move to teh next game in the list
    selectScreen();
  }
  if (joyStickButton == LOW) {            //if the button was pressed
    Serial.println("button pressed");
    if (gameSelection == 0) startSnake(); //if the selected game value is 0, start a game of snake
  }
  
  delay(200);
}

void selectScreen() {
  if (gameSelection == 0) DrawBitMap(); //if the selected game value is 0, show the icon for snake
  else if (gameSelection == 1) {        //if the selected game value is 1
    matrix.clear();                     //clear the screen(it would have been another games icon but thers currantly only snake
    matrix.writeDisplay();
  }
  gameSelection = constrain(gameSelection, 0, 1); //keep gameSelection between 0 and 1
}

/*const uint8_t gameOverBitmap[] PROGMEM = {
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000
};*/
