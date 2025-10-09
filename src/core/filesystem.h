#pragma once

#include "core/types.h"
#include "utils/singleton.h"

#include <fmt/format.h>
#include <physfs.h>

#include <filesystem>
#include <string>

namespace core
{

	template<class T>
	concept CoreFile = requires {
		{ T::PATH } -> std::convertible_to<const char*>;
	};

	struct ShaderFile
	{
		static constexpr char PATH[] = "shaders";
	};

	struct TextureFile
	{
		static constexpr char PATH[] = "textures";
	};

	template<CoreFile TBasePath>
	class FileType
	{
	public:
		explicit FileType(std::string_view path)
		        : m_path{ fmt::format("/{}/{}", TBasePath::PATH, path) }
		{
		}

		const std::string& get_path_name() const
		{
			return m_path;
		}

	private:
		std::string m_path;
	};

	using CoreShaderFile = FileType<ShaderFile>;
	using CoreTextureFile = FileType<TextureFile>;

	class Filesystem
	{
		template<typename T>
		void read_file_internal(const std::string& file_name, T* out_buf) const
		{
			if (auto* file = PHYSFS_openRead(file_name.c_str()))
			{
				i64 file_size = PHYSFS_fileLength(file);
				out_buf->resize(file_size);
				PHYSFS_readBytes(file, out_buf->data(),
				                 file_size);  // assume success for now
				PHYSFS_close(file);
			}
			else
			{
				SPDLOG_ERROR("Cannot open '{}': {}", file_name, PHYSFS_getLastError());
			}
		}

	public:
		~Filesystem();

		Filesystem(const Filesystem& other) = delete;
		Filesystem& operator=(const Filesystem& other) = default;
		Filesystem(Filesystem&& other) noexcept = default;
		Filesystem& operator=(Filesystem&& other) noexcept = default;

		template<typename TOutput, CoreFile TBaseDir>
		TOutput read_file(const FileType<TBaseDir>& f_type) const
		{
			auto    p = f_type.get_path_name();
			TOutput buf;
			read_file_internal(p, &buf);
			return buf;
		}

		// NOLINTNEXTLINE(*-convert-member-functions-to-static)
		bool file_exists(const std::string& file_name) const
		{
			return PHYSFS_exists(file_name.c_str());
		}

	private:
		explicit Filesystem(char** platform_argument);

		std::string m_content_root;

		friend Singleton<Filesystem>;
	};

	// ReSharper disable once CppInconsistentNaming
	DECLARE_SINGLETON(fs, Filesystem);
}  // namespace core
