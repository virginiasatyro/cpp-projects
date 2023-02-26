#define OLC_PGE_APPLICATION
#include "../commonlib/olcPixelGameEngine.h"

// g++ -o main.exe main.cpp -luser32 -lgdi32 -lopengl32 -lgdiplus -lShlwapi -ldwmapi -lstdc++fs -static -std=c++17

class Fireworks : public olc::PixelGameEngine
{
public:
    Fireworks()
    {
        sAppName = "Fireworks";
    }

private:
    struct Particle
    {
        float x = 0.0f;
        float y = 0.0f;
        float vx = 0.0f;
        float vy = 0.0f;
        float fuse = 0.0f;
        float lifeTime = 0.0f;
    };

    class Firework : public Particle
    {
    public:
        Firework(float position, float ground, int size)
        {
            x = position;
            y = ground;
            vy = -100.0f;
            vx = 0.0f;
        }

        void update(float fElapsedTime)
        {
            float gravity = 10.0f;

            x += vx * fElapsedTime;
            y += (vy + gravity) * fElapsedTime;
        }

        void drawSelf(olc::PixelGameEngine *gfx)
        {
            gfx->Draw(x, y, olc::WHITE);
        }
    };

    std::list<Firework> listFireworks;

protected:
    bool OnUserCreate() override
    {
        // Called once at the start, so create things here


        return true;
    }

    bool OnUserUpdate(float fElapsedTime) override
    {
        // Called once per frame
        // Clear screen
        PixelGameEngine::ConsoleClear();
        Clear(olc::BLACK);

        if (GetMouse(olc::Mouse::LEFT).bReleased)
        {
            listFireworks.push_back({128.0f, (float)ScreenHeight(), 100});
        }

        for(auto &f : listFireworks)
        {
            f.update(fElapsedTime);
            f.drawSelf(this);
        }

        return true;
    }
};

int main()
{
    Fireworks demo;
    if (demo.Construct(256, 240, 4, 4))
    {
        demo.Start();
    }

    return 0;
}

/*
x. = x + vx * dt
y. = y + vy * dt -> y. = y + (G + vy)

(0,0)
   _________
  |         |  | +y  | G
  |         |  >     >
  |_________|
  
vx = vx * drag = 0.99
*/