#include "pch.h"
#include "PlatformWindows.h"


PlatformWindows::PlatformWindows()
{
}


PlatformWindows::~PlatformWindows()
{
}


void PlatformWindows::pollKeyboard(MachineSI & machine) {
	/*// Enter key is pressed
	if (GetKeyState(VK_RETURN) & 0x8000) {
		machine_key_down(machine, '\r');
		printf("ENTER KEY\n");
	}
	else {
		machine_key_up(machine, '\r');
	}

	// Left arrow key pressed
	if (GetKeyState(VK_LEFT) & 0x8000) {
		machine_key_down(machine, 'L');
	}
	else {
		machine_key_up(machine, 'L');
	}

	// Right arrow key pressed
	if (GetKeyState(VK_RIGHT) & 0x8000) {
		machine_key_down(machine, 'R');
	}
	else {
		machine_key_up(machine, 'R');
	}

	// space bar pressed
	if (GetKeyState(VK_SPACE) & 0x8000) {
		machine_key_down(machine, ' ');
	}
	else {
		machine_key_up(machine, ' ');
	}
	*/
}