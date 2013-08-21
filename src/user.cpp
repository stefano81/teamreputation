#include "user.hpp"

#include <exception>
#include <iostream>
#include <sstream>

void user::add_competence(const unsigned &name, const float &value) noexcept {
  //  std::cerr << name << "=" << value << std::endl;
  this->competences[name] = value;
}

bool user::operator==(const user&other) const {
  return other.id == id; // && other.competences == competences;
};


float user::get_competence(const unsigned &name) const {
  auto it = this->competences.find(name);
  if (end(this->competences) == it)
    return 0.0;
  else
    return std::get<1>(*it);
}

bool user::has(const unsigned &name) const {
  return end(competences) != competences.find(name);
}

std::vector<unsigned> user::get_competences() const {
  std::vector<unsigned> v;

  for (auto it = begin(competences); end(competences) != it; ++it)
    v.push_back(std::get<0>(*it));

  return v;
}

std::string user::print_competences() const {
  std::ostringstream buffer;
  bool first = true;

  for (auto c : this->competences) {
    if (!first)
      buffer << ',';
    else
      first = false;

    buffer << std::get<0>(c) << '=' << std::get<1>(c);
  }

  return buffer.str();
}

unsigned user::get_best_competence() const {
  if (0 == competences.size())
    throw std::exception{};

  auto cit = begin(competences);

  unsigned c = std::get<0>(*cit);
  float v = std::get<1>(*cit);

  for (++cit; end(competences) != cit; ++cit) {
    if (v < std::get<1>(*cit))
      c = std::get<0>(*cit);
  }
  
  return c;
}

/*
  std::ostream & user::operator<<(const user &user) {
    return stream << "user{" << user.id << "}[" << user.competences << "]";
  }
*/
