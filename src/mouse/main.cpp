/*
    run: g++ -o main.exe main.cpp -luser32 -lgdi32 -lopengl32 -lgdiplus -lShlwapi -ldwmapi -lstdc++fs -static -std=c++17
*/

#include <iostream>

#define OLC_PGE_APPLICATION
#include "../commonlib/olcPixelGameEngine.h"

class MouseTest : public olc::PixelGameEngine
{
public:
    MouseTest()
    {
        sAppName = "Mouse Test";
    }

private:
   // float mouseX = 0.0;
 //   float mouseY = 0.0;

protected:
    virtual bool OnUserCreate() override
    {
        return true;
    }

    // called one per frame
    bool OnUserUpdate(float fElapsedTime) override
    {   
        // Clear screen
        PixelGameEngine::ConsoleClear();
        Clear(olc::BLACK);

        // DRAW --------------------------------------------------------------------------------------------
        Draw(GetMouseX() - 1, GetMouseY());
        Draw(GetMouseX(),  GetMouseY());
        Draw(GetMouseX() + 1,  GetMouseY());
        Draw(GetMouseX(),  GetMouseY() - 1);
        Draw(GetMouseX(),  GetMouseY() + 1);

        if(GetMouse(olc::Mouse::LEFT).bHeld)
        {
            FillRect(GetMouseX(), GetMouseY(), GetMouseX() + 20, GetMouseY() + 20, olc::YELLOW);
        }

        if(GetMouse(olc::Mouse::RIGHT).bHeld)
        {
            FillCircle(GetMouseX(), GetMouseY(), 8, olc::RED);
        }

        return true;
    }
};

int main()
{
    MouseTest demo;
    if (demo.Construct(128, 80, 12, 12))
    {
        demo.Start();
    }

    return 0;
}