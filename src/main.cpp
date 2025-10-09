#include "core/event_handler.h"
#include "core/filesystem.h"
#include "core/renderer.h"
#include "core/camera.h"
#include "core/timing.h"
#include "core/window.h"
#include "dev_ui/dev_ui.h"
#include "utils/helper_macros.h"

#include <SDL3/SDL.h>
#include <glad/gl.h>
#include <tracy/Tracy.hpp>
#include <tracy/TracyOpenGL.hpp>
#include <imgui/imgui.h>

using namespace core;

i32 main(M_UNUSED i32 argc, M_UNUSED char** argv)
{
	TracyNoop;

#if DEV_BUILD
	spdlog::set_level(spdlog::level::debug);
#else
	spdlog::set_level(spdlog::level::info);
#endif

	fs::create(argv);

	Window window = Window::initialize_with_context(
	        {
	                .title = "Learning OpenGL",
	                .width = 960,
	                .h = 720,
	                .is_resizable = true,
	        }
	);
	TracyGpuContext;

	Camera camera{ { 0.0f, 0.0f, 3.0f } };

	// requires an initialized OpenGL context
	Renderer renderer{ window, camera };
	renderer.setup_rendering();

	auto resizing_callback = [&renderer](i32 new_x, i32 new_y)
	{
		Window::on_window_resizing(new_x, new_y);
		renderer.reset();
	};

	auto quit_callback = [&window]()
	{
		window.on_window_quit_event();
	};

	EventHandler event_handler{ { .windows_resizing_callback = resizing_callback,
		                          .windows_quit_callback = quit_callback } };

	event_handler.register_keyboard_input_handler([&renderer, &window, &camera](EventHandler& handler)
	{
		renderer.handle_input(handler);
		window.handle_input(handler);

		// hack TODO: rethink the whole input system
		if (!handler.is_mouse_captured())
		{
			camera.handle_input(handler);
		}
	});

	event_handler.register_mouse_offset_callback([&camera](f32 x_offset, f32 y_offset)
	{
		camera.on_mouse_movement(x_offset, y_offset);
	});

	event_handler.register_mouse_wheel_direction_callback([&camera](f32 mouse_wheel_direction)
	{
		camera.on_mouse_wheel_scroll(mouse_wheel_direction);
	});

	while (window.should_stay_open())
	{
		core::timing::update_delta_time();
		dev_ui::create_frame();
		event_handler.collect_input();
		event_handler.process_input();
		renderer.render();

		if (event_handler.is_mouse_captured())
		{
			ImGui::Begin("Learning OpenGL");
			dev_ui::prepare_shortcuts_ui();
			renderer.prepare_dev_ui();
			camera.prepare_dev_ui();
			ImGui::End();
		}

		dev_ui::render_frame();

		window.gl_swap();
		FrameMark;
		TracyGpuCollect;
	}

	SDL_Quit();
	dev_ui::shutdown();
	fs::destroy();

	return 0;
}
