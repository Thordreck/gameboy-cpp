module;
#include <QGuiApplication>

export module qt:gui;

namespace qt
{
    export class gui_application
    {
    public:
        gui_application(int& argc, char** argv)
            : app { argc, argv }
        {}

        [[nodiscard]] int execute() const
        {
            return app.exec();
        }

    private:
        QGuiApplication app;
    };

}