#include <vector>

class Solution1
{
  public:
    std::vector<int> run(const std::vector<int> &nums, int target)
    {
      int sum = 0;
      for (int i = 0; i < target; ++i)
      {
        if (i % nums[0] == 0 || i % nums[1] == 0)
        {
          sum += i;
        }
      }
      return {sum};
    }
};
