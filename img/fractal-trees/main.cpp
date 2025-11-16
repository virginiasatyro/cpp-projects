/*
    Fractal Trees

    - based on: https://www.youtube.com/watch?v=0jjeOYMjmDU - The Coding Train

    - run: g++ -o main.exe main.cpp -luser32 -lgdi32 -lopengl32 -lgdiplus -lShlwapi -ldwmapi -lstdc++fs -static -std=c++17
*/

#include <iostream>
#include <vector>

#define OLC_PGE_APPLICATION
#include "../commonlib/olcPixelGameEngine.h"

class FractalTree : public olc::PixelGameEngine
{
public:
    FractalTree()
    {
        sAppName = "Fractal Trees";
    }

private:
    float PI = 3.14159f;

    void tree(float x, float y, float length, float angle, float aux)
    {
        if (length < 4)
        {
            FillCircle(x, y, 2, olc::RED);
            return;
        }

        float x1 = x + length * cosf(PI * angle / 180);
        float y1 = y - length * sinf(PI * angle / 180);

        DrawLine(x, y, x1, y1, olc::GREEN);
        tree(x1, y1, length * 0.67, angle - aux, aux);
        tree(x1, y1, length * 0.67, angle - aux, -aux);
    }

protected:
    virtual bool OnUserCreate() override
    {
        // clear screen
        PixelGameEngine::ConsoleClear();
        Clear(olc::BLACK);

        // update 
        float screenWidth = ScreenWidth();
        float screenHeight = ScreenHeight();

        // call tree
        for (int i = 0; i <= 3; i++)
        {
            tree(screenWidth / 2.0, screenHeight, screenHeight / 4.0 + 5, 90, 30);
            tree(screenWidth / 2.0, screenHeight, screenHeight / 4.0 + 5, 90, -30);
        }

        return true;
    }

    // called one per frame
    bool OnUserUpdate(float fElapsedTime) override
    {
        return true;
    }
};

int main()
{
    FractalTree demo;
    if (demo.Construct(128, 80, 8, 8))
    {
        demo.Start();
    }

    return 0;
}