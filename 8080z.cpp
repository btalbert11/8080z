// 8080z.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"

void debug_info(State8080 state) {
	std::ofstream output_file("State_dump.txt", std::ios::trunc);
	state.dump(output_file);
}

int main(int argc, char ** argv)
{
	std::cout << "Hello World!\n";

	// Open File and load the program into memory
	std::ifstream f(argv[1], std::ios::binary | std::ios::ate);
	if (!f.is_open()) {
		printf("Error opening %s\n", argv[1]);
		exit(1);
	}
	std::streamsize size = f.tellg();
	f.seekg(0, std::ios::beg);

	std::vector<uint8_t> buffer(65535); // Size of 16 bit memory address space

	if (!f.read((char *)buffer.data(), size)) {
		std::cout << "Program failed to load into memory\n";
		exit(-1);
	}
	f.close();


	State8080 state(buffer);

	// Emulate program
	//while (1) {
		// emulate a single instruction
	//state.emulate();
	//}



	// Dump emulator info
	debug_info(state);
}
