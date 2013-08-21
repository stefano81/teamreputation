#include <limits>
#include <set>
#include <string>
#include <random>
#include <unordered_set>

#include "graph.hpp"

constexpr unsigned taskSkills{5};
constexpr unsigned skilln{10};
constexpr unsigned seed{1234567890};

std::set<unsigned> get_compset(const unsigned avoid, const unsigned setsize) {
  std::set<unsigned> set;
  std::default_random_engine generator{seed};
  std::uniform_int_distribution<unsigned> competences{0, skilln};

  while (set.size() < setsize) {
    unsigned x = competences(generator);
    if (avoid != x) {
      set.insert(x);
      std::cerr << x << ' ' << set.size() << std::endl;
    }
  }

  return set;
}

void testUserCentricCSV(graph& g, const unsigned search_level, const unsigned iteration) {
  std::cerr << iteration << " up to " << search_level <<  std::endl;
  std::unordered_set<unsigned> all_comp;
  std::map<unsigned, user> top_users;

  for (int i = 0; i < 10; ++i) {
    all_comp.insert(i);
    user u = g.topuser(i);

    std::cout << "c" << i << " => " << u.get_name() << std::endl;

    top_users[i] = u;
  }

  std::cout << "teamNumber, teamRep, best_max,  best_avg,only_best" << std::endl;

  for (auto i = 0; i < iteration; ++i) {
   user suser = g.random_user();
   unsigned scomp = suser.get_best_competence();

   std::set<unsigned> taskcomp = get_compset(scomp, taskSkills-1);

   try {
     team steam = g.find_team(suser, scomp, taskcomp, search_level);

     std::cout << steam.size() << ',' << steam.reputation() << ',';

     double wtur = std::numeric_limits<double>::min(),
       wtut = 0.0;

     team otu;
     for (auto uc : steam.get_members()) {
       if (std::get<1>(uc) == suser)
	 continue;

       user bc = top_users[std::get<0>(uc)];
       team tt{steam};
       tt.add(bc, std::get<0>(uc));
       otu.add(bc, std::get<0>(uc));
       
       double tr = g.compute_reputation(tt);

       wtut +=tr;

       wtur = std::max(wtur, tr);
     }

     std::cout << wtur << ',' << (wtur / steam.size()) << ',' << g.compute_reputation(otu) << std::endl;

   } catch (int v) {
     std::cerr << "skipping iteration " << i << " because no friends" << std::endl;
   } catch (std::exception e) {
     std::cerr << "skipping iteration " << i << " because no team is possible" << std::endl;
   }
  }
}


int main(int argc, char* argv[]) {
  std::string userfile{argv[1]}, edgefile{argv[2]};

  //  std::map<std::string, Vertex> users;

  // load data
  graph g{userfile, edgefile};
  g.stats();

  testUserCentricCSV(g, std::stoi(argv[3]), 1000);

  // find all
  //auto names = get(boost::vertex_name, g);
  //  std::vector<std::future<void>> threads;
  //  int i = 0;
  //   for (auto it = vertices(g); it.first != it.second; ++it.first) {
//     /*threads.push_back(std::async(//[](int id, std::string name){
// 	  //std::cerr << i << " " << names[*(it.first)] << std::endl;
// 	  function
// 	  //std::cerr << id << " " << name << std::endl;
// 	    //}
// 	    , i, names[*(it.first)]));*/
//     threads.push_back(std::async([]{std::cerr << "TEST\n";}));
//   }

//   auto f = std::async(called_from_async);
//   f.get();

//   for (auto f = threads.begin(); threads.end() != f; ++f)
//     std::cout << "out\n";
//     //(*f).wait();
  
}
