#include "team.hpp"
#include <iostream>
#include <exception>

double team::reputation() const {
  if (is_computed)
    return r;
  else
    throw std::exception{};
}

std::set<unsigned> team::competences() {
  std::set<unsigned> comps;

  for (auto m : members)
    comps.insert(std::get<0>(m));

  return comps;
}

unsigned team::size() const {
  std::set<user> unique_members;

  for (auto up : members) {
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
