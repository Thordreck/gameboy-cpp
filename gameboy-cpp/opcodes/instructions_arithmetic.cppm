export module opcodes:arithmetic;

export import std;
export import cpu;
export import utilities;
export import :common;
import memory;

namespace opcodes
{
    export struct add_a_n8
    {
        static constexpr step_t num_steps(const cpu::cpu_state&) { return 2; }

        template<memory::ReadOnlyMemory Memory>
        static void execute(cpu::cpu_state& cpu, const step_t step, const Memory& memory)
        {
            switch (step)
            {
            case 0:
                cpu.cache.r8 = memory.read(cpu.pc++);
                break;
            case 1:
                {
                    const std::uint8_t a = cpu.reg.a();
                    const std::uint8_t n8 = cpu.cache.r8;
                    const std::uint8_t result = a + n8;

                    const bool carry = utils::check_add_overflow(a, n8);
                    const bool half_carry = utils::check_half_add_overflow(a, n8);

                    cpu.reg.flags().z = result == 0;
                    cpu.reg.flags().n = false;
                    cpu.reg.flags().h = half_carry;
                    cpu.reg.flags().c = carry;

                    cpu.reg.a() = result;
                }
                break;
            default: std::unreachable();
            }
        }
    };

    // add_a_r8
    template <ReadOnlyR8RegisterProvider reg_provider>
    struct add_a_r8
    {
        static constexpr step_t num_steps(const cpu::cpu_state&) { return 1; }

        static void execute(cpu::cpu_state& cpu, const step_t)
        {
            const cpu::readonly_register_8 r8 = reg_provider::get(cpu);
            const bool carry = utils::check_add_overflow(cpu.reg.a().value(), r8.value());
            const bool half_carry = utils::check_half_add_overflow(cpu.reg.a().value(), r8.value());

            cpu.reg.a() = cpu.reg.a() + r8;
            cpu.reg.flags().z = cpu.reg.a() == 0;
            cpu.reg.flags().n = false;
            cpu.reg.flags().h = half_carry;
            cpu.reg.flags().c = carry;
        }
    };

    export using add_a_a = add_a_r8<a_readonly_register_provider>;
    export using add_a_b = add_a_r8<b_readonly_register_provider>;
    export using add_a_c = add_a_r8<c_readonly_register_provider>;
    export using add_a_d = add_a_r8<d_readonly_register_provider>;
    export using add_a_e = add_a_r8<e_readonly_register_provider>;
    export using add_a_h = add_a_r8<h_readonly_register_provider>;
    export using add_a_l = add_a_r8<l_readonly_register_provider>;

    // inc_r8
    template <R8RegisterProvider reg_provider>
    struct inc_r8
    {
        static constexpr step_t num_steps(const cpu::cpu_state&) { return 1; }

        static void execute(cpu::cpu_state& cpu, const step_t)
        {
            using namespace literals;

            cpu::register_8 r8 = reg_provider::get(cpu);
            const bool half_carry = utils::check_half_add_overflow(r8.value(), 1_u8);

            ++r8;
            cpu.reg.flags().z = r8 == 0;
            cpu.reg.flags().n = false;
            cpu.reg.flags().h = half_carry;
        }
    };

    export using inc_a = inc_r8<a_register_provider>;
    export using inc_b = inc_r8<b_register_provider>;
    export using inc_c = inc_r8<c_register_provider>;
    export using inc_d = inc_r8<d_register_provider>;
    export using inc_e = inc_r8<e_register_provider>;
    export using inc_h = inc_r8<h_register_provider>;
    export using inc_l = inc_r8<l_register_provider>;

    template <R8RegisterProvider reg_provider>
    struct dec_r8
    {
        static constexpr step_t num_steps(const cpu::cpu_state&) { return 1; }

        static void execute(cpu::cpu_state& cpu, const step_t)
        {
            using namespace literals;

            cpu::register_8 r8 = reg_provider::get(cpu);
            const bool half_underflow = utils::check_half_substract_underflow(r8.value(), 1_u8);

            --r8;
            cpu.reg.flags().z = r8 == 0;
            cpu.reg.flags().n = true;
            cpu.reg.flags().h = half_underflow;
        }
    };

