#include "core/filesystem.hpp"

#include <SDL3/SDL_filesystem.h>
#include <physfs.h>
#include <spdlog/spdlog.h>

core::Filesystem::Filesystem(char** platform_argument)
{
	if (PHYSFS_init(std::strlen(platform_argument[0]) > 0 ? platform_argument[0] : nullptr))
	{
		SPDLOG_INFO("Virtual filesystem initialized.");
	}
	else
	{
		SPDLOG_CRITICAL("Virtual filesystem error: {}.\n", PHYSFS_getLastError());
	}

	fmt::format_to(std::back_inserter(m_content_root), "{}/contents", SDL_GetBasePath());

	if (PHYSFS_mount(m_content_root.c_str(), "/", false))
	{
		SPDLOG_DEBUG("Filesystem root '{}' mounted at '/' successfully.", m_content_root);
	}
	else
	{
		SPDLOG_CRITICAL(
		    "Cannot mount virtual filesystem from '{}': {}", m_content_root, PHYSFS_getLastError());
	}
}

core::Filesystem::~Filesystem()
{
	PHYSFS_unmount(m_content_root.c_str());
	PHYSFS_deinit();
}
