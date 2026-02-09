
export module cpu:instructions;

export import std;

import :registers;
import utilities;

namespace cpu 
{
	export class add_a_r8
	{
	public:
		add_a_r8(register_8 a, register_8 r8, flags flags)
			: a { a }
			, r8 { r8 }
			, flags { flags }
		{}

		void execute()
		{
			const bool carry = utils::check_add_overflow<register_8::type_t>(a, r8);
			const bool half_carry = utils::check_half_add_overflow(a, r8);

			a = a + r8;
			flags.z = a == 0;
			flags.n = false;
			flags.h = half_carry;
			flags.c = carry;
		}

	private:
		register_8 a;
		register_8 r8;
		flags flags;
	};
}
