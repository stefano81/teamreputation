#include "teamgenerator.hpp"

void teamgenerator::add(unsigned const& comp, const user& u) {
  if (end(candidates) == candidates.find(comp)) {
    counters[comp] = 0;
  }

  auto l = candidates[comp];
  for (auto li = begin(l); end(l) != li; ++li)
    if (u == *li) return;

  candidates[comp].push_back(u);
}

bool teamgenerator::has_next() {
  //for (auto cit = begin(counters); end(counters) != cit; ++cit) {
  for (auto cc :counters)
    if (std::get<1>(cc) != candidates[std::get<0>(cc)].size())
      return true;

  return false;
}

team teamgenerator::next() {
  team t;

  bool increment = true;
  for (auto c : counters) {
    unsigned cn = std::get<0>(c), cc = std::get<1>(c);

    t.add(candidates[cn][cc], cn);
    if (increment) {
      std::get<1>(c) = (cc + 1) % candidates[cn].size();
      
      //increment = 0 == (c + 1 % candidates[cc].size());
    }
    
  }

  return t;
}
