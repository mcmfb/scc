/*
  SDL C++ Classes
  Copyright (C) 2017 Mateus Carmo M. de F. Barbosa
 
  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.
 
  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:
 
  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
*/

#include <iostream>
#include <iomanip>
#include <vector>
#include <SDL.h>
#include "config.hpp"
#include "rwops.hpp"

const int ERR_SDL_INIT = -1;

// another 256 bytes from /dev/urandom
const unsigned char memory[] = {
	0x3a, 0xc3, 0x17, 0x22, 0x25, 0xc9, 0x17, 0xb8, 0xd5, 0x60, 0x15, 0x23,
	0x35, 0x27, 0x29, 0x12, 0x6a, 0x6d, 0x68, 0xa3, 0x80, 0xcf, 0x81, 0xd9,
	0xc9, 0x78, 0xa7, 0x56, 0x65, 0x2e, 0xdf, 0x74, 0x23, 0x04, 0x44, 0x37,
	0x30, 0xfc, 0x83, 0x08, 0xa5, 0x8d, 0x78, 0xbe, 0x19, 0x3e, 0x67, 0x3d,
	0xbc, 0xae, 0x27, 0x17, 0x9c, 0x7c, 0xaa, 0xc0, 0xbf, 0xf4, 0xed, 0x45,
	0x54, 0xba, 0x58, 0xb4, 0xd2, 0x84, 0x85, 0x3b, 0x16, 0x01, 0x7d, 0x05,
	0x67, 0xf7, 0x6a, 0xbb, 0x2c, 0x28, 0xb5, 0xb5, 0x6b, 0x11, 0x91, 0x90,
	0x55, 0xd8, 0x49, 0x87, 0xb2, 0x18, 0xfa, 0x4a, 0x4f, 0xa0, 0x1d, 0x33,
	0x9c, 0x3b, 0x1d, 0x5e, 0xd5, 0x1a, 0x42, 0xf4, 0xb4, 0xb6, 0x7f, 0x50,
	0x5b, 0x27, 0xf8, 0xb1, 0x1e, 0xc6, 0xa7, 0xd9, 0x97, 0x0a, 0xb2, 0xcd,
	0xe2, 0xdb, 0xec, 0xfc, 0x78, 0x64, 0x5b, 0x3a, 0xbf, 0x97, 0xed, 0x6b,
	0xd5, 0xa2, 0x9c, 0xce, 0xa8, 0xcd, 0xf2, 0xde, 0x6d, 0xc7, 0x06, 0x91,
	0x75, 0x81, 0x49, 0x9a, 0xb3, 0xad, 0x78, 0x6f, 0x3b, 0x8a, 0x0a, 0x1d,
	0xe9, 0x66, 0xb8, 0x3e, 0x19, 0x19, 0xf8, 0x65, 0xb1, 0x24, 0x27, 0xbd,
	0xbe, 0x13, 0xd9, 0x26, 0x00, 0x0b, 0xcb, 0xe9, 0x9f, 0x30, 0x1b, 0x02,
	0x1a, 0xf5, 0xcf, 0xbb, 0xee, 0x00, 0x51, 0x95, 0x89, 0x7f, 0x2c, 0xbe,
	0x4e, 0x3b, 0x7c, 0x24, 0x48, 0x50, 0x76, 0xa1, 0xe9, 0x94, 0x64, 0xb8,
	0x93, 0x87, 0x85, 0x25, 0x9e, 0xeb, 0x55, 0x06, 0xcf, 0x27, 0xe1, 0x27,
	0xeb, 0x51, 0xac, 0xfb, 0x9c, 0xc8, 0xe4, 0x6f, 0xca, 0x91, 0xfe, 0xf7,
	0x7d, 0x38, 0x95, 0x1b, 0xe0, 0x8f, 0x17, 0xd8, 0xd5, 0x86, 0x42, 0xc6,
	0xcd, 0x88, 0x61, 0x11, 0xc0, 0xd0, 0xba, 0xd8, 0xe7, 0xaf, 0x84, 0x54,
	0x32, 0x62, 0x08, 0x59
};

const std::vector<int> BYTES_TO_READ{3, 251, 53, 162};

bool init(Uint32 sdlInitFlags)
{
	if(SDL_Init(sdlInitFlags) < 0) {
		return false;
	}
	return true;
}

void quit()
{
	SDL_Quit();
}

void printByteAt(SDL::RWops &constMem, int pos)
{
	char c;
	constMem.seek(pos, RW_SEEK_SET);
	size_t charsRead = constMem.read(&c, sizeof(char), 1);
	if(charsRead < 1) {
		std::cout << "RWops::read() failed" << std::endl;
		return;
	}

	std::cout << "byte 0x" << std::setbase(16)
		<< (static_cast<int>(c) & 0xff) << std::setbase(10)
		<< " at position " << pos << std::endl;
	constMem.seek(0, RW_SEEK_SET);
}

void test()
{
	SDL::RWops constMem(memory, sizeof(memory));

	// read. Positions are random just to make it more interesting.
	for(int i = 0; i < BYTES_TO_READ.size(); ++i) {
		printByteAt(constMem, BYTES_TO_READ[i]);
	}

	// write. Should not change the buffer, and should give us a warning
	const unsigned char someValue = 0x01;
	constMem.seek(0, RW_SEEK_SET); // just in case
	constMem.write(&someValue, sizeof(char), 1); // write at first pos

	std::cout << std::endl << "after attempting to write:" << std::endl;
	printByteAt(constMem, 0);
	std::cout << "writing error we've got: \"" << SDL_GetError() << "\""
		<< std::endl;
}

int main(int argc, char **argv)
{
	Uint32 sdlFlags = SDL_INIT_VIDEO;
	if(!init(sdlFlags)) {
		SDL_LogCritical(SDL_LOG_CATEGORY_ERROR,
			"couldn't initialize SDL\n");
		return ERR_SDL_INIT;
	}
	test();
	quit();
	return 0;
}
