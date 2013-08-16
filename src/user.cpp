#include "user.hpp"

void user::add_competence(const std::string &name, const float &value) noexcept {
  competences[name] = value;
}
