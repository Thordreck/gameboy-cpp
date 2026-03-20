
export module graphics:oam;
export import memory;
export import std;

import utilities;

namespace graphics
{
    export constexpr memory::memory_address_t oam_start_address = 0xFE00;
    export constexpr memory::memory_address_t oam_end_address = 0xFE9F;
    export constexpr memory::memory_address_t oam_size = oam_end_address - oam_start_address + 1;
    export constexpr memory::memory_address_t oam_dma_transfer_address = 0xFF46;

    export using oam_t = std::span<memory::memory_data_t, oam_size>;
    export using const_oam_t = std::span<const memory::memory_data_t, oam_size>;

    export constexpr std::uint8_t object_memory_byte_size = 4;
    export using object_memory_t = std::span<memory::memory_data_t, object_memory_byte_size>;
    export using const_object_memory_t = std::span<const memory::memory_data_t, 4>;

    export constexpr std::uint8_t object_y_position_byte_index = 0;
    export constexpr std::uint8_t object_x_position_byte_index = 1;
    export constexpr std::uint8_t object_tile_index_byte_index = 2;
    export constexpr std::uint8_t object_flags_byte_index = 3;

    export constexpr std::uint8_t object_priority_flag_bit_index = 7;
    export constexpr std::uint8_t object_y_flip_flag_bit_index = 6;
    export constexpr std::uint8_t object_x_flip_flag_bit_index = 5;
    export constexpr std::uint8_t object_palette_bit_index = 4;

    export class object
    {
    public:
        template<std::ranges::contiguous_range R>
        explicit object(R&& data)
            : memory(data.begin(), 4)
        {}

        [[nodiscard]] std::uint8_t x() const
        {
            return memory[object_x_position_byte_index];
        }

        [[nodiscard]] std::uint8_t y() const
        {
            return memory[object_y_position_byte_index];
        }

        [[nodiscard]] std::uint8_t tile_index() const
        {
            return memory[object_tile_index_byte_index];
        }

        [[nodiscard]] bool priority() const
        {
            const auto flags_byte = memory[object_flags_byte_index];
            return utils::is_bit_set<object_priority_flag_bit_index>(flags_byte);
        }

        [[nodiscard]] bool x_flip() const
        {
            const auto flags_byte = memory[object_flags_byte_index];
            return utils::is_bit_set<object_x_flip_flag_bit_index>(flags_byte);
        }

        [[nodiscard]] bool y_flip() const
        {
            const auto flags_byte = memory[object_flags_byte_index];
            return utils::is_bit_set<object_y_flip_flag_bit_index>(flags_byte);
        }

        [[nodiscard]] bool alternate_palette() const
        {
            const auto flags_byte = memory[object_flags_byte_index];
            return utils::is_bit_set<object_palette_bit_index>(flags_byte);
        }

    private:
        const_object_memory_t memory;
    };

    export auto oam_objects(const_oam_t oam)
    {
        return oam
            | std::views::chunk(object_memory_byte_size)
            | std::ranges::views::transform([](auto chunk) { return object(chunk); });
    }
}