    export using dec_a = dec_r8<a_register_provider>;
    export using dec_b = dec_r8<b_register_provider>;
    export using dec_c = dec_r8<c_register_provider>;
    export using dec_d = dec_r8<d_register_provider>;
    export using dec_e = dec_r8<e_register_provider>;
    export using dec_h = dec_r8<h_register_provider>;
    export using dec_l = dec_r8<l_register_provider>;

    export struct dec_ind_hl
    {
        static constexpr step_t num_steps(const cpu::cpu_state&) { return 3; }

        template<memory::Memory Memory>
        static void execute(cpu::cpu_state& cpu, const step_t step, Memory& memory)
        {
            switch (step)
            {
            case 0:
                break;
            case 1:
                cpu.cache.r8 = memory.read(cpu.reg.hl());
                break;
            case 2:
                {
                    using namespace literals;

                    const std::uint8_t n8 = cpu.cache.r8;
                    const std::uint8_t result = n8 - 1;
                    const bool half_underflow = utils::check_half_substract_underflow(n8, 1_u8);

                    cpu.reg.flags().z = result == 0;
                    cpu.reg.flags().n = true;
                    cpu.reg.flags().h = half_underflow;

                    memory.write(cpu.reg.hl(), result);
                }
                break;
            default: std::unreachable();
            }
        }
    };

    // inc_r16
    template <R16RegisterProvider reg_provider>
    struct inc_r16
    {
        static constexpr step_t num_steps(const cpu::cpu_state&) { return 2; }

        static void execute(cpu::cpu_state& cpu, const step_t step)
        {
            if (step == 1)
            {
                ++reg_provider::get(cpu);
            }
        }
    };

    export using inc_bc = inc_r16<bc_register_provider>;
    export using inc_de = inc_r16<de_register_provider>;
    export using inc_hl = inc_r16<hl_register_provider>;

    export struct inc_sp
    {
        static constexpr step_t num_steps(const cpu::cpu_state&) { return 2; }

        static void execute(cpu::cpu_state& cpu, const step_t step)
        {
            if (step == 1)
            {
                ++cpu.sp;
            }
        }
    };

    export struct dec_sp
    {
        static constexpr step_t num_steps(const cpu::cpu_state&) { return 2; }

        static void execute(cpu::cpu_state& cpu, const step_t step)
        {
            if (step == 1)
            {
                --cpu.sp;
            }
        }
    };

    template <ReadOnlyR8RegisterProvider reg_provider>
    struct cp_a_r8
    {
        static constexpr step_t num_steps(const cpu::cpu_state&) { return 1; }

        static void execute(cpu::cpu_state& cpu, const step_t)
        {
            const auto a = cpu.reg.a();
            const auto r8 = reg_provider::get(cpu);

            const bool underflow = utils::check_substract_underflow(a.value(), r8.value());
            const bool half_underflow = utils::check_half_substract_underflow(a.value(), r8.value());

            cpu.reg.flags().z = (a - r8) == 0;
            cpu.reg.flags().n = true;
            cpu.reg.flags().c = underflow;
            cpu.reg.flags().h = half_underflow;
        }
    };

    export using cp_a_a = cp_a_r8<a_readonly_register_provider>;
    export using cp_a_b = cp_a_r8<b_readonly_register_provider>;
    export using cp_a_c = cp_a_r8<c_readonly_register_provider>;
    export using cp_a_d = cp_a_r8<d_readonly_register_provider>;
    export using cp_a_e = cp_a_r8<e_readonly_register_provider>;
    export using cp_a_h = cp_a_r8<h_readonly_register_provider>;
    export using cp_a_l = cp_a_r8<l_readonly_register_provider>;

    export struct cp_a_n8
    {
        static constexpr step_t num_steps(const cpu::cpu_state&) { return 2; }

