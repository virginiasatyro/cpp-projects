/**
 * @file main.cpp
 * @brief Game of Life - Cellular Automata Simulator
 *
 * Conway's Game of Life Implementation
 * Rules:
 *  1. Any live cell with 2-3 neighbors survives
 *  2. Any dead cell with exactly 3 neighbors becomes alive
 *  3. All other cells die or stay dead
 *
 * Controls:
 *  - SPACE: Pause/Resume simulation
 *
 * @author Based on OneLoneCoder (Javidx9)
 * @date 2025
 */

#define OLC_PGE_APPLICATION
#include <string>

#include "../../commonlib/olcPixelGameEngine.h"

class Example : public olc::PixelGameEngine
{
  public:
    Example()
    {
      sAppName = "Game of Life";
    }

  private:
    int *output{};
    int *state{};

  public:
    // Called once at the start, so create things here
    bool OnUserCreate() override
    {
      output = new int[ScreenWidth() * ScreenHeight()];
      state = new int[ScreenWidth() * ScreenHeight()];

      memset(output, 0, ScreenWidth() * ScreenHeight() * sizeof(int));
      memset(state, 0, ScreenWidth() * ScreenHeight() * sizeof(int));

      // random initialize
      /*for (int i = 0; i < ScreenWidth() * ScreenHeight(); i++)
      {
        state[i] = rand() % 2;
      }*/

      // manual initialize (glider)
      auto set = [&](int x, int y, std::wstring s)
      {
        int p = 0;
        for (auto c : s)
        {
          state[(y * ScreenWidth()) + x + p] = (c == L'#') ? 1 : 0;
          p++;
        }
      };

      // R-Pentonimo
      /*set(80, 50, L"  ## ");
      set(80, 51, L" ##  ");
      set(80, 52, L"  #  ");*/

      // Gosper Glider Gun
      /*set(60, 45, L"........................#............");
      set(60, 46, L"......................#.#............");
      set(60, 47, L"............##......##............##.");
      set(60, 48, L"...........#...#....##............##.");
      set(60, 49, L"##........#.....#...##...............");
      set(60, 50, L"##........#...#.##....#.#............");
      set(60, 51, L"..........#.....#.......#............");
      set(60, 52, L"...........#...#.....................");
      set(60, 53, L"............##.......................");*/

      // Infinite Growth Pattern
      set(20, 50, L"########.#####...###......#######.#####");

      return true;
    }

    // Called once per frame
    bool OnUserUpdate(float fElapsedTime) override
    {
      if (GetKey(olc::Key::SPACE).bHeld)
      {
        return true;
      }

      std::this_thread::sleep_for(std::chrono::milliseconds(50));

      auto cell = [&](int x, int y) { return output[y * ScreenWidth() + x]; };

      // store output state
      for (int i = 0; i < ScreenWidth() * ScreenHeight(); i++)
      {
        output[i] = state[i];
      }

      for (int x = 1; x < ScreenWidth() - 1; x++)
      {
        for (int y = 1; y < ScreenHeight() - 1; y++)
        {
          int neighbors = 0;
          // count neighbors
          neighbors += cell(x - 1, y - 1);
          neighbors += cell(x, y - 1);
          neighbors += cell(x + 1, y - 1);
          neighbors += cell(x - 1, y);
          neighbors += cell(x + 1, y);
          neighbors += cell(x - 1, y + 1);
          neighbors += cell(x, y + 1);
          neighbors += cell(x + 1, y + 1);

          // apply rules
          if (cell(x, y) == 1)
          {
            // is alive
            state[y * ScreenWidth() + x] = neighbors == 2 || neighbors == 3;
          }
          else
          {
            // become alive
            state[y * ScreenWidth() + x] = neighbors == 3;
          }

          // DRAW -------------------------------------------------------------
          if (cell(x, y) == 1)
          {
            // alive
            Draw(x, y, olc::GREEN);
          }
          else
          {
            // dead
            Draw(x, y, olc::BLACK);
          }
        }
      }

      return true;
    }
};

int main()
{
  Example demo;
  // construct screen
  if (demo.Construct(320, 200, 4, 4))
  {
    demo.Start();
  }

  return 0;
}
