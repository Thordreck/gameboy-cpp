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
	concept InterruptDescriptor = requires(const T& descriptor)
	{
		{ descriptor.ie_flag() } -> std::convertible_to<std::uint8_t>;
		{ descriptor.if_flag() } -> std::convertible_to<std::uint8_t>;
		{ descriptor.handler_address() } -> std::convertible_to<std::uint16_t>;
	};

	export template<std::uint8_t T>
	concept InterruptFlag = T == 1 << 0 || T == 1 << 1 || T == 1 << 2 || T == 1 << 3 || T == 1 << 4;

	export template<std::uint16_t T>
	concept InterruptAddress = T == 0x40 || T == 0x48 || T == 0x50 || T == 0x58 || T == 0x60;

	export class interrupt
	{
	public:
		template<std::uint8_t ie_flag, std::uint8_t if_flag, std::uint16_t handler_address>
		requires InterruptFlag<ie_flag> && InterruptFlag<if_flag> && InterruptAddress<handler_address>
		static constexpr interrupt create()
		{
			return { ie_flag, if_flag, handler_address };
		}

		[[nodiscard]] std::uint8_t ie_flag() const { return ie_flag_; }
		[[nodiscard]] std::uint8_t if_flag() const { return if_flag_; }
		[[nodiscard]] std::uint16_t handler_address() const { return handler_address_; }

	private:
		constexpr interrupt(const std::uint8_t ie_flag, const std::uint8_t if_flag, const std::uint16_t handler_address)
			: ie_flag_ (ie_flag)
			, if_flag_(if_flag)
			, handler_address_(handler_address)
			{}

		std::uint8_t ie_flag_;
		std::uint8_t if_flag_;
		std::uint16_t handler_address_;
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

	export constexpr auto vblank_interrupt = interrupt::create<vblank_ie_flag, vblank_if_flag, vblank_handler_address>();
	export constexpr auto lcd_interrupt = interrupt::create<lcd_ie_flag, lcd_if_flag, lcd_handler_address>();
	export constexpr auto timer_interrupt = interrupt::create<timer_ie_flag, timer_if_flag, timer_handler_address>();
	export constexpr auto serial_interrupt = interrupt::create<serial_ie_flag, serial_if_flag, serial_handler_address>();
	export constexpr auto joypad_interrupt = interrupt::create<joypad_ie_flag, joypad_if_flag, joypad_handler_address>();

	export struct interrupt_registers
	{
		memory::memory_data_t enable {};
		memory::memory_data_t flag {};
	};
}
