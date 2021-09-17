#include <iostream>
#include <vector>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>

#define MAX_COST 100
#define MAX_TIME 100001

typedef std::vector<int> VI;
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor,
                boost::property <boost::edge_weight_t, long>>>>> graph;

typedef boost::graph_traits<graph>::edge_descriptor edge_desc;

class edge_adder {
 graph &G;

 public:
  explicit edge_adder(graph &G) : G(G) {}
  void add_edge(int from, int to, long capacity, long cost) {
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    auto w_map = boost::get(boost::edge_weight, G);
    const edge_desc e = boost::add_edge(from, to, G).first;
    const edge_desc rev_e = boost::add_edge(to, from, G).first;
    c_map[e] = capacity;
    c_map[rev_e] = 0;
    r_map[e] = rev_e;
    r_map[rev_e] = e;
    w_map[e] = cost;
    w_map[rev_e] = -cost;
  }
};

struct Request {
  int s;
  int t;
  int d;
  int a;
  int p;
  Request (int a, int b, int c, int d, int e) : s(a), t(b), d(c), a(d), p(e) {}
};

void testcase() {
  int N, S; std::cin >> N >> S;
  VI l(S);
  long carTotal = 0;
  for (int i = 0; i < S; i++) {
    std::cin >> l[i];
    carTotal += l[i];
  }
  
  std::vector<Request> requests;
  std::vector<std::set<int>> times(S);
  for (int i = 0; i < N; i++) {
    int s, t, d, a, p; std::cin >> s >> t >> d >> a >> p;
    s--; t--;
    requests.push_back(Request(s, t, d, a, p));
    times[s].insert(d);
    times[t].insert(a);
  }
  
  std::map<int, int> mapping;
  int cnt = 0;
  for (int i = 0; i < S; i++) {
    times[i].insert(0);
    times[i].insert(MAX_TIME-1);
    for (int time : times[i]) {
      mapping.insert({time+MAX_TIME*i, cnt});
      cnt++;
    }
  }
  
  graph G(cnt);
  edge_adder adder(G);
  int source = boost::add_vertex(G);
  int sink = boost::add_vertex(G);
  
  for (int i = 0; i < S; i++) {
    int end = *(times[i].rbegin());
    for (auto it = times[i].begin(); *it != end;) {
      int d = *it;
      int a = *(++it);
      int u = mapping[d + MAX_TIME*i];
      int v = mapping[a + MAX_TIME*i];
      adder.add_edge(u, v, carTotal, MAX_COST*(a-d));
    }
    int begin = mapping[MAX_TIME*i];
        end   = mapping[MAX_TIME-1 + MAX_TIME*i];
    adder.add_edge(source, begin, l[i], 0);
    adder.add_edge(end, sink, carTotal, 0);
  }
  for (const Request& r : requests) {
    int u = mapping[r.d + MAX_TIME*r.s];
    int v = mapping[r.a + MAX_TIME*r.t];
    adder.add_edge(u, v, 1, MAX_COST*(r.a-r.d) - r.p);
  }
  
  boost::successive_shortest_path_nonnegative_weights(G, source, sink);
  long cost = boost::find_flow_cost(G);
  long profit = carTotal*(MAX_TIME-1)*MAX_COST - cost;
  
  std::cout << profit << std::endl;
}


int main() {
  std::ios_base::sync_with_stdio(false);
  
  int t; std::cin >> t;
  while (t--) testcase();
}
