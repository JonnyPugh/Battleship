#include "player.h"
#include <iostream>
#include <string>
#include <stdexcept>
#include <limits>

using std::cout;
using std::endl;
using std::cin;
using std::string;
using std::runtime_error;

string player::get_name() const
{
	// Get the player's name.
	cout << "Enter your name: ";
	string player_name;
	cin >> player_name;
	return player_name;
}

void player::print_initialization_header() const
{
	// Output the input format.
	cout << "Input format: ";
	cout << "<column letter><row number>";
	cout << "<v (vertical) or h (horizontal)>" << endl;
}

/*
Get a column letter from input and convert it.
*/
void get_column(unsigned int& column)
{
	char column_label;
	if (!(cin >> column_label))
	{
		throw runtime_error("Incorrect formatting in input");
	}
	column = column_label - 'A';
}

/*
Get a row number from input and convert it.
*/
void get_row(unsigned int& row)
{
	if (!(cin >> row))
	{
		throw runtime_error("Incorrect formatting in input");
	}
	--row;
}

void player::place_ship(unsigned int length)
{
	cout << "Specify the location and orientation of your ";
	cout << "ship of length " << length << ": ";

	// Get input from the user.
	unsigned int column, row;
	bool vertical;
	get_column(column);
	get_row(row);
	char orientation;
	cin >> orientation;

	// Convert orientation to a boolean
	vertical = orientation == 'v';
	if (!vertical && orientation != 'h')
	{
		throw runtime_error("Incorrect formatting in input");
	}

	// Place the ship
	map.place(row, column, length, vertical);
	cout << "Current state of this player's map:" << endl;
	map.print(true);
}

void clear_input()
{
	cin.clear();
	cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void player::handle_error(runtime_error& e)
{
	cout << e.what() << endl;
	clear_input();
}

void player::change_player() const
{
	// Wait for user to specify that they're done.
	cout << "Press enter to finish..." << endl;
	clear_input();
	getchar();

	// Output whitespace to hide information from the next user.
	for (unsigned int i = 0; i < 100; ++i)
	{
		cout << endl;
	}
}

bool player::make_shot()
{
	// Output information at the beginning of a turn.
	cout << "Input format: ";
	cout << "<column letter><row number>" << endl;
	cout << "State of this player's enemy's map:" << endl;
	enemy_map->print(false);

	// Get the location of the shot and make the shot.
	cout << "Specify the location of your shot: ";
	unsigned int column, row;
	get_column(column);
	get_row(row);
	bool game_finished = enemy_map->shot(row, column);

	// Output information after making the shot.
	cout << "New state of this player's enemy's map:" << endl;
	enemy_map->print(false);
	cout << "State of this player's map:" << endl;
	map.print(true);

	return game_finished;
}
