# A simple makefile for six degrees of kevin bacon repo

CC=g++
CXXFLAGS=-std=c++11
LDFLAGS=

# if passed "type=opt" at command-line, compile with "-O3" flag (otherwise use "-g" for debugging)

ifeq ($(type),opt)
  CPPFLAGS += -O3
  LDFLAGS += -O3
else
  CPPFLAGS += -g
  LDFLAGS += -g
endif

all: pathfinder actorconnections extension



# include what ever source code *.hpp files pathfinder relies on (these are merely the ones that were used in the solution)

pathfinder: ActorGraph.o

actorconnections: ActorGraph.o

extension: ActorGraph.o



# include what ever source code *.hpp files ActorGraph relies on (these are merely the ones that were used in the solution)

# Note: you do not have to include a *.cpp file if it aleady has a paired *.hpp file that is already included with class/method headers

# ActorGraph.o: UnionFind.hpp Movie.hpp ActorNode.hpp ActorEdge.hpp ActorGraph.hpp
ActorGraph.o: ActorNode.o ActorEdge.o ActorGraph.hpp
ActorNode.o: ActorNode.hpp
ActorEdge.o: ActorEdge.hpp

clean:
	rm -f pathfinder *.o core*

