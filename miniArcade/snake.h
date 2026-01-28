#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_LEDBackpack.h>
#include <Arduino.h>
#ifndef BITMAPS_H
#define BITMAPS_H

extern Adafruit_8x8matrix matrix;

void startSnake();
void DrawBitMap();
void moveSnake();
void spawnApple();
void losingAnimation();

bool hitSomething();

struct Point {
  int x;
  int y;
};

//extern const uint8_t snakeBitmap[] PROGMEM;

#endif