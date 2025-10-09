#include "shader.h"

#include <glad/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <spdlog/spdlog.h>

namespace core
{
	namespace
	{
		static u32 create_shader(const std::string& code, u32 type, M_UNUSED std::string_view name)
		{
			i32 success = 0;
			u32 shader_id = glCreateShader(type);

			const char* source_ptr = code.c_str();
			glShaderSource(shader_id, 1, &source_ptr, nullptr);
			glCompileShader(shader_id);

			glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				char info_log[512];
				glGetShaderInfoLog(shader_id, 512, nullptr, (char*)info_log);
				SPDLOG_ERROR("\"Shader '{}' compilation failed: {}", name, (char*)info_log);
			}
			else
			{
				SPDLOG_INFO("Shader loaded OK: {}", name);
			}

			return shader_id;
		}

		static b8 compile_from_glsl_code(
		        const std::string& vertex_code, const std::string& fragment_code,
		        u32* out_program_id
		)
		{
			i32 success = 0;
			b8  is_valid = true;

			if (vertex_code.empty() || fragment_code.empty())
			{
				is_valid = false;
				SPDLOG_ERROR("Shader source was empty");
			}
			else
			{
				const u32 vertex_id = create_shader(vertex_code, GL_VERTEX_SHADER, "VERTEX");
				const u32 fragment_id =
				        create_shader(fragment_code, GL_FRAGMENT_SHADER, "FRAGMENT");
				*out_program_id = glCreateProgram();

				glAttachShader(*out_program_id, vertex_id);
				glAttachShader(*out_program_id, fragment_id);
				glLinkProgram(*out_program_id);

				glGetProgramiv(*out_program_id, GL_LINK_STATUS, &success);
				if (!success)
				{
					char info_log[512];
					glGetProgramInfoLog(*out_program_id, 512, nullptr, (char*)info_log);
					SPDLOG_ERROR("Shader program linking failed: {}", (char*)info_log);
					is_valid = false;
				}

				glDeleteShader(vertex_id);
				glDeleteShader(fragment_id);
			}

			return is_valid;
		}
	}  // namespace

	Shader::Shader(const CoreShaderFile& vertex_file_name, const CoreShaderFile& fragment_file_name)
	{
		auto vertex_source = fs::instance().read_file<std::string>(vertex_file_name);
		auto fragment_source = fs::instance().read_file<std::string>(fragment_file_name);

		m_is_valid = compile_from_glsl_code(vertex_source, fragment_source, &m_program_id);
	}

	Shader::Shader(const std::string& vertex_code, const std::string& fragment_code)
	{
		m_is_valid = compile_from_glsl_code(vertex_code, fragment_code, &m_program_id);
	}

	Shader::~Shader()
	{
		glDeleteProgram(m_program_id);
	}

	void Shader::use() const
	{
		if (m_is_valid)
		{
			glUseProgram(m_program_id);
		}
		else if (!m_warned)
		{
			SPDLOG_ERROR("Invalid shader");
			m_warned = true;
		}
	}

	void Shader::set_bool(const std::string& name, const b8 value) const
	{
		glUniform1i(glGetUniformLocation(m_program_id, name.c_str()), static_cast<i32>(value));
	}

	void Shader::set_int32(const std::string& name, const i32 value) const
	{
		glUniform1i(glGetUniformLocation(m_program_id, name.c_str()), value);
	}

	void Shader::set_float(const std::string& name, const f32 value) const
	{
		glUniform1f(glGetUniformLocation(m_program_id, name.c_str()), value);
	}

	void Shader::set_vec2(const std::string& name, const glm::vec2& value) const
	{
		glUniform2fv(glGetUniformLocation(m_program_id, name.c_str()), 1, glm::value_ptr(value));
	}

	void Shader::set_vec2(const std::string& name, const f32 x, const f32 y) const
	{
		glUniform2f(glGetUniformLocation(m_program_id, name.c_str()), x, y);
	}

	void Shader::set_vec3(const std::string& name, const glm::vec3& value) const
	{
		glUniform3fv(glGetUniformLocation(m_program_id, name.c_str()), 1, glm::value_ptr(value));
	}

	void Shader::set_vec3(const std::string& name, const f32 x, const f32 y, const f32 z) const
	{
		glUniform3f(glGetUniformLocation(m_program_id, name.c_str()), x, y, z);
	}

	void Shader::set_vec4(const std::string& name, const glm::vec4& value) const
	{
		glUniform4fv(glGetUniformLocation(m_program_id, name.c_str()), 1, glm::value_ptr(value));
	}

	void Shader::set_vec4(
	        const std::string& name, const f32 x, const f32 y, const f32 z, const f32 w
	) const
	{
		glUniform4f(glGetUniformLocation(m_program_id, name.c_str()), x, y, z, w);
	}

	void Shader::set_mat2(const std::string& name, const glm::mat2& value) const
	{
		glUniformMatrix2fv(
		        glGetUniformLocation(m_program_id, name.c_str()), 1, GL_FALSE, glm::value_ptr(value)
		);
	}

	void Shader::set_mat3(const std::string& name, const glm::mat3& value) const
	{
		glUniformMatrix3fv(
		        glGetUniformLocation(m_program_id, name.c_str()), 1, GL_FALSE, glm::value_ptr(value)
		);
	}

	void Shader::set_mat4(const std::string& name, const glm::mat4& value) const
	{
		glUniformMatrix4fv(
		        glGetUniformLocation(m_program_id, name.c_str()), 1, GL_FALSE, glm::value_ptr(value)
		);
	}
}  // namespace core
