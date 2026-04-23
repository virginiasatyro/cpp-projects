# Webcam to Pixels

This project captures frames from the default webcam using Windows Media Foundation and renders them with `olcPixelGameEngine` in a low-resolution, pixelated style. The camera image is sampled and scaled to the engine screen, creating a simple real-time webcam-to-pixel effect.

## Build

Compile with:

```bash
g++ -o main.exe main.cpp -luser32 -lgdi32 -lopengl32 -lgdiplus -lShlwapi -ldwmapi -lmf -lmfplat -lmfreadwrite -lmfuuid -lole32 -loleaut32 -luuid -static -std=c++17
```

## Notes

- Windows only
- Requires a webcam connected and available as the default camera
- Uses a `320x240` internal resolution with pixel scaling for the visual effect