        template<memory::ReadOnlyMemory Memory>
        static void execute(cpu::cpu_state& cpu, const step_t step, const Memory& memory)
        {
            switch (step)
            {
            case 0:
                cpu.cache.r8 = memory.read(cpu.pc++);
                break;
            case 1:
                {
                    const std::uint8_t a = cpu.reg.a();
                    const std::uint8_t n8 = cpu.cache.r8;

                    const bool underflow = utils::check_substract_underflow(a, n8);
                    const bool half_underflow = utils::check_half_substract_underflow(a, n8);

                    cpu.reg.flags().z = (a - n8) == 0;
                    cpu.reg.flags().n = true;
                    cpu.reg.flags().c = underflow;
                    cpu.reg.flags().h = half_underflow;
                }
                break;
            default: std::unreachable();
            }
        }
    };

    template <ReadOnlyR8RegisterProvider reg_provider>
    struct sub_a_r8
    {
        static constexpr step_t num_steps(const cpu::cpu_state&) { return 1; }

        static void execute(cpu::cpu_state& cpu, const step_t)
        {
            const std::uint8_t r8 = reg_provider::get(cpu);
            const bool underflow = utils::check_substract_underflow(cpu.reg.a().value(), r8);
            const bool half_underflow = utils::check_half_substract_underflow(cpu.reg.a().value(), r8);

            cpu.reg.a() = cpu.reg.a() - r8;
            cpu.reg.flags().z = cpu.reg.a() == 0;
            cpu.reg.flags().n = true;
            cpu.reg.flags().h = half_underflow;
            cpu.reg.flags().c = underflow;
        }
    };

    export using sub_a_a = sub_a_r8<a_readonly_register_provider>;
    export using sub_a_b = sub_a_r8<b_readonly_register_provider>;
    export using sub_a_c = sub_a_r8<c_readonly_register_provider>;
    export using sub_a_d = sub_a_r8<d_readonly_register_provider>;
    export using sub_a_e = sub_a_r8<e_readonly_register_provider>;
    export using sub_a_h = sub_a_r8<h_readonly_register_provider>;
    export using sub_a_l = sub_a_r8<l_readonly_register_provider>;

    export struct sub_a_n8
    {
        static constexpr step_t num_steps(const cpu::cpu_state&) { return 2; }

        template<memory::ReadOnlyMemory Memory>
        static void execute(cpu::cpu_state& cpu, const step_t step, const Memory& memory)
        {
            switch (step)
            {
            case 0:
                cpu.cache.r8 = memory.read(cpu.pc++);
                break;
            case 1:
                {
                    const std::uint8_t n8 = cpu.cache.r8;
                    const bool underflow = utils::check_substract_underflow(cpu.reg.a().value(), n8);
                    const bool half_underflow = utils::check_half_substract_underflow(cpu.reg.a().value(), n8);

                    cpu.reg.a() = cpu.reg.a() - n8;
                    cpu.reg.flags().z = cpu.reg.a() == 0;
                    cpu.reg.flags().n = true;
                    cpu.reg.flags().h = half_underflow;
                    cpu.reg.flags().c = underflow;
                }
                break;
            default: std::unreachable();
            }
        }
    };

    template <ReadOnlyR8RegisterProvider reg_provider>
    struct adc_a_r8
    {
        static constexpr step_t num_steps(const cpu::cpu_state&) { return 1; }

        static void execute(cpu::cpu_state& cpu, const step_t)
        {
            const std::uint8_t a = cpu.reg.a();
            const std::uint8_t r8 = reg_provider::get(cpu);
            const std::uint8_t carry = cpu.reg.c_flag() ? 1 : 0;

            const std::uint16_t result
                = static_cast<std::uint16_t>(a)
                + static_cast<std::uint16_t>(r8)
                + carry;

            cpu.reg.a() = static_cast<std::uint8_t>(result);
            cpu.reg.flags().z = cpu.reg.a() == 0;
            cpu.reg.flags().n = false;
            cpu.reg.flags().h = ((a & 0xF) + (r8 & 0xF) + carry) > 0xF;
            cpu.reg.flags().c = (result > 0xFF);
        }
    };

