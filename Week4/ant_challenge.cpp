//Time complexity: O(n*log(n) + s*m*log(m))

#include <iostream>
#include <vector>
#include <algorithm>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

#define MAX_WEIGHT 100000

typedef boost::adjacency_list <boost::vecS, boost::vecS, boost::undirectedS,
				boost::no_property, boost::property<boost::edge_index_t, int,
				boost::property<boost::edge_weight_t, int>>> graph;
typedef boost::graph_traits<graph>::edge_descriptor edge_desc;
typedef boost::graph_traits<graph>::edge_iterator edge_it;
typedef boost::graph_traits<graph>::out_edge_iterator out_edge_it;
typedef std::vector<int> VI;
typedef std::vector<VI> MI;



void testcase() {
	//Read input
	int n; std::cin >> n;
	int m; std::cin >> m;
	int s; std::cin >> s;
	int a; std::cin >> a;
	int b; std::cin >> b;
	graph G(n);
	std::vector<graph> graphs(s, graph(n)); // Graphs with specific weights for a species
	VI weights(m, MAX_WEIGHT+1);

	for (int i = 0; i < m; i++) {
		int u; std::cin >> u;
		int v; std::cin >> v;
		edge_desc e = (boost::add_edge(u, v, G)).first;
		boost::put(boost::edge_index, G, e, i);
		for (int j = 0; j < s; j++) {
			int w_s; std::cin >> w_s;
			edge_desc es = (boost::add_edge(u, v, graphs[j])).first;
			boost::put(boost::edge_index, graphs[j], es, i);
			boost::put(boost::edge_weight, graphs[j], es, w_s);
		}
	}
	for (int i = 0; i < s; i++) { // Ignoring hives, since spanning trees from hives are MSTs
		int ignore;
		std::cin >> ignore;
	}	

	//Algorithm
	//Step 1: assign correct weights to edges
	for (int i = 0; i < s; i++) {
		std::vector<edge_desc> mst;
		boost::kruskal_minimum_spanning_tree(graphs[i], std::back_inserter(mst));
		for (edge_desc e : mst) {
			int edge_index = boost::get(boost::edge_index, G, e);
			int w_s = boost::get(boost::edge_weight, graphs[i], e);
			weights[edge_index] = std::min(w_s, weights[edge_index]);
		}
	}

	//Step 2: create a graph with edges belonging to at least one network
	graph G2(n);
	edge_it it, end;
	boost::tie(it, end) = boost::edges(G);
	for ( ; it != end; it++) {
		edge_desc e = *it;
		int index = boost::get(boost::edge_index, G, e);
		int weight = weights[index];
		if (weight <= MAX_WEIGHT) { // If I ever assigned a valid weight to e:
			edge_desc added = (boost::add_edge(boost::source(e, G), boost::target(e, G), G2)).first;
			boost::put(boost::edge_weight, G2, added, weight);
		}
	}

	//Step 3: calculate shortest path from a to b
	std::vector<int> dist_map(n);
	boost::dijkstra_shortest_paths(G2, a, boost::distance_map(boost::make_iterator_property_map(
					dist_map.begin(), boost::get(boost::vertex_index, G))));

	//Print solution!
	int solution = dist_map[b];
	std::cout << solution << std::endl;
	
}


int main() {
	std::ios_base::sync_with_stdio(false);

	int t; std::cin >> t;
	for (int i = 0; i < t; i++) {
		testcase();
	}

	return 0;
}
