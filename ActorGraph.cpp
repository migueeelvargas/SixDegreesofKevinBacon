/*
 * ActorGraph.cpp
 * Author: Miguel Vargas <m6vargas>
 * Date:   May 25, 2016
 *
 * This file is meant to exist as a container for 
 * starter code that you can use to read the input 
 * file format defined in movie_casts.tsv. 
 */
 
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <queue>
#include "ActorGraph.hpp"

using namespace std;

/** Comparator class that is used to compare two
  * ActorNode objects. It compares them using their
  * distance path away from a starting ActorNode.
  *
  * @params: 2 ActorNode objects: lhs & rhs
  * @returns: bool value from result of comparison
  */
class ActorComp {
public:
  bool operator()(ActorNode*& lhs, ActorNode*& rhs) const {
    // Check if distances are the same
    if (lhs->dist == rhs->dist) {
      // If same compare the titles
      if(lhs->actorName.compare(rhs->actorName) > 0)
        return true;

      return false;
    }
    // If not, return the highest (lowest distance) priority
    else
      return lhs->dist > rhs->dist;  
  }
};

/** Comparator class that is used to compare two
  * Movie objects. It compares them using their
  * release dates. The one with the highest date
  * gets the priority
  *
  * @params: 2 Movie objects: lhs & rhs
  * @returns: bool value from result of comparison
  */
class MovieComp {
public:
  bool operator()(Movie*& lhs, Movie*& rhs) const {
    // Check if dates are the same
    if (lhs->date == rhs->date) {
      // If same compare the titles
      if(lhs->name.compare(rhs->name) > 0)
        return true;

      return false;
    }
    // If not, return the highest (highest date) priority
    else
      return lhs->date < rhs->date;  
  }
};

/* Constructor */
ActorGraph::ActorGraph(void) {}

bool ActorGraph::loadFromFile(const char* in_filename, bool use_weighted_edges) {
  // Initialize the file stream
  ifstream infile(in_filename);

  bool have_header = false;

  // keep reading lines until the end of file is reached
  while (infile) {
    string s;

    // get the next line
    if (!getline( infile, s )) break;

    if (!have_header) {
        // skip the header
        have_header = true;
        continue;
    }

    istringstream ss( s );
    vector <string> record;

    while (ss) {
        string next;
  
        // get the next string before hitting a tab character and put it in 'next'
        if (!getline( ss, next, '\t' )) break;

        record.push_back( next );
    }

    if (record.size() != 3) {
        // we should have exactly 3 columns
        continue;
    }

    string actor_name(record[0]);
    string movie_title(record[1]);
    int movie_year = stoi(record[2]);

    // we have an actor/movie relationship, now what?
    setup(actor_name, movie_title, movie_year);
  }  
  
  if (!infile.eof()) {
    cerr << "Failed to read " << in_filename << "!\n";
    return false;
  }
  infile.close();

  return true;
}

/** This function handles the delegated task of reading/adding 
  * each actor (creating nodes, adding to hash maps) & movies
  * (adding to reference hash maps).
  *
  * @params: string -> actor's name, string -> movie title, int -> relase date
  * @returns: void
  */
void ActorGraph::setup (string actor_name, string movie_title, int movie_year) {
  // Concatenate movie title & its date to assure it gets its own key in hash map
  string movie_title_full = (movie_title + " " + to_string(movie_year));

  ActorNode* actor;

  // Add all of the actors to the graph
  if(actorsMap.find(actor_name) == actorsMap.end()) {
    actor = new ActorNode(actor_name);
    actorsMap[actor_name] = actor;
    ++actorCount; // Update the actor count
  }
  else
    actor = actorsMap[actor_name];

  // Check if movie is not in the movie hash map-> add it
  if(movies.find(movie_title_full) == movies.end()) {

    // Create new movie
    Movie* newMovie = new Movie(movie_title, movie_year);
    movies[movie_title_full] = newMovie;
    movieByYear[movie_year].push_back(newMovie);
    newMovie->cast[actor_name] = actor;
    ++movieCount;
  }
    
  // If movie is in the map, add actor to actor hash map
  else {
    auto& movie = movies.at(movie_title_full);
    movie->cast[actor_name] = actor;
  }

  // Now add the movie_title to the actor's movieList
  actorsMap[actor_name]->movieList.push_back(movie_title_full);
}

/** This function handles the delegated task of specifically
  * building all the ActorEdges that connect two ActorNodes
  * based of a movie they both starred in.
  * 
  * @params: none
  * @returns: void
  */
void ActorGraph::buildActorEdges() {
  ActorEdge* newEdge;

  for(auto& movie: movies) {
    for (auto& actor : movie.second->cast) {
      for (auto& otherActor : movie.second->cast) {
        if (actor.second->actorName != otherActor.second->actorName) {
          newEdge = new ActorEdge(movie.second, actor.second, otherActor.second);
          newEdge->weight = (2015 - (movie.second->date)) + 1;
          actor.second->adjacencyList.push_back(newEdge);
          ++edgeCount;
        }
      }
    }
  }
}

/** This function handles the delegated task of running a BFS
  * traversal from a given staring point(pointer). While performing
  * the traversal we will assign the prev pointer in the ActorNode
  * to the previously read ActorNode. This will serve to keep track
  * of the path from the starting ActorNode to a specific ActorNode.
  * The BFS traversal guantees that it will be the shortest path to
  * the specific ActorNode.
  * 
  * @params: ActorNode* -> to the Actor to perform BFS on.
  * @returns: void
  */
