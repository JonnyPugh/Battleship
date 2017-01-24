#include "field.h"
#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <vector>
#include <cstdlib>

using std::cout;
using std::endl;
using std::runtime_error;
using std::find;
using std::vector;

void field::place(unsigned int row, 
	unsigned int column, 
	unsigned int length, 
	bool vertical)
{
	ship_info_t ship_info;
	if (vertical)
	{
		// Check for errors.
		for (unsigned int i = row; i < row + length; ++i)
		{
			if (i > 9 || map[i][column] == '_' || map[i][column] == '|')
			{
				throw runtime_error("Cannot place ship here");
			}
		}

		// Add the vertical ship to the map.
		for (unsigned int i = row; i < row + length; ++i)
		{
			map[i][column] = '|';
			ship_info.coordinates.push_back({i, column});
		}
	}
	else
	{
		// Check for errors.
		for (unsigned int i = column; i < column + length; ++i)
		{
			if (i > 9 || map[row][i] == '_' || map[row][i] == '|')
			{
				throw runtime_error("Cannot place ship here");
			}
		}

		// Add the horizontal ship to the map.
		for (unsigned int i = column; i < column + length; ++i)
		{
			map[row][i] = '_';
			ship_info.coordinates.push_back({row, i});
		}
	}

	// Set the rest of the info about the ship.
	ship_info.name = length == 2 ? "Patrol boat" :
		length == 3 && placed_submarine ? "Destroyer" :
		length == 3 ? "Submarine" :
		length == 4 ? "Battleship" : "Aircraft carrier";
	ship_info.length = length;
	placed_submarine = placed_submarine || length == 3;
	ships.push_back(std::move(ship_info));
}

bool field::shot(unsigned int row, unsigned int column)
{
	// Check for errors.
	if (row > 9 || column > 9)
	{
		throw runtime_error("Specified location is out of range");
	}
	const char c = map[row][column];
	if (c == 'o' || c == 'x')
	{
		throw runtime_error("Specified location has already been shot");
	}
	
	// Output shot information.
	char column_label = 'A' + column;
	cout << "Shot at " << column_label << row + 1;

	// Handle the shot.
	if (c == '_' || c == '|')
	{
		map[row][column] = 'x';

		// Determine whether this hit caused a ship to sink.
		for (auto ship = ships.begin(); ship != ships.end(); ++ship)
		{
			auto it = find(ship->coordinates.begin(), 
				ship->coordinates.end(), 
				coordinate_t(row, column));
			if (it != ship->coordinates.end())
			{
				ship->coordinates.erase(it);
				if (ship->coordinates.empty())
				{
					cout << " sunk the " << ship->name << "!" << endl;
					ships.erase(ship);

					// Get the minimum and maximum 
					// lengths of the remaining ships.
					minimum_length = 5;
					maximum_length = 2;
					for (const ship_info_t& ship : ships)
					{
						if (ship.length < minimum_length)
						{
							minimum_length = ship.length;
						}
						if (ship.length > maximum_length)
						{
							maximum_length = ship.length;
						}
					}

					// A ship has been sunk so revert to hunting.
					hunting = true;
					potentials.clear();

					cout << "Number of remaining ships: " << ships.size() << endl;
					return ships.empty();
				}

				// A ship has been hit, so stop hunting.
				hunting = false;
				if (row > 0)
				{
					potentials.push_back({row - 1, column});
				}
				if (row < 9)
				{
					potentials.push_back({row + 1, column});
				}
				if (column > 0)
				{
					potentials.push_back({row, column - 1});
				}
				if (column < 9)
				{
					potentials.push_back({row, column + 1});
				}

				cout << " hit!" << endl;
				return false;
			}
		}
	}
	map[row][column] = 'o';
	cout << " missed!" << endl;
	return false;
}

