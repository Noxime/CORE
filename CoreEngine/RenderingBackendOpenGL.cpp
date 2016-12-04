/*
Core engine

Aaro Perämaa
2016
*/

#include "RenderingEngine.h"


#if BUILD_WITH_RENDERING_BACKEND == RENDERING_BACKEND_OPENGL

uint32_t RenderingEngine::makeShader(std::vector<char> vert, std::vector<char> frag)
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


	return program;
}

void RenderingEngine::useShader(uint32_t program)
{
	glUseProgram(program);
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

}

Window *RenderingEngine::createWindow(uint32_t width, uint32_t height, std::string title, bool vsyncRequested)
{
	m_window = new Window(width, height, title, vsyncRequested);

	return m_window;
}

void RenderingEngine::drawFrame()
{
	
	glClearColor(1, 0, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBegin(GL_TRIANGLES);

	glVertex3f(-1, 0, 0);
	glVertex3f( 1,-1, 0);
	glVertex3f( 0,-1, 0);

	glEnd();
}




RenderingEngine::~RenderingEngine()
{
	delete m_window; //Yeah no we dont want you
}

#endif