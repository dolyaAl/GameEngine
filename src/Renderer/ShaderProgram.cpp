#include "ShaderProgram.h"

#include <glm/gtc/type_ptr.hpp>

#include <iostream>

namespace Renderer
{
	ShaderProgram::ShaderProgram(const std::string& vertexShader, const std::string& fragmentShader)
	{
		GLuint vertexShaderID;
		if (!createShader(vertexShader, GL_VERTEX_SHADER, vertexShaderID))
		{
			std::cerr << "VERTEX SHADER compile time error" << std::endl;
			return;
		}

		GLuint fragmentShaderID;
		if (!createShader(fragmentShader, GL_FRAGMENT_SHADER, fragmentShaderID))
		{
			std::cerr << "FRAGMENT SHADER compile time error" << std::endl;
			glDeleteShader(vertexShaderID);                                   //если вертексный создан, а фрагментный нет удаляем вертексный;
			return;
		}

		m_ID = glCreateProgram();
		glAttachShader(m_ID, vertexShaderID);
		glAttachShader(m_ID, fragmentShaderID);
		glLinkProgram(m_ID);

		GLint success;
		glGetProgramiv(m_ID, GL_LINK_STATUS, &success); //проверяем линковку, закидываем инфу в сакцес;
		if (!success)
		{
			GLchar infolog[1024];
			glGetShaderInfoLog(m_ID, 1024, nullptr, infolog);
			std::cerr << "ERROR::SHADER: Link time error: \n" << infolog << std::endl;
		}
		else
		{
			m_isCompiled = true;
		}
		glDeleteShader(vertexShaderID);   //удаляем шейдеры, т.к. шейдерная программа готова;
		glDeleteShader(fragmentShaderID);

	}

	ShaderProgram::~ShaderProgram()
	{
		glDeleteProgram(m_ID);           //удаляем программу по ее айди;
	}

	void ShaderProgram::use() const
	{
		glUseProgram(m_ID);              //вызываем программу по айди;
	}
	void ShaderProgram::setInt(const std::string& name, const GLint value)
	{
		glUniform1i(glGetUniformLocation(m_ID, name.c_str()), value);
	}
	void ShaderProgram::setMatrix4(const std::string& name, const glm::mat4& matrix)
	{
		glUniformMatrix4fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
	}
	ShaderProgram& ShaderProgram::operator=(ShaderProgram&& shaderProgram) noexcept
	{
		glDeleteProgram(m_ID);                      //удаляем существующую;
		m_ID = shaderProgram.m_ID;                  //переносим айди;
		m_isCompiled = shaderProgram.m_isCompiled;  //переносим флаг;

		shaderProgram.m_ID = 0;                     //зануляем, чтобы проблем с деструктором не было;
		shaderProgram.m_isCompiled = false;         //значение по умолчанию, чтобы проблем с деструктором не было;

		return *this;
	}
	ShaderProgram::ShaderProgram(ShaderProgram&& shaderProgram) noexcept
	{
		m_ID = shaderProgram.m_ID;                  //переносим айди;
		m_isCompiled = shaderProgram.m_isCompiled;  //переносим флаг;

		shaderProgram.m_ID = 0;                     //зануляем, чтобы проблем с деструктором не было;
		shaderProgram.m_isCompiled = false;         //значение по умолчанию, чтобы проблем с деструктором не было;
	}
	bool ShaderProgram::createShader(const std::string& source, const GLenum shaderType, GLuint& shaderID)
	{
		shaderID = glCreateShader(shaderType);
		const char* code = source.c_str();
		glShaderSource(shaderID, 1, &code, nullptr);
		glCompileShader(shaderID);

		GLint succes;
		glGetShaderiv(shaderID, GL_COMPILE_STATUS, &succes);
		if (!succes)
		{
			GLchar infolog[1024];
			glGetShaderInfoLog(shaderID, 1024, nullptr, infolog);
			std::cerr << "ERROR:SHADER: Compile time error: \n" << infolog << std::endl;
			return false;
		}
		return true;
	}
}

