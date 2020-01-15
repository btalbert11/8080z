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

uint8_t State8080::parity(uint16_t answer) {
	int i = 0;
	for (i = 0; i < 16; i++) {
		answer = answer & (answer - 1);
		if (answer == 0) { break; }
	}

	if ((i % 2) == 0) { return 1; }
	return 0;
}

uint16_t State8080::get_16_bit(uint8_t first_byte, uint8_t second_byte) {
	uint16_t fb = (uint16_t)first_byte;
	uint16_t sb = (uint16_t)second_byte;

	fb = fb << 8;

	return (fb | sb);
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
int State8080::emulate() {

	uint8_t * opcode = memory.data() + pc;
	uint16_t op_answer_16;
	uint32_t op_answer_32;
	uint16_t return_address;
	//uint8_t port;
	uint8_t psw;
	uint8_t psw_reg;

	switch (*opcode)
	{

	case 0x00: break; // NOP
	case 0x01: // LXI B
		c = (uint8_t)opcode[1];
		b = (uint8_t)opcode[2];
		pc += 2;
		break;
	case 0x02: UnimplementedInstruction(*opcode); break;
	case 0x03: UnimplementedInstruction(*opcode); break;
	case 0x04: UnimplementedInstruction(*opcode); break;
	case 0x05: // DCR B, B-- //TODO TEST
		op_answer_16 = (uint16_t)b;
		op_answer_16--;
		//flags
		if ((op_answer_16 & 0xff) == 0) { cc.z = 1; }
		else { cc.z = 0; }
		if ((op_answer_16 & 0x80) == 0x80) { cc.s = 1; }
		else { cc.s = 0; }
		cc.p = parity(op_answer_16 & 0xff);
		b = op_answer_16 & 0xff;
		break;
	case 0x06: // MVI B Move byte into B
		b = (uint8_t)opcode[1];
		pc += 1;
		break;
	case 0x07: UnimplementedInstruction(*opcode); break;
	case 0x08: UnimplementedInstruction(*opcode); break;
	case 0x09: // DAD B, HL = HL + BC
		op_answer_32 = get_16_bit(h, l) +
			get_16_bit(b, c);
		cc.cy = (op_answer_32 > 0xffff);
		h = (op_answer_32 >> 8) & 0xff;
		l = op_answer_32 & 0xff;
		break;
	case 0x0a: UnimplementedInstruction(*opcode); break;
	case 0x0b: UnimplementedInstruction(*opcode); break;
	case 0x0c: UnimplementedInstruction(*opcode); break;
	case 0x0d: // DCR C, C--
		op_answer_16 = (uint16_t)c;
		op_answer_16--;
		//flags
		if ((op_answer_16 & 0xff) == 0) { cc.z = 1;}
		else { cc.z = 0; }
		if ((op_answer_16 & 0x80) == 0x80) { cc.s = 1; }
		else { cc.s = 0; }
		cc.p = parity(op_answer_16 & 0xff);
		c = op_answer_16 & 0xff;
		break;
	case 0x0e: // MVI C, c = byte
		c = (uint8_t)opcode[1];
		pc += 1;
		break;
	case 0x0f: // RRC, rotates the accumulator (reg a) to the right
			   // if reg a = (01001011), the after RRC reg a = (10100101), CY = 1
		cc.cy = (1 == (a & 1));
		a = ((a & 1) << 7) | (a >> 1);
		break;
	case 0x10: UnimplementedInstruction(*opcode); break;
	case 0x11: // LXI D, D <- byte 3, E <- byte 2
		e = (uint8_t)opcode[1];
		d = (uint8_t)opcode[2];
		pc += 2;
		break;
	case 0x12: UnimplementedInstruction(*opcode); break;
	case 0x13: // INX D, DE++ //TODO TEST
		op_answer_16 = get_16_bit(d, e);
		op_answer_16++;
		d = (uint8_t)(op_answer_16 >> 8);
		e = (uint8_t)(op_answer_16 & 0xff);
		break;
	case 0x14: UnimplementedInstruction(*opcode); break;
	case 0x15: UnimplementedInstruction(*opcode); break;                                                   //      #$%02x
	case 0x16: UnimplementedInstruction(*opcode); break;
	case 0x17: UnimplementedInstruction(*opcode); break;
	case 0x18: UnimplementedInstruction(*opcode); break;
	case 0x19:; // DAD D, HL = HL + DE
		op_answer_32 = get_16_bit(h, l) +
			get_16_bit(d, e);
		cc.cy = (op_answer_32 > 0xffff);
		h = (op_answer_32 >> 8) & 0xff;
		l = op_answer_32 & 0xff;
		break;
	case 0x1a: // LDAX D, A <- (DE)
				//TODO TEST
		a = memory[get_16_bit(d, e)];
		break;
	case 0x1b: UnimplementedInstruction(*opcode); break;
	case 0x1c: UnimplementedInstruction(*opcode); break;
	case 0x1d: UnimplementedInstruction(*opcode); break;
	case 0x1e: UnimplementedInstruction(*opcode); break;
	case 0x1f: UnimplementedInstruction(*opcode); break;
	case 0x20: UnimplementedInstruction(*opcode); break;
	case 0x21: // LXI H, H <- byte 3, L <- byte 2
		l = (uint8_t)opcode[1];
		h = (uint8_t)opcode[2];
		pc += 2;
		break;
	case 0x22: UnimplementedInstruction(*opcode); break;
	case 0x23: // INX H, HL++ //TODO TEST
		op_answer_16 = get_16_bit(h, l);
		op_answer_16++;
		h = (uint8_t)(op_answer_16 >> 8);
		l = (uint8_t)(op_answer_16 & 0xff);
		break;
	case 0x24: UnimplementedInstruction(*opcode); break;
	case 0x25: UnimplementedInstruction(*opcode); break;
	case 0x26: // MVI H, h = byte
		h = (uint8_t)opcode[1];
		pc++;
		break;
	case 0x27: UnimplementedInstruction(*opcode); break;
	case 0x28: UnimplementedInstruction(*opcode); break;
	case 0x29: // DAD H, HL = HL * 2
		op_answer_16 = get_16_bit(h, l);
		cc.cy = ((op_answer_16 + op_answer_16) < op_answer_16);
		op_answer_16 = op_answer_16 + op_answer_16;
		h = (op_answer_16 >> 8) & 0xff;
		l = op_answer_16 & 0xff;
		break;
	case 0x2a: UnimplementedInstruction(*opcode); break;
	case 0x2b: UnimplementedInstruction(*opcode); break;
	case 0x2c: UnimplementedInstruction(*opcode); break;
	case 0x2e: UnimplementedInstruction(*opcode); break;
	case 0x2f: UnimplementedInstruction(*opcode); break;
	case 0x30: UnimplementedInstruction(*opcode); break;
	case 0x31: // LXI SP, stores two bytes into stack pointer
		// TODO TEST
		// Make sure this works lmao
		sp = get_16_bit(opcode[2], opcode[1]);
		pc += 2;
		break;
	case 0x32: // STA adr, (byte) = a
		memory[get_16_bit(opcode[2], opcode[1])] = a;
		pc += 2;
		break;
	case 0x33: UnimplementedInstruction(*opcode); break;
	case 0x34: UnimplementedInstruction(*opcode); break;
	case 0x35: UnimplementedInstruction(*opcode); break;
	case 0x36: // MVI M, D8 (HL) = byte
		memory[get_16_bit(h, l)] = (uint8_t)opcode[1];
		pc += 1;
		break;
	case 0x37: UnimplementedInstruction(*opcode); break;
	case 0x38: UnimplementedInstruction(*opcode); break;
	case 0x39: UnimplementedInstruction(*opcode); break;
	case 0x3a: // LDA adr, a = (byte)
		a = memory[get_16_bit(opcode[2], opcode[1])];
		pc += 2;
		break;
	case 0x3b: UnimplementedInstruction(*opcode); break;
	case 0x3c: UnimplementedInstruction(*opcode); break;
	case 0x3d: UnimplementedInstruction(*opcode); break;
	case 0x3e:; // MVI A,D8 , a = byte
		a = (uint8_t)opcode[1];
		pc += 1;
		break;
	case 0x3f: UnimplementedInstruction(*opcode); break;
	case 0x40: UnimplementedInstruction(*opcode); break;
	case 0x41: UnimplementedInstruction(*opcode); break;
	case 0x42: UnimplementedInstruction(*opcode); break;
	case 0x43: UnimplementedInstruction(*opcode); break;
	case 0x44: UnimplementedInstruction(*opcode); break;
	case 0x45: UnimplementedInstruction(*opcode); break;
	case 0x46: UnimplementedInstruction(*opcode); break;
	case 0x47: UnimplementedInstruction(*opcode); break;
	case 0x48: UnimplementedInstruction(*opcode); break;
	case 0x49: UnimplementedInstruction(*opcode); break;
	case 0x4a: UnimplementedInstruction(*opcode); break;
	case 0x4b: UnimplementedInstruction(*opcode); break;
	case 0x4c: UnimplementedInstruction(*opcode); break;
	case 0x4d: UnimplementedInstruction(*opcode); break;
	case 0x4e: UnimplementedInstruction(*opcode); break;
	case 0x4f: UnimplementedInstruction(*opcode); break;
	case 0x50: UnimplementedInstruction(*opcode); break;
	case 0x51: UnimplementedInstruction(*opcode); break;
	case 0x52: UnimplementedInstruction(*opcode); break;
	case 0x53: UnimplementedInstruction(*opcode); break;
	case 0x54: UnimplementedInstruction(*opcode); break;
	case 0x55: UnimplementedInstruction(*opcode); break;
	case 0x56: // MOV D,M , d = (HL)
		d = memory[get_16_bit(h, l)];
		break;
	case 0x57: UnimplementedInstruction(*opcode); break;
	case 0x58: UnimplementedInstruction(*opcode); break;
	case 0x59: UnimplementedInstruction(*opcode); break;
	case 0x5a: UnimplementedInstruction(*opcode); break;
	case 0x5b: UnimplementedInstruction(*opcode); break;
	case 0x5c: UnimplementedInstruction(*opcode); break;
	case 0x5d: UnimplementedInstruction(*opcode); break;
	case 0x5e: // MOV E,M  ,  e = (HL)
		e = memory[get_16_bit(h, l)];
		break;
	case 0x5f: UnimplementedInstruction(*opcode); break;
	case 0x60: UnimplementedInstruction(*opcode); break;
	case 0x61: UnimplementedInstruction(*opcode); break;
	case 0x62: UnimplementedInstruction(*opcode); break;
	case 0x63: UnimplementedInstruction(*opcode); break;
	case 0x64: UnimplementedInstruction(*opcode); break;
	case 0x65: UnimplementedInstruction(*opcode); break;
	case 0x66: // MOV H,M  , h = (HL)
		h = memory[get_16_bit(h, l)];
		break;
	case 0x67: UnimplementedInstruction(*opcode); break;
	case 0x68: UnimplementedInstruction(*opcode); break;
	case 0x69: UnimplementedInstruction(*opcode); break;
	case 0x6a: UnimplementedInstruction(*opcode); break;
	case 0x6b: UnimplementedInstruction(*opcode); break;
	case 0x6c: UnimplementedInstruction(*opcode); break;
	case 0x6d: UnimplementedInstruction(*opcode); break;
	case 0x6e: UnimplementedInstruction(*opcode); break;
	case 0x6f: // MOV L,A, l=a
		l = a;
		break;
	case 0x70: UnimplementedInstruction(*opcode); break;
	case 0x71: UnimplementedInstruction(*opcode); break;
	case 0x72: UnimplementedInstruction(*opcode); break;
	case 0x73: UnimplementedInstruction(*opcode); break;
	case 0x74: UnimplementedInstruction(*opcode); break;
	case 0x75: UnimplementedInstruction(*opcode); break;
	case 0x76: // HLT, halt, stops the processor / emulation
		std::cout << "\n\nEmulation halted\n\n";
		exit(1);
		break;
	case 0x77: // MOV M,A (HL) <- A
		memory[get_16_bit(h, l)] = a;
		break;
	case 0x78: UnimplementedInstruction(*opcode); break;
	case 0x79: UnimplementedInstruction(*opcode); break;
	case 0x7a: // MOV A,D a = d
		a = d;
		break;
	case 0x7b: // MOV A,E , a = e
		a = e;
		break;
	case 0x7c: // MOV A,H A = H
		a = h;
		break;
	case 0x7d: UnimplementedInstruction(*opcode); break;
	case 0x7e: // MOV A,M , a = (HL)
		a = memory[get_16_bit(h, l)];
		break;
	case 0x7f: UnimplementedInstruction(*opcode); break;
	case 0x80: UnimplementedInstruction(*opcode); break;
	case 0x81: UnimplementedInstruction(*opcode); break;
	case 0x82: UnimplementedInstruction(*opcode); break;
	case 0x83: UnimplementedInstruction(*opcode); break;
	case 0x84: UnimplementedInstruction(*opcode); break;
	case 0x85: UnimplementedInstruction(*opcode); break;
	case 0x86: UnimplementedInstruction(*opcode); break;
	case 0x87: UnimplementedInstruction(*opcode); break;
	case 0x88: UnimplementedInstruction(*opcode); break;
	case 0x89: UnimplementedInstruction(*opcode); break;
	case 0x8a: UnimplementedInstruction(*opcode); break;
	case 0x8b: UnimplementedInstruction(*opcode); break;
	case 0x8c: UnimplementedInstruction(*opcode); break;
	case 0x8d: UnimplementedInstruction(*opcode); break;
	case 0x8e: UnimplementedInstruction(*opcode); break;
	case 0x8f: UnimplementedInstruction(*opcode); break;
	case 0x90: UnimplementedInstruction(*opcode); break;
	case 0x91: UnimplementedInstruction(*opcode); break;
	case 0x92: UnimplementedInstruction(*opcode); break;
	case 0x93: UnimplementedInstruction(*opcode); break;
	case 0x94: UnimplementedInstruction(*opcode); break;
	case 0x95: UnimplementedInstruction(*opcode); break;
	case 0x96: UnimplementedInstruction(*opcode); break;
	case 0x97: UnimplementedInstruction(*opcode); break;
	case 0x98: UnimplementedInstruction(*opcode); break;
	case 0x99: UnimplementedInstruction(*opcode); break;
	case 0x9a: UnimplementedInstruction(*opcode); break;
	case 0x9b: UnimplementedInstruction(*opcode); break;
	case 0x9c: UnimplementedInstruction(*opcode); break;
	case 0x9d: UnimplementedInstruction(*opcode); break;
	case 0x9e: UnimplementedInstruction(*opcode); break;
	case 0x9f: UnimplementedInstruction(*opcode); break;
	case 0xa0: UnimplementedInstruction(*opcode); break;
	case 0xa1: UnimplementedInstruction(*opcode); break;
	case 0xa2: UnimplementedInstruction(*opcode); break;
	case 0xa3: UnimplementedInstruction(*opcode); break;
	case 0xa4: UnimplementedInstruction(*opcode); break;
	case 0xa5: UnimplementedInstruction(*opcode); break;
	case 0xa6: UnimplementedInstruction(*opcode); break;
	case 0xa7: // ANA A, a = a AND a
		op_answer_16 = a & a;
		// flags
		cc.z = ((op_answer_16 & 0xff) == 0);
		cc.s = ((op_answer_16 & 0x80) != 0);
		cc.cy = 0; // set to 0 according to Programmers Manual
		cc.p = parity(op_answer_16 & 0xff);
		a = (uint8_t)(op_answer_16 & 0xff);
		break;
	case 0xa8: UnimplementedInstruction(*opcode); break;
	case 0xa9: UnimplementedInstruction(*opcode); break;
	case 0xaa: UnimplementedInstruction(*opcode); break;
	case 0xab: UnimplementedInstruction(*opcode); break;
	case 0xac: UnimplementedInstruction(*opcode); break;
	case 0xad: UnimplementedInstruction(*opcode); break;
	case 0xae: UnimplementedInstruction(*opcode); break;
	case 0xaf: // XRA A, a = a XOR a
		op_answer_16 = a ^ a;
		// flags
		cc.z = ((op_answer_16 & 0xff) == 0);
		cc.s = ((op_answer_16 & 0x80) != 0);
		cc.cy = 0; // set to 0 according to Programmers Manual
		cc.p = parity(op_answer_16 & 0xff);
		a = (uint8_t)(op_answer_16 & 0xff);
		break;
	case 0xb0: UnimplementedInstruction(*opcode); break;
	case 0xb1: UnimplementedInstruction(*opcode); break;
	case 0xb2: UnimplementedInstruction(*opcode); break;
	case 0xb3: UnimplementedInstruction(*opcode); break;
	case 0xb4: UnimplementedInstruction(*opcode); break;
	case 0xb5: UnimplementedInstruction(*opcode); break;
	case 0xb6: UnimplementedInstruction(*opcode); break;
	case 0xb7: UnimplementedInstruction(*opcode); break;
	case 0xb8: UnimplementedInstruction(*opcode); break;
	case 0xb9: UnimplementedInstruction(*opcode); break;
	case 0xba: UnimplementedInstruction(*opcode); break;
	case 0xbb: UnimplementedInstruction(*opcode); break;
	case 0xbc: UnimplementedInstruction(*opcode); break;
	case 0xbd: UnimplementedInstruction(*opcode); break;
	case 0xbe: UnimplementedInstruction(*opcode); break;
	case 0xbf: UnimplementedInstruction(*opcode); break;
	case 0xc0: UnimplementedInstruction(*opcode); break;
	case 0xc1: // POP B, B = mem[sp+1], c = mem[sp]
		c = memory[sp];
		b = memory[sp + 1];
		sp += 2;
		break;
	case 0xc2: // JNZ if (not z) PC <- addr
		if (!cc.z) {
			pc = get_16_bit(opcode[2], opcode[1]);
			pc--;
		}
		else {
			pc += 2;
		}
		break;
	case 0xc3: // JMP Hi Brian :-* Jump to addr Hitler was right
		pc = get_16_bit(opcode[2], opcode[1]);
#ifdef JUMP
		std::cout << "JUMPING TO:" << std::hex << pc << "\n";
#endif
		pc--;
		break;
	case 0xc4: UnimplementedInstruction(*opcode); break;
	case 0xc5: // PUSH B, push b first then c
		memory[sp - 1] = b;
		memory[sp - 2] = c;
		sp -= 2;
		break;
	case 0xc6: // ADI D8, a = a + byte
		op_answer_16 = a + (uint8_t)opcode[1];
		// flags
		cc.z = ((op_answer_16 & 0xff) == 0);
		cc.s = ((op_answer_16 & 0x80) != 0);
		cc.cy = (op_answer_16 > 0xff);
		cc.p = parity(op_answer_16 & 0xff);
		a = op_answer_16 & 0xff;
		break;
	case 0xc7: UnimplementedInstruction(*opcode); break;
	case 0xc8: UnimplementedInstruction(*opcode); break;
	case 0xc9: // RET, return to address on the stack
		//memory[sp] is more sig byte, memory[sp+1] is least sig byte
		pc = get_16_bit(memory[sp + 1], memory[sp]);
		sp += 2;
		//pc--;
#ifdef JUMP
		std::cout << "RETURNING TO:" << std::hex << pc + 1 << "\tRETURN NUMBER " << --CALL_RET_NUMBER << "\n";
#endif
		break;
	case 0xca: UnimplementedInstruction(*opcode); break;
	case 0xcb: UnimplementedInstruction(*opcode); break;
	case 0xcc: UnimplementedInstruction(*opcode); break;
	case 0xcd: // Call a function, place return address on stack
		return_address = pc + 2; // TODO Could be wrong?? TODO

		// memory[sp-1] is more sig byte, memory[sp-2] is the least sig byte
		memory[sp - 1] = (return_address >> 8) & 0xff;
		memory[sp - 2] = (return_address & 0xff);
		sp -= 2;
		pc = get_16_bit(opcode[2], opcode[1]);
#ifdef JUMP
		std::cout << "CALLING TO:" << std::hex << pc << "\tCALL NUMBER " << CALL_RET_NUMBER++ << "\n";
#endif
		pc--;
		break;
	case 0xce: UnimplementedInstruction(*opcode); break;
	case 0xcf: UnimplementedInstruction(*opcode); break;
	case 0xd0: UnimplementedInstruction(*opcode); break;
	case 0xd1: // POP D, e = mem[sp], d = mem[sp+1]
		e = memory[sp];
		d = memory[sp + 1];
		sp += 2;
		break;
	case 0xd2: UnimplementedInstruction(*opcode); break;

		// TODO REIMPLEMENT
	case 0xd3:; // OUT D8, sends reg a to port # byte
		//port = (uint8_t)opcode[1];
		//machine_out(machine, a, port);       // Device dependent code
		//pc += 1;
		//break;
	case 0xd4: UnimplementedInstruction(*opcode); break;
	case 0xd5: // PUSH D, push DE onto stack, D first then E
		memory[sp - 1] = d;
		memory[sp - 2] = e;
		sp -= 2;
		break;
	case 0xd6: UnimplementedInstruction(*opcode); break;
	case 0xd7: UnimplementedInstruction(*opcode); break;
	case 0xd8: UnimplementedInstruction(*opcode); break;
	case 0xd9: UnimplementedInstruction(*opcode); break;
	case 0xda: UnimplementedInstruction(*opcode); break;

		//TODO REIMPLEMENT
	case 0xdb:; // IN D8, reads in port # byte and stores in reg a
		//port = (uint8_t)opcode[1];
		//a = machine_in(machine, port);   // Device dependent code
		//pc += 1;
		//break;
	case 0xdc: UnimplementedInstruction(*opcode); break;
	case 0xdd: UnimplementedInstruction(*opcode); break;
	case 0xde: UnimplementedInstruction(*opcode); break;
	case 0xdf: UnimplementedInstruction(*opcode); break;
	case 0xe0: UnimplementedInstruction(*opcode); break;
	case 0xe1: // POP H, HL = first and second byte on stack
		l = memory[sp];
		h = memory[sp + 1];
		sp += 2;
		break;
	case 0xe2: UnimplementedInstruction(*opcode); break;
	case 0xe3: UnimplementedInstruction(*opcode); break;
	case 0xe4: UnimplementedInstruction(*opcode); break;
	case 0xe5: // PUSH H, PUSH HL onto stack, first H then L
		memory[sp - 1] = h;
		memory[sp - 2] = l;
		sp -= 2;
		break;
	case 0xe6: // ANI D8, a = a & byte
		a = a & ((uint8_t)opcode[1]);
		// flags
		cc.z = (a == 0);
		cc.s = ((a & 0x80) == 0x80);
		cc.p = parity((uint16_t)a);
		cc.cy = 0; // Programmers manual for 8080 says to reset cy to 0
		pc += 1;
		break;
	case 0xe7: UnimplementedInstruction(*opcode); break;
	case 0xe8: UnimplementedInstruction(*opcode); break;
	case 0xe9: UnimplementedInstruction(*opcode); break;
	case 0xea: UnimplementedInstruction(*opcode); break;
	case 0xeb: // XCHG, HL = DE, DE = HL
		h = d;
		d = h;
		l = e;
		e = l;
		break;
	case 0xec: UnimplementedInstruction(*opcode); break;
	case 0xed: UnimplementedInstruction(*opcode); break;
	case 0xee: UnimplementedInstruction(*opcode); break;
	case 0xef: UnimplementedInstruction(*opcode); break;
	case 0xf0: UnimplementedInstruction(*opcode); break;
	case 0xf1:; // POP PSW, a = mem[sp+1], flags = mem[sp]
		a = memory[sp + 1];
		psw = memory[sp];
		sp += 2;
		cc.cy = (0x01 == (psw & 0x01));
		cc.p = (0x04 == (psw & 0x04));
		cc.ac = (0x10 == (psw & 0x10));
		cc.z = (64 == (psw & 64));
		cc.s = (128 == (psw & 128));
		pc += 2;
		break;
	case 0xf2: UnimplementedInstruction(*opcode); break;
	case 0xf3: UnimplementedInstruction(*opcode); break;
	case 0xf4: UnimplementedInstruction(*opcode); break;
	case 0xf5:; // PUSH PSW, push accumulator (reg a) and flags (as a reg) onto the stack
		psw_reg = (
			(cc.cy) |
			(((uint8_t)1) << 1) |
			(cc.p << 2) |
			(((uint8_t)0) << 3) |
			(cc.ac << 4) |
			(((uint8_t)0) << 5) |
			(cc.z << 6) |
			(cc.s << 7)
			);
		memory[sp - 1] = a;
		memory[sp - 2] = psw_reg;
		sp -= 2;
		break;
	case 0xf6: UnimplementedInstruction(*opcode); break;
	case 0xf7: UnimplementedInstruction(*opcode); break;
	case 0xf8: UnimplementedInstruction(*opcode); break;
	case 0xf9: UnimplementedInstruction(*opcode); break;
	case 0xfa: UnimplementedInstruction(*opcode); break;
	case 0xfb: // EI, enables interrupts
		interrupts_enable = 1;
		break;
	case 0xfc: UnimplementedInstruction(*opcode); break;
	case 0xfd: UnimplementedInstruction(*opcode); break;
	case 0xfe: // CPI Compare A to byte, A - byte
		op_answer_16 = ((uint16_t)a) - ((uint16_t)opcode[1]);
		//flags
		if ((op_answer_16) == 0) { cc.z = 1; }
		else { cc.z = 0; }
		if ((op_answer_16 & 0x80) == 0x80) { cc.s = 1; }
		else { cc.s = 0; }
		cc.p = parity(op_answer_16 & 0xff);
		cc.cy = (a < (uint8_t)opcode[1]);
		pc++;
		break;
	case 0xff: UnimplementedInstruction(*opcode); break;


	default:
		break;
	}

	pc++;
	return 1;
}

