#define OLC_PGE_APPLICATION
#include "../commonlib/olcPixelGameEngine.h"

#include <vector>

// g++ -o main main.cpp -lX11 -lGL -lpthread -lpng -lstdc++fs -std=c++17

constexpr float pi = 3.14159f;

struct Bubble
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

class BubblePhysics : public olc::PixelGameEngine
{
public:
	BubblePhysics()
	{
		sAppName = "Bubbles Physics";
	}

private:
    std::vector<std::pair<float, float>> modelBubble;
    std::vector<Bubble> vecBubbles;

    Bubble *selectedBubble = nullptr;

    void addBubble(float x, float y, float r = 5.0f)
    {
        Bubble c;
        c.px = x;
        c.py = y;
        c.vx = 0;
        c.vy = 0;
        c.ax = 0;
        c.ay = 0;
        c.radius = r;

        c.id = vecBubbles.size();
        vecBubbles.emplace_back(c);
    }

public:
	bool OnUserCreate() override
	{
        // Called once at the start, so create things here

        // Define bubble model
        modelBubble.push_back({0.0f, 0.0f});
        int points = 20;
        for (int i = 0; i < points; i++)
        {
            modelBubble.push_back({cosf(1 / (float)(points - 1) * 2.0 * pi), (float)(points - 1) * 2.0f * pi});
        }

        float defaultRadius = 8.0f;
        // Add Bubbles
        //addBubble(ScreenWidth() * 0.25f, ScreenHeight() * 0.5f, defaultRadius);
        //addBubble(ScreenWidth() * 0.75f, ScreenHeight() * 0.5f, defaultRadius);

        // adding 10 bubbles in randon locations
        for(int i = 0; i < 10; i++)
        {
            addBubble(rand() % ScreenWidth(), rand() % ScreenHeight(), defaultRadius); // not really randon
        }

        return true;
    }

    bool OnUserUpdate(float fElapsedTime) override
    {
        // called once per frame

        auto doBubblesOverlap = [](float x1, float y1, float r1, float x2, float y2, float r2)
        {
            // sqrt((x1-x2)*(x2-x2) + (y1-y2)*(y1-y2) < (r1+r2))
            // fabs((x1-x2)*(x2-x2) + (y1-y2)*(y1-y2) <= (r1+r2)*(r1+r2))
            return fabs((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2)) <= (r1 + r2) * (r1 + r2);
        };

        auto isPointInBubble = [](float x1, float y1, float r1, float px, float py)
        {
            return fabs((x1 - px) * (x1 - px)) + ((y1 - py) * (y1 - py)) < (r1 * r1);
        };

        // INPUT ===========================================================================================================================

        // interect with the bubbles using mouse
        if(GetMouse(olc::Mouse::LEFT).bPressed || GetMouse(olc::Mouse::RIGHT).bPressed)
        {
            selectedBubble = nullptr;
            for(auto &bubble : vecBubbles)
            {
                if(isPointInBubble(bubble.px, bubble.py, bubble.radius, GetMouseX(), GetMouseY()))
                {
                    selectedBubble = &bubble; // select the bubble
                    break;
                }
            }
        }

        if(GetMouse(olc::Mouse::LEFT).bHeld)
        {
            if(selectedBubble != nullptr)
            {
                selectedBubble->px = GetMouseX();
                selectedBubble->py = GetMouseY();
            }
        }

        if(GetMouse(olc::Mouse::LEFT).bReleased)
        {
            selectedBubble = nullptr;
        }
    
        if(GetMouse(olc::Mouse::RIGHT).bReleased)
        {
            if(selectedBubble != nullptr)
            {
                // apply velocity
                selectedBubble->vx = 5.0f * ((selectedBubble->px) - (float)GetMouseX());
                selectedBubble->vy = 5.0f * ((selectedBubble->py) - (float)GetMouseY());
            }
            selectedBubble = nullptr;
        }

        std::vector<std::pair<Bubble *, Bubble *>> vecCollidingPairs;

        // Update bubbles positions
        for (auto &bubble : vecBubbles)
        {
            bubble.vx += bubble.ax * fElapsedTime;
            bubble.vy += bubble.ay * fElapsedTime;
            bubble.px += bubble.vx * fElapsedTime;
            bubble.py += bubble.vy * fElapsedTime;

            // bubble rotate
            if (bubble.px < 0)
            {
                bubble.px += (float)ScreenWidth();
            }
            if (bubble.px >= ScreenWidth())
            {
                bubble.px -= (float)ScreenWidth();
            }
            if (bubble.py < 0)
            {
                bubble.py += (float)ScreenHeight();
            }
            if (bubble.py >= ScreenHeight())
            {
                bubble.py -= (float)ScreenHeight();
            }

            if(fabs(bubble.vx * bubble.vx + bubble.vy*bubble.vy) < 0.01f) // stop is had coded
            {
                bubble.vx = 0;
                bubble.vy = 0;
            }
        }

        //  ===========================================================================================================================

        for (auto &bubble : vecBubbles) // teste every bubble
        {
            for (auto &target : vecBubbles) // teste against a target bubble
            {
                // do not test the bubble against itself
                if (bubble.id != target.id)
                {
                    if (doBubblesOverlap(bubble.px, bubble.py, bubble.radius, target.px, target.py, target.radius))
                    {
                        // collision occured
                        vecCollidingPairs.push_back({&bubble, &target});

                        // distance between bubble centers
                        float distance = sqrtf((bubble.px - target.px) * (bubble.px - target.px) + (bubble.py - target.py) * (bubble.py - target.py));

                        float overlap = 0.5f * (distance - bubble.radius - target.radius);

                        // displace current ball
                        bubble.px -= overlap * (bubble.px - target.px) / distance;
                        bubble.py -= overlap * (bubble.py - target.py) / distance;

                        // displace target ball
                        target.px += overlap * (bubble.px - target.px) / distance;
                        target.py += overlap * (bubble.py - target.py) / distance;
                    }
                }
            }
        }

        /// work out dynamic collisions
        for (auto c : vecCollidingPairs)
        {
            // the vector contain only the colliding objects
            Bubble *b1 = c.first;
            Bubble *b2 = c.second;
        }

        // DRAW ===========================================================================================================================

        PixelGameEngine::ConsoleClear();
        Clear(olc::BLACK);

        // Draw Bubbles
        for (auto bubble : vecBubbles)
        {
            DrawCircle(bubble.px, bubble.py, bubble.radius);
        }

        // Draw line
        for (auto c : vecCollidingPairs)
        {
            DrawLine(c.first->px, c.first->py, c.second->px, c.second->py, olc::RED);
        }

        // Draw cue
        if(selectedBubble != nullptr)
        {
            DrawLine(selectedBubble->px, selectedBubble->py, GetMouseX(), GetMouseY(), olc::BLUE);
        }

        return true;
    }
};

int main()
{
    BubblePhysics demo;
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
