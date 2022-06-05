#include "./ShaderLoader.h"



ShaderLoader& ShaderLoader::singleton() {
    static ShaderLoader instance;
    return instance;
}

Shader ShaderLoader::load(const std::string& path) {
	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::string FragmentShaderCode;

	auto readShaderCode = [this](const std::string& vertex_path, std::string& shaderCode) {
		std::ifstream VertexShaderStream(vertex_path, std::ios::in);
		if (VertexShaderStream.is_open()) {
			std::stringstream sstr;
			sstr << VertexShaderStream.rdbuf();
			shaderCode = sstr.str();
			VertexShaderStream.close();
		}
		else {
			rvProtocol_->write("Can't open vertex file\n");
		}
	};

	readShaderCode(path + ".vs", VertexShaderCode);
	readShaderCode(path + ".fs", FragmentShaderCode);


	GLint Result = GL_FALSE;
	int InfoLogLength;

	auto compileAndCheckShader = [this](GLuint ShaderID, const std::string& ShaderCode) {
		// compile part
		rvProtocol_->write("Compiling shader\n");
		const char* pshaderCode = ShaderCode.c_str();
		glShaderSource(ShaderID, 1, &pshaderCode, NULL);
		glCompileShader(ShaderID);
		// check part
		int logLen = 0;
		GLint res;
		glGetShaderiv(ShaderID, GL_COMPILE_STATUS, &res);
		glGetShaderiv(ShaderID, GL_INFO_LOG_LENGTH, &logLen);
		if (logLen > 0) {
			std::string errorMsg(static_cast<size_t>(logLen) + 1, ' ');
			glGetShaderInfoLog(ShaderID, logLen, NULL, errorMsg.data());
			std::cout << errorMsg << std::endl;
		}
	};
	compileAndCheckShader(VertexShaderID, VertexShaderCode);
	compileAndCheckShader(FragmentShaderID, FragmentShaderCode);

	// Link the program
	rvProtocol_->write("Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::string errorMsg(static_cast<size_t>(InfoLogLength) + 1, ' ');
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, errorMsg.data());

		rvProtocol_->write(errorMsg);
	}


	glDetachShader(ProgramID, VertexShaderID);
	glDetachShader(ProgramID, FragmentShaderID);

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return Shader(ID(ProgramID));
}



