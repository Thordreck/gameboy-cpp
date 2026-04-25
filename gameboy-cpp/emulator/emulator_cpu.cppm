module;
#include "profiling.hpp"

export module emulator.engine:cpu;

export import std;
export import cpu;
export import opcodes;
export import interrupts;
import memory;

namespace emulator
{
    export class cpu_runner
    {
        enum class state : std::uint8_t
        {
            fetch_decode,
            fetch_decode_prefixed,
            execute,
            execute_prefixed,
            halt,
            interrupt,
        };

    public:
        explicit cpu_runner(cpu::cpu_state& cpu)
            : cpu{cpu}
        {}

        [[nodiscard]] bool active() const { return true; }
        [[nodiscard]] std::uint32_t tick_batch() const { return remaining_cycles_in_state; }

        template<memory::Memory Memory>
        void tick(std::uint32_t num_ticks, Memory& memory)
        {
            PROFILER_SCOPE("CPU Runner:tick()");

            while (num_ticks > 0)
            {
                const std::uint32_t consumed_ticks = step(num_ticks, memory);
                num_ticks -= consumed_ticks;
            }
        }

    private:
        template<memory::Memory Memory>
        std::uint32_t step(const std::uint32_t num_ticks, Memory& memory)
        {
            using enum state;

            switch (current_state)
            {
            case fetch_decode: return handle_fetch_decode(num_ticks, memory);
            case fetch_decode_prefixed: return handle_fetch_decode_prefixed(num_ticks, memory);
            case execute: return handle_execute<opcodes::dispatch<Memory>>(num_ticks, memory);
            case execute_prefixed: return handle_execute<opcodes::dispatch_prefixed<Memory>>(num_ticks, memory);
            case halt: return handle_halt(num_ticks, memory);
            case interrupt: return handle_interrupt(num_ticks, memory);
            default: std::unreachable();
            }
        }

        template<memory::ReadOnlyMemory Memory>
        std::uint32_t handle_fetch_decode(const std::uint32_t num_ticks, const Memory& memory)
        {
            const std::uint32_t ticks_consumed = std::min(num_ticks, remaining_cycles_in_state);
            remaining_cycles_in_state -= ticks_consumed;

            if (remaining_cycles_in_state == 0)
            {
                current_opcode = memory.read(cpu.pc++);
                const bool prefix_mode = current_opcode == opcodes::prefix_opcode;

                using enum state;
                current_state = prefix_mode ? fetch_decode_prefixed : execute;
                remaining_cycles_in_state = current_state == execute ? opcodes::get_num_steps(cpu, current_opcode) * 4 : 4;
                current_execution_cycle = 0;
            }

            return ticks_consumed;
        }

        template<memory::ReadOnlyMemory Memory>
        std::uint32_t handle_fetch_decode_prefixed(const std::uint32_t num_ticks, const Memory& memory)
        {
            const std::uint32_t ticks_consumed = std::min(num_ticks, remaining_cycles_in_state);
            remaining_cycles_in_state -= ticks_consumed;

            if (remaining_cycles_in_state == 0)
            {
                current_opcode = memory.read(cpu.pc++);
                current_state = state::execute_prefixed;
                remaining_cycles_in_state = opcodes::get_num_steps_prefixed(cpu, current_opcode) * 4;
                current_execution_cycle = 0;
            }

            return ticks_consumed;
        }

