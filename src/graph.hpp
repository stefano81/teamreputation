#pragma once

#include <map>
#include <random>
#include <set>
#include <string>
#include <utility>


#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/random/mersenne_twister.hpp>

#include "user.hpp"
#include "team.hpp"

typedef boost::property<boost::vertex_name_t, std::string> VertexProperty;
typedef boost::property<boost::edge_name_t, unsigned, boost::property<boost::edge_weight_t, float> > EdgeProperty;
typedef boost::adjacency_list<boost::vecS, boost::listS, boost::directedS, VertexProperty, EdgeProperty> Graph;
typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;

class graph {
public:
  // constructors
  graph(const std::string &userfile, const std::string &edgefile, const std::string &type = "dot");

  void stats();

  user topuser(const unsigned &compentece) const;
  user random_user();
  team find_team(const user &suser, const unsigned &scomp, const std::set<unsigned> &taskcomp, const unsigned &search_level) const;
  team compute_top_users(const std::map<unsigned, user> &top_users, const std::set<unsigned> &competeces);
  double compute_reputation(const team &t) const;

private:
  // private fields
  const unsigned seed = 1234567;
  const unsigned ncomp = 10;
  std::map<std::string, std::pair<user, Vertex> > users;
  boost::random::mt19937 random;

  Graph g;

  // private methods
  void load_dot(const std::string &userfile, const std::string &edgefile);
  user get_user(const Vertex &v) const;
  Vertex get_vertex(const user &u) const;
  std::set<user> possible_users(const user &suser, const unsigned &search_level) const;
  double my_bfs(const Vertex &u, const Vertex &v, const unsigned &comp) const;
  double distance(const unsigned &c1, const unsigned &c2) const;
  double similarity(const unsigned &c1, const unsigned &c2) const;
  bool is_active(const std::string &name) const;
};
