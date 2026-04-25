#include <mfapi.h>
#include <mferror.h>
#include <mfidl.h>
#include <mfreadwrite.h>
#include <windows.h>

#include <algorithm>
#include <string>
#include <vector>

#define OLC_PGE_APPLICATION
#include "../commonlib/olcPixelGameEngine.h"

// g++ -o main.exe main.cpp -luser32 -lgdi32 -lopengl32 -lgdiplus -lShlwapi -ldwmapi -lmf -lmfplat
// -lmfreadwrite -lmfuuid -lole32 -loleaut32 -luuid -static -std=c++17

template <typename T>
static void SafeRelease(T *&value)
{
  if (value != nullptr)
  {
    value->Release();
    value = nullptr;
  }
}

class Video : public olc::PixelGameEngine
{
  public:
    Video()
    {
      sAppName = "Testing Video";
    }

  private:
    IMFMediaSource *mediaSource = nullptr;
    IMFSourceReader *sourceReader = nullptr;
    UINT32 frameWidth = 0;
    UINT32 frameHeight = 0;
    LONG frameStride = 0;
    bool mediaFoundationStarted = false;
    bool comStarted = false;
    std::vector<unsigned char> frameBuffer;

    struct CellStyle
    {
        olc::Pixel bg = olc::BLACK;
        olc::Pixel fg = olc::WHITE;
        uint8_t fillLevel = 0;
    };

    struct HsvColor
    {
        float h = 0.0f;
        float s = 0.0f;
        float v = 0.0f;
    };

    bool FailStartup(const std::string &message)
    {
      MessageBoxA(nullptr, message.c_str(), "Video Startup Error", MB_OK | MB_ICONERROR);
      return false;
    }

    bool InitializePlatform()
    {
      HRESULT hr = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
      if (SUCCEEDED(hr))
      {
        comStarted = true;
      }
      else if (hr != RPC_E_CHANGED_MODE)
      {
        return FailStartup("Could not initialize COM.");
      }

      hr = MFStartup(MF_VERSION);
      if (FAILED(hr))
      {
        return FailStartup("Could not start Media Foundation.");
      }

      mediaFoundationStarted = true;
      return true;
    }

    bool OpenDefaultCamera()
    {
      IMFAttributes *attributes = nullptr;
      IMFActivate **devices = nullptr;
      UINT32 deviceCount = 0;

      HRESULT hr = MFCreateAttributes(&attributes, 1);
      if (FAILED(hr))
      {
        return FailStartup("Could not create camera enumeration attributes.");
      }

      hr = attributes->SetGUID(MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE,
                               MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE_VIDCAP_GUID);
      if (FAILED(hr))
      {
        SafeRelease(attributes);
        return FailStartup("Could not configure camera enumeration.");
      }

      hr = MFEnumDeviceSources(attributes, &devices, &deviceCount);
      SafeRelease(attributes);
      if (FAILED(hr) || deviceCount == 0)
      {
        CoTaskMemFree(devices);
        return FailStartup("Media Foundation did not find any webcam devices.");
      }

      hr = devices[0]->ActivateObject(IID_PPV_ARGS(&mediaSource));
      for (UINT32 index = 0; index < deviceCount; index++)
      {
        SafeRelease(devices[index]);
      }
      CoTaskMemFree(devices);

      if (FAILED(hr) || mediaSource == nullptr)
      {
        return FailStartup("Could not open the default webcam.");
      }

      IMFAttributes *readerAttributes = nullptr;
      hr = MFCreateAttributes(&readerAttributes, 2);
      if (FAILED(hr))
      {
        return FailStartup("Could not create source reader attributes.");
      }

      readerAttributes->SetUINT32(MF_SOURCE_READER_ENABLE_VIDEO_PROCESSING, TRUE);
      readerAttributes->SetUINT32(MF_READWRITE_DISABLE_CONVERTERS, FALSE);

      hr = MFCreateSourceReaderFromMediaSource(mediaSource, readerAttributes, &sourceReader);
      SafeRelease(readerAttributes);
      if (FAILED(hr) || sourceReader == nullptr)
      {
        return FailStartup("Could not create a webcam source reader.");
      }

      return true;
    }

