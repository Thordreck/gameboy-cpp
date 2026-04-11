export module opcodes:load;

export import cpu;
export import std;
export import utilities;
export import :common;

namespace opcodes
{
    // ld_r8_r8
    template <R8RegisterProvider lhs_provider, ReadOnlyR8RegisterProvider rhs_provider>
    struct ld_r8_r8
    {
        static constexpr step_t num_steps(const cpu::cpu_state&) { return 1; }

        static void execute(cpu::cpu_state& cpu, const step_t)
        {
            lhs_provider::get(cpu) = rhs_provider::get(cpu);
        }
    };

    // ld_a_r8
    export using ld_a_a = ld_r8_r8<a_register_provider, a_readonly_register_provider>;
    export using ld_a_b = ld_r8_r8<a_register_provider, b_readonly_register_provider>;
    export using ld_a_c = ld_r8_r8<a_register_provider, c_readonly_register_provider>;
    export using ld_a_d = ld_r8_r8<a_register_provider, d_readonly_register_provider>;
    export using ld_a_e = ld_r8_r8<a_register_provider, e_readonly_register_provider>;
    export using ld_a_h = ld_r8_r8<a_register_provider, h_readonly_register_provider>;
    export using ld_a_l = ld_r8_r8<a_register_provider, l_readonly_register_provider>;

    // ld_b_r8
    export using ld_b_a = ld_r8_r8<b_register_provider, a_readonly_register_provider>;
    export using ld_b_b = ld_r8_r8<b_register_provider, b_readonly_register_provider>;
    export using ld_b_c = ld_r8_r8<b_register_provider, c_readonly_register_provider>;
    export using ld_b_d = ld_r8_r8<b_register_provider, d_readonly_register_provider>;
    export using ld_b_e = ld_r8_r8<b_register_provider, e_readonly_register_provider>;
    export using ld_b_h = ld_r8_r8<b_register_provider, h_readonly_register_provider>;
    export using ld_b_l = ld_r8_r8<b_register_provider, l_readonly_register_provider>;

    // ld_c_r8
    export using ld_c_a = ld_r8_r8<c_register_provider, a_readonly_register_provider>;
    export using ld_c_b = ld_r8_r8<c_register_provider, b_readonly_register_provider>;
    export using ld_c_c = ld_r8_r8<c_register_provider, c_readonly_register_provider>;
    export using ld_c_d = ld_r8_r8<c_register_provider, d_readonly_register_provider>;
    export using ld_c_e = ld_r8_r8<c_register_provider, e_readonly_register_provider>;
    export using ld_c_h = ld_r8_r8<c_register_provider, h_readonly_register_provider>;
    export using ld_c_l = ld_r8_r8<c_register_provider, l_readonly_register_provider>;

    // ld_d_r8
    export using ld_d_a = ld_r8_r8<d_register_provider, a_readonly_register_provider>;
    export using ld_d_b = ld_r8_r8<d_register_provider, b_readonly_register_provider>;
    export using ld_d_c = ld_r8_r8<d_register_provider, c_readonly_register_provider>;
    export using ld_d_d = ld_r8_r8<d_register_provider, d_readonly_register_provider>;
    export using ld_d_e = ld_r8_r8<d_register_provider, e_readonly_register_provider>;
    export using ld_d_h = ld_r8_r8<d_register_provider, h_readonly_register_provider>;
    export using ld_d_l = ld_r8_r8<d_register_provider, l_readonly_register_provider>;

    // ld_e_r8
    export using ld_e_a = ld_r8_r8<e_register_provider, a_readonly_register_provider>;
    export using ld_e_b = ld_r8_r8<e_register_provider, b_readonly_register_provider>;
    export using ld_e_c = ld_r8_r8<e_register_provider, c_readonly_register_provider>;
    export using ld_e_d = ld_r8_r8<e_register_provider, d_readonly_register_provider>;
    export using ld_e_e = ld_r8_r8<e_register_provider, e_readonly_register_provider>;
    export using ld_e_h = ld_r8_r8<e_register_provider, h_readonly_register_provider>;
    export using ld_e_l = ld_r8_r8<e_register_provider, l_readonly_register_provider>;

