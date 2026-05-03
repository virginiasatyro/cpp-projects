#include <vector>

// optimized solution using bit manipulation
// In giving an array except for one element others have the duplicate element
// so they will become zero except one unique element.
class Solution3
{
  public:
    int run(std::vector<int> &nums)
    {
      int result = 0;
      for (auto n : nums)
      {
        // Apply XOR operation to every element
        result ^= n;
      }

      return result;
    }
};