void ActorGraph::BFS(ActorNode* start) {
  resetActorFields();

  // Initialize the queue
  std::queue<ActorNode*> toExplore;
  start->visited = true;  // Set the starting node to visited
  toExplore.push(start);  // Queue the starting node

  // While there is actors in the queue
  while(!toExplore.empty()) {
    ActorNode* curr = toExplore.front();  // become the next value
    toExplore.pop();  // Pop the next value

    // Add all the dequeued actor's adjacent ActorNode's(neighbors) 
    // to the queue.
    for(auto& edge : curr->adjacencyList) {
      ActorNode* neighbor = edge->otherActor;

      // If the actor hasn't already been visited, update it
      if (!neighbor->visited) {
        neighbor->visited = true;
        neighbor->prev = curr;
        toExplore.push(neighbor);
      }
    }
  }
}

/** This function handles the delegated task of running
  * Djikstra's algorithm on the the graph container given
  * a starting point (ActorNode). The algorithm will 
  * find the shortest weighted distance from the starting
  * point to the each other ActorNode in the graph. In
  * order to trace a path from the starting node to any
  * other node, a pointer to the previous node is added
  * to each node during the traversal.
  *
  * @params: start - ActorNode* - pointer to starting node
  * @return: void
  */
void ActorGraph::Dijkstra(ActorNode* start) {
  resetActorFields();

  std::priority_queue<ActorNode*, vector<ActorNode*>, ActorComp> pq;
  auto curr = start;
  ActorNode* neighbor;
  int newDist = 0;

  start->dist = 0;
  pq.push(start);

  // While the priority queue is not empty
  while(!pq.empty()) {
    curr = pq.top();
    pq.pop();

    if (!curr->visited) {
      curr->visited = true;

      for(auto& edge : curr->adjacencyList) {
        auto otherActor = edge->otherActor;

        if (otherActor->dist == -1 || (curr->dist + edge->weight) < otherActor->dist) {
          otherActor->dist = curr->dist + edge->weight;
          otherActor->prev = curr;
        }
        pq.push(otherActor);
      }
    }
  }
}

/** This function handles the delegated task of reseting
  * all the fields in every ActorNode in the graph. This
  * is done before any BFS/Dijkstra's Algorithm search.
  * This is to be done so there is a proper path from 
  * any node to the starting node.
  */
void ActorGraph::resetActorFields() {
  for (auto actor = actorsMap.begin(); actor != actorsMap.end(); ++actor) {
    (*actor).second->dist = -1;
    (*actor).second->prev = 0;
    (*actor).second->visited = false;
  }
}

/** This function handles the task of printing the paths from one
  * actor to the other to the provided output file. This method
  * uses a recursive approach to back track the ending node to
  * the starting node of the path.
  *
  * @params:  2 ActorNodes (pointers) & ofstream (output file)
  * @returns: void
  */
void ActorGraph::exportResults(ActorNode* actor1, ActorNode* actor2, ofstream& out) {
  // Base case: Stop when the actor's previous pointer is NULL
  if (actor2->prev == NULL) {
    out << "(" << actor2->actorName << ")";
    return;
  }
  // Rescursive call: Call method with the first actor & the second's previous actor
  else 
    exportResults(actor1, actor2->prev, out);

  // Calls method to find any movie relating to both actors
  Movie* movie = findCommonMovie(actor2, actor2->prev);

  // Print the 2nd actor's name & movie both actors starred on
  out << "--[" << movie->name << "#@" << movie->date << "]-->"
      << "(" << actor2->actorName << ")";
}

/** This method handles the delegated task of finding a common movie
  * between two actors. This method takes each actor's individual 
  * movie list with movies they've starred in. Now, with those lists
  * the set_intersection() method from the algorithm standard library 
  * class is called. This makes a new vector list with the commonly 
  * shared movies they've both starred in. Since there can multiple
  * common movies between two actors, we only take one(first one) from
  * the new vector list & find the data on that movie from the 
  * movieReference hash map.
  *
  * @params: 2 ActorNodes-> actor & otherActor
  * @returns: A pair<string, int> -> <movie_title, movie_date>
  */
  // pair<string, int> ActorGraph::findCommonMovie(ActorNode* actor, ActorNode* otherActor) {
Movie* ActorGraph::findCommonMovie(ActorNode* actor, ActorNode* otherActor) {
  // Get references to the actor's movie lists
  auto& movieList1 = actor->movieList;
  auto& movieList2 = otherActor->movieList;
  std::priority_queue<Movie*, std::vector<Movie*>, MovieComp> pq;
  // std::priority_queue<

  // Sort the lists for set_intersection method
  sort(movieList1.begin(), movieList1.end());
  sort(movieList2.begin(), movieList2.end());

  // Create destination vector
  std::vector<std::string> commonMovies;

  // Find the common movies
  auto it = set_intersection(movieList1.begin(), movieList1.end(), 
          movieList2.begin(), movieList2.end(), std::back_inserter(commonMovies));

  // So now we take the vector & prioritize the movies based on date, highest first.
  for(auto& movie : commonMovies)
    pq.push(movies[movie]);

  // We now return the top prioritized movie
  return pq.top();
}

/* Getters */
unordered_map<string, ActorNode*> ActorGraph::getActorsMap() {
  return actorsMap;
}

map<int, std::vector<Movie*>> ActorGraph::getMovieByYear() {
  return movieByYear;
}