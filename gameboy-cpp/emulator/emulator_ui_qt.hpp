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
    using ui_framebuffer_view_t = std::span<const std::uint8_t, ui_framebuffer_size>;
    using ui_framebuffer_t = std::array<std::uint8_t, ui_framebuffer_size>;

    template <typename Backend>
    concept EmulatorUIBackendImp = requires(
        Backend& backend,
        const std::string_view rom_path,
        const std::uint16_t address,
        const std::uint32_t ticks)
    {
        { backend.has_rom() } -> std::convertible_to<bool>;
        { backend.is_running() } -> std::convertible_to<bool>;

        { backend.load_rom(rom_path) } -> std::convertible_to<ui_load_rom_result_t>;
        { backend.framebuffer() } -> std::convertible_to<ui_framebuffer_view_t>;
        { backend.resume() } -> std::same_as<void>;
        { backend.pause() } -> std::same_as<void>;
        { backend.stop() } -> std::same_as<void>;
        { backend.step(ticks) } -> std::same_as<void>;

        { backend.read_memory(address) } -> std::convertible_to<std::uint8_t>;
    };

    template <typename T>
    concept EmulatorUIFramebufferSource = requires(T& source)
    {
        { source.read() } -> std::convertible_to<ui_framebuffer_t>;
    };

    struct emulator_ui_status_wrapper
    {
        Q_GADGET
        QML_UNCREATABLE("")
        QML_NAMED_ELEMENT(EmulatorStatus)

    public:
        enum class Value : std::uint8_t
        {
            Stopped,
            Running,
            Paused
        };

        Q_ENUM(Value)
    };

    using emulator_ui_status = emulator_ui_status_wrapper::Value;

    class emulator_ui_controls : public QObject
    {
        Q_OBJECT
        QML_UNCREATABLE("")
        QML_NAMED_ELEMENT(EmulatorControls)

        Q_PROPERTY(emulator_ui_status status READ status BINDABLE bindable_status)

    public:
        template <EmulatorUIBackendImp Imp>
        explicit emulator_ui_controls(Imp& backend)
            : has_rom_fn { [&backend] { return backend.has_rom(); } }
            , is_running_fn { [&backend] { return backend.is_running(); } }
            , load_rom_fn { [&backend] (const std::string_view path) { return backend.load_rom(path); } }
            , resume_fn { [&backend] { return backend.resume(); } }
            , pause_fn { [&backend] { return backend.pause(); } }
            , stop_fn { [&backend] { return backend.stop(); } }
        {
            update_current_status();
        }

        Q_INVOKABLE QVariantMap load_rom(const QUrl& url);
        Q_INVOKABLE emulator_ui_status status() const;
        Q_INVOKABLE void resume();
        Q_INVOKABLE void pause();
        Q_INVOKABLE void stop();

        QBindable<emulator_ui_status> bindable_status();

    private:
        void update_current_status();
        QProperty<emulator_ui_status> current_status;

        std::function<bool()> has_rom_fn;
        std::function<bool()> is_running_fn;
        std::function<ui_load_rom_result_t(std::string_view)> load_rom_fn;
        std::function<void()> resume_fn;
        std::function<void()> pause_fn;
        std::function<void()> stop_fn;
    };

    class emulator_ui_framebuffer : public QObject
    {
        Q_OBJECT
        QML_UNCREATABLE("")
        QML_NAMED_ELEMENT(EmulatorFramebuffer)

    public:
        template <EmulatorUIFramebufferSource Imp>
        explicit emulator_ui_framebuffer(Imp& source)
            : get_frame_fn { [&source] { return source.read(); } }
        {}

        ui_framebuffer_t frame() const;

        void start() { emit started(QPrivateSignal {}); }
        void stop() { emit stopped(QPrivateSignal {}); }
        void render() { emit frame_acquired(QPrivateSignal {}); }

    signals:
        void started(QPrivateSignal);
        void stopped(QPrivateSignal);
        void frame_acquired(QPrivateSignal);

    private:
        std::function<ui_framebuffer_t()> get_frame_fn;
    };

    class emulator_ui_video : public QQuickItem
    {
        Q_OBJECT
        QML_NAMED_ELEMENT(EmulatorVideo)
        Q_PROPERTY(emulator_ui_framebuffer* source WRITE set_source REQUIRED)

    public:
        emulator_ui_video()
        {
            setFlag(ItemHasContents, false);
            setImplicitWidth(ui_framebuffer_width);
            setImplicitHeight(ui_framebuffer_height);
            setSmooth(false);
        }

        void set_source(emulator_ui_framebuffer* source);

    protected:
        QSGNode* updatePaintNode(QSGNode* oldNode, UpdatePaintNodeData* updatePaintNodeData) override;

    private:
        emulator_ui_framebuffer* source { nullptr };
        std::unique_ptr<QSGTexture> texture { nullptr };
    };

    struct emulator_ui_sprite
    {
        std::uint8_t index;
        std::uint8_t x;
        std::uint8_t y;
        std::uint8_t tile_index;
        bool priority;
        bool x_flip;
        bool y_flip;
        bool alternate_palette;
        std::uint8_t width;
        std::uint8_t height;
    };

    class emulator_ui_sprites_model : public QAbstractListModel
    {
        Q_OBJECT
        QML_UNCREATABLE("")

    public:
        template <EmulatorUIBackendImp Imp>
        explicit emulator_ui_sprites_model(Imp& backend, QObject* parent = nullptr)
            : QAbstractListModel { parent }
            , read_mem_fn { [&backend] (const auto address) { return backend.read_memory(address); } }
        {}

        int rowCount(const QModelIndex &parent) const override;
        QVariant data(const QModelIndex &index, int role) const override;
        QHash<int, QByteArray> roleNames() const override;

        Q_INVOKABLE void refreshSpritesCache();

    private:
        std::array<emulator_ui_sprite, 40> sprites {};
        std::function<std::uint8_t(std::uint16_t)> read_mem_fn;
    };

    class emulator_ui_sprites_image_provider : public QQuickImageProvider
    {
        Q_OBJECT
        QML_UNCREATABLE("")

    public:
        template <EmulatorUIBackendImp Imp>
        explicit emulator_ui_sprites_image_provider(Imp& backend, QObject* parent = nullptr)
            : QQuickImageProvider { Image }
            , read_mem_fn { [&backend] (const auto address) { return backend.read_memory(address); } }
        {
            setParent(parent);
        }

        QImage requestImage(const QString& id, QSize* size, const QSize& requestedSize) override;

    private:
        std::function<std::uint8_t(std::uint16_t)> read_mem_fn;
    };

    class emulator_ui_debug : public QObject
    {
        Q_OBJECT
        QML_UNCREATABLE("")
        QML_NAMED_ELEMENT(EmulatorDebug)
        Q_PROPERTY(const QAbstractItemModel* sprites READ sprites CONSTANT)

    public:
        template <EmulatorUIBackendImp Imp>
        explicit emulator_ui_debug(Imp& backend)
            : sprites_model { backend }
            , step_fn { [&backend] (const std::uint32_t ticks) { backend.step(ticks); } }
            , read_mem_fn { [&backend] (const auto address) { return backend.read_memory(address); } }
        {}

        auto* sprites() const { return &sprites_model; }
        Q_INVOKABLE void nextFrame() const;

    private:
        emulator_ui_sprites_model sprites_model;

        std::function<void(std::uint32_t)> step_fn;
        std::function<std::uint8_t(std::uint16_t)> read_mem_fn;
    };

}