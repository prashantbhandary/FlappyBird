#include "buzzer.h"

static const uint8_t BUZZER_CHANNEL = 0;
static const uint8_t BUZZER_RES_BITS = 8;

static void playTone(int freq, int durationMs) {
  ledcWriteTone(BUZZER_CHANNEL, freq);
  delay(durationMs);
  ledcWriteTone(BUZZER_CHANNEL, 0);
}

void buzzerBegin() {
  ledcSetup(BUZZER_CHANNEL, 2000, BUZZER_RES_BITS);
  ledcAttachPin(BUZZER_PIN, BUZZER_CHANNEL);
  ledcWriteTone(BUZZER_CHANNEL, 0);
}

void buzzerScore() {
  playTone(1800, 25);
}

void buzzerGameOver() {
  playTone(350, 350);
}

void buzzerStop() {
  ledcWriteTone(BUZZER_CHANNEL, 0);
}