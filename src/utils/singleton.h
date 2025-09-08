#pragma once

#include "utils/assertions.h"

#include <spdlog/spdlog.h>

#include <string>

namespace core
{
template<typename T>
class Singleton {
public:
	static const T& instance()
	{
		check_msg(s_instance != nullptr, std::format("Singleton '{}' not initialized.", s_name));
		return *s_instance;
	}

	template<typename... Args>
	static void create(Args&&... args)
	{
		SPDLOG_INFO("Initializing '{}' singleton.", s_name);
		if (!s_instance) {
			s_instance = new T{ std::forward<Args>(args)... };
		}
	};

	static void destroy()
	{
		SPDLOG_INFO("De-initializing '{}' singleton.", s_name);
		delete s_instance;
	}

private:
	static T*               s_instance;
	static std::string_view s_name;
};

// NOLINTBEGIN(*-macro-parentheses)
#define DECLARE_SINGLETON(var_name, Type)                    \
	template<>                                               \
	inline Type* Singleton<Type>::s_instance = nullptr;      \
	template<>                                               \
	inline std::string_view Singleton<Type>::s_name = #Type; \
                                                             \
	using var_name = Singleton<Type>;  // NOLINT(*-macro-parentheses)
// NOLINTEND(*-macro-parentheses)

}  // namespace core
