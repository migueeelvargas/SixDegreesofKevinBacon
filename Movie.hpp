/*
 * Movie.hpp
 * Author: Miguel Vargas <m6vargas>
 * Date:   June 1, 2016
 *
 * The Movie class is used to define all
 * the various movie objects in the graph.
 */

#ifndef MOVIE_HPP
#define MOVIE_HPP

/* Forward declaration */
class ActorNode;

class Movie {
public:
	std::string name;  // Stores movie name
	int date;          // Stores movie release date
	std::unordered_map<std::string, ActorNode*> cast;	// cast

	/* Constructor */
	Movie (std::string movieName, int movieDate) : 
		name(movieName), date(movieDate) {}

	/* Destructor */
	~Movie();
	
};

#endif // MOVIE_HPP

