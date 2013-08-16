#include "graph.hpp"

#include <iostream>
//#include <util>

#include <boost/tokenizer.hpp>
#include <boost/graph/breadth_first_search.hpp>

/* void graph::stats() {
   std::cerr << "Vertex count : " << num_vertices(this->g) << std::endl;
   std::cerr << "Edge count : " << num_edges(this->g) << std::endl;
   }*/

void load_dot(const std::string &userfilepath, const std::string &edgefilepath) {
  std::ifstream userfile{userfilepath}, edgefile{edgefilepath};

  auto user_name = get(boost::vertex_name, this->g);
  auto reputation_argument = get(boost::edge_name, g);
  auto reputation_value = get(boost::edge_weight, g);

  // add users
  std::string line;

  boost::char_separator<char> comma_sep{","};
  while (std::getline(userfile, line)) {
    std::cerr << line << std::endl;

    boost::tokenizer<boost::char_separator<char> > tokenizer{line, comma_sep};

    //auto token = begin(tonekizer);
  }
  
  // add edges
  //boost::char_separator<char> eq_sep{"="};
}

/*void graph::load_csv(const std::string &filename) {
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

