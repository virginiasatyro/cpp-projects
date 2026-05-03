#include <algorithm>
#include <iostream>
#include <string>

// not mine, but optimized solution
class Solution2
{
  public:
    bool run(std::string s)
    {
      int left = 0, right = s.length() - 1;

      while (left < right)
      {
        // Skip non-alphanumeric characters from the left
        while (left < right && !isalnum(s[left]))
        {
          left++;
        }
        // Skip non-alphanumeric characters from the right
        while (left < right && !isalnum(s[right]))
        {
          right--;
        }

        // Compare characters (case-insensitive)
        if (tolower(s[left]) != tolower(s[right]))
        {
          return false;
        }

        left++;
        right--;
      }

      return true;
    }
};