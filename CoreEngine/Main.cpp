/*
Core engine

Aaro Perämaa
2016
*/

#include "RenderingEngine.h"

#include <ctime>

int main()
{

	RenderingEngine r;

	r.createWindow(800, 600, "CoreEngine VK", false);

	uint64_t frameCounter = 0;
	clock_t nextTime = clock() + CLOCKS_PER_SEC;

	while (r.run())
	{
		if (clock() > nextTime)
		{
			nextTime = clock() + CLOCKS_PER_SEC;
			std::cout << "FPS: " << frameCounter << std::endl;
			frameCounter = 0;
		}

		frameCounter++;
	}

	return 0;
}