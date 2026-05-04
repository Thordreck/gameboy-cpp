module;
#include <QQmlApplicationEngine>
#include <QtCore/QDebug>

export module qt:qml;

import std;

import :core;
import :internal;

namespace qt
{
    export class qml_engine
    {
    public:
        void load(const std::string_view file_path)
        {
            const auto connection = QObject::connect(&engine, &QQmlApplicationEngine::objectCreationFailed,
                    [] (const auto& url) { qFatal() << "Could not create qt object from uri:" <<  url; });

            engine.load(QString(file_path.data()));
        }

        [[nodiscard]] std::vector<object> root_objects() const
        {
            return engine.rootObjects()
                | std::views::transform([] (auto qt_object) { return internal::create<object>(qt_object); })
                | std::ranges::to<std::vector<object>>();
        }

    private:
        [[nodiscard]] QObject* qt_object() { return &engine; }

        QQmlApplicationEngine engine;
        friend internal;
    };

}
