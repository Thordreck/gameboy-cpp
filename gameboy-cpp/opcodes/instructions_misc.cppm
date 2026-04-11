export module opcodes:misc;

export import cpu;
import interrupts;
import :common;

namespace opcodes
{
    export struct nop
    {
        static constexpr step_t num_steps(const cpu::cpu_state&) { return 1; }
        static void execute(cpu::cpu_state&, const step_t) {}
    };

    export struct stop_n8
    {
        static constexpr step_t num_steps(const cpu::cpu_state&) { return 1; }

        static void execute(cpu::cpu_state& cpu, const step_t)
        {
            // TODO: implement properly
            ++cpu.pc;
        }
    };

    export struct halt
    {
        static constexpr step_t num_steps(const cpu::cpu_state&) { return 1; }

        static void execute(cpu::cpu_state& cpu, const step_t)
        {
            cpu.halt.enabled = true;
            cpu.halt.ime_flag_set = cpu.ime.enabled;
            cpu.halt.interrupts_pending = interrupts::is_any_interrupt_pending(cpu);
        }
    };

    export struct daa
    {
        static constexpr step_t num_steps(const cpu::cpu_state&) { return 1; }

        static void execute(cpu::cpu_state& cpu, const step_t)
        {
            bool should_carry = false;
            std::uint8_t adjustment = 0x0;

            const std::uint8_t a = cpu.reg.a();
            const bool half_carry = cpu.reg.h_flag();
            const bool carry = cpu.reg.c_flag();
            const bool substract = cpu.reg.n_flag();

            if ((!substract && (a & 0xF) > 0x09) || half_carry)
            {
                adjustment |= 0x06;
            }

            if ((!substract && a > 0x99) || carry)
            {
                adjustment |= 0x60;
                should_carry = true;
            }

            cpu.reg.a() = substract
                ? cpu.reg.a() - adjustment
                : cpu.reg.a() + adjustment;

            cpu.reg.z_flag() = cpu.reg.a() == 0;
            cpu.reg.h_flag() = false;
            cpu.reg.c_flag() = should_carry;
        }
    };
}
