export module cpu:cycle;

import std;

namespace cpu
{
	export class machine_cycle
	{
	public:
		using type_t = std::uint8_t;

	public:
		machine_cycle(type_t value)
			: value_{ value }
		{}

		machine_cycle() = default;

		operator type_t() const { return value(); }
		type_t value() const { return value_; }

		machine_cycle& operator=(const machine_cycle other)
		{
			value_ = other;
			return *this;
		}

		machine_cycle& operator++()
		{
			value_++;
			return *this;
		}

		machine_cycle operator++(int)
		{
			machine_cycle temp = *this;
			++(*this);
			return temp;
		}

		auto operator<=>(const machine_cycle&) const = default;

	private:
		std::uint8_t value_{};
	};

	export class tick_cycle
	{
	public:
		using type_t = std::uint8_t;

	public:
		tick_cycle(type_t value)
			: value_{ value }
		{}

		tick_cycle() = default;

		operator type_t() const { return value(); }
		type_t value() const { return value_; }

		tick_cycle& operator=(const tick_cycle other)
		{
			value_ = other;
			return *this;
		}
		
		tick_cycle& operator++()
		{
			value_++;
			return *this;
		}

		tick_cycle operator++(int)
		{
			tick_cycle temp = *this;
			++(*this);
			return temp;
		}

		auto operator<=>(const tick_cycle&) const = default;

	private:
		std::uint8_t value_{};
	};

	export class cycle
	{
	public:
		cycle(machine_cycle::type_t m_cycle, tick_cycle::type_t t_cycle)
			: m_cycle_{ m_cycle }
			, t_cycle_{ t_cycle }
		{}

		cycle() = default;

		machine_cycle m_cycle() const { return m_cycle_; }
		tick_cycle t_cycle() const { return t_cycle_; }

		bool end_of_m_cycle() const { return t_cycle_.value() == 3; }

		cycle& operator++()
		{
			t_cycle_++;

			if (t_cycle_.value() > 3)
			{
				t_cycle_ = {};
				m_cycle_++;
			}

			return *this;
		}

		cycle operator++(int)
		{
			cycle temp = *this;
			++(*this);
			return temp;
		}
		
		auto operator<=>(const cycle&) const = default;

	private:
		machine_cycle m_cycle_{};
		tick_cycle t_cycle_{};
	};
}

namespace cpu::literals
{
	export machine_cycle operator"" _m_cycle(unsigned long long value)
	{
		return static_cast<std::uint8_t>(value);
	}

	export tick_cycle operator"" _t_cycle(unsigned long long value)
	{
		return static_cast<std::uint8_t>(value);
	}
}