    // ld_h_r8
    export using ld_h_a = ld_r8_r8<h_register_provider, a_readonly_register_provider>;
    export using ld_h_b = ld_r8_r8<h_register_provider, b_readonly_register_provider>;
    export using ld_h_c = ld_r8_r8<h_register_provider, c_readonly_register_provider>;
    export using ld_h_d = ld_r8_r8<h_register_provider, d_readonly_register_provider>;
    export using ld_h_e = ld_r8_r8<h_register_provider, e_readonly_register_provider>;
    export using ld_h_h = ld_r8_r8<h_register_provider, h_readonly_register_provider>;
    export using ld_h_l = ld_r8_r8<h_register_provider, l_readonly_register_provider>;

    // ld_l_r8
    export using ld_l_a = ld_r8_r8<l_register_provider, a_readonly_register_provider>;
    export using ld_l_b = ld_r8_r8<l_register_provider, b_readonly_register_provider>;
    export using ld_l_c = ld_r8_r8<l_register_provider, c_readonly_register_provider>;
    export using ld_l_d = ld_r8_r8<l_register_provider, d_readonly_register_provider>;
    export using ld_l_e = ld_r8_r8<l_register_provider, e_readonly_register_provider>;
    export using ld_l_h = ld_r8_r8<l_register_provider, h_readonly_register_provider>;
    export using ld_l_l = ld_r8_r8<l_register_provider, l_readonly_register_provider>;

    // ld_r8_n8
    template <R8RegisterProvider reg_provider>
    struct ld_r8_n8
    {
        static constexpr step_t num_steps(const cpu::cpu_state&) { return 2; }

        static void execute(cpu::cpu_state& cpu, const step_t step)
        {
            if (step == 1)
            {
                reg_provider::get(cpu) = cpu.memory->read(cpu.pc++);
            }
        }
    };

    export using ld_a_n8 = ld_r8_n8<a_register_provider>;
    export using ld_b_n8 = ld_r8_n8<b_register_provider>;
    export using ld_c_n8 = ld_r8_n8<c_register_provider>;
    export using ld_d_n8 = ld_r8_n8<d_register_provider>;
    export using ld_e_n8 = ld_r8_n8<e_register_provider>;
    export using ld_h_n8 = ld_r8_n8<h_register_provider>;
    export using ld_l_n8 = ld_r8_n8<l_register_provider>;

    // ld r16,n16
    template <R16RegisterProvider reg_provider>
    struct ld_r16_n16
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