bool field::optimal_shot()
{
	// Generate the weights of each location.
	vector<vector<unsigned int>> weights(map_size, 
		vector<unsigned int>(map_size, 0));
	for (int i = 0; i < map_size; ++i)
	{
		for (int j = 0; j < map_size; ++j)
		{
			if (map[i][j] != 'o' && map[i][j] != 'x')
			{
				for (int l = minimum_length - 1; l < maximum_length; ++l)
				{
					if (i - l >= 0)
					{
						unsigned int num_spaces = l;
						bool fits = true;
						while (fits && num_spaces > 0)
						{
							if (map[i - num_spaces][j] == 'o')
							{
								fits = false;
							}
							--num_spaces;
						}
						num_spaces = l;
						while (fits && num_spaces > 0)
						{
							++weights[i - num_spaces--][j];
						}
					}
					if (j - l >= 0)
					{
						unsigned int num_spaces = l;
						bool fits = true;
						while (fits && num_spaces > 0)
						{
							if (map[i][j - num_spaces] == 'o')
							{
								fits = false;
							}
							--num_spaces;
						}
						num_spaces = l;
						while (fits && num_spaces > 0)
						{
							++weights[i][j - num_spaces--];
						}
					}
					if (i + l <= 9)
					{
						unsigned int num_spaces = l;
						bool fits = true;
						while (fits && num_spaces > 0)
						{
							if (map[i + num_spaces][j] == 'o')
							{
								fits = false;
							}
							--num_spaces;
						}
						num_spaces = l;
						while (fits && num_spaces > 0)
						{
							++weights[i + num_spaces--][j];
						}
					}
					if (j + l <= 9)
					{
						unsigned int num_spaces = l;
						bool fits = true;
						while (fits && num_spaces > 0)
						{
							if (map[i][j + num_spaces] == 'o')
							{
								fits = false;
							}
							--num_spaces;
						}
						num_spaces = l;
						while (fits && num_spaces > 0)
						{
							++weights[i][j + num_spaces--];
						}
					}
				}
			}
		}
	}

	// If there are no more potential locations to
	// search, revert to hunting.
	hunting = hunting || potentials.empty();

	// Find a location to make a shot.
	unsigned int row, column, max_weight = 0;
	if (hunting)
	{
		for (unsigned int i = 0; i < map_size; ++i)
		{
			for (unsigned int j = 0; j < map_size; ++j)
			{
				if (weights[i][j] > max_weight)
				{
					max_weight = weights[i][j];
					row = i;
					column = j;
				}
			}
		}
	}
	else
	{
		for (coordinate_t c : potentials)
		{
			if (weights[c.first][c.second] > max_weight)
			{
				max_weight = weights[c.first][c.second];
				row = c.first;
				column = c.second;
			}
		}
		potentials.remove({row, column});
	}

	// If the algorithm couldn't find a good spot to shoot,
	// generate a random spot to shoot.
	if (map[row][column] == 'o' || map[row][column] == 'x')
	{
		row = rand() % 10;
		column = rand() % 10;
	}

	// Make the shot.
	return shot(row, column);
}

void field::print(bool owned) const noexcept
{
	// Print the column letter labels.
	cout << "   ";
	char letter = 'A';
	for (unsigned int i = 0; i < 10; ++i)
	{
		cout << letter++ << " ";
	}

	// Print the map.
	for (unsigned int row = 0; row < 10; ++row)
	{
		// Print the row number label.
		cout << endl;
		if (row < 9)
		{
			cout << " ";
		}
		cout << row + 1 << " ";

		// Print the characters in the row.
		for (unsigned int column = 0; column < 10; ++column)
		{
			char c = map[row][column];
			if (!owned && (c == '_' || c == '|'))
			{
				cout << ". ";
			}
			else if (column < 9 && c == '_' && map[row][column + 1] == '_')
			{
				cout << c << c;
			}
			else
			{
				cout << c << " ";
			}
		}
	}
	cout << endl;
}
