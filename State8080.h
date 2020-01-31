#ifndef _State8080_H
#define _State8080_H

//#include "pch.h"
#include "MachineSI.h"

struct ConditionCodes
{
	uint8_t z:1;
	uint8_t s:1;
	uint8_t p:1;
	uint8_t cy:1;
	uint8_t ac:1;
	uint8_t pad:3;

};


class State8080
{
public:
	State8080();
	State8080(std::vector<uint8_t>);
	~State8080();

	// Main Processor emulation functions
	int emulate(MachineSI &);
	void generate_interrupt(int);
	
	// Debug functions
	void dump(std::ostream &);
	void dump_state(std::ostream &);
	void dump_memory(std::ostream &);
	void UnimplementedInstruction(uint8_t opcode);

	// Helper functions
	uint8_t parity(uint16_t);
	uint16_t get_16_bit(uint8_t, uint8_t);


	uint8_t a;
	uint8_t b;
	uint8_t c;
	uint8_t d;
	uint8_t e;
	uint8_t h;
	uint8_t l;
	uint16_t sp;
	uint16_t pc;
	// program instructions are from 0x0000 to 0x1FFF
	// Graphics are drawn into memory starting from 0x2400
	std::vector<uint8_t> memory;
	ConditionCodes cc;
	uint8_t interrupts_enable;

};

#endif // _State8080_H