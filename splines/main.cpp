/*
    SPLINES

    - Splines, usadas em desenhos de engenharia, são réguas flexíveis, de madeira ou plástico, 
    que podem ser curvadas de forma a passar por um dado conjunto de pontos (xi,fi) chamados nós.
    Um spline é uma curva definida matematicamente por dois ou mais pontos de controle. 
    Os pontos de controle que ficam na curva são chamados de nós.

    - based on, for learning purposes: https://www.youtube.com/watch?v=9_aJGUTePYo

    - https://pt.wikipedia.org/wiki/Spline

    - use multiple control points and the path is garanteed to go on all points - CATMULL-ROM

    - https://en.wikipedia.org/wiki/Centripetal_Catmull%E2%80%93Rom_spline (Catmull–Rom spline)
    
    - run:  g++ -o main.exe main.cpp -luser32 -lgdi32 -lopengl32 -lgdiplus -lShlwapi -ldwmapi -lstdc++fs -static -std=c++17

    - TODO: 
        - play with the splines to generate the path of a speceship;
        - automatically generate the looped spline;
        - calculate the gradient so the spaceship will know where to point the nose;
        - interesting for charecter AI;
*/

#include <iostream>
#include <vector>

#define OLC_PGE_APPLICATION
#include "../commonlib/olcPixelGameEngine.h"

struct point2D
{
    float x;
    float y;
};

struct Spline
{
    std::vector<point2D> points;

    point2D getSplinePoint(float t, bool looped = false)
    {
        int p0, p1, p2, p3;
        if (!looped)
        {
            p1 = (int)t + 1;
            p2 = p1 + 1;
            p3 = p2 + 1;
            p0 = p1 - 1;
        }
        else
        {
            p1 = (int)t;
            p2 = (p1 + 1) % points.size();
            p3 = (p2 + 1) % points.size();
            p0 = p1 >= 1 ? p1 - 1 : points.size() - 1;
        }

        t = t - (int)t;

        float tt = t * t;
        float ttt = tt * t;

        float q1 = -ttt + 2.0 * tt - t;
        float q2 = 3.0 * ttt - 5.0 * tt + 2.0;
        float q3 = -3.0 * ttt + 4.0 * tt + t;
        float q4 = ttt - tt;

        float tx = 0.5 * (points[p0].x * q1 + points[p1].x * q2 + points[p2].x * q3 + points[p3].x * q4);
        float ty = 0.5 * (points[p0].y * q1 + points[p1].y * q2 + points[p2].y * q3 + points[p3].y * q4);

        return {tx, ty};
    }

    point2D getSplineGradient(float t, bool looped = false)
    {
        int p0, p1, p2, p3;
        if (!looped)
        {
            p1 = (int)t + 1;
            p2 = p1 + 1;
            p3 = p2 + 1;
            p0 = p1 - 1;
        }
        else
        {
            p1 = (int)t;
            p2 = (p1 + 1) % points.size();
            p3 = (p2 + 1) % points.size();
            p0 = p1 >= 1 ? p1 - 1 : points.size() - 1;
        }

        t = t - (int)t;

        float tt = t * t;
        float ttt = tt * t;

        float q1 = -3.0f * tt + 4.0f * t - 1;
        float q2 = 9.0f * tt - 10.0f * t;
        float q3 = -9.0f * tt + 8.0f * t + 1.0f;
        float q4 = 3.0f * tt - 2.0f * t;

        float tx = 0.5 * (points[p0].x * q1 + points[p1].x * q2 + points[p2].x * q3 + points[p3].x * q4);
        float ty = 0.5 * (points[p0].y * q1 + points[p1].y * q2 + points[p2].y * q3 + points[p3].y * q4);

        return {tx, ty};
    }
};

class Splines : public olc::PixelGameEngine
{
public:
    Splines()
    {
        sAppName = "Splines";
    }

private:
    Spline path;
    int selectedPoint = 0;
    float marker = 0.0;

protected:
    virtual bool OnUserCreate() override
    {
        //path.points = {{10, 41}, {40, 41}, {70, 41}, {100, 41}};
        path.points = { { 10, 41 },{ 20, 41 },{ 30, 41 },{ 40, 41 },{ 50, 41 },{ 60, 41 },{ 70, 41 },{ 80, 41 },{ 90, 41 },{ 100, 41 } };
        
        return true;
    }

    bool OnUserUpdate(float fElapsedTime) override
    {
        // Clear screen
        PixelGameEngine::ConsoleClear();
        Clear(olc::BLACK);

        // INPUT --------------------------------------------------------------------------------------------------------------
        if(GetKey(olc::X).bReleased)
        {
            selectedPoint++;
            if (selectedPoint >= path.points.size())
            {
                // to not go out of bounds
                selectedPoint = 0;
            }
        }

        if(GetKey(olc::Z).bReleased)
        {
            selectedPoint--;
            if (selectedPoint < 0)
            {
                selectedPoint = path.points.size() - 1;
            }
        }

        if(GetKey(olc::LEFT).bHeld)
        {
            path.points[selectedPoint].x -= 30.0 * fElapsedTime;
        }

        if(GetKey(olc::RIGHT).bHeld)
        {
            path.points[selectedPoint].x += 30.0 * fElapsedTime;
        }

        if(GetKey(olc::UP).bHeld)
        {
            path.points[selectedPoint].y -= 30.0 * fElapsedTime;
        }

        if(GetKey(olc::DOWN).bHeld)
        {
            path.points[selectedPoint].y += 30.0 * fElapsedTime;
        }

        if(GetKey(olc::A).bHeld)
        {
            marker -= 5.0 * fElapsedTime;
        }

        if(GetKey(olc::S).bHeld)
        {
            marker += 5.0 * fElapsedTime;
        }

        // Boundary checks for marker
        if(marker >= (float)path.points.size())
        {
            marker -= (float)path.points.size();
        }

        if(marker < 0.0)
        {
            marker += (float)path.points.size();
        }

        // DRAW --------------------------------------------------------------------------------------------------------
        // Draw spline
        // for(float t = 0.0; t < 1.0; t += 0.01)
        for(float t = 0.0; t < (float)path.points.size(); t += 0.005)
        {
            point2D pos = path.getSplinePoint(t, true);
            Draw(pos.x, pos.y);
        }
        
        // Draw points
        for(int i = 0; i < path.points.size(); i++)
        {
            FillRect(path.points[i].x - 1, path.points[i].y - 1, 3, 3, olc::RED);
			DrawString(path.points[i].x, path.points[i].y, std::to_string(i), olc::WHITE);
        }

        // Highlight control point
        FillRect(path.points[selectedPoint].x - 1, path.points[selectedPoint].y - 1, 3, 3, olc::YELLOW);
	    DrawString(path.points[selectedPoint].x, path.points[selectedPoint].y, std::to_string(selectedPoint), olc::WHITE);

        // Draw agent to demonstrante gradient
        point2D p1 = path.getSplinePoint(marker, true);
        point2D g1 = path.getSplineGradient(marker, true);
        float r = atan2(-g1.y, g1.x);
        DrawLine(5.0 * sin(r) + p1.x, 5.0 * cos(r) + p1.y, -5.0 * sin(r) + p1.x, -5.0 * cos(r) + p1.y, olc::BLUE); // offeset by 5 pixels

        return true;
    }
};

int main()
{
    Splines demo;
    //if (demo.Construct(160, 80, 10, 10))
    // have to change it because of draw string function (string is big)
    if (demo.Construct(240, 150, 4, 4))
    {
        demo.Start();
    }

    return 0;
}