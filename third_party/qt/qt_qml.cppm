module;
#include <QQmlApplicationEngine>
#include <QtCore/QDebug>

export module qt:qml;

import std;

namespace qt
{
    export class qml_engine
    {
    public:
        explicit qml_engine(const std::string_view file_path)
        {
            QObject::connect(&engine, &QQmlApplicationEngine::objectCreationFailed,
                    [] (const auto& url) { qFatal() << "Could not create qt object from uri:" <<  url; });

            engine.load(QString(file_path.data()));
        }

    private:
        QQmlApplicationEngine engine;
    };

}
