#include <iostream>
#include <vector>
#include <algorithm>

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>

typedef CGAL::Exact_predicates_exact_constructions_kernel K;

struct Biker {
  long y0;
  K::FT m;
  int id;
};

void testcase() {
  int n; std::cin >> n;
  std::vector<Biker> bikers(n);
  for (int i = 0; i < n; ++i) {
    long y0, x1, y1;
    std::cin >> y0 >> x1 >> y1;
    bikers[i] = {y0, (K::FT)(y1 - y0) / (K::FT)x1, i};
  }
  
  std::sort(bikers.begin(), bikers.end(), [](const Biker& l, const Biker& r) { 
      return l.y0 < r.y0; });

  std::vector<bool> survived(n, true);
  for (int i = 0; i < n; ++i) {
    for (int j = i - 1; j >= 0; --j) {
      if (!survived[bikers[j].id])
        continue;
      
      if (bikers[i].m >= bikers[j].m)
        break;
      
      if (abs(bikers[i].m) < abs(bikers[j].m)) {
        // i survives
        survived[bikers[j].id] = false;
      } else {
        // j survives
        survived[bikers[i].id] = false;
        break;
      }
    }
  }
  
  for (int i = 0; i < n; ++i) {
    if (survived[i])
      std::cout << i << " ";
  }
  std::cout << "\n";
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  for (int i = 0; i < t; ++i) {
    testcase();
  }
}