        template <auto Dispatcher, memory::Memory Memory>
        requires std::invocable<decltype(Dispatcher), cpu::cpu_state&, opcodes::opcode_t, opcodes::step_t, Memory&>
            && std::same_as<std::invoke_result_t<decltype(Dispatcher), cpu::cpu_state&, opcodes::opcode_t, opcodes::step_t, Memory&>, void>
        std::uint32_t handle_execute(const std::uint32_t num_ticks, Memory& memory)
        {
            const std::uint32_t ticks_consumed = std::min(num_ticks, remaining_cycles_in_state);
            remaining_cycles_in_state -= ticks_consumed;

            std::uint32_t ticks_left = ticks_consumed;

            while (ticks_left > 0)
            {
                const std::uint32_t next_boundary = (current_execution_cycle / 4 + 1) * 4;
                const std::uint32_t ticks_to_boundary = next_boundary - current_execution_cycle;
                const std::uint32_t chunk = std::min(ticks_left, ticks_to_boundary);

                current_execution_cycle += chunk;
                ticks_left -= chunk;

                if (current_execution_cycle % 4 == 0)
                {
                    const opcodes::step_t step = current_execution_cycle / 4 - 1;
                    Dispatcher(cpu, current_opcode, step, memory);
                }
            }

            if (remaining_cycles_in_state != 0)
            {
                return ticks_consumed;
            }

            if (cpu.ime.requested)
            {
                cpu.ime.enabled = cpu.ime.enabling;
                cpu.ime.enabling = !cpu.ime.enabling;
                cpu.ime.requested = !cpu.ime.enabled;
            }

            if (cpu.ime.enabled && interrupts::is_any_interrupt_pending(memory))
            {
                current_state = state::interrupt;
                current_execution_cycle = 0;
                remaining_cycles_in_state = interrupts::dispatcher::num_steps() * 4;
                current_interrupt = interrupts::get_first_pending_interrupt(memory);
            }
            else if (cpu.halt.enabled)
            {
                current_state = state::halt;
                remaining_cycles_in_state = std::numeric_limits<std::uint32_t>::max();
            }
            else
            {
                current_state = state::fetch_decode;
                remaining_cycles_in_state = 4;
                handle_fetch_decode(remaining_cycles_in_state, memory);
            }

            return ticks_consumed;
        }

        template<memory::Memory Memory>
        std::uint32_t handle_interrupt(const std::uint32_t num_ticks, Memory& memory)
        {
            const std::uint32_t ticks_consumed = std::min(num_ticks, remaining_cycles_in_state);
            remaining_cycles_in_state -= ticks_consumed;

            std::uint32_t ticks_left = ticks_consumed;

            while (ticks_left > 0)
            {
                const std::uint32_t next_boundary = (current_execution_cycle / 4 + 1) * 4;
                const std::uint32_t ticks_to_boundary = next_boundary - current_execution_cycle;
                const std::uint32_t chunk = std::min(ticks_left, ticks_to_boundary);

                current_execution_cycle += chunk;
                ticks_left -= chunk;

                if (current_execution_cycle % 4 == 0)
                {
                    const opcodes::step_t step = current_execution_cycle / 4 - 1;
                    interrupts::dispatcher::execute(current_interrupt.value(), cpu, step, memory);
                }
            }

            if (remaining_cycles_in_state == 0)
            {
                current_state = state::fetch_decode;
                remaining_cycles_in_state = 4;
            }

            return ticks_consumed;
        }

        template<memory::Memory Memory>
        std::uint32_t handle_halt(const std::uint32_t num_ticks, Memory& memory)
        {
            if (!interrupts::is_any_interrupt_pending(memory))
            {
                return num_ticks;
            }

            cpu.halt.enabled = false;
            using enum state;

            // interrupts
            if (cpu.halt.ime_flag_set)
            {
                current_state = interrupt;
                remaining_cycles_in_state = interrupts::dispatcher::num_steps() * 4;
                current_interrupt = interrupts::get_first_pending_interrupt(memory);

                const std::uint32_t available_ticks = std::min(num_ticks, remaining_cycles_in_state);
                return handle_interrupt(available_ticks, memory);
            }

            // Fetch-decode next instruction
            current_state = fetch_decode;
            remaining_cycles_in_state = 4;

            const std::uint32_t available_ticks = std::min(num_ticks, remaining_cycles_in_state);
            return handle_fetch_decode(available_ticks, memory);
        }

        cpu::cpu_state& cpu;

        state current_state { state::fetch_decode };
        std::uint8_t current_execution_cycle {};
        std::uint32_t remaining_cycles_in_state { 4 };

        std::uint8_t current_opcode {};
        std::optional<interrupts::interrupt> current_interrupt;
    };
}
