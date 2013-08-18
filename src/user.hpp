#pragma once

#include <map>
#include <string>
#include <ostream>

class user {
public:
  user(const std::string &name="") : id{name} {};
  //user(user &&other) : id{std::move(other.id)}, competences{std::move(other.competences)} {};
  
  void add_competence(const unsigned &name, const float &value) noexcept;
  float get_competence(const unsigned &name) const;

  std::string get_name() { return this->id; };
  std::string print_competences() const;
  unsigned get_best_competence() const;

  //std::ostream & operator<<(std::ostream &stream, const user &user);
private:
  std::string id;
  std::map<unsigned, float> competences;
};


