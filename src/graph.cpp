#include "graph.hpp"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <limits>
#include <mutex>
#include <queue>

#include <unordered_set>
#include <unordered_map>

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

bool graph::is_active(const std::string &name) const {
  const char *s = name.c_str();
  while(*s!='\0' && (*s < '0' || *s > '9')) s++;
  int n = atoi(s);

  return 0 != n % 9;
}

void graph::load_dot(const std::string &userfilepath, const std::string &edgefilepath) {
  std::ifstream userfile{userfilepath};
  std::ifstream edgefile{edgefilepath};

  if ( userfile.fail() ) {
    throw std::ios_base::failure( "error opening user file: " + userfilepath);
  }
  if ( edgefile.fail() ) {
    throw std::ios_base::failure( "error opening edge file: " + edgefilepath);
  }

  auto user_name = get(boost::vertex_name, this->g);
  
  // add users
  std::string line;

  boost::char_separator<char> comma_sep{","};
  while (std::getline(userfile, line)) {
    boost::tokenizer<boost::char_separator<char> > tokenizer{line, comma_sep};

    auto token = begin(tokenizer);

    user uo{*token, is_active(*token)};
    ++token;

    for(int i = 0; i < 10 && end(tokenizer) != token; ++i, ++token) {
      float val{std::stof(*token)};
      if (0.0 < val) {
	uo.add_competence(i, val);
      }
    }

    auto u = add_vertex(this->g);
    user_name[u] = uo.get_name();
    // std::cerr << "add: " << uo.get_name() << std::endl;
    auto pair = std::make_pair(std::move(uo), u);
    this->users[ pair.first.get_name()] = pair;
    // std::cerr << "added: " << pair.first.get_name()<< "add: " << uo.get_name()  << std::endl;
  }

  std::cerr << "how many users: " << users.size() << "." <<std::endl;
  // for (auto it = begin(this->users); it != end(this->users); ++it) {
  //   std::cerr << "users: " << it->first << " \"" << it->second.first.get_name() << "\"" << std::endl;
  // }

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
      throw "Error creating edge between users";
    }
  }
}

user graph::topuser(const unsigned &competence) const {
	std::cerr << "top user for " << competence << std::endl;
	auto it = begin(users);
	user u = std::get<0>(std::get<1>(*it));
	unsigned i = 0;
	std::cerr << i++ << " " << u.get_name() << std::endl;

  for (++it ; end(users) != it; ++it) {
    user t = std::get<0>(std::get<1>(*it));

    if (!t.is_active())
			continue;
		
    if (t.get_competence(competence) > u.get_competence(competence)) {
      std::cerr << i << " update " << u.get_name() << " with " << t.get_name() << std::endl;
      u = t;
    }
    i++;
  }

  return u;
}

user graph::get_user(const Vertex &v) const {
  static auto user_name = get(boost::vertex_name, this->g);
	
  const std::string un = user_name[v];
  std::pair<user, Vertex> up = users.at(un);

  return up.first;
}

Vertex graph::get_vertex(const user &u) const {
  return users.at(u.get_name()).second;
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

  unsigned distance = 0;

  while (p1 != p2) {
    if (p1 > p2)
      p1 = (p1 - 1) / 2;
    else
      p2 = (p2 - 1) / 2;
    ++distance;
  }
  return (distance > totalHighComp) ? 1 : distance / (double) totalHighComp;
}


class DijkstraQueueElement {
  Vertex const *vertex;
  double distance;
 public:
  DijkstraQueueElement(const Vertex &v, double d): vertex(&v), distance(d) {}

  DijkstraQueueElement(const DijkstraQueueElement &other): vertex(other.vertex), distance(other.distance) {}
  
  DijkstraQueueElement& operator=(const DijkstraQueueElement& other){
    vertex = other.vertex;
    distance = other.distance;
    return *this;
  }
  
  bool operator<(const DijkstraQueueElement& o) const {
    return distance > o.distance;
  }
  
  const Vertex& getVertex() const{
    return *vertex;
  }
};

double graph::my_bfs(const Vertex &sourceVertex, const Vertex &destVertex, const unsigned &comp) const {
  auto reputation_argument = get(boost::edge_name, g);
  auto reputation_value = get(boost::edge_weight, g);

  std::unordered_map<Vertex, double> distance;
  std::unordered_map<Vertex, double> reputation;
  std::unordered_set<Vertex> visited;
  
  distance[sourceVertex] = 0.0;
  reputation[sourceVertex] = 1.0;
  std::priority_queue<DijkstraQueueElement> tovisit;
  
  tovisit.push(DijkstraQueueElement(sourceVertex, 0.0));
  
  while ( !tovisit.empty() ) {
    Vertex u = tovisit.top().getVertex();
    tovisit.pop();
    if ( u == destVertex )
      break;
    if ( visited.count(u) > 0 )
      continue;
    
    visited.insert(u);
    
    for (auto e = out_edges(u, this->g); e.first != e.second; ++(e.first)) {
      Vertex currentVertex = target(*e.first, this->g); 

      double re = reputation_value[*e.first];
      if ( re < 0 && currentVertex != destVertex )
	continue; // chain of trust
      double l = similarity(comp, reputation_argument[*e.first]);
      if ( l == 0 )
	continue; // chain of trust
      
      double w = 1 / ((2 + re) * l); // dist_between(u, currentVertex)
      double alt = distance[u] + w;
      if ( distance.count(currentVertex) == 0 || alt < distance[currentVertex] ) {
	distance[currentVertex] = alt;
	reputation[currentVertex] = std::min(reputation[u], re * l);
	if ( visited.count(currentVertex) == 0 ) {
	  tovisit.push( DijkstraQueueElement(currentVertex, distance[currentVertex]) );
	}
      }
    } //for
  } //while

  if ( distance.count(destVertex) == 0 )
    return 0;
  
  return reputation[destVertex];
}




