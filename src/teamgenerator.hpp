#pragma once

#include <map>
#include <vector>

#include "team.hpp"
#include "user.hpp"
#include "graph.hpp"

class teamgenerator {
public:
  void add(unsigned const& comp, const user& u);
  bool has_next() const;
  team next();
  void print() const noexcept;
private:
  std::map<unsigned, std::vector<user> > candidates;
  std::map<unsigned, unsigned> counters;
};
