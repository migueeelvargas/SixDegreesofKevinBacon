/*
 * ActorNode.hpp
 * Author: Miguel Vargas <m6vargas>
 * Date:   May 25, 2016
 *
 * This file's purpose is to serve as the header file
 * that defines the node class used for every actor in 
 * the graph. The nodes will mainly hold a list to all
 * the connections to other Actors & a list of the movies
 * the actor has starred in.
 */

#ifndef ACTORNODE_HPP
#define ACTORNODE_HPP

#include <unordered_map>
#include <string>
#include <vector>
#include "ActorEdge.hpp"

class ActorNode {
public:

	std::string actorName;	// Stores the name of the actor.
  bool visited; // Used for Dijkstra's algorithm search.
  int dist;   // Used to measure distance during Dijkstra's algorithm
  ActorNode* prev;  // Used when traversing a path from one actor to another.
  int rank;

  // This holds all the ActorEdges that connect him to other ActorNodes.
  std::vector<ActorEdge*> adjacencyList;
  // This Vector holds all the movies the actor has starred in
  std::vector<std::string> movieList;
 
	/* Constructor */
	ActorNode (std::string name) : actorName(name) {
		visited = false;
    dist = -1;
    rank = 0;
	}
};

#endif // ACTORNODE_HPP