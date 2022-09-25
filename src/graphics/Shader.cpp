#include "Shader.h"

#include <exception>
#include <fstream>
#include <iostream>
#include <sstream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/gtc/type_ptr.hpp>

Shader::Shader(unsigned int id) : id(id) {}

Shader::~Shader() {
	glDeleteProgram(id);
}

void Shader::use() {
	glUseProgram(id);
}

void Shader::uniformMatrix(std::string name, glm::mat4 matrix) {
	GLuint transformLoc = glGetUniformLocation(id, name.c_str());
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(matrix));
}


bool _compileShader(GLuint shader, const GLchar* code, std::string err_msg) {
	GLint  success;
	GLchar infolog[512];

	glShaderSource(shader, 1, &code, nullptr);
	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(shader, 512, nullptr, infolog);

		std::cerr << err_msg << std::endl;
		std::cerr << infolog << std::endl;

		return false;
	}

	return true;
}

Shader* load_shader(std::string vertexFile, std::string fragmentFile) {
	// Reading files
	std::string   vertexCode;
	std::string   fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;

	vShaderFile.exceptions(std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::badbit);

	try {
		std::stringstream vShaderStream, fShaderStream;

		vShaderFile.open(vertexFile);
		fShaderFile.open(fragmentFile);

		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();

		vShaderFile.close();
		fShaderFile.close();

		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure& e) {
		std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;

		return nullptr;
	}

	const GLchar* vShaderCode = vertexCode.c_str();
	const GLchar* fShaderCode = fragmentCode.c_str();

	GLuint vertex, fragment;
	GLint  success;
	GLchar infolog[512];

	// Vertex Shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	if (!_compileShader(vertex, vShaderCode, "SHADER::VERTEX: compilation failed")) {
		return nullptr;
	}

	// Fragment Shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	if (!_compileShader(fragment, fShaderCode, "SHADER::FRAGMENT: compilation failed")) {
		return nullptr;
	}

	// Shader Program
	GLuint id = glCreateProgram();

	// !!! This step is necessary, because my hardware
	// doesn't support location specifier in shader
	// e.g. layout (location = 0) in vec3 position;
	glBindAttribLocation(id, 0, "v_position");
	glBindAttribLocation(id, 1, "v_texCoord");

	glAttachShader(id, vertex);
	glAttachShader(id, fragment);
	glLinkProgram(id);

	glGetProgramiv(id, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(id, 512, nullptr, infolog);
		std::cerr << "SHADER::PROGRAM: linking failed" << std::endl;
		std::cerr << infolog << std::endl;

		glDeleteShader(vertex);
		glDeleteShader(fragment);

		return nullptr;
	}

	glDeleteShader(vertex);
	glDeleteShader(fragment);

	return new Shader(id);
}