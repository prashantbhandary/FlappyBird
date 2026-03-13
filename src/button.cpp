#include "button.h"

static uint8_t gButtonPin = 0;

static bool gLastState = HIGH;

void buttonBegin() {
  pinMode(BUTTON_PIN, INPUT);
  gLastState = digitalRead(BUTTON_PIN);
}

bool buttonPressed() {
  bool current = digitalRead(BUTTON_PIN);
  bool pressed = (gLastState == HIGH && current == LOW); // active-low press
  gLastState = current;
  return pressed;
}