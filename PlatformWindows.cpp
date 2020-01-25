#include "pch.h"
#include "PlatformWindows.h"


PlatformWindows::PlatformWindows()
{
}


PlatformWindows::~PlatformWindows()
{
}


void PlatformWindows::poll_keyboard(MachineSI & machine) {
	// Enter key is pressed
	if (GetKeyState(VK_RETURN) & 0x8000) {
		machine.key_down('\r');
		printf("ENTER KEY\n");
	}
	else {
		machine.key_up('\r');
	}

	// Left arrow key pressed
	if (GetKeyState(VK_LEFT) & 0x8000) {
		machine.key_down('L');
	}
	else {
		machine.key_up('L');
	}

	// Right arrow key pressed
	if (GetKeyState(VK_RIGHT) & 0x8000) {
		machine.key_down('R');
	}
	else {
		machine.key_up('R');
	}

	// space bar pressed
	if (GetKeyState(VK_SPACE) & 0x8000) {
		machine.key_down(' ');
	}
	else {
		machine.key_up(' ');
	}
	
}