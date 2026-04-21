/*

You have a dial (like a safe lock or clock):

  - It has values from MIN to MAX (usually 0 to 99)
  - You can rotate forward or backward
  - If you go past the end, it wraps around

Examples:

  - Start 98, rotate +5 → 3
  - Start 2, rotate -5 → 97

So this is a wrap-around (circular) problem.

------------------------------------------------

If your dial has 100 positions (0–99):
pos % 100
automatically keeps pos inside [0..99]
So positive overflow is solved instantly.

------------------------------------------------

However, negative overflow needs special care.
To handle negative overflow, you can use:
((pos % MOD) + MOD) % MOD
-3 % 100 == -3   //  NOT 97
pos = pos % 100; //  breaks for negative rotations

pos % 100 -> Brings number closer to range, but might be still negative
Add 100
(pos % 100) + 100
-3 + 100 = 97 // Now it’s positive
Modulo again
((pos % 100) + 100) % 100
97 % 100 == 97 // stays inside range

*/

/*
#include <fstream>
#include <iostream>
#include <list>
#include <string>

constexpr int MOD = 100;

void printResult(char dir, int steps, int result)
{
  std::cout << "The dial is rotated " << dir + std::to_string(steps) << " to point at: " << result
            << std::endl;
}

void readFile(std::list<std::string> &instructions)
{
  std::ifstream file("test-2.txt");

  if (file.is_open())
  {
    std::string line;
    while (std::getline(file, line))
    {
      instructions.push_back(line);
    }
    file.close();
  }
  else
  {
    std::cerr << "Unable to open file" << std::endl;
  }
}

int moveDial(int start, int rotation, int *count)
{
  int pos = start + rotation;

  return pos = ((pos % MOD) + MOD) % MOD;
}

int main()
{
  std::list<std::string> instructions{};
  readFile(instructions);

  // print instructions
  int start = 50;
  int count = 0;
  std::cout << "The dial starts by pointing at: " << start << std::endl;
  for (const auto &instr : instructions)
  {
    char dir = instr[0];
    int steps = std::stoi(instr.substr(1));

    int rotation = (dir == 'L') ? -steps : steps;

    int result = moveDial(start, rotation, &count);
    result == 0 ? ++count : count;
    int pos = start + rotation;
    if (rotation > 0)
    {
      count += pos / MOD;
    }
    else
    {
      count += (MOD - 1 - pos) / MOD;
    }
    start = result;

    printResult(dir, steps, result);
  }

  std::cout << "\nCount: " << count << std::endl;  // 1071

  return 0;
}*/

#include <fstream>
#include <iostream>
#include <string>

// We are basically working mod 100

const int MOD = 100;

int main()
{
  std::ifstream file("test-2.txt");
  std::string cmd;

  int position = 50;
  int pwd = 0;

  while (std::getline(file, cmd))
  {
    char direction = cmd[0];
    int rotations = std::stoi(cmd.substr(1));

    if (direction == 'L')
    {
      if (position == 0)
        pwd += rotations / MOD;
      else
        pwd += (rotations / MOD) + ((rotations % MOD) >= position);
      position = (position - rotations % MOD + MOD) % MOD;
    }
    else if (direction == 'R')
    {
      pwd += (rotations / MOD) + ((rotations % MOD) >= (MOD - position));
      position = (position + rotations % MOD) % MOD;
    }
  }
  std::cout << pwd;
}