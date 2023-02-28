#include <iostream>
#include <vector>
#include <algorithm>

struct StrengthGroup {
  int strength;
  int count;
};

void testcase() {
  int n, m;
  std::cin >> n >> m;
  std::vector<int> strengths(n);
  for (int i = 0; i < n; ++i)
    std::cin >> strengths[i];
  std::sort(strengths.begin(), strengths.end(), std::greater<int>());
  
  std::vector<StrengthGroup> strength_groups;
  int last_strength = strengths[0], count = 1;
  for (int i = 1; i < n; ++i) {
    if (strengths[i] == last_strength) {
      ++count;
      continue;
    }
    strength_groups.push_back(StrengthGroup{last_strength, count});
    last_strength = strengths[i];
    count = 1;
  }
  strength_groups.push_back(StrengthGroup{last_strength, count});
  
  std::vector<int> box_weights(m);
  for (int i = 0; i < m; ++i) {
    std::cin >> box_weights[i];
  }
  std::sort(box_weights.begin(), box_weights.end(), std::greater<int>());
  
  long rounds, box_it, strength_group_it, count_sum;
  if (strength_groups[0].strength < box_weights[0]) {
    std::cout << "impossible\n";
    return;
  } else {
    rounds = 1;
    box_it = strength_groups[0].count;
    strength_group_it = 0;
    count_sum = strength_groups[0].count;
  }
  while (box_it < m) {
    if (strength_group_it == strength_groups.size() - 1 ||
        box_weights[box_it] > strength_groups[strength_group_it + 1].strength) {
      ++rounds;
      box_it += count_sum;
    } else {
      ++strength_group_it;
      box_it += rounds * strength_groups[strength_group_it].count;
      count_sum += strength_groups[strength_group_it].count;
    }
  }
  
  std::cout << rounds * 3 - 1 << "\n";
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  for (int i = 0; i < t; ++i) {
    testcase();
  }
}

