#pragma once

#include <map>
#include <string>
#include <utility>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_traits.hpp>

#include "user.hpp"

typedef boost::property<boost::vertex_name_t, std::string> VertexProperty;
typedef boost::property<boost::edge_name_t, std::string, boost::property<boost::edge_weight_t, float> > EdgeProperty;
typedef boost::adjacency_list<boost::setS, boost::listS, boost::directedS, VertexProperty, EdgeProperty> Graph;
typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;

class graph {
public:
  graph(const std::string &userfile, const std::string &edgefile, const std::string &type = "dot") {
    if (0 == type.compare("dot"))
      load_dot(userfile, edgefile);
  };
  
  void stats();

  user get_topuser(const std::string &compentece) const;
private:
  // private fields
  std::map<std::string, std::pair<user, Vertex> > users;

  Graph g;

  // private methods
  void load_dot(const std::string &userfile, const std::string &edgefile);

};
