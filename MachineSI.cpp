#include "pch.h"


MachineSI::MachineSI()
{

	// initialize bits that are always 1
	ports[0] = 14;
	ports[1] = 8;

}


MachineSI::~MachineSI()
{
}

void MachineSI::key_down(char key) {
	switch (key) {
		case ' '://FIRE:
			// set bit 4
			ports[1] = ports[1] | 0x10;
			break;
		case 'L'://LEFT:
			// set bit 5
			ports[1] = ports[1] | 0x20;
			break;
		case 'R'://RIGHT:
			// set bit 6
			ports[1] = ports[1] | 0x40;
			break;
		case '\r'://P1_START:
			// set bit 2
			ports[1] = ports[1] | 0x04;
			break;
	}
}	
	
void MachineSI::key_up(char key) {
	switch (key) {
	case 'L'://LEFT:
		// clear bit 5
		ports[1] = ports[1] & 0xdf;
		break;
	case 'R'://RIGHT:
		// clear bit 6
		ports[1] = ports[1] & 0xbf;
		break;
	case ' '://FIRE:
		// clear bit 4
		ports[1] = ports[1] & 0xef;
		break;
	case '\r'://P1_START:
		// clear bit 2
		ports[1] = ports[1] & 0xfb;
		break;
	}
}	
	

void MachineSI::dump_info(std::ostream &file) {

	// ports as bits



	file << "\n---------- MACHINE DUMP ----------\n";
	file << "State: shift_x=" << std::hex << (unsigned int)shift_offset << ", shift_y=" << (unsigned int)shift_y << ", shift_offset=" << (unsigned int)shift_offset << "\n";
	file << "ports: [0]=" << std::bitset<8>(ports[0]) << ", [1]=" << std::bitset<8>(ports[1]) << ", [2]=" << std::bitset<8>(ports[2]) << ", [3]=" << std::bitset<8>(ports[3]);
	file << ", [4]=" << std::bitset<8> (ports[4]) << ", [5]=" << std::bitset<8> (ports[5]) << ", [6]=" << std::bitset<8> (ports[6]) << ", [7]=" << std::bitset<8> (ports[7]) << "\n";
	file << "\n---------- END OF MACHINE DUMP ----------\n";
}

// Processor is writing to port p
void MachineSI::out(uint8_t reg_a, uint8_t p) {
	switch (p)
	{
	// Has no function when written to
	case 0:
	case 1:
		break;
	// Shift function offset. Only 3 bits
	case 2:
		// does not overwrite port 2
		shift_offset = reg_a & 0x07;
		break;
	// Plays sound effects. bits 6 and 7 not used
	case 3:
		// Unimplemented for now
		// play sound effects
		break;
	// writes data to hardware shift function.
	// shifts x into y, then writes reg_a into x
	case 4:
		shift_y = shift_x;
		shift_x = reg_a;
		break;
	// Sound/Enemy movement
	case 5:
		// bits 6 and 7 not wired
		ports[5] = reg_a & 0x3f; // Unsure of actual implementation
		break;
	// Unsure of this ports purpose, could be debug related
	case 6:
		ports[6] = reg_a;
		break;
	// port 7 unused for writes
	default:
		std::cout << "\n\n Unimplemented hardware write: port #" << p << ", data=" << reg_a << "\n\n";
		exit(-1);
		break;
	}
}

// Processor is reading from port p
uint8_t MachineSI::in(uint8_t p) {
	uint8_t ret;
	switch (p) {
		// Has hardware mapping, but not used in Space Invaders code
		case 0:
			std::cout << "\n\n Read from port 0, unimplemented\n\n";
			exit(-1);
			break;
		// Game start and controls for player one
		case 1:
			// set bit 3 to 1
			ports[1] = ports[1] | 0x08;
			// set 7 to 0, not wired
			ports[1] = ports[1] & 0x7f;
			ret = ports[1];
			// bit 0 is reset after a read
			ports[1] = ports[1] & 0xfe;
			break;
		// controls for player two
		case 2:
			// unimplemented for right now
			//ret = ports[2];
			ret = 0;
			break;
		// Reads in data from bit shift function
		case 3: {
			// shift offset is only 3 bits, check to make sure it is
			// in the proper range
			if (shift_offset < 0 || shift_offset > 7) {
				std::cout << "\n\nHardware shift error: shift offset value out of bounds\n\n";
				exit(-1);
			}
			// TEST THIS
			uint16_t temp = (shift_x << 8 | shift_y);
			ret = (uint8_t)(temp >> (8 - shift_offset));
			break;
		}
		// rest of ports unsued for reading
		default:
			std::cout << "\n\n Unimplemented hardware read: port #" << p << "\n\n";
			exit(-1);
			break;
	}
	return ret;
}
