# Webcam to Pixels Dither

This project captures frames from the default webcam using Windows Media Foundation and renders them with `olcPixelGameEngine` as a low-resolution grayscale dither image. The webcam frame is sampled pixel-by-pixel, converted to luminance, then drawn with a small 2x2 dither pattern to simulate intermediate shades.

The result is a retro black, dark gray, gray, and white webcam effect that keeps more visual detail than a simple threshold-based black-and-white conversion.

## How it works

- Media Foundation opens the default webcam and requests `RGB32` frames.
- Each camera frame is copied into a `frameBuffer`.
- `SampleFrame()` maps each engine pixel back to a source webcam coordinate.
- `ToGrey()` calculates luminance with `0.299 * R + 0.587 * G + 0.114 * B`.
- The luminance value is split into shade bands using black, dark gray, gray, and white.
- `ResolveCellPixel()` applies a 2x2 dither pattern to mix each foreground/background shade pair.

The dither fill level controls how many pixels in the 2x2 pattern use the foreground shade. Lower brightness values draw mostly background color, while higher values draw more foreground color.

## Build

Compile with:

```bash
g++ -o main.exe main.cpp -luser32 -lgdi32 -lopengl32 -lgdiplus -lShlwapi -ldwmapi -lmf -lmfplat -lmfreadwrite -lmfuuid -lole32 -loleaut32 -luuid -static -std=c++17
```

## Run

```bash
./main.exe
```

On Windows PowerShell, you can also run:

```powershell
.\main.exe
```

## Notes

- Windows only
- Requires a webcam connected and available as the default camera
- Uses Windows Media Foundation for webcam capture
- Uses a `320x240` internal resolution with `4x4` pixel scaling
- The app opens the first webcam device found by Media Foundation

![Webcam Dither](../images/web-cam-dither.png)
