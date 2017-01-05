/*
Core engine

Aaro Perämaa
2016
*/

#include "RenderingEngine.h"


#if BACKEND == BACKEND_GL

Shader RenderingEngine::makeShader(std::vector<char> vert, std::vector<char> frag)
{
	GLuint vertId = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragId = glCreateShader(GL_FRAGMENT_SHADER);

	const GLchar *vertSource = (const GLchar *)Util::charVec2cStr(vert);
	glShaderSource(vertId, 1, &vertSource, 0);
	const GLchar *fragSource = (const GLchar *)Util::charVec2cStr(frag);
	glShaderSource(fragId, 1, &fragSource, 0);


	//Compile the vertex shader
	glCompileShader(vertId);
	glCompileShader(fragId);

	GLint isCompiled = 0;
	glGetShaderiv(vertId, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(vertId, GL_INFO_LOG_LENGTH, &maxLength);

		//The maxLength includes the NULL character
		std::vector<char> infoLog(maxLength);
		glGetShaderInfoLog(vertId, maxLength, &maxLength, &infoLog[0]);

		//We don't need the shader anymore.
		glDeleteShader(vertId);

		std::cout << "GL: " << std::string(infoLog.data()).c_str() << std::endl;
	}

	glGetShaderiv(fragId, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(fragId, GL_INFO_LOG_LENGTH, &maxLength);

		//The maxLength includes the NULL character
		std::vector<char> infoLog(maxLength);
		glGetShaderInfoLog(fragId, maxLength, &maxLength, &infoLog[0]);

		//We don't need the shader anymore.
		glDeleteShader(fragId);

		std::cout << "GL: " << std::string(infoLog.data()).c_str() << std::endl;
	}

	GLuint program = glCreateProgram();

	glAttachShader(program, vertId);
	glAttachShader(program, fragId);



	glBindAttribLocation(program, Vertex::positionOffset,  "vPosition");
	glBindAttribLocation(program, Vertex::texcoord0Offset, "vTexcoord0");
	glBindAttribLocation(program, Vertex::texcoord1Offset, "vTexcoord1");
	glBindAttribLocation(program, Vertex::normalOffset,    "vNormal");
	glBindAttribLocation(program, Vertex::tangentOffset,   "vTangent");

	glLinkProgram(program);

	GLint isLinked = 0;
	glGetProgramiv(program, GL_LINK_STATUS, (int *)&isLinked);
	if (isLinked == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

		//The maxLength includes the NULL character
		std::vector<char> infoLog(maxLength);
		glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

		//We don't need the program anymore.
		glDeleteProgram(program);
		//Don't leak shaders either.
		glDeleteShader(vertId);
		glDeleteShader(fragId);

		//Use the infoLog as you see fit.

		std::cout << "GL: " << std::string(infoLog.data()).c_str() << std::endl;
	}

	

	//Always detach shaders after a successful link.
	glDetachShader(program, vertId);
	glDetachShader(program, fragId);


	return Shader(program);
}



