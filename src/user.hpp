#pragma once

#include <map>
#include <string>

class user {
public:
  user(const std::string &name) : id{name} {}
  
  void add_compentece(const std::string &name, const float &value) noexcept;
private:
  std::string id;
  std::map<std::string, float> competences;
}
