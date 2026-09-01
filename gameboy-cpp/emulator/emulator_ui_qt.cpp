
#include "emulator_ui_qt.hpp"

namespace emulator
{
    static QVariantMap toQtVariantMap(const std::expected<void, std::string>& result)
    {
        return result.has_value()
            ? QVariantMap{ { "success", true } }
            : QVariantMap{ {"success", false }, { "error", result.error().data() }};
    }

    QVariantMap emulator_ui_controls::load_rom(const QUrl& url)
    {
        const auto result = toQtVariantMap(load_rom_fn(url.toLocalFile().toStdString()));
        update_current_status();

        return result;
    }

    void emulator_ui_controls::update_current_status()
    {
        using enum emulator_ui_status;
        current_status = !has_rom_fn() ? Stopped : is_running_fn() ? Running : Paused;
    }

    QBindable<emulator_ui_status> emulator_ui_controls::bindable_status()
    {
        return { &current_status };
    }

    emulator_ui_status emulator_ui_controls::status() const
    {
        return current_status;
    }

    void emulator_ui_controls::resume()
    {
        resume_fn();
        update_current_status();
    }

    void emulator_ui_controls::pause()
    {
        pause_fn();
        update_current_status();
    }

    void emulator_ui_controls::stop()
    {
        stop_fn();
        update_current_status();
    }

    float emulator_ui_controls::volume() const
    {
        return current_volume.value();
    }

    void emulator_ui_controls::setVolume(const float volume)
    {
        set_volume_fn(volume);
        current_volume = get_volume_fn();
    }

    bool emulator_ui_controls::muted() const
    {
        return current_muted.value();
    }

    void emulator_ui_controls::setMuted(const bool muted)
    {
        set_muted_fn(muted);
        current_muted = get_muted_fn();
    }

    void emulator_ui_controls::nextFrame()
    {
        constexpr std::uint16_t ly_address { 0xFF44 };
        constexpr std::uint8_t total_scanlines { 153 };
        constexpr std::uint32_t dots_per_scanline { 456 };

        const std::uint8_t current_scanline = read_mem_fn(ly_address);
        const std::uint8_t remaining_scanlines = total_scanlines - current_scanline + 1;
        const std::uint32_t remaining_dots = remaining_scanlines * dots_per_scanline;

        step_fn(remaining_dots);
    }

    ui_framebuffer_t emulator_ui_framebuffer::frame() const
    {
        return get_frame_fn();
    }

    void emulator_ui_video::set_source(emulator_ui_framebuffer* framebuffer)
    {
        source = framebuffer;

        connect(framebuffer, &emulator_ui_framebuffer::started, this, [this] { setFlag(ItemHasContents, true); }, Qt::QueuedConnection);
        connect(framebuffer, &emulator_ui_framebuffer::stopped, this, [this] { setFlag(ItemHasContents, false); }, Qt::QueuedConnection);
        connect(framebuffer, &emulator_ui_framebuffer::frame_acquired, this, [this] { update(); }, Qt::QueuedConnection);
    }

    QSGNode* emulator_ui_video::updatePaintNode(QSGNode* oldNode, UpdatePaintNodeData* updatePaintNodeData)
    {
        auto node = static_cast<QSGSimpleTextureNode*>(oldNode);
        if (node == nullptr ) { node = new QSGSimpleTextureNode(); }

        const ui_framebuffer_t next_frame = source->frame();
        const QImage img(next_frame.data(), ui_framebuffer_width, ui_framebuffer_height, ui_framebuffer_width * 3, QImage::Format_RGB888);

        // TODO: remove copy
        texture.reset(window()->createTextureFromImage(img.copy(), QQuickWindow::TextureIsOpaque));
        texture->setFiltering(QSGTexture::Nearest);

        node->setTexture(texture.get());
        node->setRect(boundingRect());

        return node;
    }

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

    QHash<int, QByteArray> emulator_ui_sprites_model::roleNames() const
    {
        QHash<int, QByteArray> roles {};

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

    int emulator_ui_sprites_model::rowCount(const QModelIndex &parent) const
    {
        return sprites.size();
    }

    QVariant emulator_ui_sprites_model::data(const QModelIndex &model_index, const int role) const
    {
        using enum sprites_model_roles;
        const sprites_model_roles sprite_role { role };

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

    void emulator_ui_sprites_model::refreshSpritesCache()
    {
        beginResetModel();

        constexpr std::uint16_t lcdc_address = 0xFF40;
        const bool obj_size_set = read_mem_fn(lcdc_address) >> 2 & 0b1;

        const auto read_sprite_from_memory = [this, obj_size_set] (const int sprite_index)
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

    QImage emulator_ui_sprites_image_provider::requestImage(const QString& id, QSize* size, const QSize& requestedSize)
    {
        const QUrlQuery image_search_query { id };

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
            | std::views::transform([tile_index, double_height] (const auto index)
            {
                constexpr int bytes_per_tile = 16;
                constexpr int bytes_per_line = 2;

                const bool first_tile = index < lines_per_tile;
                const auto masked_tile_index = !double_height ? tile_index : first_tile ? tile_index & 0xFE : tile_index | 0x01;
                const auto tile_start_address = masked_tile_index * bytes_per_tile;
                const auto clamped_index = index % lines_per_tile;

                return tile_base_address + tile_start_address + clamped_index * bytes_per_line;
            });

        const auto tile_lines = tile_line_addresses | std::views::transform([this] (const auto address)
        {
            return static_cast<std::uint16_t>(this->read_mem_fn(address)) << 8 | this->read_mem_fn(address + 1);
        });

        const static auto extract_tile_line_palette_indexes = [] (const int tile_line)
        {
            return std::views::iota(0, 8)
                | std::views::reverse
                | std::views::transform([tile_line] (const auto pixel_index)
                    {
                        const auto lsb = (tile_line >> (8 + pixel_index)) & 0b1;
                        const auto msb = (tile_line >> pixel_index) & 0b1;

                        return msb << 1 | lsb;
                    });
        };

        const auto tile_line_palette_indexes = tile_lines
            | std::views::transform(extract_tile_line_palette_indexes);

        QImage image { sprite_width, sprite_height, QImage::Format_RGBA8888 };

        const std::uint16_t palette_address = alternate_palette ? 0xFF49 : 0xFF48;
        const std::uint8_t palette = read_mem_fn(palette_address);

        for (auto&& [line_index, line_palette_indexes] : tile_line_palette_indexes | std::views::enumerate)
        {
            for (auto&& [pixel_index, palette_index] : line_palette_indexes | std::views::enumerate)
            {
                constexpr std::array sprite_color_table { 0xFFFFFFFF, 0xFFAAAAAA, 0xFF555555, 0xFF000000 };

                const std::uint8_t color_index = (palette >> (palette_index * 2)) & 0b11;
                const QColor pixel_color = color_index == 0 ? Qt::transparent : QColor::fromRgba(sprite_color_table[color_index]);
                image.setPixelColor(pixel_index, line_index, pixel_color);
            }
        }

        size->setHeight(image.width());
        size->setWidth(image.height());

        return image;
    }
}