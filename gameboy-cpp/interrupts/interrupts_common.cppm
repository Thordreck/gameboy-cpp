export module interrupts:common;

export import std;
export import memory;
export import cpu;

namespace interrupts
{
	export constexpr std::uint16_t ie_address = 0xFFFF;
	export constexpr std::uint16_t if_address = 0xFF0F;

	export constexpr std::uint8_t if_mask = 0x1F;
	export constexpr std::uint8_t ie_mask = 0x1F;

	export template<typename T>
	concept InterruptDescriptor = requires()
	{
		{ T::ie_flag } -> std::convertible_to<std::uint8_t>;
		{ T::if_flag } -> std::convertible_to<std::uint8_t>;
		{ T::handler_address } -> std::convertible_to<std::uint16_t>;
	};

	export template<std::uint8_t T>
	concept InterruptFlag = T == 1 << 0 || T == 1 << 1 || T == 1 << 2 || T == 1 << 3 || T == 1 << 4;

	export template<std::uint16_t T>
	concept InterruptAddress = T == 0x40 || T == 0x48 || T == 0x50 || T == 0x58 || T == 0x60;

	export template<std::uint8_t ie_flag, std::uint8_t if_flag, std::uint16_t handler_address>
	requires InterruptFlag<ie_flag> && InterruptFlag<if_flag> && InterruptAddress<handler_address>
	struct interrupt
	{
		static constexpr auto ie_flag = ie_flag;
		static constexpr auto if_flag = if_flag;
		static constexpr auto handler_address = handler_address;
	};

	export constexpr std::uint8_t vblank_ie_flag = 1 << 0;
	export constexpr std::uint8_t lcd_ie_flag = 1 << 1;
	export constexpr std::uint8_t timer_ie_flag = 1 << 2;
	export constexpr std::uint8_t serial_ie_flag = 1 << 3;
	export constexpr std::uint8_t joypad_ie_flag = 1 << 4;

	export constexpr std::uint8_t vblank_if_flag = 1 << 0;
	export constexpr std::uint8_t lcd_if_flag = 1 << 1;
	export constexpr std::uint8_t timer_if_flag = 1 << 2;
	export constexpr std::uint8_t serial_if_flag = 1 << 3;
	export constexpr std::uint8_t joypad_if_flag = 1 << 4;

	export constexpr std::uint16_t vblank_handler_address = 0x40;
	export constexpr std::uint16_t lcd_handler_address = 0x48;
	export constexpr std::uint16_t timer_handler_address = 0x50;
	export constexpr std::uint16_t serial_handler_address = 0x58;
	export constexpr std::uint16_t joypad_handler_address = 0x60;

	export using vblank_interrupt = interrupt<vblank_ie_flag, vblank_if_flag, vblank_handler_address>;
	export using lcd_interrupt = interrupt<lcd_ie_flag, lcd_if_flag, lcd_handler_address>;
	export using timer_interrupt = interrupt<timer_ie_flag, timer_if_flag, timer_handler_address>;
	export using serial_interrupt = interrupt<serial_ie_flag, serial_if_flag, serial_handler_address>;
	export using joypad_interrupt = interrupt<joypad_ie_flag, joypad_if_flag, joypad_handler_address>;

	export template<typename T>
	concept InterruptDispatcher = requires(cpu::cpu_state & cpu)
	{
		{ T::num_steps() } -> std::convertible_to<std::uint8_t>;
		{ T::execute(cpu) } -> std::same_as<void>;
	};

	export using interrupt_dispatcher_execute_fn_t = void(*)(cpu::cpu_state&, std::uint8_t);
	export using interrupt_dispatcher_steps_fn_t = std::uint8_t(*)();

	export struct interrupt_dispatcher
	{
		interrupt_dispatcher_execute_fn_t execute;
		interrupt_dispatcher_steps_fn_t num_steps;
	};

	export struct interrupt_registers
	{
		memory::memory_data_t enable;
		memory::memory_data_t flag;
	};
}
