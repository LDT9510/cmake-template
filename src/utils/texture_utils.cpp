#include "texture_utils.h"

#include "core/filesystem.h"

#include "glad/gl.h"
#include "stb/stb_image.h"

#include <array>

void load_texture(std::string_view file_name, core::u32 handle)
{
	using namespace core;

	// little hack
	bool has_alpha = file_name.ends_with(".png");
	
	// load texture from image
	// load and generate the texture
	int  width, height, nr_channels;
	auto image_contents =
	        core::fs::instance().read_file<std::vector<u8>>(core::CoreTextureFile(file_name));

	// flip the image for OpenGL
	stbi_set_flip_vertically_on_load(true);
	
	unsigned char* data = stbi_load_from_memory(image_contents.data(),
	                                            static_cast<i32>(image_contents.size()), &width,
	                                            &height, &nr_channels, 0);
	
	if (data != nullptr) {
		// load texture data and configure
		glBindTexture(GL_TEXTURE_2D, handle);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, has_alpha ? GL_RGBA: GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		// set the texture wrapping/filtering options (on the currently bound texture object)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		static constexpr std::array BORDER_COLOR = { 1.0f, 1.0f, 0.0f, 1.0f };
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, BORDER_COLOR.data());
		stbi_image_free(data);
	} else {
		SPDLOG_ERROR("Failed to load texture");
	}
}