#pragma once

#include <map>
#include <string>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/topological_sort.hpp>
#include <boost/graph/visitors.hpp>

#include "user.hpp"

typedef boost::property<boost::vertex_name_t, std::string> VertexProperty;
typedef boost::property<boost::edge_name_t, std::string>/*, boost::property<boost::edge_weight_t, float>>*/ EdgeProperty;
typedef boost::adjacency_list<boost::setS, boost::listS, boost::undirectedS, VertexProperty, EdgeProperty> Graph;
typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;

class graph {
public:
  graph(const std::string &userfile, const std::string &edgefile, const std::string &type = "dot") {
    if (0 == type.compare("dot"))
      load_dot(userfile, edgefile);
  };
  
  //  void stats();
    
private:

  std::map<user, Vertex> users;

  Graph g;

  void load_dot(const std::string &userfile, const std::string &edgefile);

};
