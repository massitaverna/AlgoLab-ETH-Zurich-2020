#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include <cmath>

struct Triplet {
  int index;
  long d;
  long t;
};

bool dCmp(const Triplet& p1, const Triplet& p2) {
  return p1.d < p2.d;
}

bool tCmp(const Triplet& p1, const Triplet& p2) {
  return p1.t < p2.t;
}

bool tripletToDistanceCmp(const Triplet& tr, long l) {
  return tr.d < l;
}

void testcase() {
  //Read input
  int n; std::cin >> n;
  int m; std::cin >> m;
  long D; std::cin >> D;
  long T; std::cin >> T;

  std::vector<Triplet> moves;
  moves.reserve(n);

  for (int i = 0; i < n; i++) {
    long d, t; std::cin >> d >> t;
    Triplet tr;
    tr.index = i;
    tr.d = d;
    tr.t = t;
    moves.push_back(tr);
  }

  std::vector<long> extras(m+1);
  extras[0] = 0;
  for (int i = 1; i < m+1; i++) {
    std::cin >> extras[i];
  }

  //Algorithm

  //1 - Split
  std::vector<Triplet> S1;
  std::vector<Triplet> S2;
  S1.reserve(n); S2.reserve(n);
  for (int i = 0; i < n; i++) {
    if (i < n/2) {
      S1.push_back(moves[i]);
    } else {
      S2.push_back(moves[i]);
    }
  }

  // Binary search on potion gulps (extras[])
  int start = 0;
  int end = m;
  int pos;
  bool res = false;

  while (m == 0 or end >= start) {
    pos = (start+end)/2;

    //2 - List
    std::vector<Triplet> L1;
    std::vector<Triplet> L2;
    L1.reserve(std::pow(2,S1.size()));
    L2.reserve(std::pow(2,S2.size()));

    for (int i = 0; i < 1 << S1.size(); i++) {
      long d = 0, t = 0;
      for (int j = 0; j < S1.size(); j++) {
        if (i & 1<<j) {
          d += S1[j].d + extras[pos];
          t += S1[j].t;
        }
      }
      Triplet tr;
      tr.index = i;
      tr.d = d;
      tr.t = t;
      L1.push_back(tr);
    }
    for (int i = 0; i < 1 << S2.size(); i++) {
      long d = 0, t = 0;
      for (int j = 0; j < S2.size(); j++) {
        if (i & 1<<j) {
          d += S2[j].d + extras[pos];
          t += S2[j].t;
        }
      }
      Triplet tr;
      tr.index = i;
      tr.d = d;
      tr.t = t;
      L2.push_back(tr);
    }

    //3 - Sort L2
    std::vector<Triplet> L2d = L2;
    std::vector<Triplet> L2t = L2;  
    std::sort(L2d.begin(), L2d.end(), dCmp);
    std::sort(L2t.begin(), L2t.end(), tCmp);
    for (int i = 0; i < L2d.size(); i++) {
      L2d[i].index = i;
    }

    //4 - Build times[]
    std::vector<Triplet> times;
    times.reserve(L2d.size());
    times.push_back(L2t[0]);
    int t_index = 0;
    for (int i = 1; i < L2d.size(); i++) {
      if (times[i-1].d >= L2d[i].d) {
        times.push_back(times[i-1]);
      } else {
        while (L2t[t_index].d < L2d[i].d) {
          t_index++;
        }
        times.push_back(L2t[t_index]);
      }
    }
    //5 - Find solution
    res = false;
    for (auto l1 : L1) {
      Triplet l2;
      l2.d = D - l1.d;
      l2.t = T - l1.t;
      auto lower = std::lower_bound(L2d.begin(), L2d.end(), l2.d, tripletToDistanceCmp);
      if (lower == L2d.end()) continue;
      if (times[(*lower).index].t < l2.t) {
        res = true;
        break;
      }
    }
  
    if (m == 0) {
      if (!res) {
        std::cout << "Panoramix captured" << std::endl;
        return;
      } else {
        std::cout << 0 << std::endl;
        return;
      }
    }
    else {
      if (res) {
        end = pos-1;
      } else {
        start = pos+1;
      }
    }
  }

  //Print solution!
  if (res) std::cout << pos << std::endl;
  else if (pos < m) std::cout << pos+1 << std::endl;
  else std::cout << "Panoramix captured" << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  
  int t; std::cin >> t;
  for (int i = 0; i < t; i++) {
    testcase();
  }
}
