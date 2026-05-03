#include <algorithm>
#include <iostream>
#include <string>

// mixing my solution with the optimized one
class Solution3
{
  public:
    bool run(std::string s)
    {
      s.erase(std::remove_if(s.begin(), s.end(), [](unsigned char c) { return !std::isalnum(c); }),
              s.end());
      std::transform(s.begin(), s.end(), s.begin(), ::tolower);

      int left = 0, right = s.length() - 1;

      while (left < right)
      {
        // Compare characters (case-insensitive)
        if (s[left] != s[right])
        {
          return false;
        }

        left++;
        right--;
      }

      return true;
    }
};