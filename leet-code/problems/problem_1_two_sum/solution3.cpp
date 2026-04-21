#include <unordered_map>
#include <vector>

class Solution3
{
  public:
    std::vector<int> run(const std::vector<int> &nums, int target)
    {
      std::unordered_map<int, int> seen;
      for (int i = 0; i < (int)nums.size(); ++i)
      {
        auto it = seen.find(target - nums[i]);
        if (it != seen.end()) return {it->second, i};
        seen[nums[i]] = i;
      }
      return {};
    }
};
