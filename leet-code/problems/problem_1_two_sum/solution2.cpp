#include <unordered_map>
#include <vector>


class Solution2
{
  public:
    std::vector<int> run(const std::vector<int> &nums, int target)
    {
      std::unordered_map<int, int> pos;
      for (int i = 0; i < (int)nums.size(); ++i)
      {
        int need = target - nums[i];
        if (pos.count(need)) return {pos[need], i};
        pos[nums[i]] = i;
      }
      return {};
    }
};