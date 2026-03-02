export module cpu:cycle;

import std;

namespace cpu
{
	export class machine_cycle
	{
	public:
		using type_t = std::uint8_t;

	public:
		constexpr machine_cycle(type_t value)
			: value_{ value }
		{
		}

		constexpr machine_cycle() = default;

		constexpr operator type_t() const { return value(); }
		constexpr type_t value() const { return value_; }

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

		machine_cycle& operator+=(const machine_cycle& other)
		{
			value_ += other;
			return *this;
		}

		machine_cycle& operator-=(const machine_cycle& other)
		{
			value_ -= other;
			return *this;
		}

		constexpr auto operator<=>(const machine_cycle&) const = default;

	private:
		std::uint8_t value_{};
	};

	export class tick_cycle
	{
	public:
		using type_t = std::uint8_t;

	public:
		constexpr tick_cycle(type_t value)
			: value_{ value }
		{
		}

		constexpr tick_cycle() = default;

		constexpr operator type_t() const { return value(); }
		constexpr type_t value() const { return value_; }

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

		constexpr auto operator<=>(const tick_cycle&) const = default;

	private:
		std::uint8_t value_{};
	};

	export class cpu_cycle
	{
	public:
		cpu_cycle(machine_cycle::type_t m_cycle, tick_cycle::type_t t_cycle)
			: m_cycle_{ m_cycle }
			, t_cycle_{ t_cycle }
		{
		}

		cpu_cycle() = default;

		machine_cycle m_cycle() const { return m_cycle_; }
		tick_cycle t_cycle() const { return t_cycle_; }

		cpu_cycle& operator++()
		{
			t_cycle_++;

			if (t_cycle_.value() > 3)
			{
				t_cycle_ = {};
				m_cycle_++;
			}

			return *this;
		}

		cpu_cycle operator++(int)
		{
			cpu_cycle temp = *this;
			++(*this);
			return temp;
		}

		auto operator<=>(const cpu_cycle&) const = default;

	private:
		machine_cycle m_cycle_{};
		tick_cycle t_cycle_{};
	};

	namespace literals
	{
		export constexpr machine_cycle operator"" _m_cycle(unsigned long long value)
		{
			return static_cast<std::uint8_t>(value);
		}

		export constexpr tick_cycle operator"" _t_cycle(unsigned long long value)
		{
			return static_cast<std::uint8_t>(value);
		}
	}

	export template<std::uint8_t Cycle>
		bool is_tick_cycle(const tick_cycle& cycle)
	{
		return cycle.value() == Cycle;
	}

	export bool is_last_tick_cycle(const tick_cycle& cycle)
	{
		return is_tick_cycle<3>(cycle);
	}

	export template<std::uint8_t Cycle>
		bool is_machine_cycle(const machine_cycle& cycle)
	{
		return cycle.value() == Cycle;
	}

	export template<std::uint8_t Cycle>
		bool is_end_of_machine_cycle(const cpu_cycle& cycle)
	{
		return is_machine_cycle<Cycle>(cycle.m_cycle())
			&& is_last_tick_cycle(cycle.t_cycle());
	}

	export bool is_end_of_any_machine_cycle(const cpu_cycle& cycle)
	{
		return is_last_tick_cycle(cycle.t_cycle());
	}

	export machine_cycle operator+(machine_cycle lhs, const machine_cycle& rhs)
	{
		lhs += rhs;
		return lhs;
	}

	export machine_cycle operator-(machine_cycle lhs, const machine_cycle& rhs)
	{
		lhs -= rhs;
		return lhs;
	}
}