    export using adc_a_a = adc_a_r8<a_readonly_register_provider>;
    export using adc_a_b = adc_a_r8<b_readonly_register_provider>;
    export using adc_a_c = adc_a_r8<c_readonly_register_provider>;
    export using adc_a_d = adc_a_r8<d_readonly_register_provider>;
    export using adc_a_e = adc_a_r8<e_readonly_register_provider>;
    export using adc_a_h = adc_a_r8<h_readonly_register_provider>;
    export using adc_a_l = adc_a_r8<l_readonly_register_provider>;

    export struct adc_a_n8
    {
        static constexpr step_t num_steps(const cpu::cpu_state&) { return 2; }

        template<memory::ReadOnlyMemory Memory>
        static void execute(cpu::cpu_state& cpu, const step_t step, const Memory& memory)
        {
            switch (step)
            {
            case 0:
                cpu.cache.r8 = memory.read(cpu.pc++);
                break;
            case 1:
                {
                    const std::uint8_t a = cpu.reg.a();
                    const std::uint8_t n8 = cpu.cache.r8;
                    const std::uint8_t carry = cpu.reg.c_flag() ? 1 : 0;

                    const std::uint16_t result
                        = static_cast<std::uint16_t>(a)
                        + static_cast<std::uint16_t>(n8)
                        + carry;

                    cpu.reg.a() = static_cast<std::uint8_t>(result);
                    cpu.reg.flags().z = cpu.reg.a() == 0;
                    cpu.reg.flags().n = false;
                    cpu.reg.flags().h = ((a & 0xF) + (n8 & 0xF) + carry) > 0xF;
                    cpu.reg.flags().c = (result > 0xFF);
                }
                break;
            default: std::unreachable();
            }
        }
    };

    template <ReadOnlyR16RegisterProvider reg_provider>
    struct add_hl_r16
    {
        static constexpr step_t num_steps(const cpu::cpu_state&) { return 2; }

        static void execute(cpu::cpu_state& cpu, const step_t step)
        {
            if (step == 1)
            {
                const cpu::register_16::type_t hl = cpu.reg.hl();
                const cpu::register_16::type_t r16 = reg_provider::get(cpu);

                const bool overflow = utils::check_add_overflow(hl, r16);
                const bool half_overflow = utils::check_half_add_overflow(hl, r16);

                cpu.reg.hl() = hl + r16;
                cpu.reg.flags().n = false;
                cpu.reg.flags().h = half_overflow;
                cpu.reg.flags().c = overflow;
            }
        }
    };

    export using add_hl_bc = add_hl_r16<bc_readonly_register_provider>;
    export using add_hl_de = add_hl_r16<de_readonly_register_provider>;
    export using add_hl_hl = add_hl_r16<hl_readonly_register_provider>;

    export struct add_hl_sp
    {
        static constexpr step_t num_steps(const cpu::cpu_state&) { return 2; }

        static void execute(cpu::cpu_state& cpu, const step_t step)
        {
            if (step == 1)
            {
                const std::uint16_t hl = cpu.reg.hl();
                const std::uint16_t sp = cpu.sp;

                const bool overflow = utils::check_add_overflow(hl, sp);
                const bool half_overflow = utils::check_half_add_overflow(hl, sp);

                cpu.reg.hl() = hl + sp;
                cpu.reg.flags().n = false;
                cpu.reg.flags().h = half_overflow;
                cpu.reg.flags().c = overflow;
            }
        }
    };

    template <ReadOnlyR8RegisterProvider reg_provider>
    struct sbc_a_r8
    {
        static constexpr step_t num_steps(const cpu::cpu_state&) { return 1; }

        static void execute(cpu::cpu_state& cpu, const step_t)
        {
            const std::uint8_t a = cpu.reg.a();
            const std::uint8_t r8 = reg_provider::get(cpu);
            const std::uint8_t carry = cpu.reg.c_flag() ? 1 : 0;

            const std::uint16_t result
                = static_cast<std::uint16_t>(a)
                - static_cast<std::uint16_t>(r8)
                - static_cast<std::uint16_t>(carry);

            cpu.reg.a() = static_cast<std::uint8_t>(result);
            cpu.reg.flags().z = cpu.reg.a() == 0;
            cpu.reg.flags().n = true;
            cpu.reg.flags().h = ((a & 0xF) < ((r8 & 0xF) + carry));
            cpu.reg.flags().c = (result > 0xFF);
        }
    };

