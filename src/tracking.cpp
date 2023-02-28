#include <iostream>
#include <vector>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
    boost::no_property, boost::property<boost::edge_weight_t, int>> graph;

struct River {
  int a;
  int b;
  int c;
};

void testcase() {
  int n, m, k, x, y;
  std::cin >> n >> m >> k >> x >> y;
  
  graph G(n * (k + 1));
  for (int i = 0; i < m; ++i) {
    int a, b, c;
    bool d;
    std::cin >> a >> b >> c >> d;
    for (int j = 0; j <= k; ++j) {
      boost::add_edge(j * n + a, j * n + b, c, G);
      if (d && j < k) {
        boost::add_edge(j * n + a, (j + 1) * n + b, c, G);
        boost::add_edge(j * n + b, (j + 1) * n + a, c, G);
      }
    }
  }
  
  std::vector<int> dist_map(n * (k + 1));
  boost::dijkstra_shortest_paths(G, x,
      boost::distance_map(boost::make_iterator_property_map(
      dist_map.begin(), boost::get(boost::vertex_index, G))));
  std::cout << dist_map[k * n + y] << "\n";
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  for (int i = 0; i < t; ++i) {
    testcase();
  }
}

