#define OLC_PGE_APPLICATION
#include "../commonlib/olcPixelGameEngine.h"

#include <vector>

// g++ -o main main.cpp -lX11 -lGL -lpthread -lpng -lstdc++fs -std=c++17

constexpr float pi = 3.14159f;

struct Circle
{
    float px;
    float py; // position
    float vx;
    float vy; // velocity
    float ax;
    float ay; // acceleration
    float radius;

    int id;
};

class CirclePhysics : public olc::PixelGameEngine
{
public:
	CirclePhysics()
	{
		sAppName = "Circles Physics";
	}

private:
    std::vector<std::pair<float, float>> modelCircle;
    std::vector<Circle> vecCircles;

    void addCircle(float x, float y, float r = 5.0f)
    {
        Circle c;
        c.px = x;
        c.py = y;
        c.vx = 0;
        c.vy = 0;
        c.ax = 0;
        c.ay = 0;
        c.radius = r;

        c.id = vecCircles.size();
        vecCircles.emplace_back(c);
    }

public:
	bool OnUserCreate() override
	{
        // Called once at the start, so create things here
        // Defeine circle model
        modelCircle.push_back({0.0f, 0.0f});
        int points = 20;
        for (int i = 0; i < points; i++)
        {
            modelCircle.push_back({cosf(1 / (float)(points - 1) * 2.0 * pi), (float)(points - 1) * 2.0f * pi});
        }

        float defaultRadius = 8.0f;
        addCircle(ScreenWidth() * 0.25f, ScreenHeight() * 0.5f, defaultRadius);
        addCircle(ScreenWidth() * 0.75f, ScreenHeight() * 0.5f, defaultRadius);

        return true;
    }

    bool OnUserUpdate(float fElapsedTime) override
    {
        // called once per frame

        PixelGameEngine::ConsoleClear();

        // Draw circles
        for (auto circle : vecCircles)
        {
            //void DrawCircle(int32_t x, int32_t y, int32_t radius, Pixel p = olc::WHITE, uint8_t mask = 0xFF);
            DrawCircle(circle.px, circle.py, circle.radius);
        }

        return true;
    }
};

int main()
{
	CirclePhysics demo;
    // Construct screen
    if (demo.Construct(160, 120, 8, 8))
    {
        demo.Start();
    }
    else
    {
        std::cout << "Error: could not construct the console!" << std::endl;
    }

    return 0;
}
