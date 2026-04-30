
export module emulator.ui:qt;

import qt;
import utilities;
import emulator.core;

namespace emulator
{
    export class graphical_interface
    {
    public:
        graphical_interface(int& argc, char** argv)
            : app { argc, argv }
            , engine { ":/qt/qml/emulator/ui/emulator_ui_qt" }
        {}

        template <Emulator Imp>
        int render(Imp& emulator)
        {
            return app.execute();
        }

    private:
        qt::gui_application app;
        qt::qml_engine engine;
    };

}
