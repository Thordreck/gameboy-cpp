module;
#include "profiling.hpp"

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

    export struct object
    {
        object(const std::uint8_t index, const std::span<const std::uint8_t, 4> memory)
            : object {
                index,
                memory[object_x_position_byte_index],
                memory[object_y_position_byte_index],
                memory[object_tile_index_byte_index],
                memory[object_flags_byte_index],
            }
        {}

        object(
            const std::uint8_t index,
            const std::uint8_t x_position,
            const std::uint8_t y_position,
            const std::uint8_t tile_index,
            const std::uint8_t flags)
                : object {
                    index,
                    x_position,
                    y_position,
                    tile_index,
                    utils::is_bit_set<object_priority_flag_bit_index>(flags),
                    utils::is_bit_set<object_x_flip_flag_bit_index>(flags),
                    utils::is_bit_set<object_y_flip_flag_bit_index>(flags),
                    utils::is_bit_set<object_palette_bit_index>(flags),
                    }
        {}

        object(
            const std::uint8_t index,
            const std::uint8_t x_position,
            const std::uint8_t y_position,
            const std::uint8_t tile_index,
            const bool priority,
            const bool x_flip,
            const bool y_flip,
            const bool alternate_palette)
                : index { index }
                , x { x_position }
                , y { y_position }
                , tile_index { tile_index }
                , priority { priority }
                , x_flip { x_flip }
                , y_flip { y_flip }
                , alternate_palette { alternate_palette }
        {}

        std::uint8_t index;
        std::uint8_t x;
        std::uint8_t y;
        std::uint8_t tile_index;
        bool priority;
        bool x_flip;
        bool y_flip;
        bool alternate_palette;
    };

    export template <memory::ReadOnlyMemory Memory>
    object get_object(const std::uint8_t index, const Memory& memory)
    {
        const memory::memory_address_t initial_address = oam_start_address + index * object_memory_byte_size;

        return {
            index,
            memory.read(initial_address + object_x_position_byte_index),
            memory.read(initial_address + object_y_position_byte_index),
            memory.read(initial_address + object_tile_index_byte_index),
            memory.read(initial_address + object_flags_byte_index)
        };
    }

    export bool is_in_scanline(const object& object, const std::uint8_t height, const std::uint8_t scanline)
    {
        const int min_y = static_cast<int>(object.y) - 16;
        const int max_y = min_y + height;

        return scanline >= min_y && scanline < max_y;
    }

    export class oam_dma
    {
        enum class state : std::uint8_t
        {
            inactive,
            starting,
            restarting,
            active,
        };

    public:
        [[nodiscard]] bool is_transfer_active() const
        {
            return current_state == state::active || current_state == state::restarting;
        }

        [[nodiscard]] memory::memory_address_t start_address() const { return last_written_start; }
        [[nodiscard]] memory::memory_address_t active_start_address() const { return active_start; }
        [[nodiscard]] bool active() const { return current_state != state::inactive; }

        [[nodiscard]] std::uint32_t tick_batch() const
        {
            using enum state;

            switch (current_state)
            {
            case starting:
                return remaining_ticks_to_start_new_transfer;
            case restarting:
                return std::min(remaining_ticks_to_start_new_transfer, remaining_ticks_to_transfer_step);
            case active:
                return remaining_ticks_to_transfer_step;
            default:
                return std::numeric_limits<std::uint32_t>::max();
            }
        }

        void start_transfer(const memory::memory_address_t start_address)
        {
            using enum state;

            last_written_start = start_address;
            current_state = current_state == active ? restarting : starting;
            remaining_ticks_to_start_new_transfer = 4;
        }

        template <memory::Memory Memory>
        void tick(std::uint32_t num_ticks, Memory& memory)
        {
            PROFILER_SCOPE("OAM DMA::tick()");

            while (num_ticks > 0)
            {
                const std::uint32_t consumed_ticks = step(num_ticks, memory);
                num_ticks -= consumed_ticks;
            }
        }

    private:
        template <memory::Memory Memory>
        std::uint32_t step(const std::uint32_t num_ticks, Memory& memory)
        {
            using enum state;

            switch (current_state)
            {
            case starting: return handle_starting(num_ticks);
            case restarting: return handle_restarting(num_ticks, memory);
            case active: return handle_active(num_ticks, memory);
            default: std::unreachable();
            }
        }

        std::uint32_t handle_starting(const std::uint32_t num_ticks)
        {
            const std::uint32_t consumed_ticks = std::min(num_ticks, remaining_ticks_to_start_new_transfer);
            remaining_ticks_to_start_new_transfer -= consumed_ticks;

            if (remaining_ticks_to_start_new_transfer == 0)
            {
                current_state = state::active;
                current_byte = 0;
                active_start = last_written_start;
                remaining_ticks_to_transfer_step = 8;
            }

            return consumed_ticks;
        }

        template <memory::Memory Memory>
        std::uint32_t handle_restarting(const std::uint32_t num_ticks, Memory& memory)
        {
            const std::uint32_t consumed_ticks = std::min(
                num_ticks,
                std::min(remaining_ticks_to_start_new_transfer,
                remaining_ticks_to_transfer_step));

            remaining_ticks_to_start_new_transfer -= consumed_ticks;
            remaining_ticks_to_transfer_step -= consumed_ticks;

            if (remaining_ticks_to_transfer_step == 0)
            {
                execute_transfer_step(active_start, current_byte++, memory);
                remaining_ticks_to_transfer_step = 4;
            }

            if (remaining_ticks_to_start_new_transfer == 0)
            {
                current_state = state::active;
                current_byte = 0;
                active_start = last_written_start;
                remaining_ticks_to_transfer_step = 8;
            }

            return consumed_ticks;
        }

        template <memory::Memory Memory>
        std::uint32_t handle_active(const std::uint32_t num_ticks, Memory& memory)
        {
            using enum state;
            std::uint32_t ticks_to_consume = num_ticks;

            while (current_state == active && ticks_to_consume > 0)
            {
                const std::uint32_t next_step = std::min(ticks_to_consume, remaining_ticks_to_transfer_step);

                remaining_ticks_to_transfer_step -= next_step;
                ticks_to_consume -= next_step;

                if (remaining_ticks_to_transfer_step == 0)
                {
                    execute_transfer_step(active_start, current_byte++, memory);

                    const bool is_done = current_byte >= oam_size;
                    current_state = is_done ? inactive : active;
                    remaining_ticks_to_transfer_step = 4;
                }
            }

            return num_ticks;
        }

        template <memory::Memory Memory>
        static void execute_transfer_step(
            const memory::memory_address_t start,
            const std::uint8_t current_byte,
            Memory& memory)
        {
            using namespace memory;

            const memory_address_t raw_source_address = start + current_byte;
            const memory_address_t source_address = raw_source_address >= 0xE000
                                                        ? raw_source_address - 0x2000
                                                        : raw_source_address;

            const memory_address_t target_address = oam_start_address + current_byte;
            const memory_data_t byte_to_copy = memory.read(source_address);

            memory.write(target_address, byte_to_copy);
        }

        state current_state{state::inactive};
        std::uint32_t remaining_ticks_to_transfer_step {};
        std::uint32_t remaining_ticks_to_start_new_transfer {};

        std::uint8_t current_byte{};
        memory::memory_address_t active_start{};
        memory::memory_address_t last_written_start{};
    };
}
