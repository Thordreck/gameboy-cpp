export module cpu:halt_context;

namespace cpu
{
	struct halt_context
	{
		bool enabled;
		bool ime_flag_set;
		bool interrupts_pending;
	};
}
