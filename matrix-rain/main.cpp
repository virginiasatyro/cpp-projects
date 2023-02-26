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
        int column = 0; // colunms are overlapping!
        float position = 0.0f;
        float speed = 10.0f;
        std::string text;
    };

    std::list<Streamer> listStreamers;
    int maxStreamers = 250;

    void prepareStreamer(Streamer *s)
    {
        s->column = (int)(rand() % ScreenWidth());
        s->position = 0.0f;
        s->speed = rand() % 40 + 5;

        s->text.clear(); // clear string before start to append
        int streamLength = rand() % 80 + 10; // 10 - 90
        for(int i = 0; i < streamLength; i++)
        {
            s->text.append(1, randomCharacter());
        }
        // s->text = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    }

    char randomCharacter()
    {
        // ASCII 33 - 93
        return (char)(rand() % 93 + 33); // visiable ASCII character
        //return (char)(rand() % 0x1EF + 0x00C0); // crazy characets - not very good too many spaces - possible problem with my machine
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
            s.position += s.speed * fElapsedTime;

            for (int i = 0; i < s.text.size(); i++)
            {
                auto color = s.speed < 15.0f ? olc::DARK_GREEN : olc::GREEN;
                if(i == 0)
                {
                    color = olc::WHITE;
                }
                else
                {
                    if(i < 3)
                    {
                        color = olc::GREY;
                    }
                }

                int charIndex = (abs(i - (int)s.position)) % s.text.size();
                DrawString(s.column, 8 * ((int)s.position - i), std::string(1, s.text[charIndex]), color); // why 8*??

                if (rand() % 1000 < 5)
                {
					s.text[i] = randomCharacter();
                }
            }

            if (8 * (s.position - s.text.size()) >= ScreenHeight()) // why 8*??
            {
                prepareStreamer(&s);
            }
        }

        return true;
    }
};

int main()
{
    MatrixRain demo;
    // if (demo.Construct(120, 80, 12, 12)) // 12, 12
    // if (demo.Construct(1024, 512, 1, 1))
    if (demo.Construct(1000, 600, 1, 1))
    {
        demo.Start();
    }

    return 0;
}