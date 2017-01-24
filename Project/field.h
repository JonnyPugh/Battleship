/*
fields represent maps in Battleship.
When the field is not owned by the user,
'.' represents unknown spots, 'o' represents missed spots,
and 'x' represents enemy ships that have been hit.
When the field is owned by the user,
'.' represents empty spots, 'x' represents owned ships
that have been hit, and '_' or '|' represent owned ships.
*/

#ifndef FIELD_H
#define FIELD_H

#include <vector>
#include <string>
#include <list>

class field {
public:
	/*
	Construct a default field.
	*/
	field()
		: map(map_size, std::vector<char>(map_size, '.')) {}

	/*
	Place a ship of the specified length and orientation 
	(true for vertical) at the specified location.
	Throws a runtime_error if there is another ship in the way
	or if the ship would be out of bounds if it was placed.
	*/
	void place(unsigned int, unsigned int, unsigned int, bool);

	/*
	Handle a shot at the specified location.
	Throws a runtime_error if the specified location has 
	already received a shot or if the shot is out of bounds.
	Return true if this shot finished off the ships.
	*/
	bool shot(unsigned int, unsigned int);

	/*
	Find the optimal location to make a shot then
	make the shot at that location.
	Return true if this shot finished off the ships.
	*/
	bool optimal_shot();

	/*
	Print the map in a human readable format.
	The output mode is specified (true for owned).
	*/
	void print(bool) const noexcept;

private:
	// Size of the square map
	static constexpr unsigned int map_size = 10;

	// 2D vector of chars that represents the map
	std::vector<std::vector<char>> map;

	// Information associated with the ships in play
	typedef std::pair<unsigned int, unsigned int> coordinate_t;
	struct ship_info_t
	{
		std::string name;
		unsigned int length;
		std::list<coordinate_t> coordinates;
	};
	std::list<ship_info_t> ships;
	unsigned int minimum_length = 2;
	unsigned int maximum_length = 5;
	bool placed_submarine = false;
	bool hunting = true;
	std::list<coordinate_t> potentials;
};

#endif
