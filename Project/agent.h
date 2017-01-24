/*
agents represent participants in Battleship.
Each agent has their own field and makes
decisions of where to shoot the opponent.
*/

#ifndef AGENT_H
#define AGENT_H

#include "field.h"
#include <string>
#include <stdexcept>

class agent {
public:
	virtual ~agent() {}

	/*
	Initialize an agent.
	*/
	void initialize(agent*);

	/*
	Handle an agent's turn.
	Return true if this agent has won.
	*/
	bool turn();

protected:
	// This agent's map
	field map;

	// The enemy's map
	field* enemy_map = nullptr;

private:
	// Virtual functions that define how 
	// the non-virtual interface behaves.
	virtual std::string get_name() const = 0;
	virtual void print_initialization_header() const {};
	virtual void place_ship(unsigned int) = 0;
	virtual void handle_error(std::runtime_error&) {}
	virtual void change_player() const {}
	virtual bool make_shot() = 0;

	// This agent's name
	std::string name;
};

#endif
