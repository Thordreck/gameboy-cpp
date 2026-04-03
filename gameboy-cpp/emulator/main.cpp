
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
        [&engine](const std::stop_token& ct)
        {
            while (!ct.stop_requested())
            {
                using namespace std::chrono_literals;
                utils::execute_for([&engine] () { engine.tick(); }, 238ns);
            }
        }
    };

    while (!ui.quit_app_requested())
    {
        ui.render();

        using namespace std::chrono_literals;
        utils::execute_for([&ui] () { ui.render(); }, 16ms);
    }

    emulation_thread.request_stop();
    return 0;
}
