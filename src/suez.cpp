#include <iostream>
#include <vector>
#include <climits>

#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpq.h>

typedef CGAL::Gmpq IT;
typedef CGAL::Gmpq ET;
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

struct Nail {
  int x;
  int y;
};

void UpdateClosestDistance(const int& difference, 
    std::pair<int, int>& closest_distances) {
  if (difference < 0 && (-difference < closest_distances.first || 
      closest_distances.first == -1)) {
    closest_distances.first = -difference;
  }
  if (difference > 0 && (difference < closest_distances.second || 
      closest_distances.second == -1)) {
    closest_distances.second = difference;
  }
}

void OldPoster2LP(const int& i, const int& closest_distance, 
    const int& relevant_poster_length, Program& lp, const int& eq_number) {
  if (closest_distance != -1) {
    lp.set_a(i, eq_number, CGAL::Gmpq(relevant_poster_length) / CGAL::Gmpq(2));
    lp.set_b(eq_number, CGAL::Gmpq(closest_distance) - 
        CGAL::Gmpq(relevant_poster_length) / CGAL::Gmpq(2));
  }
}

void testcase() {
  int n, m, h, w;
  std::cin >> n >> m >> h >> w;
  
  std::vector<Nail> posters(n);
  for (int i = 0; i < n; ++i) {
    std::cin >> posters[i].x;
    std::cin >> posters[i].y;
  }
  std::vector<Nail> old_posters(m);
  for (int i = 0; i < m; ++i) {
    std::cin >> old_posters[i].x;
    std::cin >> old_posters[i].y;
  }
  
  Program lp(CGAL::SMALLER, true, 1, false, 0);
  int eq_number = 0;
  for (int i = 0; i < n; ++i) {
    // set objective function (perimeter of this new poster)
    lp.set_c(i, -2 * (w + h));
    
    // all other new posters
    for (int j = i + 1; j < n; ++j) {
      if ((double)abs(posters[i].x - posters[j].x) / (double)w > 
          (double)abs(posters[i].y - posters[j].y) / (double)h) {
        // the 2 posters are bound in x
        lp.set_a(i, eq_number, CGAL::Gmpq(w) / CGAL::Gmpq(2));
        lp.set_a(j, eq_number, CGAL::Gmpq(w) / CGAL::Gmpq(2));
        if (posters[i].x < posters[j].x)  // xi + w/2 * ai <= xj - w/2 * aj
          lp.set_b(eq_number, posters[j].x - posters[i].x);
        else                              // xj + w/2 * aj <= xi - w/2 * ai
          lp.set_b(eq_number, posters[i].x - posters[j].x);
      } else {
        // the 2 posters are bound in y
        lp.set_a(i, eq_number, CGAL::Gmpq(h) / CGAL::Gmpq(2));
        lp.set_a(j, eq_number, CGAL::Gmpq(h) / CGAL::Gmpq(2));
        if (posters[i].y < posters[j].y)  // yi + h/2 * ai <= yj - h/2 * aj
          lp.set_b(eq_number, posters[j].y - posters[i].y);
        else                              // yj + h/2 * aj <= yi - h/2 * ai
          lp.set_b(eq_number, posters[i].y - posters[j].y);
      }
      ++eq_number;
    }
    
    // find the the closest posters out of the old ones in all 4 directions
    std::pair<int, int> closest_distances_x = 
        std::make_pair<int, int>(-1, -1);
    std::pair<int, int> closest_distances_y = 
        std::make_pair<int, int>(-1, -1);
    for (int j = 0; j < m; ++j) {
      if ((double)abs(posters[i].x - old_posters[j].x) / (double)w > 
          (double)abs(posters[i].y - old_posters[j].y) / (double)h) {
        // the 2 posters are bound in x
        UpdateClosestDistance(old_posters[j].x - posters[i].x, 
            closest_distances_x);
      } else {
        // the 2 posters are bound in y
        UpdateClosestDistance(old_posters[j].y - posters[i].y, 
            closest_distances_y);
      }
    }
    OldPoster2LP(i, closest_distances_x.first, w, lp, eq_number++);
    OldPoster2LP(i, closest_distances_x.second, w, lp, eq_number++);
    OldPoster2LP(i, closest_distances_y.first, h, lp, eq_number++);
    OldPoster2LP(i, closest_distances_y.second, h, lp, eq_number++);
  }
  
  Solution s = CGAL::solve_linear_program(lp, ET());
  std::cout << std::fixed << 
      (int)std::ceil((-s.objective_value_numerator() / 
      s.objective_value_denominator()).to_double()) << "\n";
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  for (int i = 0; i < t; ++i) {
    testcase();
  }
}

