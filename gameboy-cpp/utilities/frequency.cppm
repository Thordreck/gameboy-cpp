
export module utilities:frequency;

export import std;

namespace utils
{
	export class frequency 
	{
	public:
		constexpr frequency(double hz) 
			: hz_(hz) {}

		constexpr double hertz() const 
		{ 
			return hz_; 
		}

	private:
		double hz_;
	};

	export constexpr frequency operator"" _Hz(unsigned long long int hz)
	{
		return { static_cast<double>(hz) };
	}

	export constexpr frequency operator"" _kHz(unsigned long long int khz)
	{
		return { static_cast<double>(khz * 1000) };
	}

	export template<typename Rep, typename Period>
		constexpr std::chrono::duration<Rep, Period> frequency_to_period(
			const frequency& freq)
	{
		using duration = std::chrono::duration<Rep, Period>;
		return duration(static_cast<Rep>(1.0 / freq.hertz()));
	}
}