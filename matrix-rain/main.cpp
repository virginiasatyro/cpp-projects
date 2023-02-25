#define OLC_PGE_APPLICATION
#include "../commonlib/olcPixelGameEngine.h"

// g++ -o main.exe main.cpp -luser32 -lgdi32 -lopengl32 -lgdiplus -lShlwapi -ldwmapi -lstdc++fs -static -std=c++17

class MatrixRain : public olc::PixelGameEngine
{
public:
    MatrixRain()
    {
        sAppName = "Matrix Rain";
    }

private:
    struct Streamer
    {
        int column = 0;
        float position = 0.0f;
        std::string text;
    };

    std::list<Streamer> listStreamers;
    int maxStreamers = 1;

    void prepareStreamer(Streamer *s)
    {
        s->text = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    }

protected:
    bool OnUserCreate() override
    {
        // Called once at the start, so create things here
        for (int n = 0; n < maxStreamers; n++)
        {
            Streamer s;
            prepareStreamer(&s);
            listStreamers.push_back(s);
        }

        return true;
    }

    bool OnUserUpdate(float fElapsedTime) override
    {
        // Called once per frame
        // clear screen
        PixelGameEngine::ConsoleClear();
        Clear(olc::BLACK);

        // Draw characters
        for (auto &s : listStreamers)
        {
            s.position += 10.0f * fElapsedTime;
            for (int i = 0; i < s.text.size(); i++)
            {
                int charIndex = (i - (int)s.position) % s.text.size();
                DrawString(s.column, 8 * ((int)s.position - i), std::string(1, s.text[charIndex]), olc::GREEN);
            }
        }

        return true;
    }
};

int main()
{
    MatrixRain demo;
    // if (demo.Construct(120, 80, 12, 12)) // 12, 12
    if (demo.Construct(1024, 512, 1, 1))
    {
        demo.Start();
    }

    return 0;
}