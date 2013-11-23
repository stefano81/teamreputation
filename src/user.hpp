#pragma once

#include <map>
#include <string>
#include <ostream>
#include <utility>
#include <vector>
#include <iostream>

using namespace std::rel_ops;

class user {
public:
  user(const std::string &name = "", const bool &active = true) : id{name}, _active{active} {
		//std::cerr << "user: " << name << " is " << (active?"":"not ") << "active" << std::endl;
	}
  
  void add_competence(const unsigned &name, const float &value) noexcept;
  float get_competence(const unsigned &name) const;
  std::vector<unsigned> get_competences() const;
  bool has(const unsigned &name) const;

  std::string get_name() const noexcept { return this->id; }
  std::string print_competences() const;
  unsigned get_best_competence() const;
  
  bool is_active() const noexcept {return this->_active;}

  bool operator==(const user& other) const;
  bool operator<(const user& other) const { return id < other.id;}
	user& operator=(const user& other) {
		
	}
private:
  bool _active;
  std::string id;
  std::map<unsigned, float> competences;
};



