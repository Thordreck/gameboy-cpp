export module interrupts:dispatch;

import std;

import cpu;
import memory;
import utilities;

import :common;
import :control;

namespace interrupts
{
    // TODO: remove static state.
    static bool interrupt_cancelled { false };

    export struct interrupt_dispatcher
    {
        static constexpr std::uint8_t num_steps() { return 4; }

        template<memory::Memory Memory, InterruptController Controller>
        static void execute(
            const interrupt request,
            cpu::cpu_state& cpu,
            const std::uint8_t step,
            Memory& memory,
            Controller& controller)
        {
            switch (step)
            {
            case 0:
                {
                    interrupt_cancelled = false;
                    cpu.ime.enabled = false;
                    cpu.ime.ticks_until_enabled = 0;
                    cpu.pc = cpu.pc - 1;
                }
                break;
            case 1:
                {
                    memory.write(--cpu.sp, utils::most_significant_byte(cpu.pc.value()));
                    interrupt_cancelled = !controller.is_enabled(request);
                }
                break;
            case 2:
                memory.write(--cpu.sp, utils::less_significant_byte(cpu.pc.value()));
                break;
            case 3:
                {
                    const std::optional<interrupt> dispatched_interrupt = !interrupt_cancelled
                        ? std::make_optional(request)
                        : controller.get_first_pending();

                    cpu.pc = 0x0000;

                    if (dispatched_interrupt.has_value())
                    {
                        const auto effective_interrupt = dispatched_interrupt.value();
                        cpu.pc = effective_interrupt.handler_address();
                        controller.clear_request(effective_interrupt);
                    }

                }
                break;
            default: std::unreachable();
            }
        }
    };
}