Mesh RenderingEngine::makeMesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices)
{

	std::vector<glm::vec3> position = std::vector<glm::vec3>();
	std::vector<glm::vec2> tex0     = std::vector<glm::vec2>();
	std::vector<glm::vec2> tex1     = std::vector<glm::vec2>();
	std::vector<glm::vec3> normal   = std::vector<glm::vec3>();
	std::vector<glm::vec3> tangent  = std::vector<glm::vec3>();

	position.reserve(vertices.size());
	tex0.reserve(    vertices.size());
	tex1.reserve(    vertices.size());
	normal.reserve(  vertices.size());
	tangent.reserve( vertices.size());

	for each (Vertex v in vertices)
	{
		position.push_back(v.getPos());
		tex0.push_back(    v.getTex0());
		tex1.push_back(    v.getTex1());
		normal.push_back(  v.getNorm());
		tangent.push_back( v.getTang());
	}

	uint32_t vao = 0;
	uint32_t elemBuf = 0;

	uint32_t posBuf = 0;
	uint32_t tex0Buf = 0;
	uint32_t tex1Buf = 0;
	uint32_t normBuf = 0;
	uint32_t tangBuf = 0;
	

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &posBuf);

	glBindBuffer(GL_ARRAY_BUFFER, posBuf);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(position[0]), &position[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(Vertex::positionOffset);
	glVertexAttribPointer(    Vertex::positionOffset, Vertex::positionSize, GL_FLOAT, GL_FALSE, 0, 0);


	glGenBuffers(1, &tex0Buf);
	
	glBindBuffer(GL_ARRAY_BUFFER, tex0Buf);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(tex0[0]), &tex0[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(Vertex::texcoord0Offset);
	glVertexAttribPointer(    Vertex::texcoord0Offset, Vertex::texcoord0Size, GL_FLOAT, GL_FALSE, 0, 0);


	glGenBuffers(1, &tex1Buf);

	glBindBuffer(GL_ARRAY_BUFFER, tex1Buf);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(tex1[0]), &tex1[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(Vertex::texcoord1Offset);
	glVertexAttribPointer(    Vertex::texcoord1Offset, Vertex::texcoord1Size, GL_FLOAT, GL_FALSE, 0, 0);



	glGenBuffers(1, &normBuf);
	
	glBindBuffer(GL_ARRAY_BUFFER, normBuf);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(normal[0]), &normal[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(Vertex::normalOffset);
	glVertexAttribPointer(    Vertex::normalOffset, Vertex::normalSize, GL_FLOAT, GL_FALSE, 0, 0);



	glGenBuffers(1, &tangBuf);

	glBindBuffer(GL_ARRAY_BUFFER, tangBuf);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(tangent[0]), &tangent[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(Vertex::tangentOffset);
	glVertexAttribPointer(    Vertex::tangentOffset, Vertex::tangentSize, GL_FLOAT, GL_FALSE, 0, 0);

	
	glGenBuffers(1, &elemBuf);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elemBuf);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(indices[0]), &indices[0], GL_STATIC_DRAW);

	//glGenBuffers(1, &elements);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elements);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ind), ind, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	return Mesh(vao, elemBuf, vertices.size(), indices.size());
}

void RenderingEngine::setUniform2f(Shader target, std::string name, glm::vec2 vec)
{
	glUseProgram(target.getId());
	glUniform2f(glGetUniformLocation(target.getId(), name.c_str()), vec.x, vec.y);
}

void RenderingEngine::setUniform3f(Shader target, std::string name, glm::vec3 vec)
{
	glUniform3f(glGetUniformLocation(target.getId(), name.c_str()), vec.x, vec.y, vec.z);
}

void RenderingEngine::drawMesh(Mesh mesh, Shader shader)
{
	glBindBuffer(GL_ARRAY_BUFFER,         mesh.getVBO());
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.getEB());
	glUseProgram(shader.getId());

	glBindVertexArray(mesh.getVBO());

	glDrawElements(GL_TRIANGLES, mesh.getIndexCount(), GL_UNSIGNED_INT, 0);
	//glDrawArrays(GL_TRIANGLES, 0, 3);
}




RenderingEngine::RenderingEngine(uint32_t width, uint32_t height, std::string title, bool vsyncRequested)
{
	glfwInit();
	createWindow(width, height, title, vsyncRequested);
	
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		/* Problem: glewInit failed, something is seriously wrong. */
		std::cout << "GL: " << glewGetErrorString(err) << std::endl;
	}

	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);

}

Window *RenderingEngine::createWindow(uint32_t width, uint32_t height, std::string title, bool vsyncRequested)
{
	m_window = new Window(width, height, title, vsyncRequested);

	return m_window;
}

void RenderingEngine::clearFrame()
{
	
	glClearColor(1, 0, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	GLint err = glGetError();
	if (err != GL_NO_ERROR)
	{
		std::cerr << "GL: " << err << ": " << glewGetErrorString(err) << std::endl;
	}
}




RenderingEngine::~RenderingEngine()
{
	
	delete m_window; //Yeah no we dont want you
}

#endif