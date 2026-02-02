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
  matrix.drawPixel(4, 3, LED_ON); //light up the starting location of the snake
  spawnApple();

  while (true) { //while this loop is active, the snake isnt moveing
    int xVal = analogRead(xPin); //read the X value of the joystick
    int yVal = analogRead(yPin); //read the Y value of the joystick
    if (xVal < 30 || yVal < 30 || xVal > 100 || yVal > 100) break; //stop the loop if a joystick input is detected
    Serial.println("game waiting to start");
    delay(500);
  }

  while (true) { //while this loop is actve, the game is active
    int xVal = analogRead(xPin);
    int yVal = analogRead(yPin);
    if (xVal < 30 && xDir != 1) { //if the joystick was moved far enoth to the right and the snakes currant diraction is not left(so that it cant move into itself)
      xDir = -1;                  //set the X diraction to right
      yDir = 0;                   //set the Y diraction to nutral
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
      DrawBitMap(); //so the screen wouldnt be blank when it returns to the selcetion menue
      break;        //now the void loop() is waiting for you to start the next game
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
    matrix.drawPixel(snake[snakeLength-1].x, (snake[snakeLength-1].y + 7) % 8, LED_OFF); //turn off the pixel at the tail of the snake

    for (int i = snakeLength - 1; i > 0; i--) snake[i] = snake[i - 1];                   //shif every snake segmant value down by 1

    snake[0].x += xDir; //move the head of the snake in the X diraction
    snake[0].y += yDir; //move the head of the snake in the Y diraction
    snake[0].x = constrain(snake[0].x, 0, 7); //make sure the snake cant move outside of the screen
    snake[0].y = constrain(snake[0].y, 0, 7);

    matrix.drawPixel(snake[0].x, (snake[0].y + 7) % 8, LED_ON); //turn the pixel at the new head of the snake on
    if (snake[0].x == appleLocation.x && snake[0].y == appleLocation.y) { //if the head of the snake is at the same location as the apple
      spawnApple();
      snakeLength++; //increase the snakes length by 1
    }
}

void spawnApple() {
  //matrix.drawPixel(appleLocation.x, appleLocation.y, LED_OFF);
  bool validSpawn = false;

  while (!validSpawn) {
    validSpawn = true;

    appleLocation.x = random(0, 8); //choose a random location for the apple
    appleLocation.y = random(0, 8);

    for (int i = 0; i < snakeLength; i++) {                                 //run as many times as there are segmants on the snake
      if (appleLocation.x == snake[i].x && appleLocation.y == snake[i].y) { //if the location that was chosen for the apple if the same as the currant segmant location its checking
        validSpawn = false; //so that the loop will keep going and it will choose a new location for the apple
        break;
      }
    }
  }
  matrix.drawPixel(appleLocation.x, (appleLocation.y + 7) % 8, LED_ON); //turn on the pixel of the location chosen for the apple on
  matrix.writeDisplay();
}

bool hitSomething() {
  bool hitBool = false;
  if (snake[0].x < 0 || snake[0].x > 7) hitBool = true; //check if the snake has hit a wall
  if (snake[0].y < 0 || snake[0].y > 7) hitBool = true;

  for (int i = 1; i < snakeLength; i++) { //run as many times as there are segmants on the snake
    if (snake[0].x == snake[i].x && snake[0].y == snake[i].y) { //if the location of the head of the snake is the same as as the segmant its currantly checking
      hitBool = true;
      break;
    }
  }

  return(hitBool); //if the snake hit something, this will return true
}

void losingAnimation() {
  matrix.drawPixel(appleLocation.x, (appleLocation.y + 7 % 8), LED_OFF); //turn off the pixel at the apples location
  matrix.writeDisplay();
  delay(25);
  for (int i = 0; i < snakeLength; i++) {                          //run as many times as there are segmants on the snake
      matrix.drawPixel(snake[i].x, (snake[i].y + 7) % 8, LED_OFF); //turn off the pixel at the snake segmant its currantly checking
      matrix.writeDisplay();
      delay(25);
  }
  matrix.clear(); //make sure theres nothing left on the screen
  matrix.writeDisplay();
}
