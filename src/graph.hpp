#pragma once

#include <map>
#include <random>
#include <string>
#include <utility>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/random/mersenne_twister.hpp>

#include "user.hpp"

typedef boost::property<boost::vertex_name_t, std::string> VertexProperty;
typedef boost::property<boost::edge_name_t, unsigned, boost::property<boost::edge_weight_t, float> > EdgeProperty;
typedef boost::adjacency_list<boost::vecS, boost::listS, boost::directedS, VertexProperty, EdgeProperty> Graph;
typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;
//typedef boost::graph_traits<Graph>::edge_descriptor Edge;

class graph {
public:
  // constructors
  graph(const std::string &userfile, const std::string &edgefile, const std::string &type = "dot");

  void stats();

  user topuser(const unsigned &compentece) const;
  user random_user();
private:
  // private fields
  const unsigned seed{1234567};
  std::map<std::string, std::pair<user, Vertex> > users;
  //  std::default_random_engine random;
  boost::random::mt19937 random;

  Graph g;

  // private methods
  void load_dot(const std::string &userfile, const std::string &edgefile);
  user get_user(const Vertex &v);
};
