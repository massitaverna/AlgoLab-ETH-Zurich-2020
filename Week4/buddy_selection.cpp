#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/max_cardinality_matching.hpp>

typedef std::vector<int> VI;
typedef std::map<std::string, VI> SVMap;
typedef std::vector<VI> MI;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> graph;
typedef boost::graph_traits<graph>::vertex_descriptor vertex_desc;


void testcase() {

  //Read input
  int n, c, f; std::cin >> n >> c >> f;
  SVMap cc;
  
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < c; j++) {
      std::string characteristic;
      std::cin >> characteristic;
      auto entry = cc.find(characteristic);
      if (entry != cc.end()) {
        (entry->second).push_back(i);
      } else {
        VI vec;
        vec.reserve(n);
        vec.push_back(i);
        cc.insert(std::make_pair(characteristic, vec));
      }
    }
    
  }

  //Algorithm
  MI sums(n, VI(n));
  for (auto it = cc.cbegin(); it != cc.cend(); it++) {
    VI students = it->second;
    for (int i = 0; i < students.size() - 1; i++) {
      for (int j = i+1; j < students.size(); j++) {
        sums[students[i]][students[j]]++;
      }
    }
  }

  graph G(n);
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      if (sums[i][j] > f) {
        boost::add_edge(i, j, G);
      }
    }
  }

  std::vector<vertex_desc> mate_map(n);
  boost::edmonds_maximum_cardinality_matching(G, boost::make_iterator_property_map(mate_map.begin(),
    boost::get(boost::vertex_index, G)));
  int matching_size = boost::matching_size(G, boost::make_iterator_property_map(mate_map.begin(),
    boost::get(boost::vertex_index, G)));

  //Print solution!
  if (matching_size == n/2) std::cout << "not optimal" << std::endl;
  else std::cout << "optimal" << std::endl;
}

  
  

int main() {
  std::ios_base::sync_with_stdio(false);

  int t; std::cin >> t;
  for (int i = 0; i < t; i++) {
    testcase();
  }

  return 0;
}
