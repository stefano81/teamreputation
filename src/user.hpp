#pragma once

#include <map>
#include <string>
#include <ostream>
#include <vector>

class user {
public:
  user(const std::string &name="") : id{name} {}
  
  void add_competence(const unsigned &name, const float &value) noexcept;
  float get_competence(const unsigned &name) const;
  std::vector<unsigned> get_competences() const;
  bool has(const unsigned &name) const;

  std::string get_name() const { return this->id; };
  std::string print_competences() const;
  unsigned get_best_competence() const;

  bool operator==(const user&other) const {
    return other.id == id && other.competences ==competences;
  };
private:
  std::string id;
  std::map<unsigned, float> competences;
};


