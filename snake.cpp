#include "snake.h"

extern Adafruit_8x8matrix matrix;
extern const int xPin;
extern const int yPin;

int xDir, yDir;
int snakeLength;
Point snake[64];
Point appleLocation;

void startSnake() {
  snakeLength = 2;
  snake[0] = {4, 4};

  matrix.clear();
  matrix.drawPixel(4, 3, LED_ON);
  //matrix.writeDisplay();
  spawnApple();

  while (true) {
    int xVal = analogRead(xPin);
    int yVal = analogRead(yPin);
    if (xVal < 30 || yVal < 30 || xVal > 100 || yVal > 100) break;
    Serial.println("game waiting to start");
    delay(500);
  }

  while (true) {
    int xVal = analogRead(xPin);
    int yVal = analogRead(yPin);
    if (xVal < 30 && xDir != 1) {
      xDir = -1;
      yDir = 0;
    }
    else if (yVal < 30 && yDir != 1) {
      xDir = 0;
      yDir = -1;
    }
    else if (xVal > 100 && xDir != -1) {
      xDir = 1;
      yDir = 0;
    }
    else if (yVal > 100 && yDir != -1) {
      xDir = 0;
      yDir = 1;
    }
    Serial.print("xDir: ");
    Serial.println(xDir);
    Serial.print("yDir: ");
    Serial.println(yDir);
    Serial.println("");

    moveSnake();
    if (hitSomething()) {
      losingAnimation();
      DrawBitMap();
      break;
    }

    matrix.writeDisplay();
    delay(500);
  }
}

const uint8_t snakeBitmap[] PROGMEM = {
  0b00001100,
  0b00001000,
  0b00111000,
  0b00100000,
  0b00100000,
  0b00000000,
  0b00100000,
  0b00000100
};

void DrawBitMap() {
  matrix.clear();
  matrix.drawBitmap(0, 0, snakeBitmap, 8, 8, LED_ON);
  matrix.writeDisplay();
}

void moveSnake () {
    matrix.drawPixel(snake[snakeLength-1].x, (snake[snakeLength-1].y + 7) % 8, LED_OFF);

    for (int i = snakeLength - 1; i > 0; i--) snake[i] = snake[i - 1];

    snake[0].x += xDir;
    snake[0].y += yDir;
    snake[0].x = constrain(snake[0].x, 0, 7);
    snake[0].y = constrain(snake[0].y, 0, 7);

    matrix.drawPixel(snake[0].x, (snake[0].y + 7) % 8, LED_ON);
    if (snake[0].x == appleLocation.x && snake[0].y == appleLocation.y) {
      spawnApple();
      snakeLength++;
    }
}

void spawnApple() {
  //matrix.drawPixel(appleLocation.x, appleLocation.y, LED_OFF);
  bool validSpawn = false;

  while (!validSpawn) {
    validSpawn = true;

    appleLocation.x = random(0, 8);
    appleLocation.y = random(0, 8);

    for (int i = 0; i < snakeLength; i++) {
      if (appleLocation.x == snake[i].x && appleLocation.y == snake[i].y) {
        validSpawn = false;
        break;
      }
    }
  }
  matrix.drawPixel(appleLocation.x, (appleLocation.y + 7) % 8, LED_ON);
  matrix.writeDisplay();
}

bool hitSomething() {
  bool hitBool = false;
  if (snake[0].x < 0 || snake[0].x > 7) hitBool = true;
  if (snake[0].y < 0 || snake[0].y > 7) hitBool = true;

  for (int i = 1; i < snakeLength; i++) {
    if (snake[0].x == snake[i].x && snake[0].y == snake[i].y) {
      hitBool = true;
      break;
    }
  }

  return(hitBool);
}

void losingAnimation() {
  matrix.drawPixel(appleLocation.x, (appleLocation.y + 7 % 8), LED_OFF);
  matrix.writeDisplay();
  delay(25);
  for (int i = 0; i < snakeLength; i++) {
      matrix.drawPixel(snake[i].x, (snake[i].y + 7) % 8, LED_OFF);
      matrix.writeDisplay();
      delay(25);
  }
  matrix.clear();
  matrix.writeDisplay();
}