                    reg_provider::get(cpu) = utils::encode_little_endian(low_byte, high_byte);
                }
                break;
            default: std::unreachable();
            }
        }
    };

    export using ld_bc_n16 = ld_r16_n16<bc_register_provider>;
    export using ld_de_n16 = ld_r16_n16<de_register_provider>;
    export using ld_hl_n16 = ld_r16_n16<hl_register_provider>;

    // ld hl,r8
    template <ReadOnlyR8RegisterProvider reg_provider>
    struct ld_hl_r8
    {
        static constexpr step_t num_steps(const cpu::cpu_state&) { return 2; }

        static void execute(cpu::cpu_state& cpu, const step_t step)
        {
            if (step == 1)
            {
                cpu.memory->write(cpu.reg.hl(), reg_provider::get(cpu));
            }
        }
    };

    export using ld_hl_a = ld_hl_r8<a_readonly_register_provider>;
    export using ld_hl_b = ld_hl_r8<b_readonly_register_provider>;
    export using ld_hl_c = ld_hl_r8<c_readonly_register_provider>;
    export using ld_hl_d = ld_hl_r8<d_readonly_register_provider>;
    export using ld_hl_e = ld_hl_r8<e_readonly_register_provider>;
    export using ld_hl_h = ld_hl_r8<h_readonly_register_provider>;
    export using ld_hl_l = ld_hl_r8<l_readonly_register_provider>;

    export struct ld_hl_n8
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
                cpu.memory->write(cpu.reg.hl(), cpu.cache.r8);
                break;
            default: std::unreachable();
            }
        }
    };

    // ld r8,hl
    template <R8RegisterProvider reg_provider>
    struct ld_r8_hl
    {
        static constexpr step_t num_steps(const cpu::cpu_state&) { return 2; }

        static void execute(cpu::cpu_state& cpu, const step_t step)
        {
            if (step == 1)
            {
                reg_provider::get(cpu) = cpu.memory->read(cpu.reg.hl());
            }
        }
    };

    export using ld_a_hl = ld_r8_hl<a_register_provider>;
    export using ld_b_hl = ld_r8_hl<b_register_provider>;
    export using ld_c_hl = ld_r8_hl<c_register_provider>;
    export using ld_d_hl = ld_r8_hl<d_register_provider>;
    export using ld_e_hl = ld_r8_hl<e_register_provider>;
    export using ld_h_hl = ld_r8_hl<h_register_provider>;
    export using ld_l_hl = ld_r8_hl<l_register_provider>;

    export struct ld_a_hli
    {
        static constexpr step_t num_steps(const cpu::cpu_state&) { return 2; }

        static void execute(cpu::cpu_state& cpu, const step_t step)
        {
            if (step == 1)
            {
                cpu.reg.a() = cpu.memory->read(cpu.reg.hl());
                ++cpu.reg.hl();
            }
        }
    };

    export struct ld_a_hld
    {
        static constexpr step_t num_steps(const cpu::cpu_state&) { return 2; }

        static void execute(cpu::cpu_state& cpu, const step_t step)
        {
            if (step == 1)
            {
                cpu.reg.a() = cpu.memory->read(cpu.reg.hl());
                --cpu.reg.hl();
            }
        }
    };

    export struct ld_hli_a
    {
        static constexpr step_t num_steps(const cpu::cpu_state&) { return 2; }

        static void execute(cpu::cpu_state& cpu, const step_t step)
        {
            if (step == 1)
            {
                cpu.memory->write(cpu.reg.hl(), cpu.reg.a());
                ++cpu.reg.hl();
            }
        }
    };

    export struct ld_hld_a
    {
        static constexpr step_t num_steps(const cpu::cpu_state&) { return 2; }

        static void execute(cpu::cpu_state& cpu, const step_t step)
        {
            if (step == 1)
            {
                cpu.memory->write(cpu.reg.hl(), cpu.reg.a());
                --cpu.reg.hl();
            }
        }
    };

    // ld r16,a
    template <ReadOnlyR16RegisterProvider reg_provider>
    struct ld_r16_a
    {
        static constexpr step_t num_steps(const cpu::cpu_state&) { return 2; }

        static void execute(cpu::cpu_state& cpu, const step_t step)
        {
            if (step == 1)
            {
                cpu.memory->write(reg_provider::get(cpu), cpu.reg.a());
            }
        }
    };

    export using ld_bc_a = ld_r16_a<bc_readonly_register_provider>;
    export using ld_de_a = ld_r16_a<de_readonly_register_provider>;

    // ld n16,a
    export struct ld_n16_a
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
                    const std::uint8_t low_byte = cpu.cache.r8;
                    const std::uint8_t high_byte = cpu.memory->read(cpu.pc++);

                    cpu.cache.r16 = utils::encode_little_endian(low_byte, high_byte);
                }
                break;
            case 3:
                {
                    const std::uint16_t address = cpu.cache.r16;
                    cpu.memory->write(address, cpu.reg.a());
                }
                break;
            default: std::unreachable();
            }
        }
    };

    // ld a,n16
    export struct ld_a_n16
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
                    const std::uint8_t low_byte = cpu.cache.r8;
                    const std::uint8_t high_byte = cpu.memory->read(cpu.pc++);

                    cpu.cache.r16 = utils::encode_little_endian(low_byte, high_byte);
                }
                break;
            case 3:
                {
                    const std::uint16_t address = cpu.cache.r16;
                    cpu.reg.a() = cpu.memory->read(address);
                }
                break;
            default: std::unreachable();
            }
        }
    };

    // ld a,r16
    template <ReadOnlyR16RegisterProvider reg_provider>
    struct ld_a_r16
    {
        static constexpr step_t num_steps(const cpu::cpu_state&) { return 2; }

        static void execute(cpu::cpu_state& cpu, const step_t step)
        {
            if (step == 1)
            {
                cpu.reg.a() = cpu.memory->read(reg_provider::get(cpu));
            }
        }
    };

    export using ld_a_bc = ld_a_r16<bc_readonly_register_provider>;
    export using ld_a_de = ld_a_r16<de_readonly_register_provider>;

    // ldh n16,a
    export struct ldh_n16_a
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
                    constexpr std::uint8_t high_byte = 0xFF;

                    const std::uint8_t low_byte = cpu.cache.r8;
                    const std::uint16_t address = utils::encode_little_endian(low_byte, high_byte);

                    cpu.memory->write(address, cpu.reg.a());
                }
                break;
            default: std::unreachable();
            }
        }
    };

    // ldh n16,a
    export struct ldh_a_n16
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
                    constexpr std::uint8_t high_byte = 0xFF;

                    const std::uint8_t low_byte = cpu.cache.r8;
                    const std::uint16_t address = utils::encode_little_endian(low_byte, high_byte);

                    cpu.reg.a() = cpu.memory->read(address);
                }
                break;
            default: std::unreachable();
            }
        }
    };

    // ld n16,sp
    export struct ld_n16_sp
    {
        static constexpr step_t num_steps(const cpu::cpu_state&) { return 5; }

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

                    cpu.cache.r16 = utils::encode_little_endian(low_byte, high_byte);
                }
                break;
            case 3:
                cpu.memory->write(cpu.cache.r16, utils::less_significant_byte(cpu.sp.value()));
                break;
            case 4:
                cpu.memory->write(cpu.cache.r16 + 1, utils::most_significant_byte(cpu.sp.value()));
                break;
            default: std::unreachable();
            }
        }
    };

    // ld sp,hl
    export struct ld_sp_hl
    {
        static constexpr step_t num_steps(const cpu::cpu_state&) { return 2; }

        static void execute(cpu::cpu_state& cpu, const step_t step)
        {
            if (step == 1)
            {
                cpu.sp = cpu.reg.hl();
            }
        }
    };

    // ldh a,[c]
    export struct ldh_a_c
    {
        static constexpr step_t num_steps(const cpu::cpu_state&) { return 2; }

        static void execute(cpu::cpu_state& cpu, const step_t step)
        {
            switch (step)
            {
            case 0:
                {
                    using namespace literals;
                    cpu.cache.r16 = 0xFF00_u16 + cpu.reg.c();
                }
                break;
            case 1:
                {
                    const std::uint16_t memory_pos = cpu.cache.r16;
                    cpu.reg.a() = cpu.memory->read(memory_pos);
                }
                break;
            default: std::unreachable();
            }
        }
    };

    // ldh [c],a
    export struct ldh_c_a
    {
        static constexpr step_t num_steps(const cpu::cpu_state&) { return 2; }

        static void execute(cpu::cpu_state& cpu, const step_t step)
        {
            switch (step)
            {
            case 0:
                {
                    using namespace literals;
                    cpu.cache.r16 = 0xFF00_u16 + cpu.reg.c();
                }
                break;
            case 1:
                {
                    const std::uint16_t memory_pos = cpu.cache.r16;
                    cpu.memory->write(memory_pos, cpu.reg.a());
                }
                break;
            default: std::unreachable();
            }
        }
    };
}
