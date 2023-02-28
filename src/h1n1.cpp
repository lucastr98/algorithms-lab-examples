#include <iostream>
#include <vector>
#include <queue>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_2<K> Vb;
typedef CGAL::Triangulation_face_base_with_info_2<int, K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb, Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K, Tds> Triangulation;

struct Neighbor {
  int id;
  K::FT d_sqrd;
};

void testcase(const int& n) {
  std::vector<K::Point_2> pts;
  pts.reserve(n);
  for (int i = 0; i < n; ++i) {
    int x, y;
    std::cin >> x >> y;
    pts.push_back(K::Point_2{x, y});
  }
  Triangulation t;
  t.insert(pts.begin(), pts.end());
  
  // set id of all infinite faces to 0 and of all finite faces to correct id
  Triangulation::Face_circulator f0 = t.incident_faces(t.infinite_vertex());
  do {
    f0->info() = 0;
  } while (++f0 != t.incident_faces(t.infinite_vertex()));
  int id = 1;
  for (Triangulation::Finite_faces_iterator f = t.finite_faces_begin(); 
      f != t.finite_faces_end(); ++f)
    f->info() = id++;
  
  std::vector<std::vector<Neighbor>> neighbors(id);
  for (Triangulation::Finite_faces_iterator f = t.finite_faces_begin(); 
      f != t.finite_faces_end(); ++f) {
    int u = f->info();
    for (int i = 0; i < 3; ++i) {
      Triangulation::Face_handle f2 = f->neighbor(i);
      int v = f2->info();
      K::Point_2 p1 = f->vertex((i + 1) % 3)->point();
      K::Point_2 p2 = f->vertex((i + 2) % 3)->point();
      K::FT width = CGAL::squared_distance(p1, p2);
      
      neighbors[u].push_back({v, width});
      if (v == 0) {
        neighbors[v].push_back({u, width});
      }
    }
  }
  
  std::vector<K::FT> escape_d(id, 0);
  escape_d[0] = (K::FT)std::numeric_limits<long>::max();
  std::queue<int> q;
  q.push(0);
  while (!q.empty()) {
    int u = q.front();
    q.pop();
    for (const Neighbor& v : neighbors[u]) {
      K::FT critical = escape_d[u];
      if (v.d_sqrd < critical)
        critical = v.d_sqrd;
      if (critical > escape_d[v.id]) {
        escape_d[v.id] = critical;
        q.push(v.id);
      }
    }
  }
  
  int m; std::cin >> m;
  for (int i = 0; i < m; ++i) {
    int x, y;
    K::FT d;
    std::cin >> x >> y >> d;
    K::Point_2 p(x, y);
    if (CGAL::squared_distance(p, t.nearest_vertex(p)->point()) < d) {
      std::cout << "n";
      continue;
    }
    
    Triangulation::Face_handle f = t.locate(p);
    int v = f->info();
    if (escape_d[v] >= K::FT(4) * d)
      std::cout << "y";
    else
      std::cout << "n";
  }
  std::cout << "\n";
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int n; std::cin >> n;
  while (n) {
    testcase(n);
    std::cin >> n;
  }
}

