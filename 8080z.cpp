#include "pch.h"
#include "GL/freeglut.h"

void debug_info(State8080 state) {
	std::ofstream output_file("State_dump.txt", std::ios::trunc);
	state.dump(output_file);
}

void display(void) {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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

	// initialize classes
	State8080 state = State8080(buffer);
	MachineSI machine;

	int i = 0;
	int p = 0;
	//std::ofstream debug_file("debug output.txt");

	double last_interrupt = 0;

	//Emulate program
	while (1) {

		// Dump state and machine info every thousand instructions
		i++;
		if (i >= 1000) {
			state.dump_state(std::cout);
			machine.dump_info(std::cout);
			i = 0;
		}
		// poll keyboard, change machine state
		PlatformWindows::poll_keyboard(machine);
		//emulate a single instruction
		state.emulate(machine);


		// generate an interrupt every 1/60th of a second (16.6667 ms)
		if (PlatformWindows::time() - last_interrupt > 1.0 / 60.0) {
			if (state.interrupts_enable) {
				state.generate_interrupt(2);
				last_interrupt = PlatformWindows::time();
			}
		}


	}



	// Dump emulator info
	debug_info(state);
	return 1;
}
