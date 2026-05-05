#pragma once

#include <QtCore>
#include <QtQml>
#include <QtQuick>
#include <concepts>
#include <expected>

namespace emulator
{
    constexpr std::uint8_t ui_framebuffer_width = 160;
    constexpr std::uint8_t ui_framebuffer_height = 144;
    constexpr std::uint8_t ui_framebuffer_num_channels = 3;
    constexpr std::size_t ui_framebuffer_size = ui_framebuffer_height * ui_framebuffer_width * ui_framebuffer_num_channels;

    using ui_load_rom_result_t = std::expected<void, std::string>;
    using ui_framebuffer_t = std::span<const std::uint8_t, ui_framebuffer_size>;

    template <typename Backend>
    concept EmulatorUIBackendImp = requires(Backend& backend, const std::string_view rom_path)
    {
        { backend.has_rom() } -> std::convertible_to<bool>;
        { backend.is_running() } -> std::convertible_to<bool>;
        { backend.load_rom(rom_path) } -> std::convertible_to<ui_load_rom_result_t>;
        { backend.framebuffer() } -> std::convertible_to<ui_framebuffer_t>;
        { backend.resume() } -> std::same_as<void>;
        { backend.pause() } -> std::same_as<void>;
        { backend.stop() } -> std::same_as<void>;
    };

    class emulator_ui_backend : public QObject
    {
        Q_OBJECT
        Q_PROPERTY(bool has_rom READ has_rom)
        Q_PROPERTY(bool is_running READ is_running)

    public:
        template <EmulatorUIBackendImp Imp>
        explicit emulator_ui_backend(Imp& backend)
            : has_rom_fn { [&backend] { return backend.has_rom(); } }
            , is_running_fn { [&backend] { return backend.is_running(); } }
            , load_rom_fn { [&backend] (const std::string_view path) { return backend.load_rom(path); } }
            , framebuffer_fn { [&backend] { return backend.framebuffer(); } }
            , resume_fn { [&backend] { return backend.resume(); } }
            , pause_fn { [&backend] { return backend.pause(); } }
            , stop_fn { [&backend] { return backend.stop(); } }
        {}

        Q_INVOKABLE QVariantMap load_rom(const QUrl& url) const;
        Q_INVOKABLE bool has_rom() const;
        Q_INVOKABLE bool is_running() const;
        Q_INVOKABLE void resume() const;
        Q_INVOKABLE void pause() const;
        Q_INVOKABLE void stop() const;

        auto framebuffer() const { return framebuffer_fn(); }

    private:
        std::function<bool()> has_rom_fn;
        std::function<bool()> is_running_fn;
        std::function<ui_load_rom_result_t(std::string_view)> load_rom_fn;
        std::function<ui_framebuffer_t()> framebuffer_fn;
        std::function<void()> resume_fn;
        std::function<void()> pause_fn;
        std::function<void()> stop_fn;
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

    class framebuffer_ui : public QQuickItem
    {
        Q_OBJECT
        QML_NAMED_ELEMENT(FramebufferView)

        Q_PROPERTY(QObject* backend WRITE set_backend)
        Q_PROPERTY(int framebufferWidth READ get_width CONSTANT)
        Q_PROPERTY(int framebufferHeight READ get_height CONSTANT)

    public:
        void set_backend(QObject* b);

        static int get_width() { return ui_framebuffer_width; }
        static int get_height() { return ui_framebuffer_height; }

    protected:
        QSGNode* updatePaintNode(QSGNode* oldNode, UpdatePaintNodeData* updatePaintNodeData) override;

    private:
        emulator_ui_backend* backend { nullptr };
        std::unique_ptr<QSGTexture> texture { nullptr };
    };

}
