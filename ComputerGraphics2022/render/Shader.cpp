#include "Shader.h"


Shader::Shader(ID id) : id_(id) {}

void Shader::bind() {
	glUseProgram(id_.id_);
}

void Shader::unbind() {
	glUseProgram(0);
}

void Shader::setUniform(const std::string& fileName, const glm::mat4& mat) {
	// TODO: make overload - tree
	glUniformMatrix4fv(glGetUniformLocation(id_.id_, fileName.c_str()), 1, GL_FALSE, &mat[0][0]);
}