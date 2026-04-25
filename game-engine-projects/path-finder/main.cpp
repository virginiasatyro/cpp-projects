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

    Node *nodeStart = nullptr;
    Node *nodeEnd = nullptr;

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
          auto &node = nodes[y * mapWidth + x];
          node.x = x;
          node.y = y;
          node.obstacle = false;
          node.visited = false;
          node.parent = nullptr;
        }
      }

      // create connections - in this case nodes are on a regular grid
      for (int x = 0; x < mapWidth; x++)
      {
        for (int y = 0; y < mapHeight; y++)
        {
          auto &node = nodes[y * mapWidth + x];
          // clang-format off
          if (y > 0) node.VecNeighbors.push_back(&nodes[(y - 1) * mapWidth + (x + 0)]);
          if (y < mapHeight - 1) node.VecNeighbors.push_back(&nodes[(y + 1) * mapWidth + (x + 0)]);
          if (x > 0) node.VecNeighbors.push_back(&nodes[(y + 0) * mapWidth + (x - 1)]);
          if (x < mapWidth - 1) node.VecNeighbors.push_back(&nodes[(y + 0) * mapWidth + (x + 1)]);
          // clang-format on
        }
      }

      // manually position the start and end points
      nodeStart = &nodes[(mapHeight / 2) * mapWidth + 1];
      nodeEnd = &nodes[(mapHeight / 2) * mapWidth + (mapWidth - 2)];

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

      // draw connections first
      for (int x = 0; x < mapWidth; x++)
      {
        for (int y = 0; y < mapHeight; y++)
        {
          auto &node = nodes[y * mapWidth + x];
          int drawX = x * nodeSize + nodeSize / 2;
          int drawY = y * nodeSize + nodeSize / 2;

          for (auto neighbor : node.VecNeighbors)
          {
            if (neighbor->obstacle || node.obstacle)
            {
              continue;  // don't draw connections to or from obstacles
            }

            DrawLine(drawX, drawY, neighbor->x * nodeSize + nodeSize / 2,
                     neighbor->y * nodeSize + nodeSize / 2, olc::GREY);
          }
        }
      }

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

          if (&nodes[y * mapWidth + x] == nodeStart)
          {
            // start node
            FillRect(drawX + nodeBorder, drawY + nodeBorder, nodeSize - nodeBorder * 2,
                     nodeSize - nodeBorder * 2, olc::GREEN);
          }
          else if (&nodes[y * mapWidth + x] == nodeEnd)
          {
            // end node
            FillRect(drawX + nodeBorder, drawY + nodeBorder, nodeSize - nodeBorder * 2,
                     nodeSize - nodeBorder * 2, olc::RED);
          }
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
