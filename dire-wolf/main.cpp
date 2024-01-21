/*
 
*/

#include <iostream>

#define OLC_PGE_APPLICATION
#include "../commonlib/olcPixelGameEngine.h"

class DireWolf : public olc::PixelGameEngine
{
public:
    DireWolf()
    {
        sAppName = "DireWolf";
    }

private:
    olc::Sprite *sprWolf = nullptr;
    float countTime = 0.0;


protected:
    virtual bool OnUserCreate() override
    {
        sprWolf = new olc::Sprite("sprites/wolf.png"); // 6.21 KB (180x82)

        return true;
    }

    bool OnUserUpdate(float fElapsedTime) override
    {
        // called one per frame
        // Clear screen
        PixelGameEngine::ConsoleClear();
        Clear(olc::BLACK);

        countTime += fElapsedTime;
        std::cout << "TIME: " << countTime << std::endl;

        // DRAW ---------------------------------------------------------------------------
        if(countTime >= 0.07 && countTime <= 0.14)
        {
            DrawPartialSprite((ScreenWidth() / 2) - 16, (ScreenHeight() / 2) - 16, sprWolf, 0, 0, 45, 25);
        }
        else if(countTime > 0.14 && countTime <= 0.21)
        {
            DrawPartialSprite((ScreenWidth() / 2) - 16, (ScreenHeight() / 2) - 16, sprWolf, 45, 0, 45, 24);
        }
        else if(countTime > 0.21 && countTime <= 0.28)
        {
            DrawPartialSprite((ScreenWidth() / 2) - 16, (ScreenHeight() / 2) - 16, sprWolf, 90, 0, 45, 25);
        }
        else if(countTime > 0.28)
        {
            DrawPartialSprite((ScreenWidth() / 2) - 16, (ScreenHeight() / 2) - 16, sprWolf, 135, 0, 45, 24);
            countTime = 0;
        }
        else
        {
            DrawPartialSprite((ScreenWidth() / 2) - 16, (ScreenHeight() / 2) - 16, sprWolf, 135, 0, 45, 24); // repeat last
        }

        return true;
    }
};

int main()
{
    DireWolf demo;
    if (demo.Construct(180, 80, 8, 8))
    {
        demo.Start();
    }

    return 0;
}