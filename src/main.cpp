#include "core/event_handler.h"
#include "core/filesystem.h"
#include "core/renderer.h"
#include "core/window.h"
#include "dev_ui/dev_ui.h"
#include "utils/helper_macros.h"

#include <SDL3/SDL.h>
#include <tracy/Tracy.hpp>

using namespace core;

i32 main(M_UNUSED i32 argc, M_UNUSED char** argv)
{
	TracyNoop;

	// TODO: only if debug
	spdlog::set_level(spdlog::level::debug);

	fs::create(argv);

	Window window = Window::initialize_with_context({
	        .title = "Learning OpenGL",
	        .width = 960,
	        .h = 720,
	        .is_resizable = true,
	});

	// requires an initialized OpenGL context
	Renderer renderer {window};
	renderer.setup_rendering();

	EventHandler event_handler{ {
		    .windows_resizing_callback =
		            [&renderer](i32 new_x, i32 new_y) {
		                Window::on_window_resizing(new_x, new_y);
		                renderer.reset();
		            },
		    .windows_quit_callback = [&window]() { window.on_window_quit_event(); },
	} };

	event_handler.register_keyboard_input_handler(
	        [&renderer, &window](const EventHandler& handler) {
		        renderer.handle_input(handler);
		        window.handle_input(handler);
	        });

	while (window.should_stay_open()) {
		dev_ui::create_frame();
		event_handler.collect_input();
		event_handler.process_input();
		renderer.render();
		renderer.prepare_dev_ui();
		dev_ui::render_frame();
		window.gl_swap();
	}

	SDL_Quit();
	dev_ui::shutdown();
	fs::destroy();

	return 0;
}
