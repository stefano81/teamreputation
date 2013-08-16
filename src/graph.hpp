#pragma once

#include <map>
#include <string>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/topological_sort.hpp>
#include <boost/graph/visitors.hpp>


#include "user"

typedef boost::property<boost::vertex_name_t, std::string> VertexProperty;
typedef boost::property<boost::edge_name_t, std::string, boost::property<boost::edge_weight_t, float>> EdgeProperty;
typedef boost::adjacency_list<boost::setS, boost::listS, boost::undirectedS, VertexProperty, EdgeProperty> graph_t;
typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;

class graph {
public:
  graph(const std::string &filename, const std::string &type = "dot") {
    if (0 == type.compare("dot"))
      load_dot(filename);
    if (0 == type.compare("csv"))
      load_csv(filename);
  };
  
  void stats();
    
private:
  std::map<user, Vertex> users;

  graph_t g;

  void load(const std::string &filename);

  };
