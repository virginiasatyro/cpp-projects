#define OLC_PGE_APPLICATION
#include "../commonlib/olcPixelGameEngine.h"

// g++ -o main.exe main.cpp -luser32 -lgdi32 -lopengl32 -lgdiplus -lShlwapi -ldwmapi -lstdc++fs -static -std=c++17

constexpr float PI = 3.14159f;

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
        olc::Pixel color = olc::WHITE;
    };

    class Firework : public Particle
    {
    public:
        Firework(float position, float ground, int size)
        {
            x = position;
            y = ground;
            vx = randomFloat(100.0f) - 50.0f;
            vy = -100.0f;
            fuse = randomFloat(2.0f) + 1.0f; // 1 - 3
            particlesCount = size;
        }

        void update(float fElapsedTime)
        {
            float gravity = 35.0f;
            float drag = 0.999;
            lifeTime += fElapsedTime; // fElapsedTime = time per frame

            if (lifeTime <= fuse)
            {
                // Launching time
                vx *= drag;
                x += vx * fElapsedTime;
                y += (vy + gravity) * fElapsedTime;
            }
            else
            {
                // Explode
                if (!exploded)
                {
                    exploded = true;
                    olc::Pixel particleColor = olc::Pixel(rand() % 255, rand() % 255, rand() % 255);
                    for (int i = 0; i < particlesCount; i++)
                    {
                        Particle p;
                        p.x = x;
                        p.y = y;
                        float angle = randomFloat(2.0f * PI);
                        float power = randomFloat(75.0f); 
                        p.fuse = randomFloat(4.0f) + 1.0f; // 1 - 5
                        p.vx = cosf(angle) * power;
                        p.vy = sinf(angle) * power;
                        p.color = particleColor;

                        vecParticles.push_back(p);
                    }
                }
                else
                {
                    for (auto &p : vecParticles)
                    {
                        p.lifeTime += fElapsedTime;
                        p.vx *= drag;
                        p.x += p.vx * fElapsedTime;
                        p.y += (p.vy + gravity) * fElapsedTime;
                    }
                }
            }
        }

        float randomFloat(float max)
        {
            return ((float)rand() / (float)RAND_MAX) * max;
        }

        void drawSelf(olc::PixelGameEngine *gfx)
        {
            if (lifeTime <= fuse)
            {
                // Launching time
                gfx->Draw(x, y, olc::WHITE);
            }
            else
            {
                // Explode
                expired = true;
                for (auto &p : vecParticles)
                {
                    if(p.lifeTime <= p.fuse)
                    {
                        expired = false;
                        gfx->Draw(p.x, p.y, p.color * ((p.lifeTime >= p.fuse * 0.75f) ? 0.5f : 1.0f));
                    }
                }
            }
        }

        std::vector<Particle> vecParticles;
        int particlesCount = 0;
        bool exploded = false;
        bool expired = false;
    };

    std::list<Firework> listFireworks;
    float delay = 1.0f;

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
            // listFireworks.push_back({128.0f, (float)ScreenHeight(), 100});
        }

        delay -= fElapsedTime;
        if (delay <= 0.0f)
        {
            delay = ((float)rand() / (float)RAND_MAX) * 2.0f + 0.1f;
            listFireworks.push_back({128.0f, (float)ScreenHeight(), rand() % 800 + 100});
        }

        for(auto &f : listFireworks)
        {
            f.update(fElapsedTime);
            f.drawSelf(this);
        }

        listFireworks.remove_if([](const Firework &f)
                                { return f.expired; });

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