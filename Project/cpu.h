/*
cpus represent cpu controlled agents.
*/

#ifndef CPU_H
#define CPU_H

#include "agent.h"
#include <string>

class cpu : public agent {
	virtual std::string get_name() const final;
	virtual void place_ship(unsigned int) final;
	virtual bool make_shot() final;
};

#endif
