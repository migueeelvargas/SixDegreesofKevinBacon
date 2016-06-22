/*
 * ActorEdge.hpp
 * Author: Miguel Vargas <m6vargas>
 * Date:   May 25, 2016
 *
 * This file serves as a header for the class defining
 * edges between the ActorNodes. The edges will represent
 * a movie that two actors share in common. The edge will
 * store pointers to both actors & the info on the movie
 */

#ifndef ACTOREDGE_HPP
#define ACTOREDGE_HPP

class ActorNode;
class Movie;

class ActorEdge {

public:
  Movie* movie;
  ActorNode* actor;
  ActorNode* otherActor;
  int weight;

	/* Constructor */
  ActorEdge(Movie* movie, ActorNode* actor, ActorNode* otherActor) :
    movie(movie),
    actor(actor),
    otherActor(otherActor) {}
};

#endif // ACTOREDGE_HPP