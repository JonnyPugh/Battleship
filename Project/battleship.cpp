#include "agent.h"
#include "player.h"
#include "cpu.h"
#include <iostream>

using std::cout;
using std::endl;
using std::cin;

int main()
{
	// Determine whether the game will be 0, 1, or 2 player.
	unsigned int num_players = 3;
	while (num_players != 0 && num_players != 1 && num_players != 2)
	{
		cout << "Specify number of human players (0, 1, or 2): ";
		cin >> num_players;
		cin.clear();
		while (getchar() != '\n');
	}

	// Set up the game.
	agent* p1;
	agent* p2;
	if (num_players == 0)
	{
		p1 = new cpu();
		p2 = new cpu();
	}
	else if (num_players == 1)
	{
		p1 = new player();
		p2 = new cpu();
	}
	else
	{
		p1 = new player();
		p2 = new player();
	}
	p1->initialize(p2);
	p2->initialize(p1);

	// Play the game until someone wins.
	while (!(p1->turn() || p2->turn()));

	delete p1;
	delete p2;
}
