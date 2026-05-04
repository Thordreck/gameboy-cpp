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
            if (!attached) [[unlikely]]
            {
                const auto app = QGuiApplication::instance();
                utils::assert_not_nullptr(app);

                auto* filter = new emulator_joypad_filter(state);
                filter->moveToThread(app->thread());
                filter->setParent(app);
                app->installEventFilter(filter);

                attached = true;
            }

            return state;
        }

    private:
        bool attached { false };
        std::array<bool, joypad::num_joypad_inputs> state {};
    };

}
