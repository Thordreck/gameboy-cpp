
export module emulator.core:joypad;

import std;
import joypad;

namespace emulator
{
    template<joypad::JoypadWritableInput Joypad>
    void set_input_state_imp(void* context, const joypad::const_input_state_view_t state)
    {
        Joypad* joypad = static_cast<Joypad*>(context);
        joypad->set_state(state);
    }

    export using joypad_set_input_state_fn_t = void(*)(void*, joypad::const_input_state_view_t);

    export class joypad_input_sink
    {
    public:
        template<joypad::JoypadWritableInput Imp>
        explicit joypad_input_sink(Imp& imp)
            : imp { &imp }
            , set_input_state_fn { set_input_state_imp<Imp> }
        {}

        template<typename... States>
        requires (std::same_as<States, bool> && ...) && (sizeof...(States) == joypad::num_joypad_inputs)
        void set_input_state(const States... values) const
        {
            set_input_state_fn(imp, std::array { values...} );
        }

    private:
        void* imp;
        joypad_set_input_state_fn_t set_input_state_fn;
    };

}