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


int main()
{

	RenderingEngine r(800, 600, "CoreEngine VK", true);


	Vertex v[] = { Vertex( Vector3f(-1, 0, 0), Vector2f(0, 0), Vector2f(0, 0), Vector3f(0, 0, 1), Vector3f(0, 1, 0)),
	               Vertex( Vector3f(-1, 1, 0), Vector2f(0, 0), Vector2f(0, 0), Vector3f(0, 0, 1), Vector3f(0, 1, 0)),
		           Vertex( Vector3f( 0, 1, 0), Vector2f(0, 0), Vector2f(0, 0), Vector3f(0, 0, 1), Vector3f(0, 1, 0)),
	               Vertex( Vector3f( 0, 0, 0), Vector2f(0, 0), Vector2f(0, 0), Vector3f(0, 0, 1), Vector3f(0, 1, 0)) };

	uint32_t i[] = { 0, 1, 2,
					 2, 3, 1, };
#if BUILD_WITH_RENDERING_BACKEND == RENDERING_BACKEND_OPENGL
	Shader s = Shader(r.makeShader(FileIO::loadAsciiFile("../Resources/Shaders/shader.vert"), FileIO::loadAsciiFile("../Resources/Shaders/shader.frag")));
#else
	Shader s = Shader(r.makeShader(FileIO::loadBinaryFile("../Resources/Shaders/vert.spv"), FileIO::loadBinaryFile("../Resources/Shaders/frag.spv")));
#endif

	r.useShader(s.getId());

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