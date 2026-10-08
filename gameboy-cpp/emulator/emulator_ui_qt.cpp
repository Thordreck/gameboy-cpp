#include "emulator_ui_qt.hpp"

#include <QtQuick>
#include <QString>
#include <QVariant>
#include <QtLogging>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <boost/lockfree/spsc_value.hpp>

namespace emulator
{
    [[nodiscard]] static QVariantMap toQtVariantMap(const std::expected<void, std::string>& result)
    {
        return result.has_value()
                   ? QVariantMap{{"success", true}}
                   : QVariantMap{{"success", false}, {"error", result.error().data()}};
    }

    template<typename TExpected, typename TError>
    [[nodiscard]] static TExpected required(std::expected<TExpected, TError>&& result)
    {
        if (!result.has_value())
        {
            qFatal(std::format("Unexpected error. {}", result.error()).c_str());
        }

        return std::forward<TExpected>(result.value());
    }

    template<typename TError>
    static void required(std::expected<void, TError>&& result)
    {
        if (!result.has_value())
        {
            qFatal(std::format("Unexpected error. {}", result.error()).c_str());
        }
    }

    template<typename Keys, typename ...Values>
    requires std::is_convertible_v<Keys, std::string_view>
    [[nodiscard]] static QVariantMap make_variant_map(const std::pair<Keys, Values>& ...args)
    {
        QVariantMap map {};
        (map.insert(args.first, QVariant::fromValue(args.second)), ...);

        return map;
    }

    static std::expected<QIcon, std::string> load_icon(const std::string_view path)
    {
        const QIcon icon { path.data() };

        if (icon.isNull())
        {
            return std::unexpected { std::format("Could not load window icon from path {}", path) };
        }

        return icon;
    }

    struct emulator_ui_status
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

    class emulator_ui_controls : public QObject
    {
        Q_OBJECT
        QML_UNCREATABLE("")
        QML_NAMED_ELEMENT(EmulatorControls)

        Q_PROPERTY(emulator_ui_status::Value status READ status BINDABLE bindable_status)
        Q_PROPERTY(float volume READ volume WRITE setVolume BINDABLE bindable_volume)
        Q_PROPERTY(bool muted READ muted WRITE setMuted BINDABLE bindable_muted)

    public:
        explicit emulator_ui_controls(const backend_functions& _backend)
            : backend { _backend }
        {
            update_current_status();

            current_volume.setBinding([this] { return backend.volume(); });
            current_muted.setBinding([this] { return backend.muted(); });
        }

        Q_INVOKABLE QVariantMap load_rom(const QUrl& url)
        {
            const QVariantMap result = toQtVariantMap(backend.load_rom(url.toLocalFile().toStdString()));
            update_current_status();

            return result;
        }

        Q_INVOKABLE emulator_ui_status::Value status() const
        {
            return current_status;
        }

        Q_INVOKABLE void resume()
        {
            backend.resume();
            update_current_status();
        }

        Q_INVOKABLE void pause()
        {
            backend.pause();
            update_current_status();
        }

        Q_INVOKABLE void stop()
        {
            backend.stop();
            update_current_status();
        }

        Q_INVOKABLE void nextFrame() const
        {
            constexpr std::uint16_t ly_address{0xFF44};
            constexpr std::uint8_t total_scanlines{153};
            constexpr std::uint32_t dots_per_scanline{456};

            const std::uint8_t current_scanline = backend.read_memory(ly_address);
            const std::uint8_t remaining_scanlines = total_scanlines - current_scanline + 1;
            const std::uint32_t remaining_dots = remaining_scanlines * dots_per_scanline;

            backend.step(remaining_dots);
        }

        Q_INVOKABLE float volume() const
        {
            return current_volume.value();
        }

        Q_INVOKABLE void setVolume(const float volume)
        {
            backend.set_volume(volume);
            current_volume = backend.volume();
        }

        Q_INVOKABLE bool muted() const
        {
            return current_muted.value();
        }

        Q_INVOKABLE void setMuted(const bool muted)
        {
            backend.set_muted(muted);
            current_muted = backend.muted();
        }

