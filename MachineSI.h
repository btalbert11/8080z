#ifndef MACHINESI_H
#define MACHINESI_H

//#include "pch.h"
#include <bitset>

class MachineSI
{
public:

	uint8_t ports[8]; // each port uses all 8 bits for various flags
	uint8_t shift_x = 0;
	uint8_t shift_y = 0;
	uint8_t shift_offset = 0;

	MachineSI();
	~MachineSI();

	void dump_info(std::ostream &);
	void key_down(char);
	void key_up(char);
	void out(uint8_t, uint8_t);
	uint8_t in(uint8_t);
};

#endif // MACHINESI_H