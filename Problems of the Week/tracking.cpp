#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

typedef std::vector<int> VI;
typedef std::vector<VI> MI;
typedef boost::adjacency_list<boost::vecS,
	boost::vecS,
	boost::undirectedS,
	boost::no_property,
	boost::property<boost::edge_weight_t, int,
	boost::property<boost::edge_index_t, int>>
	> weighted_graph;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor vertex_desc;
typedef boost::graph_traits<weighted_graph>::edge_descriptor edge_desc;
typedef boost::graph_traits<weighted_graph>::edge_iterator edge_it;

struct Edge {
	int id;
	int normalWeight = -1;
	int riverWeight = -1;
};


void swap (int* a, int* b) {
	int temp = *a;
	*a = *b;
	*b = temp;
}


int riversOnPath(int vertex, const std::vector<vertex_desc>& pred_map, const std::vector<std::set<int>>& rivers, VI& otherRivers, int y) {
	if (vertex == y) return 0;
	if (otherRivers[vertex] != -1) return otherRivers[vertex];
	int isRiver = 0;
	int pred = pred_map[vertex];
	if (rivers[vertex].find(pred) != rivers[vertex].end()) isRiver = 1;
	otherRivers[vertex] = isRiver + riversOnPath(pred, pred_map, rivers, otherRivers, y);
	return otherRivers[vertex];
}



void testcase() {
	//Read input
	int n, m, k, x, y;
	std::cin >> n >> m >> k >> x >> y;
	std::vector<std::vector<Edge>> inputEdges(n, std::vector<Edge>(n, Edge()));
	MI existingEdges(n, VI());

	int counter = 0;
	for (int i = 0; i < m; i++) {
		int u, v, w, r;
		std::cin >> u >> v >> w >> r;
		if (v < u) swap(&u, &v);
		if (!r && inputEdges[u][v].normalWeight == -1) {
			int rW = inputEdges[u][v].riverWeight;
			if (rW == -1) {
				inputEdges[u][v].id = counter;
				inputEdges[u][v].normalWeight = w;
				existingEdges[u].push_back(v);
				counter++;
			}
			else if (rW != -1 && w < rW) {
				inputEdges[u][v].normalWeight = w;
			}
		}
		else if (!r && inputEdges[u][v].normalWeight != -1) {
			if (w < inputEdges[u][v].normalWeight) {
				inputEdges[u][v].normalWeight = w;
			}
		}
		else if (r && inputEdges[u][v].riverWeight == -1) {
			if (inputEdges[u][v].normalWeight == -1) {
				inputEdges[u][v].id = counter;
				existingEdges[u].push_back(v);
				counter++;
			}
			inputEdges[u][v].riverWeight = w;
		}
		else if (r && inputEdges[u][v].riverWeight != -1) {
			if (w < inputEdges[u][v].riverWeight) {
				inputEdges[u][v].riverWeight = w;
			}
		}
	}

	weighted_graph normalGraph(n);
	weighted_graph riverGraph(n);
	std::vector<std::set<int>> rivers(n);
	VI edgeIsOnRiver(m);

	for (int i = 0; i < n; i++) {
		for (int j : existingEdges[i]) {
			if (inputEdges[i][j].normalWeight != -1) {
				edge_desc e = (boost::add_edge(i, j, normalGraph)).first;
				boost::put(boost::edge_weight, normalGraph, e, inputEdges[i][j].normalWeight);
				boost::put(boost::edge_index, normalGraph, e, inputEdges[i][j].id);
			} else {
				edge_desc e = (boost::add_edge(i, j, normalGraph)).first;
				boost::put(boost::edge_weight, normalGraph, e, inputEdges[i][j].riverWeight);
				boost::put(boost::edge_index, normalGraph, e, inputEdges[i][j].id);
			}
			if (inputEdges[i][j].riverWeight != -1) {
				edge_desc e = (boost::add_edge(i, j, riverGraph)).first;
				boost::put(boost::edge_weight, riverGraph, e, inputEdges[i][j].riverWeight);
				boost::put(boost::edge_index, riverGraph, e, inputEdges[i][j].id);
				if (inputEdges[i][j].normalWeight == -1) {
					rivers[i].insert(j);
					rivers[j].insert(i);
				}
				edgeIsOnRiver[inputEdges[i][j].id] = 1;
			} else {
				edge_desc e = (boost::add_edge(i, j, riverGraph)).first;
				boost::put(boost::edge_weight, riverGraph, e, inputEdges[i][j].normalWeight);
				boost::put(boost::edge_index, riverGraph, e, inputEdges[i][j].id);
			}
		}
	}

	//Algorithm
	VI distancesFromX(n);
	std::vector<vertex_desc> pred_map_X(n);
	VI otherRiversBeforehand(n, -1);
	
	boost::dijkstra_shortest_paths(normalGraph, x, boost::distance_map(boost::make_iterator_property_map(
					distancesFromX.begin(), boost::get(boost::vertex_index, normalGraph)))
					.predecessor_map(boost::make_iterator_property_map(pred_map_X.begin(),
					boost::get(boost::vertex_index, normalGraph))));
	VI otherRivers(n, -1);
	VI distancesFromY(n);
	std::vector<vertex_desc> pred_map(n);
	boost::dijkstra_shortest_paths(normalGraph, y, boost::distance_map(boost::make_iterator_property_map(
					distancesFromY.begin(), boost::get(boost::vertex_index, normalGraph)))
					.predecessor_map(boost::make_iterator_property_map(pred_map.begin(),
					boost::get(boost::vertex_index, normalGraph))));

	int d = 2147483647;
	edge_it it, end;
	for (boost::tie(it,end) = boost::edges(riverGraph); it != end; it++) {
		edge_desc e = *it;
		int edge_index = boost::get(boost::edge_index, riverGraph, e);
		if (!edgeIsOnRiver[edge_index]) continue;
		int weight = boost::get(boost::edge_weight, riverGraph, e);
		int a = boost::source(e, riverGraph);
		int b = boost::target(e, riverGraph);

		int lA = k - riversOnPath(a, pred_map, rivers, otherRivers, y) - riversOnPath(a, pred_map_X, 
						rivers, otherRiversBeforehand, x);
		int lB = k - riversOnPath(b, pred_map, rivers, otherRivers, y) - riversOnPath(a, pred_map_X, 
						rivers, otherRiversBeforehand, x);
		if (lA < 0) lA = 0;
		else if (lA%2 != 0) lA++;
		if (lB < 0) lB = 0;
		if (lB%2==0) lB++;

		d = std::min(d, distancesFromX[a] + lA*weight + distancesFromY[a]);
		d = std::min(d, distancesFromX[a] + lB*weight + distancesFromY[b]);

		lA = k - riversOnPath(a, pred_map, rivers, otherRivers, y) - riversOnPath(b, pred_map_X, 
						rivers, otherRiversBeforehand, x);
		lB = k - riversOnPath(b, pred_map, rivers, otherRivers, y) - riversOnPath(b, pred_map_X, 
						rivers, otherRiversBeforehand, x);
		if (lB < 0) lB = 0;
		else if (lB%2 != 0) lB++;
		if (lA < 0) lA = 0;
		if (lA%2 == 0) lA++;
		

		d = std::min(d, distancesFromX[b] + lB*weight + distancesFromY[b]);
		d = std::min(d, distancesFromX[b] + lA*weight + distancesFromY[a]);

	}

	//Print solution!
	std::cout << d << std::endl;
}


int main() {
	std::ios_base::sync_with_stdio(false);

	int t; std::cin >> t;
	for (int i = 0; i < t; i++) {
		testcase();
	}

	return 0;
}
