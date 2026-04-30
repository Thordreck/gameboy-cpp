module;
#include <QQmlApplicationEngine>

export module qt:qml;
import std;

namespace qt
{
    export class qml_engine
    {
    public:
        explicit qml_engine(const std::string_view file_path)
        {
            engine.load(QString(file_path.data()));
        }

    private:
        QQmlApplicationEngine engine;
    };

}
