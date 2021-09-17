#include <iostream>
#include <vector>
#include <utility>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/biconnected_components.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
	boost::no_property, boost::property<boost::edge_weight_t, int>> graph;
typedef boost::graph_traits<graph>::vertex_descriptor vertex_desc;
typedef boost::graph_traits<graph>::edge_descriptor edge_desc;
typedef boost::graph_traits<graph>::edge_iterator edge_it;
typedef boost::graph_traits<graph>::out_edge_iterator out_edge_it;

void reorder(int* s, int* t) {
	if (*s > *t) {
		int temp = *s;
		*s = *t;
		*t = temp;
	}
}

bool cmp(const std::pair<int, int>& p1, const std::pair<int, int>& p2) {
	return p1.first < p2.first or (p1.first == p2.first and p1.second < p2.second);
}

void testcase() {
	//Read input
	int n; std::cin >> n;
	int m; std::cin >> m;
	graph G(n);
	for (int i = 0; i < m; i++) {
		int u, v; std::cin >> u >> v;
		boost::add_edge(u, v, G);
	}

	//Algorithm
	int numberBiconComp = boost::biconnected_components(G, boost::get(boost::edge_weight, G));

	std::vector<std::vector<edge_desc>> compMap(numberBiconComp);
	edge_it it, end;
	for (boost::tie(it, end) = boost::edges(G); it != end; it++) {
		int nComp = boost::get(boost::edge_weight, G, *it);
		compMap[nComp].push_back(*it);
	}

	std::vector<std::pair<int, int>> result;
	for (auto vec : compMap) {
		if (vec.size() == 1) {
			int s = boost::source(vec[0], G);
			int t = boost::target(vec[0], G);
			reorder(&s, &t);
			result.push_back(std::make_pair(s, t));
		}
	}
	std::sort(result.begin(), result.end(), cmp);
	
	//Print solution!
	std::cout << result.size() << std::endl;
	for (auto p : result) {
		std::cout << p.first << " " << p.second << std::endl;
	}
	
}

int main() {
	std::ios_base::sync_with_stdio(false);

	int t; std::cin >> t;
	for (int i = 0; i < t; i++) {
		testcase();
	}
}


