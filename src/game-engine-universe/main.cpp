#define OLC_PGE_APPLICATION
#include "../commonlib/olcPixelGameEngine.h"
#include <string>

// g++ -o main.exe main.cpp -luser32 -lgdi32 -lopengl32 -lgdiplus -lShlwapi -ldwmapi -lstdc++fs -static -std=c++17

// procedural generation - control of randomness
class Universe : public olc::PixelGameEngine
{
public:
    Universe()
    {
        sAppName = "Universe Generator";
    }

private:

protected:
    bool OnUserCreate() override
    {
        // Called once at the start, so create things here
        return true;
    }

    bool OnUserUpdate(float fElapsedTime) override
    {
        // Called once per frame
        // Clear screen
        PixelGameEngine::ConsoleClear();
        Clear(olc::BLACK);

        if(GetKey(olc::SPACE).bReleased)
        {
            auto tp1 = std::chrono::system_clock::now();

            for(int x = 0; x < ScreenWidth(); x++)
            {
                for (int y = 0; y < ScreenHeight(); y++)
                {
                    bool isStar = false;

                    // Standard C++ rand()
                    isStar = (rand() % 256) < 32;

                    Draw(x, y, isStar ? olc::WHITE : olc::BLACK);
                }
            }

            auto tp2 = std::chrono::system_clock::now();
            std::chrono::duration<float> elapsedTime = tp2 - tp1;
            DrawString(3, 3, "Time: "/* + std::to_string(elapsedTime.count())*/, olc::RED, 2);

        }

        return true;
    }
};

int main()
{
    Universe demo;
    if (demo.Construct(512, 480, 2, 2))
    {
        demo.Start();
    }

    return 0;
}