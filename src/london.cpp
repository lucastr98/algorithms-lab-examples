#include <iostream>
#include <vector>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>

typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, 
    boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, 
    boost::no_property, 
    boost::property<boost::edge_capacity_t, long, 
    boost::property<boost::edge_residual_capacity_t, long, 
    boost::property<boost::edge_reverse_t, traits::edge_descriptor>>>> graph;
typedef traits::vertex_descriptor vertex_desc;
typedef traits::edge_descriptor edge_desc;

class edge_adder {
  graph &G;
 public:
  explicit edge_adder(graph &G) : G(G) {}
  void add_edge(int from, int to, long capacity) {
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    const auto e = boost::add_edge(from, to, G).first;
    const auto rev_e = boost::add_edge(to, from, G).first;
    c_map[e] = capacity;
    c_map[rev_e] = 0;
    r_map[e] = rev_e;
    r_map[rev_e] = e;
  }
};

void testcase() {
  int h, w;
  std::string note;
  std::cin >> h >> w >> note;

  std::vector<int> letter_count(26, 0);
  int n = 0;
  for (const char& c : note) {
    ++letter_count[int(c - 'A')];
    ++n;
  }

  std::vector<std::vector<int>> front(h, std::vector<int>(w));
  for (int i = 0; i < h; ++i) {
    for (int j = 0; j < w; ++j) {
      char front_letter; std::cin >> front_letter;
      front[i][j] = int(front_letter - 'A');
    }
  }
  std::vector<std::vector<int>> pieces(26, std::vector<int>(26, 0));
  for (int i = 0; i < h; ++i) {
    for (int j = 0; j < w; ++j) {
      char back_letter; std::cin >> back_letter;
      int back_letter_int = int(back_letter - 'A');
      int front_letter_int = front[i][w - j - 1];
      ++pieces[std::min(front_letter_int, back_letter_int)]
              [std::max(front_letter_int, back_letter_int)];
    }
  }
  
  graph G(26);
  edge_adder adder(G);
  const vertex_desc v_source = boost::add_vertex(G);
  const vertex_desc v_sink = boost::add_vertex(G);
  for (int i = 0; i < 26; ++i) {
    adder.add_edge(v_source, i, letter_count[i]);
  }
  int node_nr = 27;
  for (int i = 0; i < 26; ++i) {
    for (int j = i; j < 26; ++j) {
      if (pieces[i][j] > 0) {
        ++node_nr;
        adder.add_edge(node_nr, v_sink, pieces[i][j]);
        adder.add_edge(i, node_nr, letter_count[i]);
        adder.add_edge(j, node_nr, letter_count[j]);
      }
    }
  }
  int flow = boost::push_relabel_max_flow(G, v_source, v_sink);
  if (flow == n)
    std::cout << "Yes\n";
  else
    std::cout << "No\n";
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  for (int i = 0; i < t; ++i) {
    testcase();
  }
}

