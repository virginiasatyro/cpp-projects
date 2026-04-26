#include <algorithm>
#include <cstdlib>
#include <iostream>

#define OLC_PGE_APPLICATION
#include "../commonlib/olcPixelGameEngine.h"

constexpr int MODE_BW = 0;
constexpr int MODE_COLOR = 1;

class PerlinNoise : public olc::PixelGameEngine
{
  public:
    PerlinNoise()
    {
      sAppName = "Perlin Noise 2D";
    }

  protected:
    // 2D noise variables
    int outputWidth = 256;
    int outputHeight = 256;
    float *noiseSeed2D = nullptr;
    float *perlinNoise2D = nullptr;

    int octaveCount = 1;
    float scalingBias = 2.0f;

    int mode = MODE_COLOR;

    struct CellStyle
    {
        olc::Pixel bg = olc::BLACK;
        olc::Pixel fg = olc::WHITE;
        uint8_t fillLevel = 0;
    };

  private:
    void PerlinNoise2D(int width, int height, float *seed, int octaves, float scalingBias,
                       float *output)
    {
      for (int y = 0; y < height; y++)
      {
        for (int x = 0; x < width; x++)
        {
          float noise = 0.0f;
          float scale = 1.0f;
          float scaleAccumulate = 0.0f;

          for (int o = 0; o < octaves; o++)
          {
            int pitch = width >> o;
            if (pitch == 0)
            {
              break;
            }

            int sampleX1 = (x / pitch) * pitch;
            int sampleY1 = (y / pitch) * pitch;

            int sampleX2 = (sampleX1 + pitch) % width;
            int sampleY2 = (sampleY1 + pitch) % height;

            float blendX = (float)(x - sampleX1) / (float)pitch;
            float blendY = (float)(y - sampleY1) / (float)pitch;

            // blend top
            float sampleT = (1.0f - blendX) * seed[sampleY1 * width + sampleX1] +
                            blendX * seed[sampleY1 * width + sampleX2];
            // blend bottom
            float sampleB = (1.0f - blendX) * seed[sampleY2 * width + sampleX1] +
                            blendX * seed[sampleY2 * width + sampleX2];
            // final blend
            noise += ((1.0f - blendY) * sampleT + blendY * sampleB) * scale;
            scaleAccumulate += scale;
            scale /= scalingBias;
          }

          output[y * width + x] = noise / scaleAccumulate;
        }
      }
    }

    olc::Pixel ResolveCellPixel(const CellStyle &style, int x, int y) const
    {
      if (style.fillLevel == 0)
      {
        return style.bg;
      }

      if (style.fillLevel >= 4)
      {
        return style.fg;
      }

      const int patternIndex = ((y & 1) << 1) | (x & 1);
      return (patternIndex < style.fillLevel) ? style.fg : style.bg;
    }

    olc::Pixel LerpPixel(const olc::Pixel &a, const olc::Pixel &b, float t) const
    {
      t = std::clamp(t, 0.0f, 1.0f);
      const auto lerp = [t](uint8_t start, uint8_t end)
      {
        return static_cast<uint8_t>(static_cast<float>(start) +
                                    (static_cast<float>(end) - static_cast<float>(start)) * t);
      };

      return olc::Pixel(lerp(a.r, b.r), lerp(a.g, b.g), lerp(a.b, b.b));
    }

    olc::Pixel HeatMapColor(float value) const
    {
      value = std::clamp(value, 0.0f, 1.0f);
      // clang-format off
      if (value < 0.20f) return LerpPixel(olc::Pixel(0, 0, 32), olc::BLUE, value / 0.20f);
      if (value < 0.40f) return LerpPixel(olc::BLUE, olc::CYAN, (value - 0.20f) / 0.20f);
      if (value < 0.60f) return LerpPixel(olc::CYAN, olc::GREEN, (value - 0.40f) / 0.20f);
      if (value < 0.78f) return LerpPixel(olc::GREEN, olc::YELLOW, (value - 0.60f) / 0.18f);
      if (value < 0.92f) return LerpPixel(olc::YELLOW, olc::RED, (value - 0.78f) / 0.14f);
      // clang-format on

      return LerpPixel(olc::RED, olc::WHITE, (value - 0.92f) / 0.08f);
    }

