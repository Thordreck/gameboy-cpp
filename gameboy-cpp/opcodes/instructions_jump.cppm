export module opcodes:jump;

export import :common;
export import cpu;
export import utilities;
export import std;
import memory;

namespace opcodes
{
    export struct jp_n16
    {
        static constexpr step_t num_steps(const cpu::cpu_state&) { return 4; }

        template<memory::ReadOnlyMemory Memory>
        static void execute(cpu::cpu_state& cpu, const step_t step, const Memory& memory)
        {
            switch (step)
            {
            case 0:
                break;
            case 1:
                cpu.cache.r8 = memory.read(cpu.pc++);
                break;
            case 2:
                {
                    const std::uint8_t low_byte = cpu.cache.r8;
                    const std::uint8_t high_byte = memory.read(cpu.pc++);

                    cpu.cache.r16 = utils::encode_little_endian(low_byte, high_byte);
                }
                break;
            case 3:
                cpu.pc = cpu.cache.r16;
                break;
            default: std::unreachable();
            }
        }
    };

    template <CPUStateCondition condition>
    struct jp_cc_n16
    {
        static constexpr step_t num_steps(const cpu::cpu_state& cpu)
        {
            return condition::evaluate(cpu) ? 4 : 3;
        }

        template<memory::Memory Memory>
        static void execute(cpu::cpu_state& cpu, const step_t step, Memory& memory)
        {
            if (condition::evaluate(cpu))
            {
                jp_n16::execute(cpu, step, memory);
            }
            else if (step == 2)
            {
                cpu.pc += 2;
            }
        }
    };

    export using jp_z_n16 = jp_cc_n16<is_z_set>;
    export using jp_nz_n16 = jp_cc_n16<is_z_not_set>;
    export using jp_c_n16 = jp_cc_n16<is_c_set>;
    export using jp_nc_n16 = jp_cc_n16<is_c_not_set>;

    export struct jr_n16
    {
        static constexpr step_t num_steps(const cpu::cpu_state&) { return 3; }

        template<memory::ReadOnlyMemory Memory>
        static void execute(cpu::cpu_state& cpu, const step_t step, const Memory& memory)
        {
            switch (step)
            {
            case 0:
                break;
            case 1:
                cpu.cache.r8 = memory.read(cpu.pc++);
                break;
            case 2:
                {
                    const std::uint8_t raw_offset = cpu.cache.r8;
                    cpu.pc += static_cast<std::int8_t>(raw_offset);
                }
                break;
            default: std::unreachable();
            }
        }
    };

    template <CPUStateCondition condition>
    struct jr_cc_n16
    {
        static constexpr step_t num_steps(const cpu::cpu_state& cpu)
        {
            return condition::evaluate(cpu) ? 3: 2;
        }

        template<memory::ReadOnlyMemory Memory>
        static void execute(cpu::cpu_state& cpu, const step_t step, const Memory& memory)
        {
            if (condition::evaluate(cpu))
            {
                jr_n16::execute(cpu, step, memory);
            }
            else if (step == 1)
            {
                ++cpu.pc;
            }
        }
    };

    export using jr_z_n16 = jr_cc_n16<is_z_set>;
    export using jr_nz_n16 = jr_cc_n16<is_z_not_set>;
    export using jr_c_n16 = jr_cc_n16<is_c_set>;
    export using jr_nc_n16 = jr_cc_n16<is_c_not_set>;

    export struct jp_hl
    {
        static constexpr step_t num_steps(const cpu::cpu_state&) { return 1; }

        static void execute(cpu::cpu_state& cpu, const step_t)
        {
            cpu.pc = cpu.reg.hl();
        }
    };
}
