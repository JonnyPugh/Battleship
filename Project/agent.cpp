#include "agent.h"
#include <iostream>
#include <stdexcept>

using std::cout;
using std::endl;
using std::runtime_error;

void agent::initialize(agent* enemy)
{
	// Set the enemy map and agent's name.
	enemy_map = &enemy->map;
	name = get_name();

	// Print the initialization header.
	cout << "Initializing " << name << "'s ships..." << endl;
	print_initialization_header();

	// Place the agent's ships.
	unsigned int num_ships = 0;
	while (num_ships < 5)
	{
		try
		{
			unsigned int length = num_ships <= 2 ? 5 : 6;
			length -= num_ships;
			place_ship(length);
			++num_ships;
		}
		catch (runtime_error& e)
		{
			handle_error(e);
		}
	}

	change_player();
	cout << "Finished placing " << name << "'s ships" << endl;
}

bool agent::turn()
{
	// Output the agent's turn.
	cout << name << "'s turn" << endl;

	bool made_shot = false;
	bool game_finished;
	while (!made_shot)
	{
		try
		{
			game_finished = make_shot();
			made_shot = true;
		}
		catch (runtime_error& e)
		{
			handle_error(e);
		}
	}

	change_player();
	if (game_finished)
	{
		cout << name << " wins!" << endl;
	}
	else
	{
		cout << name << "'s turn is finished" << endl;
	}
	return game_finished;
}
