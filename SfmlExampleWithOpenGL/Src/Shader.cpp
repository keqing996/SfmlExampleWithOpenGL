#include "Shader.h"

class Support
{
public:
	Support() = delete;

public:
	inline static char* infoLog = new char[1024];

	static std::string ReadFileString(const std::string& path)
	{
		std::ifstream file(path);
		if (!file.is_open())
		{
			std::cout << "File open fail: " << path << std::endl;
			return "";
		}

		std::stringstream strStream;
		strStream << file.rdbuf();
		file.close();

		return strStream.str();
	}

	static bool CheckCompileError(unsigned int id)
	{
		int success;

		::glGetShaderiv(id, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			::glGetShaderInfoLog(id, 1024, nullptr, infoLog);
			std::cout << "Shader Compile Error : " << infoLog << std::endl;
		}

		return success;
	}

	static bool CheckLinkError(unsigned int id)
	{
		int success;

		::glGetProgramiv(id, GL_LINK_STATUS, &success);
		if (!success)
		{
			::glGetProgramInfoLog(id, 1024, nullptr, infoLog);
			std::cout << "Program Linking Error : " << infoLog << std::endl;
		}

		return success;
	}
};

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath)
	: _programHandle(::glCreateProgram())
{
	CreateShader(GL_VERTEX_SHADER, vertexPath);
	CreateShader(GL_FRAGMENT_SHADER, fragmentPath);

	Link();
}

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath, const std::string& geometryPath)
	: _programHandle(::glCreateProgram())
{
	CreateShader(GL_VERTEX_SHADER, vertexPath);
	CreateShader(GL_FRAGMENT_SHADER, fragmentPath);
	CreateShader(GL_GEOMETRY_SHADER, geometryPath);

	Link();
}

Shader::~Shader()
{
	for (auto shaderHandle : _shaderHandles)
		::glDeleteShader(shaderHandle);

	::glDeleteProgram(_programHandle);
}

unsigned int Shader::GetHandle() const
{
	return _programHandle;
}

void Shader::SetUniformBool(const std::string& name, bool value) const 
{ 
	::glUniform1i(::glGetUniformLocation(_programHandle, name.c_str()), static_cast<int>(value));
}

void Shader::SetUniformInt(const std::string& name, int value) const 
{ 
	::glUniform1i(::glGetUniformLocation(_programHandle, name.c_str()), value);
}

void Shader::SetUniformFloat(const std::string& name, float value) const 
{ 
	::glUniform1f(::glGetUniformLocation(_programHandle, name.c_str()), value);
}

void Shader::SetUniformMat4(const std::string& name, glm::mat4 mat) const 
{ 
	::glUniformMatrix4fv(::glGetUniformLocation(_programHandle, name.c_str()), 1, GL_FALSE, value_ptr(mat));
}

void Shader::SetUniformVec3(const std::string& name, glm::vec3 vec) const
{
	::glUniform3fv(::glGetUniformLocation(_programHandle, name.c_str()), 1, &vec[0]);
}

void Shader::SetUniformVec3(const std::string& name, float x, float y, float z) const
{
	::glUniform3f(::glGetUniformLocation(_programHandle, name.c_str()), x, y, z);
}

void Shader::CreateShader(GLenum shaderType, const std::string& filePath)
{
	std::string shaderFileContent = Support::ReadFileString(filePath);
	const char* shaderSource = shaderFileContent.c_str();

	unsigned int shaderHandle = ::glCreateShader(shaderType);
	::glShaderSource(shaderHandle, 1, &shaderSource, nullptr);
	::glCompileShader(shaderHandle);

	bool success = Support::CheckCompileError(shaderHandle);
	if (success)
	{
		_shaderHandles.push_back(shaderHandle);
		::glAttachShader(_programHandle, shaderHandle);
	}
	else
	{
		::glDeleteShader(shaderHandle);
	}
}

void Shader::Link()
{
	::glLinkProgram(_programHandle);
	Support::CheckLinkError(_programHandle);
}
