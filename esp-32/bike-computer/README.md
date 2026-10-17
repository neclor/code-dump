# ESP32 Bike Computer

Bike computer for the ideaspark ESP32 board with a built-in 0.96" OLED.

## Features

- Current and average speed
- Ride distance and time
- Total distance, saved to flash
- Auto-pause and manual pause
- Configurable wheel size

## Hardware

- ideaspark ESP32 with SSD1306 128×64 OLED
- 3.3 V analog Hall sensor and a magnet on a spoke
- Push button

| Part | Pin |
|---|---|
| Display SDA / SCL | GPIO 21 / 22 |
| Hall sensor OUT | GPIO 32 |
| Button | GPIO 19 → GND |

## Controls

| Press | Action |
|---|---|
| Short | Pause / resume |
| Long (≥ 1 s) | Reset ride |

## Build

Requires the Espressif **esp32** Arduino core and the **U8g2** library.

Open `main/main.ino` in Arduino IDE, select **ESP32 Dev Module**, and upload.

Keep the magnet away from the sensor at power-on.

## Structure

```
main/
├── main.ino
└── src/
    ├── bike_computer/    controller and flash persistence
    ├── core/             ride data and wheel sizes
    ├── modules/          button, display, storage, wheel sensor
    └── utils/            unit conversion
```

Each hardware module is an interface (`.h`) with one implementation (`.cpp`), so it can be replaced without touching the rest of the code.

## Configuration

Settings are `#define`s at the top of the related source file: pins, long press time, auto-pause, sensor thresholds, default wheel, odometer save step.
