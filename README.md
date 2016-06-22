#Six Degrees of Kevin Bacon

##Overview
This repository holds an implementation of the *Six Degrees of Kevin Bacon* game. The goal is to find the degree of separation between Hollywood actors that act in the same movies. The goal of this assignment was to implement an efficient graph-based data structure and to use it to explore large graphs of data.

The *Six Degrees of Kevin Bacon* game is based off the parlor game, the "six degrees of separation" concept, which claims that any two people on Earth are six or fewer acquaintance-links apart. The same idea can then be morphed into this game wherein movie buffs challenge each other to find the shortest path between any arbritrary actor and the prolific Hollywood actor, Kevin Bacon. It is assumed that any actor involved in the Hollywood film industry can be linked through his or her appearance in their film roles to Kevin Bacon within six steps. The game can be described as a trivia game based on the concept of the small world phenomenon.

##Implementation

###ActorGraph
The **_ActorGraph_** serves as a container for all of the actors & the movie they've each have had a role in. The implementation of the graph consists of each actor having a respective ActorNode, which, primarily holds the:

* Actor's name
* An adjacency list that holds all connections to all of the other actors the primary actor shares through movies.
* A list of all the movies he or she has starred in.

In the graph, each actor to other actors via an **_ActorEdge_**. The **_ActorEdge_** connects two **_ActorNodes_** through a common movie they've both starred in. The edge consists of: 

* A pointer to a movie object
* A pointer to the primary actor
* A pointer to the secondary actor
* Weight integer that is calculated by `weight = 1 + (2015 - Y)`, where Y is the year the movie was released.

Lastly, the last class used by the graph is the **_Movie_** class. The **_Movie_** class simply serves as an object for all the different movies in the graph & consist of: 

* Movie name
* The release date
* And a hash map holding pointers to all of the cast members (actors).

###Pathfinder
The purpose of this program is to find a path from one actor to another. The program will take different modes (weight or unweighted), a movie_cast file, a list of actors to find connections with & an output file to export the results to. The exported results will display the entire path consisting of the common shared movie & the actor's name. Essentially, this program implements the Six Degrees of Bacon game.

###actorconnections
This program aims to find the earliest year that two actors become connect through a path of movies.