    bool ConfigureReader()
    {
      IMFMediaType *requestedType = nullptr;
      IMFMediaType *actualType = nullptr;

      HRESULT hr = sourceReader->SetStreamSelection(MF_SOURCE_READER_ALL_STREAMS, FALSE);
      if (FAILED(hr))
      {
        return FailStartup("Could not configure Media Foundation stream selection.");
      }

      hr = sourceReader->SetStreamSelection(MF_SOURCE_READER_FIRST_VIDEO_STREAM, TRUE);
      if (FAILED(hr))
      {
        return FailStartup("Could not enable the webcam video stream.");
      }

      hr = MFCreateMediaType(&requestedType);
      if (FAILED(hr))
      {
        return FailStartup("Could not create a camera media type.");
      }

      requestedType->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Video);
      requestedType->SetGUID(MF_MT_SUBTYPE, MFVideoFormat_RGB32);

      hr = sourceReader->SetCurrentMediaType(MF_SOURCE_READER_FIRST_VIDEO_STREAM, nullptr,
                                             requestedType);
      SafeRelease(requestedType);
      if (FAILED(hr))
      {
        return FailStartup("Could not request RGB32 webcam frames from Media Foundation.");
      }

      hr = sourceReader->GetCurrentMediaType(MF_SOURCE_READER_FIRST_VIDEO_STREAM, &actualType);
      if (FAILED(hr) || actualType == nullptr)
      {
        return FailStartup("Could not read back the webcam media type.");
      }

      hr = MFGetAttributeSize(actualType, MF_MT_FRAME_SIZE, &frameWidth, &frameHeight);
      if (FAILED(hr) || frameWidth == 0 || frameHeight == 0)
      {
        SafeRelease(actualType);
        return FailStartup("The webcam reported an invalid frame size.");
      }

      // Log the frame size for debugging/adjusting quality
      // std::cout << "Camera frame size: " << frameWidth << "x" << frameHeight << std::endl;

      GUID subtype = GUID{};
      hr = actualType->GetGUID(MF_MT_SUBTYPE, &subtype);
      SafeRelease(actualType);
      if (FAILED(hr) || subtype != MFVideoFormat_RGB32)
      {
        return FailStartup("The webcam could not be converted to RGB32 frames.");
      }

