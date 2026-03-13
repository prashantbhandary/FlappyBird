#ifndef BIRD_H
#define BIRD_H
#include "main.h"
enum GameState {
  STATE_START,
  STATE_PLAYING,
  STATE_GAME_OVER
};

extern GameState gameState;

extern float birdY;
extern float birdVel;
extern float pipeX;
extern int pipeGapY;
extern float pipeSpeed;
extern bool pipePassed;
extern int score;
extern int bestScore;
extern unsigned long lastFrameMs;

extern const int BIRD_X;
extern const int BIRD_SIZE;
extern const float GRAVITY;
extern const float JUMP_VEL;
extern const int PIPE_WIDTH;
extern const int PIPE_GAP_HEIGHT;
extern const unsigned long FRAME_TIME_MS;

void randomizePipeGap();
extern void resetGame();
void drawBird();
void drawPipe();
void drawScore();
void updateBirdPhysics();
void updatePipe();
bool checkCollision();
void drawStartScreen();
void drawGameOverScreen();




#endif