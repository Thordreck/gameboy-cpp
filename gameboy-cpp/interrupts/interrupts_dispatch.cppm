export module interrupts:dispatch;

export import std;
export import cpu;
export import utilities;
export import :common;
export import :request;
import memory;

namespace interrupts
{
    export struct dispatcher
    {
        static constexpr std::uint8_t num_steps() { return 5; }

        template<InterruptDescriptor Interrupt, memory::Memory Memory>
        static void execute(const Interrupt& interrupt, cpu::cpu_state& cpu, const std::uint8_t step, Memory& memory)
        {
            switch (step)
            {
            case 0:
                {
                    cpu.ime.enabled = false;
                    cpu.ime.requested = false;
                    cpu.ime.enabling = false;
                    clear_request(interrupt, memory);
                }
                break;
            case 1:
                break;
            case 2:
                memory.write(--cpu.sp, utils::most_significant_byte(cpu.pc.value()));
                break;
            case 3:
                memory.write(--cpu.sp, utils::less_significant_byte(cpu.pc.value()));
                break;
            case 4:
                cpu.pc = interrupt.handler_address();
                break;
            default: std::unreachable();
            }
        }
    };
}
