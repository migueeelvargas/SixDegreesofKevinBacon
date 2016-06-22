/*
 * Movie.hpp
 * Author: Miguel Vargas <m6vargas>
 * Date:   June 1, 2016
 *
 * Add description here!
 */

#ifndef MOVIE_HPP
#define MOVIE_HPP

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

