# Webcam to Pixels Black and White

This project captures frames from the default webcam with Windows Media Foundation and renders them in `olcPixelGameEngine` with a pixelated black-and-white look. The image is sampled from the original camera frame and converted to grayscale before each pixel is drawn on screen.

## Grayscale methods

The main focus of this version is the grayscale conversion. The project includes a few different approaches:

- `ToGreyWeightedMethod`: uses the luminance formula `0.299 * R + 0.587 * G + 0.114 * B`, giving more weight to green and less to blue. This usually produces the most natural grayscale result.
- `ToGreyAverageMethod`: computes `(R + G + B) / 3`. It is simple and easy to understand, but it does not account for how the human eye perceives brightness.
- `ToGreyDesaturationMethod`: computes `(max(R, G, B) + min(R, G, B)) / 2`. This keeps part of the contrast by using the range of the RGB channels.
- `ToGreyThresholdMethod`: first computes a gray value and then converts it to either black or white using a fixed threshold. This creates a strong binary black-and-white effect instead of a smooth grayscale image.

In `SampleFrame`, the code reads the RGB values from the webcam buffer, chooses one of these methods, and returns the final pixel color. Right now, the active method is `ToGreyWeightedMethod`.

## Build

Compile with:

```bash
g++ -o main.exe main.cpp -luser32 -lgdi32 -lopengl32 -lgdiplus -lShlwapi -ldwmapi -lmf -lmfplat -lmfreadwrite -lmfuuid -lole32 -loleaut32 -luuid -static -std=c++17
```

## Notes

- Windows only
- Requires a webcam connected and available
- Uses a `320x240` internal resolution with pixel scaling


![Webcam Black and White Image](../images/webcam-capture-pb.png)
