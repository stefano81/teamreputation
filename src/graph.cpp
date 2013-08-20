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

#include "teamgenerator.hpp"

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

Vertex graph::get_vertex(const user &u) {
  return users[u.get_name()].second;
}

user graph::random_user() {
  Vertex v = random_vertex(this->g, random);
  return get_user(v);
}

void graph::compute_reputation(team &t) {
  // get vertexes
  float reputation = 0.0;
  int num = 0;
  
  for (auto it = t.begin(); it != t.end(); ++it) {
    for (auto it2 = t.begin(); it2 != t.end(); ++it2) {
      if (*it2 == *it) {
	auto u = std::get<1>(*it);
	auto v = std::get<1>(*it2);
	std::cerr << "computing reputation between ";
	std::cerr << u.get_name() << " and " << v.get_name() << " for " << std::get<0>(*it2) << std::endl;
      } else {
	std::cerr << "skipping ";// << std::get<1>(*it).get_name() << " and " << std::get<1>(*it2).get_name() << std::endl;
      }
    }
  }

}


team graph::find_team(const user &suser, const unsigned &scomp, const std::set<unsigned> &taskcomp, const unsigned &search_level) {
  auto users = possible_users(suser, search_level);
  teamgenerator tg;

  for (auto v : users)
    for (auto c : taskcomp) {
      auto u = get_user(v);
      if (u.has(c))
	tg.add(c, u);
    }

  std::vector<team> teams;
  while (tg.has_next()) {
    team t = tg.next();
    std::cerr << "computing reputation for team" << std::endl;
    compute_reputation(t);

    teams.push_back(t);
  }

  if (begin(teams) != end(teams)) {
    sort(begin(teams), end(teams), [](const team &t1, const team &t2) -> bool { return t1.reputation() > t2.reputation();});
   
    return *begin(teams);
  } else {
    return {};
  }
}

std::set<Vertex> graph::possible_users(const user &suser, const unsigned &search_level) {
  std::set<Vertex> candidates;
  std::vector<std::pair<Vertex, unsigned>> to_process;

  auto t = get_vertex(suser);
  to_process.push_back({t, 0});

  while (!to_process.empty()) {
    auto u = to_process[0];
    to_process.erase(begin(to_process));

    unsigned n = ++(std::get<1>(u));
    if (search_level > n) {
      for (auto eit = out_edges(std::get<0>(u), this->g); eit.first != eit.second; (eit.first)++) {
	auto v = target(*eit.first, this->g);

	if (std::get<1>(candidates.insert(v))) {
	  to_process.push_back({std::move(v), n});
	}
      }
    }
  }

  return std::move(candidates);
}