    private:
        QBindable<emulator_ui_status::Value> bindable_status()
        {
            return {&current_status};
        }

        void update_current_status()
        {
            using enum emulator_ui_status::Value;
            current_status = !backend.has_rom() ? Stopped : backend.is_running() ? Running : Paused;
        }

        QProperty<emulator_ui_status::Value> current_status;

        QProperty<float> current_volume;
        QBindable<float> bindable_volume() { return &current_volume; }

        QProperty<bool> current_muted;
        QBindable<bool> bindable_muted() { return &current_muted; }

        backend_functions backend;
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

        void push_frame(const ui_framebuffer_view_t frame)
        {
            const QImage new_frame(
                frame.data(),
                ui_framebuffer_width,
                ui_framebuffer_height,
                ui_framebuffer_width * 3,
                QImage::Format_RGB888
            );

            buffer.write(new_frame.copy());
            emit frameAvailable();
        }

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

        void set_source(emulator_ui_video_source* new_source)
        {
            this->source = new_source;
            connect(new_source, &emulator_ui_video_source::frameAvailable, this, &QQuickItem::update, Qt::QueuedConnection);

            const QSize source_size = source->size();
            setImplicitSize(source_size.width(), source_size.height());
        }

    protected:
        QSGNode* updatePaintNode(QSGNode* oldNode, UpdatePaintNodeData* updatePaintNodeData) override
        {
            auto node = static_cast<QSGSimpleTextureNode*>(oldNode);
            if (node == nullptr) { node = new QSGSimpleTextureNode(); }

            texture.reset(window()->createTextureFromImage(source->frame(), QQuickWindow::TextureIsOpaque));
            texture->setFiltering(QSGTexture::Nearest);

            node->setTexture(texture.get());
            node->setRect(boundingRect());

            return node;
        }

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

    enum class sprites_model_roles : int
    {
        index = Qt::UserRole + 1,
        x,
        y,
        tile_index,
        priority,
        x_flip,
        y_flip,
        image_uri,
        width,
        height
    };

    class emulator_ui_sprites_model : public QAbstractListModel
    {
        Q_OBJECT
        QML_UNCREATABLE("")
        QML_NAMED_ELEMENT(EmulatorSprites)

    public:
        explicit emulator_ui_sprites_model(const backend_functions& backend, QObject* parent = nullptr)
            : QAbstractListModel { parent }
            , read_mem_fn { [&backend] (const auto address) { return backend.read_memory(address); } }
        {}

        int rowCount(const QModelIndex &parent) const override
        {
            return sprites.size();
        }

        QVariant data(const QModelIndex &model_index, const int role) const override
        {
            using enum sprites_model_roles;
            const sprites_model_roles sprite_role{role};

            const int sprite_index = model_index.row();
            const emulator_ui_sprite& selected_sprite = sprites[sprite_index];

            switch (sprite_role)
            {
            case index:
                return selected_sprite.index;
            case x:
                return selected_sprite.x;
            case y:
                return selected_sprite.y;
            case tile_index:
                return selected_sprite.tile_index;
            case priority:
                return selected_sprite.priority;
            case x_flip:
                return selected_sprite.x_flip;
            case y_flip:
                return selected_sprite.y_flip;
            case image_uri:
                return QString("image://sprites?tile_index=%1&double_height=%2&alternate_palette=%3")
                       .arg(selected_sprite.tile_index)
                       .arg(selected_sprite.height > 8)
                       .arg(selected_sprite.alternate_palette);
            case width:
                return selected_sprite.width;
            case height:
                return selected_sprite.height;
            default:
                std::unreachable();
            }
        }

        QHash<int, QByteArray> roleNames() const override
        {
            QHash<int, QByteArray> roles{};

            roles[std::to_underlying(sprites_model_roles::index)] = "spriteIndex";
            roles[std::to_underlying(sprites_model_roles::x)] = "spriteX";
            roles[std::to_underlying(sprites_model_roles::y)] = "spriteY";
            roles[std::to_underlying(sprites_model_roles::tile_index)] = "tileIndex";
            roles[std::to_underlying(sprites_model_roles::priority)] = "priority";
            roles[std::to_underlying(sprites_model_roles::x_flip)] = "xFlip";
            roles[std::to_underlying(sprites_model_roles::y_flip)] = "yFlip";
            roles[std::to_underlying(sprites_model_roles::image_uri)] = "imageURI";
            roles[std::to_underlying(sprites_model_roles::width)] = "spriteWidth";
            roles[std::to_underlying(sprites_model_roles::height)] = "spriteHeight";

            return roles;
        }

