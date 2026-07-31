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

        void load_from_module(const std::string_view uri, const std::string_view module_name)
        {
            const auto connection = QObject::connect(&engine, &QQmlApplicationEngine::objectCreationFailed,
                    [] (const auto url)
                    { qFatal() << "Could not create qt object from module uri: " <<  url; });

            engine.loadFromModule(QString(uri.data()), QString(module_name.data()));
        }

        void clear_singletons() { engine.clearSingletons(); }
        void clear_component_cache() { engine.clearComponentCache(); }
        void add_import_path(const std::string_view path) { engine.addImportPath(QString(path.data())); }

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
