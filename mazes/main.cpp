/*
    Programming Mazes

    - Based on: https://www.youtube.com/watch?v=Y37-gB83HKE - for learning purposes

    -  g++ -o main.exe main.cpp -luser32 -lgdi32 -lopengl32 -lgdiplus -lShlwapi -ldwmapi -lstdc++fs -static -std=c++17

*/

#include <iostream>
#include <stack>

#define OLC_PGE_APPLICATION // necessary to compile - but why?
#include "../commonlib/olcPixelGameEngine.h"

class Mazes : public olc::PixelGameEngine
{
public:
    Mazes()
    {
        sAppName = "Maze Creator";
    }

private:
    int mazeWidth;
    int mazeHeight;
    int *maze;

    enum
    {
        CELL_PATH_N = 0x01,
        CELL_PATH_E = 0x02,
        CELL_PATH_S = 0x04,
        CELL_PATH_W = 0x08,
        CELL_VISITED = 0x10,
    };

    int visitedCells;

    std::stack<std::pair<int, int>> stack;

    int pathWidth;

protected:
    bool OnUserCreate() override
    {
        // Called once at the start, so create things here
        // Maze parameters
        mazeWidth = 40;
        mazeHeight = 25;

        // Initializing -----------------------------------------------
        maze = new int[mazeWidth * mazeHeight];
        memset(maze, 0x00, mazeWidth * mazeHeight);

        stack.push(std::make_pair(0, 0));
        maze[0] = CELL_VISITED;
        visitedCells = 1;

        pathWidth = 3;

        return true;
    }

    bool OnUserUpdate(float fElapsedTime) override
    {
        // Called once per frame
        // Draw ---------------------------------------------------------
        // clear screen
        PixelGameEngine::ConsoleClear();
        Clear(olc::BLACK);

        // Draw Maze
        for (int x = 0; x < mazeWidth; x++)
        {
            for (int y = 0; y < mazeHeight; y++)
            {
                if (maze[y * mazeWidth + x] & CELL_VISITED)
                {
                    Draw(x * (pathWidth + 1), y * (pathWidth + 1), olc::WHITE);
                }
                else
                {
                    Draw(x * (pathWidth + 1), y * (pathWidth + 1), olc::BLUE);
                }
            }
        }

        return true;
    }
};

int main()
{
    Mazes demo;
    if (demo.Construct(160, 100, 8, 8))
    {
        demo.Start();
    }
    return 0;
}