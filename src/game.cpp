#include "button.h"
#include "buzzer.h"
#include "main.h"
#include "bird.h"

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
    Serial.begin(115200);
    buttonBegin();
    buzzerBegin();
    Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);

    if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
        for (;;) { delay(100); }
        //display begin can failed if so halt...
    }

    display.clearDisplay();
    display.display();

    randomSeed((uint32_t)micros());
    resetGame();
    gameState = STATE_START;
}

void loop() {
  // Frame limiter for smooth, stable animation
  unsigned long now = millis();
  if (now - lastFrameMs < FRAME_TIME_MS) return;
  lastFrameMs = now;

  switch (gameState) {
    case STATE_START: {
      drawStartScreen();
      if (buttonPressed()) {
        resetGame();
        gameState = STATE_PLAYING;
      }
      break;
    }

    case STATE_PLAYING: {
      if (buttonPressed()) {
        birdVel = JUMP_VEL;
        buzzerScore(); // short jump buzz
      }

      updateBirdPhysics();
      updatePipe();

      if (checkCollision()) {
        gameState = STATE_GAME_OVER;
        buzzerGameOver(); // long game-over buzz
      }

      display.clearDisplay();
      drawPipe();
      drawBird();
      drawScore();
      display.display();
      break;
    }

    case STATE_GAME_OVER: {
      drawGameOverScreen();
      if (buttonPressed()) {
        resetGame();
        gameState = STATE_PLAYING;
      }
      break;
    }
  }
}