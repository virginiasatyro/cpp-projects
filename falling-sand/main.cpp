/*
    Falling Sand

    - based on: https://www.youtube.com/watch?v=0jjeOYMjmDU - The Coding Train

    - run: g++ -o main.exe main.cpp -luser32 -lgdi32 -lopengl32 -lgdiplus -lShlwapi -ldwmapi -lstdc++fs -static -std=c++17
*/

#include <vector>

#define OLC_PGE_APPLICATION
#include "../commonlib/olcPixelGameEngine.h"

#define SCREEN_W 32
#define SCREEN_H 32
#define SIZE 8

class FallingSand : public olc::PixelGameEngine
{
public:
    FallingSand()
    {
        sAppName = "Falling Sand";
    }

private:
    int matrix[SCREEN_W][SCREEN_H];
    int matrixAux[SCREEN_W][SCREEN_H];
    float delay = 0.0;

protected:
    virtual bool OnUserCreate() override
    {
        for (int i = 0; i < SCREEN_W; i++)
        {
            for (int j = 0; j < SCREEN_H; j++)
            {
                matrix[i][j] = 0;
                matrixAux[i][j] = 0;
            }
        }

        return true;
    }

    // called one per frame
    bool OnUserUpdate(float fElapsedTime) override
    {
        // clear screen
        PixelGameEngine::ConsoleClear();
        Clear(olc::BLACK);

        delay += fElapsedTime;

        // INPUT --------------------------------------------------------------------------------
        if (GetMouse(olc::Mouse::LEFT).bReleased)
        {
            matrix[GetMouseX()][GetMouseY()] = 1;
        }

        // DRAW ---------------------------------------------------------------------------------
        for (int i = 0; i < SCREEN_W; i++)
        {
            for (int j = 0; j < SCREEN_H; j++)
            {
                matrixAux[i][j] = matrix[i][j];
                if (matrix[i][j] == 1)
                {
                    Draw(i, j);
                }
            }
        }

        if(delay <= 0.05)
        {
            return true;
        }

        // ALGORITHM ---------------------------------------------------------------------------------
        std::cout << "CALL ALGORITH delay " << delay << std::endl;
        for (int i = 0; i < SCREEN_W; i++)
        {
            for (int j = 0; j < SCREEN_H; j++)
            {
                if (matrixAux[i][j] == 1)
                {
                    // found a particle
                    // check the send bellow
                    if (j + 1 > SCREEN_H - 1)
                    {

                    }
                    else if (matrixAux[i][j + 1] == 0)
                    {
                        matrix[i][j] = 0;
                        matrix[i][j + 1] = 1;
                    }
                }
            }
        }

        delay = 0;
        return true;
    }
};

int main()
{
    FallingSand demo;
    if (demo.Construct(SCREEN_W, SCREEN_H, SIZE, SIZE))
    {
        demo.Start();
    }

    return 0;
}