    export using sbc_a_a = sbc_a_r8<a_readonly_register_provider>;
    export using sbc_a_b = sbc_a_r8<b_readonly_register_provider>;
    export using sbc_a_c = sbc_a_r8<c_readonly_register_provider>;
    export using sbc_a_d = sbc_a_r8<d_readonly_register_provider>;
    export using sbc_a_e = sbc_a_r8<e_readonly_register_provider>;
    export using sbc_a_h = sbc_a_r8<h_readonly_register_provider>;
    export using sbc_a_l = sbc_a_r8<l_readonly_register_provider>;

    export struct sbc_a_n8
    {
        static constexpr step_t num_steps(const cpu::cpu_state&) { return 2; }

        template<memory::ReadOnlyMemory Memory>
        static void execute(cpu::cpu_state& cpu, const step_t step, const Memory& memory)
        {
            switch (step)
            {
            case 0:
                cpu.cache.r8 = memory.read(cpu.pc++);
                break;
            case 1:
                {
                    const std::uint8_t r8 = cpu.cache.r8;
                    const std::uint8_t a = cpu.reg.a();
                    const std::uint8_t carry = cpu.reg.c_flag() ? 1 : 0;

                    const std::uint16_t result
                        = static_cast<std::uint16_t>(a)
                        - static_cast<std::uint16_t>(r8)
                        - static_cast<std::uint16_t>(carry);

                    cpu.reg.a() = static_cast<std::uint8_t>(result);
                    cpu.reg.flags().z = cpu.reg.a() == 0;
                    cpu.reg.flags().n = true;
                    cpu.reg.flags().h = ((a & 0xF) < ((r8 & 0xF) + carry));
                    cpu.reg.flags().c = (result > 0xFF);
                }
                break;
            default: std::unreachable();
            }
        }
    };

    template <R16RegisterProvider reg_provider>
    struct dec_r16
    {
        static constexpr step_t num_steps(const cpu::cpu_state&) { return 2; }

        static void execute(cpu::cpu_state& cpu, const step_t step)
        {
            if (step == 1)
            {
                --reg_provider::get(cpu);
            }
        }
    };

    export using dec_bc = dec_r16<bc_register_provider>;
    export using dec_de = dec_r16<de_register_provider>;
    export using dec_hl = dec_r16<hl_register_provider>;

    export struct cp_a_hl
    {
        static constexpr step_t num_steps(const cpu::cpu_state&) { return 2; }

        template<memory::ReadOnlyMemory Memory>
        static void execute(cpu::cpu_state& cpu, const step_t step, const Memory& memory)
        {
            if (step == 1)
            {
                const std::uint8_t a = cpu.reg.a();
                const std::uint8_t n8 = memory.read(cpu.reg.hl());

                const bool underflow = utils::check_substract_underflow(a, n8);
                const bool half_underflow = utils::check_half_substract_underflow(a, n8);

                cpu.reg.flags().z = (a - n8) == 0;
                cpu.reg.flags().n = true;
                cpu.reg.flags().c = underflow;
                cpu.reg.flags().h = half_underflow;
            }
        }
    };

    export struct add_a_hl
    {
        static constexpr step_t num_steps(const cpu::cpu_state&) { return 2; }

        template<memory::ReadOnlyMemory Memory>
        static void execute(cpu::cpu_state& cpu, const step_t step, const Memory& memory)
        {
            if (step == 1)
            {
                const std::uint8_t a = cpu.reg.a();
                const std::uint8_t n8 = memory.read(cpu.reg.hl());

                const bool carry = utils::check_add_overflow(a, n8);
                const bool half_carry = utils::check_half_add_overflow(a, n8);

                cpu.reg.a() = a + n8;
                cpu.reg.flags().z = cpu.reg.a() == 0;
                cpu.reg.flags().n = false;
                cpu.reg.flags().h = half_carry;
                cpu.reg.flags().c = carry;
            }
        }
    };

