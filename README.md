
# Flappy Bird on ESP32 + SSD1306

Simple Flappy Bird style game made with ESP32, Arduino framework, and a 128x64 SSD1306 OLED.
Demo: https://www.instagram.com/reel/DV0zjzuk2FA/?utm_source=ig_web_copy_link&igsh=NTc4MTIwNjQ2YQ==

## What This Project Does

- Shows a start screen
- Starts game when button is pressed
- Bird jumps on button press and falls due to gravity
- Pipes move from right to left with a random gap
- Score increases when the bird passes a pipe
- Game over on collision with pipe or screen edges
- Keeps best score during power-on session
- Uses buzzer for jump and game-over sounds

## Hardware

- Board: ESP32 Dev Module
- Display: SSD1306 128x64 I2C (`0x3C`)
- Button: GPIO 26 (active-low)
- Buzzer: GPIO 25
- I2C SDA: GPIO 21
- I2C SCL: GPIO 22

## Code Structure

- `src/game.cpp`: main game loop (`setup` and `loop`), state transitions
- `src/bird.cpp`: game state, physics, pipe logic, collision, draw screens
- `src/button.cpp`: button init and edge-detect press handling
- `src/buzzer.cpp`: buzzer tone control
- `include/main.h`: display size, I2C pins, global display declaration
- `include/bird.h`: game variables and gameplay function declarations
- `include/button.h`: button API and pin
- `include/buzzer.h`: buzzer API and pin

## Build and Upload (PlatformIO)

1. Build:
	- `platformio run`
2. Upload:
	- `platformio run --target upload`
3. Serial monitor (optional):
	- `platformio device monitor -b 115200`

## How to Play

- Press button to start
- Press button to jump
- Avoid hitting top/bottom and pipes
- After game over, press button to restart

## Notes

- Target frame time is 16 ms (about 60 FPS)
- Pipe gap position is randomized each pipe cycle
