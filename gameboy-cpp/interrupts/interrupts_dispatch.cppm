export module interrupts:dispatch;

export import std;
export import cpu;
export import utilities;
export import :common;
export import :request;

namespace interrupts
{
    export template <InterruptDescriptor interrupt>
    struct dispatcher
    {
        static constexpr std::uint8_t num_steps() { return 5; }

        static void execute(cpu::cpu_state& cpu, const std::uint8_t step)
        {
            switch (step)
            {
            case 0:
                {
                    cpu.ime.enabled = false;
                    cpu.ime.requested = false;
                    cpu.ime.enabling = false;
                    clear_request<interrupt>(*cpu.memory);
                }
                break;
            case 1:
                break;
            case 2:
                cpu.memory->write(--cpu.sp, utils::most_significant_byte(cpu.pc.value()));
                break;
            case 3:
                cpu.memory->write(--cpu.sp, utils::less_significant_byte(cpu.pc.value()));
                break;
            case 4:
                cpu.pc = interrupt::handler_address;
                break;
            default: std::unreachable();
            }
        }
    };
}
