#ifndef PLATFORMWINDOWS_H
#define PLATFORMWINDOWS_H

//#include <WinUser.h>

// This class holds all Windows platform specific code

class PlatformWindows
{
public:
	PlatformWindows();
	~PlatformWindows();

	// polls keyboard and updates machine ports
	void static poll_keyboard(MachineSI &);
	// gets current time as a long
	double static time();
};

#endif // !PLATFORMWINDOWS_H