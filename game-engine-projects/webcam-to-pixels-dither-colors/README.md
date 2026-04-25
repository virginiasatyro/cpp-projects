# Webcam to Pixels Dither Colors

This project captures frames from the default webcam using Windows Media Foundation and renders them with `olcPixelGameEngine` as a low-resolution, color-dithered image. Each screen pixel samples the live camera frame, converts the sampled RGB color into HSV, chooses a foreground/background color pair from the hue range, and uses a small 2x2 dither pattern to represent brightness.

The result is a retro webcam effect where color is simplified into strong bands such as red, yellow, green, cyan, blue, and magenta while the dither pattern preserves some detail from the original image.

## How it works

- Media Foundation opens the default webcam and requests `RGB32` frames.
- Each frame is copied into a `frameBuffer`.
- `SampleFrame()` maps the low-resolution engine coordinates back to the source camera coordinates.
- `RgbToHsv()` converts the sampled pixel from RGB into hue, saturation, and value.
- `ToDitherColors()` picks a color pair based on hue and a fill level based on brightness.
- `ResolveCellPixel()` applies a 2x2 pattern to mix the foreground and background colors.

Low-saturation pixels are treated as grayscale-like pixels and rendered with black and white instead of a hue pair.

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

![Webcam Dither Colors](../images/web-cam-dither-colors.png)
