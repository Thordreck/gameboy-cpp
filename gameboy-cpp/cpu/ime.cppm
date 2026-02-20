
export module cpu:ime;

namespace cpu
{
	export class ime
	{
	public:
		ime()
			: enabled{ false }
			, requested{ false }
		{}

		bool is_enabled() const { return enabled; }
		bool is_requested() const { return requested; }

		void request() 
		{ 
			requested = true; 
		}

		void enable()
		{
			enabled = true;
			requested = false;
		}

		void disable()
		{
			enabled = false;
			requested = false;
		}

	private:
		bool enabled;
		bool requested;
	};
}
