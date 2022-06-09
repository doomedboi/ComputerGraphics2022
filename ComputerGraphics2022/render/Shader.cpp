#include "Shader.h"


Shader::Shader(ID id) : id_(id) {}

void Shader::bind() {
	glUseProgram(id_.id_);
}

void Shader::unbind() {
	glUseProgram(0);
}

void Shader::setUniform(const std::string& fileName, const glm::mat4& mat) {
	glUniformMatrix4fv(glGetUniformLocation(id_.id_, fileName.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::setUniform(const std::string& fileName, const int val) {
	glUniform1i(glGetUniformLocation(id_.id_, fileName.c_str()), val);
}
void Shader::setUniform(const std::string& fileName, const float val) {
	glUniform1f(glGetUniformLocation(id_.id_, fileName.c_str()), val);
}
void Shader::setUniform(const std::string& fileName, const glm::vec3& vec) {
	glUniform3fv(glGetUniformLocation(id_.id_, fileName.c_str()), 1, &vec[0]);
}