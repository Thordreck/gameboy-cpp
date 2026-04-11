export module opcodes:stack;

export import cpu;
export import std;
export import utilities;
export import :common;

namespace opcodes
{
    template <R16RegisterProvider register_provider>
    struct pop_r16
    {
        static constexpr step_t num_steps(const cpu::cpu_state&) { return 3; }

        static void execute(cpu::cpu_state& cpu, const step_t step)
        {
            switch (step)
            {
            case 0:
                break;
            case 1:
                cpu.cache.r8 = cpu.memory->read(cpu.sp++);
                break;
            case 2:
                {
                    const std::uint8_t low_byte = cpu.cache.r8;
                    const std::uint8_t high_byte = cpu.memory->read(cpu.sp++);

                    register_provider::get(cpu) = utils::encode_little_endian(low_byte, high_byte);
                }
                break;
            default: std::unreachable();
            }
        }
    };

    export using pop_bc = pop_r16<bc_register_provider>;
    export using pop_de = pop_r16<de_register_provider>;
    export using pop_hl = pop_r16<hl_register_provider>;

    export struct pop_af
    {
        static constexpr step_t num_steps(const cpu::cpu_state&) { return 3; }

        static void execute(cpu::cpu_state& cpu, const step_t step)
        {
            switch (step)
            {
            case 0:
                break;
            case 1:
                cpu.cache.r8 = cpu.memory->read(cpu.sp++);
                break;
            case 2:
                {
                    const std::uint8_t low_byte = cpu.cache.r8;
                    const std::uint8_t high_byte = cpu.memory->read(cpu.sp++);
                    const std::uint16_t popped_result = utils::encode_little_endian(low_byte, high_byte);

                    cpu.reg.a() = popped_result >> 8;
                    cpu.reg.f() = popped_result & 0xF0;
                }
                break;
            default: std::unreachable();
            }
        }
    };

    template <ReadOnlyR16RegisterProvider register_provider>
    struct push_r16
    {
        static constexpr step_t num_steps(const cpu::cpu_state&) { return 4; }

        static void execute(cpu::cpu_state& cpu, const step_t step)
        {
            switch (step)
            {
            case 0:
                break;
            case 1:
                cpu.cache.r16 = register_provider::get(cpu);
                break;
            case 2:
                cpu.memory->write(--cpu.sp, utils::most_significant_byte(cpu.cache.r16));
                break;
            case 3:
                cpu.memory->write(--cpu.sp, utils::less_significant_byte(cpu.cache.r16));
                break;
            default: std::unreachable();
            }
        }
    };

    export using push_bc = push_r16<bc_readonly_register_provider>;
    export using push_de = push_r16<de_readonly_register_provider>;
    export using push_hl = push_r16<hl_readonly_register_provider>;

    export struct push_af
    {
        static constexpr step_t num_steps(const cpu::cpu_state&) { return 4; }

        static void execute(cpu::cpu_state& cpu, const step_t step)
        {
            switch (step)
            {
            case 0:
                break;
            case 1:
                cpu.cache.r16 = cpu.reg.af() & 0xFFF0;
                break;
            case 2:
                {
                    const std::uint16_t value = cpu.cache.r16;
                    cpu.memory->write(--cpu.sp, utils::most_significant_byte(value));
                }
                break;
            case 3:
                {
                    const std::uint16_t value = cpu.cache.r16;
                    cpu.memory->write(--cpu.sp, utils::less_significant_byte(value));
                }
                break;
            default: std::unreachable();
            }
        }
    };

    export struct ld_sp_n16
    {
        static constexpr step_t num_steps(const cpu::cpu_state&) { return 3; }

        static void execute(cpu::cpu_state& cpu, const step_t step)
        {
            switch (step)
            {
            case 0:
                break;
            case 1:
                cpu.cache.r8 = cpu.memory->read(cpu.pc++);
                break;
            case 2:
                {
                    const std::uint8_t low_byte = cpu.cache.r8;
                    const std::uint8_t high_byte = cpu.memory->read(cpu.pc++);

                    cpu.sp = utils::encode_little_endian(low_byte, high_byte);
                }
                break;
            default: std::unreachable();
            }
        }
    };

    export struct add_sp_e8
    {
        static constexpr step_t num_steps(const cpu::cpu_state&) { return 4; }

        static void execute(cpu::cpu_state& cpu, const step_t step)
        {
            switch (step)
            {
            case 0:
                break;
            case 1:
                cpu.cache.r8 = cpu.memory->read(cpu.pc++);
                break;
            case 2:
                {
                    const std::uint8_t unsigned_e8 = cpu.cache.r8;
                    const std::uint8_t sp_lower_byte = cpu.sp & 0xFF;

                    const bool half_overflow = utils::check_half_add_overflow(sp_lower_byte, unsigned_e8);
                    const bool overflow = utils::check_add_overflow(sp_lower_byte, unsigned_e8);

                    cpu.reg.z_flag() = false;
                    cpu.reg.n_flag() = false;
                    cpu.reg.h_flag() = half_overflow;
                    cpu.reg.c_flag() = overflow;
                }
                break;
            case 3:
                {
                    const auto e8 = static_cast<std::int8_t>(cpu.cache.r8);
                    cpu.sp = cpu.sp + e8;
                }
                break;
            default: std::unreachable();
            }
        }
    };

    export struct ld_hl_sp_e8
    {
        static constexpr step_t num_steps(const cpu::cpu_state&) { return 3; }

        static void execute(cpu::cpu_state& cpu, const step_t step)
        {
            switch (step)
            {
            case 0:
                break;
            case 1:
                cpu.cache.r8 = cpu.memory->read(cpu.pc++);
                break;
            case 2:
                {
                    const std::uint8_t unsigned_e8 = cpu.cache.r8;
                    const auto e8 = static_cast<std::int8_t>(unsigned_e8);
                    const std::uint8_t sp_lower_byte = cpu.sp & 0xFF;

                    const bool half_overflow = utils::check_half_add_overflow(sp_lower_byte, unsigned_e8);
                    const bool overflow = utils::check_add_overflow(sp_lower_byte, unsigned_e8);

                    cpu.reg.hl() = cpu.sp + e8;

                    cpu.reg.z_flag() = false;
                    cpu.reg.n_flag() = false;
                    cpu.reg.h_flag() = half_overflow;
                    cpu.reg.c_flag() = overflow;
                }
                break;
            default: std::unreachable();
            }
        }
    };
}
