#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// ── OLED configuration ──────────────────────────────────────
#define SCREEN_WIDTH    128
#define SCREEN_HEIGHT   64
#define OLED_RESET      -1
#define SCREEN_ADDRESS  0x3C

// ── Display object ──────────────────────────────────────────
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// ── State machine ───────────────────────────────────────────
enum State {
  STATE_PLAYING,    // Lyrics sequence playing on display
  STATE_DONE        // Finished, wait, then replay
};
State currentState = STATE_PLAYING;

unsigned long stateStartMs  = 0;
unsigned long playStartMs   = 0;  // when lyric playback began
int lastShownCue            = -1; // track which lyric cue was last drawn

// =============================================================
//  LYRIC TIMELINE
//  Adjust these ms offsets to match your actual audio timing.
//  Each cue: {millisecond offset, line1, line2, textSize}
// =============================================================
struct LyricCue {
  unsigned long ms;     // offset from audio start
  const char*   line1;  // top line (or only line)
  const char*   line2;  // bottom line (nullptr if none)
  uint8_t       size;   // text size (1 = small, 2 = large)
};

static const LyricCue timeline[] = {
  //  ms      line1                line2              size
  {     0,  "There is",          "a saying...",        1 },
  {  2500,  "Yesterday",         "is history...",      1 },
  {  5500,  "Tomorrow",          "is a mystery...",    1 },
  {  9000,  "But today",         "is a gift.",         1 },
  { 12500,  "That's why",        "it's called...",     1 },
  { 15500,  "the",               "present.",           2 },
  { 18500,  "~ The Present ~",   nullptr,              1 },
};

static const int timelineCount = sizeof(timeline) / sizeof(timeline[0]);

void drawCentered(const char* text, int y, int sz) {
  display.setTextSize(sz);
  display.setTextColor(SSD1306_WHITE);
  int16_t x1, y1;
  uint16_t tw, th;
  display.getTextBounds(text, 0, y, &x1, &y1, &tw, &th);
  int cx = (SCREEN_WIDTH - (int)tw) / 2;
  if (cx < 0) cx = 0;
  display.setCursor(cx, y);
  display.print(text);
}

// Show a lyric cue on screen with animation
void showLyricCue(const LyricCue &cue) {
  display.clearDisplay();

  // Thin decorative border
  display.drawRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SSD1306_WHITE);

  if (cue.size == 2) {
    // Large text — dramatic reveal
    if (cue.line1) drawCentered(cue.line1, 8, 1);
    if (cue.line2) {
      // Big word centered
      drawCentered(cue.line2, 28, 2);
    }
  } else {
    // Normal size — two lines centered vertically
    if (cue.line1 && cue.line2) {
      drawCentered(cue.line1, 18, 1);
      drawCentered(cue.line2, 34, 1);
    } else if (cue.line1) {
      drawCentered(cue.line1, 26, 1);
    }
  }

  // Progress bar at bottom
  unsigned long elapsed = millis() - playStartMs;
  int barWidth = (int)((elapsed * (SCREEN_WIDTH - 8)) / 21000UL);
  if (barWidth > SCREEN_WIDTH - 8) barWidth = SCREEN_WIDTH - 8;
  display.fillRect(4, SCREEN_HEIGHT - 6, barWidth, 3, SSD1306_WHITE);
  display.display();
}

void confettiAnimation() {
  unsigned long startTime = millis();
  unsigned long duration  = 3000;

  while (millis() - startTime < duration) {
    display.clearDisplay();

    // Keep "present." centered
    drawCentered("present.", 8, 2);

    display.display();
    delay(30);
  }
}


void setup() {
  Serial.begin(115200);
  Serial.println(F("Present Moment Machine - Starting..."));

  // I2C for OLED
  Wire.begin(21, 22);

  // Initialize OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed!"));
    for (;;) {
      delay(200);
    }
  }

  display.clearDisplay();
  display.display();
  delay(200);

  currentState = STATE_PLAYING;
  stateStartMs = millis();
  playStartMs = millis();
  lastShownCue = -1;

  Serial.println(F("Autoplay started."));
}


void loop() {
  switch (currentState) {

    // ── PLAYING: sync lyrics to display ──────────────────────
    case STATE_PLAYING: {
      // Check which lyric cue to show based on elapsed time
      unsigned long elapsed = millis() - playStartMs;

      // Find the latest cue that should be showing
      int cueIdx = -1;
      for (int i = 0; i < timelineCount; i++) {
        if (elapsed >= timeline[i].ms) {
          cueIdx = i;
        }
      }

      // If we have a new cue to show, display it
      if (cueIdx >= 0 && cueIdx != lastShownCue) {
        lastShownCue = cueIdx;
        const LyricCue &cue = timeline[cueIdx];

        showLyricCue(cue);

        Serial.printf("Cue %d @ %lums: %s\n", cueIdx, elapsed,
                       cue.line1 ? cue.line1 : "");
      }

      // Check if we've shown the last cue and timeline has completed
      bool allCuesShown = (lastShownCue >= timelineCount - 1);

      if (allCuesShown && elapsed > 20000) {
        // Run confetti celebration
        confettiAnimation();

        // Transition to done
        currentState = STATE_DONE;
        stateStartMs = millis();

        // Show final screen
        display.clearDisplay();
        display.drawRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SSD1306_WHITE);
        drawCentered("Today is", 8, 1);
        drawCentered("a gift.", 20, 2);
        drawCentered("That's why it's", 42, 1);
        drawCentered("called the present.", 54, 1);
        display.display();

        Serial.println(F("Done. Restarting soon..."));
      }
      break;
    }

    // ── DONE: wait, then replay ──────────────────────────────
    case STATE_DONE: {
      if (millis() - stateStartMs > 1500) {
        Serial.println(F("Restarting..."));
        currentState = STATE_PLAYING;
        stateStartMs = millis();
        playStartMs = millis();
        lastShownCue = -1;
      }
      break;
    }
  }
}