#include "graph.hpp"

#include <iostream>
#include <utils>

#include <boost/tokenizer.hpp>

#include <boost/graph/breadth_first_search.hpp>

void graph::stats() {
  std::cerr << "Vertex count : " << num_vertices(g) << std::endl;
  std::cerr << "Edge count : " << num_edges(g) << std::endl;
}

void graph::load_dot(const std::string &filename) {
  auto 
}

void graph::load_csv(const std::string &filename) {
  /*  std::map<std::string, Vertex> users;
  std::ifstream file(filename);

  auto user_name = get(boost::vertex_name, g);
  auto reputation_argument = get(boost::edge_name, g);
  auto reputation_value = get(boost::edge_weight, g);

  std::string line;

  while(std::getline(file, line)) {
    auto a1 = line.substr(0, line.find(';'));
    auto title = line.substr(line.find(';') + 1, line.rfind(';') - (line.find(';') + 1));
    auto a2 = line.substr(line.rfind(';') + 1);

    //std::cout << a1 << " worked with " << a2 << " on " << title << std::endl;
    
    Vertex u, v;

    if (users.end() == users.find(a1)) {
      u = add_vertex(g);
      user_name[u] = a1;
      users[a1] = u;
    } else
      u = users[a1];
    
    if (users.end() == users.find(a2)) {
      v = add_vertex(g);
      user_name[v] = a2;
      users[a2] = v;
    } else
      u = users[a2];

    auto e = add_edge(u, v, g);
    if (e.second) {
      reputation_argument[e.first] = title;
      reputation_value[e.first] = 0.0;
    }
  }

  return std::move(g);*/
}