class reputation_cache_key {
  Vertex const *v1;
  Vertex const *v2;
  unsigned comp;
  
public:
  reputation_cache_key(const Vertex &vertex1, const Vertex &vertex2, unsigned competence): 
    v1(&vertex1), v2(&vertex2), comp(competence) {}
  
  bool operator==(const reputation_cache_key &other) const {
    return *v1 == *(other.v1) && *v2 == *(other.v2) && comp == other.comp;
  }
  
  size_t hash_code() const {
    return std::hash<Vertex>()(*v1) ^ std::hash<Vertex>()(*v2) ^ std::hash<double>()(comp);
  }
  
};

namespace std {
  template <>
  struct hash<reputation_cache_key>{
    size_t operator()(const reputation_cache_key &key ) const {
      return key.hash_code();
    }
  };
}


class reputation_cache {
  std::unordered_map<reputation_cache_key, double> cache;
  
public:
  bool contains(const reputation_cache_key &k) const {
    return cache.count(k) > 0;
  }
  
  double get(const reputation_cache_key &k) const {
    return cache.at(k);
  }
  
  double set(const reputation_cache_key &k, double val) {
    cache[k] = val;
  }
};






double graph::my_bfs_cached(const Vertex &u, const Vertex &v, const unsigned &comp, reputation_cache &cache) const {
  // std::cerr << "cached bfs ";
  reputation_cache_key k(u,v,comp);
  if ( cache.contains(k) ) {
    // std::cerr << "cache hint!" << std::endl;
    return cache.get(k);
  }
  // std::cerr << "cache miss :(" << std::endl;
  double rep = my_bfs(u, v, comp);
  cache.set(k, rep);
  return rep;
}


double graph::compute_reputation(const team &t) const {
  
  // get vertexes
  double reputation = 0.0;
  int num = 0;
  auto m = t.get_members();
  reputation_cache cache;
  for (auto it = begin(m); it != end(m); ++it) {
    for (auto it2 = begin(m); it2 != end(m); ++it2) {
      if (std::get<1>(*it2) != std::get<1>(*it)) {
	auto u = std::get<1>(*it);
	auto v = std::get<1>(*it2);
	double currRep = 0;
	try {
	  currRep = my_bfs_cached(get_vertex(u), get_vertex(v), std::get<0>(*it2), cache);
	} catch (int ex) {
	  //std::cerr << "there is no direct path between u and v" << std::endl;
	}
	//std::cerr << "reputation between " << u.get_name() << " and " << v.get_name() << " for " << std::get<0>(*it2) << " = " << currRep << std::endl;
	reputation += currRep;
      }
    }
  }
  
  return reputation/(t.size() * (t.size() - 1));
}

team graph::find_team(const user &suser, const unsigned &scomp, const std::set<unsigned> &taskcomp, const unsigned &search_level) const {
  auto users = possible_users(suser, search_level);

  if (0 == users.size()) throw 0;

  teamgenerator tg;

  for (auto u : users)
    for (auto c : taskcomp)
      if (u.has(c) && u.is_active())
	tg.add(c, u);

  team maxRepTeam;
  double maxRep = std::numeric_limits<double>::lowest();
  bool found = false;
  
  while (tg.has_next()) {
    team t = tg.next();

    if (2 > t.size())
      continue; // skip groups with less than two members

    t.reputation(compute_reputation(t));
    //std::cerr << "reputation for team " << std::endl << t << std::endl;
    if ( maxRep < t.reputation() ) {
      //std::cerr << "new best reputation" << std::endl;
      maxRep = t.reputation();
      maxRepTeam = t;
      found = true;
    }
  }

  if (found) {
    return maxRepTeam;
  } 

  throw std::exception{};
}

std::set<user> graph::possible_users(const user &suser, const unsigned &search_level) const {
  std::set<user> candidates;
  std::vector<std::pair<Vertex, unsigned>> to_process;

  auto t = get_vertex(suser);
  to_process.push_back({t, 0});

  while (!to_process.empty()) {
    std::pair<Vertex, unsigned> u = to_process[0];
    to_process.erase(begin(to_process));

    // std::cerr << "possible_users: to_process.size() == " << to_process.size() << std::endl;

    unsigned n = ++(std::get<1>(u));
    if (search_level > n) {
      // std::cerr << "possible users: " << n << std::endl;
      // std::cerr << "possible users: friends of " << get_user(std::get<0>(u)).get_name() << std::endl;
      
      for (auto eit = out_edges(std::get<0>(u), this->g); eit.first != eit.second; (eit.first)++) {
				auto v = target(*eit.first, this->g);
				// std::cerr << "possible users: " << get_user(v).get_name() << std::endl;	
	
				auto res = candidates.insert(get_user(v));
	
				if (res.second) {
				  // std::cerr << "possible users: inserted" << std::endl;
				  to_process.push_back({std::move(v), n});
				} // else {
				// 	std::cerr << "possible users: already known" << std::endl;
				// }

				// std::cerr << "possible users: candidates.size() == " << candidates.size() << std::endl;
      }
    }
  }

  std::cerr << "possible_users (starting from " << suser.get_name() << " up to " << search_level << "):" << candidates.size() << std::endl;

  return std::move(candidates);
}
