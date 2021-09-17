#include <iostream>
#include <vector>
#include <utility>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_2.h>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

typedef std::vector<int>                                       VI;
typedef CGAL::Exact_predicates_inexact_constructions_kernel    K;
typedef int                                                    Index;
typedef CGAL::Triangulation_vertex_base_with_info_2<Index,K>   Vb;
typedef CGAL::Triangulation_face_base_2<K>                     Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb>            Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>                  Triangulation;
typedef K::Point_2                                             Point;
typedef std::vector<Point>                                     VP;
typedef std::pair<Point, Index>                                IPoint;
typedef int                                                    IT;
typedef CGAL::Gmpz                                             ET;
typedef CGAL::Quadratic_program<IT>                            Program;
typedef CGAL::Quadratic_program_solution<ET>                   Solution;

void testcase() {
  // Read input
  int z, u, v, w, a, g;
  std::cin >> z >> u >> v >> w >> a >> g;
  
  std::vector<IT> U(g);
  std::vector<IT> V(g);
  std::vector<IT> W(g);
  std::vector<IPoint> gangMembers;
  gangMembers.reserve(g);
  for (int i = 0; i < g; i++) {
    int x, y;
    std::cin >> x >> y >> U[i] >> V[i] >> W[i];
    gangMembers.emplace_back(Point(x, y), i);
  }
  
  VP agents;
  agents.reserve(a);
  std::vector<IT> Z(a);
  for (int i = 0; i < a; i++) {
    int x, y;
    std::cin >> x >> y >> Z[i];
    agents.push_back(Point(x, y));
  }
  
  //Algorithm
  Triangulation t;
  t.insert(gangMembers.begin(), gangMembers.end());
  VI spiedBy(g, -1);
  for (int i = 0; i < a; i++) {
    int member = t.nearest_vertex(agents[i])->info();
    if (spiedBy[member] == -1 or Z[spiedBy[member]] > Z[i]) {
      spiedBy[member] = i;
    }
  }
  VI spiesOn(a, -1);
  for (int i = 0; i < g; i++) {
    if (spiedBy[i] != -1) {
      spiesOn[spiedBy[i]] = i;
    }
  }
  
  Program lp(CGAL::SMALLER, true, 0, true, 24);
  int var = 0;
  for (int i = 0; i < a; i++) {
    if (spiesOn[i] == -1) continue;
    lp.set_a(var, 0, -U[spiesOn[i]]);
    lp.set_a(var, 1, -V[spiesOn[i]]);
    lp.set_a(var, 2, -W[spiesOn[i]]);
    lp.set_a(var, 3, Z[i]);
    var++;
  }
  lp.set_b(0, -u);
  lp.set_b(1, -v);
  lp.set_b(2, -w);
  lp.set_b(3, z);
  Solution s = CGAL::solve_linear_program(lp, ET());
  
  //Print solution!
  if (s.is_infeasible()) {
    std::cout << "H" << std::endl;
  } else {
    std::cout << "L" << std::endl;
  }
  
}

int main() {
  std::ios_base::sync_with_stdio(false);
  
  int t; std::cin >> t;
  while(t--) testcase();
}
