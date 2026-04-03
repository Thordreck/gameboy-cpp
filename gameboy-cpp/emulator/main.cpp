
import emulator;
import utilities;
import std;

int main()
{
    using namespace emulator;

    engine engine{};
    engine_ui_adapter adapter{engine};
    ui ui{adapter};

    std::jthread emulation_thread
    {
        [&engine](const std::stop_token ct)
        {
            while (!ct.stop_requested())
            {
                // TODO: OS scheduling usually works at milliseconds, not nanoseconds. We need an alternative way
                // fort hsi loop
                using namespace std::chrono_literals;
                constexpr auto master_clock_period = 238ns;

                const auto elapsed = utils::execution_time([&engine] () { engine.tick(); });
                //std::this_thread::sleep_for(master_clock_period - elapsed);
            }
        }
    };

    while (!ui.quit_app_requested())
    {
        ui.render();

        using namespace std::chrono_literals;
        std::this_thread::sleep_for(16ms); // ~60 FPS
    }

    emulation_thread.request_stop();
    return 0;
}
