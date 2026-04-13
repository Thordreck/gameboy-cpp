export module opcodes:subroutine;

export import std;
export import cpu;
export import utilities;
import memory;

export import :common;
export import :jump;
export import :stack;

namespace opcodes
{
    export struct call_n16
    {
        static constexpr step_t num_steps(const cpu::cpu_state&) { return 6; }

        template<memory::Memory Memory>
        static void execute(cpu::cpu_state& cpu, const step_t step, Memory& memory)
        {
            switch (step)
            {
            case 0:
                break;
            case 1:
                {
                    using namespace literals;
                    cpu.cache.r16 = cpu.pc + 2_u16;
                    memory.write(--cpu.sp, utils::most_significant_byte(cpu.cache.r16));
                }
                break;
            case 2:
                memory.write(--cpu.sp, utils::less_significant_byte(cpu.cache.r16));
                break;
            case 3:
                cpu.cache.r8 = memory.read(cpu.pc++);
                break;
            case 4:
                {
                    const std::uint8_t low_byte = cpu.cache.r8;
                    const std::uint8_t high_byte = memory.read(cpu.pc++);

                    cpu.cache.r16 = utils::encode_little_endian(low_byte, high_byte);
                }
                break;
            case 5:
                cpu.pc = cpu.cache.r16;
                break;
            default: std::unreachable();
            }
        }
    };

    template <CPUStateCondition condition>
    struct call_cc_n16
    {
        static constexpr step_t num_steps(const cpu::cpu_state& cpu)
        {
            return condition::evaluate(cpu) ? 6 : 3;
        }

        template<memory::Memory Memory>
        static void execute(cpu::cpu_state& cpu, const step_t step, Memory& memory)
        {
            if (condition::evaluate(cpu))
            {
                call_n16::execute(cpu, step, memory);
            }
            else if (step == 2)
            {
                cpu.pc += 2;
            }
        }
    };

    export using call_z_n16 = call_cc_n16<is_z_set>;
    export using call_nz_n16 = call_cc_n16<is_z_not_set>;
    export using call_c_n16 = call_cc_n16<is_c_set>;
    export using call_nc_n16 = call_cc_n16<is_c_not_set>;

    export struct ret
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
                cpu.cache.r8 = memory.read(cpu.sp++);
                break;
            case 2:
                {
                    const std::uint8_t less_significant = cpu.cache.r8;
                    const std::uint8_t most_significant = memory.read(cpu.sp++);

                    cpu.cache.r16 = utils::encode_little_endian(less_significant, most_significant);
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
    struct ret_cc
    {
        static constexpr step_t num_steps(const cpu::cpu_state& cpu)
        {
            return condition::evaluate(cpu) ? 5 : 2;
        }

        template<memory::ReadOnlyMemory Memory>
        static void execute(cpu::cpu_state& cpu, const step_t step, const Memory& memory)
        {
            if (condition::evaluate(cpu))
            {
                ret::execute(cpu, step, memory);
            }
        }
    };

    export using ret_z = ret_cc<is_z_set>;
    export using ret_nz = ret_cc<is_z_not_set>;
    export using ret_c = ret_cc<is_c_set>;
    export using ret_nc = ret_cc<is_c_not_set>;

    export struct reti
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
                {
                    cpu.ime.enabled = true;
                    cpu.ime.enabling = false;
                    cpu.ime.requested = false;
                }
                break;
            case 2:
                cpu.cache.r8 = memory.read(cpu.sp++);
                break;
            case 3:
                {
                    const std::uint8_t less_significant = cpu.cache.r8;
                    const std::uint8_t most_significant = memory.read(cpu.sp++);

                    cpu.pc = utils::encode_little_endian(less_significant, most_significant);
                }
                break;
            default: std::unreachable();
            }
        }
    };

    template <std::uint8_t vec>
        requires RSTVector<vec>
    struct rst_vec
    {
        static constexpr step_t num_steps(const cpu::cpu_state&) { return 4; }

        template<memory::WriteOnlyMemory Memory>
        static void execute(cpu::cpu_state& cpu, const step_t step, Memory& memory)
        {
            switch (step)
            {
            case 0:
                cpu.cache.r16 = cpu.pc;
                break;
            case 1:
                memory.write(--cpu.sp, utils::most_significant_byte(cpu.cache.r16));
                break;
            case 2:
                memory.write(--cpu.sp, utils::less_significant_byte(cpu.cache.r16));
                break;
            case 3:
                cpu.pc = vec;
                break;
            default: std::unreachable();
            }
        }
    };

    export using rst_00 = rst_vec<0x00>;
    export using rst_08 = rst_vec<0x08>;
    export using rst_10 = rst_vec<0x10>;
    export using rst_18 = rst_vec<0x18>;
    export using rst_20 = rst_vec<0x20>;
    export using rst_28 = rst_vec<0x28>;
    export using rst_30 = rst_vec<0x30>;
    export using rst_38 = rst_vec<0x38>;
}
