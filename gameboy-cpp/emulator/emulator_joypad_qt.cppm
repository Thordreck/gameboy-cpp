module;
#include <QGuiApplication>
#include "emulator_joypad_qt.hpp"

export module emulator.joypad:qt;

import std;
import joypad;
import utilities;
import emulator.core;

namespace emulator
{
    export class joypad_source
    {
    public:
        [[nodiscard]] joypad::const_input_state_view_t read()
        {
            if (filter == nullptr) [[unlikely]]
            {
                const auto app = QGuiApplication::instance();
                (utils::assert)(app != nullptr);

                filter = std::make_unique<emulator_joypad_filter>(state);
                filter->moveToThread(app->thread());
                app->installEventFilter(filter.get());
            }

            return state;
        }

    private:
        std::unique_ptr<emulator_joypad_filter> filter { nullptr };
        std::array<bool, joypad::num_joypad_inputs> state {};
    };

}
