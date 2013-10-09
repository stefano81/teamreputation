#include "team.hpp"

#include <algorithm>
#include <iostream>

double team::reputation() const {
  if (is_computed)
    return r;
  else
    throw -1;
}

std::set<unsigned> team::competences() {
  std::set<unsigned> comps;

  for_each(std::begin(members), std::end(members),
	   [&comps](std::pair<unsigned, user> m) {
	     comps.insert(std::get<0>(m));
	   });

  return comps;
}

std::map<unsigned, user> team::get_members() const {
  return members;
}

unsigned team::size() const {
  std::set<user> unique_members;
  std::cerr << "computing size" << std::endl;

  for_each(std::begin(members), std::end(members),
	   [&unique_members](std::pair<unsigned, user> up) {
	     std::cerr << std::get<1>(up).get_name() << std::endl;
	     unique_members.insert(std::get<1>(up));
	   });

  return unique_members.size();
}

void team::print() const noexcept {
  for (auto m : members)
    std::cerr << std::get<0>(m) << ' ' << std::get<1>(m).get_name() << std::endl;

  std::cerr << "reputation";
  if (is_computed)
    std::cerr << ": " << r;
  else
    std::cerr << " unknown";

  std::cerr << std::endl;
}
