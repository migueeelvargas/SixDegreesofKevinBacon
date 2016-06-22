/*
 * actorconnections.cpp
 * Author: Miguel Vargas <m6vargas>
 * Date:   June 1, 2016
 *
 * This program aims to find the earliest 
 * year that two actors become connect through a path
 * of movies
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include "ActorGraph.hpp"

/** Performs a BFS traversal per year looking for actor connections
  *
  * @params: 2 ActorNode pointer: start & end, ofstream reference
  */
void BFS(ActorNode* start, ActorNode* end, ofstream& out);
/** Performs a UnionFind implemented using a hash map. 
  *
  * @params: 2 ActorNode pointer: start & end, ofstream reference
  */
void UnionFind(ActorNode* start, ActorNode* end, ofstream& out);
/** Builds the edges from the list of movies it is given.
  * List is based of the years.
  *
  * @params: vector of Movie pointers
  */
void buildEdges(std::vector<Movie*> movies);
/** Helper function used to reset all the edges on the actors
  *
  * @params: hash map of the Actors found in map
  */
void resetEdges(std::unordered_map<std::string, ActorNode*> actors);

// Create an ActorGraph object
ActorGraph* network = new ActorGraph();
std::unordered_set<ActorNode*> actorPool;

int main(int argc, char* argv[]) {

  // We first load all the actors & movies from the provided file
  auto success = network->loadFromFile(argv[1], false);
  auto actors = network->getActorsMap();
  bool have_header = false;

  ifstream ifs(argv[2]);
  ofstream ofs(argv[3]);
  string option = argv[4];
  // Actor1<TAB>Actor2<TAB>Year
  ofs << "Actor1" << "\t" << "Actor2" << "\t" << "Year" << endl;

  // Read while their is lines to be read
  while(ifs) {
    string s;
    if(!getline(ifs, s)) break;
    
    if (!have_header) {
      have_header = true;
      continue;
    }

    istringstream ss( s );
    vector<string> record;
    while ( ss ) {
      string next;
      if (!getline(ss, next, '\t')) break;
      record.push_back(next);
    }

    if (record.size() != 2) {
      // should have exactly 2 columns -> 2 actors
      continue;
    }

    string actor1(record[0]);
    string actor2(record[1]);

    resetEdges(actors);

    if (actors.find(actor1) == actors.end() || actors.find(actor2) == actors.end()) {
      ofs << actor1 << "\t" << actor2 << "\t" << "9999" << endl;
      continue;
    }

    /* Find all the paths to an actor from the first actor */
    if (option.compare("bfs") == 0)   // if unweighted
      BFS(actors[actor1], actors[actor2], ofs);

    // if union-find
    else if (option.compare("ufind") == 0)
      UnionFind(actors[actor1], actors[actor2], ofs);
  }

  // Close & flush streams.
  ofs.close();
  ofs.flush();
  ifs.close();

  return 1;
}

void BFS(ActorNode* start, ActorNode* end, ofstream& out) {
  // Set flag to false initially
  bool found = false;

  // We now have a forest of actors & movies (ordered by year)
  for (auto& year: network->getMovieByYear()) {
    // Build edges for specific given year.
    buildEdges(year.second);

    // So now that I have a pool of edges, run the actual BFS algorithm to find connection.
    network->BFS(start);

    // After running BFS, I now have all POSSIBLE paths from start to all nodes.
    auto curr = end;

    while(curr->prev != NULL) {
      curr = curr->prev;
    }

    if (curr == start) {
      out << start->actorName << "\t" << end->actorName << "\t" << year.first << endl;
      found = true;
      return;
    }
  }

  if (!found)
    out << start->actorName << "\t" << end->actorName << "\t" << "9999" << endl;
}

void buildEdges(std::vector<Movie*> movies) {
  ActorEdge* newEdge;

  // Access every movie in vector
  for (auto& movie : movies) {
    // For each movie, access every actor
    for (auto& actor : movie->cast) {
      for (auto& otherActor : movie->cast) {
        newEdge = new ActorEdge(movie, actor.second, otherActor.second);
        actor.second->adjacencyList.push_back(newEdge);
      }
    }
  }
}

void resetEdges(std::unordered_map<std::string, ActorNode*> actors) {
  for (auto& actor : actors)
    actor.second->adjacencyList.clear();
}

void UnionFind(ActorNode* start, ActorNode* end, ofstream& out) {
  // Make a set(map) for the movie
  std::unordered_map<std::string, ActorNode*> actorsUnion;

  bool found = false;

  // So we have movies for the specific year.
  for (auto& year: network->getMovieByYear()) {
    // Now access each movie in year
    for (auto& movie : year.second) {
      // Now add all the actors to the set
      for (auto& actor : movie->cast)
        actorsUnion.insert(actor);

      // So now the set has all possible actors, now union them.
      auto curr = *(actorsUnion.begin());

      for (auto& actor : actorsUnion) {
        if ((curr.second->rank) < (actor.second->rank)) {
          actorsUnion[curr.second->actorName] = actor.second;
          (actor.second->rank)++;
          curr.second = actor.second;
        }
        else if ((actor.second->rank) < (curr.second->rank)) {
          actorsUnion[actor.second->actorName] = curr.second;
          (curr.second->rank)++;
        }
        else if (actor.second->actorName == curr.second->actorName) {
          continue;
        }
        else {
          actorsUnion[actor.second->actorName] = curr.second;
          (curr.second->rank)++;
        }
      }
    }

    // Now that we have unioned all the movies in the year. Check if both actors are connected
    if (actorsUnion.find(start->actorName) == actorsUnion.end() ||
        actorsUnion.find(end->actorName) == actorsUnion.end()) {
      continue;
    }

    if (actorsUnion[start->actorName] == actorsUnion[end->actorName]) {
      out << start->actorName << "\t" << end->actorName << "\t" << year.first << endl;
      found = true;
      return;
    }
  }
  if (!found)
    out << start->actorName << "\t" << end->actorName << "\t" << "9999" << endl;

}