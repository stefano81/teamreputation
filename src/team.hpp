#pragma once

#include <iostream>
#include <map>
#include <set>

#include "user.hpp"

class team {
public:
  class iterator {
  public:
    iterator(const team& t_, bool end = false) {
      if (end)
	ait = t_.members.end();
      else
	ait = t_.members.begin();
    };
    std::pair<unsigned, user> operator*() {std::cerr << "calling operator*" << std::endl;return *ait;};
    iterator operator++() {std::cerr << "calling operator++" << std::endl; ++ait; return *this;};
    bool operator==(const iterator& other) { std::cerr << "calling operator==" << std::endl; return ait == other.ait; };
    bool operator!=(const iterator& other) { std::cerr << "calling operator!=" << std::endl; return !operator==(other); };

  private:
    std::map<unsigned, user>::const_iterator ait;
  };

  team() : is_computed{false}, r{0.0} {};

  void add(const user &u, const unsigned &c) { members[c] = u;};
  double reputation() const;
  std::set<unsigned> competences();
  void reputation(const double& rep) { r = rep; is_computed = true;}
  unsigned size() const;

  std::map<unsigned, user> get_members() {return members;};

  iterator begin() { return iterator(*this); };
  iterator end() {return iterator(*this, true); };
  void print() const noexcept;
private:
  bool is_computed;
  double r;
  std::map<unsigned, user> members;
};
