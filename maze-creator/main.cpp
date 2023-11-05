/*
    Programming Mazes

    - Based on: https://www.youtube.com/watch?v=Y37-gB83HKE - for learning purposes

    - https://github.com/OneLoneCoder/Javidx9/blob/master/ConsoleGameEngine/SmallerProjects/OneLoneCoder_Mazes.cpp

    -  g++ -o main.exe main.cpp -luser32 -lgdi32 -lopengl32 -lgdiplus -lShlwapi -ldwmapi -lstdc++fs -static -std=c++17

    - TODO:
        - what is flood fill algorithm?
        - write on paper the math and logic
        - use this algorthm in a game
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
        memset(maze, 0x00, mazeWidth * mazeHeight * sizeof(int));
        pathWidth = 3;

        // Choose randon starting cell
        int x = rand() % mazeWidth;
        int y = rand() % mazeHeight;
        stack.push(std::make_pair(x, y));
        maze[y * mazeWidth + x] = CELL_VISITED;
        visitedCells = 1;

        return true;
    }

    bool OnUserUpdate(float fElapsedTime) override
    {
        // Called once per frame
        // Slow down the animation
        std::this_thread::sleep_for(std::chrono::milliseconds(10));

        auto offset = [&](int x, int y)
        {
            return (stack.top().second + y) * mazeWidth + (stack.top().first + x);
        };

        // Maze Algorithm -----------------------------------------------
        if (visitedCells < mazeWidth * mazeHeight)
        {
            // Step 1: create a set of unvisited nighbours
            std::vector<int> neighbours;

            // North
            if (stack.top().second > 0 && (maze[offset(0, -1)] & CELL_VISITED) == 0)
            {
                neighbours.push_back(0);
            }
            // East
            if (stack.top().first < mazeWidth - 1 && (maze[offset(1, 0)] & CELL_VISITED) == 0)
            {
                neighbours.push_back(1);
            }
            // South
            if (stack.top().second < mazeHeight - 1 && (maze[offset(0, 1)] & CELL_VISITED) == 0)
            {
                neighbours.push_back(2);
            }
            // West
            if (stack.top().first > 0 && (maze[offset(-1, 0)] & CELL_VISITED) == 0)
            {
                neighbours.push_back(3);
            }

            // are there any neighbours available?
            if (!neighbours.empty())
            {
                // choose a random available neighbour
                int nextCellDir = neighbours[rand() % neighbours.size()];

                // create a path beteen he neighbour and the current cell
                switch (nextCellDir)
                {
                case 0: // North
                    maze[offset(0, -1)] |= CELL_VISITED | CELL_PATH_S;
                    maze[offset(0, 0)] |= CELL_PATH_N;
                    stack.push(std::make_pair(stack.top().first, (stack.top().second - 1)));
                    break;
                case 1: // East
                    maze[offset(1, 0)] |= CELL_VISITED | CELL_PATH_W;
                    maze[offset(0, 0)] |= CELL_PATH_E;
                    stack.push(std::make_pair((stack.top().first + 1), stack.top().second));
                    break;
                case 2: // South
                    maze[offset(0, 1)] |= CELL_VISITED | CELL_PATH_N;
                    maze[offset(0, 0)] |= CELL_PATH_S;
                    stack.push(std::make_pair(stack.top().first, (stack.top().second + 1)));
                    break;
                case 3: // west
                    maze[offset(-1, 0)] |= CELL_VISITED | CELL_PATH_E;
                    maze[offset(0, 0)] |= CELL_PATH_W;
                    stack.push(std::make_pair((stack.top().first - 1), stack.top().second));
                    break;
                default:
                    break;
                }
                // New cell
                // maze[offset(0, 0)] |= CELL_VISITED;
                visitedCells++;
            }
            else
            {
                // backtrack
                stack.pop();
            }
        }

        // Draw ---------------------------------------------------------
        // clear screen
        PixelGameEngine::ConsoleClear();
        Clear(olc::BLACK);

        // Draw Maze
        for (int x = 0; x < mazeWidth; x++)
        {
            for (int y = 0; y < mazeHeight; y++)
            {
                for (int py = 0; py < pathWidth; py++)
                {
                    for (int px = 0; px < pathWidth; px++)
                    {
                        if (maze[y * mazeWidth + x] & CELL_VISITED)
                        {
                            Draw(x * (pathWidth + 1) + px, y * (pathWidth + 1) + py, olc::WHITE);
                        }
                        else
                        {
                            Draw(x * (pathWidth + 1) + px, y * (pathWidth + 1) + py, olc::BLUE);
                        }
                    }
                }

                for (int p = 0; p < pathWidth; p++)
                {
                    if (maze[y * mazeWidth + x] & CELL_PATH_S)
                    {
                        Draw(x * (pathWidth + 1) + p, y * (pathWidth + 1) + pathWidth);
                    }
                    if (maze[y * mazeWidth + x] & CELL_PATH_E)
                    {
                        Draw(x * (pathWidth + 1) + pathWidth, y * (pathWidth + 1) + p);
                    }
                }
            }
        }

        // Draw Unit - the top of the stack
        for (int py = 0; py < pathWidth; py++)
        {
            for (int px = 0; px < pathWidth; px++)
            {
                // Draw Cell 
                Draw(stack.top().first * (pathWidth + 1) + px, stack.top().second * (pathWidth + 1) + py, olc::GREEN);
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