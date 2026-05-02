#include <iostream>
#include <map>
#include <string>
#include <vector>

// my solution, not the best one but it works
class Solution1
{
  public:
    int run(const std::string &roman)
    {
      std::map<std::string, int> romanMap = {{"I", 1},   {"V", 5},   {"X", 10},  {"L", 50},
                                             {"C", 100}, {"D", 500}, {"M", 1000}

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
      for (int i = letters.size() - 1; i >= 0; i--)
      {
        if (letters[i] == 'M')
        {
          result += romanMap["M"];
        }
        else if (letters[i] == 'D')
        {
          result += romanMap["D"];
        }
        else if (letters[i] == 'C')
        {
          if (i < letters.size() - 1 && letters[i + 1] == 'D')
          {
            result -= romanMap["C"];  // 400
          }
          else if (i < letters.size() - 1 && letters[i + 1] == 'M')
          {
            result -= romanMap["C"];  // 900
          }
          else
          {
            result += romanMap["C"];  // 100
          }
        }
        else if (letters[i] == 'L')
        {
          result += romanMap["L"];  // 50
        }
        else if (letters[i] == 'X')
        {
          if (i < letters.size() - 1 && letters[i + 1] == 'L')
          {
            result -= romanMap["X"];  // 40
          }
          else if (i < letters.size() - 1 && letters[i + 1] == 'C')
          {
            result -= romanMap["X"];  // 90
          }
          else
          {
            result += romanMap["X"];  // 10
          }
        }
        else if (letters[i] == 'V')
        {
          result += romanMap["V"];  // 5
        }
        else if (letters[i] == 'I')
        {
          if (i < letters.size() - 1 && letters[i + 1] == 'V')
          {
            result -= romanMap["I"];  // 4
          }
          else if (i < letters.size() - 1 && letters[i + 1] == 'X')
          {
            result -= romanMap["I"];  // 9
          }
          else
          {
            result += romanMap["I"];
          }
        }
      }
      return result;
    }
};
