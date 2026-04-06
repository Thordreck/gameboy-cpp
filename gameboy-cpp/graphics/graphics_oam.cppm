
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
    export using object_memory_t = std::array<memory::memory_data_t, object_memory_byte_size>;
    export using const_object_memory_t = std::array<const memory::memory_data_t, 4>;

    export constexpr std::uint8_t object_y_position_byte_index = 0;
    export constexpr std::uint8_t object_x_position_byte_index = 1;
    export constexpr std::uint8_t object_tile_index_byte_index = 2;
    export constexpr std::uint8_t object_flags_byte_index = 3;

    export constexpr std::uint8_t object_priority_flag_bit_index = 7;
    export constexpr std::uint8_t object_y_flip_flag_bit_index = 6;
    export constexpr std::uint8_t object_x_flip_flag_bit_index = 5;
    export constexpr std::uint8_t object_palette_bit_index = 4;

    export constexpr std::uint8_t max_num_objects_per_scanline = 10;

    export class object
    {
    public:
        template<std::ranges::contiguous_range R>
        explicit object(const std::uint8_t oam_index, const R& data)
            : oam_index { oam_index }
        {
            std::ranges::copy(data, memory.begin());
        }

        [[nodiscard]] std::uint8_t index() const
        {
            return oam_index;
        }

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
        std::uint8_t oam_index;
        object_memory_t memory {};
    };

    export object get_object(const std::uint8_t index, const memory::memory_bus& memory)
    {
        const memory::memory_address_t initial_address = oam_start_address + index * object_memory_byte_size;
        const auto bytes =
        {
            memory.read(initial_address),
            memory.read(initial_address + 1),
            memory.read(initial_address + 2),
            memory.read(initial_address + 3)
        };

        return object { index, bytes };
    }

    export bool is_in_scanline(const object& object, const std::uint8_t height, const std::uint8_t scanline)
    {
        const int min_y = static_cast<int>(object.y()) - 16;
        const int max_y = min_y + height;

        return scanline >= min_y && scanline < max_y;
    }

    export class oam_dma
    {
    public:
        void start_transfer(const memory::memory_address_t start_address)
        {
            current_byte = 0;
            current_tick = 0;
            start = start_address;
            is_transferring = true;
        }

        [[nodiscard]] bool is_transfer_active() const { return is_transferring; }
        [[nodiscard]] memory::memory_address_t start_address() const { return start; }

        void tick()
        {
            if (is_transferring && ++current_tick % 4 == 0)
            {
                using namespace memory;
                utils::assert_not_nullptr(memory);

                const memory_address_t source_address = start + current_byte;
                const memory_address_t target_address = oam_start_address + current_byte;
                const memory_data_t byte_to_copy = memory->read(source_address);

                memory->write(target_address, byte_to_copy);

                if (++current_byte >= oam_size)
                {
                    is_transferring = false;
                }
            }
        }

        void connect(memory::memory_bus& bus)
        {
            memory = &bus;
        }

    private:
        memory::memory_bus* memory { nullptr };

        bool is_transferring {};
        std::uint16_t current_tick {};
        std::uint8_t current_byte {};
        memory::memory_address_t start {};
    };

}