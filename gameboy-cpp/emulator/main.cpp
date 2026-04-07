
import emulator;
import utilities;
import sdl;
import std;

int main()
{
    using namespace emulator;
    using namespace std::chrono_literals;

    engine engine{};

    // TODO: move to its own submodule
    std::jthread emulation_thread
    {
        [&engine](const std::stop_token& ct)
        {
            while (!ct.stop_requested())
            {
                utils::execute_for([&engine] { run_frame(engine); }
                    , frame_duration
                    , [](const auto& d) { sdl::delay_precise(d); });
            }
        }
    };

    const engine_input_source input_source{engine};
    engine_ui_adapter ui_adapter{engine};
    ui ui{ui_adapter};

    while (!ui.quit_app_requested())
    {
        utils::execute_for(
            [&]
            {
                input_source.update();
                ui.render();
            }
            , 16ms
            , [](const auto& d) { std::this_thread::sleep_for(d); });
    }

    emulation_thread.request_stop();
    return 0;
}
