#include "pch.h"


/*
*	Constductores destructors
*/

// No memory initialized
State8080::State8080()
{
	a = 0;
	b = 0;
	c = 0;
	d = 0;
	e = 0;
	h = 0;
	l = 0;
	sp = 0;
	pc = 0;
	cc.z = 0;
	cc.s = 0;
	cc.p = 0;
	cc.cy = 0;
	cc.ac = 0;
	cc.pad = 0;
	interrupts_enable = 0;
}

// Memory alreay initialized
State8080::State8080(std::vector<uint8_t> m)
{
	a = 0;
	b = 0;
	c = 0;
	d = 0;
	e = 0;
	h = 0;
	l = 0;
	sp = 0;
	pc = 0;
	cc.z = 0;
	cc.s = 0;
	cc.p = 0;
	cc.cy = 0;
	cc.ac = 0;
	cc.pad = 0;
	interrupts_enable = 0;

	this->memory = m;
}

State8080::~State8080()
{
}


/*
*	Helper functions
*/

void State8080::UnimplementedInstruction(uint8_t opcode) {
	std::cout << "\n\nError: UnimplementedInstruction\tOpcode: " << std::hex << (unsigned int) opcode << "\n";
	dump_state(std::cout);
	exit(1);
}

/*
*	Debug functions
*/

void State8080::dump_state(std::ostream &file) {
	file << "\n---------- STATE DUMP ----------\n";
	file << "State: a=" << std::hex << (unsigned int)a << ", b=" << (unsigned int)b << ", c=" << (unsigned int)c << ", d=" << (unsigned int)d;
	file << ", e=" << std::hex << (unsigned int)e << ", h=" << (unsigned int)h << ", l=" << (unsigned int)l << "\n";
	file << "pc=" << std::hex << (unsigned int)pc << ", sp=" << (unsigned int)sp << "\n";
	file << "\n---------- END OF STATE DUMP ----------\n";
}

void State8080::dump_memory(std::ostream &file) {
	file << "\n---------- MEMORY DUMP ----------\n";
	for (int i = 0; i < 65535; i++) {
		file << "State info:\tpc=" << i << ", sp=" << sp << "\t\t";
		file << "Opcode:\t" << std::hex << (unsigned int)memory.at(i) << "\n";
	}
	file << "\n---------- END MEMORY DUMP ----------\n";
}

// dump all relevant info
void State8080::dump(std::ostream &file) {
	dump_state(file);
	dump_memory(file);
}


// majority of program emulation happens in this function
void State8080::emulate() {

	uint8_t * opcode = memory.data() + pc;
	uint16_t aritmatic_answer_16;
	uint32_t arithmatic_answer_32;
	uint16_t return_address;
	uint8_t port;

}

