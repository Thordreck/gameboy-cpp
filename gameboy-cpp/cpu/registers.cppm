
export module cpu:registers;
export import std;

namespace cpu
{
	export class register_8
	{
	public:
		register_8(std::uint8_t& v)
			: value{ v }
		{}

		operator std::uint8_t() const { return value; }

		register_8& operator=(const std::uint8_t v)
		{
			value = v;
			return *this;
		}

	private:
		std::uint8_t& value;
	};

	export class register_16
	{
	public:
		register_16(std::uint8_t& hi, std::uint8_t& lo)
			: lo_byte{ lo }
			, hi_byte{ hi }
		{}

		operator std::uint16_t() const
		{
			return static_cast<std::uint16_t>(hi_byte << 8)
				| static_cast<std::uint16_t>(lo_byte);
		}

		register_16& operator=(const std::uint16_t v)
		{
			hi_byte = static_cast<std::uint8_t>((v >> 8) & 0xFF);
			lo_byte = static_cast<std::uint8_t>(v & 0xFF);

			return *this;
		}

	private:
		std::uint8_t& lo_byte;
		std::uint8_t& hi_byte;
	};

	export class flag_register
	{
	public:
		flag_register(std::uint8_t& byte, const std::size_t index)
			: byte_value{ byte }
			, byte_index{ index }
		{}

		operator bool() const 
		{
			return ((byte_value >> byte_index) & 0b1) == 0b1;
		}

		flag_register& operator=(const bool value)
		{
			byte_value |= value << byte_index;
			return *this;
		}

	private:
		std::uint8_t& byte_value;
		const std::size_t byte_index;
	};

	export class registers
	{
	public:
		~registers() = default;

		register_8 a() { return { data[0] }; }
		register_8 b() { return { data[1] }; }
		register_8 c() { return { data[2] }; }
		register_8 d() { return { data[3] }; }
		register_8 e() { return { data[4] }; }
		register_8 f() { return { data[5] }; }
		register_8 h() { return { data[6] }; }
		register_8 l() { return { data[7] }; }

		register_16 af() { return { data[0], data[5]}; }
		register_16 bc() { return { data[1], data[2]}; }
		register_16 de() { return { data[3], data[4]}; }
		register_16 hl() { return { data[6], data[7]}; }

		flag_register z_flag() { return { data[5], 7 }; }
		flag_register n_flag() { return { data[5], 6 }; }
		flag_register h_flag() { return { data[5], 5 }; }
		flag_register c_flag() { return { data[5], 4 }; }

	private:
		std::array<std::uint8_t, 8> data {};
	};
}
