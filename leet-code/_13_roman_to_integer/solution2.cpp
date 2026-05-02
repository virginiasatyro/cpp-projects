#include <iostream>
#include <map>
#include <string>
#include <vector>

// optimized solution, better than mine (https://www.youtube.com/watch?v=tsmrUi5M1JU&t=1s)
class Solution2
{
  public:
    int run(const std::string &roman)
    {
      std::map<char, int> romanMap = {{'I', 1},   {'V', 5},   {'X', 10},  {'L', 50},
                                      {'C', 100}, {'D', 500}, {'M', 1000}

      };

      std::vector<char> letters{};
      for (char c : roman)
      {
        letters.push_back(c);
      }

      if (letters.size() < 1 || roman.length() > 15)
      {
        return -1;
      }

      int result = 0;
      for (int i = 0; i < letters.size() - 1; i++)
      {
        if (romanMap[letters[i]] < romanMap[letters[i + 1]])
        {
          result -= romanMap[letters[i]];
        }
        else
        {
          result += romanMap[letters[i]];
        }
      }

      return result + romanMap[letters.back()];
    }
};
