#include "team.hpp"

//#include <algorithm>
#include <iostream>

double team::reputation() const {
  if (is_computed)
    return r;
  else
    throw -1;
}

std::set<unsigned> team::competences() {
  std::set<unsigned> comps;

  for (auto m : members)
    comps.insert(std::get<0>(m));

  return comps;
}

std::map<unsigned, user> team::get_members() const {
  return members;
}

unsigned team::size() const {
  std::set<user> unique_members;
  std::cerr << "computing size" << std::endl;
  for (auto up : members) {
    std::cerr << std::get<1>(up).get_name() << std::endl;
    unique_members.insert(std::get<1>(up));
  }

  return unique_members.size();
}

void team::print() const noexcept {
  for (auto m : members)
    std::cerr << std::get<0>(m) << ' ' << std::get<1>(m).get_name() << std::endl;

  std::cerr << "reputation: ";
  if (is_computed)
    std::cerr << r;
  std::cerr << std::endl;
}
