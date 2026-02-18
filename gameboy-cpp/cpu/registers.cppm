
export module cpu:registers;
import std;

namespace cpu
{
	template<typename T>
	requires std::same_as<T, std::uint8_t&> || std::same_as<T, const std::uint8_t&>
	class register_8_t
	{
	public:
		using type_t = std::uint8_t;

		register_8_t(T& v)
			: value_{ v }
		{}

		operator type_t() const { return value(); }

		type_t value() const { return value_; }

		register_8_t& operator=(const type_t v) 
		requires !std::is_const_v<std::remove_reference_t<T>>
		{
			value_ = v;
			return *this;
		}

		register_8_t& operator++()
		requires !std::is_const_v<std::remove_reference_t<T>>
		{
			*this = static_cast<type_t>(*this) + 1;
			return *this;
		}

		register_8_t& operator--()
		requires !std::is_const_v<std::remove_reference_t<T>>
		{
			*this = static_cast<type_t>(*this) - 1;
			return *this;
		}

	private:
		T value_;
	};

	export using register_8 = register_8_t<std::uint8_t&>;
	export using readonly_register_8 = register_8_t<const std::uint8_t&>;

	template<typename T>
	requires std::same_as<T, std::uint16_t&> || std::same_as<T, const std::uint16_t&>
	class register_16_t
	{
	private:
		using internal_type_t = std::conditional_t<std::is_const_v<std::remove_reference_t<T>>, const register_8::type_t&, register_8::type_t&>;

	public:
		using type_t = std::uint16_t;

		register_16_t(internal_type_t hi, internal_type_t lo)
			: lo_byte{ lo }
			, hi_byte{ hi }
		{}

		type_t value() const
		{
			return static_cast<type_t>(hi_byte) << 8 | static_cast<type_t>(lo_byte);
		}

		operator type_t() const
		{
			return value();
		}

		register_16_t& operator=(const type_t v)
		requires !std::is_const_v<std::remove_reference_t<T>>
		{
			hi_byte = static_cast<register_8::type_t>((v >> 8) & 0xFF);
			lo_byte = static_cast<register_8::type_t>(v & 0xFF);

			return *this;
		}

		register_16_t& operator++()
		requires !std::is_const_v<std::remove_reference_t<T>>
		{
			*this = static_cast<type_t>(*this) + 1;
			return *this;
		}

		register_16_t& operator--()
		requires !std::is_const_v<std::remove_reference_t<T>>
		{
			*this = static_cast<type_t>(*this) - 1;
			return *this;
		}

	private:
		internal_type_t lo_byte;
		internal_type_t hi_byte;
	};

	export using register_16 = register_16_t<std::uint16_t&>;
	export using readonly_register_16 = register_16_t<const std::uint16_t&>;

	template<typename T>
	requires std::same_as<T, std::uint8_t&> || std::same_as<T, const std::uint8_t&>
	class flag_register_t
	{
	public:
		using type_t = bool;

		flag_register_t(T byte, const std::size_t index)
			: byte_value{ byte }
			, byte_index{ index }
		{}

		type_t value() const 
		{
			return ((byte_value >> byte_index) & 0b1) == 0b1;
		}

		operator type_t() const 
		{
			return value();
		}

		flag_register_t& operator=(const type_t value)
		requires !std::is_const_v<std::remove_reference_t<T>>
		{
			byte_value = (byte_value & ~(1 << byte_index)) | value << byte_index;
			return *this;
		}

	private:
		T byte_value;
		const std::size_t byte_index;
	};

	export using flag_register = flag_register_t<std::uint8_t&>;
	export using readonly_flag_register = flag_register_t<const std::uint8_t&>;

	export struct flags
	{
		flag_register z;
		flag_register n;
		flag_register h;
		flag_register c;
	};

	export struct readonly_flags
	{
		readonly_flag_register z;
		readonly_flag_register n;
		readonly_flag_register h;
		readonly_flag_register c;
	};

	export class registers
	{
	public:
		register_8 a() { return { data[0] }; }
		register_8 b() { return { data[1] }; }
		register_8 c() { return { data[2] }; }
		register_8 d() { return { data[3] }; }
		register_8 e() { return { data[4] }; }
		register_8 f() { return { data[5] }; }
		register_8 h() { return { data[6] }; }
		register_8 l() { return { data[7] }; }

		readonly_register_8 a() const { return { data[0] }; }
		readonly_register_8 b() const { return { data[1] }; }
		readonly_register_8 c() const { return { data[2] }; }
		readonly_register_8 d() const { return { data[3] }; }
		readonly_register_8 e() const { return { data[4] }; }
		readonly_register_8 f() const { return { data[5] }; }
		readonly_register_8 h() const { return { data[6] }; }
		readonly_register_8 l() const { return { data[7] }; }

		register_16 af() { return { data[0], data[5]}; }
		register_16 bc() { return { data[1], data[2]}; }
		register_16 de() { return { data[3], data[4]}; }
		register_16 hl() { return { data[6], data[7]}; }

		readonly_register_16 af() const { return { data[0], data[5]}; }
		readonly_register_16 bc() const { return { data[1], data[2]}; }
		readonly_register_16 de() const { return { data[3], data[4]}; }
		readonly_register_16 hl() const { return { data[6], data[7]}; }

		flag_register z_flag() { return { data[5], 7 }; }
		flag_register n_flag() { return { data[5], 6 }; }
		flag_register h_flag() { return { data[5], 5 }; }
		flag_register c_flag() { return { data[5], 4 }; }

		readonly_flag_register z_flag() const { return { data[5], 7 }; }
		readonly_flag_register n_flag() const { return { data[5], 6 }; }
		readonly_flag_register h_flag() const { return { data[5], 5 }; }
		readonly_flag_register c_flag() const { return { data[5], 4 }; }

		flags flags() { return { z_flag(), n_flag(), h_flag(), c_flag() }; }
		readonly_flags flags() const { return { z_flag(), n_flag(), h_flag(), c_flag() }; }

	private:
		std::array<std::uint8_t, 8> data {};
	};
}
