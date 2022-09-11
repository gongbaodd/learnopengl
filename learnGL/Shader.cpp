#include "Shader.h"

const char* readShaderFile(const char* path) {
	std::string code = "";
	std::ifstream file;

	file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try {
		file.open(path);
		std::stringstream stream;
		stream << file.rdbuf();
		file.close();
		code = stream.str();

	}
	catch (std::ifstream::failure e) {
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ:" << path << std::endl;
	}

	return code.c_str();
}

enum class ShaderType {
	Vertex = GL_VERTEX_SHADER,
	Fragment = GL_FRAGMENT_SHADER,
};

enum class ShaderError {
	Vertex,
	Fragment,
	Linking,
};

void handleShaderError(unsigned id, ShaderError e) {
	auto status = e == ShaderError::Linking ? GL_LINK_STATUS : GL_COMPILE_STATUS;

	int success;
	glGetProgramiv(id, status, &success);

	if (!success) {
		const char* error = "";
		switch (e)
		{
		case ShaderError::Vertex:
			error = "VERTEX::COMPILATION_FAILED\n";
			break;
		case ShaderError::Fragment:
			error = "FRAGMENT::COMPILATION_FAILED\n";
			break;
		case ShaderError::Linking:
			error = "PROGRAM::LINKING_FAILED\n";
			break;
		default:
			break;
		}
		char infoLog[512];
		glGetShaderInfoLog(id, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::" << error << infoLog << std::endl;
		/*
		std::ofstream myfile;
		myfile.open("error.txt");
		myfile << "ERROR::SHADER::" << error << infoLog;
		myfile.close();
		*/
	}
}

unsigned compileShader(const char* code, ShaderType type) {
	unsigned id = glCreateShader(GLenum(type));
	glShaderSource(id, 1, &code, NULL);
	glCompileShader(id);

	handleShaderError(id, type == ShaderType::Vertex? ShaderError::Vertex: ShaderError::Fragment);

	return id;
}

unsigned linkShader(unsigned vertex, unsigned fragment) {
	unsigned id = glCreateProgram();

	glAttachShader(id, vertex);
	glAttachShader(id, fragment);
	glLinkProgram(id);

	handleShaderError(id, ShaderError::Linking);

	return id;
}

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
	const char* vShaderCode = readShaderFile(vertexPath);
	const char* fShaderCode = readShaderFile(fragmentPath);

	auto vertex = compileShader(vShaderCode, ShaderType::Vertex);
	auto fragment = compileShader(fShaderCode, ShaderType::Fragment);
	ID = linkShader(vertex, fragment);
}

void Shader::use()
{
	glUseProgram(ID);
}

void Shader::setBool(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}