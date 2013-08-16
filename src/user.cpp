#include "user.hpp"

void user::add_competence(const std::string &name, const float &value) noexcept {
  this->competences[name] = value;
}

float user::get_competence(const std::string &name) const {
  auto it = this->competences.find(name);
  if (end(this->competences) == it)
    return 0.0;
  else
    return std::get<1>(*it);
}

/*
  std::ostream & operator<<(std::ostream &stream, const user &user) {
    return stream << "user{" << user.id << "}[" << user.competences << "]";
  }
*/
