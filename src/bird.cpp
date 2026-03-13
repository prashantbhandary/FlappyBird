#include "bird.h"

GameState gameState = STATE_START; //tracks which screen
const int BIRD_X = 24;             // birds hori pos fix
const int BIRD_SIZE = 4;           // bird is 4x4 px square
float birdY = 28.0f;               // bird's vertical position (float for smooth physics)
float birdVel = 0.0f;               // bird vertical velocity (positive = down, negative = moving up)
const float GRAVITY = 0.22f;        // downward acceleration
const float JUMP_VEL = -2.5f;      // initial velocity when jump (negative = up)

float pipeX = SCREEN_WIDTH;   //current x pos of the pipe (moves left)
const int PIPE_WIDTH = 12;    //width of the pipe
int pipeGapY = 22;            //y pos of the top of the gap (gap is PIPE_GAP_HEIGHT tall) 
const int PIPE_GAP_HEIGHT = 30;   //opening the bird must pass through
float pipeSpeed = 1.0f;    //many pixels (roughly) the pipe moves left per update
bool pipePassed = false;  //Prevents scoring multiple times on same pipe

int score = 0;  //current score
int bestScore = 0;  // best scoree
unsigned long lastFrameMs = 0;    //ttime staop for last frame updaes
const unsigned long FRAME_TIME_MS = 16;  //frame duiration in milis
//16ms ~ 62fps,,close to the 60fps 

void randomizePipeGap() {
  // Keep gap comfortably inside display
  // top pipe at least 6 px, bottom pipe at least 6 px
  int minGapTop = 6;
  int maxGapTop = SCREEN_HEIGHT - PIPE_GAP_HEIGHT - 6;
  pipeGapY = random(minGapTop, maxGapTop + 1);
}

void resetGame() {
  birdY = 28.0f;
  birdVel = 0.0f;
  pipeX = SCREEN_WIDTH;
  pipeSpeed = 1.0f;
  pipePassed = false;
  randomizePipeGap();
  score = 0;
}

void drawBird() {
  display.fillRect(BIRD_X, (int)birdY, BIRD_SIZE, BIRD_SIZE, SSD1306_WHITE);
}

//bujinaa idkk huhu some how it workss 
void drawPipe() {
  // Top pipe
  display.fillRect((int)pipeX, 0, PIPE_WIDTH, pipeGapY, SSD1306_WHITE);

  // Bottom pipe
  int bottomY = pipeGapY + PIPE_GAP_HEIGHT;
  int bottomH = SCREEN_HEIGHT - bottomY;
  display.fillRect((int)pipeX, bottomY, PIPE_WIDTH, bottomH, SSD1306_WHITE);
}

void drawScore() {
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(2, 2);
  display.print("Score:");
  display.print(score);
}

void updateBirdPhysics() {
  birdVel += GRAVITY; // adds downward acceleration to veloccity
  birdY += birdVel;   // moves the bird by its velocity
  //bird vel nef means goes up
  //bird vel postivi means goes down
}

void updatePipe() { 
  pipeX -= pipeSpeed;

  // Score when bird passes the pipe once
  if (!pipePassed && (BIRD_X > (int)pipeX + PIPE_WIDTH)) {
    pipePassed = true;
    score++;
    if (score > bestScore) bestScore = score;

    // Optional small difficulty increase
    // if (score % 5 == 0 && pipeSpeed < 3.8f) {
    //   pipeSpeed += 0.15f;
    // }
  }

  // Recycle pipe after it leaves screen
  if ((int)pipeX + PIPE_WIDTH < 0) {
    pipeX = SCREEN_WIDTH;
    pipePassed = false;
    randomizePipeGap();
  }
}

bool checkCollision() {
  // Top/bottom screen collision
  if (birdY < 0 || (birdY + BIRD_SIZE) > SCREEN_HEIGHT) {
    return true;
  }

  // Pipe collision (AABB)

  int birdLeft = BIRD_X;
  int birdRight = BIRD_X + BIRD_SIZE; //28
  int birdTop = (int)birdY; 
  int birdBottom = (int)birdY + BIRD_SIZE;

  int pipeLeft = (int)pipeX;
  int pipeRight = (int)pipeX + PIPE_WIDTH;

  //checks horizontal overlaps
  bool overlapX = (birdRight > pipeLeft) && (birdLeft < pipeRight);
  if (!overlapX) return false; //no overlaps means no collision

  int gapTop = pipeGapY;
  int gapBottom = pipeGapY + PIPE_GAP_HEIGHT;

  bool hitsTopPipe = (birdTop < gapTop);
  bool hitsBottomPipe = (birdBottom > gapBottom);

  return hitsTopPipe || hitsBottomPipe;
}

void drawStartScreen() {
  // Small start animation: bobbing bird
  int bobY = 30 + (int)(sin(millis() * 0.01f) * 3.0f);

  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(8, 6);
  display.print("Flappy");
  display.setCursor(20, 24);
  display.print("Bird");

  display.fillRect(96, bobY, BIRD_SIZE, BIRD_SIZE, SSD1306_WHITE);

  display.setTextSize(1);
  display.setCursor(10, 52);
  display.print("Press Start Button");
  display.display();
}

void drawGameOverScreen() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  display.setCursor(32, 10);
  display.print("GAME OVER");

  display.setCursor(25, 28);
  display.print("Score: ");
  display.print(score);

  display.setCursor(25, 40);
  display.print("Best : ");
  display.print(bestScore);

  display.setCursor(6, 54);
  display.print("Press Start Button");
  display.display();
}