        Q_INVOKABLE void refreshSpritesCache()
        {
            beginResetModel();

            constexpr std::uint16_t lcdc_address = 0xFF40;
            const bool obj_size_set = read_mem_fn(lcdc_address) >> 2 & 0b1;

            const auto read_sprite_from_memory = [this, obj_size_set](const int sprite_index)
            {
                constexpr std::uint16_t sprites_start_address = 0xFE00;
                constexpr std::uint8_t sprite_memory_byte_size = 4;

                const std::uint16_t initial_address = sprites_start_address + sprite_index * sprite_memory_byte_size;

                const std::array sprite_memory
                {
                    this->read_mem_fn(initial_address),
                    this->read_mem_fn(initial_address + 1),
                    this->read_mem_fn(initial_address + 2),
                    this->read_mem_fn(initial_address + 3),
                };

                const auto sprite = emulator_ui_sprite
                {
                    static_cast<std::uint8_t>(sprite_index),
                    sprite_memory[1],
                    sprite_memory[0],
                    sprite_memory[2],
                    (sprite_memory[3] >> 7 & 0b1) != 0,
                    (sprite_memory[3] >> 5 & 0b1) != 0,
                    (sprite_memory[3] >> 6 & 0b1) != 0,
                    (sprite_memory[3] >> 4 & 0b1) != 0,
                    8,
                    static_cast<std::uint8_t>(obj_size_set ? 16 : 8)
                };

                return sprite;
            };

            const auto updated_sprites = std::views::iota(0, 40)
                | std::views::transform(read_sprite_from_memory);

            std::ranges::copy(updated_sprites, sprites.begin());

            endResetModel();
        }

    private:
        std::array<emulator_ui_sprite, 40> sprites {};
        std::function<std::uint8_t(std::uint16_t)> read_mem_fn;
    };

    class emulator_ui_sprites_image_provider : public QQuickImageProvider
    {
        Q_OBJECT
        QML_UNCREATABLE("")

    public:
        explicit emulator_ui_sprites_image_provider(const backend_functions& backend, QObject* parent = nullptr)
            : QQuickImageProvider { Image }
            , read_mem_fn { [&backend] (const auto address) { return backend.read_memory(address); } }
        {
            setParent(parent);
        }

