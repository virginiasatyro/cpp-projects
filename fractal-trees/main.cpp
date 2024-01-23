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
    float angle = PI / 4.0f;

    void tree(float x, float y, float length, float angle, float aux)
    {
        if(length < 4)
        {
            std::cout << "Length < 4";
            FillCircle(x, y, 2, olc::RED);
            return;
        }

        float x1 = 0.0;
        float y1 = 0.0;

        x1 = x + (length * cosf(PI * angle / 180));
        y1 = y - length * sinf(PI * angle / 180);
        std::cout << "x = " << x << " x1 = " << x1 << "y = " << y << " y1 = " << y1 << std::endl;
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

        // call tree
        for(int i = 0; i <=3; i++)
            tree(64.0, 80, 25, 90, 30);
            tree(64.0, 80, 25, 90, -30);

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
    if (demo.Construct(128, 80, 4, 4))
    {
        demo.Start();
    }

    return 0;
}