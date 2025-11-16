/**
 * @file main.cpp
 * @brief Maze Generator using Recursive Backtracking Algorithm
 *
 * @description
 * This program generates a perfect maze (no loops, single path between any two points)
 * using the Recursive Backtracking algorithm with Depth-First Search (DFS).
 *
 * Visual Representation:
 * - WHITE: Visited cells (part of the maze paths)
 * - BLUE:  Unvisited cells (walls)
 * - GREEN: Current cell being explored
 *
 * Learning Concepts:
 * 1. Maze generation algorithms (Recursive Backtracking)
 * 2. Stack data structure and backtracking techniques
 * 3. Bitwise operations for efficient flag storage
 * 4. Game loop architecture (OnUserCreate, OnUserUpdate)
 * 5. 2D grid representation as 1D array with linear indexing
 * 6. Object-Oriented Programming with inheritance and override
 *
 * Compilation (Windows with MSYS2/MinGW64):
 *   g++ -std=c++17 -O2 -I../commonlib -o maze.exe main.cpp \
 *       -lgdi32 -lgdiplus -lopengl32 -luser32 -lShlwapi -ldwmapi
 *
 * References:
 * - Video: https://www.youtube.com/watch?v=Y37-gB83HKE
 * - GitHub:
 * https://github.com/OneLoneCoder/Javidx9/blob/master/ConsoleGameEngine/SmallerProjects/OneLoneCoder_Mazes.cpp
 *
 * @author Based on OneLoneCoder (Javidx9)
 * @date 2025
 */

#include <chrono>
#include <iostream>
#include <stack>
#include <thread>
#include <vector>

#define OLC_PGE_APPLICATION
#include "../commonlib/olcPixelGameEngine.h"

class Mazes : public olc::PixelGameEngine
{
  public:
    Mazes()
    {
      sAppName = "Maze Creator";
    }

  private:
    // Grid dimensions
    int mazeWidth{};   ///< Width of maze in cells
    int mazeHeight{};  ///< Height of maze in cells
    int *maze{};       ///< 1D array representing 2D maze grid (allocated dynamically)

    /**
     * @enum Cell Flags
     * @brief Bitwise flags stored in each maze cell (int)
     *
     * Each cell can have multiple flags set simultaneously:
     * Example: maze[i] = 0x15 means CELL_PATH_N (0x01) | CELL_VISITED (0x10)
     *
     * Usage:
     *   - To set a flag:   maze[i] |= CELL_PATH_N;
     *   - To check flag:   if (maze[i] & CELL_VISITED) { ... }
     *   - To set multiple: maze[i] |= CELL_VISITED | CELL_PATH_S;
     */
    enum
    {
      CELL_PATH_N = 0x01,   ///< Bit 0: Open path to NORTH
      CELL_PATH_E = 0x02,   ///< Bit 1: Open path to EAST
      CELL_PATH_S = 0x04,   ///< Bit 2: Open path to SOUTH
      CELL_PATH_W = 0x08,   ///< Bit 3: Open path to WEST
      CELL_VISITED = 0x10,  ///< Bit 4: Cell has been visited (part of maze)
    };

    int visitedCells{};  ///< Counter of visited cells (for algorithm termination)

    /**
     * @brief Stack of current path during maze generation
     *
     * Stores coordinates (x, y) as we move through the maze.
     * Used for backtracking: when a dead-end is reached, we pop() to go back.
     * Top of stack is always the current cell being explored.
     */
    std::stack<std::pair<int, int>> stack;

    int pathWidth{};  ///< Width in pixels of each path segment (for rendering)

  protected:
    // Called once at the start, so create things here
    bool OnUserCreate() override
    {
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

    // Called once per frame
    bool OnUserUpdate(float fElapsedTime) override
    {
      // Slow down the animation
      std::this_thread::sleep_for(std::chrono::milliseconds(10));

      auto offset = [&](int x, int y)
      { return (stack.top().second + y) * mazeWidth + (stack.top().first + x); };

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
            case 0:  // North
              maze[offset(0, -1)] |= CELL_VISITED | CELL_PATH_S;
              maze[offset(0, 0)] |= CELL_PATH_N;
              stack.push(std::make_pair(stack.top().first, (stack.top().second - 1)));
              break;
            case 1:  // East
              maze[offset(1, 0)] |= CELL_VISITED | CELL_PATH_W;
              maze[offset(0, 0)] |= CELL_PATH_E;
              stack.push(std::make_pair((stack.top().first + 1), stack.top().second));
              break;
            case 2:  // South
              maze[offset(0, 1)] |= CELL_VISITED | CELL_PATH_N;
              maze[offset(0, 0)] |= CELL_PATH_S;
              stack.push(std::make_pair(stack.top().first, (stack.top().second + 1)));
              break;
            case 3:  // west
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
          Draw(stack.top().first * (pathWidth + 1) + px, stack.top().second * (pathWidth + 1) + py,
               olc::GREEN);
        }
      }
      return true;
    }
};

int main()
{
  Mazes demo{};
  if (demo.Construct(160, 100, 8, 8))
  {
    demo.Start();
  }
  return 0;
}