#define OLC_PGE_APPLICATION
#include "../commonlib/olcPixelGameEngine.h"

// https://www.youtube.com/watch?v=jde1Jq5dF0E&list=PLrOv9FMX8xJEEQFU_eAvYTsyudrewymNl&index=30
// g++ -o main.exe main.cpp -luser32 -lgdi32 -lopengl32 -lgdiplus -lShlwapi -ldwmapi -lstdc++fs -static -std=c++17

constexpr int32_t patch = 8; // 8 x 8 pixels

class MenuObject
{
public:
    MenuObject()
    {
        name = "root";
    }

    MenuObject(const std::string &name_)
    {
        name = name_;
    }

    /* SETTERS ------------------------ */
    MenuObject &setTable(int columns, int rows)
    {
        cellTable = {columns, rows};
        return *this; // returns pointer to itself
    }

    MenuObject &setId(int32_t id_)
    {
        id = id_;
        return *this;
    }

    MenuObject &setEnabled(bool enable_)
    {
        enabled = enable_;
        return *this;
    }

    /* GETTERS ------------------------ */
    bool getEnabled()
    {
        return enabled;
    }

    int32_t getId()
    {
        return id;
    }

    std::string &getName()
    {
        return name;
    }

    // for now, cells are simply one line strings
    olc::vi2d getSize()
    {
        return {int32_t(name.size()), 1};
    }

    bool hasChildren()
    {
        return !items.empty();
    }

    MenuObject &operator[](const std::string &name)
    {
        if (itemPointer.count(name) == 0)
        {
            // create the item and link the name with the current index of the vector
            itemPointer[name] = items.size();
            items.push_back(MenuObject(name));
        }

        return items[itemPointer[name]];
    }

    // MenuObject is external to the PixelGameEngine
    // For it to access the drawing routines, we'll need to pass a reference
    void drawSelf(olc::PixelGameEngine &pge, olc::Sprite *sprGFX, olc::vi2d screenOffset)
    {
        // record current pixel mode user is using
        // 9 patch pixel has transparency
        olc::Pixel::Mode currentPixelMode = pge.GetPixelMode();
        pge.SetPixelMode(olc::Pixel::MASK);

        // Draw panel and border
        olc::vi2d patchPos = {0, 0};
        for (patchPos.x = 0; patchPos.x < sizeInPatches.x; patchPos.x++)
        {
            for (patchPos.y = 0; patchPos.y < sizeInPatches.y; patchPos.y++)
            {
                // Determine position in screen space
                olc::vi2d screenLocation = patchPos * patch + screenOffset;

                // Calculate which patch is needed
                olc::vi2d sourcePatch = {0, 0};

                if (patchPos.x > 0)
                {
                    sourcePatch.x = 1;
                }
                if (patchPos.x == sizeInPatches.x - 1)
                {
                    sourcePatch.x = 2;
                }
                if (patchPos.y > 0)
                {
                    sourcePatch.y = 1;
                }
                if (patchPos.y == sizeInPatches.y - 1)
                {
                    sourcePatch.y = 2;
                }

                // Draw actual patch
                pge.DrawPartialSprite(screenLocation, sprGFX, sourcePatch * patch, patchSize);
            }
        }

        // hack? about atd::map
        olc::vi2d cell = {0, 0};
        for (auto& m : items)
        {
            // Patch location (including border offset and padding)
            patchPos.x = cell.x * (cellSize.x + cellPadding.x) + 1;
            patchPos.y = cell.y * (cellSize.y + cellPadding.y) + 1;

            // Actual screen location in pixels
            olc::vi2d screenLocation = patchPos * patch + screenOffset;

            // Display item header
            pge.DrawString(screenLocation, m.getName(), olc::YELLOW);

            cell.y++;
        }

        pge.SetPixelMode(currentPixelMode);
    }

    void build()
    {
        // recursevely buils all childern, so they can determine their size, use that size
        // to indicate cell sizes if this object contains more than one item
        for (auto &m : items)
        {
            if(m.hasChildren())
            {
                m.build();
            }

            // longest child name determines cell width
            cellSize.x = std::max(m.getSize().x, cellSize.x);
            cellSize.y = std::max(m.getSize().y, cellSize.y);
        }

        // adjust size of this object (in patches) if it were rendered as a panel
        // magic numbers - aestethics for drawing
        sizeInPatches.x = cellTable.x * cellSize.x + (cellTable.x - 1) * cellPadding.x + 2;
        sizeInPatches.y = cellTable.y * cellSize.y + (cellTable.y - 1) * cellPadding.y + 2;

        // calculate how many rows thisitem has to hold
        totalRows = (items.size() / cellTable.x) + (((items.size() % cellTable.x) > 0) ? 1 : 0);
    }

protected:
    std::string name;
    bool enabled = true;
    int32_t id = -1;
    int32_t totalRows = 0;
    int32_t topVisibleRow = 0;

    olc::vi2d cellTable = {1, 0};
    olc::vi2d cellSize = {0, 0};
    olc::vi2d cellPadding = {2, 0};

    const olc::vi2d patchSize = {patch, patch};
    olc::vi2d sizeInPatches = {0, 0};

    std::unordered_map<std::string, size_t> itemPointer; // do not force order to be alphabetical
    std::vector<MenuObject> items;
}; // end of class MenuObject


class RetroPopUpMenu : public olc::PixelGameEngine
{
public:
    RetroPopUpMenu()
    {
        sAppName = "Retro Pop up Menu";
    }

    olc::Sprite *sprGFX = nullptr;

    MenuObject mO;

public:
    bool OnUserCreate() override
    {
        // Called once at the start, so create things here
        sprGFX = new olc::Sprite("img/RetroMenu.png");

        mO["main"].setTable(1, 10);
        mO["main"]["Attack"].setId(101);
        mO["main"]["Magic"].setId(102);
        mO["main"]["Defend"].setId(103);
        mO["main"]["Items"].setId(104);
        mO["main"]["Escape"].setId(105);
        mO.build();

        return true;
    }

    bool OnUserUpdate(float fElapsedTime) override
    {
        // called once per frame
        // MenuObject mO;

        // we can created this linked because each set functions returns a pointer to itself
        //mO["Magic"]["Black"]["Fire"].setId(101).setEnabled(false).setTable(2, 4);
        //mO["Magic"]["Black"]["Ice"].setId(101).setEnabled(false).setTable(2, 4);

        Clear(olc::BLACK);

        mO["main"].drawSelf(*this, sprGFX, {10, 10});

        return true;
    }
};

int main()
{
    RetroPopUpMenu demo;
    // construct screen
    if (demo.Construct(384, 240, 4, 4))
    {
        demo.Start();
    }

    return 0;
}

/*
Tree construction

table 2 x 4

A B
C D
E F
G H

- each item is a menu object ->
table 1 x 5

a
b
c
d
e

menuobject "main"
 |
 --> menuobject "attack"
 |
 --> menuobject "magic"
 |               |
 |               --> menuobject "white"
 |               |                |
 |               |                --> menuobject "cure"
 |               |                |
 |               |                --> menuobject "curez"
 |               |
 |               --> menuobject "black"
 |                               |
 |                               --> menuobject "fire"
 |                               |
 |                               --> menuobject "ice"
 |
 --> menuobject "defend"

 9 - patch (approach)
 1  2   3
 4  5   6
 7  8   9

 ----> 8 px x 8 px


*/
