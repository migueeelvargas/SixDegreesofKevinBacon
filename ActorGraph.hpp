/*
 * ActorGraph.hpp
 * Author: Miguel Vargas <m6vargas>
 * Date:   May 25, 2016
 *
 * This file is meant to exist as a container for starter code that you can use to read the input file format
 * defined in movie_casts.tsv. Feel free to modify any/all aspects as you wish.
 */

#ifndef ACTORGRAPH_HPP
#define ACTORGRAPH_HPP

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <map>
#include <algorithm>
#include <iterator>
#include "ActorNode.hpp"
#include "Movie.hpp"
using namespace std;

class ActorComp;
class MovieComp;

class ActorGraph {
protected:
  // A map that stores movies & a pointer to allocate Movie object
  std::unordered_map<std::string, Movie*> movies;

  // This map stores all the actor's & their corresponding ActorNode
  std::unordered_map<std::string, ActorNode*> actorsMap;

  // This map stores all the movies based on the year they were released
  std::map<int, std::vector<Movie*>> movieByYear;

public:
  /* Instance Variables */
  int actorCount = 0;
  int movieCount = 0;
  int edgeCount = 0;

  /* Constructor */
  ActorGraph(void);

  /** You can modify this method definition as you wish
   *
   * Load the graph from a tab-delimited file of actor->movie relationships.
   *
   * in_filename - input filename
   * use_weighted_edges - if true, compute edge weights as 1 + (2015 - movie_year), otherwise all edge weights will be 1
   *
   * return true if file was loaded sucessfully, false otherwise
   */
  bool loadFromFile(const char* in_filename, bool use_weighted_edges);

  void setup (string actor_name, string movie_title, int movie_year);

  void buildActorEdges();
  
  void BFS(ActorNode* start);

  void Dijkstra(ActorNode* start);

  void resetActorFields();

  void exportResults(ActorNode* start, ActorNode* end, ofstream& out);

  Movie* findCommonMovie(ActorNode* actor1, ActorNode* actor2);

  /* Getters */
  unordered_map<string, ActorNode*> getActorsMap();
  map<int, std::vector<Movie*>> getMovieByYear();
};


#endif // ACTORGRAPH_HPP