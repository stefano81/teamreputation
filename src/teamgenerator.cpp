#include "teamgenerator.hpp"

#include <iostream>

void teamgenerator::print() const noexcept {
  for (auto c : candidates) {
    for (auto u : std::get<1>(c)) {
      std::cerr << '[' << std::get<0>(c) << "] " << u.get_name() << std::endl;
    }
  }
}

void teamgenerator::add(unsigned const& comp, const user& u) {
  std::cerr << "teamgenerator: adding " << u.get_name() << " for " << comp << std::endl;
  if (end(candidates) == candidates.find(comp)) {
    counters[comp] = 0;
  }

  auto l = candidates[comp];
  for (auto li = begin(l); end(l) != li; ++li)
    if (u == *li) return;

  candidates[comp].push_back(u);
}

bool teamgenerator::has_next() {
  for (auto cc :counters)
    if (std::get<1>(cc) != (candidates[std::get<0>(cc)].size() - 1))
      return true;

  return false;
}

team teamgenerator::next() {
  team t;

  bool increment = true;
  for (auto c : counters) {
    unsigned cn = std::get<0>(c), cc = std::get<1>(c);

    std::cerr << cn << '-' << cc << std::endl;

    t.add(candidates[cn][cc], cn);
    if (increment) {
      counters[cn] = (cc + 1) % candidates[cn].size();
      
      increment = 0 == counters[cn];
    }
  }

  return t;
}
