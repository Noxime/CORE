/*
Core engine

Aaro Perämaa
2016
*/

#include "RenderingEngine.h"
#include "Mesh.h"
#include "FileIO.h"
#include "MeshRenderer.h"
#include "Shader.h"

#include <ctime>
#include "Main.h"


int main()
{

	//RenderingEngine.init(800, 600, "CoreEngine VK", false);
	RenderingEngine r(800, 600, "CoreEngine VK", false);


	Vertex v[] = { Vertex( Vector3f(-1, 0, 0), Vector2f(0, 0), Vector2f(0, 0), Vector3f(0, 0, 1), Vector3f(0, 1, 0)),
	               Vertex( Vector3f(-1, 1, 0), Vector2f(0, 0), Vector2f(0, 0), Vector3f(0, 0, 1), Vector3f(0, 1, 0)),
		           Vertex( Vector3f( 0, 1, 0), Vector2f(0, 0), Vector2f(0, 0), Vector3f(0, 0, 1), Vector3f(0, 1, 0)),
	               Vertex( Vector3f( 0, 0, 0), Vector2f(0, 0), Vector2f(0, 0), Vector3f(0, 0, 1), Vector3f(0, 1, 0)) };

	uint32_t i[] = { 0, 1, 2,
					 2, 3, 1, };

	Shader s = Shader(r.makeShader(FileIO::loadFile("../Resources/Shaders/vert.spv"), FileIO::loadFile("../Resources/Shaders/frag.spv")));

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