#pragma once

#include "core/filesystem.h"

#include <glm/fwd.hpp>

namespace core
{
class Shader {
public:
	Shader() = default;
	Shader(const CoreShaderFile& vertex_file_name, const CoreShaderFile& fragment_file_name);
	Shader(const std::string& vertex_code, const std::string& fragment_code);
	~Shader();

	Shader(const Shader& other) = delete;
	Shader& operator=(const Shader& other) = delete;

	Shader(Shader&& other) noexcept
	{
		*this = std::move(other);
	}

	Shader& operator=(Shader&& other) noexcept
	{
		if (this == &other) {
			return *this;
		}
		m_program_id = other.m_program_id;
		m_is_valid = other.m_is_valid;

		// when moved, reset "other" program ID so that its destructor
		// don't destroy the program
		other.m_program_id = 0;

		return *this;
	}

	b8 is_valid() const
	{
		return m_is_valid;
	};

	void use() const;
	void set_bool(const std::string& name, b8 value) const;
	void set_int32(const std::string& name, i32 value) const;
	void set_float(const std::string& name, f32 value) const;
	void set_vec2(const std::string& name, const glm::vec2& value) const;
	void set_vec2(const std::string& name, f32 x, f32 y) const;
	void set_vec3(const std::string& name, const glm::vec3& value) const;
	void set_vec3(const std::string& name, f32 x, f32 y, f32 z) const;
	void set_vec4(const std::string& name, const glm::vec4& value) const;
	void set_vec4(const std::string& name, f32 x, f32 y, f32 z, f32 w) const;
	void set_mat2(const std::string& name, const glm::mat2& mat) const;
	void set_mat3(const std::string& name, const glm::mat3& mat) const;
	void set_mat4(const std::string& name, const glm::mat4& mat) const;

private:
	u32        m_program_id = 0;
	b8         m_is_valid = false;
	mutable b8 m_warned = false;
};
}  // namespace core
