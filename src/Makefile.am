# what flags you want to pass to the C compiler & linker
CXXFLAGS=-std=c++11 -ggdb -O0 -Werror -I/opt/local/include
LDFLAGS=

# this lists the binaries to produce, the (non-PHONY, binary) targets in
# the previous manual Makefile
bin_PROGRAMS = teamrep
teamrep_SOURCES = config.hpp user.hpp user.cpp team.hpp team.cpp	\
		  teamgenerator.hpp teamgenerator.cpp graph.cpp		\
		  graph.hpp teamrep.cpp
