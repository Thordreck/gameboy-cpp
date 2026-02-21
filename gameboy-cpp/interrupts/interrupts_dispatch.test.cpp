
#include "doctest.h"

import cpu;
import std;
import interrupts;

namespace
{
	using namespace interrupts;

	template<InterruptDescriptor interrupt, std::uint16_t handler_address, std::uint8_t if_flag>
	struct dispatch_test_case
	{
		using interrupt_t = interrupt;
		static constexpr auto handler = handler_address;
		static constexpr auto if_flag = if_flag;
	};

	#define dispatch_test_cases \
		dispatch_test_case<vblank_interrupt, 0x40, 0b00000001>, \
		dispatch_test_case<lcd_interrupt, 0x48, 0b00000010>, \
		dispatch_test_case<timer_interrupt, 0x50, 0b00000100>, \
		dispatch_test_case<serial_interrupt, 0x58, 0b00001000>, \
		dispatch_test_case<joypad_interrupt, 0x60, 0b00010000>
}

TEST_CASE_TEMPLATE("interrupts.IME flag is disabled when interrupt is dispatched", test, dispatch_test_cases)
{
	std::array<cpu::memory_bus::type_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };

	cpu.sp() = 0xFFFE;
	cpu.ime_flag().enable();
	interrupts::dispatch<typename test::interrupt_t>(cpu);

	CHECK_FALSE(cpu.ime_flag().is_enabled());
	CHECK_FALSE(cpu.ime_flag().is_requested());
}

TEST_CASE_TEMPLATE("interrupts.IF flag is unset when interrupt is dispatched", test, dispatch_test_cases)
{
	constexpr std::uint16_t if_address = 0xFF0F;

	std::array<cpu::memory_bus::type_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };
	cpu.sp() = 0xFFFE;

	memory[if_address] |= test::if_flag;
	interrupts::dispatch<typename test::interrupt_t>(cpu);

	CHECK_EQ(memory[if_address] & test::if_flag, 0x0);
}

TEST_CASE_TEMPLATE("interrupts.PC is set to handler when interrupt is dispatched", test, dispatch_test_cases)
{
	std::array<cpu::memory_bus::type_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };
	cpu.sp() = 0xFFFE;

	interrupts::dispatch<typename test::interrupt_t>(cpu);
	CHECK_EQ(cpu.pc(), test::handler);
}

TEST_CASE_TEMPLATE("interrupts.Previous pc is push to stack when interrupt is dispatched", test, dispatch_test_cases)
{
	constexpr std::uint16_t stack_origin = 0xFFFE;

	std::array<cpu::memory_bus::type_t, cpu::memory_bus::size> memory{};
	cpu::cpu cpu{ memory };
	cpu.sp() = stack_origin;
	cpu.pc() = 0xABCD;

	interrupts::dispatch<typename test::interrupt_t>(cpu);

	CHECK_EQ(memory[stack_origin - 1], 0xAB);
	CHECK_EQ(memory[stack_origin - 2], 0xCD);
	CHECK_EQ(cpu.sp(), stack_origin - 2);
}
