#pragma once

#include <QtCore>
#include <QtQml>
#include <QtQuick>
#include <concepts>
#include <expected>
#include <boost/lockfree/spsc_value.hpp>

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
        const std::uint32_t ticks,
        const float volume,
        const bool muted)
    {
        { backend.has_rom() } -> std::convertible_to<bool>;
        { backend.is_running() } -> std::convertible_to<bool>;

        { backend.load_rom(rom_path) } -> std::convertible_to<ui_load_rom_result_t>;
        { backend.resume() } -> std::same_as<void>;
        { backend.pause() } -> std::same_as<void>;
        { backend.stop() } -> std::same_as<void>;
        { backend.step(ticks) } -> std::same_as<void>;

        { backend.read_memory(address) } -> std::convertible_to<std::uint8_t>;

        { backend.volume() } -> std::convertible_to<float>;
        { backend.set_volume(volume) } -> std::same_as<void>;

        { backend.muted() } -> std::convertible_to<bool>;
        { backend.set_muted(muted) } -> std::same_as<void>;
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
        Q_PROPERTY(float volume READ volume WRITE setVolume BINDABLE bindable_volume)
        Q_PROPERTY(bool muted READ muted WRITE setMuted BINDABLE bindable_muted)

    public:
        template <EmulatorUIBackendImp Imp>
        explicit emulator_ui_controls(Imp& backend)
            : has_rom_fn { [&backend] { return backend.has_rom(); } }
            , is_running_fn { [&backend] { return backend.is_running(); } }
            , load_rom_fn { [&backend] (const std::string_view path) { return backend.load_rom(path); } }
            , resume_fn { [&backend] { return backend.resume(); } }
            , pause_fn { [&backend] { return backend.pause(); } }
            , stop_fn { [&backend] { return backend.stop(); } }
            , step_fn { [&backend] (const std::uint32_t ticks) { backend.step(ticks); } }
            , read_mem_fn { [&backend] (const auto address) { return backend.read_memory(address); } }
            , get_volume_fn { [&backend] { return backend.volume(); } }
            , set_volume_fn { [&backend] (const float volume) { backend.set_volume(volume); } }
            , get_muted_fn { [&backend] { return backend.muted(); } }
            , set_muted_fn { [&backend] (const bool muted) { backend.set_muted(muted); } }
        {
            update_current_status();

            current_volume.setBinding([this] { return get_volume_fn(); });
            current_muted.setBinding([this] { return get_muted_fn(); });
        }

        Q_INVOKABLE QVariantMap load_rom(const QUrl& url);
        Q_INVOKABLE emulator_ui_status status() const;

        Q_INVOKABLE void resume();
        Q_INVOKABLE void pause();
        Q_INVOKABLE void stop();
        Q_INVOKABLE void nextFrame();

        Q_INVOKABLE float volume() const;
        Q_INVOKABLE void setVolume(float volume);

        Q_INVOKABLE bool muted() const;
        Q_INVOKABLE void setMuted(bool muted);

    private:
        QBindable<emulator_ui_status> bindable_status();
        void update_current_status();
        QProperty<emulator_ui_status> current_status;

        QProperty<float> current_volume;
        QBindable<float> bindable_volume() { return &current_volume; }

        QProperty<bool> current_muted;
        QBindable<bool> bindable_muted() { return &current_muted; }

        std::function<bool()> has_rom_fn;
        std::function<bool()> is_running_fn;
        std::function<ui_load_rom_result_t(std::string_view)> load_rom_fn;
        std::function<void()> resume_fn;
        std::function<void()> pause_fn;
        std::function<void()> stop_fn;
        std::function<void(std::uint32_t)> step_fn;
        std::function<std::uint8_t(std::uint16_t)> read_mem_fn;
        std::function<float()> get_volume_fn;
        std::function<void(float)> set_volume_fn;
        std::function<bool()> get_muted_fn;
        std::function<void(bool)> set_muted_fn;
    };

    class emulator_ui_video_source : public QObject
    {
        Q_OBJECT
        QML_UNCREATABLE("")

    public:
        virtual QImage frame() = 0;
        virtual QSize size() = 0;

    signals:
        void frameAvailable();
    };

    class emulator_ui_framebuffer_source : public emulator_ui_video_source
    {
        Q_OBJECT
        QML_UNCREATABLE("")
        QML_NAMED_ELEMENT(EmulatorFramebuffer)

        Q_PROPERTY(QImage frame READ frame NOTIFY frameAvailable)
        Q_PROPERTY(QSize size READ size CONSTANT)

    public:
        QSize size() override { return { ui_framebuffer_width, ui_framebuffer_height }; }
        QImage frame() override { return buffer.read(boost::lockfree::uses_optional).value(); }

        void push_frame(ui_framebuffer_view_t frame);

    private:
        boost::lockfree::spsc_value<
            QImage,
            boost::lockfree::allow_multiple_reads<true>
        > buffer {};
    };

    class emulator_ui_video : public QQuickItem
    {
        Q_OBJECT
        QML_NAMED_ELEMENT(EmulatorVideo)
        Q_PROPERTY(emulator_ui_video_source* source READ get_source WRITE set_source REQUIRED)

    public:
        emulator_ui_video()
        {
            setFlag(ItemHasContents, true);
            setSmooth(false);
        }

        emulator_ui_video_source* get_source() const { return source; }
        void set_source(emulator_ui_video_source* new_source);

    protected:
        QSGNode* updatePaintNode(QSGNode* oldNode, UpdatePaintNodeData* updatePaintNodeData) override;

    private:
        emulator_ui_video_source* source { nullptr };
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
        QML_NAMED_ELEMENT(EmulatorSprites)

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

    class emulator_ui_background : public emulator_ui_video_source
    {
        Q_OBJECT
        QML_UNCREATABLE("")
        QML_NAMED_ELEMENT(EmulatorBackground)

        Q_PROPERTY(QImage image READ frame BINDABLE bindable_image NOTIFY frameAvailable)
        Q_PROPERTY(QRect window READ window BINDABLE bindable_window)

    public:
        template <EmulatorUIBackendImp Imp>
        explicit emulator_ui_background(Imp& backend)
            : read_mem_fn { [&backend] (const auto address) { return backend.read_memory(address); } }
        {}

        QImage frame() override { return current_image.value(); }
        QSize size() override { return { 256, 256 }; }

        QRect window() const { return current_window.value(); }

        Q_INVOKABLE void refresh();

    private:
        QProperty<QImage> current_image;
        QBindable<QImage> bindable_image() { return &current_image; }

        QProperty<QRect> current_window;
        QBindable<QRect> bindable_window() { return &current_window; }

        std::function<std::uint8_t(std::uint16_t)> read_mem_fn;
    };

    class emulator_ui_background_image_provider : public QQuickImageProvider
    {
        Q_OBJECT
        QML_UNCREATABLE("")

    public:
        explicit emulator_ui_background_image_provider(emulator_ui_background& bg, QObject* parent = nullptr)
            : QQuickImageProvider { Image }
            , background_source { bg }
        {
            setParent(parent);
        }

        QImage requestImage(const QString& id, QSize* size, const QSize& requestedSize) override;

    private:
        emulator_ui_background& background_source;
    };

}

Q_DECLARE_INTERFACE(emulator::emulator_ui_video_source, "emulator.ui.VideoSource")

