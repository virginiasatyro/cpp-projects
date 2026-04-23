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

    // Weighted Method (Luminance): Apply a formula that respects human perception, such as
    // as used in and MathWorks
    olc::Pixel ToGreyWeightedMethod(unsigned char r, unsigned char g, unsigned char b) const
    {
      const unsigned char gray = static_cast<unsigned char>(0.299f * r + 0.587f * g + 0.114f * b);
      return olc::Pixel(gray, gray, gray);
    }

    // Average Method: Average the red, green, and blue values for each pixel (R + G + B) / 3 to
    // determine the grey intensity.
    olc::Pixel ToGreyAverageMethod(unsigned char r, unsigned char g, unsigned char b) const
    {
      const unsigned char gray = static_cast<unsigned char>((r + g + b) / 3);
      return olc::Pixel(gray, gray, gray);
    }

    // Desaturation Method: Take the average of the maximum and minimum values among the red, green,
    // and blue channels to determine the grey intensity. This method can help preserve some
    // contrast.
    olc::Pixel ToGreyDesaturationMethod(unsigned char r, unsigned char g, unsigned char b) const
    {
      const unsigned char max = std::max({r, g, b});
      const unsigned char min = std::min({r, g, b});
      const unsigned char gray = static_cast<unsigned char>((max + min) / 2);
      return olc::Pixel(gray, gray, gray);
    }

    olc::Pixel ToGreyThresholdMethod(unsigned char r, unsigned char g, unsigned char b) const
    {
      const unsigned char gray = (r + g + b) / 3;
      const unsigned char threshold = 128;  // Adjust this threshold as needed
      const unsigned char bw = (gray >= threshold) ? 255 : 0;
      return olc::Pixel(bw, bw, bw);
    }

    olc::Pixel SampleFrame(int x, int y) const
    {
      if (frameBuffer.empty() || frameWidth == 0 || frameHeight == 0)
      {
        return olc::BLACK;
      }

      const UINT32 sourceX = static_cast<UINT32>((static_cast<unsigned long long>(x) * frameWidth) /
                                                 static_cast<unsigned long long>(ScreenWidth()));
      const UINT32 sourceY =
          static_cast<UINT32>((static_cast<unsigned long long>(y) * frameHeight) /
                              static_cast<unsigned long long>(ScreenHeight()));
      const size_t offset = static_cast<size_t>(sourceY) * static_cast<size_t>(frameStride) +
                            static_cast<size_t>(sourceX) * 4;

      unsigned char b = frameBuffer[offset + 0];
      unsigned char g = frameBuffer[offset + 1];
      unsigned char r = frameBuffer[offset + 2];

      return ToGreyWeightedMethod(r, g, b);
      // return ToGreyAverageMethod(r, g, b);
      // return ToGreyDesaturationMethod(r, g, b);
      // return ToGreyThresholdMethod(r, g, b);
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
          Draw(x, y, SampleFrame(x, y));
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
