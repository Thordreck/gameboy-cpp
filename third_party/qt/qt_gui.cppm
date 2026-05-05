module;
#include <QGuiApplication>
#include <QIcon>

export module qt:gui;
import std;

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

        std::expected<void, std::string> set_window_icon(const std::string_view path)
        {
            const QIcon icon { path.data() };

            if (icon.isNull())
            {
                return std::unexpected { std::format("Could not load window icon from path {}", path) };
            }

            app.setWindowIcon(icon);
            return {};
        }

    private:
        QGuiApplication app;
    };

}