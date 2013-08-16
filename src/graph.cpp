#include "graph.hpp"

#include <iostream>
#include <fstream>

#include <boost/tokenizer.hpp>
#include <boost/graph/breadth_first_search.hpp>
#include <boost/format.hpp>
#include <boost/graph/topological_sort.hpp>
#include <boost/graph/visitors.hpp>


void graph::stats() {
  std::cerr << "Vertex count : " << num_vertices(this->g) << std::endl;
  std::cerr << "Edge count : " << num_edges(this->g) << std::endl;
}


void graph::load_dot(const std::string &userfilepath, const std::string &edgefilepath) {
  std::ifstream userfile{userfilepath};
  std::ifstream edgefile{edgefilepath};

  auto user_name = get(boost::vertex_name, this->g);
  auto reputation_argument = get(boost::edge_name, g);
  auto reputation_value = get(boost::edge_weight, g);

  // add users
  std::string line;

  boost::char_separator<char> comma_sep{","};
  while (std::getline(userfile, line)) {
    //std::cerr << line << std::endl;

    boost::tokenizer<boost::char_separator<char> > tokenizer{line, comma_sep};

    auto token = begin(tokenizer);

    user uo{*(token++)};
    
    for(int i = 0; i < 10 && end(tokenizer) != token; ++i, ++token) {
      float val{std::stof(*token)};

      if (0.0 < val) {
	uo.add_competence({"c"+std::to_string(i)}, val);
      }
    }

    auto u = add_vertex(this->g);
    user_name[u] = uo.get_name();
    this->users[uo.get_name()] = std::make_pair(uo, u);
  }

  std::cerr << "print vertices" << std::endl;
  for (auto vl = vertices(this->g); vl.first != vl.second; ++vl.first) {
    std::cerr << user_name[*(vl.first)] << std::endl;
  }
  
  // add edges
  boost::char_separator<char> eq_sep{"="};

  while (std::getline(edgefile, line)) {
    boost::tokenizer<boost::char_separator<char> > user_tokenizer{line, comma_sep};

    auto tokit = begin(user_tokenizer);

    auto u = get<1>(this->users[*(tokit)]);
    auto v = get<1>(this->users[*(++tokit)]);
    
    std::cerr << user_name[u] << ' ' << user_name[u] << std::endl;
    auto e = add_edge(u, v, this->g);
    if (e.second) {
      boost::tokenizer<boost::char_separator<char> > edge_tokenizer{*(++tokit), eq_sep};
      auto etoit = begin(edge_tokenizer);

      unsigned int cid =  std::stoi((*etoit).substr(1));
      float ev = std::stof(*(++etoit));

      reputation_argument[e.first] = {"c"+cid};
      reputation_value[e.first] = ev;
    }
  }
}

user graph::get_topuser(const std::string &compentece) const {
  user u;
  for (auto it = begin(users); end(users) != it; ++it) {
    user t = std::get<0>(std::get<1>(*it));

    if (t.get_
  }
}
