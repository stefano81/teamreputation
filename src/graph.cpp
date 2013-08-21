#include "graph.hpp"

#include <algorithm>
#include <exception>
#include <fstream>
#include <future>
#include <iostream>
#include <limits>
#include <mutex>
#include <queue>


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
    
    auto e = add_edge(u, v, this->g);
    if (e.second) {
      reputation_argument[e.first] = cid;
      reputation_value[e.first] = ev;
    } else {
      std::cerr << "Error creating edge between " << user_name[u] << " and " << user_name[v] << std::endl;
      throw std::exception{};
    }
  }
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

double graph::similarity(const unsigned &c1, const unsigned &c2) const {
  return 1 - distance(c1, c2);
}

double graph::distance(const unsigned &c1, const unsigned &c2) const {
  unsigned p1{c1};
  unsigned p2{c2};

  unsigned distance;

  while (p1 != p2) {
    if (p1 > p2)
      p1 = (p1 - 1) / 2;
    else
      p2 = (p2 - 1) / 2;
    ++distance;
  }

  return distance / (double) ncomp;
}


double graph::my_bfs(const Vertex &u, const Vertex &v, const unsigned &comp) const {
  std::queue<Vertex> tovisit;
  std::set<Vertex> visited;
  double rep;
  std::map<Vertex, double> reps;
  std::map<Vertex, double> min_w;

  rep = std::numeric_limits<double>::min();
  reps[u] = std::numeric_limits<double>::min();
  min_w[v] = std::numeric_limits<double>::max();

  tovisit.push(u);

  auto reputation_argument = get(boost::edge_name, g);
  auto reputation_value = get(boost::edge_weight, g);

  while (!tovisit.empty()) {
    bool inserted = false;

    Vertex i = tovisit.front();
    tovisit.pop();

    for (auto e = out_edges(i, this->g); e.first != e.second; ++(e.first)) {
      auto j = target(*e.first, this->g);

      if (j == u)
	continue; // cicle with source

      auto re = reputation_value[*e.first];
      auto l = similarity(comp, reputation_argument[*e.first]);
      if (v != j) {
	auto w = 1 / ((2 + re) * l);

	if (min_w[j] < w) {
	  min_w[j] = w;
	  reps[j] = std::min(reps[j], (re * l));
	}
      } else {
	auto cp = reps[source(*e.first, this->g)];
	auto cr = std::min(cp, (re * l));

	rep = std::min(rep, cr);
      }
    }

    if (!inserted)
      break;
  }

  return rep;
}

double graph::compute_reputation(const team &t) {
  // get vertexes
  double reputation = 0.0;
  int num = 0;
  auto m = t.get_members();

  for (auto it = begin(m); it != end(m); ++it) {
    for (auto it2 = begin(m); it2 != end(m); ++it2) {
      if (std::get<1>(*it2) != std::get<1>(*it)) {
	auto u = std::get<1>(*it);
	auto v = std::get<1>(*it2);
	std::cerr << "computing reputation between " << u.get_name() << " and " << v.get_name() << " for " << std::get<0>(*it2) << std::endl;

	try {
	  reputation += my_bfs(get_vertex(u), get_vertex(v), std::get<0>(*it2));
	} catch (int ex) {
	  std::cerr << "there is no direct path between u and v" << std::endl;
	}
      }
    }
  }

  return reputation/(t.size() * (t.size() - 1));
}

team graph::find_team(const user &suser, const unsigned &scomp, const std::set<unsigned> &taskcomp, const unsigned &search_level) {
  auto users = possible_users(suser, search_level);

  if (0 == users.size()) throw 0;

  teamgenerator tg;

  for (auto u : users)
    for (auto c : taskcomp)
      if (u.has(c))
	tg.add(c, u);

  std::vector<team> teams;

  std::vector<std::future<void> > asyncs;

    std::mutex m;

  while (tg.has_next()) {
    team t = tg.next();

    asyncs.push_back(std::async([&,t]{ // capture everying by reference, t by value or t will "disappear" (go out of scope) before the execution of the functor
    	  team t1{t};
	  t1.reputation(compute_reputation(t1));
	  std::lock_guard<std::mutex> _m{m}; // unlocked at the exit of the function
	  teams.push_back(t1);
	})
      );
  }

  for (auto f = begin(asyncs); end(asyncs) != f; ++f)
    f->get();

  if (begin(teams) != end(teams)) {
    sort(begin(teams), end(teams), [](const team &t1, const team &t2) -> bool { return t1.reputation() > t2.reputation();});
   
    return *begin(teams);
  } else {
    return {};
  }
}

std::set<user> graph::possible_users(const user &suser, const unsigned &search_level) {
  std::set<user> candidates;
  std::vector<std::pair<Vertex, unsigned>> to_process;

  std::cerr << "possible_users: starting from " << suser.get_name() << " up to " << search_level << std::endl;

  auto t = get_vertex(suser);
  to_process.push_back({t, 0});

  while (!to_process.empty()) {
    std::pair<Vertex, unsigned> u = to_process[0];
    to_process.erase(begin(to_process));

    std::cerr << "possible_users: to_process.size() == " << to_process.size() << std::endl;

    unsigned n = ++(std::get<1>(u));
    if (search_level > n) {
      std::cerr << "possible users: " << n << std::endl;

      std::cerr << "possible users: friends of " << get_user(std::get<0>(u)).get_name() << std::endl;
      
      for (auto eit = out_edges(std::get<0>(u), this->g); eit.first != eit.second; (eit.first)++) {
	auto v = target(*eit.first, this->g);
	std::cerr << "possible users: " << get_user(v).get_name() << std::endl;	
	
	auto res = candidates.insert(get_user(v));
	
	if (res.second) {
	  std::cerr << "possible users: inserted" << std::endl;
	  to_process.push_back({std::move(v), n});
	} else {
	  std::cerr << "possible users: already known" << std::endl;
	}

	std::cerr << "possible users: candidates.size() == " << candidates.size() << std::endl;
      }
    }
  }

  return std::move(candidates);
}
