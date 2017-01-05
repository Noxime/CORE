/*
CoreEngine

Aaro Perämaa
2017
*/

#include "RenderingEngine.h"
#include "Mesh.h"
#include "FileIO.h"
#include "MeshRenderer.h"
#include "Shader.h"

#include <ctime>


int main()
{


	std::vector<Vertex> v = { Vertex(Vector3f(0.25f,0.25f, 0), Vector2f(0, 0), Vector2f(0, 0), Vector3f(0, 0, 1), Vector3f(0, 1, 0)),
							  Vertex(Vector3f(0.75f,0.75f, 0), Vector2f(1, 0), Vector2f(0, 0), Vector3f(0, 0, 1), Vector3f(0, 1, 0)),
							  Vertex(Vector3f(0.25f,0.75f, 0), Vector2f(0, 1), Vector2f(0, 0), Vector3f(0, 0, 1), Vector3f(0, 1, 0)),
							  Vertex(Vector3f(0.75f,0.25f, 0), Vector2f(1, 1), Vector2f(0, 0), Vector3f(0, 0, 1), Vector3f(0, 1, 0)), };
	
	std::vector<uint32_t> i = { 0, 1, 2,
								3, 1, 0, };





	RenderingEngine r(800, 600, "CoreEngine VK", true);
	


#if BACKEND == BACKEND_GL
	Shader s = r.makeShader(FileIO::loadAsciiFile("../Resources/Shaders/shader.vert"), FileIO::loadAsciiFile("../Resources/Shaders/shader.frag"));
#else
	Shader s = r.makeShader(FileIO::loadBinaryFile("../Resources/Shaders/vert.spv"),   FileIO::loadBinaryFile("../Resources/Shaders/frag.spv"));
#endif

	Mesh m = r.makeMesh(v, i);

	uint64_t frameCounter = 0;
	clock_t nextTime = clock() + CLOCKS_PER_SEC;


	while (r.run())
	{

		r.setUniform3f(s, "uTest", Vector3f(0, 1, 1));


		//r.clearFrame();
		r.drawMesh(m, s);



		if (clock() > nextTime)
		{
			

			nextTime = clock() + CLOCKS_PER_SEC;
			std::cout << "Render: " << 1.0 / frameCounter << "ms, in FPS: " << frameCounter << std::endl;
			frameCounter = 0;
		}


		frameCounter++;
	}

	
	return 0;
}