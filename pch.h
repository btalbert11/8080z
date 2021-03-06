#ifndef PCH_H
#define PCH_H

// TODO: add headers that you want to pre-compile here


// Standard Library files
#include <cstdint>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <Windows.h>

// My classes
#include "MachineSI.h"
#include "PlatformWindows.h"
#include "State8080.h"

// My defs and macros
#define z_DISPLAY_BUFFER_SIZE 7168 // 224 x 256 = 57,344 pixels, 1 bit a pixel, = 7,168 bytes
#endif //PCH_H
