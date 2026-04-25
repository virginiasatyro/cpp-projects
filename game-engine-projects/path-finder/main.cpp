#include <iostream>
#include <vector>

#define OLC_PGE_APPLICATION
#include "../commonlib/olcPixelGameEngine.h"

class PathFinder : public olc::PixelGameEngine
{
  public:
    PathFinder()
    {
      sAppName = "Path Finder";
    }

  private:
    struct Node
    {
        bool obstacle = false;             // is the node an obstacle?
        bool visited = false;              // has the node been visited?
        float globalGoal = INFINITY;       // distance to goal so far
        float localGoal = INFINITY;        // distance to goal if we took the alternative route
        int x = 0;                         // x position in grid
        int y = 0;                         // y position in grid
        std::vector<Node *> VecNeighbors;  // connections to neighbors
        Node *parent = nullptr;  // node connecting to this node that offers shortest path so far
    };

    Node *nodes = nullptr;
    int mapWidth = 16;
    int mapHeight = 16;

  protected:
    bool OnUserCreate() override
    {
      // Called once at the start, so create things here
      // Create a 2D array of nodes
      nodes = new Node[mapWidth * mapHeight];
      for (int x = 0; x < mapWidth; x++)
      {
        for (int y = 0; y < mapHeight; y++)
        {
          nodes[y * mapWidth + x].x = x;
          nodes[y * mapWidth + x].y = y;
          nodes[y * mapWidth + x].obstacle =
              false;  // (rand() % 5 == 0) randomly place some obstacles
          nodes[y * mapWidth + x].visited = false;
          nodes[y * mapWidth + x].parent = nullptr;
        }
      }
      return true;
    }

    bool OnUserUpdate(float fElapsedTime) override
    {
      // called once per frame
      int nodeSize = 9;
      int nodeBorder = 2;

      // use integer division to nicely get cursor position in node
      int selectedNodeX = GetMouseX() / nodeSize;
      int selectedNodeY = GetMouseY() / nodeSize;

      // use mouse click to toggle obstacle state of node
      if (GetMouse(0).bPressed)
      {
        if (selectedNodeX >= 0 && selectedNodeX < mapWidth && selectedNodeY >= 0 &&
            selectedNodeY < mapHeight)
        {
          nodes[selectedNodeY * mapWidth + selectedNodeX].obstacle =
              !nodes[selectedNodeY * mapWidth + selectedNodeX].obstacle;
        }
      }

      // clear screen
      Clear(olc::BLACK);

      // draw nodes on top
      for (int x = 0; x < mapWidth; x++)
      {
        for (int y = 0; y < mapHeight; y++)
        {
          int drawX = x * nodeSize;
          int drawY = y * nodeSize;

          FillRect(drawX + nodeBorder, drawY + nodeBorder, nodeSize - nodeBorder * 2,
                   nodeSize - nodeBorder * 2,
                   nodes[y * mapWidth + x].obstacle ? olc::WHITE : olc::BLUE);

          // FillRect(drawX + nodeBorder, drawY + nodeBorder, nodeSize - nodeBorder * 2,
          //          nodeSize - nodeBorder * 2, olc::BLUE);
          //  DrawRect(drawX, drawY, nodeSize, nodeSize, olc::BLACK);
        }
      }

      return true;
    }
};

int main()
{
  PathFinder demo;
  // construct screen
  if (demo.Construct(160, 160, 6, 6))
  {
    demo.Start();
  }

  return 0;
}
