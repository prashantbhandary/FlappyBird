// buzzer.h
#ifndef BUZZER_H
#define BUZZER_H

#include <Arduino.h>

extern void buzzerBegin();
void buzzerScore();      // short beep
void buzzerGameOver();   // long buzz
void buzzerStop();
#define BUZZER_PIN 25  

#endif