#pragma once

#include <QtCore>
#include <QtQml>
#include <concepts>
#include <expected>

namespace emulator
{
    template <typename Backend>
    concept EmulatorUIBackendImp = requires(Backend& backend, const std::string_view rom_path)
    {
        { backend.has_rom() } -> std::convertible_to<bool>;
        { backend.is_running() } -> std::convertible_to<bool>;
        { backend.load_rom(rom_path) } -> std::convertible_to<std::expected<void,std::string>>;
    };

    class emulator_ui_backend : public QObject
    {
        Q_OBJECT

    public:
        template <EmulatorUIBackendImp Imp>
        explicit emulator_ui_backend(Imp& backend)
            : has_rom_fn { [&backend] { return backend.has_rom(); } }
            , is_running_fn { [&backend] { return backend.is_running(); } }
            , load_rom_fn { [&backend] (const std::string_view path) { return backend.load_rom(path); } }
        {}

        Q_INVOKABLE QVariantMap load_rom(const QUrl& url);

    private:
        std::function<bool()> has_rom_fn;
        std::function<bool()> is_running_fn;
        std::function<std::expected<void,std::string>(std::string_view)> load_rom_fn;
    };

    class emulator_ui_backend_singleton
    {
        Q_GADGET
        QML_FOREIGN(emulator_ui_backend)
        QML_SINGLETON
        QML_NAMED_ELEMENT(Backend)

    public:
        static void set_backend(emulator_ui_backend* backend);
        static emulator_ui_backend* create(QQmlEngine*, QJSEngine* engine);

    private:
        inline static QJSEngine* js_engine { nullptr };
        inline static emulator_ui_backend* backend_instance{ nullptr };
    };
}
