#include <string>

#include "graph.hpp"

int main(int argc, char* argv[]) {
  std::string userfile{argv[1]}, edgefile{argv[2]};

  //  std::map<std::string, Vertex> users;

  // load data
  graph g{userfile, edgefile};
  g.stats();


  for (int i = 0; i < 10; ++i) {
    user u = g.get_topuser({"c"+i});
  }

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
