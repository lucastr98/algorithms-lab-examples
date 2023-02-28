#include <iostream>
#include <vector>

struct Target {
  int vertex_nr;
  long edge_weight;
};

void testcase() {
  int n, m, k;
  long x;
  std::cin >> n >> m >> x >> k;
  
  std::vector<std::vector<Target>> canals(n);
  for (int i = 0; i < m; ++i) {
    Target target;
    int u; std::cin >> u;
    std::cin >> target.vertex_nr;
    std::cin >> target.edge_weight;
    canals[u].push_back(target);
  }
  
  // at element i memo contains the maximum score at hole i after num_moves
  std::vector<long> memo(n, -1);
  memo[0] = 0;
  for (int num_moves = 1; num_moves <= k; ++num_moves) {
    const std::vector<long> reference_memo = memo;
    for (int source = 0; source < n; ++source) {
      if (reference_memo[source] == -1) continue;
      for (auto const& target : canals[source]) {
        if (reference_memo[source] + target.edge_weight > 
            memo[target.vertex_nr]) {
          memo[target.vertex_nr] = reference_memo[source] + target.edge_weight;
          if (canals[target.vertex_nr].size() == 0 && 
              memo[target.vertex_nr] > memo[0]) {
            // weyaya
            memo[0] = memo[target.vertex_nr];
          } 
          // finished
          if (memo[target.vertex_nr] >= x) {
            std::cout << num_moves << "\n";
            return;
          }
        }
      }
    }
  }
  
  std::cout << "Impossible\n";
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  for (int i = 0; i < t; ++i) {
    testcase();
  }
}
