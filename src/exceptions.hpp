#ifndef EXCEPTION_HPP
#define EXCEPTION_HPP

#include <exception>
#include <string>

class no_user_exception : public std::runtime_error {
public: 
  no_user_exception(const std::string& what) : std::runtime_error(what){}
};

class no_team_exception : public std::runtime_error {
public: 
  no_team_exception(const std::string& what) : std::runtime_error(what){}
};

class graph_exception : public std::runtime_error {
public: 
  graph_exception(const std::string& what): std::runtime_error(what) {}
};

class cache_exception : public std::runtime_error {
public: 
  cache_exception(const std::string& what): std::runtime_error(what) {}
};

#endif
