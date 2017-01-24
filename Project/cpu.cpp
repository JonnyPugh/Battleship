#include "cpu.h"
#include <string>
#include <random>
#include <chrono>
#include <functional>

using std::uniform_int_distribution;
using std::bind;
using namespace std::chrono;

unsigned int cpu_num = 1;
std::default_random_engine generator(duration_cast<milliseconds>(
    system_clock::now().time_since_epoch()).count());
uniform_int_distribution<unsigned int> random_row_col(0, 9);
uniform_int_distribution<unsigned int> random_bool(0, 1);
auto get_row_col = bind(random_row_col, generator);
auto get_bool = bind(random_bool, generator);

std::string cpu::get_name() const
{
	return "CPU" + std::to_string(cpu_num++);
}

void cpu::place_ship(unsigned int length)
{
	unsigned int column = get_row_col();
	unsigned int row = get_row_col();
	bool vertical = get_bool();
	map.place(row, column, length, vertical);
}

bool cpu::make_shot()
{
	return enemy_map->optimal_shot();
}
