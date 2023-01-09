#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <thread>

#define OLC_PGE_APPLICATION
#include "../commonlib/olcPixelGameEngine.h"

// g++ -o main.exe main.cpp -luser32 -lgdi32 -lopengl32 -lgdiplus -lShlwapi -ldwmapi -lstdc++fs -static -std=c++17

class GameOfLife : public olc::PixelGameEngine
{
public:
    GameOfLife()
    {
        sAppName = "Game Of Life";
    }
    int32_t totalScreen = ScreenWidth() * ScreenHeight();

private:
    int *output;
    int *state;

protected:
    virtual bool OnUserCreate() override
    {
        output = new int[totalScreen];
        state = new int[totalScreen];
        memset(output, 0, totalScreen * sizeof(int));
        memset(state, 0, totalScreen * sizeof(int));

        // set initial cell states
        for (auto i = 0; i < totalScreen; i++)
        {
            state[i] = rand() % 2; // initialize cells randonly
        }

        // one more lambda function
        auto set = [&](int x, int y, std::wstring s)
        {
            int p = 0;
            for (auto c : s)
            {
                state[y * ScreenWidth() + x + p] = c == L'#' ? 1 : 0;
                p++;
            }
        };

       /* // R-Pentonimo
        set(80, 50, L"  ## ");
        set(80, 50, L" ##  ");
        set(80, 50, L" #   ");
*/
        return true;
    }

    bool OnUserUpdate(float fElapsedTime) override
    {
        /*if (!GetKey(olc::Key::SPACE).bHeld)
        {
            return true; // can be used to stop and see the state
        }*/
        // delay a bit
        std::this_thread::sleep_for( std::chrono::milliseconds(25));

        // lambda function - I need to study it more
        auto cell = [&](int x, int y)
        {
            return output[y * ScreenWidth() + x];
        };

        // store output state
        for (int i = 0; i < totalScreen; i++)
        {
            output[i] = state[i];
        }

        for (int x = 1; x < ScreenWidth() - 1; x++)
        {
            for (int y = 1; y < ScreenHeight() - 1; y++)
            {
                int neighbours = cell(x - 1, y - 1) + cell(x - 0, y - 1) +
                                 cell(x + 1, y - 1) + cell(x - 1, y + 0) +
                                 0 + cell(x + 1, y + 0) + cell(x - 1, y + 1) +
                                 cell(x + 0, y + 1) + cell(x + 1, y + 1);

                if (cell(x, y) == 1)
                {
                    state[y * ScreenWidth() + x] = neighbours == 2 || neighbours == 3;
                }
                else
                {
                    state[y * ScreenWidth() + x] = neighbours == 3;
                }

                if (cell(x, y) == 1) // check if cell is alive
                {
                    Draw(x, y, olc::WHITE);
                }
                else
                {
                    Draw(x, y, olc::BLACK);
                }
            }
        }

        return true;
    }
};

int main()
{
    GameOfLife game;
    if (game.Construct(160, 100, 8, 8))
    {
        game.Start();
    }

    return 0;
}