// button.h
#ifndef BUTTON_H
#define BUTTON_H

#include <Arduino.h>

void buttonBegin();
bool buttonPressed();  // true only on new press (edge detect)
#define BUTTON_PIN 26   // Game start button
#endif