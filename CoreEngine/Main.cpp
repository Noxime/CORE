/*
Core engine

Aaro Perämaa
2016
*/

#include "RenderingEngine.h"

int main()
{

	RenderingEngine r;

	r.createWindow(800, 600, "CoreEngine VK", true);

	while (r.run())
	{

	}

	return 0;
}