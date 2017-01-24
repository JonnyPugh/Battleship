/*
players represent human controlled agents.
*/

#ifndef PLAYER_H
#define PLAYER_H

#include "agent.h"
#include <string>
#include <stdexcept>

class player : public agent {
	virtual std::string get_name() const final;
	virtual void print_initialization_header() const final;
	virtual void place_ship(unsigned int) final;
	virtual void handle_error(std::runtime_error&) final;
	virtual void change_player() const final;
	virtual bool make_shot() final;
};

#endif
