#include <iostream>

#define OLC_PGE_APPLICATION
#include "../commonlib/olcPixelGameEngine.h"

class PerlinNoise : public olc::PixelGameEngine
{
  public:
    PerlinNoise()
    {
      sAppName = "Perlin Noise 1D";
    }

  protected:
    // 1D noise variables
    float *noiseSeed1D = nullptr;
    float *perlinNoise1D = nullptr;
    int outputSize = 256;

    int octaveCount = 1;
    float scalingBias = 2.0f;

  private:
    void PerlinNoise1D(int count, float *seed, int octaves, float scalingBias, float *output)
    {
      // used 1D Perlin Noise
      for (int x = 0; x < count; x++)
      {
        float noise = 0.0f;
        float scale = 1.0f;
        float scaleAccumulate = 0.0f;

        for (int o = 0; o < octaves; o++)
        {
          int pitch = count >> o;
          int sample1 = (x / pitch) * pitch;
          int sample2 = (sample1 + pitch) % count;

          float blend = (float)(x - sample1) / (float)pitch;
          float sample = (1.0f - blend) * seed[sample1] + blend * seed[sample2];
          noise += sample * scale;
          scaleAccumulate += scale;
          scale /= scalingBias;
        }

        output[x] = noise / scaleAccumulate;
      }
    }

    bool OnUserCreate() override
    {
      outputSize = ScreenWidth();
      noiseSeed1D = new float[outputSize];
      perlinNoise1D = new float[outputSize];

      // fill with random values between 0 and 1
      for (int i = 0; i < outputSize; i++)
      {
        noiseSeed1D[i] = (float)rand() / (float)RAND_MAX;
      }

      return true;
    }

    bool OnUserUpdate(float fElapsedTime) override
    {
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
        for (int i = 0; i < outputSize; i++)
        {
          noiseSeed1D[i] = (float)rand() / (float)RAND_MAX;
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

      // ALGORITHM ----------------------------------------------------------
      PerlinNoise1D(outputSize, noiseSeed1D, octaveCount, scalingBias, perlinNoise1D);

      // DRAW ----------------------------------------------------------------
      for (int x = 0; x < outputSize; x++)
      {
        int y = (perlinNoise1D[x] + 1.0f) / 2.0f * (ScreenHeight() - 1);
        Draw(x, y, olc::WHITE);
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
