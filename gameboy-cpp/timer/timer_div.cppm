
export module timer:div;

export import std;

namespace timer
{
	export class div
	{
	public:
		div()
			: counter{}
		{}

		void tick() { counter++; }
		void reset() { counter = 0; }

		operator std::uint16_t() const { return value(); }

		div& operator=(const uint16_t value)
		{
			counter = value;
			return *this;
		}

		std::uint16_t value() const { return counter; }

	private:
		std::uint16_t counter;
	};
}