    bool OnUserCreate() override
    {
      outputWidth = ScreenWidth();
      outputHeight = ScreenHeight();

      noiseSeed2D = new float[outputWidth * outputHeight];
      perlinNoise2D = new float[outputWidth * outputHeight];
      for (int i = 0; i < outputWidth * outputHeight; i++)
      {
        noiseSeed2D[i] = (float)rand() / (float)RAND_MAX;
        perlinNoise2D[i] = 0.0f;
      }

      return true;
    }

    bool OnUserUpdate(float fElapsedTime) override
    {
      (void)fElapsedTime;

      // called once per frame
      // clear screen
      Clear(olc::BLACK);

      // KEYS ---------------------------------------------------------------
      if (GetKey(olc::SPACE).bReleased)
      {
        octaveCount++;
      }

      if (octaveCount > 8)
      {
        octaveCount = 1;
      }

      // re-seed
      if (GetKey(olc::S).bReleased)
      {
        for (int i = 0; i < outputWidth * outputHeight; i++)
        {
          noiseSeed2D[i] = (float)rand() / (float)RAND_MAX;
        }
      }

      if (GetKey(olc::UP).bReleased)
      {
        scalingBias += 0.2f;
      }

      if (GetKey(olc::DOWN).bReleased)
      {
        scalingBias -= 0.2f;
      }

      if (scalingBias < 0.2f)
      {
        scalingBias = 0.2f;
      }

      if (GetKey(olc::C).bReleased)
      {
        // toggle color mode
        mode = !mode;
      }

      // ALGORITHM ----------------------------------------------------------
      PerlinNoise2D(outputWidth, outputHeight, noiseSeed2D, octaveCount, scalingBias,
                    perlinNoise2D);

      // DRAW ----------------------------------------------------------------
      for (int x = 0; x < outputWidth; x++)
      {
        for (int y = 0; y < outputHeight; y++)
        {
          const int pixel_bw =
              std::clamp(static_cast<int>(perlinNoise2D[x + y * outputWidth] * 12.0f), 0, 12);
          CellStyle style{};

          if (mode == MODE_BW)
          {
            switch (pixel_bw)
            {
                // clang-format off
                case 0: style.bg = olc::BLACK; style.fg = olc::BLACK; style.fillLevel = 4; break;
                case 1: style.bg = olc::BLACK; style.fg = olc::DARK_GREY; style.fillLevel = 1; break;
                case 2: style.bg = olc::BLACK; style.fg = olc::DARK_GREY; style.fillLevel = 2; break;
                case 3: style.bg = olc::BLACK; style.fg = olc::DARK_GREY; style.fillLevel = 3; break;
                case 4: style.bg = olc::BLACK; style.fg = olc::DARK_GREY; style.fillLevel = 4; break;

                case 5: style.bg = olc::DARK_GREY; style.fg = olc::GREY; style.fillLevel = 1; break;
                case 6: style.bg = olc::DARK_GREY; style.fg = olc::GREY; style.fillLevel = 2; break;
                case 7: style.bg = olc::DARK_GREY; style.fg = olc::GREY; style.fillLevel = 3; break;
                case 8: style.bg = olc::DARK_GREY; style.fg = olc::GREY; style.fillLevel = 4; break;

                case 9:  style.bg = olc::GREY; style.fg = olc::WHITE; style.fillLevel = 1; break;
                case 10: style.bg = olc::GREY; style.fg = olc::WHITE; style.fillLevel = 2; break;
                case 11: style.bg = olc::GREY; style.fg = olc::WHITE; style.fillLevel = 3; break;
                case 12:
                default: style.bg = olc::GREY; style.fg = olc::WHITE; style.fillLevel = 4; break;
                // clang-format on
            }
          }
          else
          {
            Draw(x, y, HeatMapColor(perlinNoise2D[x + y * outputWidth]));
            continue;
          }

          Draw(x, y, ResolveCellPixel(style, x, y));
        }
      }

      return true;
    }
};

int main()
{
  PerlinNoise demo;

  if (demo.Construct(256, 256, 3, 3))
  {
    demo.Start();
  }

  return 0;
}