        QImage requestImage(const QString& id, QSize* size, const QSize& requestedSize) const
        {
            const QUrlQuery image_search_query{id};

            const bool double_height = image_search_query.queryItemValue("double_height").toInt();
            const std::uint8_t alternate_palette = image_search_query.queryItemValue("alternate_palette").toInt();
            const std::uint8_t tile_index = image_search_query.queryItemValue("tile_index").toUInt();

            constexpr int sprite_width = 8;
            const int sprite_height = double_height ? 16 : 8;

            constexpr std::uint16_t tile_base_address = 0x8000;
            constexpr int lines_per_tile = 8;

            const int number_of_tiles = double_height ? 2 : 1;
            const auto tile_line_indexes = std::views::iota(0, lines_per_tile * number_of_tiles);

            const auto tile_line_addresses = tile_line_indexes
                | std::views::transform([tile_index, double_height](const auto index)
                {
                    constexpr int bytes_per_tile = 16;
                    constexpr int bytes_per_line = 2;

                    const bool first_tile = index < lines_per_tile;
                    const auto masked_tile_index = !double_height
                                                       ? tile_index
                                                       : first_tile
                                                       ? tile_index & 0xFE
                                                       : tile_index | 0x01;
                    const auto tile_start_address = masked_tile_index * bytes_per_tile;
                    const auto clamped_index = index % lines_per_tile;

                    return tile_base_address + tile_start_address + clamped_index * bytes_per_line;
                });

            const auto tile_lines = tile_line_addresses | std::views::transform([this](const auto address)
            {
                return static_cast<std::uint16_t>(this->read_mem_fn(address)) << 8 | this->read_mem_fn(address + 1);
            });

            const static auto extract_tile_line_palette_indexes = [](const int tile_line)
            {
                return std::views::iota(0, 8)
                    | std::views::reverse
                    | std::views::transform([tile_line](const auto pixel_index)
                    {
                        const auto lsb = (tile_line >> (8 + pixel_index)) & 0b1;
                        const auto msb = (tile_line >> pixel_index) & 0b1;

                        return msb << 1 | lsb;
                    });
            };

            const auto tile_line_palette_indexes = tile_lines
                | std::views::transform(extract_tile_line_palette_indexes);

            QImage image{sprite_width, sprite_height, QImage::Format_RGBA8888};

            const std::uint16_t palette_address = alternate_palette ? 0xFF49 : 0xFF48;
            const std::uint8_t palette = read_mem_fn(palette_address);

            for (auto&& [line_index, line_palette_indexes] : tile_line_palette_indexes | std::views::enumerate)
            {
                for (auto&& [pixel_index, palette_index] : line_palette_indexes | std::views::enumerate)
                {
                    constexpr std::array sprite_color_table{0xFFFFFFFF, 0xFFAAAAAA, 0xFF555555, 0xFF000000};

                    const std::uint8_t color_index = (palette >> (palette_index * 2)) & 0b11;
                    const QColor pixel_color = color_index == 0
                                                   ? Qt::transparent
                                                   : QColor::fromRgba(sprite_color_table[color_index]);
                    image.setPixelColor(pixel_index, line_index, pixel_color);
                }
            }

            size->setHeight(image.width());
            size->setWidth(image.height());

            return image;
        }


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
        explicit emulator_ui_background(const backend_functions& backend)
            : read_mem_fn { [&backend] (const auto address) { return backend.read_memory(address); } }
        {}

        QImage frame() override { return current_image.value(); }
        QSize size() override { return { 256, 256 }; }

        QRect window() const { return current_window.value(); }

        Q_INVOKABLE void refresh()
        {
            constexpr std::uint16_t lcdc_address { 0xFF40 };
            constexpr std::size_t tilemap_size { 32 * 32 };
            constexpr std::array bg_color_table{0xFFFFFFFF, 0xFFAAAAAA, 0xFF555555, 0xFF000000};

            const std::uint8_t lcdc = read_mem_fn(lcdc_address);
            const bool bg_tile_map_flag = lcdc >> 3 & 0b1;
            const bool addressing_mode_flag = lcdc >> 4 & 0b1;
            const std::uint16_t bg_tile_map_address = bg_tile_map_flag ? 0x9C00 : 0x9800;
            const std::uint16_t bg_tile_data_address = addressing_mode_flag ? 0x8000 : 0x9000;

            constexpr std::uint16_t palette_address { 0xFF47 };
            const std::uint8_t palette = read_mem_fn(palette_address);

            QImage background(256, 256, QImage::Format_ARGB32);
            QImage tile(8, 8, QImage::Format_ARGB32);

            QPainter painter(&background);

            for (std::size_t tilemap_index = 0; tilemap_index < tilemap_size; ++tilemap_index)
            {
                const std::uint16_t tilemap_address = bg_tile_map_address + tilemap_index;
                const std::uint8_t tile_index = read_mem_fn(tilemap_address);
                const std::uint16_t tile_address = bg_tile_data_address + static_cast<std::int8_t>(tile_index) * 16;

                for (std::size_t tile_line = 0; tile_line < 8; ++tile_line)
                {
                    const std::uint16_t tile_line_address = tile_address + tile_line * 2;
                    const std::uint16_t tile_line_data = static_cast<std::uint16_t>(read_mem_fn(tile_line_address)) << 8 | read_mem_fn(tile_line_address + 1);
                    const auto tile_line_pixel_indexes = std::views::iota(0, 8)
                        | std::views::reverse
                        | std::views::transform([tile_line_data] (const auto i)
                        {
                            const auto lsb = (tile_line_data >> (8 + i)) & 0b1;
                            const auto msb = (tile_line_data >> i) & 0b1;

                            return msb << 1 | lsb;
                        });

                    const auto tile_line_pixel_colors
                        = tile_line_pixel_indexes
                        | std::views::transform([palette](const auto index) { return palette >> (index * 2) & 0b11; })
                        | std::views::transform([&bg_color_table] (const auto index) { return QRgb { bg_color_table[index] }; });

                    std::span tile_line_view { reinterpret_cast<QRgb*>(tile.scanLine(tile_line)), 8 };
                    std::ranges::copy(tile_line_pixel_colors, tile_line_view.begin());
                }

                const int tile_x = tilemap_index % 32 * 8;
                const int tile_y = tilemap_index / 32 * 8;

                painter.drawImage(tile_x, tile_y, tile);
            }

            current_image.setValue(background);
            emit frameAvailable();
        }


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

