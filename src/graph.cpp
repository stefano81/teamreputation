#include "graph.hpp"

#include <exception>
#include <iostream>
#include <fstream>

#include <boost/format.hpp>

#include <boost/graph/breadth_first_search.hpp>
#include <boost/graph/random.hpp>
#include <boost/graph/topological_sort.hpp>
#include <boost/graph/visitors.hpp>

#include <boost/tokenizer.hpp>

graph::graph(const std::string &userfile, const std::string &edgefile, const std::string &type) : random{seed} {
    if (0 == type.compare("dot"))
      this->load_dot(userfile, edgefile);
}

void graph::stats() {
  std::cerr << "Vertex count : " << num_vertices(this->g) << std::endl;
  std::cerr << "Edge count : " << num_edges(this->g) << std::endl;
}

void graph::load_dot(const std::string &userfilepath, const std::string &edgefilepath) {
  std::ifstream userfile{userfilepath};
  std::ifstream edgefile{edgefilepath};

  auto user_name = get(boost::vertex_name, this->g);
  
  // add users
  std::string line;

  boost::char_separator<char> comma_sep{","};
  while (std::getline(userfile, line)) {
    std::cerr << line << std::endl;

    boost::tokenizer<boost::char_separator<char> > tokenizer{line, comma_sep};

    auto token = begin(tokenizer);

    user uo{*(token++)};
    
    for(int i = 0; i < 10 && end(tokenizer) != token; ++i, ++token) {
      float val{std::stof(*token)};
      if (0.0 < val) {
	uo.add_competence(i, val);
      }
    }

    auto u = add_vertex(this->g);
    user_name[u] = uo.get_name();
    this->users[uo.get_name()] = std::make_pair(uo, u);
  }

  /*  std::cerr << "#print vertices" << std::endl;
  for (auto vl = vertices(this->g); vl.first != vl.second; ++vl.first) {
    std::cerr << "#" << user_name[*(vl.first)] << '['
	      << std::get<0>(this->users[user_name[*(vl.first)]]).print_competences() << ']' << std::endl;
	      }*/

  // add edges
  boost::char_separator<char> eq_sep{"="};
  auto reputation_argument = get(boost::edge_name, g);
  auto reputation_value = get(boost::edge_weight, g);

  while (std::getline(edgefile, line)) {
    boost::tokenizer<boost::char_separator<char> > user_tokenizer{line, comma_sep};

    auto tokit = begin(user_tokenizer);

    auto u = get<1>(this->users[*(tokit)]);
    auto v = get<1>(this->users[*(++tokit)]);

    boost::tokenizer<boost::char_separator<char> > edge_tokenizer{*(++tokit), eq_sep};
    auto etoit = begin(edge_tokenizer);
    
    unsigned int cid =  std::stoi((*etoit).substr(1));
    float ev = std::stof(*(++etoit));
    
    //std::cerr << user_name[u] << ' ' << user_name[v] << std::endl;
    auto e = add_edge(u, v, this->g);
    if (e.second) {
      reputation_argument[e.first] = cid;
      reputation_value[e.first] = ev;
    } else {
      std::cerr << "Error creating edge between " << user_name[u] << " and " << user_name[v] << std::endl;
      throw std::exception{};
    }
  }
  /*
  // print edges
  std::cerr << "#print edges" << std::endl;
  for (auto el = edges(this->g); el.first != el.second; ++el.first) {
    auto e = *(el.first);
    std::cerr << "#" << user_name[source(e, this->g)] << " - (" << reputation_argument[e] << "=" << reputation_value[e] << ") - " << user_name[target(e, this->g)] << std::endl;
  }
  */
}

user graph::topuser(const unsigned &competence) const {
  user u;
  for (auto it = begin(users); end(users) != it; ++it) {
    user t = std::get<0>(std::get<1>(*it));

    float ct{t.get_competence(competence)};
    float ut{u.get_competence(competence)};
    //    std::cerr << ct << " > " << ut << std::endl;
    if (ct > ut) {
      u = t;
    }
  }

  return u;
}

user graph::get_user(const Vertex &v) {
  auto user_name = get(boost::vertex_name, this->g);
  std::string un = user_name[v];
  std::pair<user, Vertex> up = users[un];

  return up.first;
}

user graph::random_user() {
  Vertex v = random_vertex(this->g, random);
  return get_user(v);
}
