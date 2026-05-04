
#include "emulator_ui_qt.hpp"

namespace emulator
{
    static QVariantMap toQtVariantMap(const std::expected<void, std::string>& result)
    {
        return result.has_value()
            ? QVariantMap{ { "success", true } }
            : QVariantMap{ {"success", false }, { "error", result.error().data() }};
    }

    QVariantMap emulator_ui_backend::load_rom(const QUrl& url)
    {
        return toQtVariantMap(load_rom_fn(url.toLocalFile().toStdString()));
    }

    void emulator_ui_backend_singleton::set_backend(emulator_ui_backend* backend)
    {
        backend_instance = backend;
    }

    emulator_ui_backend* emulator_ui_backend_singleton::create(QQmlEngine*, QJSEngine* engine)
    {
        js_engine = js_engine == nullptr ? engine : js_engine;

        Q_ASSERT(engine == js_engine);
        Q_ASSERT(backend_instance);
        Q_ASSERT(js_engine->thread() == backend_instance->thread());

        QJSEngine::setObjectOwnership(backend_instance, QJSEngine::CppOwnership);
        return backend_instance;
    }
}