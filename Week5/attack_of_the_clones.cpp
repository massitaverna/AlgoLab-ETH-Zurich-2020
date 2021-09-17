#include <iostream>
#include <vector>
#include <map>
#include <algorithm>

struct Jedi {
  int a;
  int b;
  Jedi(int x, int y) : a(x), b(y) {}
};

typedef std::vector<int> VI;
typedef std::vector<Jedi> VJ;

bool jediCmp(const Jedi& j1, const Jedi& j2) {
  return j1.b < j2.b;
}

void testcase() {
  int n, m; std::cin >> n >> m;
  VJ jedis;
  jedis.reserve(n);
  for (int i = 0; i < n; i++) {
    int a, b; std::cin >> a >> b;
    jedis.push_back(Jedi(a-1, b-1));
  }
  
  int sep_segm;
  std::map<int, int> cover;
  cover[0] = 0;
  for (const Jedi& jedi : jedis) {
    int a = jedi.a;
    int b = jedi.b;
    if (a <= b) {
      if (cover.find(a) == cover.end()) cover[a] = 0;
      cover[a]++;
      if (b != m-1) {
        if (cover.find(b+1) == cover.end()) cover[b+1] = 0;
        cover[b+1]--;
      }
    } else {
      if (cover.find(a) == cover.end()) cover[a] = 0;
      cover[a]++;
      cover[0]++;
      if (cover.find(b+1) == cover.end()) cover[b+1] = 0;
      cover[b+1]--;
    }
  }
  int sum = 0;
  for (auto pos : cover) {
    sum += pos.second;
    if (sum <= 10) {
      sep_segm = pos.first;
      break;
    }
  }
  
  VJ importantJedis;
  for (const Jedi& jedi : jedis) {
    int a = jedi.a;
    int b = jedi.b;
    if (a <= b and a <= sep_segm and sep_segm <= b or
        a > b and (a <= sep_segm or sep_segm <= b)) {
      importantJedis.push_back(jedi);
    }
  }
  
  VJ jedisToUse;
  jedisToUse.reserve(n);
  for (const Jedi& jedi : jedis) {
    int a = jedi.a;
    int b = jedi.b;
    bool useful = true;
    for (const Jedi& impJedi : importantJedis) {
      if (a == impJedi.a and b == impJedi.b) useful = false;
    }
    if (!useful) continue;
    a = (a - sep_segm)%m;
    if (a < 0) a += m;
    b = (b - sep_segm)%m;
    if (b < 0) b += m;
    jedisToUse.push_back(Jedi(a, b));
  }
  std::sort(jedisToUse.begin(), jedisToUse.end(), jediCmp);
  int last_f = 0;
  int max = 0;
  for (const Jedi& jedi : jedisToUse) {
    if (jedi.a > last_f) {
      last_f = jedi.b;
      max++;
    }
  }
  
  for (const Jedi& impJedi : importantJedis) {
    int cnt = 1;
    last_f = (impJedi.b - sep_segm)%m;
    int notBeyond = (impJedi.a - sep_segm)%m;
    if (last_f < 0) last_f += m;
    if (notBeyond <= 0) notBeyond += m;
    for (const Jedi& jedi : jedisToUse) {
      if (jedi.b >= notBeyond) break;
      if (jedi.a > last_f) {
        last_f = jedi.b;
        cnt++;
      }
    }
    max = std::max(max, cnt);
  }
  
  std::cout << max << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  
  int t; std::cin >> t;
  while (t--) testcase();
}
