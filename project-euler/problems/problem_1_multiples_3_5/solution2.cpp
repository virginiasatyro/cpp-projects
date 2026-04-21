#include <numeric>
#include <vector>

class Solution2
{
  private:
    // soma de múltiplos de k até abaixo de target:
    // k + 2k + 3k + ... + nk
    long long sumMultiples(long long k, long long target)
    {
      long long n = (target - 1) / k;  // quantidade de múltiplos
      return k * n * (n + 1) / 2;      // soma da PA
    }

  public:
    std::vector<int> run(const std::vector<int> &nums, int target)
    {
      long long a = nums[0];
      long long b = nums[1];

      long long sumA = sumMultiples(a, target);
      long long sumB = sumMultiples(b, target);
      long long sumAB = sumMultiples(std::lcm(a, b), target);  // evita contagem dupla

      long long result = sumA + sumB - sumAB;

      return {static_cast<int>(result)};
    }
};