    export struct adc_a_hl
    {
        static constexpr step_t num_steps(const cpu::cpu_state&) { return 2; }

        template<memory::ReadOnlyMemory Memory>
        static void execute(cpu::cpu_state& cpu, const step_t step, const Memory& memory)
        {
            if (step == 1)
            {
                const std::uint8_t a = cpu.reg.a();
                const std::uint8_t n8 = memory.read(cpu.reg.hl());
                const std::uint8_t carry = cpu.reg.c_flag() ? 1 : 0;

                const std::uint16_t result
                    = static_cast<std::uint16_t>(a)
                    + static_cast<std::uint16_t>(n8)
                    + carry;

                cpu.reg.a() = static_cast<std::uint8_t>(result);
                cpu.reg.flags().z = cpu.reg.a() == 0;
                cpu.reg.flags().n = false;
                cpu.reg.flags().h = ((a & 0xF) + (n8 & 0xF) + carry) > 0xF;
                cpu.reg.flags().c = (result > 0xFF);
            }
        }
    };

    export struct sub_a_hl
    {
        static constexpr step_t num_steps(const cpu::cpu_state&) { return 2; }

        template<memory::ReadOnlyMemory Memory>
        static void execute(cpu::cpu_state& cpu, const step_t step, const Memory& memory)
        {
            if (step == 1)
            {
                const std::uint8_t a = cpu.reg.a();
                const std::uint8_t n8 = memory.read(cpu.reg.hl());

                const bool underflow = utils::check_substract_underflow(a, n8);
                const bool half_underflow = utils::check_half_substract_underflow(a, n8);

                cpu.reg.a() = a - n8;
                cpu.reg.flags().z = cpu.reg.a() == 0;
                cpu.reg.flags().n = true;
                cpu.reg.flags().h = half_underflow;
                cpu.reg.flags().c = underflow;
            }
        }
    };

    export struct sbc_a_hl
    {
        static constexpr step_t num_steps(const cpu::cpu_state&) { return 2; }

        template<memory::ReadOnlyMemory Memory>
        static void execute(cpu::cpu_state& cpu, const step_t step, const Memory& memory)
        {
            if (step == 1)
            {
                const std::uint8_t a = cpu.reg.a();
                const std::uint8_t n8 = memory.read(cpu.reg.hl());
                const std::uint8_t carry = cpu.reg.c_flag() ? 1 : 0;

                const std::uint16_t result
                    = static_cast<std::uint16_t>(a)
                    - static_cast<std::uint16_t>(n8)
                    - static_cast<std::uint16_t>(carry);

                cpu.reg.a() = static_cast<std::uint8_t>(result);
                cpu.reg.flags().z = cpu.reg.a() == 0;
                cpu.reg.flags().n = true;
                cpu.reg.flags().h = ((a & 0xF) < ((n8 & 0xF) + carry));
                cpu.reg.flags().c = (result > 0xFF);
            }
        }
    };

    export struct inc_indirect_hl
    {
        static constexpr step_t num_steps(const cpu::cpu_state&) { return 3; }

        template<memory::Memory Memory>
        static void execute(cpu::cpu_state& cpu, const step_t step, Memory& memory)
        {
            switch (step)
            {
            case 0:
                break;
            case 1:
                cpu.cache.r8 = memory.read(cpu.reg.hl());
                break;
            case 2:
                {
                    using namespace literals;

                    const std::uint8_t n8 = cpu.cache.r8;
                    const std::uint8_t result = n8 + 1;
                    const bool half_carry = utils::check_half_add_overflow(n8, 1_u8);

                    cpu.reg.flags().z = result == 0;
                    cpu.reg.flags().n = false;
                    cpu.reg.flags().h = half_carry;

                    memory.write(cpu.reg.hl(), result);
                }
                break;
            default: std::unreachable();
            }
        }
    };
}
