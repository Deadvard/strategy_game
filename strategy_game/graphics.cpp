#include "graphics.h"

#include <fstream>
#include <sstream>
#include <string>
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


void checkCompileErrors(unsigned int shader, const char* type);

unsigned int createShader(const char* vertexPath, const char* fragmentPath)
{
	// 1. retrieve the vertex/fragment source code from filePath
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;

	// open files
	vShaderFile.open(vertexPath);
	fShaderFile.open(fragmentPath);
	std::stringstream vShaderStream;
	std::stringstream fShaderStream;

	// read file's buffer contents into streams
	vShaderStream << vShaderFile.rdbuf();
	fShaderStream << fShaderFile.rdbuf();

	// close file handlers
	vShaderFile.close();
	fShaderFile.close();

	// convert stream into string
	vertexCode = vShaderStream.str();
	fragmentCode = fShaderStream.str();

	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();

	// vertex shader
	unsigned int vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	checkCompileErrors(vertex, "VERTEX");

	// fragment Shader
	unsigned int fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	checkCompileErrors(fragment, "FRAGMENT");

	// shader Program
	unsigned int shader = glCreateProgram();
	glAttachShader(shader, vertex);
	glAttachShader(shader, fragment);
	glLinkProgram(shader);
	checkCompileErrors(shader, "PROGRAM");

	// delete the shaders as they're linked into our program now and no longer necessary
	glDeleteShader(vertex);
	glDeleteShader(fragment);

	return shader;
}

unsigned int createShader(const char* computePath)
{
	// 1. retrieve the vertex/fragment source code from filePath
	std::string computeCode;
	std::ifstream cShaderFile;

	// open files
	cShaderFile.open(computePath);
	std::stringstream cShaderStream;

	// read file's buffer contents into streams
	cShaderStream << cShaderFile.rdbuf();

	// close file handlers
	cShaderFile.close();

	// convert stream into string
	computeCode = cShaderStream.str();

	const char* cShaderCode = computeCode.c_str();

	// vertex shader
	unsigned int compute = glCreateShader(GL_COMPUTE_SHADER);
	glShaderSource(compute, 1, &cShaderCode, NULL);
	glCompileShader(compute);
	checkCompileErrors(compute, "COMPUTE");

	// shader Program
	unsigned int shader = glCreateProgram();
	glAttachShader(shader, compute);
	glLinkProgram(shader);
	checkCompileErrors(shader, "PROGRAM");

	// delete the shaders as they're linked into our program now and no longer necessary
	glDeleteShader(compute);

	return shader;
}

void checkCompileErrors(unsigned int shader, const char* type)
{
	int success;
	char infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			printf("ERROR::SHADER_COMPILATION_ERROR of type: %s\n%s\n-- --------------------------------------------------- -- \n", type, infoLog);

		}
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			printf("ERROR::SHADER_COMPILATION_ERROR of type: %s\n%s\n-- --------------------------------------------------- -- \n", type, infoLog);
		}
	}
}

graphics create_graphics()
{
	graphics g = {};

	g.shader = createShader("resources/shaders/shadermain.vert", "resources/shaders/shadermain.frag");

	// Configure VAO/VBO
	float quad[]
	{
		// positions     // uv
		1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 0.0f, 0.0f, 1.0f,

		0.0f, 0.0f, 0.f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.f, 0.0f, 1.0f
	};

	glGenVertexArrays(1, &g.quad_vao);
	glGenBuffers(1, &g.quad_vbo);

	glBindBuffer(GL_ARRAY_BUFFER, g.quad_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad, GL_STATIC_DRAW);

	glBindVertexArray(g.quad_vao);
	
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

	glGenBuffers(1, &g.instance_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, g.instance_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * 2000, 0, GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*)0);
	glVertexAttribDivisor(2, 1);

	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*)(sizeof(float) * 3));
	glVertexAttribDivisor(3, 1);


	g.view = glm::lookAt(glm::vec3(0, 0, 1), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	g.projection = glm::perspective(glm::radians(90.0f), 16.0f / 9.0f, 0.01f, 1000.0f);

	return g;
}

static const glm::vec3 colors[]
{
	glm::vec3(0.3f,0.3f,0.3f),
	glm::vec3(0.2f,1.0f,0.2f),
	glm::vec3(1.0f,0.2f,0.2f),
	glm::vec3(0,0,0),
	glm::vec3(1,1,1)
};

void uniform(unsigned int shader, const char* name, const glm::mat4& value)
{
	glUniformMatrix4fv(glGetUniformLocation(shader, name),
		1, GL_FALSE, glm::value_ptr(value));
}

void render(graphics* g, game_data* gd)
{
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//g->view = glm::rotate(g->view, 10.0f * (float)gd->dt, glm::vec3(1, 0, 0));

	static glm::vec3* pts = new glm::vec3[2000];
	int numV = 0;
	g->num_quads = 0;
	for (int i = 0; i < (int)(gd->x * gd->y); ++i)
	{
		int x = i % gd->x;
		int y = i / gd->x;

		pts[numV++] = glm::vec3(x, y, 0.0f);
		pts[numV++] = glm::vec3(x* 0.1f, y * 0.1f, 0.0f); //colors[gd->tilemap[i]];
		++g->num_quads;
	}

	glBindBuffer(GL_ARRAY_BUFFER, g->instance_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * numV, pts, GL_DYNAMIC_DRAW);

	glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(-1.0f, -1.0f, 0.0f));
	//model = glm::scale(model, glm::vec3(0.1f));
	glUseProgram(g->shader);
	uniform(g->shader, "model", model);
	uniform(g->shader, "view", g->view);
	uniform(g->shader, "projection", g->projection);
	glBindVertexArray(g->quad_vao);
	glDrawArraysInstanced(GL_TRIANGLES, 0, 6, g->num_quads);
}