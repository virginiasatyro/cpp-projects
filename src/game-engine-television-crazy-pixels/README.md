# Television Crazy Pixels

A simple yet fun visual effect that mimics the nostalgic look of old television static or "snow" — continuously filling the screen with random colored pixels to create a chaotic, colorful animation.

![Television Crazy Pixels](../../img/television-crazy-pixels.gif)

## Overview

This project demonstrates a basic graphics program using the `olcPixelGameEngine` library. It serves as an excellent starting point for understanding game loops, pixel manipulation, and real-time rendering in C++.

## What You'll Learn

1. **Game Loop Architecture** — Understanding `OnUserCreate()` (initialization) and `OnUserUpdate()` (per-frame updates).
2. **Pixel-Level Graphics** — Direct pixel manipulation using the `Draw()` function.
3. **Random Number Generation** — Using `rand()` to generate pseudo-random values.
4. **Color Representation** — Working with RGB color values (0–255 per channel).
5. **Frame-by-Frame Rendering** — How graphics are updated and displayed every frame.
6. **Performance Basics** — Running nested loops efficiently for screen-sized operations.

## How It Works

The program creates a window and, every frame, iterates through every pixel on the screen (256×240 resolution) and draws it with a random RGB color. This happens repeatedly at ~60 FPS (or based on system performance), creating the effect of flickering, colorful static.

### Algorithm (Pseudocode)

```
OnUserCreate():
    Initialize (nothing to set up in this simple example)

OnUserUpdate():
    For each pixel (x, y) on screen:
        Generate random R (0-255)
        Generate random G (0-255)
        Generate random B (0-255)
        Draw pixel at (x, y) with color (R, G, B)
```

### Visual Result

Every frame, the entire screen is filled with random colors, creating a "TV static" effect. Since this happens continuously at high speed, the eye perceives a flickering, colorful noise pattern.

## Compilation

### Requirements

- **Compiler**: g++ (MSYS2/MinGW-w64 on Windows)
- **C++ Standard**: C++11 or later
- **Library**: olcPixelGameEngine (included in `../../commonlib/olcPixelGameEngine.h`)

### Compile Command

From the project directory (`src/television-crazy-pixels`):

```powershell
cd 'C:\Users\vivi_\github\cpp-projects\src\television-crazy-pixels'
g++ -std=c++17 -O2 -I../../commonlib -o television.exe main.cpp -lgdi32 -lgdiplus -lopengl32 -luser32 -lShlwapi -ldwmapi
.\television.exe
```

## Variations and Experiments

### 1. Adjust Screen Resolution
Modify the `Construct()` call:
```cpp
demo.Construct(512, 480, 2, 2);  // Larger, but slower
demo.Construct(128, 120, 8, 8);  // Smaller, but faster
```

### 2. Add Delay Between Frames
In `OnUserUpdate()`, add a short sleep:
```cpp
#include <chrono>
#include <thread>

std::this_thread::sleep_for(std::chrono::milliseconds(50));  // 50ms delay
```

### 3. Change Color Channels
Limit colors to a specific palette:
```cpp
// Only grayscale (R = G = B)
int gray = rand() % 255;
Draw(x, y, olc::Pixel(gray, gray, gray));

// Only red and green
Draw(x, y, olc::Pixel(rand() % 255, rand() % 255, 0));
```

### 4. Draw Specific Patterns
Instead of random pixels, draw patterns:
```cpp
// Vertical stripes
if (x % 2 == 0)
    Draw(x, y, olc::WHITE);
else
    Draw(x, y, olc::BLACK);

// Checkerboard
if ((x + y) % 2 == 0)
    Draw(x, y, olc::WHITE);
else
    Draw(x, y, olc::BLACK);
```

### 5. Optimize with Seeding
Set a fixed random seed for reproducible results, or use system time for variety:
```cpp
srand(time(nullptr));  // Different each run
// or
srand(42);  // Same pattern each run
```

### 6. Track Time for Animation
Use `fElapsedTime` parameter to create time-based animations:
```cpp
static float time = 0;
time += fElapsedTime;

// Use 'time' to vary colors or patterns based on elapsed seconds
int colorShift = (int)(time * 100) % 255;
Draw(x, y, olc::Pixel(colorShift, rand() % 255, rand() % 255));
```

## Performance Considerations

- **Resolution Impact**: This program draws 256×240 = 61,440 pixels every frame. At 60 FPS, that's ~3.6 million pixel operations per second. Modern computers handle this easily, but larger resolutions will slow it down.
- **Pixel Scale**: Increasing pixel scale (4th parameter in `Construct()`) reduces actual pixels drawn, speeding up the program.
- **Optimization**: Using `O2` flag during compilation makes the program run faster.

## Concepts to Explore

1. **Random Number Generators** — `rand()` is simple but not ideal for cryptography. For games, consider `std::mt19937` (Mersenne Twister).
2. **Color Spaces** — RGB is one way to represent color; HSV, HSL, and other color models offer different creative possibilities.
3. **Rendering Techniques** — This is pixel-perfect rendering; learn about texture mapping, shaders, and other advanced techniques.
4. **Frame Rate Control** — The engine handles FPS; understand `OnUserUpdate(float fElapsedTime)` for frame-rate-independent movement.

## Possible Extensions

- **Add Sound** — Play random beeps or noise when colors change (requires additional audio library).
- **Interactive Control** — Use keyboard/mouse to modify the effect (e.g., press 'R' for only red pixels).
- **Save Screenshot** — Add functionality to save the current frame as an image file.
- **Gradient Animation** — Instead of random pixels, create smooth color gradients that animate over time.
- **Particle System** — Build on this to create a particle effect engine.

## References

- [olcPixelGameEngine GitHub](https://github.com/OneLoneCoder/olcPixelGameEngine) — Official repository and documentation.
- [OneLoneCoder YouTube](https://www.youtube.com/c/javidx9) — Tutorials on graphics programming and game development.

## License

Based on the olcPixelGameEngine by OneLoneCoder (Javidx9). See the original repository for license details.

---

**Author**: Based on olcPixelGameEngine tutorials  
**Purpose**: Educational — learning graphics programming and game loops  
**Difficulty**: Beginner
