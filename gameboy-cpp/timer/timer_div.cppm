
export module timer:div;

export import std;
export import memory;

namespace timer
{
	export class div
	{
	public:
		div(memory::memory_span_t memory)
			: counter{}
			, memory{ memory }
		{}

		void tick() { counter++; }
		void reset() { counter = 0; }

		operator std::uint16_t() const { return value(); }
		std::uint16_t value() const { return counter; }

		div& operator++()
		{
			counter++;
			return *this;
		}

		div operator++(int)
		{
			div temp = *this;
			++(*this);
			return temp;
		}

	private:
		std::uint16_t counter;
		memory::memory_span_t memory;
	};
}
