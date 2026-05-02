#include <iostream>
#include <map>
#include <regex>
#include <string>
#include <vector>

// optimized solution, better than mine (https://www.youtube.com/watch?v=tsmrUi5M1JU&t=1s)
class Solution3
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
      std::string newString{};
      std::regex pattern("IV");
      newString = std::regex_replace(roman, pattern, "IIII");
      std::regex pattern2("IX");
      newString = std::regex_replace(newString, pattern2, "VIIII");
      std::regex pattern3("XL");
      newString = std::regex_replace(newString, pattern3, "XXXX");
      std::regex pattern4("XC");
      newString = std::regex_replace(newString, pattern4, "LXXXX");
      std::regex pattern5("CD");
      newString = std::regex_replace(newString, pattern5, "CCCC");
      std::regex pattern6("CM");
      newString = std::regex_replace(newString, pattern6, "DCCCC");

      for (char c : newString)
      {
        result += romanMap[std::string(1, c)];
      }

      return result;
    }
};
