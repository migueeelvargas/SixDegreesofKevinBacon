/*
 * pathfinder.cpp
 * Author: Miguel Vargas <m6vargas>
 * Date:   May 25, 2016
 *
 * The purpose of this program is to find a path
 * from one actor to another. The program will take
 * different modes (weight or unweighted), a movie_cast file,
 * a list of actors to find connections with & an output file
 * to export the results to. The exported results will display
 * the entire path consisting of the common shared movie & the 
 * actor's name. Essentially, this program implements the 
 * Six Degrees of Bacon game.
 */

 #include <iostream>
 #include <fstream>
 #include <sstream>
 #include "ActorGraph.hpp"

 int main(int argc, char* argv[]) {

 	// Create an ActorGraph object
 	ActorGraph* network = new ActorGraph();
  string option = argv[2];
  ifstream ifs(argv[3]);
  ofstream ofs(argv[4]);
  bool have_header = false;

  // Print header to output file
  ofs << "(actor)--[movie#@year]-->(actor)--..." << endl;

 	// Load info from provided filename
 	auto success = network->loadFromFile(argv[1], false);
  // Now we need to make ActorEdges for actor connections
  network->buildActorEdges();
  auto actors = network->getActorsMap();

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

    /* Find all the paths to an actor from the first actor */
    if (option.compare("u") == 0)   // if unweighted
      network->BFS(actors[actor1]);

    // If weighted
    else
      network->Dijkstra(actors[actor1]);

    // Export the results to outfile
    network->exportResults(actors[actor1], actors[actor2], ofs);
    ofs << std::endl; // Add line break
  }

  // Close & flush streams.
  ofs.close();
  ofs.flush();
  ifs.close();

  return 1;
 }