      frameStride = static_cast<LONG>(frameWidth * 4);
      frameBuffer.assign(static_cast<size_t>(frameStride) * static_cast<size_t>(frameHeight), 0);
      return true;
    }

    bool ReadFrame()
    {
      for (int attempt = 0; attempt < 60; attempt++)
      {
        DWORD streamIndex = 0;
        DWORD flags = 0;
        LONGLONG timestamp = 0;
        IMFSample *sample = nullptr;

        HRESULT hr = sourceReader->ReadSample(MF_SOURCE_READER_FIRST_VIDEO_STREAM, 0, &streamIndex,
                                              &flags, &timestamp, &sample);
        if (FAILED(hr))
        {
          SafeRelease(sample);
          return false;
        }

        if ((flags & MF_SOURCE_READERF_ENDOFSTREAM) != 0)
        {
          SafeRelease(sample);
          return false;
        }

        if ((flags & MF_SOURCE_READERF_STREAMTICK) != 0 || sample == nullptr)
        {
          SafeRelease(sample);
          Sleep(15);
          continue;
        }

        IMFMediaBuffer *buffer = nullptr;
        hr = sample->ConvertToContiguousBuffer(&buffer);
        SafeRelease(sample);
        if (FAILED(hr) || buffer == nullptr)
        {
          SafeRelease(buffer);
          Sleep(15);
          continue;
        }

        BYTE *rawData = nullptr;
        DWORD maxLength = 0;
        DWORD currentLength = 0;
        hr = buffer->Lock(&rawData, &maxLength, &currentLength);
        if (SUCCEEDED(hr) && rawData != nullptr && currentLength >= frameBuffer.size())
        {
          std::copy(rawData, rawData + frameBuffer.size(), frameBuffer.begin());
          buffer->Unlock();
          SafeRelease(buffer);
          return true;
        }

        if (rawData != nullptr)
        {
          buffer->Unlock();
        }

        SafeRelease(buffer);
        Sleep(15);
      }

      return false;
    }

    void ShutdownCamera()
    {
      SafeRelease(sourceReader);

      if (mediaSource != nullptr)
      {
        mediaSource->Shutdown();
      }
      SafeRelease(mediaSource);

      frameBuffer.clear();
      frameWidth = 0;
      frameHeight = 0;
      frameStride = 0;

      if (mediaFoundationStarted)
      {
        MFShutdown();
        mediaFoundationStarted = false;
      }

      if (comStarted)
      {
        CoUninitialize();
        comStarted = false;
      }
    }

    HsvColor RgbToHsv(unsigned char r, unsigned char g, unsigned char b) const
    {
      const float red = static_cast<float>(r) / 255.0f;
      const float green = static_cast<float>(g) / 255.0f;
      const float blue = static_cast<float>(b) / 255.0f;

      const float maxChannel = std::max({red, green, blue});
      const float minChannel = std::min({red, green, blue});
      const float delta = maxChannel - minChannel;

      HsvColor hsv{};
      hsv.v = maxChannel;
      hsv.s = (maxChannel == 0.0f) ? 0.0f : delta / maxChannel;

      if (delta == 0.0f)
      {
        return hsv;
      }

      if (maxChannel == red)
      {
        hsv.h = 60.0f * ((green - blue) / delta);
        if (hsv.h < 0.0f)
        {
          hsv.h += 360.0f;
        }
      }
      else if (maxChannel == green)
      {
        hsv.h = 60.0f * (((blue - red) / delta) + 2.0f);
      }
      else
      {
        hsv.h = 60.0f * (((red - green) / delta) + 4.0f);
      }

      return hsv;
    }

    CellStyle ToDitherColors(unsigned char r, unsigned char g, unsigned char b) const
    {
      const HsvColor hsv = RgbToHsv(r, g, b);
      const int hueBand = static_cast<int>(hsv.h / 60.0f) % 6;
      const uint8_t fillLevel =
          static_cast<uint8_t>(std::clamp(static_cast<int>(hsv.v * 4.0f), 0, 4));
      CellStyle style{};

      switch (hueBand)
      {
          // clang-format off
        case 0: style.bg = olc::YELLOW; style.fg = olc::RED; break;
        case 1: style.bg = olc::GREEN; style.fg = olc::YELLOW; break;
        case 2: style.bg = olc::CYAN; style.fg = olc::GREEN; break;
        case 3: style.bg = olc::BLUE; style.fg = olc::CYAN; break;
        case 4: style.bg = olc::MAGENTA; style.fg = olc::BLUE; break;
        case 5: style.bg = olc::RED; style.fg = olc::MAGENTA; break;
        default: style.bg = olc::BLACK; style.fg = olc::WHITE; break;
          // clang-format on
      }

      if (hsv.s < 0.12f)
      {
        style.bg = olc::BLACK;
        style.fg = olc::WHITE;
      }

      style.fillLevel = fillLevel;
      return style;
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

      // Simple 2x2 dither pattern to simulate foreground over background.
      const int patternIndex = ((y & 1) << 1) | (x & 1);
      return (patternIndex < style.fillLevel) ? style.fg : style.bg;
    }

    CellStyle SampleFrame(int x, int y) const
    {
      if (frameBuffer.empty() || frameWidth == 0 || frameHeight == 0)
      {
        return {};
      }

      const UINT32 sourceX = static_cast<UINT32>((static_cast<unsigned long long>(x) * frameWidth) /
                                                 static_cast<unsigned long long>(ScreenWidth()));
      const UINT32 sourceY =
          static_cast<UINT32>((static_cast<unsigned long long>(y) * frameHeight) /
                              static_cast<unsigned long long>(ScreenHeight()));
      const size_t offset = static_cast<size_t>(sourceY) * static_cast<size_t>(frameStride) +
                            static_cast<size_t>(sourceX) * 4;

      const unsigned char b = frameBuffer[offset + 0];
      const unsigned char g = frameBuffer[offset + 1];
      const unsigned char r = frameBuffer[offset + 2];

      return ToDitherColors(r, g, b);
    }

  protected:
    bool OnUserCreate() override
    {
      if (!InitializePlatform())
      {
        return false;
      }

      if (!OpenDefaultCamera())
      {
        ShutdownCamera();
        return false;
      }

      if (!ConfigureReader())
      {
        ShutdownCamera();
        return false;
      }

      if (!ReadFrame())
      {
        ShutdownCamera();
        return FailStartup("Could not read the first webcam frame.");
      }

      return true;
    }

    bool OnUserDestroy() override
    {
      ShutdownCamera();
      return true;
    }

    bool OnUserUpdate(float fElapsedTime) override
    {
      (void)fElapsedTime;

      ReadFrame();
      Clear(olc::BLACK);

      for (int x = 0; x < ScreenWidth(); x++)
      {
        for (int y = 0; y < ScreenHeight(); y++)
        {
          const CellStyle style = SampleFrame(x, y);
          Draw(x, y, ResolveCellPixel(style, x, y));
        }
      }

      return true;
    }
};

int main()
{
  Video demo;

  if (demo.Construct(320, 240, 4, 4))
  {
    demo.Start();
  }

  return 0;
}