        QImage requestImage(const QString& id, QSize* size, const QSize& requestedSize) const
        {
            const QUrlQuery image_search_query{id};
            const std::uint16_t tile_index = image_search_query.queryItemValue("tile_index").toUInt();

            const int tile_x = tile_index % 32;
            const int tile_y = tile_index / 32;

            QImage tile_image = background_source.frame().copy(tile_x * 8, tile_y * 8, 8, 8);
            *size = tile_image.size();

            return tile_image;
        }

    private:
        emulator_ui_background& background_source;
    };

    struct qt_graphical_interface::pimpl
    {
        QGuiApplication app;
        QQmlApplicationEngine engine;
        emulator_ui_framebuffer_source framebuffer_source;

        pimpl(int argc, char** argv)
            : app { argc, argv }
            , engine {}
            , framebuffer_source {}
        {}
    };

    qt_graphical_interface::qt_graphical_interface(int argc, char** argv) noexcept
        : imp { std::make_unique<pimpl>(argc, argv) }
    {}

    qt_graphical_interface::~qt_graphical_interface() = default;

    void qt_graphical_interface::start_rendering()
    {
        // Implement or remove from interface
    }

    void qt_graphical_interface::stop_rendering()
    {
        // Implement or remove from interface
    }

    void qt_graphical_interface::present_frame(const ui_framebuffer_view_t frame)
    {
        imp->framebuffer_source.push_frame(frame);
    }

    int qt_graphical_interface::render(const backend_functions& backend)
    {
        required(
            load_icon(":/icons/gameboy-icon.png")
            .transform([this] (const auto& icon) { imp->app.setWindowIcon(icon); }));

        emulator_ui_controls ui_controls { backend };

#ifdef QT_UI_DEBUG_MODE
        emulator_ui_sprites_model ui_debug_sprites_model { backend };
        emulator_ui_sprites_image_provider ui_debug_sprites_provider { backend };
        emulator_ui_background ui_debug_background { backend };
        emulator_ui_background_image_provider ui_debug_background_provider { backend };

        engine.add_image_provider("sprites", &ui_debug_sprites_provider);
        engine.add_image_provider("background", &ui_debug_background_provider);

        qt::register_shortcut(qt::standard_key::refresh, [this]
        {
            std::ranges::for_each(engine.root_objects(), [] (auto object) { object.delete_later(); });
            engine.clear_singletons();
            engine.clear_component_cache();

            engine.load(QML_HOT_RELOAD_PATH);

        }, qt::shortcut_context::application, app);
#endif

        imp->engine.setInitialProperties(make_variant_map(
            std::make_pair("controls", &ui_controls),
            std::make_pair("framebuffer", &imp->framebuffer_source),
#ifdef QT_UI_DEBUG_MODE
            std::make_pair("debugMode", true),
            std::make_pair("sprites", &ui_debug_sprites_model),
            std::make_pair("bg", &ui_debug_background));
#else
            std::make_pair("debugMode", false)));
#endif

        imp->engine.loadFromModule("Gameboy.UI", "EmulatorUI");
        return imp->app.exec();
    }

}

#include "emulator_ui_qt.moc"
Q_DECLARE_INTERFACE(emulator::emulator_ui_video_source, "emulator.ui.VideoSource")
