#include <iostream>
#include <map>
#include <unordered_map>
#include <vector>

// my solution, not the best one but it works
class Solution2
{
  public:
    int run(std::vector<int> &nums)
    {
      std::map<int, int> myMap{};

      for (auto n : nums)
      {
        if (myMap.find(n) == myMap.end())
        {
          // not found - lets add
          myMap[n] = 1;
        }
        else
        {
          myMap[n] = myMap[n] + 1;
        }
      }

      for (const auto &[a, b] : myMap)
      {
        if (b == 1)
        {
          return a;
        }
      }

      return 0;
    }
};
