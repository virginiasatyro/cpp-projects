#include <algorithm>
#include <iostream>
#include <string>

// my solution, not the best one but it works
class Solution1
{
  public:
    bool run(std::string s)
    {
      std::transform(s.begin(), s.end(), s.begin(), ::tolower);

      s.erase(std::remove_if(s.begin(), s.end(), [](unsigned char c) { return !std::isalnum(c); }),
              s.end());

      if (s == " " || s == "" || s.size() == 1)
      {
        return true;
      }

      int sSize = s.size();
      std::string s1;
      std::string s2;

      if (sSize == 2)
      {
        s1 = s.substr(0, 1);
        s2 = s.substr(1, 1);
      }
      else if (sSize % 2 == 1)  // impar
      {
        s1 = s.substr(0, sSize / 2);
        s2 = s.substr((sSize / 2) + 1, sSize - 1);
      }
      else
      {
        s1 = s.substr(0, sSize / 2);  // par
        s2 = s.substr(sSize / 2, sSize - 1);
      }

      std::string s2Reverse = s2;
      std::reverse(s2Reverse.begin(), s2Reverse.end());

      if (s1 == s2Reverse)
      {
        return true;
      }

      return false;
    }
};