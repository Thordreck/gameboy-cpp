export module interrupts:dispatch;

import std;

import cpu;
import memory;
import utilities;

import :common;
import :control;

namespace interrupts
{
    export struct interrupt_dispatcher
    {
        static constexpr std::uint8_t num_steps() { return 4; }

        template<memory::Memory Memory, InterruptRequestController Controller>
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
                    cpu.ime.enabled = false;
                    cpu.ime.requested = false;
                    cpu.ime.enabling = false;
                }
                break;
            case 1:
                memory.write(--cpu.sp, utils::most_significant_byte(cpu.pc.value()));
                break;
            case 2:
                memory.write(--cpu.sp, utils::less_significant_byte(cpu.pc.value()));
                break;
            case 3:
                {
                    cpu.pc = request.handler_address();
                    controller.clear_request(request);
                }
                break;
            default: std::unreachable();
            }
        }
    };
}
